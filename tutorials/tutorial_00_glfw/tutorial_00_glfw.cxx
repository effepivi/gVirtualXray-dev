/**
********************************************************************************
*
*   @file       tutorial_00_glfw.cxx
*
*   @brief      Creating a Window using GLFW.
*
*   @version    1.0
*
*   @date       26/08/2014
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
*               http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <GLFW/glfw3.h> // Create an OpenGL context

#include <iostream>   // Print error messages in the console
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

// GLFW window ID
GLFWwindow* g_p_window_id(0);


//******************************************************************************
//  Function declaration
//******************************************************************************
void initGLFW();
void initGL();
void displayCallback();
void idleCallback();
void quitCallback();
void framebufferSizeCallback(GLFWwindow* apWindow, int aWidth, int aHeight);
void keyCallback(GLFWwindow* apWindow, int aKey, int aScanCode, int anAction, int aModifierKey);
void errorCallback(int error, const char* description);


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
        // Initialise GLFW
        initGLFW();

        // Initialise OpenGL
        initGL();

        // Set the projection matrix
        framebufferSizeCallback(g_p_window_id, g_window_width, g_window_height);

        // Launch the event loop
        while (!glfwWindowShouldClose(g_p_window_id))
        {
            // Render here
            displayCallback();

            // Swap front and back buffers
            glfwSwapBuffers(g_p_window_id);

            // Poll for and process events
            glfwPollEvents();

            // Idle callback
            idleCallback();
        }
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


//-------------
void initGLFW()
//-------------
{
    // Set an error callback
    glfwSetErrorCallback(errorCallback);

    // Initialize GLFW
    if (!glfwInit())
    {
        throw ("ERROR: cannot initialise GLFW.");
    }

    // Create a windowed mode window and its OpenGL context
    g_p_window_id = glfwCreateWindow(g_window_width, g_window_height,
            "gVirtualXRay -- Tutorial 00", NULL, NULL);

    // The window has not been created
    if (!g_p_window_id)
    {
        throw ("ERROR: cannot create a GLFW windowed mode window and its OpenGL context.");
    }

    // Make the window's context current
    glfwMakeContextCurrent(g_p_window_id);

    // Register GLFW callbacks
    glfwSetFramebufferSizeCallback(g_p_window_id, framebufferSizeCallback);
    glfwSetKeyCallback(g_p_window_id, keyCallback);
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
    if (g_p_window_id)
    {
        // Close the window
        glfwDestroyWindow(g_p_window_id);
        g_p_window_id = 0;

        // Cleanup GLFW
        glfwTerminate();
    }
}


//-----------------------------------------------------------------------
void framebufferSizeCallback(GLFWwindow* apWindow, int width, int height)
//-----------------------------------------------------------------------
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
}


//-------------------------------------------------------------------------------
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
//-------------------------------------------------------------------------------
{
    if (action == GLFW_PRESS)
    {
        switch(key)
        {
        // Close the program
        case GLFW_KEY_Q:
        case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(g_p_window_id, GL_TRUE);
                break;

        default:
            break;
        }
    }
}


//----------------------------------------------------
void errorCallback(int error, const char* description)
//----------------------------------------------------
{
    // Throw an error
    throw (description);
}

