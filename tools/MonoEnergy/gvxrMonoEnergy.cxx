/*

Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
http://www.fpvidal.net/
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the Bangor University nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


/**
********************************************************************************
*
*	@file		gvxrMonoEnergy.cxx
*
*	@brief		Command line tool to simulate an X-ray projection using 
*               a monochromatic beam.
*
*	@version	1.0
*
*	@date		12/09/2016
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
*				http://www.fpvidal.net/, Sept 2016, 2016, version 1.1,
*				BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifdef HAS_GLEW
#include <GL/glew.h>
#endif

#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <stdlib.h>
#include <ctime>
#include <algorithm>

#include "cmdline.h"

#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>

#ifndef __Types_h
#include "gVirtualXRay/Types.h"
#endif

#ifndef __Units_h
#include "gVirtualXRay/Units.h"
#endif

#ifndef __PolygonMesh_h
#include "gVirtualXRay/PolygonMesh.h"
#endif

#ifndef __CubeMesh_h
#include "gVirtualXRay/CubeMesh.h"
#endif

#ifndef __XRayBeam_h
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef __XRayDetector_h
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef __OpenGLUtilities_h
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef __Matrix4x4_h
#include "gVirtualXRay/Matrix4x4.h"
#endif

#ifndef __XRayRenderer_h
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef __Shader_h
#include "gVirtualXRay/Shader.h"
#endif

#ifndef __Utilities_h
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef __Exception_h
#include "gVirtualXRay/Exception.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//	Global variables
//******************************************************************************
GLFWwindow* g_p_window(0);

GLfloat g_incident_energy(0.0);

VEC3 g_source_position;
VEC3 g_detector_position;
VEC3 g_detector_up_vector;

Vec2ui g_number_of_pixels;
GLfloat g_resolution(0.0);

std::vector<PolygonMesh> g_p_polygon_data_set;

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;


//******************************************************************************
//	Function declaration
//******************************************************************************
void errorCallback(int error, const char* description);
void quit();
void idle();
void loadDetector();
void loadSource();
void loadXRaySimulator();
void loadSTLFile(const std::string& aFileName, double aHUValue, double aUnit);
void updateXRayImage();


//---------
void quit()
//---------
{
	if (g_p_window)
	{
		glfwDestroyWindow(g_p_window);
        g_p_window = 0;
        glfwTerminate();
	}
}


//---------------------------------------------------------------------------
void loadSTLFile(const std::string& aFileName, double aHUValue, double aUnit)
//---------------------------------------------------------------------------
{
   	// Set geometry
    PolygonMesh polygon_mesh;
    
	polygon_mesh.setFilename(aFileName.data());
	polygon_mesh.loadSTLFile(false, false, true, true, aUnit, GL_STATIC_DRAW);
	polygon_mesh.setHounsfieldValue(aHUValue);

    g_p_polygon_data_set.push_back(polygon_mesh);
    g_xray_renderer.addInnerSurface(&(g_p_polygon_data_set.back()));
}


//-----------------
void loadDetector()
//-----------------
{
	g_xray_detector.setResolutionInUnitsOfLengthPerPixel(g_resolution);
	g_xray_detector.setNumberOfPixels(g_number_of_pixels);
	g_xray_detector.setDetectorPosition(g_detector_position);
	g_xray_detector.setUpVector(g_detector_up_vector);

    // Print parameters in the console
    std::cout << "Detector position (in cm):     " << g_detector_position / cm << std::endl;
    std::cout << "Detector up vector:            " << g_detector_up_vector     << std::endl;
    std::cout << "Number of pixels:              " << g_number_of_pixels       << std::endl;
    std::cout << "Pixel resolution (in cm):      " << g_resolution / cm        << std::endl;
}


//---------------
void loadSource()
//---------------
{
	// Set the energy
	g_xray_beam.initialise(g_incident_energy);

	// Set the source position
    XRayDetector::SourceShape shape(g_xray_detector.getSourceShape());
	g_xray_detector.setXrayPointSource(g_source_position);
    
    if (shape == XRayDetector::PARALLEL)
        {
        g_xray_detector.setParallelBeam();
        }

    // Print parameters in the console
    std::cout << "Source position (in cm):       " << g_source_position / cm << std::endl;
    std::cout << "Incident beam energy (in keV): " << g_incident_energy / keV << std::endl;
    std::cout << "Source shape:                  " << (shape == XRayDetector::PARALLEL?"Parallel":"Point") << std::endl;
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


//--------------------
void updateXRayImage()
//--------------------
{
    // Compute the X-Ray image
    g_xray_renderer.computeImage(MATRIX4());
}


//----------------------------------------------------
void errorCallback(int error, const char* description)
//----------------------------------------------------
{
	std::cerr << "GLFW error: " << description << std::endl;
}


//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
    try
        {
	    // Register the exit callback
		atexit(quit);

        // Read the command line arguments
        gengetopt_args_info ai;
        if (cmdline_parser (argc, argv, &ai) != 0)
        {
	    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: cannot read the command line arguments.");
        }

        if (ai.energy_given)
            {
            g_incident_energy = ai.energy_arg * keV;
            }
        
        if (ai.source_given)
            {
            g_source_position[0] = ai.source_arg[0] * cm;
            g_source_position[1] = ai.source_arg[1] * cm;
            g_source_position[2] = ai.source_arg[2] * cm;
            }
        
        if (ai.detector_given)
            {
            g_detector_position[0] = ai.detector_arg[0] * cm;
            g_detector_position[1] = ai.detector_arg[1] * cm;
            g_detector_position[2] = ai.detector_arg[2] * cm;
            }
        
        if (ai.up_given)
            {
            g_detector_up_vector[0] = ai.up_arg[0];
            g_detector_up_vector[1] = ai.up_arg[1];
            g_detector_up_vector[2] = ai.up_arg[2];
            }
        
        if (ai.pixels_given)
            {
            g_number_of_pixels[0] = ai.pixels_arg[0];
            g_number_of_pixels[1] = ai.pixels_arg[1];
            }
        
        if (ai.resolution_given)
            {
            g_resolution = ai.resolution_arg;
            }
        
        if (ai.shape_given)
            {
            std::string shape(ai.shape_arg);
            
            // Use a point source
            if (shape == "point" || shape == "Point" || shape == "POINT")
                {
				g_xray_detector.setPointSource();
                }
            // Use a parallel source
            else if (shape == "parallel" || shape == "Parallel" || shape == "PARALLEL")
                {
				g_xray_detector.setParallelBeam();
                }
            }
        
        std::string output_file_name;
        if (ai.output_given)
            {
            output_file_name = ai.output_arg;
            }

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

	    // Enable anti-aliasing
	    glfwWindowHint(GLFW_SAMPLES, 16);

		// Offscreen rendering
		glfwWindowHint(GLFW_VISIBLE, false);
		
		// Create the window title
		std::stringstream window_title;
		window_title << "gVirtualXRay -- Command Line Tool -- Monochromatic";

	    // Create a windowed mode window and its OpenGL context
	    g_p_window = glfwCreateWindow(600, 600, window_title.str().data(), NULL, NULL);
        
        // Window cannot be created 
        if (!g_p_window)
            {
	        glfwTerminate();
	    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: cannot create a GLFW windowed mode window and its OpenGL context.");
            }

	    // Make the window's context current
	    glfwMakeContextCurrent(g_p_window);

        // Initialise GLEW
        initialiseGLEW();

        // Check the OpenGL version
        std::cout << "GL:\t" << glGetString(GL_VERSION) << std::endl;

        // Check the GLSL version
        std::cout << "GLSL:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

		// Check the current FBO
		checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Load the data
		loadDetector();
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		loadSource();
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		loadXRaySimulator();
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        if (ai.geometries_given % 3)
            {
	    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: STL files should be followed by their respective HU values and unit of length (mm or cm).");
            }
        
        for (unsigned int i(0); i < ai.geometries_given; i+= 3)
            {
            std::string stl_file_name(ai.geometries_arg[i]);
            double HU_value(atof(ai.geometries_arg[i + 1]));
            std::string unit_as_string(ai.geometries_arg[i + 2]);
            double unit(1.0);
            
            if (unit_as_string == "um" || unit_as_string == "micrometre" || unit_as_string == "micrometer")
                {
                unit = um;
                }
            else if (unit_as_string == "mm" || unit_as_string == "millimetre" || unit_as_string == "millimeter")
                {
                unit = mm;
                }
            else if (unit_as_string == "cm" || unit_as_string == "centimetre" || unit_as_string == "centimeter")
                {
                unit = cm;
                }
            else if (unit_as_string == "dm" || unit_as_string == "decimetre" || unit_as_string == "decimeter")
                {
                unit = dm;
                }
            else if (unit_as_string == "m" || unit_as_string == "metre" || unit_as_string == "meter")
                {
                unit = m;
                }
            else if (unit_as_string == "dam" || unit_as_string == "decametre" || unit_as_string == "decameter")
                {
                unit = dam;
                }
            else if (unit_as_string == "hm" || unit_as_string == "hectometre" || unit_as_string == "hectometer")
                {
                unit = hm;
                }
            else if (unit_as_string == "km" || unit_as_string == "kilometre" || unit_as_string == "kilometer")
                {
                unit = km;
                }
            else
                {
                std::string error_message;
                error_message += "ERROR: ";
                error_message += unit_as_string;
                error_message += " is not a valid unit of length.";
                
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
                }

            std::cout << "Output file name:              " << output_file_name        << std::endl;
            std::cout << "Mesh:           :" << stl_file_name << " in " << unit_as_string << " and HU " << HU_value << std::endl;
            
            loadSTLFile(stl_file_name, HU_value, unit);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
            }
    
		// Update the X-ray image
        std::cout << "Output file name:              " << output_file_name        << std::endl;
        double start = glfwGetTime();
        updateXRayImage();
        g_xray_renderer.printEnergyFluence(output_file_name);
        double end = glfwGetTime();
        std::cout << "Computing time in sec:         " << end - start << std::endl;
        }
	// Catch exception if any
	catch (const std::exception& error)
        {
		std::cerr << error.what() << std::endl;
        }

	// Return an exit code
	return (EXIT_SUCCESS);
}
