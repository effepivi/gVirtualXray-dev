//******************************************************************************
//  Include
//******************************************************************************
#include <string>
//#include <ranges> // C++ 23
#include <vector>
#include <iostream>
#include <chrono>
#include <cmath>

#ifdef HAS_FILESYSTEM_HEADER
#include <filesystem>
#else
#include <experimental/filesystem>
#endif

#ifndef __SimpleGVXR_h
#include "SimpleGVXR.h"
#endif


//******************************************************************************
//  Namespaces
//******************************************************************************
#ifdef HAS_FILESYSTEM_HEADER
namespace fs = std::filesystem;
#else
namespace fs = std::experimental::filesystem;
#endif

using namespace std::chrono;


//******************************************************************************
//  Function declarations
//******************************************************************************
std::string getMatrixLabel(unsigned int i);
std::string getFoamLabel(unsigned int i);
std::string getParticleLabel(unsigned int i);
void setDetector();
void setSource();
void createSTL();
void createEnvironment(float aScalingFactor,
    unsigned int aNumberOfBitsPerFloat,
    unsigned int aMeshResolution);


//******************************************************************************
//  Global variables
//******************************************************************************
fs::path g_output_path("benchmark-results");

std::vector<unsigned int> g_volume_resolution_set({15, 32, 64, 128});
std::vector<float> g_detector_resolution_scaling_factor_set({1, 2, 3, 4, 5});

// std::vector<unsigned int> g_volume_resolution_set({15, 32});
// std::vector<float> g_detector_resolution_scaling_factor_set({1, 2, 3});

unsigned int g_total_runs = 50;


//******************************************************************************
//  Function definitions
//******************************************************************************


//-----------------------------------------------
inline std::string getMatrixLabel(unsigned int i)
//-----------------------------------------------
{
    return std::string("matrix-") + std::to_string(i) + "x" + std::to_string(i) + "x" + std::to_string(i);
}


//---------------------------------------------
inline std::string getFoamLabel(unsigned int i)
//---------------------------------------------
{
    return std::string("foam-") + std::to_string(i) + "x" + std::to_string(i) + "x" + std::to_string(i);
}

//-------------------------------------------------
inline std::string getParticleLabel(unsigned int i)
//-------------------------------------------------
{
    return std::string("particle-") + std::to_string(i) + "x" + std::to_string(i) + "x" + std::to_string(i);
}


//-------------------------------------------
inline void setDetector(float aScalingFactor)
//-------------------------------------------
{
    setDetectorPosition(10.0, 0.0, 0.0, "cm");
    setDetectorUpVector(0, 0, -1);
    setDetectorPixelSize(0.5 / aScalingFactor, 0.5 / aScalingFactor, "mm");
    setDetectorNumberOfPixels(lround(640 * aScalingFactor), lround(320 * aScalingFactor));
}


//---------------------
inline void setSource()
//---------------------
{
    setSourcePosition(-40.0,  0.0, 0.0, "cm");
    usePointSource();
    setVoltage(80, "kV");
    addFilter("Cu", 1, "mm");
    setmAs(0.5);
    std::vector<double> energy_bins = getEnergyBins("keV");
    std::vector<double> photon_counts = getPhotonCountsPerCm2At1m();

    resetBeamSpectrum();

    for (auto energy_ite = energy_bins.begin(), count_ite = photon_counts.begin();
         (energy_ite != energy_bins.end()) && (count_ite != photon_counts.end());
         ++energy_ite, ++count_ite)
    {
        addEnergyBinToSpectrumPerCm2At1m(*energy_ite, "keV", *count_ite);
    }

    // C++ 23
    //for(auto [energy, count]: std::views::zip(energy_bins, photon_counts))
    //{
    //    addEnergyBinToSpectrumPerCm2At1m(energy, "keV", count);
    //}

    enablePoissonNoise();
}


//--------------
void createSTL()
//--------------
{
    for (std::vector<unsigned int>::const_iterator ite = g_volume_resolution_set.begin();
        ite != g_volume_resolution_set.end();
        ++ite)
    {
        std::cout << "Create the models for " << *ite << "x" << *ite << "x" << *ite << std::endl;
        std::string particle_label = getParticleLabel(*ite);
        std::string matrix_label = getMatrixLabel(*ite);
        std::string foam_label = getFoamLabel(*ite);

        float voxel_size = 5.0 / *ite;
        makeFoamInCuboid(particle_label,
                             "",
                             5, 5, 5, // 5mm in total
                             0.25,
                             50,
                             50,
                             voxel_size, // voxel size:
                             "mm",
                             1);

       // Create SITK image stack from the array output by foam function
       //sitk_image = sitk.GetImageFromArray(getImplicitSurfaceLastVoxelisation(particle_label))
       //sitk_image.SetSpacing(3*[voxel_size])
       //sitk.WriteImage(sitk_image, os.path.join(output_path, particle_label + ".mha"))

       // Use implicit surface to create foam mesh
       emptyMesh(foam_label);
       createMeshFromImplicitSurface(particle_label, particle_label, 0.05, foam_label, 1);
       createMeshFromImplicitSurface(particle_label, matrix_label, -10.0, 0.05, foam_label, 1);

       saveSTLfile(particle_label, g_output_path / fs::path(particle_label + ".stl"));
       saveSTLfile(matrix_label, g_output_path / fs::path(matrix_label + ".stl"));
    }
}


//--------------------------------------------------------
void createEnvironment(float aScalingFactor,
                       unsigned int aNumberOfBitsPerFloat,
                       unsigned int aMeshResolution)
//--------------------------------------------------------
{
    // Create a simulation engine
    createWindow(0,
        true,
        "OPENGL",
        4, 6,
        aNumberOfBitsPerFloat);

    setDetector(aScalingFactor);
    setSource();

    std::string foam_label = getFoamLabel(aMeshResolution);
    std::string matrix_label = getMatrixLabel(aMeshResolution);
    std::string particle_label = getParticleLabel(aMeshResolution);

    removePolygonMeshesFromSceneGraph();
    emptyMesh(foam_label);

    loadMeshFile(particle_label,
        g_output_path / fs::path(particle_label + ".stl"),
        "mm",
        true,
        foam_label);

    loadMeshFile(matrix_label,
        g_output_path / fs::path(matrix_label + ".stl"),
        "mm",
        true,
        foam_label);

    // Set the material composition

    // AlSi10Mg alloy
    setColor(matrix_label, 136 / 255,139 / 255,141 / 255, 0.5);
    setMixture(matrix_label,
        std::vector<std::string>({"Al", "Si", "Mg"}),
        std::vector<double>({100 - 9.0 - 0.25, 9.0, 0.25}));
    setDensity(matrix_label, 2.67, "g/cm3");

    // Silicon carbide
    setColor(particle_label, 136 / 255,139 / 255,141 / 255, 0.5);
    setCompound(particle_label, "SiC");
    setDensity(particle_label, 3.21, "g/cm3");

    float best_sod = getBestSOD(foam_label, "mm");
    applySOD(foam_label, best_sod, "mm");
}


//--------
int main()
//--------
{
    try {
        // Check if g_output_path directory exists
        if (!fs::is_directory(g_output_path) || !fs::exists(g_output_path))
        {
            // Create the output directory if needed
            fs::create_directory(g_output_path);
        }

        // We set the random seed so that the foam is always the same
        setSeed(1234567890);
        createSTL();

        std::vector<float> p_result;

        for (std::vector<unsigned int>::const_iterator volume_resolution_ite = g_volume_resolution_set.begin();
            volume_resolution_ite != g_volume_resolution_set.end();
            ++volume_resolution_ite)
        {
            for (std::vector<float>::const_iterator detector_resolution_scaling_factor_ite = g_detector_resolution_scaling_factor_set.begin();
                detector_resolution_scaling_factor_ite != g_detector_resolution_scaling_factor_set.end();
                ++detector_resolution_scaling_factor_ite)
            {
                // Full-precision floating-point numbers
                unsigned int number_of_bits_per_float = 32;
                createEnvironment(*detector_resolution_scaling_factor_ite,
                    number_of_bits_per_float,
                    *volume_resolution_ite);

                unsigned int cols = getDetectorNumberOfPixels()[0];
                unsigned int rows = getDetectorNumberOfPixels()[1];

                float* p_x_ray_image_fp32 = new float[cols * rows];
                computeXRayImage(p_x_ray_image_fp32, rows, cols);
                saveLastXRayImage(g_output_path / fs::path("x_ray_image_fp32.mha"));

                auto start_fp32 = high_resolution_clock::now();
                for (unsigned int i = 0; i < g_total_runs; ++i)
                    computeXRayImage(p_x_ray_image_fp32, rows, cols);
                auto end_fp32 = high_resolution_clock::now();

                destroy();

                // Half-precision floating-point numbers
                number_of_bits_per_float = 16;
                createEnvironment(*detector_resolution_scaling_factor_ite,
                    number_of_bits_per_float,
                    *volume_resolution_ite);
                float* p_x_ray_image_fp16 = new float[cols * rows];
                computeXRayImage(p_x_ray_image_fp16, rows, cols);
                saveLastXRayImage(g_output_path / fs::path("x_ray_image_fp16.mha"));

                auto start_fp16 = high_resolution_clock::now();
                for (unsigned int i = 0; i < g_total_runs; ++i)
                    computeXRayImage(p_x_ray_image_fp16, rows, cols);
                auto end_fp16 = high_resolution_clock::now();

                destroy();

                // Mixed-precision floating-point numbers
                number_of_bits_per_float = 0;
                createEnvironment(*detector_resolution_scaling_factor_ite,
                    number_of_bits_per_float,
                    *volume_resolution_ite);
                float* p_x_ray_image_fp_mixed = new float[cols * rows];
                computeXRayImage(p_x_ray_image_fp_mixed, rows, cols);
                saveLastXRayImage(g_output_path / fs::path("x_ray_image_fp_mixed.mha"));

                auto start_fp_mixed = high_resolution_clock::now();
                for (unsigned int i = 0; i < g_total_runs; ++i)
                    computeXRayImage(p_x_ray_image_fp_mixed, rows, cols);
                auto end_fp_mixed = high_resolution_clock::now();

                std::string matrix_label = getMatrixLabel(*volume_resolution_ite);
                std::string particle_label = getParticleLabel(*volume_resolution_ite);

                unsigned int number_of_triangles = getNumberOfPrimitives(matrix_label) + getNumberOfPrimitives(particle_label);
                unsigned int number_of_vertices = getVertexNumber(matrix_label) + getVertexNumber(particle_label);

                destroy();



                float duration_fp32 =  duration_cast<milliseconds>(end_fp32 - start_fp32).count();
                float duration_fp16 = duration_cast<milliseconds>(end_fp16 - start_fp16).count();
                float duration_fp_mixed = duration_cast<milliseconds>(end_fp_mixed - start_fp_mixed).count();

                std::cout << "FP32 avg run time: " << duration_fp32 / g_total_runs << " seconds" << std::endl;
                std::cout << "FP_mixed avg run time: " << duration_fp_mixed / g_total_runs << " seconds" << std::endl;
                std::cout << "FP16 avg run time: " << duration_fp16 / g_total_runs << " seconds" << std::endl;

                std::cout << "FP32: " << g_total_runs / (1e-3 * duration_fp32) << " FPS" << std::endl;
                std::cout << "FP16: " << g_total_runs / (1e-3 * duration_fp16) << " FPS" << std::endl;
                std::cout << "FP_mixed: " << g_total_runs / (1e-3 * duration_fp_mixed) << " FPS" << std::endl;

                delete [] p_x_ray_image_fp_mixed;
                delete [] p_x_ray_image_fp16;
                delete [] p_x_ray_image_fp32;


                p_result.push_back(32);
                p_result.push_back(cols);
                p_result.push_back(rows);
                p_result.push_back(number_of_triangles);
                p_result.push_back(number_of_vertices);
                p_result.push_back(duration_fp32 / g_total_runs);
                p_result.push_back(g_total_runs / (1e-3 * duration_fp32));
                p_result.push_back(duration_fp32 / duration_fp32);

                p_result.push_back(16);
                p_result.push_back(cols);
                p_result.push_back(rows);
                p_result.push_back(number_of_triangles);
                p_result.push_back(number_of_vertices);
                p_result.push_back(duration_fp16 / g_total_runs);
                p_result.push_back(g_total_runs / (1e-3 * duration_fp16));
                p_result.push_back(duration_fp32 / duration_fp16);

                p_result.push_back(1);
                p_result.push_back(cols);
                p_result.push_back(rows);
                p_result.push_back(number_of_triangles);
                p_result.push_back(number_of_vertices);
                p_result.push_back(duration_fp_mixed / g_total_runs);
                p_result.push_back(g_total_runs / (1e-3 * duration_fp_mixed));
                p_result.push_back(duration_fp32 / duration_fp_mixed);
            }
        }

        unsigned int number_of_cols = p_result.size() / (g_volume_resolution_set.size() * g_detector_resolution_scaling_factor_set.size()) / 3;
        unsigned int number_of_rows = p_result.size() / number_of_cols;

        std::cout << "Precision\tCols\tRows\tTriangles\tVertices\tAvg (millisec)\tFPS\tSpeedup" << std::endl;

        for (unsigned int i = 0; i < number_of_rows; ++i)
        {
            for (unsigned int j = 0; j < number_of_cols; ++j)
            {
                float value = p_result[i * number_of_cols + j];

                if (j == 0 && value < 15)
                {
                    std::cout << "mixed" << "\t";
                }
                else
                {
                    if (j == number_of_cols - 2)
                        std::cout << std::round(p_result[i * number_of_cols + j]) << "\t";
                    else
                        std::cout << p_result[i * number_of_cols + j] << "\t";
                }
            }
            std::cout << std::endl;
        }

        // Interactive visualisation
        // The user can rotate the 3D scene and zoom-in and -out in the visualisation window.

        // - Keys are:
        //     - Q/Escape: to quit the event loop (does not close the window)
        //     - B: display/hide the X-ray beam
        //     - W: display the polygon meshes in solid or wireframe
        //     - N: display the X-ray image in negative or positive
        //     - H: display/hide the X-ray detector
        // - Mouse interactions:
        //     - Zoom in/out: mouse wheel
        //     - Rotation: Right mouse button down + move cursor```
        //renderLoop();

        return 0;
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
