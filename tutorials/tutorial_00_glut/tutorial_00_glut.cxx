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
*   @file       tutorial_00_glut.cxx
*
*   @brief      Creating a window using GLUT.
*
*   @version    1.0
*
*   @date       26/08/2014
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
#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

// Create an OpenGL context and attach a window to it
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>   // Print error messages in the console
#include <sstream>    // Generate error messages
#include <exception>  // Catch C++ exception
#include <cstdlib>    // Define return status (EXIT_SUCCESS and EXIT_FAILURE)


//******************************************************************************
//  Name space
//******************************************************************************
using namespace std;


//******************************************************************************
//  Global variables
//******************************************************************************
// Keep track of the window width
GLsizei g_window_width(640);

// Keep track of the window height
GLsizei g_window_height(480);

// GLUT window ID
int g_window_id(0);


//******************************************************************************
//  Function declaration
//******************************************************************************
void initGLUT(int argc, char** argv);
void initGL();
void displayCallback();
void idleCallback();
void quitCallback();
void framebufferSizeCallback(int aWidth, int aHeight);
void keyCallback(unsigned char aKey, int aXPosition, int aYPosition);


//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
    // Return code
    int return_code(EXIT_SUCCESS);

    // Register the exit callback
    atexit(quitCallback);

    try
    {
        // Initialise GLUT
        initGLUT(argc, argv);

        // Initialise OpenGL
        initGL();

        // Set the projection matrix
        framebufferSizeCallback(g_window_width, g_window_height);

        // Launch the event loop
        glutMainLoop();
    }
    // Catch exception if any
    catch (const char* error)
    {
        cerr << error << endl;
        return_code = EXIT_FAILURE;
    }
    catch (const string& error)
    {
        cerr << error << endl;
        return_code = EXIT_FAILURE;
    }
    // Catch exception if any
    catch (const exception& error)
    {
        cerr << error.what() << endl;
        return_code = EXIT_FAILURE;
    }

    // Close the window and shut GLFW if needed
    quitCallback();

    // Return an exit code
    return (return_code);
}


//----------------------------------
void initGLUT(int argc, char** argv)
//----------------------------------
{
	// GLUT initialisation
	glutInit (&argc, argv);

	// Create a windowed mode window and its OpenGL context
	glutInitWindowSize(g_window_width, g_window_height);
	glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    g_window_id = glutCreateWindow("gVirtualXRay -- Tutorial 00 using Glut");

    // The window has not been created
    if (!g_window_id)
    {
        throw ("ERROR: cannot create a GLUT windowed mode window and its OpenGL context.");
    }

    // Register GLUT callbacks
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(framebufferSizeCallback);
    glutKeyboardFunc(keyCallback);
    glutIdleFunc(idleCallback);
}


//-----------
void initGL()
//-----------
{
    // Set the background colour
    glClearColor(0.5, 0.5, 0.5, 1.0);
}


//--------------------
void displayCallback()
//--------------------
{
    // Clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Swap front and back buffers
    glutSwapBuffers();
}


//-----------------
void idleCallback()
//-----------------
{
    // Do nothing
}


//-----------------
void quitCallback()
//-----------------
{
    // The window exists
    if (g_window_id)
    {
        // Close the window
        glutDestroyWindow(g_window_id);
        g_window_id = 0;
    }
}


//-------------------------------------------------
void framebufferSizeCallback(int width, int height)
//-------------------------------------------------
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

	// Redisplay the scene
	glutPostRedisplay();
}


//------------------------------------------------------------------
void keyCallback(unsigned char aKey, int aXPosition, int aYPosition)
//------------------------------------------------------------------
{
	switch(aKey)
	{
	// Close the program
	case 27:
	case 'q':
        quitCallback();
		exit(EXIT_SUCCESS);
		break;

	default:
		break;
	}

	// Redisplay the scene
	glutPostRedisplay();
}

