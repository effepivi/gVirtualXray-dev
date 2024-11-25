//******************************************************************************
//  Include
//******************************************************************************
#include "gtest/gtest.h"

#ifndef __SimpleGVXR_h
#include "SimpleGVXR.h"
#endif




//******************************************************************************
//  Include
//******************************************************************************
#include <cmath>
#include "gtest/gtest.h"

#ifndef __Logger_h
#include "gVirtualXRay/Logger.h"
#endif

#ifndef __SimpleGVXR_h
#include "SimpleGVXR.h"
#endif


//******************************************************************************
//  Global variables
//******************************************************************************
int g_main_window_width(1280 / 2.0);
int g_main_window_height(800 / 2.0);

double g_incident_energy_in_keV(80.0);
const double g_scaling_factor(2.0);
double g_resolution_in_mm = (0.15 * g_scaling_factor); // 1mm per pixel
int g_number_of_pixels[] = { int(600 / g_scaling_factor), 1 };
double g_source_position_in_cm[] = { 0.0, 0.0, -10.0 };
double g_detector_position_in_cm[] = { 0.0, 0.0,  10.0 };
double g_detector_up_vector[] = { 0.0, 1.0,  0.0 };

double g_literature_result_in_keV(0.0);
double g_expected_result_in_keV(0.0);


//******************************************************************************
//  Function declaration
//******************************************************************************
void loadSTLFile();
void loadDetector();
void loadSource();


//--------------------------------------------------
TEST(TestLoadMeshMemoryLeak, TestLoadMeshMemoryLeak)
//--------------------------------------------------
{
    // Make sure the detector has an odd number of columns
    if ((g_number_of_pixels[0] % 2) == 0)
    {
        g_number_of_pixels[0] = g_number_of_pixels[0] + 1;
    }

    // Make sure the detector has an odd number of rows
    if ((g_number_of_pixels[1] % 2) == 0)
    {
        g_number_of_pixels[1] = g_number_of_pixels[1] + 1;
    }

    // Create an OpenGL context
#ifdef __APPLE__
 // OpenGL 4.2 and above are not supported on MacOS (at least on my compute with MacOS Big Sur)
    createWindow(-1, 0, "OPENGL", 4, 1);
#else // __APPLE__
    createWindow(-1, 0, "OPENGL", 4, 5);
#endif // __APPLE__

    setWindowSize(g_main_window_width, g_main_window_height);

    // Load the data
    loadDetector();
    loadSource();

    // Add the geometry to the X-ray renderer
    for (int i = 0; i < 1000000; ++i)
    {
        removePolygonMeshesFromSceneGraph();
        loadSTLFile();
        addPolygonMeshAsInnerSurface("inside");
        addPolygonMeshAsOuterSurface("outside");
    }
                
    // Enable artefact filtering on GPU
    enableArtefactFilteringOnGPU();

    // Compute the X-ray image
    std::vector<std::vector<float> > x_ray_image = computeXRayImage();

    std::vector<float> row = x_ray_image[x_ray_image.size() / 2];
    float central_pixel = row[row.size() / 2];

    float simulated_result_in_keV = central_pixel / getUnitOfEnergy("keV");

    gVirtualXRay::LOGGER.logNow() << "On GPU, with full precision, the energy (in keV) transmitted "
        "orthogonally throw the middle of cube and "
        "cylinder actually is:" << std::endl;

    gVirtualXRay::LOGGER.logNow() << "\t\t           = " << simulated_result_in_keV << " keV" <<
        std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The absolute error is: "
        "|I_expected - I_GPU_full_precision| keV" << std::endl;

    gVirtualXRay::LOGGER.logNow() << "                     : |" <<
        g_expected_result_in_keV << " - " << simulated_result_in_keV <<
        "| keV" << std::endl;

    gVirtualXRay::LOGGER.logNow() << "                     : " <<
        std::fixed << std::abs(g_expected_result_in_keV -
            simulated_result_in_keV) << " keV" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The relative error is: |I_expected - I_GPU_half_precision| "
        "/ I_expected" << std::endl;

    gVirtualXRay::LOGGER.logNow() << "                     : " <<
        std::fixed << 100.0 * std::abs(g_expected_result_in_keV -
            simulated_result_in_keV) / g_expected_result_in_keV <<
        " \%" << std::endl;

    destroyAllWindows();

    ASSERT_NEAR(1, simulated_result_in_keV / g_expected_result_in_keV, 0.01);
    ASSERT_FLOAT_EQ(g_expected_result_in_keV, simulated_result_in_keV);
}


//----------------
void loadSTLFile()
//----------------
{
    // Set the geometry
    makeCube("outside",
        3.0,
        "cm",
        "root");

    makeCylinder("inside",
        20,
        3.0,
        1.0,
        "cm",
        "root");
}


//-----------------
void loadDetector()
//-----------------
{
    setDetectorPixelSize(g_resolution_in_mm, g_resolution_in_mm, "mm");

    setDetectorNumberOfPixels(g_number_of_pixels[0], g_number_of_pixels[1]);

    setDetectorPosition(
        g_detector_position_in_cm[0],
        g_detector_position_in_cm[1],
        g_detector_position_in_cm[2],
        "cm");

    setDetectorUpVector(
        g_detector_up_vector[0],
        g_detector_up_vector[1],
        g_detector_up_vector[2]);
}


//---------------
void loadSource()
//---------------
{
    // Set the energy
    setMonoChromatic(g_incident_energy_in_keV, "keV", 1);

    // Set the source position
    usePointSource();
    setSourcePosition(g_source_position_in_cm[0],
        g_source_position_in_cm[1],
        g_source_position_in_cm[2],
        "cm");

    // Use a parallel source
    useParallelBeam();
}
