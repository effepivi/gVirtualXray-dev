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
*   @file       AnatomicalData-glfw-moreRealistic.cxx
*
*   @brief      Demo to illustrate the tomography acquisition and reconstruction
*               processesusing anatomical data.
*
*   @version    1.0
*
*   @date       01/09/2016
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

#define GLFW_INCLUDE_GLCOREARB 1

#include <GLFW/glfw3.h>

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif


#include "AnatomicalDataHandler.h"


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Global variables
//******************************************************************************
GLFWwindow* g_p_main_window_id(0);
GLuint g_main_window_width(600);
GLuint g_main_window_height(600);
AnatomicalDataHandler g_anatomical_data;


//******************************************************************************
//  Function declaration
//******************************************************************************
void framebufferSizeCallback(GLFWwindow* apWindow, int aWidth, int aHeight);

void keyCallback(GLFWwindow* apWindow,
    int aKey,
    int aScanCode,
    int anAction,
    int aModifierKey);

void mouseButtonCallback(GLFWwindow* apWindow,
    int aButton,
    int aButtonState,
    int aModifierKey);

void cursorPosCallback(GLFWwindow* apWindow, double x, double y);

void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset);

void quit();

void errorCallback(int error, const char* description);



//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
    try
        {
        // Initialize random seed
        srand(time(0));

        // Set an error callback
        glfwSetErrorCallback(errorCallback);

        // Register the exit callback
        atexit(quit);

        //Image<float> CT_data_set("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/frozenCT.mhd");
        //CT_data_set = -1000.0f + (2357.0f / 255.0f) * CT_data_set;
        //CT_data_set = CT_data_set.convertHU2mu(0.08 * MeV);
        Image<float> CT_data_set("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/ct_data.mhd");

        Sinogram<float> sinogram(CT_data_set.radonTransform(0, 1, 179));

        Image<float> projection_set(sinogram.getProjectionSet());

//#ifndef NDEBUG
        CT_data_set.save("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/ct_data_80keV.mhd");
        sinogram.save("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/sinogram.mhd");
        projection_set.save("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/projection.mhd");
//#endif

        // Initialize GLFW
        if (!glfwInit())
            {
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "ERROR: cannot initialise GLFW.");
            }

        // Enable OpenGL 3.2 if possible
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Enable anti-aliasing
        glfwWindowHint(GLFW_SAMPLES, 16);

        // Create a windowed mode window and its OpenGL context
        g_p_main_window_id = glfwCreateWindow(g_main_window_width,
                g_main_window_height,
                "gVirtualXRay -- Visible Human Demo -- GLFW3",
                0,
                0);
        
        // Window cannot be created
        if (!g_p_main_window_id)
            {
            glfwTerminate();
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "ERROR: cannot create a GLFW windowed mode window and "
                "its OpenGL context.");
            }

        // Make the window's context current
        glfwMakeContextCurrent(g_p_main_window_id);

        // Initialise GLEW
        initialiseGLEW();
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Check the OpenGL version
        std::cout << "GL:\t" << glGetString(GL_VERSION) << std::endl;

        // Check the GLSL version
        std::cout << "GLSL:\t" <<
            glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

        // Initialise OpenGL
        g_anatomical_data.initializeGL();
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Check the current FBO
        checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Initialize GLFL callback
        glfwSetKeyCallback(g_p_main_window_id, keyCallback);
        glfwSetFramebufferSizeCallback(g_p_main_window_id,
            framebufferSizeCallback);
        glfwSetMouseButtonCallback(g_p_main_window_id, mouseButtonCallback);
        glfwSetCursorPosCallback(g_p_main_window_id, cursorPosCallback);
        glfwSetScrollCallback(g_p_main_window_id, scrollCallback);

        // Set the projection matrix
        GLint width(0);
        GLint height(0);
        glfwGetFramebufferSize(g_p_main_window_id, &width, &height);
        framebufferSizeCallback(g_p_main_window_id, width, height);


        // Launch the event loop
        while (!glfwWindowShouldClose(g_p_main_window_id))
            {
            g_anatomical_data.draw();

            // Swap front and back buffers
            glfwSwapBuffers(g_p_main_window_id);

            // Poll for and process events
            glfwPollEvents();
            }
        }
    // Catch exception if any
    catch (const std::exception& error)
        {
        std::cerr << error.what() << std::endl;
        }
    catch (const std::string& error)
        {
        std::cerr << error << std::endl;
        }
    catch (const char* error)
        {
        std::cerr << error << std::endl;
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


//-------------------------------------------------------------------------
void framebufferSizeCallback(GLFWwindow* apWindow, int aWidth, int aHeight)
//-------------------------------------------------------------------------
{
    g_anatomical_data.resizeGL(aWidth, aHeight);
}


//------------------------------------
void keyCallback(GLFWwindow* apWindow,
                 int aKey,
                 int aScanCode,
                 int anAction,
                 int aModifierKey)
//------------------------------------
{
    if (anAction == GLFW_PRESS)
        {
        switch (aKey)
            {
            case GLFW_KEY_H:
                //g_display_help = !g_display_help;
                break;

            case GLFW_KEY_Q:
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(g_p_main_window_id, GL_TRUE);
                break;

            case GLFW_KEY_SPACE:
                /*g_run_tomography_acquisition = !g_run_tomography_acquisition;

                if (g_run_tomography_acquisition)
                    {
                    //if (g_next_angle_to_process == g_angle_number)
                        {
                        reset();
                        }
                    }*/
                break;

            case GLFW_KEY_B:
                //g_display_beam = !g_display_beam;
                break;

            case GLFW_KEY_N:
                /*g_xray_renderer.useNegativeFilteringFlag(
                    !g_xray_renderer.getNegativeFilteringFlag());*/
                break;

            case GLFW_KEY_W:
                //g_display_wireframe = !g_display_wireframe;
                break;

            case GLFW_KEY_KP_ADD:
                /*g_zoom -= 1.0 * cm;
                framebufferSizeCallback(
                    g_p_main_window_id,
                    g_main_window_width,
                    g_main_window_height);*/
                break;

            case GLFW_KEY_EQUAL:
/*
                if (g_use_left_shift || g_use_right_shift)
                    {
                    g_zoom -= 1.0 * cm;
                    framebufferSizeCallback(
                        g_p_main_window_id,
                        g_main_window_width,
                        g_main_window_height);
                    }*/
                break;

            case GLFW_KEY_MINUS:
            case GLFW_KEY_KP_SUBTRACT:
               /* g_zoom += 1.0 * cm;
                framebufferSizeCallback(
                    g_p_main_window_id,
                    g_main_window_width,
                    g_main_window_height);*/
                break;

            case GLFW_KEY_LEFT_SHIFT:
                //g_use_left_shift = true;
                break;

            case GLFW_KEY_RIGHT_SHIFT:
                //g_use_right_shift = true;
                break;

            case GLFW_KEY_S:
                /*if (g_display_skin)
                    {
                    takeScreenshot("screenshot_with_skin.tga");
                    }
                else
                    {
                    takeScreenshot("screenshot_without_skin.tga");
                    }
                g_display_skin = !g_display_skin;*/
                break;

            default:
                break;
            }
        }
    else if (anAction == GLFW_RELEASE)
        {
        switch (aKey)
            {
            case GLFW_KEY_LEFT_SHIFT:
                //g_use_left_shift = false;
                break;

            case GLFW_KEY_RIGHT_SHIFT:
                //g_use_right_shift = false;
                break;
            }
        }

    //g_take_screenshot = true;
}


//--------------------------------------------
void mouseButtonCallback(GLFWwindow* apWindow,
                         int aButton,
                         int aButtonState,
                         int aModifierKey)
//--------------------------------------------
{
    double xpos(0);
    double ypos(0);
    glfwGetCursorPos (apWindow, &xpos, &ypos);

    // Use the arc ball
    if (aButtonState == GLFW_PRESS)
        {
        g_anatomical_data.enableArcBall(xpos, ypos);
        }
    // Stop using the arc ball
    else
        {
        g_anatomical_data.disableArcBall();
        }
}


//------------------------------------------
void cursorPosCallback(GLFWwindow* apWindow,
                       double x,
                       double y)
//------------------------------------------
{
    // Update the rotation matrix if needed
    g_anatomical_data.computeRotation(x, y);
}


//---------------------------------------
void scrollCallback(GLFWwindow* apWindow,
                    double xoffset,
                    double yoffset)
//---------------------------------------
{
    // Scrolling along the Y-axis
    if (fabs(yoffset) > EPSILON)
        {
        if (yoffset > 0)
            {
            g_anatomical_data.zoomIn();
            }
        else
            {
            g_anatomical_data.zoomOut();
            }
        }
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
