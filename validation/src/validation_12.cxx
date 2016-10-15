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
*  @file        validation_11.cxx
*
*  @brief       Main test program to make sur that the received energy on 
*               a detector without any scanned object is equal to 
*               the number of photons times the incident energy.
*
*  @version     1.0
*
*  @date        14/10/2016
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
*               http://www.fpvidal.net/, Oct 2016, 2016, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <cmath>
#include <iomanip>
#include <cstdlib>

#define GLFW_INCLUDE_GLCOREARB 1

#include <GLFW/glfw3.h>

#ifndef __Types_h
#include "gVirtualXRay/Types.h"
#endif

#ifndef __Units_h
#include "gVirtualXRay/Units.h"
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

#ifndef __Utilities_h
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef __Exception_h
#include "gVirtualXRay/Exception.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Global variables
//******************************************************************************
GLsizei g_main_window_width(1);
GLsizei g_main_window_height(1);
GLFWwindow* g_p_main_window_id(0);

Matrix4x4<GLfloat> g_scene_rotation_matrix;
Matrix4x4<GLfloat> g_detector_rotation_matrix;
Matrix4x4<GLfloat> g_sample_rotation_matrix;

GLfloat g_resolution(1.0 * mm); // 1mm per pixel
Vec2ui g_number_of_pixels(g_main_window_width, g_main_window_height);
VEC3 g_source_position(   0.0, 0.0, -10.0 * cm);
VEC3 g_detector_position( 0.0, 0.0,  10.0 * cm);
VEC3 g_detector_up_vector(0.0, 1.0,   0.0);

RATIONAL_NUMBER g_cube_thickness(1.0 * mm);
CubeMesh<float> g_inside_polygon_data(g_cube_thickness);

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;

unsigned int g_number_of_photons(5);
float g_photon_energy(80.0 * keV);


//******************************************************************************
//  Function declaration
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
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "ERROR: cannot initialise GLFW.");
            }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Enable anti-aliasing
        glfwWindowHint(GLFW_SAMPLES, 4);

        // Create a windowed mode window and its OpenGL context
        g_p_main_window_id = glfwCreateWindow(g_main_window_width,
            g_main_window_height,
            "gVirtualXRay -- Validation 11",
            0,
            0);
       
        // Window cannot be created
        if (!g_p_main_window_id)
            {
            glfwTerminate();
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "ERROR: cannot create a GLFW windowed mode window and its OpenGL context.");
            }

        // Make the window's context current
        glfwMakeContextCurrent(g_p_main_window_id);

        // Initialise GLEW
        initialiseGLEW();

        // Check the current FBO
        checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Load the data
        loadDetector();
        loadSource();
        loadXRaySimulator();

        // Add an empty geometry to the X-ray renderer
        g_xray_renderer.addInnerSurface(&g_inside_polygon_data);

        // Set the projection matrix
        GLint width(0);
        GLint height(0);
        glfwGetFramebufferSize(g_p_main_window_id, &width, &height);

        // Compute the projection set
        g_xray_renderer.computeProjectionSet(g_sample_rotation_matrix,
            VEC3(),
            1,
            0);
        
        // Get the pixel value
        float simulated_energy_fluence(g_xray_renderer.getProjectionSet().getPixel(0, 0, 0));
        float incident_energy(g_number_of_photons * g_photon_energy);

        // Output the results
        std::cout << "The total energy of the incident beam is " << g_number_of_photons <<
                " * " << g_photon_energy / MeV << " = " <<
                incident_energy / MeV << " MeV" << std::endl;
        
        std::cout << "The thickness of the cube is " << g_cube_thickness / mm << std::endl;
        
        float theoretical_energy_fluence(incident_energy * std::exp(-1.837E-01 * g_cube_thickness / cm));

        std::cout << "The theoretical energy fluence is " << g_number_of_photons << " * " << incident_energy / MeV << " * exp(-1.837E-01 * " << g_cube_thickness / cm << ") = " << theoretical_energy_fluence / MeV << " MeV" << std::endl;

        std::cout << "The simulated energy fluence is " << simulated_energy_fluence << " MeV" << std::endl;
        
        std::cout << "The relative error is " << std::abs(simulated_energy_fluence - theoretical_energy_fluence) / theoretical_energy_fluence << "%" << std::endl;
        
        //
        
        // Close the window
        glfwSetWindowShouldClose(g_p_main_window_id, GL_TRUE);
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
    g_xray_beam.addChannel(g_number_of_photons, g_photon_energy);

    // Set the source position
    g_xray_detector.setXrayPointSource(g_source_position);
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
