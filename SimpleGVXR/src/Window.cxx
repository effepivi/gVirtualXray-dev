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

#ifdef HAS_CLIPBOARD
#include "clip.h"
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

#ifndef GVXR_TEXT_RENDERER_H
#include "gVirtualXRay/TextRenderer.h"
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

#ifndef SGVXR_SimpleGVXR_h
#include "SimpleGVXR.h"
#endif


//******************************************************************************
//  Namespaces
//******************************************************************************


//******************************************************************************
//  Function declarations
//******************************************************************************
#ifdef HAS_GLFW
void windowCloseCallback(GLFWwindow* apWindow);
#endif



// bool usingArcBallRotation();



gVirtualXRay::Vec3<GLfloat> getArcballVector(int x,
                                             int y,
                                             float aCentreX,
                                             float aCentreY,
                                             float aRadius);


void updateSOD(float aStep);

void updateODD(float aStep);

void updateSampleTranslation(float aStepX, float aStepY, float aStepZ);
bool updateRotation(const gVirtualXRay::MATRIX4& aRotationMatrix);


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

// extern gVirtualXRay::Matrix4x4<GLfloat> g_sample_rotation_matrix;
// extern gVirtualXRay::Matrix4x4<GLfloat> g_sample_translation_matrix;
// extern gVirtualXRay::Matrix4x4<GLfloat> g_sample_transformation_matrix;

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

extern std::map<int, std::unique_ptr<SimpleGVXR::Window> > g_p_window_set;      // The set of windows

int g_button(-1);
int g_button_state(-1);
bool g_use_arc_ball(false);
GLint g_last_x_position(0);
GLint g_last_y_position(0);
GLint g_current_x_position(0);
GLint g_current_y_position(0);

// Translate the whole 3-D view
gVirtualXRay::Matrix4x4 < GLfloat > g_scene_rotation_offset_matrix;

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


bool g_do_not_call_framebufferSizeOpenGlCallback = false;


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
    m_latest_screenshot_width(0),
    m_latest_screenshot_height(0),
    m_first_pass(false),
    m_auto_unbind_context(false)
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
    const std::vector<std::string> shader_id_set = {
        "Uncleaned d_j",
        "Cleaned d_j",
        "sum_j d_j",
        "d_0 - sum_j d_j",
        "sum_j mu(E_i) d_j",
        "I",
        "Display X-ray"
    };

    for (const auto& ite: shader_id_set) {
        gVirtualXRay::LOGGER.logNow("Shader \"") << ite <<
            "\" total runtime (in sec): " << getShaderTimeInSeconds(ite) << std::endl;
        gVirtualXRay::LOGGER.logNow("Shader \"") << ite <<
            "\" number of calls (in sec): " << getShaderCounter(ite) << std::endl;
    }
    gVirtualXRay::LOGGER.logNow();

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
        if (m_context.getWindowHandle() && g_do_not_call_framebufferSizeOpenGlCallback == false)
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


//-----------------------------------------------------------------------
const std::vector<uint8_t>& SimpleGVXR::Window::getLastScreenshot() const
//-----------------------------------------------------------------------
{
    return m_p_latest_screenshot;
}


//-------------------------------------------------------
size_t SimpleGVXR::Window::getLastScreenshotWidth() const
//-------------------------------------------------------
{
    return m_latest_screenshot_width;
}


//--------------------------------------------------------
size_t SimpleGVXR::Window::getLastScreenshotHeight() const
//--------------------------------------------------------
{
    return m_latest_screenshot_height;
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

        // Centre the rotation of the scene on the middle point between the source and the detector
        const float distance = std::max(distance_detector_object,
                                  distance_source_object);

        const float half_fovy = (static_cast<float>(M_PI * g_initial_fovy) / 2.0f) / 180.0f;

        // Initialise the camera
        g_camera_lookat = (g_xray_detector.getDetectorPosition() + g_xray_detector.getXraySourceCentre()) / 2.0;
        g_camera_position = g_camera_lookat + g_xray_detector.getRightVector() * (atan(half_fovy) * distance * 4.0F);
        g_camera_up = -g_xray_detector.getUpVector();

        // Set the frustrum distances
        if (g_p_scene_graph)
        {
            g_initial_near = (g_p_scene_graph->getBoundingBox().second -
                g_p_scene_graph->getBoundingBox().first).length() / 5.0;

            g_initial_far = distance * 20.0;

            m_viewer_initialised = true;
        }

        gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
                                         g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
                                            g_camera_up[0], g_camera_up[1], g_camera_up[2]);

        m_initial_camera_position = g_camera_position;
        m_initial_camera_lookat = g_camera_lookat;
        m_initial_camera_up = g_camera_up;

        m_initial_source_position = g_xray_detector.getXraySourceCentre();
        m_initial_detector_position = g_xray_detector.getDetectorPosition();
        m_initial_up_vector = g_xray_detector.getUpVector();
        m_initial_right_vector = g_xray_detector.getRightVector();

        if (g_p_scene_graph) {
            gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode("root");
            if (p_node) m_root_initial_transformation_matrix = p_node->getLocalTransformationMatrix();
        }

        std::vector<double> detector_size = getDetectorSize("mm");
        auto max_val = *std::max_element(detector_size.begin(), detector_size.end());

        setMarkerLength(static_cast<float>(max_val * 0.015), "mm");

        g_text_renderer.initialise(16, 3.0);
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
void SimpleGVXR::Window::restoreInitialViewAndGeometry() const
//------------------------------------------------------------
{
    g_camera_position = m_initial_camera_position;
    g_camera_lookat = m_initial_camera_lookat;
    g_camera_up = m_initial_camera_up;

    // Update the view matrix using the new camera frame.
    gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
                                            g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
                                            g_camera_up[0], g_camera_up[1], g_camera_up[2]);


    double unit_of_length = getUnitOfLength("mm");

    setSourcePosition(m_initial_source_position[0] / unit_of_length,
        m_initial_source_position[1] / unit_of_length,
        m_initial_source_position[2] / unit_of_length,
        "mm");
    g_xray_detector.setDetectorPosition(m_initial_detector_position);
    g_xray_detector.setUpVector(m_initial_up_vector);
    g_xray_detector.setRightVector(m_initial_right_vector);

    if (g_p_scene_graph) {
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode("root");
        if (p_node) p_node->setLocalTransformation(m_root_initial_transformation_matrix);
    }

    internalComputeXRayImage(true);

    // g_sample_rotation_matrix.loadIdentity();
    // g_sample_translation_matrix.loadIden1tity();
    // g_sample_transformation_matrix.loadIdentity();
}


//--------------------------------------------------------
void SimpleGVXR::Window::useAutoUnbindContext(bool aState)
//--------------------------------------------------------
{
    m_auto_unbind_context = aState;
}


//---------------------------------------------------------
bool SimpleGVXR::Window::getAutoUnbindContextStatus() const
//---------------------------------------------------------
{
    return m_auto_unbind_context;
}


//------------------------------------------------------------
void SimpleGVXR::Window::displayScene(bool aSceneRotationFlag)
//------------------------------------------------------------
{}


//---------------------------------------
void SimpleGVXR::Window::takeScreenshot()
//---------------------------------------
{}


//---------------------------------------
void SimpleGVXR::Window::copyScreenshot()
//---------------------------------------
{
    takeScreenshot();

#ifdef HAS_CLIPBOARD
#ifdef CLIP_ENABLE_IMAGE
    // Copy to the clipboard

    // Retrieve the latest screenshot and format it in RGBA
    const size_t width = m_latest_screenshot_width;
    const size_t height = m_latest_screenshot_height;

    // Allocate the RGBA data buffer
    std::vector<uint8_t> p_dst(width * height * 4);

    for (size_t j = 0; j < height; ++j)
    {
        // glReadPixels returns the rows bottom-up, the clipboard expects them top-down
        const uint8_t* p_src = &m_p_latest_screenshot[(height - 1 - j) * width * 3];

        for (size_t i = 0; i < width; ++i)
        {
            p_dst[4 * j * width + 4 * i + 0] = p_src[3 * i + 0];
            p_dst[4 * j * width + 4 * i + 1] = p_src[3 * i + 1];
            p_dst[4 * j * width + 4 * i + 2] = p_src[3 * i + 2];
            p_dst[4 * j * width + 4 * i + 3] = 255;
        }
    }

    // Create the descriptor
    clip::image_spec spec;
    spec.width = width;
    spec.height = height;
    spec.bits_per_pixel = 32;
    spec.bytes_per_row = width * 4;

    spec.red_mask    = 0x000000ff;
    spec.green_mask  = 0x0000ff00;
    spec.blue_mask   = 0x00ff0000;
    spec.alpha_mask  = 0xff000000;
    spec.red_shift   = 0;
    spec.green_shift = 8;
    spec.blue_shift  = 16;
    spec.alpha_shift = 24;

    // Create the clip::image
    clip::image img(&p_dst[0], spec);

    // Copy the image to the clipboard
    if (!clip::set_image(img))
    {
        gVirtualXRay::LOGGER.logWarning("Error copying the image to clipboard") << std::endl;
    }

    if (!clip::has(clip::image_format()))
    {
        gVirtualXRay::LOGGER.logWarning("The image was not copied to clipboard") << std::endl;
    }

#else
    gVirtualXRay::LOGGER.logWarning("Copying the screenshot to the system clipboard is not support, "
                                    "image support was not enabled when compiling clip") << std::endl;
#endif
#else
    gVirtualXRay::LOGGER.logWarning("Copying the screenshot to the system clipboard is not support") << std::endl;
#endif
}


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

        glfwSetKeyCallback(m_context.getWindowHandle(), glfwKeyCallback);
        glfwSetMouseButtonCallback(m_context.getWindowHandle(), glfwMouseButtonCallback);
        glfwSetCursorPosCallback(m_context.getWindowHandle(), glfwCursorPositionCallback);
        glfwSetScrollCallback(m_context.getWindowHandle(), glfwScrollCallback);
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
//--------------------------------------------------
void SimpleGVXR::glfwKeyCallback(GLFWwindow* window,
                                 int key,
                                 int scancode,
                                 int action,
                                 int mods)
//--------------------------------------------------
{
    std::cout << "action: " << action << std::endl;
    std::cout << "action == GLFW_PRESS: " << (action == GLFW_PRESS) << std::endl;
    std::cout << "action == GLFW_RELEASE: " << (action == GLFW_RELEASE) << std::endl;
    std::cout << "key: " << static_cast<char>(key) << std::endl;
    std::cout << "mods CTRL: " << (mods & GLFW_MOD_CONTROL)  << std::endl;
    std::cout << "mods SHIFT: " << (mods & GLFW_MOD_SHIFT)  << std::endl;


    // Ctrl+Shift+C
    if (action == GLFW_PRESS && key == GLFW_KEY_C && (mods & GLFW_MOD_CONTROL) && (mods & GLFW_MOD_SHIFT))
    {
#ifdef HAS_CLIPBOARD
#ifdef CLIP_ENABLE_IMAGE
        std::cout << "(action == GLFW_PRESS && key == GLFW_KEY_C && (mods & GLFW_MOD_CONTROL) && (mods & "
                     "GLFW_MOD_SHIFT))" << std::endl;
        // Copy to the clipboard

        // Retrieve the latest screenshot and format it in RGBA
        auto xray_image = g_xray_renderer.getEnergyFluence();
        const size_t width = xray_image.getWidth();
        const size_t height = xray_image.getHeight();

        std::cout << "Copy to clipboard a " << width << "x" << height <<
            " radiograph" << std::endl;

        // Allocate the RGBA data buffer
        std::vector<unsigned char> p_dst(width * height * 4);
        const float* p_src = xray_image.getRawData();
        auto total_energy = static_cast<float>(getTotalEnergyWithDetectorResponse());

        for (size_t i = 0; i < width * height; ++i)
        {
            auto pixel_value_flt = 255.0F * static_cast<float>(p_src[i]) / total_energy;
            auto pixel_value_uint8 = static_cast<uint8_t>(round(pixel_value_flt));

            p_dst[4 * i + 0] = pixel_value_uint8;
            p_dst[4 * i + 1] = pixel_value_uint8;
            p_dst[4 * i + 2] = pixel_value_uint8;
            p_dst[4 * i + 3] = 255;
        }

        // Create the descriptor
        clip::image_spec spec;
        spec.width = width;
        spec.height = height;
        spec.bits_per_pixel = 8 * 4;
        spec.bytes_per_row = width * 4;

        spec.red_mask    = 0x000000ff;
        spec.green_mask  = 0x0000ff00;
        spec.blue_mask   = 0x00ff0000;
        spec.alpha_mask  = 0xff000000;
        spec.red_shift   = 0;
        spec.green_shift = 8;
        spec.blue_shift  = 16;
        spec.alpha_shift = 24;

        // Create the clip::image
        clip::image img(&p_dst[0], spec);

        // Copy the image to the clipboard
        if (!clip::set_image(img))
        {
            gVirtualXRay::LOGGER.logWarning("Error copying the X-ray image to clipboard") << std::endl;
        }

        if (!clip::has(clip::image_format()))
        {
            gVirtualXRay::LOGGER.logWarning("The X-ray image was not copied to clipboard") << std::endl;
        }
#else
        gVirtualXRay::LOGGER.logWarning("Copying the X-ray image to the system clipboard is not support, "
                                        "image support was not enabled when compiling clip") << std::endl;
#endif
#else
        gVirtualXRay::LOGGER.logWarning("Copying the X-ray image to the system clipboard is not support") << std::endl;
#endif
    }
    // Ctrl+C
    else if (action == GLFW_PRESS && key == GLFW_KEY_C && (mods & GLFW_MOD_CONTROL))
    {
        std::cout << "(action == GLFW_PRESS && key == GLFW_KEY_C && (mods & GLFW_MOD_CONTROL))" << std::endl;
        SimpleGVXR::Window* p_window = SimpleGVXR::Window::getWindow(window);
        p_window->copyScreenshot();
    }
    // Alt+F4
    else if (action == GLFW_PRESS && key == GLFW_KEY_F4 && (mods & GLFW_MOD_ALT))
    {
            g_run_loop = false;
    }
    else if (action == GLFW_PRESS)
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

        case GLFW_KEY_H:
            g_display_help = !g_display_help;
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

        case GLFW_KEY_R:
            {
                SimpleGVXR::Window* p_window = SimpleGVXR::Window::getWindow(window);
                p_window->restoreInitialViewAndGeometry();
            }
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
    if (g_S_key_pressed)
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
        float step_y = 0.0F;
        bool need_update = false;

        if (g_up_arrow_key_pressed)
        {
            step_y = -step;
            need_update = true;
        }
        else if (g_down_arrow_key_pressed)
        {
            step_y = step;
            need_update = true;
        }

        if (g_left_arrow_key_pressed)
        {
            step_x = step;
            need_update = true;
        }
        else if (g_right_arrow_key_pressed)
        {
            step_x = -step;
            need_update = true;
        }

        if (need_update)
        {
            updateSampleTranslation(step_x, step_y, 0.0F);
        }
    }

    // Move sample
    if (g_3_key_pressed)
    {
        // Get the current SOD
        float sod = getSOD("mm");
        float step = 0.01F * sod;
        float step_z = 0.0F;
        bool need_update = false;

        if (g_up_arrow_key_pressed)
        {
            step_z = -step;
            need_update = true;
        }
        else if (g_down_arrow_key_pressed)
        {
            step_z = step;
            need_update = true;
        }

        if (need_update)
        {
            updateSampleTranslation(0.0F, 0.0F, step_z);
        }
    }

    // Rotate/tilt sample
    if (g_4_key_pressed)
    {
        // Get the current SOD
        float step = 5.0F;
        float angle = 0.0F;
        bool need_update = false;
        gVirtualXRay::VEC3 rotation_axis(1.0, 0.0, 0.0);

        if (g_left_arrow_key_pressed)
        {
            angle = step;
            rotation_axis = g_xray_detector.getUpVector();
            need_update = true;
        }
        else if (g_right_arrow_key_pressed)
        {
            angle = -step;
            rotation_axis = g_xray_detector.getUpVector();
            need_update = true;
        }
        else if (g_up_arrow_key_pressed)
        {
            angle = step;
            rotation_axis = (g_camera_lookat - g_camera_position).normal();
            need_update = true;
        }
        else if (g_down_arrow_key_pressed)
        {
            angle = -step;
            rotation_axis = (g_camera_lookat - g_camera_position).normal();
            need_update = true;
        }

        if (need_update)
        {
            gVirtualXRay::MATRIX4 rotation_matrix = gVirtualXRay::MATRIX4::buildRotationMatrix(angle,
                rotation_axis);

            if (updateRotation(rotation_matrix))
            {
                internalComputeXRayImage(true);
            }
        }
    }
}


//------------------------------------------------------------
void SimpleGVXR::glfwMouseButtonCallback(GLFWwindow* apWindow,
                                         int aButton,
                                         int aButtonState,
                                         int aModifierKey)
//------------------------------------------------------------
{
#ifdef HAS_GLFW
    for (const auto& [key, window] : g_p_window_set)
    {
        if (window->getContext().getWindowHandle() == apWindow) {
            double xpos, ypos;
            glfwGetCursorPos(apWindow, &xpos, &ypos);
            mouseButtonCallback(aButton, aButtonState, aButtonState, xpos, ypos, key);
            return;
        }
    }

#endif
}


//-----------------------------------------------------------------------------------
void SimpleGVXR::glfwCursorPositionCallback(GLFWwindow* apWindow, double x, double y)
//-----------------------------------------------------------------------------------
{
#ifdef HAS_GLFW
    for (const auto& [key, window] : g_p_window_set)
    {
        if (window->getContext().getWindowHandle() == apWindow) {
            cursorPositionCallback(x, y, key);
            return;
        }
    }

#endif
}

//---------------------------------------------------------------------------------------
void SimpleGVXR::glfwScrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset)
//---------------------------------------------------------------------------------------
{
#ifdef HAS_GLFW
    for (const auto& [key, window] : g_p_window_set)
    {
        if (window->getContext().getWindowHandle() == apWindow) {
            scrollCallback(xoffset, yoffset, key);
            return;
        }
    }

#endif
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




bool usingArcBallRotation() {
    return (g_use_arc_ball);
}





//---------------------------------------------------------------
gVirtualXRay::Vec3<GLfloat> getArcballVector(int x,
                                             int y,
                                             float aCentreX,
                                             float aCentreY,
                                             float aRadius)
//---------------------------------------------------------------
{
    // Position of the cursor with respect to the centre of the virtual
    // sphere, in units of its radius. The same radius is used along both
    // axes so that a horizontal and a vertical drag of the same number of
    // pixels give the same angle, whatever the aspect ratio of the viewport
    gVirtualXRay::Vec3 < GLfloat
    > P((static_cast<float>(x) - aCentreX) / aRadius,
    (static_cast<float>(y) - aCentreY) / aRadius, 0.0F);

    // The window coordinates increase downwards, the camera coordinates
    // increase upwards
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
    gVirtualXRay::VEC3 old_source_position = g_xray_detector.getXraySourceCentre() / unit_of_length;
    gVirtualXRay::VEC3 detector_position = g_xray_detector.getDetectorPosition() / unit_of_length;
    gVirtualXRay::VEC3 translation_vector = detector_position - old_source_position;
    translation_vector.normalise();
    translation_vector *= aStep;

    gVirtualXRay::VEC3 new_source_position = old_source_position - translation_vector;
    setSourcePosition(new_source_position[0], new_source_position[1], new_source_position[2], "mm");

    // The sample crashed into the detector
    if (sampleCrashedIntoDetector() || sampleCrashedIntoSource())
    {
        // Restore the transformation
        setSourcePosition(old_source_position[0], old_source_position[1], old_source_position[2], "mm");
    }
    // Update the X-ray image
    else
    {
        internalComputeXRayImage(true);
    }
}


//-------------------------
void updateODD(float aStep)
//-------------------------
{
    double unit_of_length = getUnitOfLength("mm");
    gVirtualXRay::VEC3 source_position = g_xray_detector.getXraySourceCentre() / unit_of_length;
    gVirtualXRay::VEC3 old_detector_position = g_xray_detector.getDetectorPosition() / unit_of_length;
    gVirtualXRay::VEC3 translation_vector = old_detector_position - source_position;
    translation_vector.normalise();
    translation_vector *= aStep;

    gVirtualXRay::VEC3 new_detector_position = old_detector_position + translation_vector;
    setDetectorPosition(new_detector_position[0], new_detector_position[1], new_detector_position[2], "mm");

    // The sample crashed into the detector
    if (sampleCrashedIntoDetector())
    {
        // Restore the transformation
        setDetectorPosition(old_detector_position[0], old_detector_position[1], old_detector_position[2], "mm");
    }
    // Update the X-ray image
    else
    {
        internalComputeXRayImage(true);
    }
}


void updateSampleTranslation(float aStepX, float aStepY, float aStepZ)
{
    double unit_of_length = getUnitOfLength("mm");
    gVirtualXRay::VEC3 source_position_in_mm = g_xray_detector.getXraySourceCentre() / unit_of_length;
    gVirtualXRay::VEC3 detector_position_in_mm = g_xray_detector.getDetectorPosition() / unit_of_length;

    gVirtualXRay::VEC3 translation_x_vector = detector_position_in_mm - source_position_in_mm;
    gVirtualXRay::VEC3 translation_y_vector = g_xray_detector.getRightVector();
    gVirtualXRay::VEC3 translation_z_vector = g_xray_detector.getUpVector();

    translation_x_vector.normalise();
    translation_y_vector.normalise();
    translation_z_vector.normalise();

    translation_x_vector *= aStepX;
    translation_y_vector *= aStepY;
    translation_z_vector *= aStepZ;

    gVirtualXRay::VEC3 translation_vector = translation_x_vector + translation_y_vector + translation_z_vector;
    gVirtualXRay::MATRIX4 translation_matrix = gVirtualXRay::MATRIX4::buildTranslationMatrix(translation_vector * unit_of_length);

    if (g_p_scene_graph) {
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode("root");

        const gVirtualXRay::MATRIX4 old_transformation_matrix = p_node->getLocalTransformationMatrix();

        p_node->setLocalTransformation(translation_matrix * old_transformation_matrix);

        if (!sampleCrashedIntoDetector() && !sampleCrashedIntoSource())
        {
            internalComputeXRayImage(true);
        }
        else
        {
            p_node->setLocalTransformation(old_transformation_matrix);
        }
    }
}


bool updateRotation(const gVirtualXRay::MATRIX4& aRotationMatrix)
{
    double unit_of_length = getUnitOfLength("mm");
    bool sample_crashed = false;

    if (g_p_scene_graph) {
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode("root");
        const gVirtualXRay::MATRIX4 old_transformation_matrix = p_node->getLocalTransformationMatrix();

        std::vector<double> bbox_centre_global = getNodeAndChildrenBoundingBoxCentre("root", "mm");

        gVirtualXRay::VEC3 bbox_centre = gVirtualXRay::VEC3(
            bbox_centre_global[0],
            bbox_centre_global[1],
            bbox_centre_global[2]) / unit_of_length;

        gVirtualXRay::MATRIX4 centre_translation_matrix = gVirtualXRay::MATRIX4::buildTranslationMatrix(
            -bbox_centre[0], -bbox_centre[1], -bbox_centre[2]);

        gVirtualXRay::MATRIX4 new_transformation_matrix = centre_translation_matrix.getInverse() *
            aRotationMatrix * centre_translation_matrix * old_transformation_matrix;

        p_node->setLocalTransformation(new_transformation_matrix);

        // Return true if the sample hasn't crashed into the detector and source
        // Return false if the sample crashed into the detector or source
        bool sample_crashed = sampleCrashedIntoDetector() || sampleCrashedIntoSource();

        if (sample_crashed)
        {
            p_node->setLocalTransformation(old_transformation_matrix);
        }
    }

    return !sample_crashed;
}
