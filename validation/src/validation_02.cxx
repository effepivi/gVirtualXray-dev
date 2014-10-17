/**
********************************************************************************
*
*	@file		validation_02.cxx
*
*	@brief		Main program to test the value of density (rho)
*				for various tissues.
*
*	@version	1.0
*
*	@date		15/10/2014
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
#include <GL/glew.h>

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
void initGLEW();
void loadDetector();
void loadSource();
void loadXRaySimulator();
void errorCallback(int error, const char* description);
void printAttenuationCoeffiecient(double& anEnergy, double aThreshold, double anOffset, std::ostream& anOutput);

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

        // Enable OpenGL 2.1 if possible
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

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

        // Initialise GLEW
        initGLEW();

        // Load the data
		loadDetector();
		loadSource();
		loadXRaySimulator();

		// Print the density in g/cm3 for a range of HU values
		std::ofstream test_stream(PREFIX"/data/test_density.csv");

        if (!test_stream.is_open())
        {
            std::stringstream error_message;
            error_message << "ERROR: cannot open file:\t" << PREFIX"/data/test_density.csv";

            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
        }

        test_stream     << "HU\t" <<
                "density (in g/cm3)" <<
                std::endl;

        for (short HU(-1000); HU <= 1600; ++HU)
        {
        	test_stream << HU << "\t" << g_xray_renderer.getDensityFromHU(HU) / (g/cm3) << std::endl;
        }

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


//-------------
void initGLEW()
//-------------
{
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::stringstream error_message;
        error_message << "ERROR: cannot initialise GLEW:\t" << glewGetErrorString(err);

        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
    }
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
	g_xray_beam.initialise(g_incident_energy);
	//g_xray_beam.initialise(PREFIX"/data/90kv_filtered_5mmCU_5mmAIR_5mmCU.txt", keV, 1);
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
