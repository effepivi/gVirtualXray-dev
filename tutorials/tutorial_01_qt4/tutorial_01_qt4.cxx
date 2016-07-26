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
*   @file       tutorial_01_qt4.cxx
*
*   @brief      Creating a Window and an OpenGL Core Profile 3.2 Context Using Qt.
*
*   @version    1.0
*
*   @date       30/06/2016
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

#include <QtGui>
#include <QGLWidget>

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
//  Class declaration
//******************************************************************************
class GLWidget;


//******************************************************************************
//  Function declaration
//******************************************************************************
void initQt(GLWidget& aWindow);
void initGLFW();
void initGLEW();
void initGL();
void load3DObjects();
void quitCallback();
///void framebufferSizeCallback(GLFWwindow* apWindow, int aWidth, int aHeight);
///void keyCallback(GLFWwindow* apWindow, int aKey, int aScanCode, int anAction, int aModifierKey);
///void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset);
void errorCallback(int error, const char* description);


//******************************************************************************
//  Class definition
//******************************************************************************
class GLWidget: public QGLWidget
{
public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    
protected:
    void initializeGL();
    void paintGL();
    void animate();
    void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent * event);
};


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
        // Initialise Qt and OpenGL
        QApplication application(argc, argv);
        GLWidget window;
        initQt(window);

        // Launch the event loop
        return_code = application.exec();
    }
    // Catch exception if any
    catch (const exception& error)
    {
        cerr << error.what() << endl;
        return_code = EXIT_FAILURE;
    }

    // Clean up
    quitCallback();

    // Return an exit code
    return (return_code);
}


//---------------------------
void initQt(GLWidget& aWindow)
//---------------------------
{
    // Initialize Qt
    // and create a windowed mode window and its OpenGL context

    aWindow.resize(g_window_width, g_window_height);
    aWindow.show();
    aWindow.setWindowTitle(QApplication::translate("toplevel", "gVirtualXRay -- Tutorial 01"));
    
    // Make the window's context current
    aWindow.makeCurrent();
}


//----------------------------------
GLWidget::GLWidget(QWidget *parent):
//----------------------------------
    QGLWidget(parent)
//----------------------------------
{
}


//-------------------
GLWidget::~GLWidget()
//-------------------
{}


//-------------------------------------
QSize GLWidget::minimumSizeHint() const
//-------------------------------------
{
    return QSize(50, 50);
}


//------------------------------
QSize GLWidget::sizeHint() const
//------------------------------
{
    return QSize(g_window_width, g_window_height);
}
   

//---------------------------
void GLWidget::initializeGL()
//---------------------------
{
    // Initialise GLFW
    //initGLFW();

    // Initialise GLEW
    initGLEW();

    // Initialise OpenGL
    initGL();

    // Give a text ID to the vertex and fragment shaders, it can be useful when debuging shaders
    g_display_shader.setLabels("g_vertex_shader", "g_fragment_shader");

    // Load the source code of the shaders onto the GPU
    g_display_shader.loadSource(g_vertex_shader, g_fragment_shader);

    // Initialise the geometry of the 3D objects
    load3DObjects();
}


//----------------------
void GLWidget::paintGL()
//----------------------
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

    animate();
}


//----------------------
void GLWidget::animate()
//---------------------
{
    // Rotate the objects
    g_object_1_rotation_matrix.rotate(1.0, VEC3(1.0, 0.0, 0.0));
    g_object_2_rotation_matrix.rotate(2.0, VEC3(0.0, 1.0, 0.0));

    QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
}


//--------------------------------------------
void GLWidget::resizeGL(int width, int height)
//--------------------------------------------
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


//-------------------------------------------
void GLWidget::keyPressEvent(QKeyEvent * event)
//-------------------------------------------
{
    switch(event->key())
    {
    // Close the program
    case Qt::Key_Q:
    case Qt::Key_Escape:
            close();
            break;

    default:
    std::cout << event->key() << std::endl;
        break;
    }
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


//-----------------
void quitCallback()
//-----------------
{
    // Do nothing
}

