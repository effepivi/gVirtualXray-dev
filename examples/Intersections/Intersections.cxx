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
*   @file        test.cxx
*
*   @brief      Main test program.
*
*   @version    1.0
*
*   @date       11/11/2013
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
#ifdef HAS_GLEW
#include <GL/glew.h>
#endif

#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <cstdlib>
#include <memory>

#define GLFW_INCLUDE_GLCOREARB 1

#include <GLFW/glfw3.h>

#ifndef __Types_h
#include "gVirtualXRay/Types.h"
#endif

#ifndef __Units_h
#include "gVirtualXRay/Units.h"
#endif

#ifndef __CylinderMesh_h
#include "gVirtualXRay/CylinderMesh.h"
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

#ifndef __Shader_h
#include "gVirtualXRay/Shader.h"
#endif

#ifndef __Utilities_h
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef __Exception_h
#include "gVirtualXRay/Exception.h"
#endif

#ifndef __StereoHelper_h
#include "gVirtualXRay/StereoHelper.h"
#endif

#ifndef __TextRenderer_h
#include "gVirtualXRay/TextRenderer.h"
#endif

#include "display_gl2.frag.h"
#include "display_gl2.vert.h"

#include "display_gl3.frag.h"
#include "display_gl3.vert.h"


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;
using namespace std;


//******************************************************************************
//  Defines
//******************************************************************************
#define PREFIX ".."


//******************************************************************************
//  Constant variables
//******************************************************************************
const GLfloat g_rotation_speed(2.0);

const int NONE(0);
const int SCENE(1);
const int OBJECT(2);
const int DETECTOR(3);


//******************************************************************************
//  Global variables
//******************************************************************************
GLsizei g_main_window_width(600);
GLsizei g_main_window_height(600);
GLFWwindow* g_p_main_window_id(0);
GLfloat g_zoom(60.0 * cm);
GLfloat g_eye_offset(2.0 * cm);

int g_button(-1);
int g_button_state(-1);
bool g_use_arc_ball(false);
GLint g_last_x_position(0);
GLint g_last_y_position(0);
GLint g_current_x_position(0);
GLint g_current_y_position(0);

bool g_display_beam(true);

Matrix4x4<GLfloat> g_scene_rotation_matrix;
Matrix4x4<GLfloat> g_detector_rotation_matrix;
Matrix4x4<GLfloat> g_sample_rotation_matrix;
Matrix4x4<GLfloat> g_text_2D_projection_matrix;

GLfloat g_incident_energy(0.08 * MeV);
const GLfloat g_scaling_factor(2.0);
GLfloat g_resolution(0.3 * mm * g_scaling_factor); // 1mm per pixel
Vec2ui g_number_of_pixels(600 / g_scaling_factor, 600 / g_scaling_factor);
VEC3 g_source_position(   0.0, 0.0, -10.0 * cm);
VEC3 g_detector_position( 0.0, 0.0,  10.0 * cm);
VEC3 g_detector_up_vector(0.0, 1.0,   0.0);
const VEC3 g_background_colour(0.5, 0.5, 0.5);

CylinderMesh g_inside_polygon_data(20, 3.0 * cm, 1.0 * cm);
CubeMesh<GLfloat> g_outside_polygon_data(3.0 * cm, GL_UNSIGNED_INT);

#if __cplusplus <= 199711L
    std::auto_ptr<VBO> g_intersection_box_detector_vbo;
#else
    std::unique_ptr<VBO> g_intersection_box_detector_vbo;
#endif

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;
Shader g_display_shader;

bool g_is_xray_image_up_to_date(false);
int g_rotation_mode(NONE);
bool g_display_wireframe(false);
bool g_display_outside_polygon(true);

StereoHelper g_stereo_helper;

// Intraocular distance
const double g_initial_intraocular_distance(1.5 * cm);

// Field of view along the y-axis
const double g_initial_fovy(45);

// Near clipping plane
const double g_initial_near(5.0 * cm);

// Far clipping plane
const double g_initial_far(500.0 * cm);

// Screen projection plane
const double g_initial_screen_projection_plane(10000.0 * cm);

TextRenderer g_font_rendered;
bool g_display_help(true);
bool g_has_freetype(true);

bool g_use_left_shift_key(false);
bool g_use_right_shift_key(false);


//******************************************************************************
//  Function declaration
//******************************************************************************
void initGL();

void initFreeType();

void display();

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

void loadSTLFile();

void loadDetector();

void loadSource();

void loadXRaySimulator();

Vec3<GLfloat> getArcballVector(int x, int y);

void computeRotation(MATRIX4& aRotationMatrix);

void updateXRayImage();

void errorCallback(int error, const char* description);

void draw();

void updateIntersectionSet();

void displayHelp();


//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
    try
        {
        // Set an error callback
        glfwSetErrorCallback(errorCallback);

        // Register the exit callback
        atexit(quit);

        // Initialize GLFW
        if (!glfwInit())
            {
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                    "ERROR: cannot initialise GLFW.");
            }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

        // Enable quad-buffering if possible
        glfwWindowHint(GLFW_STEREO, GL_TRUE);

        // Enable anti-aliasing
        glfwWindowHint(GLFW_SAMPLES, 16);

        // Create a windowed mode window and its OpenGL context
        g_p_main_window_id = glfwCreateWindow(g_main_window_width,
            g_main_window_height,
            "gVirtualXRay demo -- Intersections",
            0,
            0);

        // Window cannot be created
        if (!g_p_main_window_id)
            {
            // Disable quad-buffering
            glfwWindowHint(GLFW_STEREO, GL_FALSE);

            // Create the window
            g_p_main_window_id = glfwCreateWindow(g_main_window_width,
                g_main_window_height,
                "GPU X-Ray Demo",
                0,
                0);
            }
        
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

         // Is stereo enable
        if (g_stereo_helper.enable())
            {
            std::cout << "Stereo is enable." << std::endl;
            }
        else
            {
            std::cout << "Stereo is not enable." << std::endl;
            }
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Check the OpenGL version
        std::cout << "GL:\t" << glGetString(GL_VERSION) << std::endl;

        // Check the GLSL version
        std::cout << "GLSL:\t" <<
            glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

        // Initialise OpenGL
        initGL();

        // Initialise FreeType
        initFreeType();

        // Check the current FBO
        checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Initialize GLFL callback
        glfwSetKeyCallback(g_p_main_window_id, keyCallback);
        glfwSetMouseButtonCallback(g_p_main_window_id, mouseButtonCallback);
        glfwSetCursorPosCallback(g_p_main_window_id, cursorPosCallback);
        glfwSetScrollCallback(g_p_main_window_id, scrollCallback);
        glfwSetFramebufferSizeCallback(g_p_main_window_id,
            framebufferSizeCallback);

        // Load the data
        loadDetector();
        loadSource();
        loadXRaySimulator();

        // Add the geometry to the X-ray renderer
        loadSTLFile();
        g_xray_renderer.addInnerSurface(&g_inside_polygon_data);
        g_xray_renderer.addOuterSurface(&g_outside_polygon_data);

        // Update the X-ray image
        updateXRayImage();

        // Update the intersections
        updateIntersectionSet();

        // Set the stereo parameters
        g_stereo_helper.setIntraocularDistance(g_initial_intraocular_distance);
        g_stereo_helper.setFieldOfViewY(g_initial_fovy);
        g_stereo_helper.setNear(g_initial_near);
        g_stereo_helper.setFar(g_initial_far);
        g_stereo_helper.setScreenProjectionPlane(
            g_initial_screen_projection_plane);

        // Set the projection matrix
        GLint width(0);
        GLint height(0);
        glfwGetFramebufferSize(g_p_main_window_id, &width, &height);
        framebufferSizeCallback(g_p_main_window_id, width, height);

        // Launch the event loop
        while (!glfwWindowShouldClose(g_p_main_window_id))
            {
            // Render here
            display();

            // Swap front and back buffers
            glfwSwapBuffers(g_p_main_window_id);

            // Poll for and process events
            glfwPollEvents();
            }
        }
    // Catch exception if any
    catch (const exception& error)
        {
        cerr << error.what() << endl;
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


//-----------
void initGL()
//-----------
{
    std::cout << "OpenGL renderer:   " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version:    " << glGetString(GL_VERSION)  << std::endl;
    std::cout << "OpenGL vender:     " << glGetString(GL_VENDOR)   << std::endl;
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Enable the Z-buffer
    glEnable(GL_DEPTH_TEST);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Set the background colour
    glClearColor(
        g_background_colour.getX(),
        g_background_colour.getY(),
        g_background_colour.getZ(),
        1.0);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glEnable(GL_MULTISAMPLE);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Initialise the shaders
    char* p_vertex_shader(0);
    char* p_fragment_shader(0);

    int z_lib_return_code_vertex(0);
    int z_lib_return_code_fragment(0);

    std::string vertex_shader;
    std::string fragment_shader;

    // Display shader
    if (useOpenGL3_2OrAbove())
    {
        z_lib_return_code_vertex   = inflate(g_display_gl3_vert,
            sizeof(g_display_gl3_vert),
            &p_vertex_shader);

        z_lib_return_code_fragment = inflate(g_display_gl3_frag,
            sizeof(g_display_gl3_frag),
            &p_fragment_shader);
        g_display_shader.setLabels("display_gl3.vert", "display_gl3.frag");
    }
    else
    {
        z_lib_return_code_vertex   = inflate(g_display_gl2_vert,
            sizeof(g_display_gl2_vert),
            &p_vertex_shader);

        z_lib_return_code_fragment = inflate(g_display_gl2_frag,
            sizeof(g_display_gl2_frag),
            &p_fragment_shader);
        g_display_shader.setLabels("display_gl2.vert", "display_gl2.frag");
    }

    vertex_shader   = p_vertex_shader;
    fragment_shader = p_fragment_shader;

    delete [] p_vertex_shader;
    delete [] p_fragment_shader;

    p_vertex_shader = 0;
    p_fragment_shader = 0;

    if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 ||
            !vertex_shader.size() || !fragment_shader.size())
        {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "Cannot decode the shader using ZLib.");
        }

    g_display_shader.loadSource(vertex_shader, fragment_shader);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//-----------------
void initFreeType()
//-----------------
{
    try
        {
        g_font_rendered.initialise(PREFIX"/data/arial.ttf", 16*27);
        g_display_help = true;
        g_has_freetype = true;
        }
    catch (const std::exception& error)
        {
        std::cerr << "WARNING:\t" << error.what() << std::endl;
        g_display_help = false;
        g_has_freetype = false;
        }
}


//-----------
void render()
//-----------
{
    try
        {
        // Display the 3D

        // Handle for shader variables
        GLuint handle(0);

        // Enable back face culling
        pushEnableDisableState(GL_CULL_FACE);
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);

        // Enable the shader
        pushShaderProgram();
        g_display_shader.enable();
        GLint shader_id(g_display_shader.getProgramHandle());

        // Define the light colours here
        GLfloat light_global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
        GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
        GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

        handle = glGetUniformLocation(shader_id, "light_global_ambient");
        glUniform4fv(handle, 1, &light_global_ambient[0]);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "light_ambient");
        glUniform4fv(handle, 1, &light_ambient[0]);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "light_diffuse");
        glUniform4fv(handle, 1, &light_diffuse[0]);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "light_specular");
        glUniform4fv(handle, 1, &light_specular[0]);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        VEC3 light_position(0, 0, g_zoom);
        handle = glGetUniformLocation(shader_id, "light_position");
        glUniform4f(
            handle,
            light_position[0],
            light_position[1],
            light_position[2],
            1.0);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Define colours
        {
        const GLfloat material_ambient[]  = {0.19225,  0.0, 0.0, 1.0};
        const GLfloat material_diffuse[]  = {0.50754,  0.0, 0.0, 1.0};
        const GLfloat material_specular[] = {0.508273, 0.0, 0.0, 1.0};
        const GLfloat material_shininess = 50.2;

        handle = glGetUniformLocation(shader_id, "material_ambient");
        glUniform4fv(handle, 1, &material_ambient[0]);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "material_diffuse");
        glUniform4fv(handle, 1, &material_diffuse[0]);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "material_specular");
        glUniform4fv(handle, 1, &material_specular[0]);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "material_shininess");
        glUniform1fv(handle, 1, &material_shininess);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        }

        // Check the status of OpenGL and of the current FBO
        checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Store the current transformation matrix
        pushModelViewMatrix();

        // Rotate the sample
        g_current_modelview_matrix *= g_scene_rotation_matrix;

        handle = glGetUniformLocation(shader_id, "g_projection_matrix");
        glUniformMatrix4fv(
            handle,
            1,
            GL_FALSE,
            g_current_projection_matrix.get());
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "g_modelview_matrix");
        glUniformMatrix4fv(
            handle,
            1,
            GL_FALSE,
            g_current_modelview_matrix.get());
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        MATRIX4 normal_matrix(g_current_modelview_matrix);
        handle = glGetUniformLocation(shader_id, "g_normal_matrix");
        glUniformMatrix3fv(handle, 1, GL_FALSE, normal_matrix.get3x3());
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        GLint lighting;
        lighting = 1;
        handle = glGetUniformLocation(shader_id,"g_use_lighting");
        glUniform1iv(handle, 1, &lighting);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Store the current transformation matrix
        pushModelViewMatrix();

        g_current_modelview_matrix *= g_sample_rotation_matrix;
        applyModelViewMatrix();

        if (g_display_wireframe)
            {
            // Disable back face culling
            glDisable(GL_CULL_FACE);

            g_outside_polygon_data.displayWireFrame();
            g_inside_polygon_data.displayWireFrame();

            // Enable back face culling
            glEnable(GL_CULL_FACE);
            }
        else
            {
            g_outside_polygon_data.display();
            }

        //g_inside_polygon_data.displayVertexNormal();
        //g_outside_polygon_data.displayFaceNormal();

        // Restore the current transformation matrix
        popModelViewMatrix();

        // Define colours
        {
        const GLfloat material_ambient[]  = {0.0, 0.39225,  0.39225,  1.0};
        const GLfloat material_diffuse[]  = {0.0, 0.70754,  0.70754,  1.0};
        const GLfloat material_specular[] = {0.0, 0.708273, 0.708273, 1.0};
        const GLfloat material_shininess = 50.2;

        handle = glGetUniformLocation(shader_id, "material_ambient");
        glUniform4fv(handle, 1, &material_ambient[0]);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "material_diffuse");
        glUniform4fv(handle, 1, &material_diffuse[0]);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "material_specular");
        glUniform4fv(handle, 1, &material_specular[0]);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "material_shininess");
        glUniform1fv(handle, 1, &material_shininess);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        }

        // Display the source
        g_xray_detector.displaySource();

        // Disable back face culling
        glDisable(GL_CULL_FACE);

        // Display the X-Ray image
        g_xray_renderer.display(true, false, true);

        // Display the intersections
        g_intersection_box_detector_vbo->display();

        // Display the beam
        if (g_display_beam)
            {
            const GLfloat material_ambient[]  = {0.75, 0, 0.5, 0.3};
            handle = glGetUniformLocation(shader_id, "material_ambient");
            glUniform4fv(handle, 1, &material_ambient[0]);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            lighting = 0;
            handle = glGetUniformLocation(shader_id,"g_use_lighting");
            glUniform1iv(handle, 1, &lighting);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            g_xray_detector.displayBeam();
            }

        // Disable the shader
        popShaderProgram();

        // Restore the current transformation matrix
        popModelViewMatrix();

        // Restore the attributes
        popEnableDisableState();

        // Check the status of OpenGL and of the current FBO
        checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        if (g_has_freetype && g_display_help)
            {
            // Enable the shader
            pushShaderProgram();

            pushEnableDisableState(GL_CULL_FACE);
            pushEnableDisableState(GL_DEPTH_TEST);
            pushEnableDisableState(GL_BLEND);

            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
            glDisable(GL_BLEND);

            displayHelp();

            // Disable the shader
            popShaderProgram();

            // Restore the attributes
            popEnableDisableState();
            popEnableDisableState();
            popEnableDisableState();

            // Check the status of OpenGL and of the current FBO
            checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
            }
        }
    // Catch exception if any
    catch (const std::exception& error)
        {
        std::cerr << error.what() << std::endl;
        }
}


//------------
void display()
//------------
{
    // Store the current transformation matrix
    pushModelViewMatrix();
    pushProjectionMatrix();

    // Update the X-ray image
    updateXRayImage();

    // Use stereo
    if (g_stereo_helper.isActive() && g_stereo_helper.enable())
        {
        draw();
        draw();
        }
    // Cannot use stereo
    else
        {
        draw();
        }

    // Restore the current transformation matrix
    popModelViewMatrix();
    popProjectionMatrix();
}


//----------------------------------
void keyCallback(GLFWwindow* window,
                 int key,
                 int scancode,
                 int action,
                 int mods)
//----------------------------------
{
    if (action == GLFW_PRESS)
        {
        switch (key)
            {
            case GLFW_KEY_H:
                if (g_has_freetype)
                    {
                    g_display_help = !g_display_help;
                    }
                break;

            case GLFW_KEY_Q:
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(g_p_main_window_id, GL_TRUE);
                break;

            case GLFW_KEY_S:
                if (g_stereo_helper.isActive())
                    {
                    g_stereo_helper.disable();
                    }
                else
                    {
                    g_stereo_helper.enable();
                  }
                break;

            case GLFW_KEY_O:
                g_stereo_helper.setIntraocularDistance(
                    g_stereo_helper.getIntraocularDistance() + 5.0 * mm);

                std::cout << "IOD in cm" <<
                    g_stereo_helper.getIntraocularDistance() / cm << std::endl;
                break;

            case GLFW_KEY_P:
                g_stereo_helper.setIntraocularDistance(
                    g_stereo_helper.getIntraocularDistance() - 5.0 * mm);

                std::cout << "IOD in cm" <<
                    g_stereo_helper.getIntraocularDistance() / cm << std::endl;
                break;

            case GLFW_KEY_RIGHT:
                {
                VEC3 translation(g_detector_position - g_source_position);
                translation.normalise();
                g_source_position -= translation * 1.5 * mm;

                // Set the source position
                g_xray_detector.setXrayPointSource(g_source_position);

                // Update the intersections
                updateIntersectionSet();

                // The X-ray image is not up-to-date
                g_is_xray_image_up_to_date = false;
                }
                break;

            case GLFW_KEY_LEFT:
                {
                VEC3 translation(g_detector_position - g_source_position);
                translation.normalise();
                g_source_position += translation * 1.5 * mm;

                // Set the source position
                g_xray_detector.setXrayPointSource(g_source_position);

                // Update the intersections
                updateIntersectionSet();

                // The X-ray image is not up-to-date
                g_is_xray_image_up_to_date = false;
                }
                break;

            case GLFW_KEY_UP:
                {
                VEC3 translation(g_detector_up_vector);
                translation.normalise();
                g_source_position += translation * 1.5 * mm;

                // Set the source position
                g_xray_detector.setXrayPointSource(g_source_position);

                // Update the intersections
                updateIntersectionSet();

                // The X-ray image is not up-to-date
                g_is_xray_image_up_to_date = false;
                }
                break;

            case GLFW_KEY_DOWN:
                {
                VEC3 translation(g_detector_up_vector);
                translation.normalise();
                g_source_position -= translation * 1.5 * mm;

                // Set the source position
                g_xray_detector.setXrayPointSource(g_source_position);

                // Update the intersections
                updateIntersectionSet();

                // The X-ray image is not up-to-date
                g_is_xray_image_up_to_date = false;
                }
                break;

            case GLFW_KEY_B:
                g_display_beam = !g_display_beam;
                break;

            case GLFW_KEY_X:
                // Compute the sinogram
                g_xray_renderer.computeSinogram(
                    g_sample_rotation_matrix,
                    180,
                    1.0);

                // Save the sinogram
                g_xray_renderer.printSinogram(PREFIX"/data/sinogram.mhd");

                // Compute the X-Ray image
                g_xray_renderer.computeImage(g_sample_rotation_matrix);
                break;

            case GLFW_KEY_L:
                g_xray_renderer.computeLBuffer(
                    g_inside_polygon_data,
                    VEC3(),
                    g_sample_rotation_matrix);

                g_xray_renderer.printLBuffer(PREFIX"/data/inside_lbuffer.mhd");

                g_xray_renderer.computeLBuffer(
                    g_outside_polygon_data,
                    VEC3(),
                    g_sample_rotation_matrix);

                g_xray_renderer.printLBuffer(
                    PREFIX"/data/outside_lbuffer2.mhd");

                g_xray_renderer.removeInnerSurfaces();
                g_xray_renderer.removeOuterSurface();
                g_xray_renderer.addOuterSurface(&g_outside_polygon_data);

                g_xray_renderer.computeLBuffer(
                    g_outside_polygon_data,
                    VEC3(),
                    g_sample_rotation_matrix);

                g_xray_renderer.printLBuffer(
                    PREFIX"/data/outside_lbuffer1.mhd");

                g_xray_renderer.addInnerSurface(&g_inside_polygon_data);
                break;

            case GLFW_KEY_C:
                g_xray_renderer.computeImage(g_sample_rotation_matrix);
                g_xray_renderer.printCumulatedLBuffer(
                    PREFIX"/data/cumulated_lbuffer.mhd");
                break;

            case GLFW_KEY_F:
                g_xray_renderer.computeImage(g_sample_rotation_matrix);
                g_xray_renderer.printEnergyFluence(
                    PREFIX"/data/energy_fluence.mhd");
                break;

            case GLFW_KEY_M:
                g_xray_renderer.computeImage(g_sample_rotation_matrix);
                g_xray_renderer.printSumMuxDx(PREFIX"/data/Mux_Dx.mhd");
                break;

            case GLFW_KEY_N:
                g_xray_renderer.useNegativeFilteringFlag(
                    !g_xray_renderer.getNegativeFilteringFlag());
                break;

            case GLFW_KEY_A:
                g_outside_polygon_data.splitFaces(true);

                // Update the intersections
                updateIntersectionSet();

                // The X-ray image is not up-to-date
                g_is_xray_image_up_to_date = false;
                break;

            case GLFW_KEY_1:
                // This is a point source
                if (g_xray_detector.getSourceShape() != XRayDetector::PARALLEL)
                    {
                    // Use a parallel source
                    g_xray_detector.setParallelBeam();
                    }
                // This is a cubic source
                else
                    {
                    g_xray_detector.setPointSource();
                    }

                // Update the intersections
                updateIntersectionSet();

                // The X-ray image is not up-to-date
                g_is_xray_image_up_to_date = false;
                break;

            case GLFW_KEY_2:
                // This is not a line source
                if (g_xray_detector.getSourceShape() != XRayDetector::LINE)
                    {
                    g_xray_detector.setLineSource(
                        g_xray_detector.getXraySourceCentre(),
                        VEC3(1, 0, 0),
                        16,
                        5.0 * mm);
                    }
                // This is a cubic source
                else
                    {
                    g_xray_detector.setPointSource();
                    }

                // Update the intersections
                updateIntersectionSet();

                // The X-ray image is not up-to-date
                g_is_xray_image_up_to_date = false;
                break;

            case GLFW_KEY_3:
                // This is not a square source
                if (g_xray_detector.getSourceShape() != XRayDetector::SQUARE)
                    {
                    g_xray_detector.setSquareSource(
                        g_xray_detector.getXraySourceCentre(),
                        4,
                        5.0 * mm);
                    }
                // This is a cubic source
                else
                    {
                    g_xray_detector.setPointSource();
                    }

                // Update the intersections
                updateIntersectionSet();

                // The X-ray image is not up-to-date
                g_is_xray_image_up_to_date = false;
                break;

            case GLFW_KEY_4:
                // This is not a cubic source
                if (g_xray_detector.getSourceShape() != XRayDetector::CUBE)
                {
                    g_xray_detector.setCubicSource(
                        g_xray_detector.getXraySourceCentre(),
                        3,
                        5.0 * mm);
                }
                // This is a cubic source
                else
                {
                    g_xray_detector.setPointSource();
                }

                // Update the intersections
                updateIntersectionSet();

                // The X-ray image is not up-to-date
                g_is_xray_image_up_to_date = false;
                break;

            case GLFW_KEY_W:
                g_display_wireframe = !g_display_wireframe;
                break;

            case GLFW_KEY_V:
                g_display_outside_polygon = !g_display_outside_polygon;
                break;

            case GLFW_KEY_KP_ADD:
                g_zoom -= 1.0 * cm;
                framebufferSizeCallback(
                    g_p_main_window_id,
                    g_main_window_width,
                    g_main_window_height);
                break;

            case GLFW_KEY_EQUAL:
                if (g_use_left_shift_key || g_use_right_shift_key)
                    {
                    g_zoom -= 1.0 * cm;
                    framebufferSizeCallback(
                        g_p_main_window_id,
                        g_main_window_width,
                        g_main_window_height);
                    }
                break;

            case GLFW_KEY_MINUS:
            case GLFW_KEY_KP_SUBTRACT:
                g_zoom += 1.0 * cm;
                framebufferSizeCallback(
                    g_p_main_window_id,
                    g_main_window_width,
                    g_main_window_height);
                break;

            case GLFW_KEY_LEFT_SHIFT:
                g_use_left_shift_key = true;
                break;

            case GLFW_KEY_RIGHT_SHIFT:
                g_use_right_shift_key = true;
                break;

            default:
                cout << "Key: " << key << endl;
                break;
            }
        }
    else if (action == GLFW_RELEASE)
        {
        switch (key)
            {
            case GLFW_KEY_LEFT_SHIFT:
                g_use_left_shift_key = false;
                break;

            case GLFW_KEY_RIGHT_SHIFT:
                g_use_right_shift_key = false;
                break;
            }
        }
}


//-----------------------------------------------------------------------
void framebufferSizeCallback(GLFWwindow* apWindow, int width, int height)
//-----------------------------------------------------------------------
{
    glViewport(0, 0, width, height);

    if (height == 0)
        {
        // Prevent divide by 0
        height = 1;
        }

    g_main_window_width = width;
    g_main_window_height = height;

    double screen_aspect_ratio(
        double(g_main_window_width) /
        double(g_main_window_height));

    g_stereo_helper.setScreenAspectRatio(screen_aspect_ratio);

    loadPerspectiveProjectionMatrix(
        g_initial_fovy,
        screen_aspect_ratio,
        g_initial_near,
        g_initial_far);

    loadLookAtModelViewMatrix(0.0, 0.0, g_zoom,
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);

    g_text_2D_projection_matrix = buildOrthoProjectionMatrix(0,
            g_main_window_width,
            g_main_window_height,
            0,
            -1.0,
            1.0);
}


//--------------------------------------------
void mouseButtonCallback(GLFWwindow* apWindow,
                         int aButton,
                         int aButtonState,
                         int aModifierKey)
//--------------------------------------------
{
    g_button = aButton;
    g_button_state = aButtonState;

    cout << g_button << "\t" << g_button_state << endl;

    // Use the arc ball
    if (g_button_state == GLFW_PRESS)
    {
        g_use_arc_ball = true;

        // Select the right transformation
        if (g_rotation_mode == NONE)
            {
            if (aModifierKey & GLFW_MOD_SHIFT)
                {
                g_rotation_mode = SCENE;
                }
            else if (aModifierKey & GLFW_MOD_CONTROL)
                {
                g_rotation_mode = OBJECT;
                }
            else if (aModifierKey & GLFW_MOD_ALT)
                {
                g_rotation_mode = DETECTOR;
                }
            else
                {
                switch (g_button)
                    {
                    case GLFW_MOUSE_BUTTON_LEFT:
                        g_rotation_mode = SCENE;
                        break;

                    case GLFW_MOUSE_BUTTON_RIGHT:
                        g_rotation_mode = OBJECT;
                        break;

                    case GLFW_MOUSE_BUTTON_MIDDLE:
                        g_rotation_mode = DETECTOR;
                        break;

                    default:
                        g_rotation_mode = SCENE;
                        break;
                    }
                }
            }
        }
    // Stop using the arc ball
    else
        {
        g_use_arc_ball = false;
        g_rotation_mode = NONE;
        }

    double xpos(0);
    double ypos(0);
    glfwGetCursorPos (apWindow, &xpos, &ypos);

    g_last_x_position = xpos;
    g_last_y_position = ypos;

    g_current_x_position = xpos;
    g_current_y_position = ypos;
}


//--------------------------------------------------------------
void cursorPosCallback(GLFWwindow* apWindow, double x, double y)
//--------------------------------------------------------------
{
    g_current_x_position = x;
    g_current_y_position = y;

    // Update the rotation matrix if needed
    switch (g_rotation_mode)
        {
        case SCENE:
            computeRotation(g_scene_rotation_matrix);
            break;

        case DETECTOR:
            computeRotation(g_detector_rotation_matrix);
            g_xray_detector.setRotationMatrix(g_detector_rotation_matrix);

            // Update the intersections
            updateIntersectionSet();


            // The X-ray image is not up-to-date
            g_is_xray_image_up_to_date = false;
            break;

        case OBJECT:
            computeRotation(g_sample_rotation_matrix);

            // Update the intersections
            updateIntersectionSet();

            // The X-ray image is not up-to-date
            g_is_xray_image_up_to_date = false;
            break;

        default:
            break;
        }
}


//-----------------------------------------------------------------------
void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset)
//-----------------------------------------------------------------------
{
    // Scrolling along the Y-axis
    if (fabs(yoffset) > EPSILON)
        {
        g_use_arc_ball = false;
        g_zoom += yoffset * cm;
        framebufferSizeCallback(
            apWindow,
            g_main_window_width,
            g_main_window_height);
        }
}


//----------------
void loadSTLFile()
//----------------
{
    // Set geometry
    g_inside_polygon_data.setHounsfieldValue(0.0);
    g_outside_polygon_data.setHounsfieldValue(500.0);

    g_inside_polygon_data.translate( VEC3(3.0 * cm, 3.0 * cm, 3.0 * cm));
    g_outside_polygon_data.translate(VEC3(3.0 * cm, 3.0 * cm, 3.0 * cm));

    // The X-ray image is not up-to-date
    g_is_xray_image_up_to_date = false;
}


//-----------------
void loadDetector()
//-----------------
{
    g_xray_detector.setResolutionInUnitsOfLengthPerPixel(g_resolution);
    g_xray_detector.setNumberOfPixels(g_number_of_pixels);
    g_xray_detector.setDetectorPosition(g_detector_position);
    g_xray_detector.setUpVector(g_detector_up_vector);

    // The X-ray image is not up-to-date
    g_is_xray_image_up_to_date = false;
}


//---------------
void loadSource()
//---------------
{
    // Set the energy
    g_xray_beam.initialise(g_incident_energy);

    // Set the source position
    g_xray_detector.setXrayPointSource(g_source_position);

    // Use a parallel source
    g_xray_detector.setParallelBeam();

    // The X-ray image is not up-to-date
    g_is_xray_image_up_to_date = false;
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

    // The X-ray image is not up-to-date
    g_is_xray_image_up_to_date = false;
}


//------------------------------------------
Vec3<GLfloat> getArcballVector(int x, int y)
//------------------------------------------
{
    Vec3<GLfloat> P(2.0 * float(x) / float(g_main_window_width) - 1.0,
        2.0 * float(y) / float(g_main_window_height) - 1.0,
        0);

    P.setY(-P.getY());

    float OP_squared = P.getX() * P.getX() + P.getY() * P.getY();
    if (OP_squared <= 1.0)
        {
        P.setZ(sqrt(1.0 - OP_squared));  // Pythagore
        }
    else
        {
        P.normalise();  // nearest point
        }

    return (P);
}


//--------------------------------
float radian2degree(float anAngle)
//--------------------------------
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
            Vec3<GLfloat> va(getArcballVector(
                g_last_x_position,
                g_last_y_position));

            Vec3<GLfloat> vb(getArcballVector(
                g_current_x_position,
                g_current_y_position));

            float angle(g_rotation_speed *
                radian2degree(acos(min(1.0, va.dotProduct(vb)))));

            Vec3<GLfloat> axis_in_camera_coord(va ^ vb);

            Matrix4x4<GLfloat> camera2object(aRotationMatrix.getInverse());
            Vec3<GLfloat> axis_in_object_coord =
                camera2object * axis_in_camera_coord;

            Matrix4x4<GLfloat> rotation_matrix;
            rotation_matrix.rotate(angle, axis_in_object_coord);
            aRotationMatrix = aRotationMatrix * rotation_matrix;

            g_last_x_position = g_current_x_position;
            g_last_y_position = g_current_y_position;
            }
        }
}


//--------------------
void updateXRayImage()
//--------------------
{
    // The X-ray image is not up-to-date
    if (!g_is_xray_image_up_to_date)
        {
        // Compute the X-Ray image
        g_xray_renderer.computeImage(g_sample_rotation_matrix);

        // Normalise the X-ray image
        g_xray_renderer.getEnergyFluenceMinMax();

        // The X-ray image is up-to-date
        g_is_xray_image_up_to_date = true;
        }
}


//---------
void draw()
//---------
{
    // Get the current draw buffer if needed
    GLint back_buffer(0);
    glGetIntegerv(GL_DRAW_BUFFER, &back_buffer);

    g_stereo_helper.swapEye();

    // Clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Display the 3D scene
    render();

    // Make sure all the OpenGL code is done
    glFinish();

    // Restore the draw buffer if needed
    glDrawBuffer(back_buffer);
}


//--------------------------
void updateIntersectionSet()
//--------------------------
{
    // Get the number of vertices
    unsigned int number_of_vertices(g_outside_polygon_data.getVertexNumber());

    // Compute the positions
    // X lines, two vertices per line, three components per vertex
    std::vector<float> intersection_box_detector_vertex_set(
        number_of_vertices * 2 * 3);

    // Process every vertex of the box
    for (unsigned int i(0); i < number_of_vertices; ++i)
        {
        // Get the vertex position
        VEC3 vertex_position(g_outside_polygon_data.getVertex(i));

        // Transform the vertex
        vertex_position = g_sample_rotation_matrix * vertex_position;

        // Compute the intersection position
        VEC3 intersection_position(vertex_position);

        // Use parallel beam
        VEC3 viewing_vector;
        if (g_xray_detector.getSourceShape() == XRayDetector::PARALLEL)
            {
            VEC3 ray_direction((g_xray_detector.getDetectorPosition() -
                g_xray_detector.getXraySourceCentre()).normal());

            viewing_vector = ray_direction;
            }
        // Use perspective beam
        else
            {
            viewing_vector = (vertex_position - g_source_position).normal();
            }

        // Compute the distance between the ray and the detector
        // (using ray tracing between a ray and a plane)

        GLfloat p_float_4[4];
        g_xray_detector.getPlaneEquation(
            p_float_4[0],
            p_float_4[1],
            p_float_4[2],
            p_float_4[3]);

        VEC3 detector_plane(p_float_4[0], p_float_4[1], p_float_4[2]);

        float distance_intersection_detector =
            (vertex_position.dotProduct(detector_plane) + p_float_4[3]) /
                -viewing_vector.dotProduct(detector_plane);

        // Find the intersection between the ray and the detector
        intersection_position = vertex_position +
            distance_intersection_detector * viewing_vector;

        // Parallel projection
        if (g_xray_detector.getSourceShape() == XRayDetector::PARALLEL)
            {
            // Add the vertex position
            intersection_box_detector_vertex_set.push_back(
                vertex_position.getX());

            intersection_box_detector_vertex_set.push_back(
                vertex_position.getY());

            intersection_box_detector_vertex_set.push_back(
                vertex_position.getZ());
            }
        // Perspective projection
        else
            {
            // Add the source position
            intersection_box_detector_vertex_set.push_back(
                g_source_position.getX());

            intersection_box_detector_vertex_set.push_back(
                g_source_position.getY());

            intersection_box_detector_vertex_set.push_back(
                g_source_position.getZ());
            }

        // Add the intersection position
        intersection_box_detector_vertex_set.push_back(
            intersection_position.getX());

        intersection_box_detector_vertex_set.push_back(
            intersection_position.getY());

        intersection_box_detector_vertex_set.push_back(
            intersection_position.getZ());
        }

    // Create the VBO if needed
    if (!g_intersection_box_detector_vbo.get())
        {
        g_intersection_box_detector_vbo.reset(createVBO());
        }

    // Update the VBO
    g_intersection_box_detector_vbo->setBufferUsageHint(GL_STATIC_DRAW);

    g_intersection_box_detector_vbo->setVertexData(
        GL_LINES,
        intersection_box_detector_vertex_set.size() / 3,
        3,
        GL_FLOAT,
        &intersection_box_detector_vertex_set[0],
        0,
        0,
        GL_NONE,
        0,
        0,
        0,
        GL_NONE,
        0);
}


//----------------
void displayHelp()
//----------------
{
    GLfloat p_background_colour [] = {
        g_background_colour.getX(),
        g_background_colour.getY(),
        g_background_colour.getZ(),
        0.0
    };

    GLfloat p_font_colour [] = {
        1.0,
        1.0,
        0.0,
        1.0
    };

    pushEnableDisableState(GL_BLEND);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int x_position(15);
    int offset(29);
    g_font_rendered.renderText(
        std::string("Help"),
        20,   x_position += offset,
        g_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    g_font_rendered.renderText(
        std::string("\'h\': display|hide help message"),
        40, x_position += offset,
        g_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    g_font_rendered.renderText(std::string("\'s\': toggle stereo"),
        40, x_position += offset,
        g_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    g_font_rendered.renderText(
        std::string("\'o|p\': adjust inter-ocular distance"),
        40, x_position += offset,
        g_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    g_font_rendered.renderText(
        std::string("Arrows: move the source"),
        40, x_position += offset,
        g_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    g_font_rendered.renderText(
        std::string("\'b\': display|hide X-ray beam"),
        40, x_position += offset,
        g_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    g_font_rendered.renderText(
        std::string("\'w\': wireframe|solid rendering"),
        40, x_position += offset,
        g_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    g_font_rendered.renderText(
        std::string("\'n\': toggle negative image"),
        40, x_position += offset,
        g_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    g_font_rendered.renderText(
        std::string("\'x\': save sinogram"),
        40, x_position += offset,
        g_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    g_font_rendered.renderText(
        std::string("\'l\': save L-buffers"),
        40, x_position += offset,
        g_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    g_font_rendered.renderText(
        std::string("\'c\': save cumulated L-buffer"),
        40, x_position += offset,
        g_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    g_font_rendered.renderText(
        std::string("\'f\': save energy fluence"),
        40, x_position += offset,
        g_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    g_font_rendered.renderText(
        std::string("\'1\': use point source|parallel beam"),
        40, x_position += offset,
        g_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    g_font_rendered.renderText(
        std::string("\'2\': use point|line source"),
        40, x_position += offset,
        g_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    g_font_rendered.renderText(
        std::string("\'3\': use point|square source"),
        40, x_position += offset,
        g_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    g_font_rendered.renderText(
        std::string("\'4\': use point|cubic source"),
        40, x_position += offset,
        g_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    g_font_rendered.renderText(
        std::string("\'+|-\': zoom in|out"),
        40, x_position += offset,
        g_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    g_font_rendered.renderText(
        std::string("\'a\': split faces (subdivision)"),
        40, x_position += offset,
        g_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    g_font_rendered.renderText(
        std::string("\'q\', 'Esc': quit"),
        40, x_position += offset,
        g_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    popEnableDisableState();
}
