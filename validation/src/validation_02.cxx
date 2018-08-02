/*

Copyright (c) 2016, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*  @file        validation_02.cxx
*
*  @brief       Main program to test the value of density (rho)
*               for various tissues.
*
*  @version1.0
*
*  @date        15/10/2014
*
*  @author      Dr Franck P. Vidal
*
*  @section     License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*  @section     Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifdef HAS_GLEW
#include <GL/glew.h>
#endif

#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <cstdlib>

#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>

#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Defines
//******************************************************************************
#define PREFIX "../data/validation_02"


//******************************************************************************
//  Global variables
//******************************************************************************
RATIONAL_NUMBER g_incident_energy(80.0 * keV);
Vec2ui g_number_of_pixels(64, 64);
GLfloat g_resolution(0.15 * mm); // 1mm per pixel

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;
GLFWwindow* g_p_main_window_id = 0;


//******************************************************************************
//  Function declaration
//******************************************************************************
void quit();
void loadDetector();
void loadSource();
void loadXRaySimulator();
void errorCallback(int error, const char* description);
void printAttenuationCoeffiecient(double& anEnergy,
    double aThreshold,
    double anOffset,
    std::ostream& anOutput);


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
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "ERROR: cannot initialise GLFW.");
            }

        // Enable OpenGL 2.1 if possible
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

        // Create the window
        g_p_main_window_id = glfwCreateWindow(64,
            64,
            "gVirtualXRay -- Validation 02",
            0,
            0);

        if (!g_p_main_window_id)
            {
            glfwTerminate();
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "ERROR: cannot create a GLFW windowed mode window and its OpenGL context.");
            }

        // Make the window's context current
        glfwMakeContextCurrent(g_p_main_window_id);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Initialise GLEW
        initialiseGLEW();

        // Load the data
        loadDetector();
        loadSource();
        loadXRaySimulator();

        // Print the density in g/cm3 for a range of HU values
        std::ofstream test_stream(PREFIX"/test_density.csv");

        if (!test_stream.is_open())
            {
            std::stringstream error_message;
            error_message << "ERROR: cannot open file:\t" <<
                PREFIX"//test_density.csv";

            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                error_message.str());
            }

        test_stream     << "HU\t" <<
                "density (in g/cm3)" <<
                std::endl;

        for (short HU(-1000); HU <= 1600; ++HU)
            {
            test_stream << HU << "\t" <<
                g_xray_renderer.getDensityFromHU(HU) / (g/cm3) << std::endl;
            }

        // Close the window and shut GLFW
        quit();
        }
    // Catch exception if any
    catch (const std::exception& error)
        {
        std::cerr << error.what() << std::endl;
        }

    return (EXIT_SUCCESS);
}


//---------
void quit()
//---------
{
    if (g_p_main_window_id)
        {
        glfwDestroyWindow(g_p_main_window_id);
        g_p_main_window_id = 0;
        //glfwTerminate();
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
