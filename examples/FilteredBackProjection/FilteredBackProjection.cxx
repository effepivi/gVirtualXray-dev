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
*   @file       FilteredBackProjection.cxx
*
*   @brief      Demo to illustrate the tomography acquisition and
*               filtered-back-projection processes.
*
*   @version    1.0
*
*   @date       13/09/2016
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
*               http://www.fpvidal.net/, Sept 2016, 2016, version 1.0,
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

#include <exception>
#include <memory>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <string>
#include <ctime>
#include <cmath>
#include <cfloat>

#ifdef __APPLE__
#define GL3_PROTOTYPES 1
#else
#define GL_GLEXT_PROTOTYPES 1
#endif

#define GLFW_INCLUDE_GLCOREARB 1

#include <GLFW/glfw3.h>

#ifndef __Types_h
#include "gVirtualXRay/Types.h"
#endif

#ifndef __Units_h
#include "gVirtualXRay/Units.h"
#endif

#ifndef __SceneGraphNode_h
#include "gVirtualXRay/SceneGraphNode.h"
#endif

#ifndef __PolygonMesh_h
#include "gVirtualXRay/PolygonMesh.h"
#endif

#ifndef __CubeMesh_h
#include "gVirtualXRay/CubeMesh.h"
#endif

#ifndef GVXR_XRAY_BEAM_H
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

#ifndef __TextRenderer_h
#include "gVirtualXRay/TextRenderer.h"
#endif

#ifndef __SphereMesh_h
#include "gVirtualXRay/SphereMesh.h"
#endif

#ifndef __FFT_h
#include "gVirtualXRay/FFT.h"
#endif

#ifndef __Sinogram_h
#include "gVirtualXRay/Sinogram.h"
#endif

#ifndef __CubeeMesh_h
#include "gVirtualXRay/CubeMesh.h"
#endif

#include "display_gl2.frag.h"
#include "display_gl2.vert.h"

#include "display_gl3.frag.h"
#include "display_gl3.vert.h"

#include "display_image_gl2.frag.h"
#include "display_image_gl2.vert.h"

#include "display_image_gl3.frag.h"
#include "display_image_gl3.vert.h"


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Defines
//******************************************************************************
#define PREFIX ".."


//******************************************************************************
//  Constant variables
//******************************************************************************
const GLfloat g_rotation_speed(2.0);

const float g_incident_energy(0.08 * MeV);
const VEC2 g_resolution(0.75 * mm, 5.0 * mm);
const Vec2ui g_number_of_pixels(257, 1);

const double g_initial_fovy(45);        // Field of view along the y-axis
const double g_initial_near(5.0 * cm);  // Near clipping plane
const double g_initial_far(500.0 * cm); // Far clipping plane

const VEC3 g_initial_source_position(   0.0,  20.0 * cm, 0.0);
const VEC3 g_initial_detector_position( 0.0, -20.0 * cm, 0.0);
const VEC3 g_initial_detector_up_vector(0.0, 0.0, 1.0);

typedef enum AcquisitionTypeTag
{
    ROTATE_SAMPLE,
    ROTATE_SOURCE_DETECTOR
} AcquisitionType;


//******************************************************************************
//  Global variables
//******************************************************************************
GLsizei g_main_window_width( 640);
GLsizei g_main_window_height(480);
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

bool g_beam_on(true);

Matrix4x4<GLfloat> g_scene_rotation_matrix;
Matrix4x4<GLfloat> g_sample_rotation_matrix;
Matrix4x4<GLfloat> g_current_2D_projection_matrix;
Matrix4x4<GLfloat> g_text_2D_projection_matrix;
VEC3 g_translation_vector;

std::vector<float> g_p_HU_set;

SceneGraphNode g_scene_graph_node;

#if __cplusplus <= 199711L
    std::auto_ptr<VBO> g_sinogram_rectangle;
    std::auto_ptr<VBO> g_tomography_rectangle;
#else
    std::unique_ptr<VBO> g_sinogram_rectangle;
    std::unique_ptr<VBO> g_tomography_rectangle;
#endif

PolygonMesh g_rotation_axis_geometry;

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;
Shader g_display_shader;
Shader g_display_image_shader;
const VEC3 g_background_colour(0.5, 0.5, 0.5);

bool g_is_xray_image_up_to_date(false);
bool g_update_projection_and_sinogram(false);
bool g_display_wireframe(false);

unsigned int g_angle_number(360 / 1);
RATIONAL_NUMBER g_angle_step(0.5 * 1);
RATIONAL_NUMBER g_first_angle(0);
RATIONAL_NUMBER g_last_angle(g_first_angle +
    g_angle_step * (g_angle_number - 1));
unsigned int g_next_angle_to_process(0);
unsigned int g_current_angle(0);
AcquisitionType g_acquisition_type(ROTATE_SAMPLE);
VEC3 g_rotation_axis(g_initial_detector_up_vector);
VEC3 g_rotation_centre(0, 0, 0);
GLuint g_projection_texture_id(0);
GLuint g_raw_sinogram_texture_id(0);
GLuint g_filtered_sinogram_texture_id(0);
GLuint g_raw_tomography_texture_id(0);
GLuint g_filtered_tomography_texture_id(0);

typedef Image<float>    ProjectionImageType;
typedef Sinogram<float> SinogramType;

ProjectionImageType g_projection_data;
Sinogram<float> g_raw_sinogram_data;
Sinogram<float> g_filtered_sinogram_data;
ProjectionImageType g_raw_tomography_data;
ProjectionImageType g_filtered_tomography_data;

float g_pixel_width(0);

VEC3 g_source_position(g_initial_source_position);
VEC3 g_detector_position(g_initial_detector_position);
VEC3 g_detector_up_vector(g_initial_detector_up_vector);

bool g_display_help(true);
bool g_has_freetype(true);
TextRenderer g_font_rendered;
bool g_run_tomography_acquisition(false);

bool g_use_left_shift(false);
bool g_use_right_shift(false);
bool g_take_screenshot(false);


//******************************************************************************
//  Function declaration
//******************************************************************************
void createPhantom();

void initGLEW();

void initGL();

void initFreeType();

void display();

void framebufferSizeCallback(GLFWwindow* apWindow,
    int aWidth,
    int aHeight);

void keyCallback(GLFWwindow* apWindow,
    int aKey,
    int aScanCode,
    int anAction,
    int aModifierKey);

void mouseButtonCallback(GLFWwindow* apWindow,
    int aButton,
    int aButtonState,
    int aModifierKey);

void cursorPosCallback(GLFWwindow* apWindow,
    double x,
    double y);

void scrollCallback(GLFWwindow* apWindow,
    double xoffset,
    double yoffset);

void quit();

void loadSTLFile();

void loadDetector();

void loadSource();

void loadXRaySimulator();

Vec3<GLfloat> getArcballVector(int x, int y);

void computeRotation(MATRIX4& aRotationMatrix);

bool updateXRayImage();

void errorCallback(int error, const char* description);

void draw();

void createProjection();

void createSinogram();

void createTomography();

void updateProjection();

void updateRawSinogram();

void updateFilteredSinogram();

void updateTomography(const Sinogram<float>& aSinogram,
        ProjectionImageType& aTomographySlice,
        GLuint aTextureID);

void displayProjection();

void displayRawSinogram();

void displayFilteredSinogram();

void displayRawTomography();

void displayFilteredTomography();

void reset();

void displayHelp();

void takeScreenshot(const std::string& aFileName);


//------------------------
inline int ipart(double X)
//------------------------
{
    return (int(X));
}


//---------------------------
inline double fpart(double X)
//---------------------------
{
    return (double(X) - double(ipart(X)));
}


//----------------------------
inline double rfpart(double X)
//----------------------------
{
    return (1.0 - fpart(X));
}


//------------------------------
inline void swap(int& a, int& b)
//------------------------------
{
    int temp(b);
    b = a;
    a = temp;
}


//-------------------------------
inline void plot(int x,
                 int y,
                 float distance,
                 int width,
                 int height,
                 float* apData,
                 float intensity)
//-------------------------------
{
    int index(x + y * width);

    if (index >= 0 && index < width * height)
        {
        apData[x + y * width] += fabs(distance * intensity);
        }
}


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
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "ERROR: cannot initialise GLFW.");
            }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

        // Enable anti-aliasing
        glfwWindowHint(GLFW_SAMPLES, 16);

        // Create a windowed mode window and its OpenGL context
        g_p_main_window_id = glfwCreateWindow(
            g_main_window_width,
            g_main_window_height,
            "FBP Tomography Reconstruction Demo",
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
        initGLEW();
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Check the OpenGL version
        std::cout << "GL:\t" <<
            glGetString(GL_VERSION) << std::endl;

        // Check the GLSL version
        std::cout << "GLSL:\t" <<
            glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

        // Initialise OpenGL
        initGL();
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

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

        // Create the phantom
        createPhantom();

        // Load the data
        loadDetector();
        loadSource();
        loadXRaySimulator();

        // Add the geometry to the X-ray renderer
        loadSTLFile();

    	// Look for all the meshes in the scenegraph
    	std::vector<SceneGraphNode*> p_temp_pull;
    	p_temp_pull.push_back(&g_scene_graph_node);

    	while (p_temp_pull.size())
    	{
    		// Get the last node
    		SceneGraphNode* p_node = p_temp_pull.back();

    		// Remove the node
    		p_temp_pull.pop_back();

    		// Add its children
    		for (unsigned int i = 0; i < p_node->getNumberOfChildren(); ++i)
    		{
    			p_temp_pull.push_back(&p_node->getChild(i));
    		}

    		if (p_node->getPolygonMesh().getVertexNumber())
			{
    			g_xray_renderer.addInnerSurface(p_node);
			}
    	}

        // Set the projection matrix
        GLint width(0);
        GLint height(0);
        glfwGetFramebufferSize(g_p_main_window_id, &width, &height);
        framebufferSizeCallback(g_p_main_window_id, width, height);

        createProjection();
        createSinogram();
        createTomography();

        g_scene_rotation_matrix *= Matrix4x4<GLfloat>::buildRotationMatrix(
            120.0, VEC3(1, 0, 0));

        // Launch the event loop
        while (!glfwWindowShouldClose(g_p_main_window_id))
            {
            if (g_run_tomography_acquisition)
                {
                switch (g_acquisition_type)
                    {
                    case ROTATE_SAMPLE:
                        {
                        Matrix4x4<GLfloat> rotation_matrix;
                        rotation_matrix *=
                            Matrix4x4<GLfloat>::buildTranslationMatrix(
                                g_translation_vector);
                        
                        rotation_matrix *=
                            Matrix4x4<GLfloat>::buildRotationMatrix(
                                -g_angle_step,
                                g_rotation_axis);
                        
                        rotation_matrix *=
                            Matrix4x4<GLfloat>::buildTranslationMatrix(
                                -g_translation_vector);

                        g_sample_rotation_matrix *= rotation_matrix;
                        }
                        break;

                    case ROTATE_SOURCE_DETECTOR:
                        {
                        Matrix4x4<GLfloat> rotation_matrix;
                        rotation_matrix *=
                            Matrix4x4<GLfloat>::buildTranslationMatrix(
                                g_rotation_centre);
                        
                        rotation_matrix *=
                            Matrix4x4<GLfloat>::buildRotationMatrix(
                                g_angle_step,
                                g_rotation_axis);
                        
                        rotation_matrix *=
                            Matrix4x4<GLfloat>::buildTranslationMatrix(
                                -g_rotation_centre);

                        g_source_position   =
                            rotation_matrix * g_source_position;
                        
                        g_detector_position =
                            rotation_matrix * g_detector_position;

                        // Set the source position
                        g_xray_detector.setXrayPointSource(g_source_position);

                        // Use a parallel source
                        g_xray_detector.setParallelBeam();

                        // Set the detector position
                        g_xray_detector.setDetectorPosition(g_detector_position);
                        }
                        break;
                    }

                // Update the X-ray image
                g_is_xray_image_up_to_date = false;

                // Update the projection and sinogram
                g_update_projection_and_sinogram = true;

                // Update the current angle
                g_current_angle = g_next_angle_to_process;
                ++g_next_angle_to_process;

                // Stop the acquisition
                if (g_next_angle_to_process == g_angle_number)
                    {
                    g_run_tomography_acquisition = false;
                    }
                }

            // Update the X-ray image
            bool updated_image(updateXRayImage());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Update the projection and sinogram
            if (g_update_projection_and_sinogram)
                {
                updateProjection();
                checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

                updateRawSinogram();
                checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

                updateFilteredSinogram();
                checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

                updateTomography(g_raw_sinogram_data,
                        g_raw_tomography_data,
                        g_raw_tomography_texture_id);
                checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

                updateTomography(g_filtered_sinogram_data,
                        g_filtered_tomography_data,
                        g_filtered_tomography_texture_id);
                checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

                g_update_projection_and_sinogram = false;
                }

            // Render here
            display();
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Swap front and back buffers
            glfwSwapBuffers(g_p_main_window_id);

            if (/*updated_image ||*/ g_take_screenshot)
                {
                //if (g_first_angle == g_current_angle ||
                //        g_current_angle == (g_angle_number - 1) ||
                //        !(g_current_angle % 60))
                    {
                    //std::cout << g_current_angle << std::endl;

                    std::stringstream filename;
                    static unsigned int screenshot_counter(0);
                    filename << "tga/screenshot_" <<
                        std::setfill('0') <<
                        std::setw(5) <<
                        ++screenshot_counter <<
                        ".tga";

                    takeScreenshot(filename.str());
                    }

                //g_take_screenshot = false;
                }

            // Poll for and process events
            glfwPollEvents();
            }
        }
    catch (const std::exception& error)
        {
        std::cerr << error.what() << std::endl;
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


//------------------
void createPhantom()
//------------------
{
    // 3D objects as VAOs and VBOs
    PolygonMesh cube_1(CubeMesh<float>(16 * mm,  GL_UNSIGNED_BYTE));
    PolygonMesh cube_2(CubeMesh<float>(16 * mm, GL_UNSIGNED_BYTE));
    PolygonMesh cube_3(CubeMesh<float>(64 * mm, GL_UNSIGNED_BYTE));

    cube_1 += VEC3( 10 * mm, -10.0 * mm, 0.0);
    cube_2 += VEC3(-20 * mm,  20.0 * mm, 0.0);

    g_scene_graph_node.addChild("cube_1", cube_1);
    g_scene_graph_node.addChild("cube_2", cube_2);
    g_scene_graph_node.addChild("cube_3", cube_3 - cube_1 - cube_2);

    g_p_HU_set.clear();
    g_p_HU_set.push_back(1000);
    g_p_HU_set.push_back( 500);
    g_p_HU_set.push_back(   0);
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
        if (g_projection_texture_id)
            {
            glDeleteTextures(1, &g_projection_texture_id);
            g_projection_texture_id = 0;
            }

        if (g_raw_sinogram_texture_id)
            {
            glDeleteTextures(1, &g_raw_sinogram_texture_id);
            g_raw_sinogram_texture_id = 0;
            }

        if (g_filtered_sinogram_texture_id)
            {
            glDeleteTextures(1, &g_filtered_sinogram_texture_id);
            g_filtered_sinogram_texture_id = 0;
            }

        if (g_raw_tomography_texture_id)
            {
            glDeleteTextures(1, &g_raw_tomography_texture_id);
            g_raw_tomography_texture_id = 0;
            }

        if (g_filtered_tomography_texture_id)
            {
            glDeleteTextures(1, &g_filtered_tomography_texture_id);
            g_filtered_tomography_texture_id = 0;
            }

        glfwDestroyWindow(g_p_main_window_id);
        g_p_main_window_id = 0;
        glfwTerminate();
        }
}


//-------------
void initGLEW()
//-------------
{
    initialiseGLEW();
}


//-----------
void initGL()
//-----------
{
    std::cout << "OpenGL renderer:   " <<
        glGetString(GL_RENDERER)   << std::endl;
    
    std::cout << "OpenGL version:    " <<
        glGetString(GL_VERSION)    << std::endl;
    
    std::cout << "OpenGL vender:     " <<
        glGetString(GL_VENDOR)     << std::endl;
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
        z_lib_return_code_vertex   = inflate(
            g_display_gl3_vert,
            sizeof(g_display_gl3_vert),
            &p_vertex_shader);
    
        z_lib_return_code_fragment = inflate(
            g_display_gl3_frag,
            sizeof(g_display_gl3_frag),
            &p_fragment_shader);
    
        g_display_shader.setLabels(
            "display_gl3.vert",
            "display_gl3.frag");
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

        g_display_shader.setLabels(
            "display_gl2.vert",
            "display_gl2.frag");
        }

    vertex_shader   = p_vertex_shader;
    fragment_shader = p_fragment_shader;
    
    p_vertex_shader = 0;
    p_fragment_shader = 0;
    
    delete [] p_vertex_shader;
    delete [] p_fragment_shader;

    if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 ||
        !vertex_shader.size() || !fragment_shader.size())
        {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "Cannot decode the shader using ZLib.");
        }
    g_display_shader.loadSource(vertex_shader, fragment_shader);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Display shader
    if (useOpenGL3_2OrAbove())
        {
        z_lib_return_code_vertex   = inflate(
            g_display_image_gl3_vert,
            sizeof(g_display_image_gl3_vert),
            &p_vertex_shader);

        z_lib_return_code_fragment = inflate(
            g_display_image_gl3_frag,
            sizeof(g_display_image_gl3_frag),
            &p_fragment_shader);

        g_display_image_shader.setLabels(
            "display_image_gl3.vert",
            "display_image_gl3.frag");
        }
    else
        {
        z_lib_return_code_vertex   = inflate(
            g_display_image_gl2_vert,
            sizeof(g_display_image_gl2_vert),
            &p_vertex_shader);
        
        z_lib_return_code_fragment = inflate(
            g_display_image_gl2_frag,
            sizeof(g_display_image_gl2_frag),
            &p_fragment_shader);
        
        g_display_image_shader.setLabels(
            "display_image_gl2.vert",
            "display_image_gl2.frag");
        }

    vertex_shader   = p_vertex_shader;
    fragment_shader = p_fragment_shader;
    
    p_vertex_shader = 0;
    p_fragment_shader = 0;
    
    delete [] p_vertex_shader;
    delete [] p_fragment_shader;

    if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 ||
        !vertex_shader.size() || !fragment_shader.size())
        {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "Cannot decode the shader using ZLib.");
        }
    
    g_display_image_shader.loadSource(vertex_shader, fragment_shader);
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
        // Display the 3D elements
        glViewport(
            g_main_window_width / 2,
            0,
            g_main_window_width / 2,
            g_main_window_height);

        // Handle for shader variables
        GLuint handle(0);

        // Enable back face culling
        pushEnableDisableState(GL_CULL_FACE);
        pushEnableDisableState(GL_DEPTH_TEST);
        pushEnableDisableState(GL_BLEND);

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // Enable the shader
        pushShaderProgram();
        g_display_shader.enable();
        GLint shader_id(g_display_shader.getProgramHandle());

        // Define the light colours here
            {
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
            }

        VEC3 light_position(0, 0, g_zoom);
        handle = glGetUniformLocation(shader_id, "light_position");
        glUniform4f(
            handle,
            light_position.getX(),
            light_position.getY(),
            light_position.getZ(), 1.0);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

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

        // Get the material
        const Material& material(g_rotation_axis_geometry.getMaterial());

        // Define colours inshader
        handle = glGetUniformLocation(shader_id, "material_ambient");
        glUniform4fv(handle, 1, &(material.getAmbientColour()[0]));
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "material_diffuse");
        glUniform4fv(handle, 1, &(material.getDiffuseColour()[0]));
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "material_specular");
        glUniform4fv(handle, 1, &(material.getSpecularColour()[0]));
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "material_shininess");
        glUniform1f(handle, material.getShininess());
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        g_rotation_axis_geometry.display();

    	// Look for all the meshes in the scenegraph
    	std::vector<SceneGraphNode*> p_temp_pull;
    	p_temp_pull.push_back(&g_scene_graph_node);

    	while (p_temp_pull.size())
    	{
    		// Get the last node
    		SceneGraphNode* p_node = p_temp_pull.back();

    		// Remove the node
    		p_temp_pull.pop_back();

    		// Add its children
    		for (unsigned int i = 0; i < p_node->getNumberOfChildren(); ++i)
    		{
    			p_temp_pull.push_back(&p_node->getChild(i));
    		}

    		if (p_node->getPolygonMesh().getVertexNumber())
    		{
				// Get the material
				const Material& material(p_node->getPolygonMesh().getMaterial());

				// Define colours inshader
				handle = glGetUniformLocation(shader_id, "material_ambient");
				glUniform4fv(handle, 1, &(material.getAmbientColour()[0]));
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

				handle = glGetUniformLocation(shader_id, "material_diffuse");
				glUniform4fv(handle, 1, &(material.getDiffuseColour()[0]));
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

				handle = glGetUniformLocation(shader_id, "material_specular");
				glUniform4fv(handle, 1, &(material.getSpecularColour()[0]));
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

				handle = glGetUniformLocation(shader_id, "material_shininess");
				glUniform1f(handle, material.getShininess());
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

				if (g_display_wireframe)
					{
					p_node->displayWireFrame();
					}
				else
					{
					p_node->display();
					}
    		}
        }

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

        // Display the X-Ray image
        glDisable(GL_CULL_FACE);
        g_xray_renderer.display(true, true, false);

        // Display the beam
        if (g_beam_on)
            {
            const GLfloat material_ambient[]  = {0.75, 0, 0.5, 0.1};
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
        popEnableDisableState();
        popEnableDisableState();

        // Check the status of OpenGL and of the current FBO
        checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Display the 2D images
        glViewport(0, 0, g_main_window_width / 2, g_main_window_height);

        // Enable the shader
        pushShaderProgram();
        g_display_image_shader.enable();
        shader_id = g_display_image_shader.getProgramHandle();


        pushEnableDisableState(GL_CULL_FACE);
        pushEnableDisableState(GL_DEPTH_TEST);
        pushEnableDisableState(GL_BLEND);

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);

        // Draw the projections
        handle = glGetUniformLocation(shader_id, "g_projection_matrix");
        glUniformMatrix4fv(
            handle,
            1,
            GL_FALSE,
            g_current_2D_projection_matrix.get());
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(shader_id, "imageTexture"), 0);

        pushTexture2D();

        displayProjection();
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        displayRawSinogram();
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        displayFilteredSinogram();
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        displayRawTomography();
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        displayFilteredTomography();
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        popTexture();

        // Disable the shader
        popShaderProgram();

        // Restore the attributes
        popEnableDisableState();
        popEnableDisableState();
        popEnableDisableState();

        // Check the status of OpenGL and of the current FBO
        checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        if (g_has_freetype && g_display_help)
            {
            displayHelp();

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

    draw();

    // Restore the current transformation matrix
    popModelViewMatrix();
    popProjectionMatrix();
}


//-------------------------------------------------------------------------------
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
//-------------------------------------------------------------------------------
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

            case GLFW_KEY_SPACE:
                g_run_tomography_acquisition = !g_run_tomography_acquisition;

                if (g_run_tomography_acquisition)
                    {
                    //if (g_next_angle_to_process == g_angle_number)
                        {
                        reset();
                        }
                    }
                break;

            case GLFW_KEY_B:
                g_beam_on = !g_beam_on;

                // Beam is on
                if (g_beam_on)
                    {
                    // Set the energy
                    g_xray_beam.initialise(g_incident_energy);
                    }
                // Beam is off
                else
                    {
                    // Set the energy
                    g_xray_beam.initialise(0.0 * eV);
                    }

                // Update the X-ray image
                g_is_xray_image_up_to_date = false;
                break;

            case GLFW_KEY_N:
                g_xray_renderer.useNegativeFilteringFlag(
                    !g_xray_renderer.getNegativeFilteringFlag());
                break;

            case GLFW_KEY_W:
                g_display_wireframe = !g_display_wireframe;
                break;

            case GLFW_KEY_T:
                g_take_screenshot = !g_take_screenshot;
                break;

            case GLFW_KEY_KP_ADD:
                g_zoom -= 1.0 * cm;
                framebufferSizeCallback(
                    g_p_main_window_id,
                    g_main_window_width,
                    g_main_window_height);
                break;

            case GLFW_KEY_EQUAL:

                if (g_use_left_shift || g_use_right_shift)
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
                g_use_left_shift = true;
                break;

            case GLFW_KEY_RIGHT_SHIFT:
                g_use_right_shift = true;
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
                g_use_left_shift = false;
                break;

            case GLFW_KEY_RIGHT_SHIFT:
                g_use_right_shift = false;
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

    double screen_aspect_ratio(double(g_main_window_width / 2) /
        double(g_main_window_height));

    loadPerspectiveProjectionMatrix(
        g_initial_fovy,
        screen_aspect_ratio,
        g_initial_near,
        g_initial_far);

    loadLookAtModelViewMatrix(0.0, 0.0, g_zoom,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);


    double x(float(g_number_of_pixels.getX()) /
        float(g_number_of_pixels.getX()));
    
    double y(float(g_angle_number) /
        float(g_number_of_pixels.getX()));
    
    y *= 2.1;
    y += 1.0;

    double w(double(g_main_window_width) / 2.0);
    double h(g_main_window_height);

    double scaling_x(x / w);
    double scaling_y(y / h);

    if (scaling_x < scaling_y)
        {
        scaling_x = y * w / h;
        scaling_y = y;
        }
    else
        {
        scaling_x = x;
        scaling_y = x * h / w;
        }

    g_current_2D_projection_matrix = buildOrthoProjectionMatrix(0,
        scaling_x,
        0,
        scaling_y,
        -1.0,
        1.0);

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

    // Use the arc ball
    if (g_button_state == GLFW_PRESS)
        {
        g_use_arc_ball = true;
        }
    // Stop using the arc ball
    else
        {
        g_use_arc_ball = false;
        }

    double xpos(0);
    double ypos(0);
    glfwGetCursorPos (apWindow, &xpos, &ypos);

    g_last_x_position = xpos;
    g_last_y_position = ypos;

    g_current_x_position = xpos;
    g_current_y_position = ypos;

//    g_take_screenshot = true;
}


//--------------------------------------------------------------
void cursorPosCallback(GLFWwindow* apWindow, double x, double y)
//--------------------------------------------------------------
{
    g_current_x_position = x;
    g_current_y_position = y;

    // Update the rotation matrix if needed
    computeRotation(g_scene_rotation_matrix);

    //g_take_screenshot = true;
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

    //g_take_screenshot = true;
}


//----------------
void loadSTLFile()
//----------------
{
    VEC3 min_corner( FLT_MAX,  FLT_MAX,  FLT_MAX);
    VEC3 max_corner(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    unsigned int i(0);

	// Look for all the meshes in the scenegraph
	std::vector<SceneGraphNode*> p_temp_pull;
	p_temp_pull.push_back(&g_scene_graph_node);

	while (p_temp_pull.size())
	{
		// Get the last node
		SceneGraphNode* p_node = p_temp_pull.back();

		// Remove the node
		p_temp_pull.pop_back();

		// Add its children
		for (unsigned int i = 0; i < p_node->getNumberOfChildren(); ++i)
		{
			p_temp_pull.push_back(&p_node->getChild(i));
		}

		if (p_node->getPolygonMesh().getVertexNumber())
		{
			// Set geometry
			p_node->getPolygonMesh().setHounsfieldValue(g_p_HU_set[i]);

			float red((float(rand()) / (RAND_MAX)) + 1);
			float green((float(rand()) / (RAND_MAX)) + 1);
			float blue((float(rand()) / (RAND_MAX)) + 1);
			float alpha(1.0);

			if (i == 2)
				{
				alpha = 1.0 / 3.0;
				}

			VEC3 base_colour(red, green, blue);
			base_colour.normalise();

			p_node->getPolygonMesh().getMaterial().setAmbientColour(
				base_colour.getX() * 0.19225,
				base_colour.getY() * 0.19225,
				base_colour.getZ() * 0.19225,
				alpha);

			p_node->getPolygonMesh().getMaterial().setDiffuseColour(
				base_colour.getX() * 0.50754,
				base_colour.getY() * 0.50754,
				base_colour.getZ() * 0.50754,
				alpha);

			p_node->getPolygonMesh().getMaterial().setSpecularColour(
				base_colour.getX() * 0.50827,
				base_colour.getY() * 0.50827,
				base_colour.getZ() * 0.50827,
				alpha);

			p_node->getPolygonMesh().getMaterial().setShininess(50);

			if (min_corner.getX() > p_node->getWorldMinCorner().getX())
				{
				min_corner.setX(p_node->getWorldMinCorner().getX());
				}

			if (min_corner.getY() > p_node->getWorldMinCorner().getY())
				{
				min_corner.setY(p_node->getWorldMinCorner().getY());
				}

			if (min_corner.getZ() > p_node->getWorldMinCorner().getZ())
				{
				min_corner.setZ(p_node->getWorldMinCorner().getZ());
				}

			if (max_corner.getX() < p_node->getWorldMaxCorner().getX())
				{
				max_corner.setX(p_node->getWorldMaxCorner().getX());
				}

			if (max_corner.getY() < p_node->getWorldMaxCorner().getY())
				{
				max_corner.setY(p_node->getWorldMaxCorner().getY());
				}

			if (max_corner.getZ() < p_node->getWorldMaxCorner().getZ())
				{
				max_corner.setZ(p_node->getWorldMaxCorner().getZ());
				}
			}

			++i;
		}
	}

    g_rotation_axis_geometry.getMaterial().setAmbientColour(0.0, 0.0, 0.0, 1.0);
    g_rotation_axis_geometry.getMaterial().setDiffuseColour(0.0, 0.0, 0.0, 1.0);
    g_rotation_axis_geometry.getMaterial().setSpecularColour(0.0, 0.0, 0.0, 1.0);
    g_rotation_axis_geometry.getMaterial().setShininess(50);

    // Compute the translation vector
    g_translation_vector = min_corner + (max_corner - min_corner) / 2.0;
    g_sample_rotation_matrix = Matrix4x4<GLfloat>::buildTranslationMatrix(
        -g_translation_vector);

    std::vector<float> p_line_vertex_data;
    std::vector<unsigned int> p_line_index_data;

    p_line_vertex_data.push_back(g_translation_vector.getX() +
        g_initial_detector_up_vector.getX() * 10.0 * cm);
    
    p_line_vertex_data.push_back(g_translation_vector.getY() +
        g_initial_detector_up_vector.getY() * 10.0 * cm);
    
    p_line_vertex_data.push_back(g_translation_vector.getZ() +
        g_initial_detector_up_vector.getZ() * 10.0 * cm);

    p_line_vertex_data.push_back(g_translation_vector.getX() -
        g_initial_detector_up_vector.getX() * 10.0 * cm);

    p_line_vertex_data.push_back(g_translation_vector.getY() -
        g_initial_detector_up_vector.getY() * 10.0 * cm);

    p_line_vertex_data.push_back(g_translation_vector.getZ() -
        g_initial_detector_up_vector.getZ() * 10.0 * cm);

    p_line_index_data.push_back(0);
    p_line_index_data.push_back(1);

    g_rotation_axis_geometry.setInternalData(GL_LINES,
        &p_line_vertex_data[0],
        p_line_vertex_data.size() / 3,
        GL_FLOAT,
        &p_line_index_data[0],
        p_line_index_data.size(),
        GL_UNSIGNED_INT,
        true,
        GL_STATIC_DRAW);

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
                radian2degree(acos(std::min(1.0, va.dotProduct(vb)))));

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
bool updateXRayImage()
//--------------------
{
    bool return_value(false);

    // The X-ray image is not up-to-date
    if (!g_is_xray_image_up_to_date)
        {
        // Compute the X-Ray image
        g_xray_renderer.computeImage(g_sample_rotation_matrix);

        // Normalise the X-ray image
        g_xray_renderer.getEnergyFluenceMinMax();

        // The X-ray image is up-to-date
        g_is_xray_image_up_to_date = true;

        return_value = true;
        }

    return (return_value);
}


//---------
void draw()
//---------
{
    // Get the current draw buffer if needed
    GLint back_buffer(0);
    glGetIntegerv(GL_DRAW_BUFFER, &back_buffer);

    // Clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Display the 3D scene
    render();

    // Make sure all the OpenGL code is done
    glFinish();

    // Restore the draw buffer if needed
    glDrawBuffer(back_buffer);
}


//----------------------------------------------
void createTexture(GLuint& aTextureID,
                   unsigned int aImageWidth,
                   unsigned int aImageHeight,
                   float aPixelWidth,
                   float aPixelHeight,
                   ProjectionImageType& anImage)
//----------------------------------------------
{
    glActiveTexture(GL_TEXTURE0);

    if (aTextureID)
        {
        glDeleteTextures(1, &aTextureID);
        aTextureID = 0;
        }

    anImage = ProjectionImageType(aImageWidth, aImageHeight);
    anImage.setSpacing(VEC3(aPixelWidth, aPixelHeight));

    glGenTextures(1, &aTextureID);

    pushTexture2D();
    glBindTexture(GL_TEXTURE_2D, aTextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_R32F,
        aImageWidth,
        aImageHeight,
        0,
        GL_RED,
        GL_FLOAT,
        anImage.getRawData());

    popTexture();

    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//---------------------
void createProjection()
//---------------------
{
    createTexture(
        g_projection_texture_id,
        g_number_of_pixels.getX(),
        g_angle_number,
        g_pixel_width,
        g_angle_step,
        g_projection_data);

    float x(float(g_number_of_pixels.getX()) / float(g_number_of_pixels.getX()));
    float y(float(g_angle_number) / float(g_number_of_pixels.getX()));
    std::vector<GLfloat> p_vertices;
    p_vertices.push_back(0);
    p_vertices.push_back(0);
    p_vertices.push_back(0);

    p_vertices.push_back(x);
    p_vertices.push_back(0);
    p_vertices.push_back(0);

    p_vertices.push_back(x);
    p_vertices.push_back(y);
    p_vertices.push_back(0);

    p_vertices.push_back(0);
    p_vertices.push_back(0);
    p_vertices.push_back(0);

    p_vertices.push_back(x);
    p_vertices.push_back(y);
    p_vertices.push_back(0);

    p_vertices.push_back(0);
    p_vertices.push_back(y);
    p_vertices.push_back(0);

    // Texture coordinates
    std::vector<float> p_texture_coordinates;

    p_texture_coordinates.push_back(0);
    p_texture_coordinates.push_back(0);
    p_texture_coordinates.push_back(0);

    p_texture_coordinates.push_back(1);
    p_texture_coordinates.push_back(0);
    p_texture_coordinates.push_back(0);

    p_texture_coordinates.push_back(1);
    p_texture_coordinates.push_back(1);
    p_texture_coordinates.push_back(0);

    p_texture_coordinates.push_back(0);
    p_texture_coordinates.push_back(0);
    p_texture_coordinates.push_back(0);

    p_texture_coordinates.push_back(1);
    p_texture_coordinates.push_back(1);
    p_texture_coordinates.push_back(0);

    p_texture_coordinates.push_back(0);
    p_texture_coordinates.push_back(1);
    p_texture_coordinates.push_back(0);

    if (!g_sinogram_rectangle.get())
        {
        g_sinogram_rectangle.reset(createVBO());
        }

    g_sinogram_rectangle->setBufferUsageHint(GL_STATIC_DRAW);

    g_sinogram_rectangle->setVertexData(GL_TRIANGLES,
        p_vertices.size() / 3,
        3,
        GL_FLOAT,
        &p_vertices[0],
        0, 0, GL_NONE, 0,
        p_texture_coordinates.size() / 3,
        3,
        GL_FLOAT,
        &p_texture_coordinates[0]);

    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//-------------------
void createSinogram()
//-------------------
{
    createTexture(
        g_raw_sinogram_texture_id,
        g_number_of_pixels.getX(),
        g_angle_number,
        g_pixel_width,
        g_angle_step,
        g_raw_sinogram_data);
    
    createTexture(
        g_filtered_sinogram_texture_id,
        g_number_of_pixels.getX(),
        g_angle_number,
        g_pixel_width,
        g_angle_step,
        g_filtered_sinogram_data);
}


//---------------------
void createTomography()
//---------------------
{
    createTexture(
        g_raw_tomography_texture_id,
        g_number_of_pixels.getX(),
        g_number_of_pixels.getX(),
        g_pixel_width,
        g_pixel_width,
        g_raw_tomography_data);
    
    createTexture(
        g_filtered_tomography_texture_id,
        g_number_of_pixels.getX(),
        g_number_of_pixels.getX(),
        g_pixel_width,
        g_pixel_width,
        g_filtered_tomography_data);

    std::vector<GLfloat> p_vertices;
    p_vertices.push_back(0);
    p_vertices.push_back(0);
    p_vertices.push_back(0);

    p_vertices.push_back(1);
    p_vertices.push_back(0);
    p_vertices.push_back(0);

    p_vertices.push_back(1);
    p_vertices.push_back(1);
    p_vertices.push_back(0);

    p_vertices.push_back(0);
    p_vertices.push_back(0);
    p_vertices.push_back(0);

    p_vertices.push_back(1);
    p_vertices.push_back(1);
    p_vertices.push_back(0);

    p_vertices.push_back(0);
    p_vertices.push_back(1);
    p_vertices.push_back(0);

    // Texture coordinates
    std::vector<float> p_texture_coordinates;

    p_texture_coordinates.push_back(1);
    p_texture_coordinates.push_back(0);
    p_texture_coordinates.push_back(0);

    p_texture_coordinates.push_back(0);
    p_texture_coordinates.push_back(0);
    p_texture_coordinates.push_back(0);

    p_texture_coordinates.push_back(0);
    p_texture_coordinates.push_back(1);
    p_texture_coordinates.push_back(0);

    p_texture_coordinates.push_back(1);
    p_texture_coordinates.push_back(0);
    p_texture_coordinates.push_back(0);

    p_texture_coordinates.push_back(0);
    p_texture_coordinates.push_back(1);
    p_texture_coordinates.push_back(0);

    p_texture_coordinates.push_back(1);
    p_texture_coordinates.push_back(1);
    p_texture_coordinates.push_back(0);

    if (!g_tomography_rectangle.get())
        {
        g_tomography_rectangle.reset(createVBO());
        }

    g_tomography_rectangle->setBufferUsageHint(GL_STATIC_DRAW);
    g_tomography_rectangle->setVertexData(GL_TRIANGLES,
        p_vertices.size() / 3, 3, GL_FLOAT, &p_vertices[0],
        0, 0, GL_NONE, 0,
        p_texture_coordinates.size() / 3,
        3,
        GL_FLOAT,
        &p_texture_coordinates[0]);

    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//---------------------
void updateProjection()
//---------------------
{
    memcpy(&g_projection_data.getPixel(0, g_current_angle, 0),
            g_xray_renderer.getEnergyFluence().getRawData(),
            sizeof(float) * g_number_of_pixels.getX()
    );

    pushTexture2D();
    glBindTexture(GL_TEXTURE_2D, g_projection_texture_id);
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        g_current_angle,
        g_number_of_pixels.getX(),
        1,
        GL_RED,
        GL_FLOAT,
        g_projection_data.getRawData() +
            g_current_angle * g_number_of_pixels.getX());
    popTexture();
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//----------------------
void updateRawSinogram()
//----------------------
{
    float total_input_energy(g_xray_beam.getTotalEnergy());
    float scaling_factor(float(g_number_of_pixels.getX()) /
        ((g_resolution.getX() * g_number_of_pixels.getX()) / cm)
    );

    // Convert the projection into a sinogram
    Sinogram<float> raw_sinogram;
    
    if (g_beam_on)
        {
        raw_sinogram = (scaling_factor *
            (log(total_input_energy / g_xray_renderer.getEnergyFluence())));
        }
    else
        {
        raw_sinogram = g_xray_renderer.getEnergyFluence();
        }

    memcpy(&(g_raw_sinogram_data(0, g_current_angle,0)),
            raw_sinogram.getRawData(),
            sizeof(float) * g_number_of_pixels.getX());

    pushTexture2D();
    glBindTexture(GL_TEXTURE_2D, g_raw_sinogram_texture_id);
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        g_current_angle,
        g_number_of_pixels.getX(),
        1,
        GL_RED,
        GL_FLOAT,
        g_raw_sinogram_data.getRawData() +
            g_current_angle * g_number_of_pixels.getX());
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    popTexture();
}


//---------------------------
void updateFilteredSinogram()
//---------------------------
{
    float total_input_energy(g_xray_beam.getTotalEnergy());
    float scaling_factor(float(g_number_of_pixels.getX()) /
        ((g_resolution.getX() * g_number_of_pixels.getX()) / cm)
    );

    // Convert the projection into a sinogram
    Sinogram<float> raw_sinogram;

    if (g_beam_on)
        {
        raw_sinogram = (scaling_factor *
            (log(total_input_energy / g_xray_renderer.getEnergyFluence())));
        }
    else
        {
        raw_sinogram = g_xray_renderer.getEnergyFluence();
        }

    // Do not save image
    // raw_sinogram.saveASCII("raw_line.mat");

    // Compute the FFTs
    FFT<float> sinogram_fft(raw_sinogram.getFFT());

    // Compute the ramp filter
    sinogram_fft = sinogram_fft.rhoFilter("hamming", 1.0);

    // Compute the iFFTs
    ProjectionImageType filtered_projection(sinogram_fft.getInverseFFT());
    
    // Do not save image
    // filtered_projection.saveASCII("filtered_line.mat");

    memcpy(&(g_filtered_sinogram_data(0, g_current_angle,0)),
            filtered_projection.getRawData(),
            sizeof(float) * g_number_of_pixels.getX());

    pushTexture2D();
    glBindTexture(GL_TEXTURE_2D, g_filtered_sinogram_texture_id);
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        g_current_angle,
        g_number_of_pixels.getX(),
        1,
        GL_RED,
        GL_FLOAT,
        &g_filtered_sinogram_data(0, g_current_angle, 0));
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    popTexture();
}


//----------------------------------------------------------
void updateTomography(const Sinogram<float>& aSinogram,
                      ProjectionImageType& aTomographySlice,
                      GLuint aTextureID)
//----------------------------------------------------------
{
    Image<float> sinogram(aSinogram.getROI(
        0, g_current_angle,
        0, aSinogram.getWidth(),
        1, aSinogram.getDepth()));
    
    sinogram = sinogram.resize(
        sinogram.getWidth() * 2,
        1,
        aSinogram.getDepth());
    
    unsigned int image_width(2 * floor (sinogram.getWidth() /
        (2.0 * std::sqrt(2.0))));

    Image<float> back_projection(
        sinogram.getWidth(),
        sinogram.getWidth(),
        sinogram.getDepth());

    for (int k(0); k < back_projection.getDepth(); ++k)
        {
        for (int j(0); j < back_projection.getHeight(); ++j)
            {
            for (int i(0); i < back_projection.getWidth(); ++i)
                {
                back_projection(i, j, k) += sinogram.getPixel(i, 0, k);
                }
            }
        }

    back_projection = back_projection.rotate(-(
        g_first_angle + g_angle_step * g_current_angle)) /
            back_projection.getHeight();

    back_projection = back_projection.setCanvasSize(
        image_width,
        image_width,
        back_projection.getDepth());
    
    back_projection *= M_PI / ( back_projection.getHeight());

    back_projection = back_projection.resize(
        aSinogram.getWidth(),
        aSinogram.getWidth(),
        back_projection.getDepth());

    aTomographySlice += back_projection;

    // Update the texture
    pushTexture2D();
    glBindTexture(GL_TEXTURE_2D, aTextureID);
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        0,
        aTomographySlice.getWidth(),
        aTomographySlice.getHeight(),
        GL_RED,
        GL_FLOAT,
        aTomographySlice.getRawData());
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    popTexture();
}


//----------------------
void displayProjection()
//----------------------
{
    float min_value(g_projection_data.getMinValue());
    float max_value(g_projection_data.getMaxValue());

    GLint shader_id(g_display_image_shader.getProgramHandle());

    MATRIX4 model_view;
    model_view.loadIdentity();
    GLuint handle(glGetUniformLocation(shader_id, "g_modelview_matrix"));
    glUniformMatrix4fv(handle, 1, GL_FALSE, model_view.get());
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glBindTexture(GL_TEXTURE_2D, g_projection_texture_id);

    handle = glGetUniformLocation(shader_id, "shift");
    glUniform1f(handle, -min_value);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    handle = glGetUniformLocation(shader_id, "scale");
    float scale(1);
    if (max_value != min_value)
        {
        scale = 1.0 / (max_value - min_value);
        }
    
    glUniform1f(handle, scale);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    g_sinogram_rectangle->display();
}


//-----------------------
void displayRawSinogram()
//-----------------------
{
    float min_value(g_raw_sinogram_data.getMinValue());
    float max_value(g_raw_sinogram_data.getMaxValue());

    GLint shader_id(g_display_image_shader.getProgramHandle());

    float y(float(g_angle_number) / float(g_number_of_pixels.getX()));
    MATRIX4 model_view(MATRIX4::buildTranslationMatrix(0, y * 1.05, 0));
    GLuint handle(glGetUniformLocation(shader_id, "g_modelview_matrix"));
    glUniformMatrix4fv(handle, 1, GL_FALSE, model_view.get());
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glBindTexture(GL_TEXTURE_2D, g_raw_sinogram_texture_id);

    handle = glGetUniformLocation(shader_id, "shift");
    glUniform1f(handle, -min_value);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    handle = glGetUniformLocation(shader_id, "scale");
    float scale(1);
    if (max_value != min_value)
        {
        scale = 1.0 / (max_value - min_value);
        }
    
    glUniform1f(handle, scale);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    g_sinogram_rectangle->display();
}


//----------------------------
void displayFilteredSinogram()
//----------------------------
{
    float min_value(g_filtered_sinogram_data.getMinValue());
    float max_value(g_filtered_sinogram_data.getMaxValue());

    GLint shader_id(g_display_image_shader.getProgramHandle());

    double x(double(g_number_of_pixels.getX()) /
        double(g_number_of_pixels.getX()));
    double y(double(g_angle_number) /
        double(g_number_of_pixels.getX()));
    MATRIX4 model_view(MATRIX4::buildTranslationMatrix(x * 1.05, y * 1.05, 0));
    GLuint handle(glGetUniformLocation(shader_id, "g_modelview_matrix"));
    glUniformMatrix4fv(handle, 1, GL_FALSE, model_view.get());
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glBindTexture(GL_TEXTURE_2D, g_filtered_sinogram_texture_id);

    handle = glGetUniformLocation(shader_id, "shift");
    glUniform1f(handle, -min_value);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    handle = glGetUniformLocation(shader_id, "scale");
    float scale(1);
    if (max_value != min_value)
        {
        scale = 1.0 / (max_value - min_value);
        }
    
    glUniform1f(handle, scale);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    g_sinogram_rectangle->display();
}


//-------------------------
void displayRawTomography()
//-------------------------
{
    float min_value(g_raw_tomography_data.getMinValue());
    float max_value(g_raw_tomography_data.getMaxValue());

    GLint shader_id(g_display_image_shader.getProgramHandle());

    float y(float(g_angle_number) / float(g_number_of_pixels.getX()));
    MATRIX4 model_view(MATRIX4::buildTranslationMatrix(0, y * 2.1, 0));
    GLuint handle(glGetUniformLocation(shader_id, "g_modelview_matrix"));
    glUniformMatrix4fv(handle, 1, GL_FALSE, model_view.get());
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glBindTexture(GL_TEXTURE_2D, g_raw_tomography_texture_id);

    handle = glGetUniformLocation(shader_id, "shift");
    glUniform1f(handle, -min_value);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    handle = glGetUniformLocation(shader_id, "scale");
    float scale(1);
    if (max_value != min_value)
        {
        scale = 1.0 / (max_value - min_value);
        }
    
    glUniform1f(handle, scale);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    g_tomography_rectangle->display();
}


//------------------------------
void displayFilteredTomography()
//------------------------------
{
    float min_value(g_filtered_tomography_data.getMinValue());
    float max_value(g_filtered_tomography_data.getMaxValue());

    GLint shader_id(g_display_image_shader.getProgramHandle());

    double x(double(g_number_of_pixels.getX()) /
        double(g_number_of_pixels.getX()));
    
    double y(double(g_angle_number) /
        double(g_number_of_pixels.getX()));
    MATRIX4 model_view(MATRIX4::buildTranslationMatrix(x * 1.05, y * 2.1, 0));
    GLuint handle(glGetUniformLocation(shader_id, "g_modelview_matrix"));
    glUniformMatrix4fv(handle, 1, GL_FALSE, model_view.get());
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glBindTexture(GL_TEXTURE_2D, g_filtered_tomography_texture_id);

    handle = glGetUniformLocation(shader_id, "shift");
    glUniform1f(handle, -min_value);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    handle = glGetUniformLocation(shader_id, "scale");
    float scale(1);
    if (max_value != min_value)
        {
        scale = 1.0 / (max_value - min_value);
        }
    
    glUniform1f(handle, scale);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    g_tomography_rectangle->display();
}


//----------
void reset()
//----------
{
    unsigned int number_of_pixels(g_number_of_pixels.getX() * g_angle_number);
    for (unsigned int i(0); i < number_of_pixels; ++i)
        {
        g_projection_data.getRawData()[i] = 0;
        g_raw_sinogram_data.getRawData()[i] = 0;
        g_filtered_sinogram_data.getRawData()[i] = 0;
        }

    number_of_pixels = g_number_of_pixels.getX() * g_number_of_pixels.getX();
    for (unsigned int i(0); i < number_of_pixels; ++i)
        {
        g_raw_tomography_data.getRawData()[i] = 0;
        g_filtered_tomography_data.getRawData()[i] = 0;
        }

    pushTexture2D();
    glBindTexture(GL_TEXTURE_2D, g_projection_texture_id);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        0,
        g_number_of_pixels.getX(),
        g_angle_number,
        GL_RED,
        GL_FLOAT,
        g_projection_data.getRawData());
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glBindTexture(GL_TEXTURE_2D, g_raw_sinogram_texture_id);
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        0,
        g_number_of_pixels.getX(),
        g_angle_number,
        GL_RED,
        GL_FLOAT,
        g_raw_sinogram_data.getRawData());
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glBindTexture(GL_TEXTURE_2D, g_filtered_sinogram_texture_id);
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        0,
        g_number_of_pixels.getX(),
        g_angle_number,
        GL_RED,
        GL_FLOAT,
        g_filtered_sinogram_data.getRawData());
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glBindTexture(GL_TEXTURE_2D, g_raw_tomography_texture_id);
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        0,
        g_number_of_pixels.getX(),
        g_number_of_pixels.getX(),
        GL_RED, GL_FLOAT,
        g_raw_tomography_data.getRawData());
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glBindTexture(GL_TEXTURE_2D, g_filtered_tomography_texture_id);
    glTexSubImage2D(GL_TEXTURE_2D,
        0,
        0,
        0,
        g_number_of_pixels.getX(),
        g_number_of_pixels.getX(),
        GL_RED,
        GL_FLOAT,
        g_filtered_tomography_data.getRawData());
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    popTexture();

    g_next_angle_to_process = 0;
    g_current_angle = 0;

    if (g_acquisition_type == ROTATE_SOURCE_DETECTOR)
        {
        g_acquisition_type = ROTATE_SAMPLE;
        }
    else
        {
        g_acquisition_type = ROTATE_SOURCE_DETECTOR;
        }
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

    glViewport(0, 0, g_main_window_width, g_main_window_height);

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

    g_font_rendered.renderText(
        std::string("\'n\': toggle negative image"),
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
        std::string("space bar': start|stop tomography"),
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


void takeScreenshot(const std::string& aFileName)
{
    // Code adapted from:
    // https://www.opengl.org/discussion_boards/showthread.php/129137-Taking-screenshots-How-to

    int HEADER_SIZE = 18;
    unsigned char *buffer;
    int w = g_main_window_width;
    int h = g_main_window_height;
    int buf_size = HEADER_SIZE + (w * h * 3);
    int i;
    unsigned char temp;
    FILE *out_file;

    // open file for output
    if (!(out_file = fopen(aFileName.data(), "wb")))
        {
        return;
        }

    // allocate mem to read from frame buf
    if (!(buffer = (unsigned char *) calloc(1, buf_size)))
        {
        return;
        }

    // set header info
    buffer[2] = 2;    // uncompressed
    buffer[12] = w & 255;
    buffer[13] = w >> 8;
    buffer[14] = h & 255;
    buffer[15] = h >> 8;
    buffer[16] = 24;    // 24 bits per pix

    // read frame buf
    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, buffer + HEADER_SIZE);

    // RGB to BGR
    for (i = HEADER_SIZE; i < buf_size; i += 3)
        {
        temp = buffer[i];
        buffer[i] = buffer[i + 2];
        buffer[i + 2] = temp;
        }

    // write header + color buf to file
    fwrite(buffer, sizeof(unsigned char), buf_size, out_file);

    // cleanup
    fclose(out_file);
    free(buffer);
}
