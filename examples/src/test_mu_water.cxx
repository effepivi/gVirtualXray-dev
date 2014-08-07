/**
********************************************************************************
*
*	@file		test_mu_water.cxx
*
*	@brief		Main test program to test the value of mu water.
*
*	@version	1.0
*
*	@date		24/12/2013
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
#include <windows.h>
#endif

#define GLFW_INCLUDE_GLCOREARB 1

#include <GLFW/glfw3.h>

#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
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
RATIONAL_NUMBER g_incident_energy(80.0 * keV);
Vec2ui g_number_of_pixels(64, 64);
GLfloat g_resolution(0.15 * mm); // 1mm per pixel

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;


//******************************************************************************
//	Function declaration
//******************************************************************************
void loadDetector();
void loadSource();
void loadXRaySimulator();
void errorCallback(int error, const char* description);


//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
	try
	{
	    // Set an error callback
	    glfwSetErrorCallback(errorCallback);

	    // Initialize GLFW
	    if (!glfwInit())
	    {
	    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: cannot initialise GLFW.");
	    }

        // Enable OpenGL 3.2 if possible
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	    // Create the window
	    GLFWwindow* p_main_window_id(glfwCreateWindow(64, 64, "", 0, 0));
	    if (!p_main_window_id)
	    {
	        glfwTerminate();
	    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: cannot create a GLFW windowed mode window and its OpenGL context.");
	    }

	    // Make the window's context current
	    glfwMakeContextCurrent(p_main_window_id);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Load the data
		loadDetector();
		loadSource();
		loadXRaySimulator();

		// Print the energy in MeV and the attenuation coefficient of water in cm2/g
		std::ofstream test_linear_interpoation_stream(PREFIX"/examples/data/test_mu_water_linear_interpolation.csv");
		std::ofstream test_log_linear_interpoation_stream(PREFIX"/examples/data/test_mu_water_log_linear_interpolation.csv");
		double energy(0);
		for (energy = 1.00E-003; energy / MeV <= 0.1 * MeV; energy += 0.0001)
		{
			test_linear_interpoation_stream     << energy / MeV << " MeV\t" << g_xray_renderer.getMuWaterLinearInterpolation(energy)    / (cm2/g) << " cm2/g" << std::endl;
			test_log_linear_interpoation_stream << energy / MeV << " MeV\t" << g_xray_renderer.getMuWaterLogLinearInterpolation(energy) / (cm2/g) << " cm2/g" << std::endl;
		}

		for (; energy / MeV <= 100 * MeV; energy += 0.1)
		{
			test_linear_interpoation_stream     << energy / MeV << " MeV\t" << g_xray_renderer.getMuWaterLinearInterpolation(energy)    / (cm2/g) << " cm2/g" << std::endl;
			test_log_linear_interpoation_stream << energy / MeV << " MeV\t" << g_xray_renderer.getMuWaterLogLinearInterpolation(energy) / (cm2/g) << " cm2/g" << std::endl;
		}

		for (; energy / MeV <= 1.00E+005 / MeV; energy += 5)
		{
			test_linear_interpoation_stream     << energy / MeV << " MeV\t" << g_xray_renderer.getMuWaterLinearInterpolation(energy)    / (cm2/g) << " cm2/g" << std::endl;
			test_log_linear_interpoation_stream << energy / MeV << " MeV\t" << g_xray_renderer.getMuWaterLogLinearInterpolation(energy) / (cm2/g) << " cm2/g" << std::endl;
		}

		std::ofstream reference_stream(PREFIX"/examples/data/test_mu_water1.csv");
		g_xray_renderer.printMuWaterTable(reference_stream);

		// Close the window and shut GLFW
		if (p_main_window_id)
		{
			glfwDestroyWindow(p_main_window_id);
			p_main_window_id = 0;
			glfwTerminate();
		}
	}
	// Catch exception if any
	catch (const std::exception& error)
	{
		std::cerr << error.what() << std::endl;
	}

	return (EXIT_SUCCESS);
}


//-----------------
void loadDetector()
//-----------------
{
	g_xray_detector.setResolutionInUnitsOfLengthPerPixel(g_resolution);
	g_xray_detector.setNumberOfPixels(g_number_of_pixels);
}


//---------------
void loadSource()
//---------------
{
	// Set the energy
	//g_xray_beam.initialise(g_incident_energy);
	g_xray_beam.initialise(PREFIX"/examples/data/90kv_filtered_5mmCU_5mmAIR_5mmCU.txt", keV, 1);
}


//----------------------
void loadXRaySimulator()
//----------------------
{
	// Initialise the X-ray renderer
    g_xray_renderer.initialise(XRayRenderer::OPENGL,
            GL_RGB16F,
            &g_xray_detector,
            &g_xray_beam);
}


//----------------------------------------------------
void errorCallback(int error, const char* description)
//----------------------------------------------------
{
	std::cerr << "GLFW error: " << description << std::endl;
}
