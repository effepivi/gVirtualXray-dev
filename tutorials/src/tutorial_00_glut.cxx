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
*               http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
//#include <GL/glew.h>  // Handle shaders

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
void initGLEW();
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

        // Initialise GLEW
        initGLEW();

        // Initialise OpenGL
        initGL();

        // Set the projection matrix
        framebufferSizeCallback(g_window_width, g_window_height);

        // Launch the event loop
        glutMainLoop();
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
    g_window_id = glutCreateWindow("gVirtualXRay -- Tutorial 00");

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


//-------------
void initGLEW()
//-------------
{
/*	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::stringstream error_message;
		error_message << "ERROR: cannot initialise GLEW:\t" << glewGetErrorString(err);

        throw (error_message.str());
	}*/
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
		exit(EXIT_SUCCESS);
		break;

	default:
		break;
	}

	// Redisplay the scene
	glutPostRedisplay();
}

