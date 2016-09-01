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
*	@file		AnatomicalData-glfw-moreRealistic.cxx
*
*	@brief      Demo to illustrate the tomography acquisition and reconstruction
*	             processesusing anatomical data.
*
*	@version	1.0
*
*	@date		01/09/2016
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
*				http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
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
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//	Global variables
//******************************************************************************
GLFWwindow* g_p_main_window_id(0);
AnatomicalDataHandler g_anatomical_data(600, 600);


//******************************************************************************
//	Function declaration
//******************************************************************************
/*void initGL();
void initFreeType();*/
void framebufferSizeCallback(GLFWwindow* apWindow, int aWidth, int aHeight);
void keyCallback(GLFWwindow* apWindow, int aKey, int aScanCode, int anAction, int aModifierKey);
void mouseButtonCallback(GLFWwindow* apWindow, int aButton, int aButtonState, int aModifierKey);
void cursorPosCallback(GLFWwindow* apWindow, double x, double y);
void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset);/*
void draw();
void render();*/
void quit();/*
void loadSTLFile();
void loadDetector();
void loadSource();
void loadXRaySimulator();
Vec3<GLfloat> getArcballVector(int x, int y);
void computeRotation(MATRIX4& aRotationMatrix);
void updateXRayImage();*/
void errorCallback(int error, const char* description);/*
void reset();
void displayHelp();
void usePatient(const PatientID& aPatientID);
*/


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

        // Enable anti-aliasing
        glfwWindowHint(GLFW_SAMPLES, 16);

        // Create a windowed mode window and its OpenGL context
        g_p_main_window_id = glfwCreateWindow(g_anatomical_data.getWidth(),
            g_anatomical_data.getHeight(),
            "gVirtualXRay -- Anatomical Data Demo -- GLFW3", NULL, NULL);
        
        // Window cannot be created 
        if (!g_p_main_window_id)
        {
            glfwTerminate();
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: cannot create a GLFW windowed mode window and its OpenGL context.");
        }

        // Make the window's context current
        glfwMakeContextCurrent(g_p_main_window_id);

        // Initialise GLEW
        initialiseGLEW();
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Check the OpenGL version
        std::cout << "GL:\t" << glGetString(GL_VERSION) << std::endl;

        // Check the GLSL version
        std::cout << "GLSL:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

        // Initialise OpenGL
		g_anatomical_data.initializeGL();
	    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Check the current FBO
		checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Initialize GLFL callback
		glfwSetKeyCallback(g_p_main_window_id, keyCallback);
		glfwSetFramebufferSizeCallback(g_p_main_window_id, framebufferSizeCallback);
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


void keyCallback(GLFWwindow* apWindow, int aKey, int aScanCode, int anAction, int aModifierKey)
{
}


void mouseButtonCallback(GLFWwindow* apWindow, int aButton, int aButtonState, int aModifierKey)
{
}


void cursorPosCallback(GLFWwindow* apWindow, double x, double y)
{
}


void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset)
{
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
