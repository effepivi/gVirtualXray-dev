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
*   @file       gvxrMarchingCubes.cxx
*
*   @brief      Command line tool to extract an iso-surface using 
*               the Marchin Cubes algorithm.
*
*   @version    1.0
*
*   @date       06/10/2016
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
*               http://www.fpvidal.net/, Sept 2016, 2016, version 1.1,
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

#include <string>
#include <cstdlib>
//#include <ctime>
#include <sstream>
//#include <algorithm>
#include <vector>

#include "cmdline.h"

#ifdef HAS_GLFW
#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>
#endif

#ifndef __Image_h
#include "gVirtualXRay/Image.h"
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

/*

#ifndef GVXR_POLYGON_MESH_H
#include "gVirtualXRay/PolygonMesh.h"
#endif

#ifndef GVXR_CUBE_MESH_H
#include "gVirtualXRay/CubeMesh.h"
#endif

#ifndef GVXR_XRAY_BEAM_H
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef GVXR_XRAY_DETECTOR_H
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef GVXR_MATRIX4X4_H
#include "gVirtualXRay/Matrix4x4.h"
#endif

#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif


*/

#ifdef HAS_GLFW

#ifndef GVXR_SHADER_H
#include "gVirtualXRay/Shader.h"
#endif

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#include "display_gl2.frag.h"
#include "display_gl2.vert.h"

#include "display_gl3.frag.h"
#include "display_gl3.vert.h"

#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Global variables
//******************************************************************************
#ifdef HAS_GLFW
GLFWwindow* g_p_main_window_id(0);
int g_current_main_window_width(600);
int g_current_main_window_height(600);
bool g_use_arc_ball(false);
bool g_use_left_shift_key(false);
bool g_use_right_shift_key(false);
bool g_display_wireframe(false);
float g_zoom(1.0);
Shader g_display_shader;
Matrix4x4<GLfloat> g_scene_rotation_matrix;
#endif

PolygonMesh g_iso_surface;
/*
GLfloat g_incident_energy(0.0);

VEC3 g_source_position;
VEC3 g_detector_position;
VEC3 g_detector_up_vector;

Vec2ui g_number_of_pixels;
GLfloat g_resolution(0.0);

std::vector<PolygonMesh> g_p_polygon_data_set;

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;
*/

//******************************************************************************
//  Function declaration
//******************************************************************************
//void errorCallback(int error, const char* description);
void quit();

#ifdef HAS_GLFW
void framebufferSizeCallback(
        GLFWwindow* apWindow,
        int aWidth,
        int aHeight);

void keyCallback(
        GLFWwindow* apWindow,
        int aKey,
        int aScanCode,
        int anAction,
        int aModifierKey);

void mouseButtonCallback(
        GLFWwindow* apWindow,
        int aButton,
        int aButtonState,
        int aModifierKey);

void cursorPosCallback(GLFWwindow* apWindow, double x, double y);

void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset);

void errorCallback(int error, const char* description);

void initGL();

void display();
#endif

void quit();
/*void idle();
void loadDetector();
void loadSource();
void loadXRaySimulator();
void loadSTLFile(const std::string& aFileName, double aHUValue, double aUnit);
void updateXRayImage();
*/
/*template<typename T> void marchingCubes(const Image<T>& anInputImage,
                                        const std::string& anOutputFileName);

template<typename T> void marchingCubes(const Image<T>& anInputImage,
                                        const char*& anOutputFileName);

*/
//---------
void quit()
//---------
{
#ifdef HAS_GLFW
    if (g_p_main_window_id)
    {
        glfwDestroyWindow(g_p_main_window_id);
        g_p_main_window_id = 0;
        glfwTerminate();
    }
#endif
}

/*
//---------------------------------------------------------------------------
void loadSTLFile(const std::string& aFileName, double aHUValue, double aUnit)
//---------------------------------------------------------------------------
{
       // Set geometry
    PolygonMesh polygon_mesh;
    
    polygon_mesh.setFilename(aFileName.data());
    polygon_mesh.loadSTLFile(false, false, true, true, aUnit, GL_STATIC_DRAW);
    polygon_mesh.setHounsfieldValue(aHUValue);

    g_p_polygon_data_set.push_back(polygon_mesh);
    g_xray_renderer.addInnerSurface(&(g_p_polygon_data_set.back()));
}


//-----------------
void loadDetector()
//-----------------
{
    g_xray_detector.setResolutionInUnitsOfLengthPerPixel(g_resolution);
    g_xray_detector.setNumberOfPixels(g_number_of_pixels);
    g_xray_detector.setDetectorPosition(g_detector_position);
    g_xray_detector.setUpVector(g_detector_up_vector);

    // Print parameters in the console
    std::cout << "Detector position (in cm):     " << g_detector_position / cm << std::endl;
    std::cout << "Detector up vector:            " << g_detector_up_vector     << std::endl;
    std::cout << "Number of pixels:              " << g_number_of_pixels       << std::endl;
    std::cout << "Pixel resolution (in cm):      " << g_resolution / cm        << std::endl;
}


//---------------
void loadSource()
//---------------
{
    // Set the energy
    g_xray_beam.initialise(g_incident_energy);

    // Set the source position
    XRayDetector::SourceShape shape(g_xray_detector.getSourceShape());
    g_xray_detector.setXrayPointSource(g_source_position);
    
    if (shape == XRayDetector::PARALLEL)
        {
        g_xray_detector.setParallelBeam();
        }

    // Print parameters in the console
    std::cout << "Source position (in cm):       " << g_source_position / cm << std::endl;
    std::cout << "Incident beam energy (in keV): " << g_incident_energy / keV << std::endl;
    std::cout << "Source shape:                  " << (shape == XRayDetector::PARALLEL?"Parallel":"Point") << std::endl;
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


//--------------------
void updateXRayImage()
//--------------------
{
    // Compute the X-Ray image
    g_xray_renderer.computeImage(MATRIX4());
}
*/



//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
    try
        {
        // Register the exit callback
        atexit(quit);

        // Read the command line arguments
        gengetopt_args_info ai;
        if (cmdline_parser (argc, argv, &ai) != 0)
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: cannot read the command line arguments.");
        }

        std::string input_file;
        std::string output_file;
        std::vector<double> p_threshold_set;
        std::string error_message;
        bool gui(false);

        if (ai.input_given)
            {
            input_file = ai.input_arg;
            std::cout << "Input file:\t\"" << input_file << "\"" << std::endl;
            }
        else
            {
            error_message += "No input file given.\n";
            }

        if (ai.output_given)
            {
            output_file = ai.output_arg;
            std::cout << "Output file:\t\"" << output_file << "\"" << std::endl;
            }
        else
            {
            error_message += "No output file given.\n";
            }

        if (ai.threshold_given)
            {
            if (ai.threshold_given > 2)
                {
                std::stringstream temp;
                temp << ai.threshold_given << " iso-values given, no more than two iso-values should be given.";
                error_message += temp.str();
                }
            else
                {
                for (unsigned int i(0); i < ai.threshold_given; ++i)
                    {
                    p_threshold_set.push_back(int(ai.threshold_arg[i]));
                    std::cout << "Theshold[" << i << "]:\t\"" << p_threshold_set.back() << "\"" << std::endl;

                    }
                }
            }
        else
            {
            error_message += "No iso-value given.\n";
            }

        if (ai.gui_given)
            {
            gui = ai.gui_flag;
            }

#ifndef HAS_GLFW
        if (gui)
            {
            std::cerr << "WARNING:\tVisualisation using OpenGL has been requested, but it is not available. It will be disable." << std::endl;
            gui = false;
            }
#endif
        std::cout << "Visualisation:\t" << (gui?"ON":"OFF") << std::endl;

        if (error_message.size())
            {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
            }

        // Create an image
        Image<short> image;
        image.load(input_file);

        if (ai.threshold_given == 1)
            {
            g_iso_surface = image.marchingCubes(p_threshold_set[0]);
            }
        else
            {
            g_iso_surface = image.marchingCubes(p_threshold_set[0], p_threshold_set[1]);
            }

        g_iso_surface.setFileName(output_file);
        g_iso_surface.writeSTLFile();

#ifdef HAS_GLFW

        // Set an error callback
        glfwSetErrorCallback(errorCallback);

        // Initialize GLFW
        if (!glfwInit())
            {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: cannot initialise GLFW.");
            }

        // Enable OpenGL 2.1 if possible
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

        // Enable anti-aliasing
        glfwWindowHint(GLFW_SAMPLES, 16);

        // Create the window title
        std::string window_title(
                "gVirtualXRay -- Command Line Tool -- MarchingCubes");

        // Create a windowed mode window and its OpenGL context
        g_p_main_window_id = glfwCreateWindow(
                g_current_main_window_width,
                g_current_main_window_height,
                window_title.data(),
                0,
                0);
        
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

        // Check the OpenGL version
        std::cout << "GL:\t" << glGetString(GL_VERSION) << std::endl;

        // Check the GLSL version
        std::cout << "GLSL:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

        // Check the current FBO
        checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Set the callbacks
        // Initialize GLFW callback
        glfwSetKeyCallback(g_p_main_window_id, keyCallback);
        glfwSetFramebufferSizeCallback(g_p_main_window_id, framebufferSizeCallback);
        glfwSetMouseButtonCallback(g_p_main_window_id, mouseButtonCallback);
        glfwSetCursorPosCallback(g_p_main_window_id, cursorPosCallback);
        glfwSetScrollCallback(g_p_main_window_id, scrollCallback);

        // Intialise OpenGL states
        initGL();

        // Set the projection matrix
        framebufferSizeCallback(g_p_main_window_id, g_current_main_window_width,
                g_current_main_window_height);

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
#endif
        }
    // Catch exception if any
    catch (const std::exception& error)
        {
        std::cerr << error.what() << std::endl;
        return (EXIT_FAILURE);
        }

    // Return an exit code
    return (EXIT_SUCCESS);
}


#ifdef HAS_GLFW
//-------------------------------------------------------------------------------
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
//-------------------------------------------------------------------------------
{
    if (action == GLFW_PRESS)
        {
        switch(key)
            {
            case GLFW_KEY_Q:
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(g_p_main_window_id, GL_TRUE);
                break;

            case GLFW_KEY_UP:
                break;

            case GLFW_KEY_DOWN:
                break;

            case GLFW_KEY_W:
                g_display_wireframe = !g_display_wireframe;
                break;

            case GLFW_KEY_KP_ADD:
                /*g_zoom -= 1.0 * cm;
                framebufferSizeCallback(
                        g_p_main_window_id,
                        g_current_main_window_width,
                        g_current_main_window_height);*/
                break;

            case GLFW_KEY_EQUAL:

                /*if (g_use_left_shift_key || g_use_right_shift_key)
                    {
                    g_zoom -= 1.0 * cm;
                    framebufferSizeCallback(
                            g_p_main_window_id,
                            g_current_main_window_width,
                            g_current_main_window_height);
                    }*/
                break;

            case GLFW_KEY_MINUS:
            case GLFW_KEY_KP_SUBTRACT:
                /*g_zoom += 1.0 * cm;
                framebufferSizeCallback(
                        g_p_main_window_id,
                        g_current_main_window_width,
                        g_current_main_window_height);*/
                break;

            case GLFW_KEY_LEFT_SHIFT:
                g_use_left_shift_key = true;
                break;

            case GLFW_KEY_RIGHT_SHIFT:
                g_use_right_shift_key = true;
                break;

            default:
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
    if (height == 0)
    {
        // Prevent divide by 0
        height = 1;
    }

    g_current_main_window_width  = width;
    g_current_main_window_height = height;

    glViewport(0, 0, g_current_main_window_width, g_current_main_window_height);

    VEC3 min_corner(g_iso_surface.getMinCorner());
    VEC3 max_corner(g_iso_surface.getMaxCorner());
    VEC3 range(max_corner - min_corner);
    VEC3 middle(min_corner + range / 2.0);

    loadLookAtModelViewMatrix(
            middle.getX(), middle.getY() - range.length(), middle.getZ(),
            middle.getX(), middle.getY(), middle.getZ(),
            0.0, 0.0, -1.0);

    loadOrthoProjectionMatrix(
            -g_zoom * range.getX(),
            g_zoom * range.getX(),
            0.0001,
            range.length() * 1.5,
            -g_zoom * range.getZ(),
            g_zoom * range.getZ());
}


//--------------------------------------------
void mouseButtonCallback(GLFWwindow* apWindow,
                         int aButton,
                         int aButtonState,
                         int aModifierKey)
//--------------------------------------------
{
    /*g_button = aButton;
    g_button_state = aButtonState;

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
    g_current_y_position = ypos;*/
}


//-----------------------------------------------------------------------
void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset)
//-----------------------------------------------------------------------
{
    // Scrolling along the Y-axis
   if (fabs(yoffset) > EPSILON)
        {
        g_use_arc_ball = false;
        g_zoom += yoffset * 0.1;

        framebufferSizeCallback(
                apWindow,
                g_current_main_window_width,
                g_current_main_window_height);
        }
}


//--------------------------------------------------------------
void cursorPosCallback(GLFWwindow* apWindow, double x, double y)
//--------------------------------------------------------------
{
  /*  g_current_x_position = x;
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

            // The X-ray image is not up-to-date
            g_is_xray_image_up_to_date = false;
            break;

        case OBJECT:
            computeRotation(g_sample_rotation_matrix);

            // The X-ray image is not up-to-date
            g_is_xray_image_up_to_date = false;
            break;

        default:
            break;
        }*/
}


//----------------------------------------------------
void errorCallback(int error, const char* description)
//----------------------------------------------------
{
    throw Exception(__FILE__, __FUNCTION__, __LINE__, std::string("GLFW error: ") + description);
}


//-----------
void initGL()
//-----------
{
    // Enable the Z-buffer
    glEnable(GL_DEPTH_TEST);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Enable back face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Set the background colour
    glClearColor(0.5, 0.5, 0.5, 1.0);
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
        z_lib_return_code_vertex   = inflate(
                g_display_gl3_vert,
                sizeof(g_display_gl3_vert),
                &p_vertex_shader);

        z_lib_return_code_fragment = inflate(
                g_display_gl3_frag,
                sizeof(g_display_gl3_frag),
                &p_fragment_shader);

        g_display_shader.setLabels("display_gl3.vert", "display_gl3.frag");
        }
    else
        {
        z_lib_return_code_vertex   = inflate(
                g_display_gl2_vert,
                sizeof(g_display_gl2_vert),
                &p_vertex_shader);

        z_lib_return_code_fragment = inflate(
                g_display_gl2_frag,
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

    // Enable the shader
    g_display_shader.enable();
    GLint shader_id(g_display_shader.getProgramHandle());

    GLfloat light_global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    // Handle for shader variables
    GLuint handle(0);

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
    glUniform4f(handle, light_position.getX(), light_position.getY(), light_position.getZ(), 1.0);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    GLint lighting(1);
    handle = glGetUniformLocation(shader_id,"g_use_lighting");
    glUniform1iv(handle, 1, &lighting);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Set the colour of the sample
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


//------------
void display()
//------------
{
    // Store the current transformation matrix
    pushModelViewMatrix();
    pushProjectionMatrix();

    // Clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // Rotate the sample
    g_current_modelview_matrix *= g_scene_rotation_matrix;
    applyModelViewMatrix();

    GLint shader_id(g_display_shader.getProgramHandle());
    GLuint handle(0);
    handle = glGetUniformLocation(shader_id, "g_projection_matrix");
    glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_projection_matrix.get());
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    handle = glGetUniformLocation(shader_id, "g_modelview_matrix");
    glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_modelview_matrix.get());
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    MATRIX4 normal_matrix(g_current_modelview_matrix);
    handle = glGetUniformLocation(shader_id, "g_normal_matrix");
    glUniformMatrix3fv(handle, 1, GL_FALSE, normal_matrix.get3x3());
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Display the sample
    if (g_display_wireframe)
        {
        g_iso_surface.displayWireFrame();
        }
    else
        {
        g_iso_surface.display();
        }

    // Restore the current transformation matrix
    popModelViewMatrix();
    popProjectionMatrix();

    // Check the status of OpenGL and of the current FBO
    checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Make sure all the OpenGL code is done
    glFinish();
}
#endif
