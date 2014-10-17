/**
********************************************************************************
*
*	@file		validation_03.cxx
*
*	@brief		Main test program.
*
*	@version	1.0
*
*	@date		11/11/2013
*
*	@author		Dr Franck P. Vidal
*
*	@section	License
*				BSD 3-Clause License.
*
*				For details on use and redistribution please refer
*				to http://opensource.org/licenses/BSD-3-Clause
*
*	@section	Copyright
*				(c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*				http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
*				BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#if defined(_WIN32) || defined(_WIN64)
#include <GL/glew.h>
#endif

#include <cmath>
#include <iomanip>

#include <boost/scoped_ptr.hpp>

#ifdef __APPLE__
#define GL3_PROTOTYPES 1
#else
#define GL_GLEXT_PROTOTYPES 1
#endif

#define GLFW_INCLUDE_GLCOREARB 1

#include <GLFW/glfw3.h>

#include <stdlib.h>

#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#ifndef GVXR_POLYGON_MESH_H
#include "gVirtualXRay/PolygonMesh.h"
#endif

#ifndef GVXR_CUBE_MESH_H
#include "gVirtualXRay/CubeMesh.h"
#endif

#ifndef GVXR_XRAY_BEAM_H
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef GVXR_XRAY_DETECTOR_H
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef GVXR_MATRIX4X4_H
#include "gVirtualXRay/Matrix4x4.h"
#endif

#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef GVXR_SHADER_H
#include "gVirtualXRay/Shader.h"
#endif

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//	Defines
//******************************************************************************
#define PREFIX ".."


//******************************************************************************
//	Global variables
//******************************************************************************
GLsizei g_main_window_width(1280 / 2.0);
GLsizei g_main_window_height(800 / 2.0);
GLFWwindow* g_p_main_window_id(0);

Matrix4x4<GLfloat> g_scene_rotation_matrix;
Matrix4x4<GLfloat> g_detector_rotation_matrix;
Matrix4x4<GLfloat> g_sample_rotation_matrix;

GLfloat g_incident_energy(80.0 * keV);
const GLfloat g_scaling_factor(2.0);
GLfloat g_resolution(0.15 * mm * g_scaling_factor); // 1mm per pixel
Vec2ui g_number_of_pixels(600 / g_scaling_factor, 1);
VEC3 g_source_position(   0.0, 0.0, -10.0 * cm);
VEC3 g_detector_position( 0.0, 0.0,  10.0 * cm);
VEC3 g_detector_up_vector(0.0, 1.0,   0.0);

PolygonMesh g_inside_polygon_data;
CubeMesh g_outside_polygon_data(3.0 * cm);
boost::scoped_ptr<VBO> g_intersection_box_detector_vbo;

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;

RATIONAL_NUMBER g_literature_result(0.0);
RATIONAL_NUMBER g_expected_result(0.0);

//******************************************************************************
//	Function declaration
//******************************************************************************
void initGLEW();
void initGL();
void loadSTLFile();
void loadDetector();
void loadSource();
void loadXRaySimulator();
void errorCallback(int error, const char* description);


//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
    // Make sure the detector has an odd number of columns
    if ((g_number_of_pixels.getX() % 2) == 0)
    {
        g_number_of_pixels.setX(g_number_of_pixels.getX() + 1);
    }

    // Make sure the detector has an odd number of rows
    if ((g_number_of_pixels.getY() % 2) == 0)
    {
        g_number_of_pixels.setY(g_number_of_pixels.getY() + 1);
    }

	try
	{
	    // Set an error callback
	    glfwSetErrorCallback(errorCallback);

		// Initialize GLFW
	    if (!glfwInit())
	    {
	    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: cannot initialise GLFW.");
	    }

	    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	    // Enable quad-buffering if possible
	    glfwWindowHint(GLFW_STEREO, GL_TRUE);

	    // Enable anti-aliasing
	    glfwWindowHint(GLFW_SAMPLES, 4);

	    // Create a windowed mode window and its OpenGL context
	    g_p_main_window_id = glfwCreateWindow(g_main_window_width, g_main_window_height, "GPU X-Ray Demo", NULL, NULL);
	   
        // Window cannot be created 
        if (!g_p_main_window_id)
	    {
            // Disable quad-buffering
            glfwWindowHint(GLFW_STEREO, GL_FALSE);

            // Create the window
	        g_p_main_window_id = glfwCreateWindow(g_main_window_width, g_main_window_height, "GPU X-Ray Demo", NULL, NULL);
        }
        
        // Window cannot be created 
        if (!g_p_main_window_id)
	    {
	        glfwTerminate();
	    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: cannot create a GLFW windowed mode window and its OpenGL context.");
	    }

	    // Make the window's context current
	    glfwMakeContextCurrent(g_p_main_window_id);

        // Initialise GLEW
        initGLEW();

        // Initialise OpenGL
		initGL();

		// Check the current FBO
		checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Load the data
		loadDetector();
		loadSource();
		loadXRaySimulator();

		// Add the geometry to the X-ray renderer
		loadSTLFile();
		g_xray_renderer.addInnerSurface(&g_inside_polygon_data);
		g_xray_renderer.addOuterSurface(&g_outside_polygon_data);

		// Set the projection matrix
		GLint width(0);
		GLint height(0);
		glfwGetFramebufferSize(g_p_main_window_id, &width, &height);

		// Compute the X-ray image
	    g_xray_renderer.computeImage(g_sample_rotation_matrix);
	    RATIONAL_NUMBER* energy_fluence(g_xray_renderer.getEnergyFluence());
	    RATIONAL_NUMBER central_pixel(energy_fluence[g_number_of_pixels.getX() / 2]);

	    RATIONAL_NUMBER simulated_result(central_pixel);
	    std::cout << "On GPU, the energy (in MeV) transmitted orthogonally throw the middle of cube and cylinder actually is:" << std::endl;
	    std::cout << "\t\t           = " << std::scientific << simulated_result / MeV << " MeV" << std::endl << std::endl;

	    std::cout << "The absolute error is: |I_expected - I_reference| MeV" << std::endl;
	    std::cout << "                     : |" << std::scientific << simulated_result / MeV << " - " << g_literature_result / MeV << "| MeV" << std::endl;
	    std::cout << "                     : " << std::fixed << std::abs(simulated_result - g_literature_result) / MeV << " MeV" << std::endl;
	}
	// Catch exception if any
	catch (const std::exception& error)
	{
		std::cerr << error.what() << std::endl;
	}

	// Close the window and shut GLFW
	if (g_p_main_window_id)
	{
		glfwDestroyWindow(g_p_main_window_id);
		g_p_main_window_id = 0;
		glfwTerminate();
	}

	// Return an exit code
	return (EXIT_SUCCESS);
}


//----------------------------------------------------
void errorCallback(int error, const char* description)
//----------------------------------------------------
{
	std::cerr << "GLFW error: " << description << std::endl;
}


//---------
void quit()
//---------
{
	if (g_p_main_window_id)
	{
		glfwDestroyWindow(g_p_main_window_id);
        g_p_main_window_id = 0;
        glfwTerminate();
	}
}


//-------------
void initGLEW()
//-------------
{
#if defined(_WIN32) || defined(_WIN64)
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::stringstream error_message;
		error_message << "ERROR: cannot initialise GLEW:\t" << glewGetErrorString(err);

        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
	}
#endif
}


//-----------
void initGL()
//-----------
{
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Enable the Z-buffer
	glEnable(GL_DEPTH_TEST);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Set the background colour
	glClearColor(0.5, 0.5, 0.5, 1.0);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	glEnable(GL_MULTISAMPLE);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//----------------
void loadSTLFile()
//----------------
{
	std::string stl_filename("cylinder_bin.stl");

	// Open the file
	FILE* p_file_descriptor(fopen(stl_filename.data(), "rb"));

	// The file is not in the same directory as the executable
	if (!p_file_descriptor)
	{
		// Use the prefix
		stl_filename = "../data/" + stl_filename;
	}
	else
	{
		fclose(p_file_descriptor);
	}

	// HU values
    RATIONAL_NUMBER HU_water(0.0);
    RATIONAL_NUMBER HU_bone(1329.885683);
    RATIONAL_NUMBER HU_soft_tissue(51.714752);

	// Set geometry
	g_inside_polygon_data.setFilename(stl_filename.data());
	g_inside_polygon_data.loadSTLFile(true, true, true, true, cm, GL_STATIC_DRAW);
	g_inside_polygon_data.mergeVertices();
    g_inside_polygon_data.setHounsfieldValue(HU_bone);

	//g_outside_polygon_data.setFilename(stl_filename_1.data());
	//g_outside_polygon_data.loadSTLFile(true, true, true, true, cm, GL_STATIC_DRAW);
	//g_outside_polygon_data.mergeVertices(true);
    g_outside_polygon_data.setHounsfieldValue(HU_soft_tissue);

    std::cout << "We run the simulation as follows:" << std::endl;
    std::cout << "- A cube, which has edge length of 3 cm, is made of soft tissue (HU = " << std::floor(HU_soft_tissue + 0.5) << "). " << std::endl;
    std::cout << "- Inside, a cylinder is inserted. " <<
            "It is made of bone (HU = " << std::floor(HU_bone + 0.5) << "), its height is 3 cm and its diameter is 2 cm." << std::endl;
    std::cout << "- We consider the energy of the incident beam is " << g_incident_energy / keV << " keV (i.e. " << g_incident_energy / MeV << " MeV). " << std::endl << std::endl;

    std::cout << "From the literature, we know:" << std::endl;
    std::cout.precision(3);
    std::cout <<
            "HU =    "     << std::fixed << HU_water << "\t" <<
            "rho = "    << std::scientific << 1.0 << " g/cm3,\t" <<
            "mu/rho = " << std::scientific << 1.837E-01 << " cm2/g,\t" <<
            "mu = "     << std::scientific << 1.837E-01 << " cm-1" << std::endl;

    std::cout <<
            "HU = "     << std::fixed << HU_bone << "\t" <<
            "rho = "    << std::scientific << 1.920E+00 << " g/cm3,\t" <<
            "mu/rho = " << std::scientific << 2.229E-01 << " cm2/g,\t" <<
            "mu = "     << std::scientific << 4.280E-01 << " cm-1" << std::endl;

    std::cout <<
            "HU =   "     << std::fixed << HU_soft_tissue << "\t" <<
            "rho = "    << std::scientific << 1.060E+00 << " g/cm3,\t" <<
            "mu/rho = " << std::scientific << 1.823E-01 << " cm2/g,\t" <<
            "mu = "     << std::scientific << 1.932E-01 << " cm-1" << std::endl << std::endl;

    g_literature_result = g_incident_energy * exp(-(4.280e-01 * 2 + 1.932e-01 * 1));
    std::cout << "The energy (in MeV) transmitted orthogonally throw the middle of cube and cylinder should be:" << std::endl;
    std::cout << "\t\tI_reference = I_0 * exp(-Sum(mu * x))" << std::endl;
    std::cout << "\t\t            = " << std::fixed << g_incident_energy / MeV << " * exp(-(mu_bone * 2 + mu_soft_tissue * 1))" << std::endl;
    std::cout << "\t\t            = " << g_incident_energy / MeV << " * exp(-(4.280e-01 * 2 + 1.932e-01 * 1))" << std::endl;
    std::cout << "\t\t            = " << std::scientific << g_literature_result / MeV << " MeV" << std::endl << std::endl;

    std::cout << "To obtain linear attenuation coefficients (mu values) in our implementation, densities (rho values) and mass attenuation coefficients (mu/rho values) are estimated as follows:" << std::endl;
    std::cout <<
            "HU =    "     << std::fixed << HU_water << "\t" <<
            "rho = "    << std::scientific << g_xray_renderer.getDensityFromHU(HU_water)  / (g/cm3) << " g/cm3,\t" <<
            "mu/rho = " << std::scientific << g_xray_renderer.getMassAttenuationFromHU(HU_water, g_incident_energy)  / (cm2/g) << " cm2/g,\t" <<
            "mu = "     << std::scientific << g_xray_renderer.getMuFromHU(HU_water, g_incident_energy) / (1.0 / cm) << " cm-1" << std::endl;

    std::cout <<
            "HU = "     << std::fixed << HU_bone << "\t" <<
            "rho = "    << std::scientific << g_xray_renderer.getDensityFromHU(HU_bone)  / (g/cm3) << " g/cm3,\t" <<
            "mu/rho = " << std::scientific << g_xray_renderer.getMassAttenuationFromHU(HU_bone, g_incident_energy)  / (cm2/g) << " cm2/g,\t" <<
            "mu = "     << std::scientific << g_xray_renderer.getMuFromHU(HU_bone, g_incident_energy) / (1.0 / cm) << " cm-1" << std::endl;

    std::cout <<
            "HU =   "     << std::fixed << HU_soft_tissue << "\t" <<
            "rho = "    << std::scientific << g_xray_renderer.getDensityFromHU(HU_soft_tissue)  / (g/cm3) << " g/cm3,\t" <<
            "mu/rho = " << std::scientific << g_xray_renderer.getMassAttenuationFromHU(HU_soft_tissue, g_incident_energy)  / (cm2/g) << " cm2/g,\t" <<
            "mu = "     << std::scientific << g_xray_renderer.getMuFromHU(HU_soft_tissue, g_incident_energy) / (1.0 / cm) << " cm-1" << std::endl << std::endl;

    g_expected_result = g_incident_energy * exp(-(g_xray_renderer.getMuFromHU(HU_bone, g_incident_energy) / (1.0 / cm) * 2 + g_xray_renderer.getMuFromHU(HU_soft_tissue, g_incident_energy) / (1.0 / cm) * 1));
    std::cout << "The energy (in MeV) transmitted orthogonally throw the middle of cube and cylinder should be:" << std::endl;
    std::cout << "\t\tI_expected = I_0 * exp(-Sum(mu * x))" << std::endl;
    std::cout << "\t\t           = " << std::fixed << g_incident_energy / MeV << " * exp(-(mu_bone * 2 + mu_soft_tissue * 1))" << std::endl;
    std::cout << "\t\t           = " << std::fixed << g_incident_energy / MeV << " * exp(-(" << std::scientific << g_xray_renderer.getMuFromHU(HU_bone, g_incident_energy) / (1.0 / cm) << " * 2 + " << g_xray_renderer.getMuFromHU(HU_soft_tissue, g_incident_energy) / (1.0 / cm) << " * 1))" << std::endl;
    std::cout << "\t\t           = " << std::scientific << g_expected_result / MeV << " MeV" << std::endl << std::endl;

    std::cout << "The absolute error is: |I_expected - I_reference| MeV" << std::endl;
    std::cout << "                     : |" << std::scientific << g_expected_result / MeV << " - " << g_literature_result / MeV << "| MeV" << std::endl;
    std::cout << "                     : " << std::fixed << std::abs(g_expected_result - g_literature_result) / MeV << " MeV" << std::endl << std::endl;

    g_inside_polygon_data.splitFaces(false);
	g_inside_polygon_data.splitFaces(false);
	g_inside_polygon_data.splitFaces(false);

    g_outside_polygon_data.splitFaces(false);
    g_outside_polygon_data.splitFaces(false);
    g_outside_polygon_data.splitFaces(false);
    g_outside_polygon_data.splitFaces(false);
}


//-----------------
void loadDetector()
//-----------------
{
	g_xray_detector.setResolutionInUnitsOfLengthPerPixel(g_resolution);
	g_xray_detector.setNumberOfPixels(g_number_of_pixels);
	g_xray_detector.setDetectorPosition(g_detector_position);
	g_xray_detector.setUpVector(g_detector_up_vector);
}


//---------------
void loadSource()
//---------------
{
	// Set the energy
	g_xray_beam.initialise(g_incident_energy);
	//g_xray_beam.initialise(PREFIX"/data/90kv_filtered_5mmCU_5mmAIR_5mmCU.txt", keV, 1);

	// Set the source position
	g_xray_detector.setXrayPointSource(g_source_position);

	// Use a parallel source
	g_xray_detector.setParallelBeam();
}


//----------------------
void loadXRaySimulator()
//----------------------
{
	// Initialise the X-ray renderer
    g_xray_renderer.initialise(XRayRenderer::OPENGL,
            GL_RGB32F,
            &g_xray_detector,
            &g_xray_beam);
}
