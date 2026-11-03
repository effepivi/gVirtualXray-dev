/*

Copyright (c) 2020, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*   @file       OpenGLWindow.cxx
*
*   @brief      OpenGLWindow is a class to handle windows. It uses
*               GLFW to support any OpenGL version.
*
*   @version    1.0
*
*   @date       03/03/2020
*
*   @author     Dr Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*   http://www.fpvidal.net/, Mar 2020, 2020, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <iostream>
#include <cstring>

#ifdef HAS_CLIPBOARD
#include "clip.h"
#endif

#ifdef HAS_GLFW
#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>
#endif

// To perform the computations of the X-ray simulation
#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

// To initialise GLEW, to define matrix stacks, to define state stacks, etc.
#ifndef OpenGLUtilities_h
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef GVXR_SHADER_H
#include "gVirtualXRay/Shader.h"
#endif

// To load a scenegraph
#ifndef GVXR_SCENEGRAPH_BINDER_H
#include "gVirtualXRay/SceneGraphBinder.h"
#endif

// To perform the computations of the X-ray simulation
#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef GVXR_LOGGER_H
#include <gVirtualXRay/Logger.h>
#endif

#ifndef GVXR_COORDINATE_SYSTEM_MESH_H
#include "gVirtualXRay/CoordinateSystemMesh.h"
#endif

#ifndef GVXR_TEXT_RENDERER_H
#include "gVirtualXRay/TextRenderer.h"
#endif

#ifndef SGVXR_OpenGLWindow_h
#include "OpenGLWindow.h"
#endif

#ifndef SGVXR_Internal_h
#include "internal.h"
#endif


// Shaders for rendering
#include "display_gl2.frag.h"
#include "display_gl2.vert.h"

#include "display_gl3.frag.h"
#include "display_gl3.vert.h"

#include "display_gl4.frag.h"
#include "display_gl4.vert.h"


//******************************************************************************
//  Namespaces
//******************************************************************************


//******************************************************************************
//  Function declarations
//******************************************************************************
#ifdef HAS_GLFW
void framebufferSizeOpenGlCallback(GLFWwindow* apWindow, int aWidth, int aHeight);
#endif


//******************************************************************************
//  Global variables
//******************************************************************************
extern double g_initial_fovy;
extern double g_initial_near;
extern double g_initial_far;

extern gVirtualXRay::VEC3 g_camera_position;
extern gVirtualXRay::VEC3 g_camera_lookat;
extern gVirtualXRay::VEC3 g_camera_up;

extern bool g_run_loop;
extern bool g_display_beam;
extern bool g_display_wireframe;
extern bool g_use_lighing;
extern bool g_display_detector;
extern bool g_display_normal_vectors;
extern bool g_display_help;

extern gVirtualXRay::XRayBeam g_xray_beam;         // X-ray beam
extern gVirtualXRay::XRayDetector g_xray_detector; // X-ray detector
extern gVirtualXRay::XRayRenderer g_xray_renderer; // X-ray graphics engine
extern gVirtualXRay::TextRenderer g_text_renderer; // Text visualisation

extern gVirtualXRay::Shader g_display_shader;
extern bool g_has_own_gl_context;

// Bias/gain of the shift/scale filter for the display of the X-ray image
extern double g_shift_filter;
extern double g_scale_filter;

extern gVirtualXRay::CoordinateSystemMesh g_world_coordinate_system_mesh;

extern bool g_do_not_call_framebufferSizeOpenGlCallback;

const wchar_t* g_help_message =
        L"Help message:\n"
        L"\n"
        L"- General keyboard controls:\n"
        L"  - <H>          :  Show/hide this message\n"
        L"  - <B>          :  Show/hide the beam\n"
        L"  - <N>          :  Toggle X-ray Positive/Negative display\n"
        L"  - <Q> or <ESC> :  Quit this program\n"
        L"  - <R>          :  Reset the view and geometry\n"
        L"  - <V>          :  Show/hide normal vectors\n"
        L"  - <W>          :  Toggle Wireframe/Solid rendering\n"
        L"  - <Ctrl> + <C> :  Take a screenshot and copy it to the clipboard\n"
        L"  - <Ctrl> + <Shift> + <C> :  Copy the current X-ray image to the clipboard\n"
        L"\n"
        L"- Distance Controls:\n"
        L"  - <S> + \u2191|\u2192  :   Increase SOD, <S> + \u2193|\u2190 to decrease it\n"
        L"  - <D> + \u2191|\u2190  :   Increase ODD, <D> + \u2193|\u2192 to decrease it\n"
        L"  - <1> + \u2190     :   Increase ODD, <1> + \u2192 to decrease it\n"
        L"  - <S> + drag   :   Adjust SOD\n"
        L"  - <D> + drag   :   Adjust ODD\n"
        L"  - <S> + wheel  :   Adjust SOD\n"
        L"  - <D> + wheel  :   Adjust ODD\n"
        L"\n"
        L"- Sample Controls:\n"
        L"  - <2> + \u2190|\u2192  :   Move sample along the source-detector axis\n"
        L"  - <2> + \u2191|\u2193  :   Move sample along the detector right vector\n"
        L"  - <3> + \u2191|\u2193  :   Move sample up/down\n"
        L"  - <4> + \u2190|\u2192  :   Rotate sample\n"
        L"  - <4> + \u2191|\u2193  :   Tilt sample\n"
        L"\n"
        L"- Mouse Controls:\n"
        L"  - Left drag:    Rotate camera\n"
        L"  - Middle drag:  Pan camera\n"
        L"  - Right drag:   Rotate sample\n"
        L"  - Wheel:        Zoom in/out";


//******************************************************************************
//  Function definitions
//******************************************************************************


//---------------------------------------------------
SimpleGVXR::OpenGLWindow::OpenGLWindow(int aVisibilityFlag,
                           int anOpenGLMajorVersion,
                           int anOpenGLMinorVersion):
//---------------------------------------------------
    Window(aVisibilityFlag)
//---------------------------------------------------
{
    // Create the window
    m_context.create("OpenGL", anOpenGLMajorVersion, anOpenGLMinorVersion);

    // Set the window size
#ifdef HAS_GLFW
    if (m_context.getWindowHandle())
    {
        glfwSetWindowSize(m_context.getWindowHandle(), m_width, m_height);

        framebufferSizeOpenGlCallback(m_context.getWindowHandle(), m_width, m_height);

        // Initialize GLFW callbacks
        SimpleGVXR::OpenGLWindow::registerCallbacks();

        // Initialise OpenGL
        SimpleGVXR::OpenGLWindow::initShader();
    }
#endif

    // Window has not been created
    if (!m_context.isReady())
    {
        // Display a warning
        std::cerr << "gvxrWarning:\tOpenGLWindow " <<
            " cannot be created." <<
            std::endl;
    }
}


//-------------------------------------------------------
void SimpleGVXR::OpenGLWindow::setWindowSize(int aWidth, int aHeight)
//-------------------------------------------------------
{
    if (m_auto_unbind_context) m_context.bindContext();

    Window::setWindowSize(aWidth, aHeight);

#ifdef HAS_GLFW
    if (m_context.getWindowHandle())
    	framebufferSizeOpenGlCallback(m_context.getWindowHandle(), aWidth, aHeight);
#endif
}


//------------------------------------------------------------------
void SimpleGVXR::OpenGLWindow::displayScene(bool aSceneRotationFlag)
//------------------------------------------------------------------
{
    // Run the simulation that the interactive changes since the last frame have
    // asked for, if any. It has to happen before the context is bound because
    // it binds and releases it itself
    updateXRayImageIfNeeded();

    if (m_auto_unbind_context) m_context.bindContext();

    // Initialise the viewer if needed
    if (!m_viewer_initialised) initViewer();

#ifdef HAS_GLFW
    // Show the window if needed
    if (!g_has_own_gl_context && m_context.getWindowHandle())
    {
        if (!m_visible)
        {
            if (g_run_loop)
            {
                glfwShowWindow(m_context.getWindowHandle());
                m_visible = true;
            }
            else
            {
                gVirtualXRay::LOGGER.logWarning("Rendering in an hidden window may work, it may not.") << std::endl;
            }
        }

        if (!m_first_pass)
        {
            framebufferSizeOpenGlCallback(m_context.getWindowHandle(), m_width, m_height);
            m_first_pass = true;
            displayScene(aSceneRotationFlag);
        }

        // Clear the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    // Display the 3D scene
    render(aSceneRotationFlag);

    // Display 2D elements
#ifdef HAS_FREETYPE
    if (g_display_help  && g_run_loop) {
        if (g_text_renderer.isInitialised()) {
            gVirtualXRay::MATRIX4 projection_matrix = gVirtualXRay::buildOrthoProjectionMatrix(0, m_width,
                m_height, 0, -1, 1);

            GLfloat p_background_colour [] = {
                m_background_colour[0],
                m_background_colour[1],
                m_background_colour[2],
                0.0
            };

            GLfloat p_font_colour [] = {
                1.0,
                1.0,
                0.0,
                1.0
            };

            gVirtualXRay::pushEnableDisableState(GL_BLEND);
            glEnable (GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            gVirtualXRay::pushEnableDisableState (GL_CULL_FACE);
            glDisable(GL_CULL_FACE);

            gVirtualXRay::pushEnableDisableState (GL_DEPTH_TEST);
            glDisable(GL_DEPTH_TEST);

            g_text_renderer.renderText(g_help_message,
                10, 10,
                projection_matrix,
                p_background_colour,
                p_font_colour);

            gVirtualXRay::popEnableDisableState();
            gVirtualXRay::popEnableDisableState();
            gVirtualXRay::popEnableDisableState();
        }
    }
#endif

    // Swap front and back buffers
    if (!g_has_own_gl_context && m_context.getWindowHandle())
    {
        glfwSwapBuffers(m_context.getWindowHandle());
    }
    // Make sure all the OpenGL code is done
    else {
        glFlush();
    }

#endif

    if (m_auto_unbind_context)
        m_context.unbindContext();
}


//---------------------------------------------
void SimpleGVXR::OpenGLWindow::takeScreenshot()
//---------------------------------------------
{
    if (m_auto_unbind_context) m_context.bindContext();

#ifdef HAS_GLFW
    // Update the window size, just in case
    glfwGetWindowSize(m_context.getWindowHandle(), &m_width, &m_height);
#endif

    m_p_latest_screenshot.resize(3 * m_width * m_height);
    m_latest_screenshot_width = m_width;
    m_latest_screenshot_height = m_height;

    if (!m_p_latest_screenshot.empty())
    {
#ifdef HAS_OPENGL
        // Rows of 3 * m_width bytes are not necessarily a multiple of the
        // default pack alignment (4), which would make OpenGL pad every row
        // and overrun the last one
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, &m_p_latest_screenshot[0]);
#endif
    }

    // if (m_auto_unbind_context) m_context.unbindContext();
}


//-----------------------------------------
void SimpleGVXR::OpenGLWindow::renderLoop()
//-----------------------------------------
{
    if (m_auto_unbind_context) m_context.bindContext();

#ifdef HAS_GLFW
    g_run_loop = true;
    if (m_context.getWindowHandle())
    {
        if (!m_first_pass)
        {
            framebufferSizeOpenGlCallback(m_context.getWindowHandle(), m_width, m_height);
            m_first_pass = true;
            displayScene(false);
        }

        SimpleGVXR::Window::renderLoop();
    }
    g_run_loop = false;
#endif
}


//---------------------------------------------------------------------------------
void SimpleGVXR::OpenGLWindow::setWindowBackGroundColour(float R, float G, float B)
//---------------------------------------------------------------------------------
{
    if (m_auto_unbind_context) m_context.bindContext();

    Window::setWindowBackGroundColour(R, G, B);

#ifdef HAS_OPENGL
    glClearColor(m_background_colour[0], m_background_colour[1], m_background_colour[2], 1.0);
#endif
}


//--------------------------------------------------------------------------------
void SimpleGVXR::OpenGLWindow::setWindowBackGroundColor(float R, float G, float B)
//--------------------------------------------------------------------------------
{
    setWindowBackGroundColour(R, G, B);
}


//---------------------------------------------------
void SimpleGVXR::OpenGLWindow::createWindow(bool aVisibilityFlag)
//---------------------------------------------------
{
	// Do nothing
}


//-----------------------------------------
void SimpleGVXR::OpenGLWindow::initShader()
//-----------------------------------------
{
    if (m_auto_unbind_context) m_context.bindContext();

#ifdef HAS_OPENGL
    gVirtualXRay::LOGGER.logNow("OpenGL vendor: ") << glGetString(GL_VENDOR) << std::endl;
    gVirtualXRay::LOGGER.logNow("OpenGL renderer: ") << glGetString(GL_RENDERER) << std::endl;
    gVirtualXRay::LOGGER.logNow("OpenGL version: ") << glGetString(GL_VERSION) << std::endl;

    gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Enable the Z-buffer
    glEnable (GL_DEPTH_TEST);
    gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Set the background colour
    SimpleGVXR::OpenGLWindow::setWindowBackGroundColour(m_background_colour[0], m_background_colour[1], m_background_colour[2]);
    gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glEnable (GL_MULTISAMPLE);
    gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Initialise the shaders
    char* p_vertex_shader(nullptr);
    char* p_fragment_shader(nullptr);

    int z_lib_return_code_vertex(0);
    int z_lib_return_code_fragment(0);

    // Display shader
    // OpenGL 4.5
    if (gVirtualXRay::useOpenGL45())
    {
        z_lib_return_code_vertex = gVirtualXRay::inflate(
            g_display_gl4_vert,
            sizeof(g_display_gl4_vert),
            &p_vertex_shader);

        z_lib_return_code_fragment = gVirtualXRay::inflate(
            g_display_gl4_frag,
            sizeof(g_display_gl4_frag),
            &p_fragment_shader);

        g_display_shader.setLabels("display_gl4.vert", "display_gl4.frag");
    }
    // OpenGL 3.2
    else if (gVirtualXRay::useOpenGL32())
    {
        z_lib_return_code_vertex = gVirtualXRay::inflate(
            g_display_gl3_vert,
            sizeof(g_display_gl3_vert),
            &p_vertex_shader);

        z_lib_return_code_fragment = gVirtualXRay::inflate(
            g_display_gl3_frag,
            sizeof(g_display_gl3_frag),
            &p_fragment_shader);

        g_display_shader.setLabels("display_gl3.vert", "display_gl3.frag");
    }
    else
    {
        z_lib_return_code_vertex = gVirtualXRay::inflate(
            g_display_gl2_vert,
            sizeof(g_display_gl2_vert),
            &p_vertex_shader);

        z_lib_return_code_fragment = gVirtualXRay::inflate(
            g_display_gl2_frag,
            sizeof(g_display_gl2_frag),
            &p_fragment_shader);

        g_display_shader.setLabels("display_gl2.vert", "display_gl2.frag");
        // throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create shader programs for your chosen OpenGL context.");
    }

    std::string vertex_shader = p_vertex_shader;
    std::string fragment_shader = p_fragment_shader;

    delete[] p_vertex_shader;
    delete[] p_fragment_shader;

    p_vertex_shader = nullptr;
    p_fragment_shader = nullptr;

    if (z_lib_return_code_vertex <= 0 ||
        z_lib_return_code_fragment <= 0 ||
        vertex_shader.empty() ||
        fragment_shader.empty())
    {
        throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
            "Cannot decode the shader using ZLib.");
    }

    g_display_shader.loadSource(vertex_shader, fragment_shader);
    gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
#endif

    initViewer();
}


//------------------------------------------------------------
void SimpleGVXR::OpenGLWindow::render(bool aSceneRotationFlag)
//------------------------------------------------------------
{
    try
    {
#ifdef HAS_OPENGL
        // Check the status of OpenGL and of the current FBO
        gVirtualXRay::checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Enable back face culling
        gVirtualXRay::pushEnableDisableState (GL_CULL_FACE);
        glEnable(GL_CULL_FACE);
        glCullFace (GL_BACK);

        // Enable the shader
        gVirtualXRay::pushShaderProgram();
        g_display_shader.enable();
        GLuint shader_id(g_display_shader.getProgramHandle());

        // Store the current transformation matrix
        gVirtualXRay::pushModelViewMatrix();

        GLint handle(0);
        handle = glGetUniformLocation(shader_id, "g_projection_matrix");
        glUniformMatrix4fv(handle, 1, GL_FALSE,
        gVirtualXRay::g_current_projection_matrix.get());
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "g_modelview_matrix");
        glUniformMatrix4fv(handle, 1, GL_FALSE,
        gVirtualXRay::g_current_modelview_matrix.get());
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // The normal matrix is the inverse transpose of the modelview matrix without the translation,
        // as in gVirtualXRay::applyModelViewMatrix
        gVirtualXRay::MATRIX4 normal_matrix(
            gVirtualXRay::g_current_modelview_matrix.getWithoutTranslation().getInverse().getTranspose());
        handle = glGetUniformLocation(shader_id, "g_normal_matrix");
        glUniformMatrix3fv(handle, 1, GL_FALSE, normal_matrix.get3x3());
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        GLint lighting;
        if (g_use_lighing)
        {
            lighting = 1;
        }
        else
        {
            lighting = 0;
        }
        handle = glGetUniformLocation(shader_id, "g_use_lighting");
        glUniform1iv(handle, 1, &lighting);
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // The light must be set before anything is drawn
        {
            // Define the light colours here
            GLfloat light_global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
            GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
            GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
            GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

            handle = glGetUniformLocation(shader_id, "light_global_ambient");
            glUniform4fv(handle, 1, &light_global_ambient[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "light_ambient");
            glUniform4fv(handle, 1, &light_ambient[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "light_diffuse");
            glUniform4fv(handle, 1, &light_diffuse[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "light_specular");
            glUniform4fv(handle, 1, &light_specular[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            // The head light is located at the camera. The fragment shader
            // works in eye space, i.e. the space in which g_object_position
            // and g_normal are expressed, so the position of the light is
            // converted from world coordinates to eye coordinates using the
            // view matrix. At this stage the modelview matrix only holds the
            // view transformation
            gVirtualXRay::VEC3 light_position(
            gVirtualXRay::g_current_modelview_matrix * g_camera_position);

            handle = glGetUniformLocation(shader_id, "light_position");
            glUniform4f(handle, light_position.getX(), light_position.getY(),
            light_position.getZ(), 1.0);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);
        }

        // Store the current transformation matrix
        gVirtualXRay::pushModelViewMatrix();

        // Look for all the meshes in the scenegraph
        std::vector<gVirtualXRay::SceneGraphNode*> p_temp_pull;
        p_temp_pull.push_back(g_p_scene_graph->getNode("root"));

        while (!p_temp_pull.empty())
        {
            // Get the last node
            gVirtualXRay::SceneGraphNode* p_node = p_temp_pull.back();

            // Remove the node
            p_temp_pull.pop_back();

            // Add its children
            for (unsigned int i = 0; i < p_node->getNumberOfChildren(); ++i)
            {
                p_temp_pull.push_back(&p_node->getChild(i));
            }

            // Add its geometry
            gVirtualXRay::PolygonMesh& mesh = p_node->getPolygonMesh();
            if (mesh.getVertexNumber())
            {
                // Store the current transformation matrix
                gVirtualXRay::pushModelViewMatrix();

                gVirtualXRay::g_current_modelview_matrix *=
                p_node->getWorldTransformationMatrix();

                gVirtualXRay::applyModelViewMatrix();

                // Set the colour of the sample
                handle = glGetUniformLocation(shader_id, "material_ambient");
                glUniform4fv(handle, 1, mesh.getMaterial().getAmbientColour());
                gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                     __FUNCTION__,
                                                     __LINE__);

                handle = glGetUniformLocation(shader_id, "material_diffuse");
                glUniform4fv(handle, 1, mesh.getMaterial().getDiffuseColour());
                gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                     __FUNCTION__,
                                                     __LINE__);

                handle = glGetUniformLocation(shader_id, "material_specular");
                glUniform4fv(handle, 1, mesh.getMaterial().getSpecularColour());
                gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                     __FUNCTION__,
                                                     __LINE__);

                handle = glGetUniformLocation(shader_id, "material_shininess");
                GLfloat shininess = mesh.getMaterial().getShininess();
                glUniform1fv(handle, 1, &shininess);
                gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                     __FUNCTION__,
                                                     __LINE__);

                // Use transparency
                gVirtualXRay::pushEnableDisableState(GL_BLEND);
                GLint blend_src_alpha;
                GLint blend_dst_alpha;
                glGetIntegerv(GL_BLEND_SRC_ALPHA, &blend_src_alpha);
                glGetIntegerv(GL_BLEND_DST_ALPHA, &blend_dst_alpha);

                if (mesh.getMaterial().getAmbientColour()[3] < 0.99)
                {
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                }

                // Display the sample
                if (g_display_wireframe)
                {
                    mesh.displayWireFrame();
                }
                else
                {
                    mesh.display();
                }

                // Restore states
                gVirtualXRay::popEnableDisableState();

                if (mesh.getMaterial().getAmbientColour()[3] < 0.99)
                {
                    glBlendFunc(blend_src_alpha, blend_dst_alpha);
                }

                if (g_display_normal_vectors)
                {
                    GLfloat line_width;
                    glGetFloatv(GL_LINE_WIDTH, &line_width);
                    glLineWidth(3.0f);

                    if (mesh.getVertexNormalNumber())
                    {
                        mesh.displayVertexNormal();
                    }

                    if (mesh.getFaceNormalNumber())
                    {
                        mesh.displayFaceNormal();
                    }
                    glLineWidth(line_width);
                }

                // Restore the current transformation matrix
                gVirtualXRay::popModelViewMatrix();
            }
        }

        // Restore the current transformation matrix
        gVirtualXRay::popModelViewMatrix();

        // Set the colour of the source
        {
            constexpr GLfloat material_ambient[] = {0.0, 0.39225, 0.39225, 1.0};
            constexpr GLfloat material_diffuse[] = {0.0, 0.70754, 0.70754, 1.0};
            constexpr GLfloat material_specular[] = {0.0, 0.708273, 0.708273, 1.0};
            constexpr GLfloat material_shininess = 50.2;

            handle = glGetUniformLocation(shader_id, "material_ambient");
            glUniform4fv(handle, 1, &material_ambient[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "material_diffuse");
            glUniform4fv(handle, 1, &material_diffuse[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "material_specular");
            glUniform4fv(handle, 1, &material_specular[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "material_shininess");
            glUniform1fv(handle, 1, &material_shininess);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);
        }

        // Display the source
        if (!g_display_wireframe)
            g_xray_detector.displaySource();
        else
            g_xray_detector.displaySourceWireframe();

        // Disable back face culling
        glDisable(GL_CULL_FACE);

        // Display the X-Ray image
        if (g_display_detector)
        {
            g_xray_renderer.display(true, true, false, g_shift_filter,
            g_scale_filter);
        }

        // Display the beam
        if (g_display_beam)
        {
            constexpr GLfloat material_ambient[] = { 0.75, 0, 0.5, 0.3 };
            handle = glGetUniformLocation(shader_id, "material_ambient");
            glUniform4fv(handle, 1, &material_ambient[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__,
            __LINE__);

            lighting = 0;
            handle = glGetUniformLocation(shader_id, "g_use_lighting");
            glUniform1iv(handle, 1, &lighting);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__,
            __LINE__);

            g_xray_detector.displayBeam();
        }

        // Draw the first pixel of the image and the detector position
        {
            constexpr GLfloat material_ambient[] = {0.0, 0.0, 1.0, 1.0};
            constexpr GLfloat material_diffuse[] = {0.0, 0.0, 1.0, 1.0};
            constexpr GLfloat material_specular[] = {0.0, 0.0, 1.0, 1.0};
            constexpr GLfloat material_shininess = 50.2;

            handle = glGetUniformLocation(shader_id, "material_ambient");
            glUniform4fv(handle, 1, &material_ambient[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "material_diffuse");
            glUniform4fv(handle, 1, &material_diffuse[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "material_specular");
            glUniform4fv(handle, 1, &material_specular[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "material_shininess");
            glUniform1fv(handle, 1, &material_shininess);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

    		if (!g_display_wireframe) {
		        // Draw the detector position
    			g_xray_detector.displayDetectorPosition();

    			// Draw the first pixel of the image
    			g_xray_detector.displayFirstPixelPosition();
    		}
    		else {
		        // Draw the detector position
    			g_xray_detector.displayDetectorPositionWireframe();

    			// Draw the first pixel of the image
	    		g_xray_detector.displayFirstPixelPositionWireframe();
    		}
        }

        lighting = 0;
        handle = glGetUniformLocation(shader_id, "g_use_lighting");
        glUniform1iv(handle, 1, &lighting);
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        g_world_coordinate_system_mesh.display(shader_id);

        // Disable the shader
        gVirtualXRay::popShaderProgram();

        // Restore the current transformation matrix
        gVirtualXRay::popModelViewMatrix();

        // Restore the attributes
        gVirtualXRay::popEnableDisableState();

        // Check the status of OpenGL and of the current FBO
        gVirtualXRay::checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
#endif
    }
    // Catch exception if any
    catch (const std::exception& error)
    {
        std::cerr << error.what() << std::endl;
    }
}


//------------------------------------
void SimpleGVXR::OpenGLWindow::registerCallbacks()
//------------------------------------
{
#ifdef HAS_GLFW
	if (m_context.getWindowHandle() && !m_callback_registered)
	{
		Window::registerCallbacks();

        // Initialize GLFW callbacks
        glfwSetFramebufferSizeCallback(m_context.getWindowHandle(), framebufferSizeOpenGlCallback);
	}
#endif
}


#ifdef HAS_GLFW
//-----------------------------------------------------------------------------
void framebufferSizeOpenGlCallback(GLFWwindow* apWindow, int width, int height)
//-----------------------------------------------------------------------------
{
    if (height == 0) {
      // Prevent divide by 0
      height = 1;
    }

    double screen_aspect_ratio(static_cast<double>(width) / static_cast<double>(height));
    glViewport(0, 0, width, height);

    SimpleGVXR::Window* p_window = SimpleGVXR::Window::getWindow(apWindow);
    if (p_window)
    {
        if (g_do_not_call_framebufferSizeOpenGlCallback == false)
        {
            g_do_not_call_framebufferSizeOpenGlCallback = true;
            p_window->setWindowSize(width, height);
            g_do_not_call_framebufferSizeOpenGlCallback = false;
        }
    }

    gVirtualXRay::loadPerspectiveProjectionMatrix(g_initial_fovy,
    screen_aspect_ratio, g_initial_near, g_initial_far);
}
#endif
