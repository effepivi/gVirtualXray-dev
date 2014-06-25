/**
********************************************************************************
*
*   @file       tutorial_01.cxx
*
*   @brief      Creating a Window and an OpenGL Core Profile 3.2 Context Using GLFW.
*
*   @version    1.0
*
*   @date       06/05/2014
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
// Ensure we are using opengl's core profile only
#ifdef HAS_OPENGL3
#ifdef __APPLE__
#define GL3_PROTOTYPES 1
#else
#define GL_GLEXT_PROTOTYPES 1
#endif

#define GLFW_INCLUDE_GLCOREARB 1 // Tell GLFW to include the OpenGL core profile header
#endif

#include <GLFW/glfw3.h> // Create an OpenGL context and attach a window to it

#include <iostream>   // Print error messages in the console
#include <exception>  // Catch C++ exception
#include <cstdlib>    // Define return status (EXIT_SUCCESS and EXIT_FAILURE)

// Define new types, e.g.~RATIONAL_NUMBER, VEC2, VEC3 and MATRIX4 to name a few
#include "gVirtualXRay/Types.h"

// Define units such as metre, kilometre, electronvolt, gram, kilogram, etc.
#include "gVirtualXRay/Units.h"

// Some utility functions about OpenGL, e.g. matrix stacks,
// how to set the projection matrix, etc.
#include "gVirtualXRay/OpenGLUtilities.h"

#include "gVirtualXRay/PolygonMesh.h"  // Handle 3D triangle meshes
#include "gVirtualXRay/Shader.h"       // Handle GLSL programs

#include "buildCube.h" // Create the triangle mesh of a cube


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;
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

// Shader program used to display the 3D scene
Shader g_display_shader;

// 3D objects as VAOs and VBOs
PolygonMesh g_polygon_mesh_1;
PolygonMesh g_polygon_mesh_2;

// Transformation matrices
MATRIX4 g_object_1_rotation_matrix;
MATRIX4 g_object_2_rotation_matrix;

// Geometric data
vector<double> g_p_vertex_set_1;
vector<unsigned char> g_p_index_set_1;
vector<float> g_p_vertex_set_2;

// Control the zoom
RATIONAL_NUMBER g_zoom(50.0 * cm);

// Vertex shader
const GLchar* g_vertex_shader = "\
\n#version 150\n \
\n \
in vec3 in_Vertex;\n \
\n \
uniform mat4 g_projection_matrix;\n \
uniform mat4 g_modelview_matrix;\n \
\n \
void main(void)\n \
{\n \
    gl_Position = g_projection_matrix * g_modelview_matrix * vec4(in_Vertex, 1.0);\n \
}\n \
";

// Fragment shader
const GLchar* g_fragment_shader = "\
\n#version 150\n \
precision highp float;\n \
\n \
out vec4 fragColor;\n \
void main(void)\n \
{\n \
    fragColor = vec4(1.0, 1.0, 1.0, 1.0);\n \
}\n \
";


//******************************************************************************
//  Function declaration
//******************************************************************************
void initGLFW();
void initGL();
void load3DObjects();
void displayCallback();
void idleCallback();
void quitCallback();
void framebufferSizeCallback(GLFWwindow* apWindow, int aWidth, int aHeight);
void keyCallback(GLFWwindow* apWindow, int aKey, int aScanCode, int anAction, int aModifierKey);
void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset);
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

        // Give a text ID to the vertex and fragment shaders, it can be useful when debuging shaders
        g_display_shader.setLabels("g_vertex_shader", "g_fragment_shader");

        // Load the source code of the shaders onto the GPU
        g_display_shader.loadSource(g_vertex_shader, g_fragment_shader);

        // Initialise the geometry of the 3D objects
        load3DObjects();

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
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "ERROR: cannot initialise GLFW.");
    }

    // Enable OpenGL 3.2
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Enable anti-aliasing
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create a windowed mode window and its OpenGL context
    g_p_window_id = glfwCreateWindow(g_window_width, g_window_height,
            "gVirtualXRay -- Tutorial 01", NULL, NULL);

    // The window has not been created
    if (!g_p_window_id)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "ERROR: cannot create a GLFW windowed mode window and its OpenGL context.");
    }

    // Make the window's context current
    glfwMakeContextCurrent(g_p_window_id);

    // Register GLFW callbacks
    glfwSetFramebufferSizeCallback(g_p_window_id, framebufferSizeCallback);
    glfwSetKeyCallback(g_p_window_id, keyCallback);
    glfwSetScrollCallback(g_p_window_id, scrollCallback);
}


//-----------
void initGL()
//-----------
{
    // Enable the Z-buffer
    glEnable(GL_DEPTH_TEST);

    // Set the background colour
    glClearColor(0.5, 0.5, 0.5, 1.0);

    // Check if any OpenGL error has occurred.
    // If any has, an exception is thrown
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//------------------
void load3DObjects()
//------------------
{
    // Centre of the cubes
    VEC3 cube_centre(0, 0, 0);

    // Size of the cubes
    RATIONAL_NUMBER length_1( 5.0 * cm);
    RATIONAL_NUMBER length_2(10.0 * cm);

    // Create the cube using vertex data and index data
    buildCube(length_1, cube_centre, g_p_vertex_set_1, g_p_index_set_1);

    // Create the cube using vertex data only
    buildCube(length_2, cube_centre, g_p_vertex_set_2);

    // Set geometry (using VAOs and VBOs)
    g_polygon_mesh_1.setExternalData(GL_TRIANGLES,
            &g_p_vertex_set_1,
            &g_p_index_set_1,
            true,
            GL_STATIC_DRAW);

    g_polygon_mesh_2.setExternalData(GL_TRIANGLES,
            &g_p_vertex_set_2,
            true,
            GL_STATIC_DRAW);
}


//--------------------
void displayCallback()
//--------------------
{
    // Clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Add the current shader to the shader stack
    pushShaderProgram();

    // Enable the shader
    g_display_shader.enable();
    GLint shader_id(g_display_shader.getProgramHandle());

    // Check the status of OpenGL and of the current FBO
    checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // A handle for shader resources
    GLuint handle(0);

    // Upload the projection matrix
    handle = glGetUniformLocation(shader_id, "g_projection_matrix");
    glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_projection_matrix.get());
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Upload the modelview matrix
    handle = glGetUniformLocation(shader_id, "g_modelview_matrix");
    glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_modelview_matrix.get());
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Store the current transformation matrices
    pushModelViewMatrix();
    pushProjectionMatrix();

    // Translate the 1st object
    g_current_modelview_matrix *= MATRIX4::buildTranslationMatrix(
            VEC3(8.0 * cm, 0.0, 0.0));

    // Rotate the 1st object
    g_current_modelview_matrix *= g_object_1_rotation_matrix;

    // Apply the change to the shader program
    applyModelViewMatrix();

    // Display the 1st object
    g_polygon_mesh_1.display();

    // Restore the current transformation matrix
    popModelViewMatrix();
    popProjectionMatrix();

    // Store the current transformation matrices
    pushModelViewMatrix();
    pushProjectionMatrix();

    // Translate the 2nd object
    g_current_modelview_matrix *= MATRIX4::buildTranslationMatrix(
            VEC3(-8.0 * cm, 0.0, 0.0));

    // Rotate the 2nd object
    g_current_modelview_matrix *= g_object_2_rotation_matrix;

    // Apply the change to the shader program
    applyModelViewMatrix();

    // Display the 2nd object
    g_polygon_mesh_2.display();

    // Restore the current transformation matrix
    popModelViewMatrix();
    popProjectionMatrix();

    // Disable the shader and restore the previous shader from the stack
    popShaderProgram();

    // Check the status of OpenGL and of the current FBO
    checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//-----------------
void idleCallback()
//-----------------
{
    // Rotate the objects
    g_object_1_rotation_matrix.rotate(1.0, VEC3(1.0, 0.0, 0.0));
    g_object_2_rotation_matrix.rotate(2.0, VEC3(0.0, 1.0, 0.0));
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

    // Set up the projection matrix (g_current_projection_matrix)
    loadPerspectiveProjectionMatrix(45.0, screen_aspect_ratio, 0.1 * cm, 5000.0 * cm);

    // Set up the modelling-viewing matrix (g_current_modelview_matrix)
    loadLookAtModelViewMatrix(50.0 * cm, 50.0 * cm, g_zoom,
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);
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


//-----------------------------------------------------------------------
void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset)
//-----------------------------------------------------------------------
{
    // Scrolling along the Y-axis
    if (fabs(yoffset) > EPSILON)
    {
        // Change the zoom
        g_zoom += 5 * yoffset * cm;

        // Update the projection matrix
        framebufferSizeCallback(apWindow, g_window_width, g_window_height);
    }
}


//----------------------------------------------------
void errorCallback(int error, const char* description)
//----------------------------------------------------
{
    // Throw an error
    throw Exception(__FILE__, __FUNCTION__, __LINE__, description);
}
