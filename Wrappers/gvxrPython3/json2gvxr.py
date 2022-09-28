import os
from pathlib import Path
import matplotlib.colors as mcolors
import sys
import numpy as np
from PIL import Image

try:
    import spekpy as sp
    has_speckpy = True
except:
    print("Speckpy is not install, you won't be able to load a beam spectrum using Speckpy")
    has_speckpy = False

import json # Load the JSON file
from gvxrPython3 import gvxr # Simulate X-ray images

# Define the NoneType
NoneType = type(None);
params  = None;
context_created = False
tomo_params = {}
tomo_setup = False


# Print the libraries' version
print (gvxr.getVersionOfSimpleGVXR())
print (gvxr.getVersionOfCoreGVXR())


def initGVXR(fname, renderer="OPENGL", major=3, minor=2):
    """
    Create a simulation environment from a JSON file.

    :param str fname: The file name of the JSON file
    :param str renderer: The renderer (e.g. OPENGL, or EGL)
    :param int major: The major version of the OpenGL context to create
    :param int minor: The minor version of the OpenGL context to create
    """

    global params, context_created

    # Load the JSON file
    with open(fname) as f:
        params = json.load(f)

    # Create an OpenGL context
    window_size = params["WindowSize"]
    print("Create an OpenGL context:",
        str(window_size[0]) + "x" + str(window_size[1])
    )

    if not context_created:
        if renderer.upper() == "OPENGL":
            visibility = True
        else:
            visibility = False

        gvxr.createWindow(-1,
            True,
            renderer,
            major,
            minor)

    gvxr.setWindowSize(
        window_size[0],
        window_size[1]
    )

    context_created = True


def initSourceGeometry(fname=""):
    """
    Initialise the X-ray source from a JSON file.

    :param str fname: The file name of the JSON file
    """

    global params;
    # Load the JSON file
    if fname != "":
        with open(fname) as f:
            params = json.load(f)
    # Set up the beam
    print("Set up the beam")
    source_position = params["Source"]["Position"];
    print("\tSource position:", source_position)
    gvxr.setSourcePosition(
        source_position[0],
        source_position[1],
        source_position[2],
        source_position[3]
    );
    source_shape = params["Source"]["Shape"]
    print("\tSource shape:", source_shape);
    if source_shape.upper() == "PARALLELBEAM" or source_shape.upper() == "PARALLEL":
        gvxr.useParallelBeam();
    elif source_shape.upper() == "POINTSOURCE" or source_shape.upper() == "POINT" or source_shape.upper() == "CONE" or source_shape.upper() == "CONEBEAM":
        gvxr.usePointSource();
    else:
        raise "Unknown source shape:" + source_shape;


def initSpectrum(fname="", verbose=0):
    """
    Initialise the X-ray spectrum from a JSON file.

    :param str fname: The file name of the JSON file
    :param int verbose: The level of verbosity
    :return: spectrum, unit (e.g. "keV", "MeV"), k, f
    """

    global params, has_speckpy

    min_energy = sys.float_info.max
    max_energy = -sys.float_info.max

    gvxr.resetBeamSpectrum()
    spectrum = {};

    # Load the JSON file
    if fname != "":
        with open(fname) as f:
            params = json.load(f)

    if type(params["Source"]["Beam"]) == list:
        k = []
        f = []

        for energy_channel in params["Source"]["Beam"]:
            energy = energy_channel["Energy"];
            unit = energy_channel["Unit"];
            count = energy_channel["PhotonCount"];
            spectrum[energy] = count;

            if verbose > 0:
                if count == 1:
                    print("\t", str(count), "photon of", energy, unit);
                else:
                    print("\t", str(count), "photons of", energy, unit);

            gvxr.addEnergyBinToSpectrum(energy, unit, count);
            k.append(energy)
            f.append(count)

        k = np.array(k)
        f = np.array(f)

    else:
        if "GateMacro" in params["Source"]["Beam"]:
            k = []
            f = []
            unit = params["Source"]["Beam"]["Unit"]

            # Read the file
            gate_macro_file = open(params["Source"]["Beam"]["GateMacro"], 'r')
            lines = gate_macro_file.readlines()

            # Process every line
            for line in lines:
                # Check if this is a comment or not
                comment = True
                index_first_non_space_character = len(line) - len(line.lstrip())

                if index_first_non_space_character >= 0 and index_first_non_space_character < len(line):
                    if line[index_first_non_space_character] != '#':
                        comment = False

                # This is not a comment
                if not comment:
                    x = line.split()

                energy = float(x[1])
                count = float(x[2])
                spectrum[energy] = count

                if verbose > 0:
                    if count == 1:
                        print("\t", str(count), "photon of", energy, unit);
                    else:
                        print("\t", str(count), "photons of", energy, unit);

                gvxr.addEnergyBinToSpectrum(energy, unit, count);
                k.append(energy)
                f.append(count)

            k = np.array(k)
            f = np.array(f)

        elif "TextFile" in params["Source"]["Beam"]:
            k = []
            f = []
            unit = params["Source"]["Beam"]["Unit"]

            # Read the file
            gate_macro_file = open(params["Source"]["Beam"]["TextFile"], 'r')
            lines = gate_macro_file.readlines()

            # Process every line
            for line in lines:

                # Check if this is a comment or not
                comment = True
                index_first_non_space_character = len(line) - len(line.lstrip())
                if index_first_non_space_character >= 0 and index_first_non_space_character < len(line):
                    if line[index_first_non_space_character] != '#':
                        comment = False

                # This is not a comment
                if not comment:
                    x = line.split()

                energy = float(x[0])
                count = float(x[1])
                spectrum[energy] = count

                if verbose > 0:
                    if count == 1:
                        print("\t", str(count), "photon of", energy, unit);
                    else:
                        print("\t", str(count), "photons of", energy, unit);

                gvxr.addEnergyBinToSpectrum(energy, unit, count);
                k.append(energy)
                f.append(count)

            k = np.array(k)
            f = np.array(f)

        elif "kvp" in params["Source"]["Beam"]:

            # Speckpy is not installed, raise an ImportError
            import spekpy as sp

            kvp_in_kV = params["Source"]["Beam"]["kvp"];
            th_in_deg = 12

            if "tube angle" in params["Source"]["Beam"]:
                th_in_deg = params["Source"]["Beam"]["tube angle"];

            if verbose > 0:
                print("kVp (kV):", kvp_in_kV)
                print("tube angle (degrees):", th_in_deg)

            s = sp.Spek(kvp=kvp_in_kV, th=th_in_deg) # Generate a spectrum (80 kV, 12 degree tube angle)

            if "filter" in params["Source"]["Beam"]:
                print('params["Source"]["Beam"]', params["Source"]["Beam"])

                for beam_filter in params["Source"]["Beam"]["filter"]:
                    print(beam_filter)

                    filter_material = beam_filter[0]
                    filter_thickness_in_mm = beam_filter[1]

                    if verbose > 0:
                        print("Filter", filter_thickness_in_mm, "mm of", filter_material)

                    s.filter(filter_material, filter_thickness_in_mm)

            unit = "keV"
            k, f = s.get_spectrum(edges=True) # Get the spectrum

            for energy, count in zip(k, f):
                count = round(count)

                if count > 0:
                    max_energy = max(max_energy, energy)
                    min_energy = min(min_energy, energy)

                    if energy in spectrum.keys():
                        spectrum[energy] += count
                    else:
                        spectrum[energy] = count

        else:
            raise IOError("Invalid beam spectrum in JSON file")

        if verbose > 0:
            print("/gate/source/mybeam/gps/emin", min_energy, "keV")
            print("/gate/source/mybeam/gps/emax", max_energy, "keV")

        for energy in spectrum.keys():
            count = spectrum[energy]
            gvxr.addEnergyBinToSpectrum(energy, unit, count);

            if verbose > 0:
                print("/gate/source/mybeam/gps/histpoint", energy / 1000, count)

    return spectrum, unit, k, f;


def initDetector(fname = ""):
    """
    Initialise the X-ray detector from a JSON file.

    :param str fname: The file name of the JSON file
    """

    global params;

    # Load the JSON file
    if fname != "":
        with open(fname) as f:
            params = json.load(f);

    assert params is not None
    # Set up the detector
    print("Set up the detector");

    detector_position = params["Detector"]["Position"];
    gvxr.setDetectorPosition(
        detector_position[0],
        detector_position[1],
        detector_position[2],
        detector_position[3]
    );
    print("\tDetector position:", detector_position)

    detector_up = params["Detector"]["UpVector"];
    gvxr.setDetectorUpVector(
        detector_up[0],
        detector_up[1],
        detector_up[2]
    );
    print("\tDetector up vector:", detector_up)

    detector_number_of_pixels = params["Detector"]["NumberOfPixels"];
    gvxr.setDetectorNumberOfPixels(
        detector_number_of_pixels[0],
        detector_number_of_pixels[1]
    );
    print("\tDetector number of pixels:", detector_number_of_pixels)

    if "Oversampling" in params["Detector"].keys():
        gvxr.useLBufferOversamplingFactor(params["Detector"]["Oversampling"])

    if "Spacing" in params["Detector"].keys() == list and "Size" in params["Detector"].keys():
        raise "Cannot use both 'Spacing' and 'Size' for the detector";

    if "Spacing" in params["Detector"].keys():
        pixel_spacing = params["Detector"]["Spacing"];
    elif "Size" in params["Detector"].keys():
        detector_size = params["Detector"]["Size"];
        pixel_spacing = [];
        pixel_spacing.append(detector_size[0] / detector_number_of_pixels[0]);
        pixel_spacing.append(detector_size[1] / detector_number_of_pixels[1]);
        pixel_spacing.append(detector_size[2]);

    if "Energy response" in params["Detector"].keys():
        print("\tEnergy response:", params["Detector"]["Energy response"]["File"], "in", params["Detector"]["Energy response"]["Energy"])
        gvxr.clearDetectorEnergyResponse()
        gvxr.loadDetectorEnergyResponse(params["Detector"]["Energy response"]["File"],
                                        params["Detector"]["Energy response"]["Energy"])

    gvxr.setDetectorPixelSize(
        pixel_spacing[0],
        pixel_spacing[1],
        pixel_spacing[2]
    );
    print("\tPixel spacing:", pixel_spacing)

    if "LSF" in params["Detector"].keys():
        print("LSF:", params["Detector"]["LSF"])
        if isinstance(params["Detector"]["LSF"], str) or isinstance(params["Detector"]["LSF"], list):
            gvxr.setLSF(params["Detector"]["LSF"])
        else:
            raise "LSF must be a filepath or an array."

def initSamples(fname = "", verbose = 0):
    """
    Initialise the scanned objects from a JSON file.

    :param str fname: The file name of the JSON file
    :param int verbose: The level of verbosity
    """

    global params;

    move_to_centre = False

    gvxr.removePolygonMeshesFromXRayRenderer()
    gvxr.removePolygonMeshesFromSceneGraph()

    # Load the JSON file
    if fname != "":
        with open(fname) as f:
            params = json.load(f);

    # Load the data
    if verbose > 0:
        print("Load the 3D data\n");

    colours = list(mcolors.TABLEAU_COLORS);
    colour_id = 0;

    mesh_source = None
    mesh_unit = None

    if "SceneGraph" in params:
        mesh_source = params["SceneGraph"]["Samples"]
        gvxr.loadSceneGraph(params["SceneGraph"]["Path"], params["SceneGraph"]["Unit"])

        if verbose > 0:
            print("Load the 3D objects from a scenegraph (" + params["SceneGraph"]["Path"] + ")")

            for i, mesh in enumerate(mesh_source):
                print(i, mesh["Label"])

    if "Samples" in params:
        mesh_source = params["Samples"]

    for mesh in mesh_source:
        if "Cube" in mesh:
            if verbose == 1:
                print(mesh["Label"] + " is a cube")

            gvxr.makeCube(mesh["Label"], mesh["Cube"][0], mesh["Cube"][1]);

        elif "Cylinder" in mesh:
            if verbose == 1:
                print(mesh["Label"] + " is a cylinder")

            gvxr.makeCylinder(mesh["Label"], mesh["Cylinder"][0], mesh["Cylinder"][1], mesh["Cylinder"][2], mesh["Cylinder"][3]);

        elif "Path" in mesh and "Samples" in params:
            if verbose > 0:
                print("\tLoad", mesh["Label"], "in", mesh["Path"], "using", mesh["Unit"]);

            gvxr.loadMeshFile(
                mesh["Label"],
                mesh["Path"],
                mesh["Unit"],
                False
            );

        elif type(mesh) == str:
            if mesh == "MoveToCenter" or mesh == "MoveToCentre":
                move_to_centre = True

        elif "SceneGraph" not in params:
            raise IOError("Cannot find the geometry of Mesh " + mesh["Label"])

        if type(mesh) != str:
            material = mesh["Material"];

            if material[0].upper() == "ELEMENT":
                gvxr.setElement(
                    mesh["Label"],
                    material[1]
                );

            elif material[0].upper() == "MIXTURE":
                if type(material[1]) == str:
                    gvxr.setMixture(
                        mesh["Label"],
                        material[1]
                    );

                else:
                    elements = [];
                    weights = [];

                    if verbose == 2:
                        print(mesh["Label"] + ":",
                              "d="+str(mesh["Density"]), "g/cm3 ;",
                              "n=" + str(len(material[1][0::2])),
                              "; state=solid");

                    for Z, weight in zip(material[1][0::2], material[1][1::2]):
                        elements.append(Z);
                        weights.append(weight);

                        if verbose == 2:
                            print("        +el: name="+gvxr.getElementName(Z) + " ; f=" +str(weight) )

                    if verbose == 2:
                        print()

                    gvxr.setMixture(
                        mesh["Label"],
                        elements,
                        weights
                    );

            elif material[0].upper() == "COMPOUND":
                gvxr.setCompound(
                    mesh["Label"],
                    material[1]
                );

            elif material[0].upper() == "HU":
                gvxr.setHounsfieldValue(
                    mesh["Label"],
                    material[1]
                );

            elif material[0].upper() == "MU":
                gvxr.setLinearAttenuationCoefficient(
                    mesh["Label"],
                    material[1],
                    "cm-1"
                );

            else:
                raise IOError("Unknown material type: " + material[0]);

            if "Density" in mesh.keys():
                gvxr.setDensity(
                    mesh["Label"],
                    mesh["Density"],
                    "g/cm3"
                );

            if "Transform" in mesh.keys():
                for transform in mesh["Transform"]:
                    if transform[0] == "Rotation":
                        if len(transform) == 5:
                            gvxr.rotateNode(mesh["Label"],
                                            transform[1],
                                            transform[2],
                                            transform[3],
                                            transform[4])

                        else:
                            raise IOError("Invalid rotation:", transform)

                    elif transform[0] == "Translation":
                        if len(transform) == 5:
                            gvxr.translateNode(mesh["Label"],
                                            transform[1],
                                            transform[2],
                                            transform[3],
                                            transform[4])

                        else:
                            raise IOError("Invalid translation:", transform)

                    elif transform[0] == "Scaling":
                        if len(transform) == 4:
                            gvxr.scaleNode(mesh["Label"],
                                            transform[1],
                                            transform[2],
                                            transform[3])

                        else:
                            raise IOError("Invalid scaling:", transform)

                    else:
                        raise IOError("Invalid transformation:", transform)

                gvxr.applyCurrentLocalTransformation(mesh["Label"])

            # Add the mesh to the simulation
            if "Type" in mesh.keys():
                if mesh["Type"] == "inner":
                    gvxr.addPolygonMeshAsInnerSurface(mesh["Label"]);

                elif mesh["Type"] == "outer":
                    # gvxr.addPolygonMeshAsInnerSurface(mesh["Label"]);
                    gvxr.addPolygonMeshAsOuterSurface(mesh["Label"]);

            else:
                gvxr.addPolygonMeshAsInnerSurface(mesh["Label"]);

            # Change the colour
            colour = mcolors.to_rgb(colours[colour_id]);

            # Get the opacity
            opacity = 1.0

            if "Opacity" in mesh.keys():
                opacity = mesh["Opacity"]

            gvxr.setColour(mesh["Label"], colour[0], colour[1], colour[2], opacity);
            colour_id += 1;

            if colour_id == len(colours):
                colour_id = 0;

    # Move the samples on the centre
    if move_to_centre:
        gvxr.moveToCentre()


def initScan(fname = ""):
    global params;
    global tomo_setup
    global tomo_params

    # Load the JSON file
    if fname != "":
        with open(fname) as f:
            params = json.load(f)

    assert params is not None
    print("Set up the CT Scan")

    if "OutFolder" in params["Scan"]: # Optional
        tomo_params["OutFolder"] = params["Scan"]["OutFolder"]
    else:
        tomo_params["OutFolder"] = "./output/"

    if "GifPath" in params["Scan"]: # Optional
        tomo_params["GifPath"] = params["Scan"]["GifPath"]

    if "NumberOfProjections" in params["Scan"]:
        tomo_params["NumberOfProjections"] = int(params["Scan"]["NumberOfProjections"])
    else:
        raise "NumberOfProjections must exist in Scan key."

    # If both keys exist, FinalAngle will be ignored.
    if "AngleStep" in params["Scan"]:
        tomo_params["AngleStep"] = float(params["Scan"]["AngleStep"])
    elif "FinalAngle" in params["Scan"]:
        tomo_params["AngleStep"] = float(params["Scan"]["FinalAngle"] / tomo_params["NumberOfProjections"])
    else:
        raise "AngleStep or FinalAngle must exist in Scan key."

    if "CenterOfRotation" in params["Scan"]:
        tomo_params["RotCentre"] = params["Scan"]["CenterOfRotation"]
    elif "CentreOfRotation" in params["Scan"]:
        tomo_params["RotCentre"] = params["Scan"]["CenterOfRotation"]
    else:
        tomo_params["RotCentre"] = [0, 0, 0, "mm"]
    tomo_setup = True

def doCTScan():
    global tomo_setup
    global tomo_params

    # Ensure tomo is setup first
    if not tomo_setup:
        raise "initScan() must be called before doCTScan("

    save_gif = "GifPath" in tomo_params
    out_folder = Path(tomo_params["OutFolder"])

    print("Performing CT Scan")
    print("\tRotation Centre:",tomo_params["RotCentre"])
    print("\tNumber of Projections:",tomo_params["NumberOfProjections"])
    print("\tAngle Step:",tomo_params["AngleStep"])
    print("\tOutput folder:", tomo_params["OutFolder"])

    projections = np.asarray(gvxr.computeProjectionSet(tomo_params["RotCentre"][0],
        tomo_params["RotCentre"][1],
        tomo_params["RotCentre"][2],
        tomo_params["RotCentre"][3],
        tomo_params["NumberOfProjections"],
        tomo_params["AngleStep"]))

    os.makedirs(tomo_params["OutFolder"], exist_ok=True)

    print("CT Complete, writing to files")
    # Save projections
    for i in range(0, projections.shape[0]):
        # Looks like convention is for 4 digits 0-padding, with inital projection starting at 1
        Image.fromarray(projections[i]).save(out_folder/f"projection-{i+1:04}.tiff")
    print(f"\tProjections saved to '{tomo_params['OutFolder']}projections-xxxx.tiff'")

    if save_gif:
        # normalize to uint8
        uint_projections = ((projections - projections.min()) / (projections.max() - projections.min()) * 255).astype("uint8")

        # Annoyingly have to convert each individual frame into a PIL Image
        ims = [Image.fromarray(proj) for proj in uint_projections]

        # Create a gif image
        ims[0].save(tomo_params["GifPath"], "gif", append_images=ims[1:], duration=50, save_all=True, loop=0)
        print("\tAnimation Saved to ", tomo_params["GifPath"])

    print("Simulated",tomo_params["NumberOfProjections"], "projections.")
