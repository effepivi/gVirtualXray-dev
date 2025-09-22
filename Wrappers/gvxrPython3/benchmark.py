#!/usr/bin/env python3


# Import packages
from gvxrPython3 import gvxr
import os
import SimpleITK as sitk
import numpy as np
import matplotlib.pyplot as plt # Plotting
from jedi import create_environment
from matplotlib.colors import LogNorm # Look up table
from matplotlib.colors import PowerNorm # Look up table

import time


def getMatrixLabel(i:int)->str:
    return "matrix-" + str(i) + "x" + str(i) + "x" + str(i)

def getFoamLabel(i:int)->str:
    return "foam-" + str(i) + "x" + str(i) + "x" + str(i)

def getParticleLabel(i:int)->str:
    return "particles-" + str(i) + "x" + str(i) + "x" + str(i)


def getBestSOD(label:str, unit:str="mm", multiplier:float=1.25)->float:
    detector_size = gvxr.getDetectorSize(unit)

    min_x, min_y, min_z, max_x, max_y, max_z = gvxr.getNodeAndChildrenBoundingBox(label, unit)

    sample_size_x = max_x - min_x
    sample_size_y = max_y - min_y
    sample_size_z = max_z - min_z

    source_position = np.asarray(gvxr.getSourcePosition(unit))
    detector_position = np.asarray(gvxr.getDetectorPosition(unit))

    sdd = np.linalg.norm(source_position - detector_position)

    horizontal_ratio = sdd / detector_size[0]
    vertical_ratio = sdd / detector_size[1]

    diagonal_xy = np.linalg.norm(np.array([max_x, max_y, 0]) - np.array([min_x, min_y, 0]))

    horizontal_sod = diagonal_xy * horizontal_ratio
    vertical_sod = sample_size_z * vertical_ratio

    return multiplier * max(horizontal_sod, vertical_sod)

def applySOD(label:str, sod:float, unit:str):
    sample_centre = np.asarray(gvxr.getNodeAndChildrenBoundingBoxCentre(label, unit))
    source_position = np.asarray(gvxr.getSourcePosition(unit))
    detector_position = np.asarray(gvxr.getDetectorPosition(unit))


    view_vector = detector_position - source_position
    view_vector /= np.linalg.norm(view_vector)
    translation_vector = source_position + view_vector * sod

    gvxr.setNodeTransformationMatrix(label, [[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]])
    gvxr.translateNode(label, *translation_vector, unit)

def setDetector():
    gvxr.setDetectorPosition(10.0, 0.0, 0.0, "cm");
    gvxr.setDetectorUpVector(0, 0, -1);
    gvxr.setDetectorPixelSize(0.5 / 4, 0.5 / 4, "mm");
    gvxr.setDetectorNumberOfPixels(640 * 4, 320 * 4);

def setSource():
    gvxr.setSourcePosition(-40.0,  0.0, 0.0, "cm");
    gvxr.usePointSource();
    gvxr.setVoltage(80, "kV")
    gvxr.addFilter("Cu", 1, "mm")
    gvxr.setmAs(0.5)
    energy_bins = gvxr.getEnergyBins("keV")
    photon_counts = gvxr.getPhotonCountsPerCm2At1m()

    gvxr.resetBeamSpectrum()

    for energy, count in zip(energy_bins, photon_counts):
        gvxr.addEnergyBinToSpectrumPerCm2At1m(energy, "keV", count)

    gvxr.disablePoissonNoise()

def plotSpectrum():
    energy_bins = gvxr.getEnergyBins("keV")
    photon_counts = gvxr.getPhotonCountsPerCm2At1m()
    plt.figure()
    plt.plot(energy_bins, photon_counts)
    plt.xlabel("Energy in keV")
    plt.ylabel("Number of photons per cm^2 at 1m")

def plotXRayImage(image):
    # Display the X-ray image and compare three different lookup tables
    plt.figure(figsize=(17, 7.5))

    plt.suptitle("Image simulated with gVirtualXray visualised", y=0.75)

    plt.subplot(131)
    plt.imshow(image, cmap="gray")
    plt.colorbar(orientation='horizontal')
    plt.title("using a linear colour scale")

    plt.subplot(132)
    plt.imshow(image, norm=LogNorm(), cmap="gray")
    plt.colorbar(orientation='horizontal')
    plt.title("using a logarithmic colour scale")

    plt.subplot(133)
    plt.imshow(image, norm=PowerNorm(gamma=1./2.), cmap="gray")
    plt.colorbar(orientation='horizontal');
    plt.title("using a Power-law colour scale ($\gamma=0.5$)")

    plt.tight_layout()

def createSTL():
    # Create the samples
    for i in [15, 32, 64, 128]:
        print("Create the models for %ix%ix%i voxels" % (i, i, i))
        particle_label = getParticleLabel(i)
        matrix_label = getMatrixLabel(i)
        foam_label = getFoamLabel(i)

        voxel_size = 5 / i
        gvxr.makeFoamInCuboid(particle_label,
                              "",
                              5, 5, 5, # 5mm in total
                              0.25,
                              50,
                              50,
                              voxel_size, # voxel size:
                              "mm",
                              1)

        # Create SITK image stack from the array output by foam function
        sitk_image = sitk.GetImageFromArray(gvxr.getImplicitSurfaceLastVoxelisation(particle_label))
        sitk_image.SetSpacing(3*[voxel_size])
        sitk.WriteImage(sitk_image, os.path.join(output_path, particle_label + ".mha"))

        # Use implicit surface to create foam mesh
        gvxr.emptyMesh(foam_label)
        gvxr.createMeshFromImplicitSurface(particle_label, particle_label, 0.05, foam_label, 1)
        gvxr.createMeshFromImplicitSurface(particle_label, matrix_label, -10.0, 0.05, foam_label, 1)

        gvxr.saveSTLfile(particle_label, os.path.join(output_path, particle_label + ".stl"))
        gvxr.saveSTLfile(matrix_label, os.path.join(output_path, matrix_label + ".stl"))

def createEnvironment(number_of_bits_per_float:int):


    # Create a FP16 simulation engine
    gvxr.createWindow(0,
                      True,
                      "OPENGL",
                      4, 6, number_of_bits_per_float)

    setDetector()
    setSource()
    plotSpectrum()

    foam_label = getFoamLabel(128)
    matrix_label = getMatrixLabel(128)
    particle_label = getParticleLabel(128)

    gvxr.removePolygonMeshesFromSceneGraph()
    gvxr.emptyMesh(foam_label)

    gvxr.loadMeshFile(particle_label,
                      os.path.join(output_path, particle_label + ".stl"),
                      "mm",
                      True,
                      foam_label)

    gvxr.loadMeshFile(matrix_label,
                      os.path.join(output_path, matrix_label + ".stl"),
                      "mm",
                      True,
                      foam_label)

    # Set the material composition

    # AlSi10Mg alloy
    gvxr.setColor(matrix_label, 136 / 255,139 / 255,141 / 255, 0.5)
    gvxr.setMixture(matrix_label, ["Al", "Si", "Mg"], [100 - 9.0 - 0.25, 9.0, 0.25])
    gvxr.setDensity(matrix_label, 2.67, "g/cm3")

    # Silicon carbide
    gvxr.setColor(particle_label, 136 / 255,139 / 255,141 / 255, 0.5)
    gvxr.setCompound(particle_label, "SiC")
    gvxr.setDensity(particle_label, 3.21, "g/cm3")

    best_sod = getBestSOD(foam_label, "mm")
    applySOD(foam_label, best_sod, "mm")




# Create the output directory if needed
output_path = "benchmark-results"
if not os.path.exists(output_path):
    os.mkdir(output_path)

gvxr.setSeed(1234567890)
createSTL()


number_of_bits_per_float = 0
createEnvironment(number_of_bits_per_float)
x_ray_image_fp_mixed = np.asarray(gvxr.computeXRayImage()).astype(np.single)
print(x_ray_image_fp_mixed.shape, x_ray_image_fp_mixed.min(), x_ray_image_fp_mixed.max())

total_runs = 50
start_fp_mixed = time.time()
for i in range(total_runs):
    gvxr.computeXRayImage(x_ray_image_fp_mixed)
end_fp_mixed = time.time()

plotXRayImage(x_ray_image_fp_mixed)
gvxr.saveLastXRayImage(os.path.join(output_path, "x_ray_image_fp_mixed.tif"))
gvxr.destroy()

number_of_bits_per_float = 16
createEnvironment(number_of_bits_per_float)
x_ray_image_fp16 = np.asarray(gvxr.computeXRayImage()).astype(np.single)
print(x_ray_image_fp16.shape, x_ray_image_fp16.min(), x_ray_image_fp16.max())

total_runs = 50
start_fp16 = time.time()
for i in range(total_runs):
    gvxr.computeXRayImage(x_ray_image_fp16)
end_fp16 = time.time()

plotXRayImage(x_ray_image_fp16)
gvxr.saveLastXRayImage(os.path.join(output_path, "x_ray_image_fp16.tif"))
gvxr.destroy()

number_of_bits_per_float = 32
createEnvironment(number_of_bits_per_float)
x_ray_image_fp32 = np.asarray(gvxr.computeXRayImage()).astype(np.single)
print(x_ray_image_fp32.shape, x_ray_image_fp32.min(), x_ray_image_fp32.max())

start_fp32 = time.time()
for i in range(total_runs):
    gvxr.computeXRayImage(x_ray_image_fp32)
end_fp32 = time.time()

print("FP_mixed avg run time:", (end_fp_mixed - start_fp_mixed) / total_runs, "seconds")
print("FP16 avg run time:", (end_fp16 - start_fp16) / total_runs, "seconds")
print("FP32 avg run time:", (end_fp32 - start_fp32) / total_runs, "seconds")

print("FP_mixed:", total_runs / (end_fp_mixed - start_fp_mixed), "FPS")
print("FP16:", total_runs / (end_fp16 - start_fp16), "FPS")
print("FP32:", total_runs / (end_fp32 - start_fp32), "FPS")


plotXRayImage(x_ray_image_fp32)
gvxr.saveLastXRayImage(os.path.join(output_path, "x_ray_image_fp32.tif"))
gvxr.destroy()

plt.show()

exit()

