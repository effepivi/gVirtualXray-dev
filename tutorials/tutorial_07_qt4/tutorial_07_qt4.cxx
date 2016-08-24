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
*   @file       tutorial_07_qt4.cxx
*
*   @brief      Improve the second tutorial by:
*               - Adding a better shader program to display
*                 the 3D scene with Phong shading.
*               - Adding the X-ray simulation.
*               - Display objects in wireframe or not
*               - Using operator - to combine two polygon meshes
*                 into a single one
*
*   @version    1.0
*
*   @date       24/08/2016
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

#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <QtGui>
#include <QGLWidget>

#include <iostream>   // Print error messages in the console
#include <sstream>    // Generate error messages
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

#include "gVirtualXRay/Utilities.h" // Handle decompression using the Z library

#include "gVirtualXRay/XRayBeam.h"     // Manage X-Ray beams
#include "gVirtualXRay/XRayDetector.h" // Handle virtual X-Ray detectors
#include "gVirtualXRay/XRayRenderer.h" // Compute X-Ray images on the GPU

#include "buildCube.h" // Create the triangle mesh of a cube

#include "tutorial_07_gl2.frag.h" // Fragment shader for OpenGL 2.x
#include "tutorial_07_gl2.vert.h" // Vertex shader for OpenGL 2.x

#include "tutorial_07_gl3.frag.h" // Fragment shader for OpenGL 3.x
#include "tutorial_07_gl3.vert.h" // Vertex shader for OpenGL 3.x


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;
using namespace std;


//******************************************************************************
//  Constant variables
//******************************************************************************
const GLfloat g_rotation_speed(2.0);


//******************************************************************************
//  Global variables
//******************************************************************************
// Keep track of the window width
GLsizei g_window_width(640);

// Keep track of the window height
GLsizei g_window_height(480);

// Shader program used to display the 3D scene
Shader g_display_shader;

// 3D object as VAO and VBO
PolygonMesh g_final_polygon_mesh;

// Transformation matrices
MATRIX4 g_sample_rotation_matrix;
MATRIX4 g_scene_rotation_matrix;

// Control the zoom
RATIONAL_NUMBER g_zoom(50.0 * cm);

// Use the arc ball rotation
bool g_use_arc_ball(false);

// States of the mouse
int g_button(-1); // Button
int g_button_state(-1); // Button state
GLint g_last_x_position(0); // Previous x position of the cursor
GLint g_last_y_position(0); // Previous y position of the cursor
GLint g_current_x_position(0); // Current x position of the cursor
GLint g_current_y_position(0); // Current x position of the cursor

// Components needed to simulate X-ray images
XRayDetector g_xray_detector;
XRayBeam     g_xray_beam;
XRayRenderer g_xray_simulator;

// Control the display in wireframe or not
bool g_wireframe(false);


//******************************************************************************
//  Class declaration
//******************************************************************************
class GLWidget;


//******************************************************************************
//  Function declaration
//******************************************************************************
void initQt(GLWidget& aWindow);

void initGLUT(int argc, char** argv);

void initGL();

void initShader();

void load3DObjects();

void displayCallback();

void idleCallback();

void quitCallback();

void framebufferSizeCallback(int aWidth, int aHeight);

void keyCallback(unsigned char aKey, int aXPosition, int aYPosition);

void mouseButtonCallback(int aButton,
        int aState,
        int aXPosition,
        int aYPosition);

void cursorPosCallback(int aXPosition, int aYPosition);

VEC3 getArcballVector(int x, int y);

float radian2degree(float anAngle);

void computeRotation(MATRIX4& aRotationMatrix);

void setDetector();

void setSource();

void setBeam();

void setScannedObjects();

void setSimulator();


//******************************************************************************
//  Class definition
//******************************************************************************
class GLWidget: public QGLWidget
{
public:
    GLWidget(QWidget *parent = 0);
    GLWidget(QGLFormat& aGLFormat, QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    
protected:
    void initializeGL();
    void paintGL();
    void animate();
    void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent * event);
    void mouseMoveEvent(QMouseEvent *eventMove);
    void mousePressEvent(QMouseEvent *eventPress);
    void mouseReleaseEvent(QMouseEvent *releaseEvent);
    void wheelEvent(QWheelEvent *wheelEvent);
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

        // Create a GL format to OpenGL 3.2 Core
        QGLFormat gl_format;
        gl_format.setVersion(3, 2);
        gl_format.setProfile(QGLFormat::CoreProfile);

        // Set the GL format of the QGLWidget
        GLWidget window(gl_format);
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
    aWindow.setWindowTitle(QApplication::translate("toplevel",
            "gVirtualXRay -- Tutorial 07 using Qt4"));
    
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


//--------------------------------------------------------
GLWidget::GLWidget(QGLFormat& aGLFormat, QWidget *parent):
//--------------------------------------------------------
    QGLWidget(aGLFormat, parent)
//--------------------------------------------------------
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
    // Initialise GLEW
    initialiseGLEW();

    // Initialise OpenGL
    initGL();

    // Initialise the shader from files compressed using the Z library
    initShader();

    // Initialise the geometry of the 3D objects
    load3DObjects();

    // Initialise the position and shape of the X-ray detector
    setDetector();

    // Initialise the position and shape of the X-ray source
    setSource();

    // Initialise the spectrum of the X-ray beam
    setBeam();

    // Initialise the geometry of the scanned object
    setScannedObjects();

    // Initialise the X-ray simulation
    setSimulator();
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

    // Rotate the 3D scene
    g_current_modelview_matrix *= g_scene_rotation_matrix;

    // Wireframe mode
    handle = glGetUniformLocation(shader_id, "g_wireframe");
    glUniform1i(handle, false);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // The first object is the inner cube
    handle = glGetUniformLocation(shader_id, "g_inner_surface");
    glUniform1i(handle, 0);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Display the source
    g_xray_detector.displaySource();

    // Display the detector with the corresponding X-ray image
    g_xray_simulator.display();

    // Rotate the 1st object
    g_current_modelview_matrix *= g_sample_rotation_matrix;

    // Apply the change to the shader program
    applyModelViewMatrix();

    // Wireframe mode
    handle = glGetUniformLocation(shader_id, "g_wireframe");
    glUniform1i(handle, g_wireframe);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Store the state of alpha blending
    if (!g_wireframe)
        {
        pushEnableDisableState(GL_BLEND);
        glEnable(GL_BLEND);
        glDepthMask (GL_FALSE);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_DST_COLOR);
        }

    // Display the object
    if (!g_wireframe)
        {
        g_final_polygon_mesh.display();
        }
    else
        {
        g_final_polygon_mesh.displayWireFrame();
        }

    // Restore the state of alpha blending
    if (!g_wireframe)
        {
        popEnableDisableState();
        glDepthMask (GL_TRUE);
        }

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
    g_sample_rotation_matrix.rotate(1.0, VEC3(1.0, 0.0, 0.0));

    // Compute the X-Ray image
    if (g_xray_simulator.isReady())
        {
        g_xray_simulator.computeImage(g_sample_rotation_matrix);

        // Normalise the X-ray image
        g_xray_simulator.normalise();
        }

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
    double screen_aspect_ratio(double(g_window_width) /
            double(g_window_height));

    // Update the viewport
    glViewport(x, y, w, h);

    // Set up the projection matrix (g_current_projection_matrix)
    loadPerspectiveProjectionMatrix(45.0,
            screen_aspect_ratio,
            0.1 * cm,
            5000.0 * cm);

    // Set up the modelling-viewing matrix (g_current_modelview_matrix)
    loadLookAtModelViewMatrix(0.0 * cm, 0.0 * cm, g_zoom,
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

        case Qt::Key_W:
            g_wireframe = !g_wireframe;
            break;

        default:
        std::cout << event->key() << std::endl;
            break;
        }
}


//---------------------------------------------------
void GLWidget::mouseMoveEvent(QMouseEvent *eventMove)
//---------------------------------------------------
{
    g_current_x_position = eventMove->x();
    g_current_y_position = eventMove->y();

    computeRotation(g_scene_rotation_matrix);
}


//-----------------------------------------------------
void GLWidget::mousePressEvent(QMouseEvent *eventPress)
//-----------------------------------------------------
{
    g_use_arc_ball = true;

    g_last_x_position = eventPress->x();
    g_last_y_position = eventPress->y();

    g_current_x_position = eventPress->x();
    g_current_y_position = eventPress->y();
}


//---------------------------------------------------------
void GLWidget::mouseReleaseEvent(QMouseEvent *releaseEvent)
//---------------------------------------------------------
{
    g_use_arc_ball = false;
}


//------------------------------------------------
void GLWidget::wheelEvent(QWheelEvent *wheelEvent)
//------------------------------------------------
{
    // Scrolling along the Y-axis
    if (std::abs(wheelEvent->delta()) > EPSILON)
        {
        // Change the zoom
        g_zoom += 0.1 * wheelEvent->delta() * cm;

        // Update the projection matrix
        resizeGL(g_window_width, g_window_height);
        }
}


//---------------
void initShader()
//---------------
{
    // Initialise the shaders
    char* p_vertex_shader(0);
    char* p_fragment_shader(0);

    int z_lib_return_code_vertex(0);
    int z_lib_return_code_fragment(0);

    std::string vertex_shader;
    std::string fragment_shader;

    // Decompress the shader using the ZLib
    // Modern OpenGL 3.2 or above is supported
    if (useOpenGL3_2OrAbove())
        {
        z_lib_return_code_vertex   = inflate(g_tutorial_07_gl3_vert,
                sizeof(g_tutorial_07_gl3_vert),
                &p_vertex_shader);

        z_lib_return_code_fragment = inflate(g_tutorial_07_gl3_frag,
                sizeof(g_tutorial_07_gl3_frag),
                &p_fragment_shader);
        }
    // Modern OpenGL 3.2 or above isnot supported
    else
        {
        z_lib_return_code_vertex   = inflate(g_tutorial_07_gl2_vert,
                sizeof(g_tutorial_07_gl2_vert),
                &p_vertex_shader);

        z_lib_return_code_fragment = inflate(g_tutorial_07_gl2_frag,
                sizeof(g_tutorial_07_gl2_frag),
                &p_fragment_shader);
        }

    vertex_shader   = p_vertex_shader;
    fragment_shader = p_fragment_shader;

    delete [] p_vertex_shader;
    delete [] p_fragment_shader;

    p_vertex_shader = 0;
    p_fragment_shader = 0;

    if (z_lib_return_code_vertex <= 0 ||
            z_lib_return_code_fragment <= 0 ||
            !vertex_shader.size() ||
            !fragment_shader.size())
        {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "Cannot decode the shader using ZLib.");
        }

    g_display_shader.setLabels("display.vert", "display.frag");
    g_display_shader.loadSource(vertex_shader, fragment_shader);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
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
    // Create the cube
    VEC3 cube_centre(0, 0, 0);
    RATIONAL_NUMBER length_1( 5.0 * cm);
    RATIONAL_NUMBER length_2(10.0 * cm);

    // Geometric data
    vector<float> p_vertex_set_1;
    vector<float> p_vertex_set_2;

    buildCube(length_1, cube_centre, p_vertex_set_1);
    buildCube(length_2, cube_centre, p_vertex_set_2);

    // Rotate the sample
    g_sample_rotation_matrix.rotate( 30, VEC3(0, 1, 0));
    g_sample_rotation_matrix.rotate( 30, VEC3(0, 0, 1));
    g_sample_rotation_matrix.rotate( 30, VEC3(0, 1, 0));

    // 3D objects as VAOs and VBOs
    PolygonMesh cube_1;
    PolygonMesh cube_2;

    // Set geometry
    cube_1.setExternalData(GL_TRIANGLES,
            &p_vertex_set_1,
            true,
            GL_STATIC_DRAW);

    cube_2.setExternalData(GL_TRIANGLES,
            &p_vertex_set_2,
            true,
            GL_STATIC_DRAW);

    g_final_polygon_mesh = cube_2 - cube_1;
}


//-----------------
void quitCallback()
//-----------------
{
    // Do nothing
}


//----------------------------------------------------
void errorCallback(int error, const char* description)
//----------------------------------------------------
{
    // Throw an error
    throw Exception(__FILE__, __FUNCTION__, __LINE__, description);
}


//---------------------------------
VEC3 getArcballVector(int x, int y)
//---------------------------------
{
    VEC3 P(2.0 * float(x) / float(g_window_width) - 1.0,
           2.0 * float(y) / float(g_window_height) - 1.0,
           0);

    P.setY(-P.getY());

    float OP_squared = P.getX() * P.getX() + P.getY() * P.getY();
    if (OP_squared <= 1.0)
        {
        P.setZ(sqrt(1.0 - OP_squared));  // Pythagore
        }
    else
        {
        P.normalise();  // Nearest point
        }

    return (P);
}


//---------------------------------------
inline float radian2degree(float anAngle)
//---------------------------------------
{
    return (180.0 * anAngle / gVirtualXRay::PI);
}


//--------------------------------------------
void computeRotation(MATRIX4& aRotationMatrix)
//--------------------------------------------
{
    if (g_use_arc_ball)
        {
        if (g_current_x_position != g_last_x_position ||
                g_current_y_position != g_last_y_position)
            {
            VEC3 va(getArcballVector(g_last_x_position,
                    g_last_y_position));

            VEC3 vb(getArcballVector(g_current_x_position,
                    g_current_y_position));

            float angle(g_rotation_speed *
                    radian2degree(acos(min(1.0, va.dotProduct(vb)))));

            VEC3 axis_in_camera_coord(va ^ vb);
            axis_in_camera_coord.normalize();

            MATRIX4 camera2object(aRotationMatrix.getInverse());
            VEC3 axis_in_object_coord = camera2object * axis_in_camera_coord;
            axis_in_object_coord.normalize();

            MATRIX4 rotation_matrix;
            rotation_matrix.rotate(angle, axis_in_object_coord);
            aRotationMatrix = aRotationMatrix * rotation_matrix;

            g_last_x_position = g_current_x_position;
            g_last_y_position = g_current_y_position;
            }
        }
}


//----------------
void setDetector()
//----------------
{
    // Number of pixels of the detector
    Vec2ui number_of_pixels(640, 640);
    g_xray_detector.setNumberOfPixels(number_of_pixels);

    // Size of pixels (in unit of length)
    RATIONAL_NUMBER resolution(320.0 * mm / number_of_pixels.getX());
    g_xray_detector.setResolutionInUnitsOfLengthPerPixel(resolution);

    // Position of the detector
    VEC3 detector_position( 0.0, 0.0, 10.0 * cm);
    g_xray_detector.setDetectorPosition(detector_position);

    // Orientation of the detector
    VEC3 detector_up_vector(0.0, 1.0, 0.0);
    g_xray_detector.setUpVector(detector_up_vector);
}


//--------------
void setSource()
//--------------
{
    // Set the source position and shape
    VEC3 source_position(0.0, 0.0, -40.0 * cm);
    g_xray_detector.setXrayPointSource(source_position);
}


//------------
void setBeam()
//------------
{
    // Set the energy
    RATIONAL_NUMBER incident_energy(80.0 * keV);
    g_xray_beam.initialise(incident_energy);
}


//----------------------
void setScannedObjects()
//----------------------
{
    // Set the material properties
    g_final_polygon_mesh.setHounsfieldValue(0.0);
}


//-----------------
void setSimulator()
//-----------------
{
    // Initialise the X-ray renderer
    g_xray_simulator.initialise(XRayRenderer::OPENGL,
            GL_RGB16F,
            &g_xray_detector,
            &g_xray_beam);

    // Add the geometry to the X-ray renderer
    g_xray_simulator.addInnerSurface(&g_final_polygon_mesh);
}
