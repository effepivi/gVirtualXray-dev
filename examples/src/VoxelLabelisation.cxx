/**
********************************************************************************
*
*   @file       VoxelLabelisation.cxx
*
*   @brief      Program to automatically label voxels of a CT scan.
*
*   @version    1.0
*
*   @date       16/06/2014
*
*   @author     Dr Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifdef HAS_GLEW
#include <GL/glew.h>
#endif

#ifdef HAS_OPENGL2
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#endif
#endif

#ifdef HAS_OPENGL3
#ifdef __APPLE__
#define GL3_PROTOTYPES 1
#else
#define GL_GLEXT_PROTOTYPES 1
#endif

#define GLFW_INCLUDE_GLCOREARB 1
#endif

#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <exception>
#include <sstream>
#include <cstdlib>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImageDuplicator.h>
#include <itkGradientAnisotropicDiffusionImageFilter.h>
#include <itkCastImageFilter.h>
#include <itkResampleImageFilter.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkStatisticsImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkBinaryMask3DMeshSource.h>
#include <itkMesh.h>
#include <itkQuadEdgeMesh.h>
#include <itkQuadEdgeMeshDecimationCriteria.h>
//#include <itkQuadEdgeMeshSquaredEdgeLengthDecimation.h>
#include <itkNormalizedCorrelationImageToImageMetric.h>
#include <itkMeanSquaresImageToImageMetric.h>
#include <itkIdentityTransform.h>
#include <itkConstantPadImageFilter.h>
#include <itkCorrelationImageToImageMetricv4.h>
#include <itkImageRegistrationMethod.h>
#include <itkTranslationTransform.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkRegularStepGradientDescentOptimizer.h>

#ifdef HAS_OCTAVE
#include <octave/config.h>
#include <octave/octave.h>
#include <octave/oct.h>
#include <octave/parse.h>
#include <octave/toplev.h>
#include <octave/str-vec.h>
#endif

#ifdef HAS_GSL
#include<gsl/gsl_multimin.h>
#endif

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#ifndef GVXR_XRAY_BEAM_H
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef GVXR_POLYGON_MESH_H
#include "gVirtualXRay/PolygonMesh.h"
#endif

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#ifdef HAS_OPENGL2
#include "display_gl2.frag.h"
#include "display_gl2.vert.h"
#endif

#ifdef HAS_OPENGL3
#include "display_gl3.frag.h"
#include "display_gl3.vert.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Types
//******************************************************************************
typedef itk::Image<float, 3> RealImageType;
typedef itk::ImageFileReader<RealImageType> ImageFileReaderType;
typedef itk::ImageFileWriter<RealImageType> ImageFileWriterType;
typedef itk::StatisticsImageFilter<RealImageType> StatisticsImageFilterType;


//******************************************************************************
//  Constant variables
//******************************************************************************
const GLfloat g_rotation_speed(2.0);


//******************************************************************************
//  Global variables
//******************************************************************************

// Keep track of the window width
GLsizei g_window_width(640);

// Keep track of the window height
GLsizei g_window_height(480);

// Shader program used to display the 3D scene
Shader g_display_shader;

// Transformation matrices
MATRIX4 g_scene_rotation_matrix;

// Control the zoom
RATIONAL_NUMBER g_zoom(100.0 * cm);

// Use the arc ball rotation
bool g_use_arc_ball(false);

// States of the mouse
int g_button(-1); // Button
int g_button_state(-1); // Button state
GLint g_last_x_position(0); // Previous x position of the cursor
GLint g_last_y_position(0); // Previous y position of the cursor
GLint g_current_x_position(0); // Current x position of the cursor
GLint g_current_y_position(0); // Current x position of the cursor

std::string g_input_file_name;
std::string g_output_file_name;
std::vector<double> g_p_energy_set;
std::vector<double> g_p_angle_set;
std::vector<RealImageType::Pointer> g_p_high_resolution_mu_volume_set;
std::vector<RealImageType::Pointer> g_p_low_resolution_mu_volume_set;
std::vector<RealImageType::Pointer> g_p_high_resolution_sinogram_set;
std::vector<RealImageType::Pointer> g_p_low_resolution_sinogram_set;
std::vector<RealImageType::Pointer> g_p_high_resolution_reference_projection_set;
std::vector<RealImageType::Pointer> g_p_low_resolution_reference_projection_set;
std::vector<RealImageType::Pointer> g_p_high_resolution_simulated_projection_set;
std::vector<RealImageType::Pointer> g_p_low_resolution_simulated_projection_set;
std::vector<short> g_p_threshold_set;
int g_default_number_of_threshold(2);
std::vector<std::vector<float> > g_p_vertex_set;
std::vector<std::vector<unsigned int> > g_p_index_set;
std::vector<PolygonMesh> g_p_polygon_mesh_set;

ImageFileReaderType::Pointer g_p_reader;
RealImageType::Pointer g_p_high_res_volume(RealImageType::New());
RealImageType::Pointer g_p_low_res_volume;
StatisticsImageFilterType::Pointer g_p_ct_statistics;

bool g_use_filter(false);
bool g_use_low_res(true);
bool g_use_high_res(false);

GLFWwindow* g_p_window_id(0);

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;

Vec2ui g_number_of_pixels(64, 64);
VEC2 g_detector_size(320.0 * mm, 320.0 * mm);
VEC2 g_resolution(g_detector_size.getX() / g_number_of_pixels.getX(),
        g_detector_size.getY() / g_number_of_pixels.getY());

VEC3 g_source_position(   0.0, 0.0, -40.0 * cm);
VEC3 g_detector_position( 0.0, 0.0,  10.0 * cm);
VEC3 g_detector_up_vector(0.0, 0.0, 1.0);

bool g_display_detector(true);
bool g_display_source(true);
bool g_display_beam(true);

std::vector<VEC3> g_p_polygon_mesh_colour_set;
VEC3 g_centre;

bool g_is_xray_image_up_to_date(true);

bool g_restart(false);
int g_max_iteration(100);
short g_min_value(0);
short g_max_value(0);

std::string g_optimizer("random");
std::vector<bool> g_p_mesh_visibility_flag_set;
int g_pop_size(50);

std::vector<std::vector<short> > g_p_parent_population;
std::vector<std::vector<short> > g_p_child_population;

int g_number_of_polygon_meshes(0);
double g_immigration_probability(0.2);
double g_mutation_probability(0.5);
double g_crossover_probability(0.3);


//******************************************************************************
//  Function declaration
//******************************************************************************
void initGLFW();
void initGL();
void initShader();

void displayCallback();
void idleCallback();
void quitCallback();
void framebufferSizeCallback(GLFWwindow* apWindow, int aWidth, int aHeight);
void keyCallback(GLFWwindow* apWindow, int aKey, int aScanCode, int anAction, int aModifierKey);
void mouseButtonCallback(GLFWwindow* apWindow, int aButton, int aButtonState, int aModifierKey);
void cursorPosCallback(GLFWwindow* apWindow, double x, double y);
void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset);
void errorCallback(int error, const char* description);
VEC3 getArcballVector(int x, int y);
float radian2degree(float anAngle);
void computeRotation(MATRIX4& aRotationMatrix);

void loadParameters(int argc, char** argv);
void loadVolumeDataset(const std::string& aFileName);
void loadSinograms();
void volume2sinogram(const RealImageType::Pointer& aVolume,
        std::vector<RealImageType::Pointer>& aSinogramSet,
        double anEnergy);
void loadProjections();
void loadPoygonMeshes(const RealImageType::Pointer& aVolume, bool aSaveSegmentationFlag);

void convertHU2mu(const RealImageType::Pointer& aVolume,
        RealImageType::Pointer& aMuVolume,
		double anEnergy);

void sinogram2projection(const RealImageType::Pointer& aSinogram,
        RealImageType::Pointer& aReferenceProjection,
        RealImageType::Pointer& aSimulatedProjection);
void quitCallback();
void errorCallback(int error, const char* description);
void extractIsosurface(const RealImageType::Pointer& aVolume,
        short aMinHUValue,
        short aMaxHUValue,
        short& anAverageHounsfielValue,
        bool aSaveSegmentationFlag,
        int temp);
void setColours();
double computeMetrics(const std::vector<RealImageType::Pointer>& aReferenceProjectionSet,
        const std::vector<RealImageType::Pointer>& aSimulatedProjectionSet);
int optimise(const RealImageType::Pointer& aVolume,
        const std::vector<RealImageType::Pointer>& aReferenceProjectionSet,
        std::vector<RealImageType::Pointer>& aSimulatedProjectionSet);
void Normalise(RealImageType::Pointer& aVolume);


//---------------------------------------------
void Normalise(RealImageType::Pointer& aVolume)
//---------------------------------------------
{
    typedef itk::StatisticsImageFilter< RealImageType > StatisticsFilterType;
    StatisticsFilterType::Pointer p_statistics(StatisticsFilterType::New());
    p_statistics->SetInput(aVolume);
    p_statistics->Update();

    // Convert voxel values from HU to attenuation coefficients
    typedef itk::ImageRegionIterator<RealImageType> ImageIterator;
    RealImageType::RegionType region(aVolume->GetLargestPossibleRegion());
    ImageIterator image_ite(aVolume, region);

    for (image_ite.GoToBegin(); !image_ite.IsAtEnd(); ++image_ite)
    {
        image_ite.Set((image_ite.Get() - p_statistics->GetMean()) / p_statistics->GetSigma());
    }
}


//-------------------------------------------------------------
void loadDetector(const RealImageType::Pointer& aProjectionSet,
                  const RealImageType::Pointer& aCTVolume)
//-------------------------------------------------------------
{
    RealImageType::SizeType projection_size(aProjectionSet->GetBufferedRegion().GetSize());
    RealImageType::SizeType ct_size(aCTVolume->GetBufferedRegion().GetSize());
    RealImageType::SpacingType projection_spacing(aProjectionSet->GetSpacing());

    g_number_of_pixels.setX(projection_size[0]);
    g_number_of_pixels.setY(projection_size[1]);

    g_detector_size.setX(projection_spacing[0] * projection_size[0]);
    g_detector_size.setY(projection_spacing[1] * projection_size[1]);

    g_resolution.setX(g_detector_size.getX() / g_number_of_pixels.getX());
    g_resolution.setY(g_detector_size.getY() / g_number_of_pixels.getY());

    RealImageType::IndexType index1;
    index1[0]= 0;
    index1[1]= 0;
    index1[2]= 0;

    RealImageType::IndexType index2;
    index2[0]= 0;
    index2[1]= 0;
    index2[2]= 1;

    itk::Point<double, 3> point1;
    itk::Point<double, 3> point2;

    aCTVolume->TransformIndexToPhysicalPoint(index1, point1);
    aCTVolume->TransformIndexToPhysicalPoint(index2, point2);

    g_detector_up_vector.setX(point2[0] - point1[0]);
    g_detector_up_vector.setY(point2[1] - point1[1]);
    g_detector_up_vector.setZ(point2[2] - point1[2]);
    g_detector_up_vector.normalise();


    RealImageType::IndexType index_corner_1;
    index_corner_1[0]= 0;
    index_corner_1[1]= 0;
    index_corner_1[2]= 0;

    RealImageType::IndexType index_corner_2;
    index_corner_2[0]= ct_size[0] - 1;
    index_corner_2[1]= 0;
    index_corner_2[2]= 0;

    RealImageType::IndexType index_corner_3;
    index_corner_3[0]= 0;
    index_corner_3[1]= 0;
    index_corner_3[2]= ct_size[2] - 1;

    RealImageType::IndexType index_corner_4;
    index_corner_4[0]= ct_size[0] - 1;
    index_corner_4[1]= 0;
    index_corner_4[2]= ct_size[2] - 1;

    RealImageType::IndexType index_corner_5;
    index_corner_5[0]= 0;
    index_corner_5[1]= ct_size[1] - 1;
    index_corner_5[2]= 0;

    RealImageType::IndexType index_corner_6;
    index_corner_6[0]= ct_size[0] - 1;
    index_corner_6[1]= ct_size[1] - 1;
    index_corner_6[2]= 0;

    RealImageType::IndexType index_corner_7;
    index_corner_7[0]= 0;
    index_corner_7[1]= ct_size[1] - 1;
    index_corner_7[2]= ct_size[2] - 1;

    RealImageType::IndexType index_corner_8;
    index_corner_8[0]= ct_size[0] - 1;
    index_corner_8[1]= ct_size[1] - 1;
    index_corner_8[2]= ct_size[2] - 1;

    itk::Point<double, 3> corner_1;
    itk::Point<double, 3> corner_2;
    itk::Point<double, 3> corner_3;
    itk::Point<double, 3> corner_4;
    itk::Point<double, 3> corner_5;
    itk::Point<double, 3> corner_6;
    itk::Point<double, 3> corner_7;
    itk::Point<double, 3> corner_8;

    aCTVolume->TransformIndexToPhysicalPoint(index_corner_1, corner_1);
    aCTVolume->TransformIndexToPhysicalPoint(index_corner_2, corner_2);
    aCTVolume->TransformIndexToPhysicalPoint(index_corner_3, corner_3);
    aCTVolume->TransformIndexToPhysicalPoint(index_corner_4, corner_4);
    aCTVolume->TransformIndexToPhysicalPoint(index_corner_5, corner_5);
    aCTVolume->TransformIndexToPhysicalPoint(index_corner_6, corner_6);
    aCTVolume->TransformIndexToPhysicalPoint(index_corner_7, corner_7);
    aCTVolume->TransformIndexToPhysicalPoint(index_corner_8, corner_8);

    g_xray_detector.setResolutionInUnitsOfLengthPerPixel(g_resolution);
    g_xray_detector.setNumberOfPixels(g_number_of_pixels);
    g_xray_detector.setUpVector(g_detector_up_vector);

    g_detector_position.setX(mm * (corner_1[0] + corner_2[0] + corner_3[0] + corner_4[0]) / 4.0);
    g_detector_position.setY(mm * (corner_1[1] + corner_2[1] + corner_3[1] + corner_4[1]) / 4.0);
    g_detector_position.setZ(mm * (corner_1[2] + corner_2[2] + corner_3[2] + corner_4[2]) / 4.0);

    g_source_position.setX(mm * (corner_5[0] + corner_6[0] + corner_7[0] + corner_8[0]) / 4.0);
    g_source_position.setY(mm * (corner_5[1] + corner_6[1] + corner_7[1] + corner_8[1]) / 4.0);
    g_source_position.setZ(mm * (corner_5[2] + corner_6[2] + corner_7[2] + corner_8[2]) / 4.0);

    g_centre = (g_detector_position + g_source_position) / 2.0;

    // Set up the modelling-viewing matrix (g_current_modelview_matrix)
    loadLookAtModelViewMatrix(g_centre.getX(), g_centre.getY(), g_centre.getZ() + g_zoom,
            g_centre.getX(), g_centre.getY(), g_centre.getZ(),
            0.0, 1.0, 0.0);

    g_source_position   += (g_source_position - g_centre).normal() * 10.0 * cm;
    g_detector_position -= (g_source_position - g_centre).normal() * 10.0 * cm;
    g_xray_detector.setDetectorPosition(g_detector_position);

    // Set the source position
    g_xray_detector.setXrayPointSource(g_source_position);
    g_xray_detector.setParallelBeam();

#ifdef HAS_OPENGL2
    g_xray_renderer.initialise(XRayRenderer::OPENGL,
            GL_RGB32F_ARB,
            &g_xray_detector,
            &g_xray_beam);
#endif

#ifdef HAS_OPENGL3
    g_xray_renderer.initialise(XRayRenderer::OPENGL,
            GL_RGB32F,
            &g_xray_detector,
            &g_xray_beam);
#endif

    g_is_xray_image_up_to_date = false;
}


//------------------------------
void loadSource(double anEnergy)
//------------------------------
{
    // Set the energy
    g_xray_beam.initialise(anEnergy);

    g_is_xray_image_up_to_date = false;
}


//--------------------------------------------------------------------------------
void updateXRayImage(std::vector<RealImageType::Pointer>& aSimulatedProjectionSet)
//--------------------------------------------------------------------------------
{
    // The X-ray image is not up-to-date
    if (!g_is_xray_image_up_to_date)
    {
        std::string file_name_prefix;
        if (g_output_file_name.size())
        {
            file_name_prefix = g_output_file_name + "/";
        }

        // Process all the energy
        int energy(0);
        for (std::vector<double>::const_iterator ite_energy(g_p_energy_set.begin());
                ite_energy != g_p_energy_set.end();
                ++ite_energy, ++energy)
        {
            loadSource(*ite_energy);

            // Process all the angles;
            int angle(0);
            for (std::vector<double>::const_iterator ite_angle(g_p_angle_set.begin());
                    ite_angle != g_p_angle_set.end();
                    ++ite_angle, ++angle)
            {
                // Rotate the object
                MATRIX4 modelling_matrix(MATRIX4::buildIdentityMatrix());
                modelling_matrix *= MATRIX4::buildTranslationMatrix(g_centre);
                modelling_matrix *= MATRIX4::buildRotationMatrix(g_p_angle_set[g_p_angle_set.size() - 1 - angle], g_detector_up_vector);
                modelling_matrix *= MATRIX4::buildTranslationMatrix(-g_centre);

                // Compute the X-Ray image
                g_xray_renderer.computeImage(modelling_matrix);

                float* p_simulated_image;
                float* p_energy_fluence(g_xray_renderer.getEnergyFluence());
                p_simulated_image = aSimulatedProjectionSet[energy]->GetBufferPointer() + angle * g_number_of_pixels.getX() * g_number_of_pixels.getY();

                for (int i(0); i < g_number_of_pixels.getX() * g_number_of_pixels.getY(); ++i)
                {
                    p_simulated_image[i] = -log((p_energy_fluence[i] * MeV) / *ite_energy);
                }
            }

            Normalise(aSimulatedProjectionSet[energy]);
        }

        // The X-ray image is up-to-date
        g_is_xray_image_up_to_date = true;
    }
}


//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
    // initialize random seed
     srand (time(0));

    // Set a nice rotation first
    g_scene_rotation_matrix.rotate( 60.0, VEC3(0.0, 1.0, 0.0));
    g_scene_rotation_matrix.rotate(-30.0, VEC3(1.0, 0.0, 0.0));
    g_scene_rotation_matrix.rotate(-45.0, VEC3(0.0, 1.0, 0.0));
    g_scene_rotation_matrix.rotate(-60.0, VEC3(1.0, 0.0, 0.0));
    g_scene_rotation_matrix.rotate( 90.0, VEC3(0.0, 0.0, 1.0));

    // Return code
    int return_code(EXIT_SUCCESS);

    // Register the exit callback
    atexit(quitCallback);

    try
    {
        // Initialise GLFW
        initGLFW();

        // Initialise GLEW
        if (glewInit() != GLEW_OK)
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: cannot initialise GLEW.");
        }
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Initialise OpenGL
        initGL();

        // Initialise the shader from files compressed using the Z library
        initShader();

        // Process the command line arguments
        loadParameters(argc, argv);

        // Load the input data
        loadVolumeDataset(g_input_file_name);

        // Load sinograms
        loadSinograms();

        // Load projections
        loadProjections();

        setColours();

        // Set the projection matrix
        framebufferSizeCallback(g_p_window_id, g_window_width, g_window_height);

        if (g_use_low_res)
        {
            optimise(g_p_low_res_volume, g_p_low_resolution_reference_projection_set, g_p_low_resolution_simulated_projection_set);

            if (g_restart)
            {
                optimise(g_p_low_res_volume, g_p_low_resolution_reference_projection_set, g_p_low_resolution_simulated_projection_set);
            }
        }

        if (g_use_high_res)
        {
            g_p_low_resolution_mu_volume_set.clear();
            g_p_low_resolution_sinogram_set.clear();
            g_p_low_resolution_reference_projection_set.clear();
            g_p_low_resolution_simulated_projection_set.clear();
        }

        if (g_use_high_res)
        {
            optimise(g_p_high_res_volume, g_p_high_resolution_reference_projection_set, g_p_high_resolution_simulated_projection_set);

            if (g_restart)
            {
                optimise(g_p_high_res_volume, g_p_high_resolution_reference_projection_set, g_p_high_resolution_simulated_projection_set);
            }
        }

        // Save the iso-surfaces
        if (g_use_high_res)
        {
            loadPoygonMeshes(g_p_high_res_volume, true);
        }
        else
        {
            loadPoygonMeshes(g_p_low_res_volume, true);
        }

        g_p_low_res_volume = 0;
        g_p_high_res_volume = 0;

        // Launch the event loop
        while (!glfwWindowShouldClose(g_p_window_id))
        {
            // Render here
            displayCallback();

            // Swap front and back buffers
            glfwSwapBuffers(g_p_window_id);

            // Poll for and process events
            glfwPollEvents();

            // Idle callback
            idleCallback();
        }
    }
    // Catch exception if any
    catch (const std::exception& error)
    {
        std::cerr << error.what() << std::endl;
        return_code = EXIT_FAILURE;
    }

    // Close the window and shut GLFW if needed
    quitCallback();

    // Clean Octave
    clean_up_and_exit(0);

    // Return an exit code
    return (return_code);
}


//-------------
void initGLFW()
//-------------
{
    // Set an error callback
    glfwSetErrorCallback(errorCallback);

    // Initialize GLFW
    if (!glfwInit())
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "ERROR: cannot initialise GLFW.");
    }

    // Enable OpenGL 2 if possible
#ifdef HAS_OPENGL2
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

    // Enable OpenGL 3.2 if possible
#ifdef HAS_OPENGL3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    // Enable anti-aliasing
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create a windowed mode window and its OpenGL context
    g_p_window_id = glfwCreateWindow(g_window_width, g_window_height,
            "gVirtualXRay -- Voxel labelisation", NULL, NULL);

    // The window has not been created
    if (!g_p_window_id)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "ERROR: cannot create a GLFW windowed mode window and its OpenGL context.");
    }

    // Make the window's context current
    glfwMakeContextCurrent(g_p_window_id);

    // Register GLFW callbacks
    glfwSetFramebufferSizeCallback(g_p_window_id, framebufferSizeCallback);
    glfwSetKeyCallback(g_p_window_id, keyCallback);
    glfwSetMouseButtonCallback(g_p_window_id, mouseButtonCallback);
    glfwSetCursorPosCallback(g_p_window_id, cursorPosCallback);
    glfwSetScrollCallback(g_p_window_id, scrollCallback);
}


//---------------
void initShader()
//---------------
{
    // Initialise the shaders
    char* p_vertex_shader(0);
    char* p_fragment_shader(0);

    int z_lib_return_code_vertex(0);
    int z_lib_return_code_fragment(0);

    std::string vertex_shader;
    std::string fragment_shader;

	// Display shader
#ifdef HAS_OPENGL2
	z_lib_return_code_vertex   = inflate(g_display_gl2_vert, sizeof(g_display_gl2_vert),   &p_vertex_shader);
    z_lib_return_code_fragment = inflate(g_display_gl2_frag, sizeof(g_display_gl2_frag), &p_fragment_shader);
#endif

#ifdef HAS_OPENGL3
    z_lib_return_code_vertex   = inflate(g_display_gl3_vert, sizeof(g_display_gl3_vert),   &p_vertex_shader);
    z_lib_return_code_fragment = inflate(g_display_gl3_frag, sizeof(g_display_gl3_frag), &p_fragment_shader);
#endif

    vertex_shader   = p_vertex_shader;
    fragment_shader = p_fragment_shader;
    delete [] p_vertex_shader;     p_vertex_shader = 0;
    delete [] p_fragment_shader; p_fragment_shader = 0;

    if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
    }
    g_display_shader.setLabels("display.vert", "display.frag");
    g_display_shader.loadSource(vertex_shader, fragment_shader);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Enable the shader
    g_display_shader.enable();
    GLint shader_id(g_display_shader.getProgramHandle());

    GLfloat light_global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    // Handle for shader variables
    GLuint handle(0);

    handle = glGetUniformLocation(shader_id, "light_global_ambient");
    glUniform4fv(handle, 1, &light_global_ambient[0]);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    handle = glGetUniformLocation(shader_id, "light_ambient");
    glUniform4fv(handle, 1, &light_ambient[0]);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    handle = glGetUniformLocation(shader_id, "light_diffuse");
    glUniform4fv(handle, 1, &light_diffuse[0]);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    handle = glGetUniformLocation(shader_id, "light_specular");
    glUniform4fv(handle, 1, &light_specular[0]);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    VEC3 light_position(0, 0, g_zoom);
    handle = glGetUniformLocation(shader_id, "light_position");
    glUniform4f(handle, light_position.getX(), light_position.getY(), light_position.getZ(), 1.0);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Disable the shader
    g_display_shader.disable();
}


//-----------
void initGL()
//-----------
{
    // Enable the Z-buffer
    glEnable(GL_DEPTH_TEST);

    // Set the background colour
    glClearColor(0.5, 0.5, 0.5, 1.0);

    // Check if any OpenGL error has occurred.
    // If any has, an exception is thrown
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//--------------------
void displayCallback()
//--------------------
{
    // Enable back face culling
    pushEnableDisableState(GL_CULL_FACE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Add the current shader to the shader stack
    pushShaderProgram();

    // Enable the shader
    g_display_shader.enable();
    GLint shader_id(g_display_shader.getProgramHandle());

    // Check the status of OpenGL and of the current FBO
    checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // A handle for shader resources
    GLuint handle(0);

    // Upload the projection matrix
    handle = glGetUniformLocation(shader_id, "g_projection_matrix");
    glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_projection_matrix.get());
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Store the current transformation matrices
    pushModelViewMatrix();
    pushProjectionMatrix();

    // Rotate the 3D scene
    g_current_modelview_matrix *= MATRIX4::buildTranslationMatrix(g_centre);
    g_current_modelview_matrix *= g_scene_rotation_matrix;
    g_current_modelview_matrix *= MATRIX4::buildTranslationMatrix(-g_centre);

    // Upload the modelview matrix
    handle = glGetUniformLocation(shader_id, "g_modelview_matrix");
    glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_modelview_matrix.get());
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    handle = glGetUniformLocation(shader_id,"g_use_lighting");
    glUniform1i(handle, 1);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Display the 1st object
    int i(0);
    for (std::vector<PolygonMesh>::iterator ite(g_p_polygon_mesh_set.begin() + i);
            ite != g_p_polygon_mesh_set.end();
            ++ite, ++i)
    {
        if (g_p_mesh_visibility_flag_set[i])
        {
            try
            {
                // Set the colour of the sample
                VEC3 colour(g_p_polygon_mesh_colour_set[i] * 0.19225);
                const GLfloat material_ambient[]  = {colour.getX(), colour.getY(), colour.getZ(), 1.0};

                colour = g_p_polygon_mesh_colour_set[i] * 0.50754;
                const GLfloat material_diffuse[]  = {colour.getX(), colour.getY(), colour.getZ(), 1.0};

                colour = g_p_polygon_mesh_colour_set[i] * 0.508273;
                const GLfloat material_specular[]  = {colour.getX(), colour.getY(), colour.getZ(), 1.0};

                const GLfloat material_shininess = 50.2;

                handle = glGetUniformLocation(shader_id, "material_ambient");
                glUniform4fv(handle, 1, &material_ambient[0]);
                checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

                handle = glGetUniformLocation(shader_id, "material_diffuse");
                glUniform4fv(handle, 1, &material_diffuse[0]);
                checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

                handle = glGetUniformLocation(shader_id, "material_specular");
                glUniform4fv(handle, 1, &material_specular[0]);
                checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

                handle = glGetUniformLocation(shader_id, "material_shininess");
                glUniform1fv(handle, 1, &material_shininess);
                checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

                ite->display();
            }
            catch (const std::exception& e)
            {
                // Do nothing
                //std::cerr << "Exception caught:" << e.what() << std::endl;
            }
        }
    }

    // Display the source
    if (g_display_source)
    {
        // Set the colour of the sample
        VEC3 colour(VEC3(0.0, 0.0, 1.0) * 0.19225);
        const GLfloat material_ambient[]  = {colour.getX(), colour.getY(), colour.getZ(), 1.0};

        colour = VEC3(0.0, 0.0, 1.0) * 0.50754;
        const GLfloat material_diffuse[]  = {colour.getX(), colour.getY(), colour.getZ(), 1.0};

        colour = VEC3(0.0, 0.0, 1.0) * 0.508273;
        const GLfloat material_specular[]  = {colour.getX(), colour.getY(), colour.getZ(), 1.0};

        const GLfloat material_shininess = 75.2;

        handle = glGetUniformLocation(shader_id, "material_ambient");
        glUniform4fv(handle, 1, &material_ambient[0]);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "material_diffuse");
        glUniform4fv(handle, 1, &material_diffuse[0]);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "material_specular");
        glUniform4fv(handle, 1, &material_specular[0]);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "material_shininess");
        glUniform1fv(handle, 1, &material_shininess);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        g_xray_detector.displaySource();
    }

    // Disable back face culling
    glDisable(GL_CULL_FACE);

    // Display the X-Ray image
    if (g_display_detector)
    {
        // Normalise the X-ray image
        g_xray_renderer.normalise();

        g_xray_renderer.display();
    }

    // Display the beam
    if (g_display_beam)
    {
        const GLfloat material_ambient[]  = {0.75, 0, 0.5, 0.15};
        handle = glGetUniformLocation(shader_id, "material_ambient");
        glUniform4fv(handle, 1, &material_ambient[0]);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id,"g_use_lighting");
        glUniform1i(handle, 0);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        g_xray_detector.displayBeam();
    }
    // Restore the current transformation matrix
    popModelViewMatrix();
    popProjectionMatrix();

    // Disable the shader and restore the previous shader from the stack
    popShaderProgram();

    // Restore the attributes
    popEnableDisableState();

    // Check the status of OpenGL and of the current FBO
    checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//-----------------
void idleCallback()
//-----------------
{
}


//-----------------
void quitCallback()
//-----------------
{
    // The window exists
    if (g_p_window_id)
    {
        // Close the window
        glfwDestroyWindow(g_p_window_id);
        g_p_window_id = 0;

        // Cleanup GLFW
        glfwTerminate();
    }
}


//-----------------------------------------------------------------------
void framebufferSizeCallback(GLFWwindow* apWindow, int width, int height)
//-----------------------------------------------------------------------
{
    // Avoid a division by zero
    if (height == 0)
    {
        // Prevent divide by 0
        height = 1;
    }

    int x(0), y(0), w(width), h(height);

    // Store the width and height of the window
    g_window_width = width;
    g_window_height = height;

    // Compute the aspect ratio of the size of the window
    double screen_aspect_ratio(double(g_window_width) / double(g_window_height));

    // Update the viewport
    glViewport(x, y, w, h);

    // Set up the projection matrix (g_current_projection_matrix)
    loadPerspectiveProjectionMatrix(45.0, screen_aspect_ratio, 0.01 * cm, 10000.0 * cm);

    // Set up the modelling-viewing matrix (g_current_modelview_matrix)
    loadLookAtModelViewMatrix(g_centre.getX(), g_centre.getY(), g_centre.getZ() + g_zoom,
            g_centre.getX(), g_centre.getY(), g_centre.getZ(),
            0.0, 1.0, 0.0);
}


inline std::vector<RealImageType::Pointer>& getCurrentSimulatedProjectionSet()
{
    if (g_use_high_res)
    {
        return (g_p_high_resolution_simulated_projection_set);
    }
    else
    {
        return (g_p_low_resolution_simulated_projection_set);
    }
}


//------------------------
inline void reloadMeshes()
//------------------------
{
    try
    {
        g_is_xray_image_up_to_date = false;

        g_xray_renderer.removeInnerSurfaces();
        g_xray_renderer.removeOuterSurface();

        for (int i(0); i < g_number_of_polygon_meshes; ++i)
        {
            if (g_p_mesh_visibility_flag_set[i])
            {
                g_xray_renderer.addInnerSurface(&g_p_polygon_mesh_set[i]);
            }
        }

        updateXRayImage(getCurrentSimulatedProjectionSet());
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}


//-------------------------------------------------------------------------------
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
//-------------------------------------------------------------------------------
{
    if (action == GLFW_PRESS)
    {
        switch(key)
        {
        // Close the program
        case GLFW_KEY_Q:
        case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(g_p_window_id, GL_TRUE);
                break;

        case GLFW_KEY_B:
            g_display_beam = !g_display_beam;
            break;

        case GLFW_KEY_D:
            g_display_detector = !g_display_detector;
            break;

        case GLFW_KEY_S:
            g_display_source = !g_display_source;
            break;

        case GLFW_KEY_C:
            setColours();
            break;

        case GLFW_KEY_0:
            {
                int index(0);
                if (g_p_mesh_visibility_flag_set.size() > index)
                {
                    g_p_mesh_visibility_flag_set[index] = !g_p_mesh_visibility_flag_set[index];
                    reloadMeshes();
                }
            }
            break;

        case GLFW_KEY_1:
            {
                int index(1);
                if (g_p_mesh_visibility_flag_set.size() > index)
                {
                    g_p_mesh_visibility_flag_set[index] = !g_p_mesh_visibility_flag_set[index];
                    reloadMeshes();
                }
            }
            break;

        case GLFW_KEY_2:
            {
                int index(2);
                if (g_p_mesh_visibility_flag_set.size() > index)
                {
                    g_p_mesh_visibility_flag_set[index] = !g_p_mesh_visibility_flag_set[index];
                    reloadMeshes();
                }
            }
            break;

        case GLFW_KEY_3:
            {
                int index(3);
                if (g_p_mesh_visibility_flag_set.size() > index)
                {
                    g_p_mesh_visibility_flag_set[index] = !g_p_mesh_visibility_flag_set[index];
                    reloadMeshes();
               }
            }
            break;

        case GLFW_KEY_4:
            {
                int index(4);
                if (g_p_mesh_visibility_flag_set.size() > index)
                {
                    g_p_mesh_visibility_flag_set[index] = !g_p_mesh_visibility_flag_set[index];
                    reloadMeshes();
                }
            }
            break;

        case GLFW_KEY_5:
            {
                int index(5);
                if (g_p_mesh_visibility_flag_set.size() > index)
                {
                    g_p_mesh_visibility_flag_set[index] = !g_p_mesh_visibility_flag_set[index];
                    reloadMeshes();
                }
            }
            break;

        case GLFW_KEY_6:
            {
                int index(6);
                if (g_p_mesh_visibility_flag_set.size() > index)
                {
                    g_p_mesh_visibility_flag_set[index] = !g_p_mesh_visibility_flag_set[index];
                    reloadMeshes();
                }
            }
            break;

        case GLFW_KEY_7:
            {
                int index(7);
                if (g_p_mesh_visibility_flag_set.size() > index)
                {
                    g_p_mesh_visibility_flag_set[index] = !g_p_mesh_visibility_flag_set[index];
                    reloadMeshes();
                }
            }
            break;

        case GLFW_KEY_8:
            {
                int index(8);
                if (g_p_mesh_visibility_flag_set.size() > index)
                {
                    g_p_mesh_visibility_flag_set[index] = !g_p_mesh_visibility_flag_set[index];
                    reloadMeshes();
                }
            }
            break;

        case GLFW_KEY_9:
            {
                int index(9);
                if (g_p_mesh_visibility_flag_set.size() > index)
                {
                    g_p_mesh_visibility_flag_set[index] = !g_p_mesh_visibility_flag_set[index];
                    reloadMeshes();
                }
            }
            break;

        default:
            break;
        }
    }
}


//---------------
void setColours()
//---------------
{
    g_p_polygon_mesh_colour_set.clear();
    for (int i(0); i < g_number_of_polygon_meshes; ++i)
    {
        VEC3 colour(float(rand()) / RAND_MAX,
                float(rand()) / RAND_MAX,
                float(rand()) / RAND_MAX);
        colour.normalise();
        g_p_polygon_mesh_colour_set.push_back(colour);
    }
}


//--------------------------------------------
void mouseButtonCallback(GLFWwindow* apWindow,
                         int aButton,
                         int aButtonState,
                         int aModifierKey)
//--------------------------------------------
{
    g_button = aButton;
    g_button_state = aButtonState;

    // Use the arc ball
    if (g_button_state == GLFW_PRESS)
    {
        g_use_arc_ball = true;
    }
    // Stop using the arc ball
    else
    {
        g_use_arc_ball = false;
    }

    double xpos(0);
    double ypos(0);
    glfwGetCursorPos (apWindow, &xpos, &ypos);

    g_last_x_position = xpos;
    g_last_y_position = ypos;

    g_current_x_position = xpos;
    g_current_y_position = ypos;
}


//--------------------------------------------------------------
void cursorPosCallback(GLFWwindow* apWindow, double x, double y)
//--------------------------------------------------------------
{
    g_current_x_position = x;
    g_current_y_position = y;

    computeRotation(g_scene_rotation_matrix);
}


//-----------------------------------------------------------------------
void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset)
//-----------------------------------------------------------------------
{
    // Scrolling along the Y-axis
    if (fabs(yoffset) > EPSILON)
    {
        // Change the zoom
        g_zoom += 5 * yoffset * cm;

        // Add the current shader to the shader stack
        pushShaderProgram();

        // Enable the shader
        g_display_shader.enable();
        GLint shader_id(g_display_shader.getProgramHandle());

        VEC3 light_position(0, 0, g_zoom);
        GLuint handle(glGetUniformLocation(shader_id, "light_position"));
        glUniform4f(handle, light_position.getX(), light_position.getY(), light_position.getZ(), 1.0);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Disable the shader and restore the previous shader from the stack
        popShaderProgram();

        // Update the projection matrix
        framebufferSizeCallback(apWindow, g_window_width, g_window_height);
    }
}


//----------------------------------------------------
void errorCallback(int error, const char* description)
//----------------------------------------------------
{
    // Throw an error
    throw Exception(__FILE__, __FUNCTION__, __LINE__, description);
}


//---------------------------------
VEC3 getArcballVector(int x, int y)
//---------------------------------
{
    VEC3 P(2.0 * float(x) / float(g_window_width) - 1.0,
           2.0 * float(y) / float(g_window_height) - 1.0,
           0);

    P.setY(-P.getY());

    float OP_squared = P.getX() * P.getX() + P.getY() * P.getY();
    if (OP_squared <= 1.0)
    {
        P.setZ(sqrt(1.0 - OP_squared));  // Pythagore
    }
    else
    {
        P.normalise();  // Nearest point
    }

    return (P);
}


//--------------------------------
float radian2degree(float anAngle)
//--------------------------------
{
    return (180.0 * anAngle / gVirtualXRay::PI);
}


//--------------------------------------------
void computeRotation(MATRIX4& aRotationMatrix)
//--------------------------------------------
{
    if (g_use_arc_ball)
    {
        if (g_current_x_position != g_last_x_position || g_current_y_position != g_last_y_position)
        {
            VEC3 va(getArcballVector(g_last_x_position,    g_last_y_position));
            VEC3 vb(getArcballVector(g_current_x_position, g_current_y_position));

            float angle(g_rotation_speed * radian2degree(acos(std::min(1.0, va.dotProduct(vb)))));

            VEC3 axis_in_camera_coord(va ^ vb);
            //axis_in_camera_coord.normalize();

            MATRIX4 camera2object(aRotationMatrix.getInverse());
            VEC3 axis_in_object_coord = camera2object * axis_in_camera_coord;
            //axis_in_object_coord.normalize();

            MATRIX4 rotation_matrix;
            rotation_matrix.rotate(angle, axis_in_object_coord);
            aRotationMatrix = aRotationMatrix * rotation_matrix;

            g_last_x_position = g_current_x_position;
            g_last_y_position = g_current_y_position;
        }
    }
}


//----------------------------------------
void loadParameters(int argc, char** argv)
//----------------------------------------
{
    // Declare the namespace
    namespace po = boost::program_options;

    // Declare messages
    std::stringstream input_message;
    input_message << "set the name of the file containing the CT scan";

    std::stringstream output_message;
    input_message << "set the output path (i.e. where files will be saved)";

    std::stringstream filter_message;
    filter_message << "use anisotropic diffusion";

    std::stringstream energy_message;
    energy_message << "set the energy in keV (several values can be used)";

    std::stringstream angle_message;
    angle_message << "number of angles in the sinogram";

    std::stringstream low_res_message;
    low_res_message << "optimisation using low res volumes";

    std::stringstream high_res_message;
    high_res_message << "optimisation using high res volumes";

    std::stringstream threshold_message;
    threshold_message << "number of thresholds to be generated (at least 3 are needed) or the set of threshold values";

    std::stringstream restart_message;
    restart_message << "restart the optimisation";

    std::stringstream max_iteration_message;
    max_iteration_message << "maximum number of iterations";

    std::stringstream optimizer_message;
#ifdef HAS_GSL
    optimizer_message << "optimization method (valid values are: nmsimplex, nmsimplex2, nmsimplex2rand, genetic, and random)";
#else
    optimizer_message << "optimization method (valid values are: genetic, and random)";
#endif

    std::stringstream pop_size_message;
    pop_size_message << "number of individuals in the population (used by genetic optimisation)";

    std::stringstream mutation_message;
    mutation_message << "probability of mutation (used by genetic optimisation)";

    std::stringstream immigration_message;
    immigration_message << "probability of immigration (used by genetic optimisation)";

    std::stringstream crossover_message;
    crossover_message << "probability of crossover (used by genetic optimisation)";

    std::stringstream low_threshold_message;
    low_threshold_message << "lowest segmentation threshold (if same as highest segmentation threshold, then it will be automatically computed)";

    std::stringstream high_threshold_message;
    high_threshold_message << "highest segmentation threshold (if same as lowest segmentation threshold, then it will be automatically computed)";

    std::stringstream default_number_of_threshold;
    default_number_of_threshold<< g_default_number_of_threshold;

    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help",                                                                                           "produce this help message")
            ("input,I",            po::value<std::string>(),                                                   input_message.str().c_str())
            ("output,O",           po::value<std::string>(),                                                   output_message.str().c_str())
            ("filter,F",           po::value<bool>()->default_value(false),                                    filter_message.str().c_str())
            ("energy,E",           po::value<std::string>()->default_value("80 160"),                          energy_message.str().c_str())
            ("angles,A",           po::value<int>()->default_value(4),                                         angle_message.str().c_str())
            ("low_res,L",          po::value<bool>()->default_value(g_use_low_res),                            low_res_message.str().c_str())
            ("high_res,H",         po::value<bool>()->default_value(g_use_high_res),                           high_res_message.str().c_str())
            ("thresholds,T",       po::value<std::string>()->default_value(default_number_of_threshold.str()), threshold_message.str().c_str())
            ("restart,R",          po::value<bool>()->default_value(g_restart),                                restart_message.str().c_str())
            ("max_nb_iteration,M", po::value<int>()->default_value(g_max_iteration),                           max_iteration_message.str().c_str())
            ("pop_size",           po::value<int>()->default_value(g_pop_size),                                pop_size_message.str().c_str())
            ("optimizer",          po::value<std::string>()->default_value(g_optimizer),                       optimizer_message.str().c_str())
            ("mutation",           po::value<double>()->default_value(g_mutation_probability),                 mutation_message.str().c_str())
            ("immigration",        po::value<double>()->default_value(g_immigration_probability),              immigration_message.str().c_str())
            ("crossover",          po::value<double>()->default_value(g_crossover_probability),                crossover_message.str().c_str())
            ("low_threshold",      po::value<short>()->default_value(g_min_value),                             low_threshold_message.str().c_str())
            ("high_threshold",     po::value<short>()->default_value(g_max_value),                             high_threshold_message.str().c_str())
            ;

    // Parse command line options
    po::variables_map variables_map;
    po::store(po::parse_command_line(argc, argv, desc), variables_map);
    po::notify(variables_map);

    // Produce the help message
    if (variables_map.count("help"))
    {
        std::cout << desc << std::endl;
        exit(EXIT_SUCCESS);
    }

    // Threshold limits
    if (variables_map.count("low_threshold"))
    {
        g_min_value =  variables_map["low_threshold"].as<short>();
    }

    // Threshold limits
    if (variables_map.count("high_threshold"))
    {
        g_max_value =  variables_map["high_threshold"].as<short>();
    }
    if (g_max_value < g_min_value)
    {
        short temp(g_max_value);
        g_max_value = g_min_value;
        g_min_value = temp;
    }

    // Probability operators
    double sum_of_probabilities(0);
    if (variables_map.count("mutation"))
    {
        g_mutation_probability =  variables_map["mutation"].as<double>();
        sum_of_probabilities += g_mutation_probability;
    }

    if (variables_map.count("immigration"))
    {
        g_immigration_probability =  variables_map["immigration"].as<double>();
        sum_of_probabilities += g_immigration_probability;
    }

    if (variables_map.count("crossover"))
    {
        g_crossover_probability =  variables_map["crossover"].as<double>();
        sum_of_probabilities += g_crossover_probability;
    }
    g_mutation_probability    /= sum_of_probabilities;
    g_immigration_probability /= sum_of_probabilities;
    g_crossover_probability   /= sum_of_probabilities;

    // Population size
    if (variables_map.count("pop_size"))
    {
        g_pop_size =  variables_map["pop_size"].as<int>();
    }

    // Restart the optimisation
    if (variables_map.count("restart"))
    {
        g_restart =  variables_map["restart"].as<bool>();
    }

    // Restart the optimisation
    if (variables_map.count("max_nb_iteration"))
    {
        g_max_iteration =  variables_map["max_nb_iteration"].as<int>();
    }

    // Select the optimiser
    if (variables_map.count("optimizer"))
    {
        g_optimizer =  variables_map["optimizer"].as<std::string>();
    }

    // Set the input file
    if (variables_map.count("input"))
    {
        g_input_file_name =  variables_map["input"].as<std::string>();
    }

    // Set the output directory
    if (variables_map.count("output"))
    {
        g_output_file_name =  variables_map["output"].as<std::string>();
    }

    // Set the output directory
    if (variables_map.count("filter"))
    {
        g_use_filter =  variables_map["filter"].as<bool>();
    }

    // Set the angle span
    if (variables_map.count("energy"))
    {
        double energy;
        std::stringstream energy_stream;
        energy_stream << variables_map["energy"].as<std::string>();

        while (!energy_stream.eof())
        {
            energy_stream >> energy;
            g_p_energy_set.push_back(energy * keV);
        }
    }

    // Set the angle span
    if (variables_map.count("angles"))
    {
        int angles(variables_map["angles"].as<int>());
        if (angles < 5)
        {
            angles = 5;
        }

        double first_angle(0.0);
        double last_angle(90.0);
        double angle_offset((last_angle - first_angle) / double(angles - 1));
        for (int i(0); i < angles; ++i)
        {
            g_p_angle_set.push_back(first_angle + angle_offset * i);
        }
    }

    // Set the number of thresholds
    if (variables_map.count("thresholds"))
    {
        std::stringstream thresholds_from_list_of_parameters;
        thresholds_from_list_of_parameters << variables_map["thresholds"].as<std::string>();

        g_p_threshold_set.clear();
        std::vector<short> p_threshold_set;
        while (!thresholds_from_list_of_parameters.eof())
        {
            short current_threshold;
            thresholds_from_list_of_parameters >> current_threshold;
            p_threshold_set.push_back(current_threshold);
        }

        short number_of_thresholds;
        if (p_threshold_set.size() == 1)
        {
            // At least two thresholds are needed
            number_of_thresholds = p_threshold_set.front();
            if (number_of_thresholds < g_default_number_of_threshold)
            {
                number_of_thresholds = g_default_number_of_threshold;
            }
        }
        else
        {
            number_of_thresholds = p_threshold_set.size();
            g_p_threshold_set = p_threshold_set;
        }

        g_number_of_polygon_meshes = number_of_thresholds - 1;
        g_p_mesh_visibility_flag_set.resize(g_number_of_polygon_meshes, true);
    }

    // Set the resolution
    if (variables_map.count("low_res"))
    {
        g_use_low_res = variables_map["low_res"].as<bool>();
    }
    if (variables_map.count("high_res"))
    {
        g_use_high_res = variables_map["high_res"].as<bool>();
    }
    if (!g_use_low_res && !g_use_high_res)
    {
    	g_use_low_res = true;
    }
}


//--------------------------------------------------
void loadVolumeDataset(const std::string& aFileName)
//--------------------------------------------------
{
    // Read the volume
    g_p_reader = ImageFileReaderType::New();
    g_p_reader->SetFileName(aFileName);
    g_p_reader->Update();

    // Compute stats
    g_p_ct_statistics = StatisticsImageFilterType::New();
    g_p_ct_statistics->SetInput(g_p_reader->GetOutput());
    g_p_ct_statistics->Update();

    // Compute default threshold
    if (!g_p_threshold_set.size())
    {
        if (g_min_value == g_max_value)
        {
            g_min_value = g_p_ct_statistics->GetMinimum();
            g_max_value = g_p_ct_statistics->GetMaximum();
        }

        short threshold_offset((1000 - -1000) / (g_number_of_polygon_meshes));

        short first_threshold(-1000);

        int i(0);
        for (i = 0; i < g_number_of_polygon_meshes - 1; ++i)
        {
            g_p_threshold_set.push_back(first_threshold + threshold_offset * i);
        }
        /*g_p_threshold_set.clear();

        g_p_threshold_set.push_back(-1000);
        g_p_threshold_set.push_back(-500);
        g_p_threshold_set.push_back(-75);
        g_p_threshold_set.push_back(0);
        g_p_threshold_set.push_back(15);
        g_p_threshold_set.push_back(30);
        g_p_threshold_set.push_back(40);
        g_p_threshold_set.push_back(60);
        g_p_threshold_set.push_back(100);
        g_p_threshold_set.push_back(300);
        g_p_threshold_set.push_back(700);
    */
    }

    // Compute the prefix file name of the output files
    std::string file_name_prefix;
    if (g_output_file_name.size())
    {
        file_name_prefix = g_output_file_name + "/";
    }

    // Copy the image
    typedef itk::ImageDuplicator<RealImageType> DuplicatorType;
    DuplicatorType::Pointer p_duplicator(DuplicatorType::New());

    // Filter
    if (g_use_filter)
    {
        typedef itk::GradientAnisotropicDiffusionImageFilter<RealImageType, RealImageType> SmoothingFilterType;
        SmoothingFilterType::Pointer p_smoothing_filter(SmoothingFilterType::New());
        p_smoothing_filter->SetNumberOfIterations(3);
        p_smoothing_filter->SetTimeStep(0.0443115);
        p_smoothing_filter->SetConductanceParameter(5.0);
        p_smoothing_filter->SetInput(g_p_reader->GetOutput());
        p_smoothing_filter->Update();

        p_duplicator->SetInputImage(p_smoothing_filter->GetOutput());

        // Write the filter data to a file
        ImageFileWriterType::Pointer p_writer(ImageFileWriterType::New());
        p_writer->SetFileName(file_name_prefix + "filtered_input.mhd");
        p_writer->SetInput(p_smoothing_filter->GetOutput());
        p_writer->Update();
    }
    else
    {
        p_duplicator->SetInputImage(g_p_reader->GetOutput());
    }
    p_duplicator->Update();
    itk::Point<double, 3> origin(0);
    p_duplicator->GetOutput()->SetOrigin(origin);
    g_p_high_res_volume = p_duplicator->GetOutput();

    if (g_use_low_res)
    {
        g_p_low_res_volume = RealImageType::New();

        // Create interpolator
        typedef itk::LinearInterpolateImageFunction<RealImageType, double> InterpolatorType;
        typename InterpolatorType::Pointer interpolator(InterpolatorType::New());

        // Create a filter
        typedef itk::ResampleImageFilter<RealImageType, RealImageType>  FilterType;
        typename FilterType::Pointer p_resampling_filter(FilterType::New());

        // Set the volume size
        RealImageType::SizeType old_size(g_p_high_res_volume->GetBufferedRegion().GetSize());
        RealImageType::SizeType new_size(old_size);
        new_size[0] /= 8;
        new_size[1] /= 8;
        new_size[2] /= 4;
        p_resampling_filter->SetSize(new_size);

        // Set the first voxel
        typename RealImageType::IndexType start;
        start[0] = 0;
        start[1] = 0;
        start[2] = 0;
        p_resampling_filter->SetOutputStartIndex(start);

        // Set the origin
        p_resampling_filter->SetOutputOrigin(g_p_high_res_volume->GetOrigin());

        // Set the new voxel size
        RealImageType::SpacingType old_spacing(g_p_high_res_volume->GetSpacing());
        RealImageType::SpacingType new_spacing;
        new_spacing[0] = old_spacing[0] * (double(old_size[0]) / double(new_size[0]));
        new_spacing[1] = old_spacing[1] * (double(old_size[1]) / double(new_size[1]));
        new_spacing[2] = old_spacing[2] * (double(old_size[2]) / double(new_size[2]));
        p_resampling_filter->SetOutputSpacing(new_spacing);

        // Set the interpolator
        p_resampling_filter->SetInterpolator(interpolator);

        // Set the input data
        p_resampling_filter->SetInput(g_p_high_res_volume);

        // Resample the data
        p_resampling_filter->Update();

        g_p_low_res_volume = p_resampling_filter->GetOutput();

        // Write the filter data to a file
        std::stringstream low_resolution_file_name;
        ImageFileWriterType::Pointer p_writer(ImageFileWriterType::New());
        low_resolution_file_name  << file_name_prefix << "low_resolution_ct.mhd";
        p_writer->SetFileName(low_resolution_file_name.str());
        p_writer->SetInput(g_p_low_res_volume);
        p_writer->Update();

    }

    // Initialise the detector
    g_xray_detector.setResolutionInUnitsOfLengthPerPixel(g_resolution);
    g_xray_detector.setNumberOfPixels(g_number_of_pixels);

    // Initialise the beam (set a dummy energy)
    g_xray_beam.initialise(0);

    // Initialise the X-ray renderer
#ifdef HAS_OPENGL2
    g_xray_renderer.initialise(XRayRenderer::OPENGL,
            GL_RGB16F_ARB,
            &g_xray_detector,
            &g_xray_beam);
#endif

#ifdef HAS_OPENGL3
    g_xray_renderer.initialise(XRayRenderer::OPENGL,
            GL_RGB16F,
            &g_xray_detector,
            &g_xray_beam);
#endif

    if (g_use_high_res)
    {
        g_p_high_resolution_mu_volume_set.clear();
        g_p_high_resolution_mu_volume_set.resize(g_p_energy_set.size());
    }

    if (g_use_low_res)
    {
        g_p_low_resolution_mu_volume_set.clear();
        g_p_low_resolution_mu_volume_set.resize(g_p_energy_set.size());
    }

    int i;
#ifdef HAS_OPENMP
#pragma omp parallel for \
        default(none) \
        private(i) \
        shared(file_name_prefix, g_p_energy_set) \
        shared(g_use_high_res, g_p_high_res_volume, g_p_high_resolution_mu_volume_set) \
        shared(g_use_low_res,  g_p_low_res_volume,  g_p_low_resolution_mu_volume_set)
#endif
    for (i = 0; i < g_p_energy_set.size(); ++i)
    {
    	if (g_use_high_res)
    	{
			convertHU2mu(g_p_high_res_volume, g_p_high_resolution_mu_volume_set[i], g_p_energy_set[i]);

			// Write the filter data to a file
			std::stringstream high_resolution_file_name;
			ImageFileWriterType::Pointer p_writer(ImageFileWriterType::New());
			high_resolution_file_name << file_name_prefix << "high_resolution_mu_" << g_p_energy_set[i] / keV << "keV.mhd";
			p_writer->SetFileName(high_resolution_file_name.str());
			p_writer->SetInput(g_p_high_resolution_mu_volume_set[i]);
			p_writer->Update();
    	}

    	if (g_use_low_res)
    	{
        	convertHU2mu(g_p_low_res_volume, g_p_low_resolution_mu_volume_set[i], g_p_energy_set[i]);

            // Write the filter data to a file
            std::stringstream low_resolution_file_name;
            ImageFileWriterType::Pointer p_writer(ImageFileWriterType::New());
            low_resolution_file_name << file_name_prefix << "low_resolution_mu_" << g_p_energy_set[i] / keV << "keV.mhd";
            p_writer->SetFileName(low_resolution_file_name.str());
            p_writer->SetInput(g_p_low_resolution_mu_volume_set[i]);
            p_writer->Update();
    	}
    }
}


//------------------------------------------------------------------
void convertHU2mu(const RealImageType::Pointer& aVolume,
                  RealImageType::Pointer& aMuVolume,
                  double anEnergy)
//------------------------------------------------------------------
{
    // Copy the image
    typedef itk::ImageDuplicator<RealImageType> DuplicatorType;
    DuplicatorType::Pointer p_duplicator(DuplicatorType::New());
    p_duplicator->SetInputImage(aVolume);
    p_duplicator->Update();

    // Convert voxel values from HU to attenuation coefficients
    typedef itk::ImageRegionIterator<RealImageType> ImageIterator;
    RealImageType::RegionType region(p_duplicator->GetOutput()->GetLargestPossibleRegion());
    ImageIterator image_ite(p_duplicator->GetOutput(), region);

    for (image_ite.GoToBegin(); !image_ite.IsAtEnd(); ++image_ite)
    {
        double HU(image_ite.Get());
        double mu_water(g_xray_renderer.getMuWaterLogLinearInterpolation(anEnergy));
        double mu(g_xray_renderer.convertHounsfieldValue2Mu(HU, mu_water));

        if (mu > 0.0)
        {
            image_ite.Set(mu);
        }
        else
        {
            image_ite.Set(0.0);
        }
    }

    aMuVolume = p_duplicator->GetOutput();
}


//------------------
void loadSinograms()
//------------------
{
    // Initialise Octave
    string_vector octave_argv(2);
    octave_argv(0) = "embedded";
    octave_argv(1) = "-q";


    if (!octave_main (2, octave_argv.c_str_vec (), 1))
    {
        throw ("Cannot initialise Octave.");
    }

    // Load the image package
    octave_value_list load_package_arguments;
    load_package_arguments(0) = "load";
    load_package_arguments(1) = "image";
    const octave_value_list load_package_result = feval ("pkg", load_package_arguments);

    std::string file_name_prefix;
    if (g_output_file_name.size())
    {
        file_name_prefix = g_output_file_name + "/";
    }

    int i(0);
    for (std::vector<RealImageType::Pointer>::const_iterator ite(g_p_low_resolution_mu_volume_set.begin());
            ite != g_p_low_resolution_mu_volume_set.end();
            ++ite, ++i)
    {
        volume2sinogram(*ite, g_p_low_resolution_sinogram_set, g_p_energy_set[i]);

        // Write the filter data to a file
        std::stringstream sinogram_file_name;
        ImageFileWriterType::Pointer p_writer(ImageFileWriterType::New());
        sinogram_file_name  << file_name_prefix << "low_resolution_sinogram_mu_"  << g_p_energy_set[i] / keV << "keV.mhd";
        p_writer->SetFileName(sinogram_file_name.str());
        p_writer->SetInput(g_p_low_resolution_sinogram_set[i]);
        p_writer->Update();
    }

	i = 0;
	for (std::vector<RealImageType::Pointer>::const_iterator ite(g_p_high_resolution_mu_volume_set.begin());
			ite != g_p_high_resolution_mu_volume_set.end();
			++ite, ++i)
	{
		volume2sinogram(*ite, g_p_high_resolution_sinogram_set, g_p_energy_set[i]);

		// Write the filter data to a file
		std::stringstream sinogram_file_name;
		ImageFileWriterType::Pointer p_writer(ImageFileWriterType::New());
		sinogram_file_name  << file_name_prefix << "high_resolution_sinogram_mu_"  << g_p_energy_set[i] / keV << "keV.mhd";
		p_writer->SetFileName(sinogram_file_name.str());
		p_writer->SetInput(g_p_high_resolution_sinogram_set[i]);
		p_writer->Update();
	}
}


//----------------------------------------------------------
void loadPoygonMeshes(const RealImageType::Pointer& aVolume,
                      bool aSaveSegmentationFlag)
//----------------------------------------------------------
{
    g_xray_renderer.removeOuterSurface();
    g_xray_renderer.removeInnerSurfaces();

    std::sort(g_p_threshold_set.begin(), g_p_threshold_set.end());

    int i;
    g_p_polygon_mesh_set.clear();
    g_p_vertex_set.clear();
    g_p_index_set.clear();

    g_p_polygon_mesh_set.resize(g_number_of_polygon_meshes);
    g_p_vertex_set.resize(g_number_of_polygon_meshes);
    g_p_index_set.resize(g_number_of_polygon_meshes);

#ifdef HAS_OPENMP
#pragma omp parallel for \
        default(none) \
        private(i) \
        shared(g_p_threshold_set, g_max_value, aVolume, aSaveSegmentationFlag, g_p_polygon_mesh_set, g_p_vertex_set, g_p_index_set)
#endif
    for (i = 0; i < g_p_polygon_mesh_set.size(); ++i)
    {
        short low_threshold(g_p_threshold_set[i]);
        if (i != 0)
        {
            low_threshold += 1;
        }

        short high_threshold(g_p_threshold_set[i + 1]);

        // Extract the iso-surface
        short hounsfield_value(0);
        extractIsosurface(aVolume, low_threshold, high_threshold, hounsfield_value, aSaveSegmentationFlag, i);

        g_p_polygon_mesh_set[i].setHounsfieldValue(hounsfield_value);
    }

    for (i = 0; i < g_p_polygon_mesh_set.size(); ++i)
    {
        short HU(g_p_polygon_mesh_set[i].getHounsfieldValue());
        g_p_polygon_mesh_set[i].setExternalData(GL_TRIANGLES,
                &(g_p_vertex_set[i]),
                &(g_p_index_set[i]),
                true,
                GL_STATIC_DRAW);

        g_p_polygon_mesh_set[i].setHounsfieldValue(HU);

        // There is data in the mesh
        if (g_p_vertex_set[i].size())
        {
            g_xray_renderer.addInnerSurface(&g_p_polygon_mesh_set[i]);
        }
    }

    setColours();
    g_is_xray_image_up_to_date = false;
}


//---------------------------------------------------------------------
void volume2sinogram(const RealImageType::Pointer& aVolume,
                     std::vector<RealImageType::Pointer>& aSinogramSet,
                     double anEnergy)
//---------------------------------------------------------------------
{
    RealImageType::SizeType volume_size(aVolume->GetBufferedRegion().GetSize());

    RealImageType::Pointer sinogram(RealImageType::New());

    Matrix ct_slice(volume_size[0], volume_size[1]);
    Matrix slice_sinogram;

    // The slice size
    int slice_width(0);
    int slice_height(0);

    // The volume data
    float* p_volume_data(aVolume->GetBufferPointer());

    // Set the angle span
    Matrix angle_span(1, g_p_angle_set.size());
    int angle_id(0);
    for (std::vector<double>::const_iterator ite(g_p_angle_set.begin());
            ite != g_p_angle_set.end();
            ++ite, ++angle_id)
    {
        angle_span(0, angle_id) = *ite;
    }

    // Process all the slices
    int sinogram_width(0);
    int sinogram_height(0);
    for (int k(0); k < volume_size[2]; ++k)
    {
        // Copy the slice into Octave's matrix
        for (octave_idx_type j(0); j < volume_size[1]; ++j)
        {
            for (octave_idx_type i(0); i < volume_size[0]; ++i)
            {
                ct_slice(i, j) = p_volume_data[k * volume_size[0] * volume_size[1] +
                                               j * volume_size[0] +
                                               i];
            }
        }

        // Set the radon transform using Octave
        octave_value_list function_arguments;
        function_arguments(0) = ct_slice;
        function_arguments(1) = angle_span;

        // Perform the reconstruction
        const octave_value_list result(feval ("radon", function_arguments, 1));

        // Save the result
        slice_sinogram = Matrix(result(0).matrix_value());

        // Allocate the memory
        if (k == 0)
        {
            // Store the 2D sinogram size
            sinogram_width  = slice_sinogram.dim1();
            sinogram_height = slice_sinogram.dim2();

            // Set volume size (in number of voxel)
            RealImageType::SizeType sinogram_size;
            sinogram_size[0] = sinogram_width;
            sinogram_size[1] = sinogram_height;
            sinogram_size[2] = volume_size[2];

            // Set start index
            RealImageType::IndexType start;
            start[0] = 0;
            start[1] = 0;
            start[2] = 0;

            // Set region
            RealImageType::RegionType region;
            region.SetSize(sinogram_size);
            region.SetIndex(start);

            // Allocate the data
            sinogram->SetRegions(region);
            sinogram->Allocate();
            sinogram->FillBuffer(0);

            // Set the voxel size
            RealImageType::SpacingType spacing(aVolume->GetSpacing());

            if (g_p_angle_set.size() >= 2)
            {
                spacing[1] = g_p_angle_set[1] - g_p_angle_set[0];
            }
            else
            {
                spacing[1] = 1;
            }
            sinogram->SetSpacing(spacing);
        }

        // Copy voxels
        RealImageType::IndexType index;
        for (unsigned int j = 0; j < sinogram_height; ++j)
        {
            for (unsigned int i = 0; i < sinogram_width; ++i)
            {
                index[0] = i;
                index[1] = j;
                index[2] = k;
                sinogram->SetPixel(index, slice_sinogram(i, j));
            }
        }
    }

    // Copy the image
    typedef itk::ImageDuplicator<RealImageType> DuplicatorType;
    DuplicatorType::Pointer p_duplicator(DuplicatorType::New());
    p_duplicator->SetInputImage(sinogram);
    p_duplicator->Update();

    // Add the image to the list of sinograms
    aSinogramSet.push_back(p_duplicator->GetOutput());
}


//--------------------
void loadProjections()
//--------------------
{
    g_p_low_resolution_reference_projection_set.clear();
    g_p_low_resolution_simulated_projection_set.clear();

    g_p_high_resolution_reference_projection_set.clear();
    g_p_high_resolution_simulated_projection_set.clear();

    g_p_low_resolution_reference_projection_set.resize(g_p_low_resolution_sinogram_set.size());
    g_p_low_resolution_simulated_projection_set.resize(g_p_low_resolution_sinogram_set.size());

    g_p_high_resolution_reference_projection_set.resize(g_p_high_resolution_sinogram_set.size());
    g_p_high_resolution_simulated_projection_set.resize(g_p_high_resolution_sinogram_set.size());

    std::string file_name_prefix;
    if (g_output_file_name.size())
    {
        file_name_prefix = g_output_file_name + "/";
    }

    int i(0);
#ifdef HAS_OPENMP
#pragma omp parallel for \
        default(none) \
        private(i) \
        shared(file_name_prefix, g_p_energy_set) \
        shared(g_p_low_resolution_sinogram_set,  g_p_low_resolution_reference_projection_set,  g_p_low_resolution_simulated_projection_set)
#endif
    for (i = 0; i < g_p_low_resolution_sinogram_set.size(); ++i)
    {
        sinogram2projection(g_p_low_resolution_sinogram_set[i], g_p_low_resolution_reference_projection_set[i], g_p_low_resolution_simulated_projection_set[i]);

        // Write the filter data to a file
        std::stringstream projection_file_name;
        ImageFileWriterType::Pointer p_writer(ImageFileWriterType::New());
        projection_file_name  << file_name_prefix << "low_resolution_reference_projection_"  << g_p_energy_set[i] / keV << "keV.mhd";
        p_writer->SetFileName(projection_file_name.str());
        p_writer->SetInput(g_p_low_resolution_reference_projection_set[i]);
        p_writer->Update();
    }

    i = 0;
#ifdef HAS_OPENMP
#pragma omp parallel for \
        default(none) \
        private(i) \
        shared(file_name_prefix, g_p_energy_set) \
        shared(g_p_high_resolution_sinogram_set, g_p_high_resolution_reference_projection_set, g_p_high_resolution_simulated_projection_set)
#endif
    for (i = 0; i < g_p_high_resolution_sinogram_set.size(); ++i)
    {
        sinogram2projection(g_p_high_resolution_sinogram_set[i],
                g_p_high_resolution_reference_projection_set[i],
                g_p_high_resolution_simulated_projection_set[i]);

        // Write the filter data to a file
        std::stringstream projection_file_name;
        ImageFileWriterType::Pointer p_writer(ImageFileWriterType::New());
        projection_file_name  << file_name_prefix << "high_resolution_reference_projection_"  << g_p_energy_set[i] / keV << "keV.mhd";
        p_writer->SetFileName(projection_file_name.str());
        p_writer->SetInput(g_p_high_resolution_reference_projection_set[i]);
        p_writer->Update();
    }
}


//------------------------------------------------------------------------------------
void sinogram2projection(const RealImageType::Pointer& aSinogram,
                         RealImageType::Pointer& aReferenceProjection,
                         RealImageType::Pointer& aSimulatedProjection)
//------------------------------------------------------------------------------------
{
    // Set sinogram size (in number of voxel)
    RealImageType::SizeType sinogram_size(aSinogram->GetBufferedRegion().GetSize());

    RealImageType::Pointer projection(RealImageType::New());

    // Set projection size (in number of voxel)
    RealImageType::SizeType projection_size;
    projection_size[0] = sinogram_size[0];
    projection_size[1] = sinogram_size[2];
    projection_size[2] = sinogram_size[1];

    // Set start index
    RealImageType::IndexType start;
    start[0] = 0;
    start[1] = 0;
    start[2] = 0;

    // Set region
    RealImageType::RegionType region;
    region.SetSize(projection_size);
    region.SetIndex(start);

    // Allocate the data
    projection->SetRegions(region);
    projection->Allocate();
    projection->FillBuffer(0);

    // Set the voxel size
    RealImageType::SpacingType old_spacing(aSinogram->GetSpacing());
    RealImageType::SpacingType new_spacing;
    new_spacing[0] = old_spacing[0];
    new_spacing[1] = old_spacing[2];
    new_spacing[2] = old_spacing[1];
    projection->SetSpacing(new_spacing);

    // Copy the reference image
    typedef itk::ImageDuplicator<RealImageType> DuplicatorType;
    DuplicatorType::Pointer p_duplicator_simulated(DuplicatorType::New());
    p_duplicator_simulated->SetInputImage(projection);
    p_duplicator_simulated->Update();

    // Add the image to the list of sinograms
    aSimulatedProjection = p_duplicator_simulated->GetOutput();

    RealImageType::IndexType sinogram_index;
    RealImageType::IndexType projection_index;
    float value;
    for (unsigned int k = 0; k < sinogram_size[2]; ++k)
    {
        for (unsigned int j = 0; j < sinogram_size[1]; ++j)
        {
            for (unsigned int i = 0; i < sinogram_size[0]; ++i)
            {
                sinogram_index[0] = i;
                sinogram_index[1] = j;
                sinogram_index[2] = k;

                projection_index[0] = i;
                projection_index[1] = k;
                projection_index[2] = j;

                projection->SetPixel(projection_index, aSinogram->GetPixel(sinogram_index));
            }
        }
    }

    Normalise(projection);

    // Copy the reference image
    DuplicatorType::Pointer p_duplicator_reference(DuplicatorType::New());
    p_duplicator_reference->SetInputImage(projection);
    p_duplicator_reference->Update();

    // Add the image to the list of sinograms
    aReferenceProjection = p_duplicator_reference->GetOutput();
}


//-----------------------------------------------------------
void extractIsosurface(const RealImageType::Pointer& aVolume,
                       short aMinHUValue,
                       short aMaxHUValue,
                       short& anAverageHounsfielValue,
                       bool aSaveSegmentationFlag,
                       int temp)
//-----------------------------------------------------------
{
    typedef itk::QuadEdgeMesh<float, 3>  MeshType;

    typedef itk::BinaryThresholdImageFilter< RealImageType, RealImageType > BinaryThresholdType;
    typedef itk::BinaryMask3DMeshSource< RealImageType, MeshType >   MeshSourceType;

    BinaryThresholdType::Pointer binary_threshold_filter(BinaryThresholdType::New());
    binary_threshold_filter->SetInput(aVolume);
    binary_threshold_filter->SetOutsideValue(0);
    binary_threshold_filter->SetInsideValue(1);

    if (aMinHUValue < aMaxHUValue)
    {
        binary_threshold_filter->SetLowerThreshold(aMinHUValue);
        binary_threshold_filter->SetUpperThreshold(aMaxHUValue);
    }
    else
    {
        binary_threshold_filter->SetLowerThreshold(aMaxHUValue);
        binary_threshold_filter->SetUpperThreshold(aMinHUValue);
    }
    binary_threshold_filter->Update();

    int voxel_count(0);
    double total(0);

    // Convert voxel values from HU to attenuation coefficients
    typedef itk::ImageRegionIterator<RealImageType> ImageIterator;
    RealImageType::RegionType region1(binary_threshold_filter->GetOutput()->GetLargestPossibleRegion());
    ImageIterator image_ite1(binary_threshold_filter->GetOutput(), region1);

    RealImageType::RegionType region2(aVolume->GetLargestPossibleRegion());
    ImageIterator image_ite2(aVolume, region2);

    // Compute the average value of the segmented volume
    int number_of_voxels(0);
    for (image_ite1.GoToBegin(), image_ite2.GoToBegin(); !image_ite1.IsAtEnd() && !image_ite2.IsAtEnd(); ++image_ite1,  ++image_ite2, ++number_of_voxels)
    {
        if (image_ite1.Get() > 0.5)
        {
            ++voxel_count;
            total += image_ite2.Get();
        }
    }

    //g_p_vertex_set[temp] = std::vector<float>();
    //g_p_index_set[temp] = std::vector<unsigned int>();

    if (100.0 * voxel_count / number_of_voxels >= 0.001)
    {
        anAverageHounsfielValue = total / voxel_count;

        // Write the filter data to a file
        if (aSaveSegmentationFlag)
        {
            std::stringstream file_name_prefix;
            if (g_output_file_name.size())
            {
                file_name_prefix << g_output_file_name << "/";
            }
            file_name_prefix << "segmentation_" << aMinHUValue << "_" << aMaxHUValue << ".mhd";
            ImageFileWriterType::Pointer p_writer(ImageFileWriterType::New());
            p_writer->SetFileName(file_name_prefix.str());
            p_writer->SetInput(binary_threshold_filter->GetOutput());
            p_writer->Update();
        }

        // Padding to make sure the mesh is closed
        RealImageType::SizeType lower_extend_region;
        lower_extend_region[0] = 1;
        lower_extend_region[1] = 1;
        lower_extend_region[2] = 1;

        RealImageType::SizeType upper_extend_region;
        upper_extend_region[0] = 1;
        upper_extend_region[1] = 1;
        upper_extend_region[2] = 1;

        RealImageType::PixelType constant_pixel(0);

        typedef itk::ConstantPadImageFilter<RealImageType, RealImageType> PaddingFilter;
        PaddingFilter::Pointer p_padding_filter(PaddingFilter::New());
        p_padding_filter->SetInput(binary_threshold_filter->GetOutput());
        p_padding_filter->SetPadLowerBound(lower_extend_region);
        p_padding_filter->SetPadUpperBound(upper_extend_region);
        p_padding_filter->SetConstant(constant_pixel);
        p_padding_filter->Update();


        MeshSourceType::Pointer meshSource = MeshSourceType::New();
        const float objectValue = static_cast<float>(1.0);
        meshSource->SetObjectValue( objectValue );
        meshSource->SetInput( p_padding_filter->GetOutput() );
        meshSource->Update();

        if (aSaveSegmentationFlag)
        {
            std::cout << "Nodes = " << meshSource->GetNumberOfNodes() << std::endl;
            std::cout << "Cells = " << meshSource->GetNumberOfCells() << std::endl;
            std::cout << "Max HU = " << (aMinHUValue < aMaxHUValue? aMaxHUValue: aMinHUValue) << std::endl;
            std::cout << "Min HU = " << (aMinHUValue < aMaxHUValue? aMinHUValue: aMaxHUValue) << std::endl;
            std::cout << "Average HU = " << anAverageHounsfielValue << std::endl << std::endl;
        }

        MeshType::Pointer p_mesh(meshSource->GetOutput());
        g_p_vertex_set[temp].resize(p_mesh->GetNumberOfPoints() * 3);
        g_p_index_set[temp].resize(p_mesh->GetNumberOfCells() * 3);

    // There are triangles
        if (meshSource->GetOutput()->GetNumberOfCells() && meshSource->GetOutput()->GetNumberOfPoints())
        {
            // Decimate the mesh by 75%
    /*        typedef itk::NumberOfFacesCriterion<MeshType> CriterionType;
            CriterionType::Pointer p_criterion(CriterionType::New());
            p_criterion->SetTopologicalChange(false);
            p_criterion->SetNumberOfElements(meshSource->GetNumberOfCells() * 0.25);

            typedef itk::QuadEdgeMeshSquaredEdgeLengthDecimation<MeshType, MeshType, CriterionType> DecimationType;
            DecimationType::Pointer p_decimate = DecimationType::New();
            p_decimate->SetInput(meshSource->GetOutput());
            p_decimate->SetCriterion(p_criterion);
        //    p_decimate->SetRelocate(true);
    */
    /*        try
            {
                p_decimate->Update();
                p_mesh = p_decimate->GetOutput();
            }
            catch( itk::ExceptionObject & e)
            {
                std::cerr << "ITK EXCEPTION: could not decimate the isosurface:\t" << std::endl << e << std::endl;
            }
            catch (std::exception& e)
            {
                std::cerr << "EXCEPTION: could not decimate the isosurface:\t" << std::endl << e.what() << std::endl;
            }
    */
            // The mesh is not empty
            if (p_mesh->GetNumberOfCells() && p_mesh->GetNumberOfPoints())
            {
                MeshType::PointsContainer* p_itk_point_set = p_mesh->GetPoints();
                MeshType::CellsContainer* p_itk_cell_set = p_mesh->GetCells();

                // Copy points
                for(MeshType::PointsContainer::Iterator ite(p_itk_point_set->Begin());
                    ite != p_itk_point_set->End();
                    ++ite)
                {
                    // Get the point index from the point container iterator
                    int idx = ite.Index();

                    // Get the point
                    MeshType::PointType p = ite.Value(); // access the point

                    // Copy the point
                    g_p_vertex_set[temp][idx * 3 + 0] = p[0] * mm;
                    g_p_vertex_set[temp][idx * 3 + 1] = p[1] * mm;
                    g_p_vertex_set[temp][idx * 3 + 2] = p[2] * mm;
                }

                typedef MeshType::CellType CellType;
                typedef itk::TriangleCell< CellType > TriangleType;
                typedef CellType::PointIdIterator PointIdIterator;

                for(MeshType::CellsContainer::Iterator ite(p_itk_cell_set->Begin());
                    ite != p_itk_cell_set->End();
                    ++ite)
                {
                    // Get the cell index from the point container iterator
                    int idx = ite.Index();

                    // Get the cell
                    CellType* p_cell = ite.Value();

                    // Copy point indices
                    int i(0);
                    for (PointIdIterator ite_point_idx(p_cell->PointIdsBegin());
                        ite_point_idx != p_cell->PointIdsEnd();
                        ++ite_point_idx, ++i)
                    {
                        unsigned int point_id = *ite_point_idx;
                        g_p_index_set[temp][idx * 3 + i] = point_id;
                    }
                }
            }
        }
    }
    else
    {
        //std::cerr << "WARNING: thresholds HU " << aMinHUValue << " and " << aMaxHUValue << ": " << voxel_count << " voxels of " << number_of_voxels << ", i.e " << 100.0 * voxel_count / number_of_voxels << "\% of the CT." << std::endl;

    }
}


//---------------------------------------------------------------------------------------
double computeMetrics(const std::vector<RealImageType::Pointer>& aReferenceProjectionSet,
                      const std::vector<RealImageType::Pointer>& aSimulatedProjectionSet)
//---------------------------------------------------------------------------------------
{
    double metrics2(0);

    if (!aReferenceProjectionSet.size() || !aSimulatedProjectionSet.size() || aReferenceProjectionSet.size() != aSimulatedProjectionSet.size())
    {
        std::stringstream error_message;
        error_message << "Incorrect number of projections (references: " << aReferenceProjectionSet.size() << ", simulated: " << aSimulatedProjectionSet.size() << ")";
        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
    }

    int i;
/*#ifdef HAS_OPENMP
#pragma omp parallel for \
            default(none) \
            private(i) \
            shared(aReferenceProjectionSet, aSimulatedProjectionSet) \
            reduction(+: metrics2)
#endif
*/    for (i = 0; i < aReferenceProjectionSet.size(); ++i)
    {/*
        typedef itk::MeanSquaresImageToImageMetric < RealImageType , RealImageType >
                                                       MetricType;
        typedef itk::LinearInterpolateImageFunction<RealImageType, double >
                                                       InterpolatorType;
        typedef itk::IdentityTransform < double , 3 > TransformType;

        MetricType::Pointer metric = MetricType::New();
        TransformType::Pointer transform = TransformType::New();

        InterpolatorType::Pointer interpolator = InterpolatorType::New();
        interpolator->SetInputImage( aReferenceProjectionSet[i] );

        metric->SetFixedImage( aReferenceProjectionSet[i] );
        metric->SetMovingImage( aSimulatedProjectionSet[i] );
        metric->SetFixedImageRegion( aReferenceProjectionSet[i]->GetLargestPossibleRegion() );
        metric->SetTransform( transform );
        metric->SetInterpolator( interpolator );

        TransformType::ParametersType params(transform->GetNumberOfParameters());
        params.Fill(0.0);

        metric->Initialize();
        metrics2 += metric->GetValue( params );*/


        typedef itk::ImageRegionIterator<RealImageType> ImageIterator;

        RealImageType::RegionType reference_region(aReferenceProjectionSet[i]->GetLargestPossibleRegion());
        RealImageType::RegionType test_region(aSimulatedProjectionSet[i]->GetLargestPossibleRegion());

        ImageIterator reference_image_ite(aReferenceProjectionSet[i], reference_region);
        ImageIterator test_image_ite(aSimulatedProjectionSet[i], test_region);

        for (reference_image_ite.GoToBegin(), test_image_ite.GoToBegin();
                !reference_image_ite.IsAtEnd() && !test_image_ite.IsAtEnd();
                ++reference_image_ite, ++test_image_ite)
        {
            double temp(reference_image_ite.Get() - test_image_ite.Get());
            metrics2 += temp * temp;
        }
    }

    metrics2 /= double(aReferenceProjectionSet.size());
    return (metrics2);
}


void checkThreshold(short& aThreshold)
{
    if (aThreshold < g_min_value) aThreshold = g_min_value;
    if (aThreshold > g_max_value) aThreshold = g_max_value;
}


double my_f (const std::vector<short>& v, void *params)
{
    void** p_param_set;
    p_param_set = (void**)params;

    const RealImageType::Pointer* p_volume(reinterpret_cast<const RealImageType::Pointer*>(p_param_set[0]));
    const std::vector<RealImageType::Pointer>* p_reference_set(reinterpret_cast<const std::vector<RealImageType::Pointer>*>(p_param_set[1]));
    std::vector<RealImageType::Pointer>* p_simulated_set(reinterpret_cast<std::vector<RealImageType::Pointer>*>(p_param_set[2]));

    // Get the parameters
    g_p_threshold_set = v;
    std::sort(g_p_threshold_set.begin(), g_p_threshold_set.end());

    // Set the iso-surfaces
    loadPoygonMeshes(*p_volume, false);

    // Compute the x-ray images
    g_is_xray_image_up_to_date = false;
    updateXRayImage(*p_simulated_set);

    // Compute the metrics
    double metrics(computeMetrics(*p_reference_set, *p_simulated_set));
    //std::cout << metrics << std::endl;
    return (metrics);
}


#ifdef HAS_GSL
double my_f (const gsl_vector *v, void *params)
{
    // Get the parameters
    std::vector<short> threshold_set;
    for (int i(0); i < g_p_threshold_set.size(); ++i)
    {
        threshold_set.push_back(gsl_vector_get (v, i));
        checkThreshold(threshold_set[i]);
    }

    return (my_f(threshold_set, params));
}
#endif // HAS_GSL


void writeProjections(std::vector<RealImageType::Pointer>& aSimulatedProjectionSet)
{
    std::string file_name_prefix;
    if (g_output_file_name.size())
    {
        file_name_prefix = g_output_file_name + "/";
    }

    int energy(0);
    for (std::vector<double>::const_iterator ite_energy(g_p_energy_set.begin());
            ite_energy != g_p_energy_set.end();
            ++ite_energy, ++energy)
    {
        // Write the filter data to a file
        ImageFileWriterType::Pointer p_writer(ImageFileWriterType::New());

        std::stringstream file_name;
        file_name << file_name_prefix;
        if (&g_p_low_resolution_reference_projection_set == &aSimulatedProjectionSet)
        {
            file_name << "low_";
            p_writer->SetInput(aSimulatedProjectionSet[energy]);
        }
        else
        {
            file_name << "high_";
            p_writer->SetInput(aSimulatedProjectionSet[energy]);
        }
        file_name << "resolution_simulated_projection_" << g_p_energy_set[energy] / keV << "keV.mhd";
        p_writer->SetFileName(file_name.str());
        p_writer->Update();
    }
}


int nmsimplexSearch(const RealImageType::Pointer& aVolume,
                    const std::vector<RealImageType::Pointer>& aReferenceProjectionSet,
                    std::vector<RealImageType::Pointer>& aSimulatedProjectionSet)
{
#ifdef HAS_GSL
    // The parameter vector for the function defn: UNUSED
    const void* par[3];
    par[0] = &aVolume;
    par[1] = &aReferenceProjectionSet;
    par[2] = &aSimulatedProjectionSet;

    size_t np = g_p_threshold_set.size();

    gsl_multimin_fminimizer *s ;
    gsl_vector *ss, *x;
    gsl_multimin_function ex4_fn;
    size_t iter = 0, i;
    int status;
    double size;

    // Initial vertex size vector
    ss = gsl_vector_alloc (np);

    // Set all step sizes to 10% of voxel range
    gsl_vector_set_all (ss, (g_max_value - g_min_value) * 0.10);

    // Starting point
    x = gsl_vector_alloc (np);
    for (int i(0); i < g_p_threshold_set.size(); ++i)
    {
        gsl_vector_set (x, i, g_p_threshold_set[i]);
    }

    // Initialize method and iterate
    ex4_fn.f = &my_f;
    ex4_fn.n = np;
    ex4_fn.params = (void *)&par;
    s = gsl_multimin_fminimizer_alloc (gsl_multimin_fminimizer_nmsimplex, np);
    gsl_multimin_fminimizer_set (s, &ex4_fn, x, ss);

    do
    {
        iter++;
        status = gsl_multimin_fminimizer_iterate(s);
        if (status)
        {
            break;
        }

        size = gsl_multimin_fminimizer_size (s);
        status = gsl_multimin_test_size (size, 1e-3);
        if (status == GSL_SUCCESS)
        {
            printf ("converged to minimum at\n");
        }

        printf ("%5d ", iter);
        for (i = 0; i < np; i++)
        {
            short param(gsl_vector_get (s->x, i));
            g_p_threshold_set[i] = param;
        }
        std::sort(g_p_threshold_set.begin(), g_p_threshold_set.end());

        for (i = 0; i < np; i++)
        {
            printf ("%5d ", g_p_threshold_set[i]);
        }

        double metrics(s->fval);
        printf ("f() = %7.6f size = %.3f\n", metrics, size);

        // Set the iso-surfaces
        loadPoygonMeshes(aVolume, false);

        // Compute the x-ray images
        updateXRayImage(aSimulatedProjectionSet);

        // Write the x-ray images
        writeProjections(aSimulatedProjectionSet);

        // Render here
        displayCallback();

        // Swap front and back buffers
        glfwSwapBuffers(g_p_window_id);

        // Poll for and process events
        glfwPollEvents();
    }
    while (status == GSL_CONTINUE && iter < g_max_iteration);

    gsl_vector_free(x);
    gsl_vector_free(ss);
    gsl_multimin_fminimizer_free (s);
#endif // HAS_GSL
}


int nmsimplex2Search(const RealImageType::Pointer& aVolume,
                    const std::vector<RealImageType::Pointer>& aReferenceProjectionSet,
                    std::vector<RealImageType::Pointer>& aSimulatedProjectionSet)
{
#ifdef HAS_GSL
    // The parameter vector for the function defn: UNUSED
    const void* par[3];
    par[0] = &aVolume;
    par[1] = &aReferenceProjectionSet;
    par[2] = &aSimulatedProjectionSet;

    size_t np = g_p_threshold_set.size();

    gsl_multimin_fminimizer *s ;
    gsl_vector *ss, *x;
    gsl_multimin_function ex4_fn;
    size_t iter = 0, i;
    int status;
    double size;

    // Initial vertex size vector
    ss = gsl_vector_alloc (np);

    // Set all step sizes to 10% of voxel range
    gsl_vector_set_all (ss, (g_max_value - g_min_value) * 0.10);

    // Starting point
    x = gsl_vector_alloc (np);
    for (int i(0); i < g_p_threshold_set.size(); ++i)
    {
        gsl_vector_set (x, i, g_p_threshold_set[i]);
    }

    // Initialize method and iterate
    ex4_fn.f = &my_f;
    ex4_fn.n = np;
    ex4_fn.params = (void *)&par;
    s = gsl_multimin_fminimizer_alloc (gsl_multimin_fminimizer_nmsimplex2, np);
    gsl_multimin_fminimizer_set (s, &ex4_fn, x, ss);

    do
    {
        iter++;
        status = gsl_multimin_fminimizer_iterate(s);
        if (status)
        {
            break;
        }

        size = gsl_multimin_fminimizer_size (s);
        status = gsl_multimin_test_size (size, 1e-3);
        if (status == GSL_SUCCESS)
        {
            printf ("converged to minimum at\n");
        }

        printf ("%5d ", iter);
        for (i = 0; i < np; i++)
        {
            short param(gsl_vector_get (s->x, i));
            g_p_threshold_set[i] = param;
        }
        std::sort(g_p_threshold_set.begin(), g_p_threshold_set.end());

        for (i = 0; i < np; i++)
        {
            printf ("%5d ", g_p_threshold_set[i]);
        }

        double metrics(s->fval);
        printf ("f() = %7.6f size = %.3f\n", metrics, size);

        // Set the iso-surfaces
        loadPoygonMeshes(aVolume, false);

        // Compute the x-ray images
        updateXRayImage(aSimulatedProjectionSet);

        // Write the x-ray images
        writeProjections(aSimulatedProjectionSet);

        // Render here
        displayCallback();

        // Swap front and back buffers
        glfwSwapBuffers(g_p_window_id);

        // Poll for and process events
        glfwPollEvents();
    }
    while (status == GSL_CONTINUE && iter < g_max_iteration);

    gsl_vector_free(x);
    gsl_vector_free(ss);
#endif // HAS_GSL
}


int nmsimplex2randSearch(const RealImageType::Pointer& aVolume,
                    const std::vector<RealImageType::Pointer>& aReferenceProjectionSet,
                    std::vector<RealImageType::Pointer>& aSimulatedProjectionSet)
{
#ifdef HAS_GSL
    // The parameter vector for the function defn: UNUSED
    const void* par[3];
    par[0] = &aVolume;
    par[1] = &aReferenceProjectionSet;
    par[2] = &aSimulatedProjectionSet;

    size_t np = g_p_threshold_set.size();

    gsl_multimin_fminimizer *s ;
    gsl_vector *ss, *x;
    gsl_multimin_function ex4_fn;
    size_t iter = 0, i;
    int status;
    double size;

    // Initial vertex size vector
    ss = gsl_vector_alloc (np);

    // Set all step sizes to 10% of voxel range
    gsl_vector_set_all (ss, (g_max_value - g_min_value) * 0.10);

    // Starting point
    x = gsl_vector_alloc (np);
    for (int i(0); i < g_p_threshold_set.size(); ++i)
    {
        gsl_vector_set (x, i, g_p_threshold_set[i]);
    }

    // Initialize method and iterate
    ex4_fn.f = &my_f;
    ex4_fn.n = np;
    ex4_fn.params = (void *)&par;
    s = gsl_multimin_fminimizer_alloc (gsl_multimin_fminimizer_nmsimplex2rand, np);
    gsl_multimin_fminimizer_set (s, &ex4_fn, x, ss);

    do
    {
        iter++;
        status = gsl_multimin_fminimizer_iterate(s);
        if (status)
        {
            break;
        }

        size = gsl_multimin_fminimizer_size (s);
        status = gsl_multimin_test_size (size, 1e-3);
        if (status == GSL_SUCCESS)
        {
            printf ("converged to minimum at\n");
        }

        printf ("%5d ", iter);
        for (i = 0; i < np; i++)
        {
            short param(gsl_vector_get (s->x, i));
            g_p_threshold_set[i] = param;
        }
        std::sort(g_p_threshold_set.begin(), g_p_threshold_set.end());

        for (i = 0; i < np; i++)
        {
            printf ("%5d ", g_p_threshold_set[i]);
        }

        double metrics(s->fval);
        printf ("f() = %7.6f size = %.3f\n", metrics, size);

        // Set the iso-surfaces
        loadPoygonMeshes(aVolume, false);

        // Compute the x-ray images
        updateXRayImage(aSimulatedProjectionSet);

        // Write the x-ray images
        writeProjections(aSimulatedProjectionSet);

        // Render here
        displayCallback();

        // Swap front and back buffers
        glfwSwapBuffers(g_p_window_id);

        // Poll for and process events
        glfwPollEvents();
    }
    while (status == GSL_CONTINUE && iter < g_max_iteration);

    gsl_vector_free(x);
    gsl_vector_free(ss);
#endif // HAS_GSL
}


//-----------------------------------------------
void evaluate(std::vector<short>& apThresholdSet,
        double& aFitness,
        std::vector<short>& aBestParamSet,
        double& aBestFitness,
        const void** par)
//-----------------------------------------------
{
    // Call the fitness function
    aFitness = my_f(apThresholdSet, par);

    // Check the fitness function
    if (aBestFitness > aFitness)
    {
        aBestFitness = aFitness;
        aBestParamSet = apThresholdSet;

        // Render here
        displayCallback();

        // Swap front and back buffers
        glfwSwapBuffers(g_p_window_id);
    }
}


//-------------------------------------------------------------------------------------
int randomSearch(const RealImageType::Pointer& aVolume,
                    const std::vector<RealImageType::Pointer>& aReferenceProjectionSet,
                    std::vector<RealImageType::Pointer>& aSimulatedProjectionSet)
//-------------------------------------------------------------------------------------
{
    // The parameter vector for the function defn: UNUSED
    const void* par[3];
    par[0] = &aVolume;
    par[1] = &aReferenceProjectionSet;
    par[2] = &aSimulatedProjectionSet;

    std::vector<short> p_best_param(g_p_threshold_set);
    double best_fitness(DBL_MAX);

    size_t np = g_p_threshold_set.size();

    size_t iter(0);
    do
    {
        iter++;

        double fitness(0);
        evaluate(g_p_threshold_set, fitness, p_best_param, best_fitness, par);

        // Print the results
        printf ("%5d ", iter);
        for (int i(0); i < np; i++)
        {
            printf ("%5d ", p_best_param[i]);
        }

        printf ("f() = %7.6f\n", best_fitness);

        // Generate random parameters
        for (int i(0); i < np; i++)
        {
            double param(double(rand()) / double(RAND_MAX));
            param *= g_max_value - g_min_value;
            param += g_min_value;

            g_p_threshold_set[i] = short(floor(param));
        }

        if (fitness == best_fitness)
        {
            // Write the x-ray images
            writeProjections(aSimulatedProjectionSet);
        }

        // Poll for and process events
        glfwPollEvents();
    }
    while (iter < g_max_iteration);

    g_p_threshold_set = p_best_param;

    return (0);
}

/*
//--------------------------------------------------------------
int selectIndividualToKill(const std::vector<double>& p_fitness)
//--------------------------------------------------------------
{
    int to_kill(0);
    int to_kill_1(0);
    int to_kill_2(0);

    while (to_kill_1 == to_kill_2)
    {
        to_kill_1 = rand() % g_p_parent_population.size();
        to_kill_2 = rand() % g_p_parent_population.size();

        double fitness_1(p_fitness[to_kill_1]);
        double fitness_2(p_fitness[to_kill_2]);
        if (fitness_1 > fitness_2)
        {
            to_kill = to_kill_1;
        }
        else
        {
            to_kill = to_kill_2;
        }
    }

    return (to_kill);
}
*/

//-------------------------------------------------------------------
int selectIndividualToReproduce(const std::vector<double>& p_fitness)
//-------------------------------------------------------------------
{
    int to_reproduce(0);
    int to_reproduce_1(0);
    int to_reproduce_2(0);

    while (to_reproduce_1 == to_reproduce_2)
    {
        to_reproduce_1 = rand() % g_p_parent_population.size();
        to_reproduce_2 = rand() % g_p_parent_population.size();

        double fitness_1(p_fitness[to_reproduce_1]);
        double fitness_2(p_fitness[to_reproduce_2]);
        if (fitness_1 < fitness_2)
        {
            to_reproduce = to_reproduce_1;
        }
        else
        {
            to_reproduce = to_reproduce_2;
        }
    }

    return (to_reproduce);
}


//-------------------------------------------------
void mutation(int anIndividualToKill,
              const std::vector<double>& p_fitness)
//-------------------------------------------------
{
    int individual_to_reproduce;

    do
    {
        individual_to_reproduce = selectIndividualToReproduce(p_fitness);
    }
    while (individual_to_reproduce == anIndividualToKill);

    // Mutation of each gene
    for (int gene(0); gene < g_p_threshold_set.size(); gene++)
    {
        double delta(double(rand()) / double(RAND_MAX));
        delta *= 0.01 * (g_max_value - g_min_value);

        if (double(rand()) / double(RAND_MAX) >= 0.5)
        {
            g_p_child_population[anIndividualToKill][gene] = g_p_parent_population[individual_to_reproduce][gene] + delta;
        }
        else
        {
            g_p_child_population[anIndividualToKill][gene] = g_p_parent_population[individual_to_reproduce][gene] - delta;
        }
    }
}


//--------------------------------------------------
void crossOver(int anIndividualToKill,
               const std::vector<double>& p_fitness)
//--------------------------------------------------
{
    int individual_to_reproduce_1;
    int individual_to_reproduce_2;

    do
    {
        individual_to_reproduce_1 = selectIndividualToReproduce(p_fitness);
        individual_to_reproduce_2 = selectIndividualToReproduce(p_fitness);
    }
    while (individual_to_reproduce_1 == anIndividualToKill || individual_to_reproduce_2 == anIndividualToKill);

    // Crossover of each gene
    for (int gene(0); gene < g_p_threshold_set.size(); gene++)
    {
        double t(double(rand()) / double(RAND_MAX));

        g_p_child_population[anIndividualToKill][gene] = t * g_p_parent_population[individual_to_reproduce_1][gene] +
                (t - 1.0) * g_p_parent_population[individual_to_reproduce_2][gene];
    }
}


//--------------------------------------
void immigration(int anIndividualToKill)
//--------------------------------------
{
    for (int i(0); i < g_p_threshold_set.size(); i++)
    {
        double param(double(rand()) / double(RAND_MAX));
        param *= g_max_value - g_min_value;
        param += g_min_value;

        g_p_child_population[anIndividualToKill][i] = floor(param);
    }
}


//-----------------------------------------------------------------------------------
int geneticSearch(const RealImageType::Pointer& aVolume,
                  const std::vector<RealImageType::Pointer>& aReferenceProjectionSet,
                  std::vector<RealImageType::Pointer>& aSimulatedProjectionSet)
//-----------------------------------------------------------------------------------
{
    // The parameter vector for the function defn: UNUSED
    const void* par[3];
    par[0] = &aVolume;
    par[1] = &aReferenceProjectionSet;
    par[2] = &aSimulatedProjectionSet;

    std::vector<short> p_best_param(g_p_threshold_set);
    std::vector<double> p_parent_fitness(g_pop_size);
    std::vector<double> p_child_fitness(g_pop_size);
    double best_fitness(DBL_MAX);

    size_t np = g_p_threshold_set.size();

    // Starting point
    std::vector<short> x(np);

    // Set the initial population
    g_p_parent_population.clear();
    g_p_parent_population.resize(g_pop_size, std::vector<short>(g_p_threshold_set.size()));

    g_p_child_population.clear();
    g_p_child_population.resize(g_pop_size, std::vector<short>(g_p_threshold_set.size()));

    // Add the default individual
    g_p_parent_population[0] = g_p_threshold_set;
    evaluate(g_p_parent_population[0], p_parent_fitness[0], p_best_param, best_fitness, par);

    // Add random individuals
    for (int j(1); j < g_pop_size; ++j)
    {
        immigration(j);

        evaluate(g_p_parent_population[j], p_parent_fitness[j], p_best_param, best_fitness, par);
    }


    size_t iter(0);
    do
    {
        iter++;

        // Elitism
        g_p_child_population[0] = p_best_param;
        p_child_fitness[0] = best_fitness;

        for (int population(1); population < g_pop_size; ++population)
        {
            // Individual to kill
            //int to_kill(selectIndividualToKill(p_fitness));

            // Select the operator
            double probability(double(rand()) / double(RAND_MAX));

            // Immigration
            if (probability <= g_immigration_probability)
            {
                immigration(population);
            }
            // Crossover
            else if (probability <= g_immigration_probability + g_crossover_probability)
            {
                crossOver(population, p_parent_fitness);
            }
            // Mutation
            else
            {
                mutation(population, p_parent_fitness);
            }

            // Compute the fitness of the new individual
            evaluate(g_p_child_population[population], p_child_fitness[population], p_best_param, best_fitness, par);

            if (p_child_fitness[population] == best_fitness)
            {
                // Write the x-ray images
                writeProjections(aSimulatedProjectionSet);
            }
        }

        p_parent_fitness = p_child_fitness;
        g_p_parent_population = g_p_child_population;

        // Print the results
        printf ("%5d ", iter);
        for (int i(0); i < np; i++)
        {
            printf ("%5d ", p_best_param[i]);
        }

        printf ("f() = %7.6f\n", best_fitness);

        // Poll for and process events
        glfwPollEvents();
    }
    while (iter < g_max_iteration);

    g_p_threshold_set = p_best_param;

    return (0);
}


//------------------------------------------------------------------------------
int optimise(const RealImageType::Pointer& aVolume,
             const std::vector<RealImageType::Pointer>& aReferenceProjectionSet,
             std::vector<RealImageType::Pointer>& aSimulatedProjectionSet)
//------------------------------------------------------------------------------
{
    loadDetector(aReferenceProjectionSet.front(), aVolume);

    if (g_optimizer == "nmsimplex")
    {
        return (nmsimplexSearch(aVolume, aReferenceProjectionSet, aSimulatedProjectionSet));
    }
    else if (g_optimizer == "nmsimplex2")
    {
        return (nmsimplex2Search(aVolume, aReferenceProjectionSet, aSimulatedProjectionSet));
    }
    else if (g_optimizer == "nmsimplex2rand")
    {
        return (nmsimplex2randSearch(aVolume, aReferenceProjectionSet, aSimulatedProjectionSet));
    }
    else if (g_optimizer == "genetic")
    {
        return (geneticSearch(aVolume, aReferenceProjectionSet, aSimulatedProjectionSet));
    }
    else
    {
        return (randomSearch(aVolume, aReferenceProjectionSet, aSimulatedProjectionSet));
    }

    std::sort(g_p_threshold_set.begin(), g_p_threshold_set.end());

    // Set the iso-surfaces
    loadPoygonMeshes(aVolume, true);

    // Compute the x-ray images
    updateXRayImage(aSimulatedProjectionSet);

    // Write the x-ray images
    writeProjections(aSimulatedProjectionSet);

    // Render here
    displayCallback();

    // Swap front and back buffers
    glfwSwapBuffers(g_p_window_id);
}

