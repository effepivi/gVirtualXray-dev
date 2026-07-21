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
*   @file       Window.cxx
*
*   @brief      Window is a virtual class to handle windows. The plan is to use
*               GLFW to support any OpenGL version as well as Vulkan.
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

#include <cmath>
#include <list>
#include <algorithm>

#ifdef HAS_GLFW
#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>
#endif

// To load a scenegraph
#ifndef GVXR_SCENEGRAPH_BINDER_H
#include "gVirtualXRay/SceneGraphBinder.h"
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

// To perform the computations of the X-ray simulation
#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef GVXR_LOGGER_H
#include <gVirtualXRay/Logger.h>
#endif

#ifndef SGVXR_Window_h
#include "Window.h"
#endif

#ifndef SGVXR_Acquisition_h
#include "acquisition.h"
#endif

#ifndef SGVXR_Source_h
#include "source.h"
#endif

#ifndef SGVXR_Detector_h
#include "detector.h"
#endif

#ifndef SGVXR_Sample_h
#include "sample.h"
#endif

#ifndef SGVXR_Internal_h
#include "internal.h"
#endif


//******************************************************************************
//  Namespaces
//******************************************************************************


//******************************************************************************
//  Function declarations
//******************************************************************************
#ifdef HAS_GLFW
void keyCallback(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods);

void mouseButtonCallback(GLFWwindow* apWindow,
    int aButton,
    int aButtonState,
    int aModifierKey);

void cursorPosCallback(GLFWwindow* apWindow, double x, double y);

void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset);

void windowCloseCallback(GLFWwindow* apWindow);
#endif

void startArcBallRotation(double x, double y);

void stopArcBallRotation();

bool usingArcBallRotation();

void cursorPositionCallback(double x, double y, int aViewportWidth,
    int aViewportHeight);

void scrollCallback(double xoffset, double yoffset);

void computeRotation(gVirtualXRay::MATRIX4& aRotationMatrix,
                     int aViewportWidth,
                     int aViewportHeight);

gVirtualXRay::Vec3<GLfloat> getArcballVector(int x,
                                             int y,
                                             int aViewportWidth,
                                             int aViewportHeight);

void updateSOD(float aStep);

void updateODD(float aStep);

void updateSample(float aStepX, float aStepY, float aStepZ);

void updateSampleTransformationMatrix();


//******************************************************************************
//  Static class members
//******************************************************************************
std::list<SimpleGVXR::Window*> SimpleGVXR::Window::m_p_window_set;


//******************************************************************************
//  Global variables
//******************************************************************************
extern double g_initial_fovy;
extern double g_initial_near;
extern double g_initial_far;

extern gVirtualXRay::VEC3 g_camera_position;
extern gVirtualXRay::VEC3 g_camera_lookat;
extern gVirtualXRay::VEC3 g_camera_up;

extern gVirtualXRay::Matrix4x4 < GLfloat > g_scene_rotation_matrix;
extern gVirtualXRay::Matrix4x4<GLfloat> g_sample_rotation_matrix;
extern gVirtualXRay::Matrix4x4<GLfloat> g_centre_sample_translation_matrix;
extern gVirtualXRay::Matrix4x4<GLfloat> g_sample_translation_matrix;
extern gVirtualXRay::Matrix4x4<GLfloat> g_sample_transformation_matrix;

extern bool g_run_loop;
extern bool g_display_beam;
extern bool g_display_wireframe;
extern bool g_use_lighing;
extern bool g_display_detector;
extern bool g_display_normal_vectors;

extern gVirtualXRay::XRayBeam g_xray_beam;         // X-ray beam
extern gVirtualXRay::XRayDetector g_xray_detector; // X-ray detector
extern gVirtualXRay::XRayRenderer g_xray_renderer; // X-ray graphics engine

extern gVirtualXRay::Shader g_display_shader;

int g_button(-1);
int g_button_state(-1);
bool g_use_arc_ball(false);
GLint g_last_x_position(0);
GLint g_last_y_position(0);
GLint g_current_x_position(0);
GLint g_current_y_position(0);

// Translate the whole 3-D view
gVirtualXRay::Matrix4x4 < GLfloat > g_scene_translation_matrix;

bool g_S_key_pressed(false);
bool g_D_key_pressed(false);

bool g_1_key_pressed(false);
bool g_2_key_pressed(false);
bool g_3_key_pressed(false);
bool g_4_key_pressed(false);

bool g_left_arrow_key_pressed(false);
bool g_right_arrow_key_pressed(false);
bool g_up_arrow_key_pressed(false);
bool g_down_arrow_key_pressed(false);

const GLfloat ROTATION_SPEED(2.0);


//******************************************************************************
//  Function definitions
//******************************************************************************


//----------------------------------
SimpleGVXR::Window::Window(int aVisibilityFlag):
//----------------------------------
    m_title("gVirtualXRay -- Window"),
    m_visible(aVisibilityFlag),
    m_width(500),
    m_height(500),
    m_viewer_initialised(false),
	m_callback_registered(false),
    m_first_pass(false)
//----------------------------------
{
	// gVirtualXRay::LOGGER.logNow("Create window")  << std::endl;

    m_background_colour[0] = 0.5;
    m_background_colour[1] = 0.5;
    m_background_colour[2] = 0.5;

	m_p_window_set.push_back(this);
}


//---------------------------
SimpleGVXR::Window::~Window()
//---------------------------
{
	// Find the current window in the set of windows
	auto ite = std::find(m_p_window_set.begin(), m_p_window_set.end(), this);

	// If the window is found, remove it from the set
	if (ite != m_p_window_set.end())
	{
		m_p_window_set.erase(ite);
	}
}


//-------------------------------------------------------------
void SimpleGVXR::Window::setWindowSize(int aWidth, int aHeight)
//-------------------------------------------------------------
{
    // The size has changed
    if (m_width != aWidth || m_height != aHeight)
    {
        m_width  = aWidth;
        m_height = aHeight;

#ifdef HAS_GLFW
        if (m_context.getWindowHandle())
        {
        	glfwSetWindowSize(m_context.getWindowHandle(), aWidth, aHeight);
        }
#endif
    }
}


//-----------------------------------------------------------
std::pair<int, int> SimpleGVXR::Window::getWindowSize() const
//-----------------------------------------------------------
{
    return {m_width, m_height};
}


//-----------------------------------
void SimpleGVXR::Window::renderLoop()
//-----------------------------------
{
    g_run_loop = true;

#ifdef HAS_GLFW
    // Launch the event loop
    if (m_context.getWindowHandle())
    {
		while (!glfwWindowShouldClose(m_context.getWindowHandle()) &&
				g_run_loop)
        {
			// Process the event
			glfwPollEvents();

			// Render here
			displayScene(true);
        }

        g_run_loop = false;
    }
#endif
}


//-----------------------------------
void SimpleGVXR::Window::showWindow()
//-----------------------------------
{
#ifdef HAS_GLFW
    // The window exists
    if (m_context.getWindowHandle())
    {
        // It is hidden
        if (!glfwGetWindowAttrib(m_context.getWindowHandle(),
            GLFW_VISIBLE))
        {
            glfwShowWindow(m_context.getWindowHandle());
            m_visible = true;
        }
    }
#endif
}


//-----------------------------------
void SimpleGVXR::Window::hideWindow()
//-----------------------------------
{
#ifdef HAS_GLFW
    // The window exists
    if (m_context.getWindowHandle())
    {
        // It is shown
        if (glfwGetWindowAttrib(m_context.getWindowHandle(),
            GLFW_VISIBLE))
        {
            glfwHideWindow(m_context.getWindowHandle());
            m_visible = false;
        }
    }
#endif

    g_run_loop = false;
}


//------------------------------------------------------------------
void SimpleGVXR::Window::setPosition(int aPositionX, int aPositionY)
//------------------------------------------------------------------
{
#ifdef HAS_GLFW
    // The window exists
    if (m_context.getWindowHandle())
    {
        glfwSetWindowPos(m_context.getWindowHandle(), aPositionX, aPositionY);
    }
#endif
}


//---------------------------------------------------------------------------
void SimpleGVXR::Window::setWindowBackGroundColour(float R, float G, float B)
//---------------------------------------------------------------------------
{
    m_background_colour[0] = R;
    m_background_colour[1] = G;
    m_background_colour[2] = B;
}


//--------------------------------------------------------------------------
void SimpleGVXR::Window::setWindowBackGroundColor(float R, float G, float B)
//--------------------------------------------------------------------------
{
    setWindowBackGroundColour(R, G, B);
}


//--------------------------------------
void SimpleGVXR::Window::destroyWindow()
//--------------------------------------
{
    g_run_loop = false;
	m_context.release();
}


//-----------------------------------
void SimpleGVXR::Window::initViewer()
//-----------------------------------
{
    if (!m_viewer_initialised && !g_xray_detector.getSourcePositionSet().empty())
    {
        auto distance_source_object =
            static_cast<float>(g_xray_detector.getSourcePositionSet()[0].length());

        auto distance_detector_object =
            static_cast<float>(g_xray_detector.getDetectorPosition().length());

        const float distance = std::max(distance_detector_object,
                                  distance_source_object);

        const float half_fovy = (static_cast<float>(M_PI * g_initial_fovy) / 2.0f) / 180.0f;

        g_camera_position[2] = atan(half_fovy) * distance * 8;

        if (g_p_scene_graph)
        {
            g_initial_near = (g_p_scene_graph->getBoundingBox().second -
                g_p_scene_graph->getBoundingBox().first).length() / 5.0;

            m_viewer_initialised = true;
        }

        // Centre the rotation of the scence on the middle point between the source and the detector
        gVirtualXRay::VEC3 centre(g_xray_detector.getDetectorPosition() + g_xray_detector.getXraySourceCentre());
        centre /= 2.0;
        g_centre_sample_translation_matrix = gVirtualXRay::Matrix4x4< GLfloat >::buildTranslationMatrix(-centre);
        updateSampleTransformationMatrix();

        // Check if the source_detector vector is parallel to the eye_lookat vector
        gVirtualXRay::VEC3 source_detector(g_xray_detector.getXraySourceCentre() - g_xray_detector.getDetectorPosition());
        source_detector.normalise();
        gVirtualXRay::VEC3 eye_lookat(0, 0, 1);

        // Get the angle between the two vectors in degrees
        float angle = static_cast<float>(gVirtualXRay::radianToDegree(acos(source_detector.dotProduct(eye_lookat))));

        // If   -10 < angle < 10,
        if (abs(angle) < 10 || abs(M_PI - angle)  < 10)
        {
        	// then rotate the scene so that we see it from a viewing direction
        	// Perpendicular to the source_detector vector
            g_scene_rotation_matrix *= gVirtualXRay::Matrix4x4<GLfloat>::buildRotationMatrix(90 - angle, g_xray_detector.getUpVector());

            // Slightly alter the rotation matrix so that it's a bit nicer
            g_scene_rotation_matrix *= gVirtualXRay::Matrix4x4<GLfloat>::buildRotationMatrix(-45, g_xray_detector.getUpVector());
            //g_scene_rotation_matrix *= gVirtualXRay::Matrix4x4<GLfloat>::buildRotationMatrix(0, source_detector);
            //g_scene_rotation_matrix *= gVirtualXRay::Matrix4x4<GLfloat>::buildRotationMatrix(0, 0, 0, 1);
        }

        g_initial_far = g_camera_position[2] * 100;

        gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
                                         g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
                                            g_camera_up[0], g_camera_up[1], g_camera_up[2]);
    }
}


#ifdef HAS_GLFW
SimpleGVXR::Window* SimpleGVXR::Window::getWindow(GLFWwindow* apGLFWwindow)
{
	for (auto& ite: m_p_window_set)
	{
		if (ite->m_context.getWindowHandle() == apGLFWwindow)
			return ite;
	}

	return nullptr;
}
#endif


//------------------------------------------------------------
void SimpleGVXR::Window::displayScene(bool aSceneRotationFlag)
//------------------------------------------------------------
{}


//---------------------------------------
void SimpleGVXR::Window::takeScreenshot()
//---------------------------------------
{}


//-----------------------------------------------------
gVirtualXRay::Context& SimpleGVXR::Window::getContext()
//-----------------------------------------------------
{
    return m_context;
}


//------------------------------------------
void SimpleGVXR::Window::registerCallbacks()
//------------------------------------------
{
#ifdef HAS_GLFW
	if (m_context.getWindowHandle() && !m_callback_registered)
	{
		m_callback_registered = true;

        glfwSetKeyCallback(m_context.getWindowHandle(), keyCallback);
        glfwSetMouseButtonCallback(m_context.getWindowHandle(), mouseButtonCallback);
        glfwSetCursorPosCallback(m_context.getWindowHandle(), cursorPosCallback);
        glfwSetScrollCallback(m_context.getWindowHandle(), scrollCallback);
        glfwSetWindowCloseCallback(m_context.getWindowHandle(), windowCloseCallback);
	}
#endif
}


//---------------------------------------------------------
void SimpleGVXR::Window::createWindow(bool aVisibilityFlag)
//---------------------------------------------------------
{}


//-----------------------------------
void SimpleGVXR::Window::initShader()
//-----------------------------------
{}


//------------------------------------------------------
void SimpleGVXR::Window::render(bool aSceneRotationFlag)
//------------------------------------------------------
{}



#ifdef HAS_GLFW
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
        case GLFW_KEY_Q:
        case GLFW_KEY_ESCAPE:
            g_run_loop = false;
            break;

        case GLFW_KEY_B:
            g_display_beam = !g_display_beam;
            break;

        case GLFW_KEY_W:
            g_display_wireframe = !g_display_wireframe;
            break;

        case GLFW_KEY_N:
            g_xray_renderer.useNegativeFilteringFlag(
                !g_xray_renderer.getNegativeFilteringFlag());
            break;

        case GLFW_KEY_L:
            g_use_lighing = !g_use_lighing;
            break;

        //case GLFW_KEY_D:
        //    g_display_detector = !g_display_detector;
        //    break;

        case GLFW_KEY_V:
            g_display_normal_vectors = !g_display_normal_vectors;
            break;

        case GLFW_KEY_T:
            {
                SimpleGVXR::Window* p_window = SimpleGVXR::Window::getWindow(window);
                p_window->takeScreenshot();
            }
            break;

        case GLFW_KEY_S:
            g_S_key_pressed = true;
            break;

        case GLFW_KEY_D:
            g_D_key_pressed = true;
            break;

        case GLFW_KEY_1:
            g_1_key_pressed = true;
            break;

        case GLFW_KEY_2:
            g_2_key_pressed = true;
            break;

        case GLFW_KEY_3:
            g_3_key_pressed = true;
            break;

        case GLFW_KEY_4:
            g_4_key_pressed = true;
            break;

        case GLFW_KEY_LEFT:
            g_left_arrow_key_pressed = true;
            break;

        case GLFW_KEY_RIGHT:
            g_right_arrow_key_pressed = true;
            break;

        case GLFW_KEY_UP:
            g_up_arrow_key_pressed = true;
            break;

        case GLFW_KEY_DOWN:
            g_down_arrow_key_pressed = true;
            break;

        default:
            break;
        }
    }
    else if (action == GLFW_RELEASE)
    {
        switch (key)
        {
            case GLFW_KEY_S:
                g_S_key_pressed = false;
                break;

            case GLFW_KEY_D:
                g_D_key_pressed = false;
                break;

            case GLFW_KEY_1:
                g_1_key_pressed = false;
                break;

            case GLFW_KEY_2:
                g_2_key_pressed = false;
                break;

            case GLFW_KEY_3:
                g_3_key_pressed = false;
                break;

            case GLFW_KEY_4:
                g_4_key_pressed = false;
                break;

            case GLFW_KEY_LEFT:
                g_left_arrow_key_pressed = false;
                break;

            case GLFW_KEY_RIGHT:
                g_right_arrow_key_pressed = false;
                break;

            case GLFW_KEY_UP:
                g_up_arrow_key_pressed = false;
                break;

            case GLFW_KEY_DOWN:
                g_down_arrow_key_pressed = false;
                break;

            default:
                break;
        }
    }

    // Change the SOD
    if (g_S_key_pressed || g_2_key_pressed)
    {
        // Get the current SOD
        float sod = getSOD("mm");
        float step = 0.05F * sod;

        // Increase the SOD
        if (g_S_key_pressed && (g_up_arrow_key_pressed || g_right_arrow_key_pressed))
        {
            updateSOD(step);
        }
        // Decrease the SOD
        else if (g_S_key_pressed && (g_down_arrow_key_pressed || g_left_arrow_key_pressed))
        {
            updateSOD(-step);
        }
    }

    // Change the ODD
    if (g_D_key_pressed || g_1_key_pressed)
    {
        // Get the current ODD
        float odd = getODD("mm");
        float step = 0.05F * odd;

        // Increase the ODD
        if ((g_D_key_pressed && (g_up_arrow_key_pressed || g_left_arrow_key_pressed)) ||
            (g_1_key_pressed && g_left_arrow_key_pressed))
        {
            updateODD(step);
        }
        // Decrease the ODD
        else if ((g_D_key_pressed && (g_down_arrow_key_pressed || g_right_arrow_key_pressed)) ||
            (g_1_key_pressed && g_right_arrow_key_pressed))
        {
            updateODD(-step);
        }
    }

    // Move sample
    if (g_2_key_pressed)
    {
        // Get the current SOD
        float sod = getSOD("mm");
        float step = 0.05F * sod;
        float step_x = 0.0F;
        float step_z = 0.0F;

        if (g_up_arrow_key_pressed)
        {
            step_z = -step;
        }
        else if (g_down_arrow_key_pressed)
        {
            step_z = step;
        }

        if (g_left_arrow_key_pressed)
        {
            step_x = step;
        }
        else if (g_right_arrow_key_pressed)
        {
            step_x = -step;
        }

        updateSample(step_x, 0.0F, step_z);
    }

    // Move sample
    if (g_3_key_pressed)
    {
        // Get the current SOD
        float sod = getSOD("mm");
        float step = 0.01F * sod;
        float step_y = 0.0F;

        if (g_up_arrow_key_pressed)
        {
            step_y = step;
        }
        else if (g_down_arrow_key_pressed)
        {
            step_y = -step;
        }

        updateSample(0.0F, step_y, 0.0F);
    }

    // Rotate/tilt sample
    if (g_4_key_pressed)
    {
        // Get the current SOD
        float step = 5.0F;
        float angle = 0.0F;

        if (g_left_arrow_key_pressed)
        {
            angle = step;
        }
        else if (g_right_arrow_key_pressed)
        {
            angle = -step;
        }

        g_sample_rotation_matrix = gVirtualXRay::MATRIX4::buildRotationMatrix(angle, g_xray_detector.getUpVector()) *
            g_sample_rotation_matrix;
        updateSampleTransformationMatrix();
        internalComputeXRayImage(true);
    }
}


//--------------------------------------------
void mouseButtonCallback(GLFWwindow* apWindow, int aButton, int aButtonState,
    int aModifierKey)
//--------------------------------------------
{
    g_button = aButton;
    g_button_state = aButtonState;

    double xpos(0);
    double ypos(0);
    glfwGetCursorPos(apWindow, &xpos, &ypos);

    // Start panning the view
    if (g_button == GLFW_MOUSE_BUTTON_RIGHT && g_button_state == GLFW_PRESS)
    {
        g_last_x_position = static_cast<int>(xpos);
        g_last_y_position = static_cast<int>(ypos);

        g_current_x_position = static_cast<int>(xpos);
        g_current_y_position = static_cast<int>(ypos);
    }
    // Not start panning the view, maybe use the arc ball
    else
    {
        // Use the arc ball
        if (g_button_state == GLFW_PRESS && !g_S_key_pressed && !g_D_key_pressed)
        {
            startArcBallRotation(xpos, ypos);
        }
        // Stop using the arc ball
        else
        {
            stopArcBallRotation();
        }
    }
}


//--------------------------------------------------------------
void cursorPosCallback(GLFWwindow* apWindow, double x, double y)
//--------------------------------------------------------------
{
    int viewport_width  = 0;
    int viewport_height = 0;

    glfwGetWindowSize(apWindow, &viewport_width, &viewport_height);

    // Compute the rotation
    cursorPositionCallback(x, y, viewport_width, viewport_height);
}

//-----------------------------------------------------------------------
void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset)
//-----------------------------------------------------------------------
{
    if (!g_S_key_pressed && !g_D_key_pressed)
    {
        scrollCallback(xoffset, yoffset);
    }
    // Update the SOD
    else if (g_S_key_pressed)
    {
        // Get the current SOD
        float sod = getSOD("mm");
        auto step = static_cast<float>(0.05 * sod * yoffset);
        updateSOD(step);
    }
    // Update the ODD
    else if (g_D_key_pressed)
    {
        // Get the current ODD
        float odd = getODD("mm");
        auto step = static_cast<float>(0.05 * odd * yoffset);
        updateODD(step);
    }
}


//--------------------------------------------
void windowCloseCallback(GLFWwindow* apWindow)
//--------------------------------------------
{
    glfwSetWindowShouldClose(apWindow, GLFW_FALSE);

    SimpleGVXR::Window* p_window = SimpleGVXR::Window::getWindow(apWindow);
    p_window->hideWindow();

}
#endif


void startArcBallRotation(double x, double y) {
    g_last_x_position = static_cast<int>(x);
    g_last_y_position = static_cast<int>(y);

    g_current_x_position = static_cast<int>(x);
    g_current_y_position = static_cast<int>(y);

    g_use_arc_ball = true;
}

void stopArcBallRotation() {
    g_use_arc_ball = false;
}

bool usingArcBallRotation() {
    return (g_use_arc_ball);
}


//--------------------------------------------------------------------------------------
void cursorPositionCallback(double x, double y, int aViewportWidth, int aViewportHeight)
//--------------------------------------------------------------------------------------
{
    g_current_x_position = static_cast<int>(x);
    g_current_y_position = static_cast<int>(y);

    // Rotate the view
    if (!g_S_key_pressed && !g_D_key_pressed)
    {
        // Rotate the scene
        if (g_button == GLFW_MOUSE_BUTTON_LEFT)
            computeRotation(g_scene_rotation_matrix, aViewportWidth, aViewportHeight);
        // Pan the view
        else if (g_button == GLFW_MOUSE_BUTTON_RIGHT && g_button_state == GLFW_PRESS)
        {
            gVirtualXRay::VEC3 view_direction = g_camera_lookat - g_camera_position;
            view_direction.normalise();

            float near_height = 2.0F * tan(gVirtualXRay::degreeToRadian(g_initial_fovy) / 2.0F) * g_initial_near;
            auto view_ratio = static_cast<float>(aViewportWidth) / static_cast<float>(aViewportHeight);
            float near_width = near_height * view_ratio;

            auto offset_x = static_cast<float>(g_current_x_position - g_last_x_position) * 0.05F * near_width;
            auto offset_y = static_cast<float>(g_current_y_position - g_last_y_position) * 0.05F * near_height;

            g_camera_position[0] -= offset_x;
            g_camera_position[1] += offset_y;

            g_camera_lookat[0] -= offset_x;
            g_camera_lookat[1] += offset_y;

            gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
                                                    g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
                                                    g_camera_up[0], g_camera_up[1], g_camera_up[2]);
        }
        // Rotate the sample
        else if (g_button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
            computeRotation(g_sample_rotation_matrix, aViewportWidth, aViewportHeight);
            updateSampleTransformationMatrix();
            internalComputeXRayImage(true);
        }
    }
    // Update the SOD or ODD
    else
    {
        if (g_last_x_position != 0 || g_last_y_position != 0 || g_current_x_position != 0 || g_current_y_position != 0)
        {
            auto offset_x = static_cast<float>(g_current_x_position - g_last_x_position);
            auto offset_y = static_cast<float>(g_current_y_position - g_last_y_position);
            float offset = 0.0F;

            if (abs(offset_x) > abs(offset_y))
                offset = offset_x;
            else
                offset = offset_y;

            if (g_S_key_pressed)
            {
                // Get the current SOD
                float sod = getSOD("mm");
                float step = 0.005F * sod * offset;
                updateSOD(step);
            }
            else if (g_D_key_pressed)
            {
                // Get the current ODD
                float odd = getODD("mm");
                float step = 0.005F * odd * offset;
                updateODD(step);
            }
        }
    }
    g_last_x_position = g_current_x_position;
    g_last_y_position = g_current_y_position;
}


//-------------------------------------------------
void scrollCallback(double xoffset, double yoffset)
//-------------------------------------------------
{
    // Scrolling along the Y-axis
    if (fabs(yoffset) > EPSILON)
    {
        g_use_arc_ball = false;
        g_camera_position[2] += static_cast<float>(yoffset * 0.05 * g_camera_position[2]);

        gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
                                         g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
                                            g_camera_up[0], g_camera_up[1], g_camera_up[2]);
    }
}

//----------------------------------------------------------
void computeRotation(gVirtualXRay::MATRIX4& aRotationMatrix,
                     int aViewportWidth,
                     int aViewportHeight)
//----------------------------------------------------------
{
    if (g_use_arc_ball)
    {
        if (g_current_x_position != g_last_x_position ||
            g_current_y_position != g_last_y_position)
        {
            gVirtualXRay::Vec3<GLfloat> va(
                getArcballVector(g_last_x_position,
                                 g_last_y_position,
                                 aViewportWidth,
                                 aViewportHeight));

            gVirtualXRay::Vec3<GLfloat> vb(
                getArcballVector(g_current_x_position,
                                 g_current_y_position,
                                 aViewportWidth,
                                 aViewportHeight));

            float angle = ROTATION_SPEED *
                radian2degree(static_cast<float>(acos(std::min(1.0, va.dotProduct(vb)))));

            gVirtualXRay::Vec3 < GLfloat > axis_in_camera_coord(va ^ vb);
            //axis_in_camera_coord.normalize();

            gVirtualXRay::Matrix4x4<GLfloat> camera2object(
                aRotationMatrix.getInverse());

            gVirtualXRay::Vec3<GLfloat> axis_in_object_coord =
            camera2object * axis_in_camera_coord;
            //axis_in_object_coord.normalize();

            gVirtualXRay::Matrix4x4 < GLfloat > rotation_matrix;
            rotation_matrix.rotate(angle, axis_in_object_coord);
            aRotationMatrix = aRotationMatrix * rotation_matrix;

            g_last_x_position = g_current_x_position;
            g_last_y_position = g_current_y_position;
        }
    }
}



//---------------------------------------------------------------
gVirtualXRay::Vec3<GLfloat> getArcballVector(int x,
                                             int y,
                                             int aViewportWidth,
                                             int aViewportHeight)
//---------------------------------------------------------------
{
    gVirtualXRay::Vec3 < GLfloat
    > P(2.0F * float(x) / float(aViewportWidth) - 1.0F,
    2.0F * float(y) / float(aViewportHeight) - 1.0F, 0.0F);

    P.setY(-P.getY());

    float OP_squared = P.getX() * P.getX() + P.getY() * P.getY();
    if (OP_squared <= 1.0) {
        P.setZ(sqrt(1.0F - OP_squared));  // Pythagore
    } else {
        P.normalise();  // nearest point
    }

    return (P);
}


//-------------------------
void updateSOD(float aStep)
//-------------------------
{
    double unit_of_length = getUnitOfLength("mm");
    gVirtualXRay::VEC3 source_position = g_xray_detector.getXraySourceCentre() / unit_of_length;
    gVirtualXRay::VEC3 detector_position = g_xray_detector.getDetectorPosition() / unit_of_length;
    gVirtualXRay::VEC3 translation_vector = detector_position - source_position;
    translation_vector.normalise();
    translation_vector *= aStep;

    source_position -= translation_vector;

    // The source will not be in the sample
    if (!isPointInNodeAndChildrenBoundingBox(source_position[0], source_position[1], source_position[2], "mm", "root"))
    {
        setSourcePosition(source_position[0], source_position[1], source_position[2], "mm");
        internalComputeXRayImage(true);
    }
}


//-------------------------
void updateODD(float aStep)
//-------------------------
{
    double unit_of_length = getUnitOfLength("mm");
    gVirtualXRay::VEC3 source_position = g_xray_detector.getXraySourceCentre() / unit_of_length;
    gVirtualXRay::VEC3 detector_position = g_xray_detector.getDetectorPosition() / unit_of_length;
    gVirtualXRay::VEC3 translation_vector = detector_position - source_position;
    translation_vector.normalise();
    translation_vector *= aStep;

    detector_position += translation_vector;

    // The detector will not be in the sample
    if (!isPointInNodeAndChildrenBoundingBox(detector_position[0], detector_position[1], detector_position[2], "mm", "root"))
    {
        setDetectorPosition(detector_position[0], detector_position[1], detector_position[2], "mm");
        internalComputeXRayImage(true);
    }
}


void updateSample(float aStepX, float aStepY, float aStepZ)
{
    double unit_of_length = getUnitOfLength("mm");
    gVirtualXRay::VEC3 source_position = g_xray_detector.getXraySourceCentre() / unit_of_length;
    gVirtualXRay::VEC3 detector_position = g_xray_detector.getDetectorPosition() / unit_of_length;


    gVirtualXRay::VEC3 translation_x_vector = detector_position - source_position;
    gVirtualXRay::VEC3 translation_y_vector = g_xray_detector.getRightVector();
    gVirtualXRay::VEC3 translation_z_vector = g_xray_detector.getUpVector();

    translation_x_vector.normalise();
    translation_z_vector.normalise();
    translation_z_vector.normalise();

    translation_x_vector *= aStepX;
    translation_y_vector *= aStepY;
    translation_z_vector *= aStepZ;

    gVirtualXRay::VEC3 translation_vector = translation_x_vector + translation_y_vector + translation_z_vector;

    detector_position -= translation_vector;
    source_position -= translation_vector;

    // The detector and source will not be in the sample
    if (!isPointInNodeAndChildrenBoundingBox(detector_position[0], detector_position[1], detector_position[2], "mm",
        "root") &&
        !isPointInNodeAndChildrenBoundingBox(source_position[0], source_position[1], source_position[2], "mm", "root"))
    {
        g_sample_translation_matrix *= gVirtualXRay::MATRIX4::buildTranslationMatrix(translation_vector);
        updateSampleTransformationMatrix();
        internalComputeXRayImage(true);
    }
}


//-------------------------------------
void updateSampleTransformationMatrix()
//-------------------------------------
{
    g_sample_transformation_matrix =
        g_sample_translation_matrix * g_sample_rotation_matrix * g_centre_sample_translation_matrix;

}