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
*   @file       Context.cxx
*
*   @brief      Create an OpenGL context using EGL or GLFW.
*
*   @version    1.0
*
*   @date       10/09/2020
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
*               http://www.fpvidal.net/, Sept 2020, 2020, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <iostream>

#include <algorithm> // For capitalising a string

#ifndef __OpenGLUtilities_h
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

// #define GL_GLEXT_PROTOTYPES 1
// #include <GLFW/glfw3.h>
//
// #ifdef __APPLE__
// #include <OpenGL/gl.h>
// #else
// #include <GL/gl.h>
// #include <GL/glext.h>
// #endif
//
#ifdef HAS_EGL
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

#define GLFW_INCLUDE_NONE 1
#include <GLFW/glfw3.h>

// #ifdef HAS_EGL
// #define GLFW_EXPOSE_NATIVE_EGL 1
// #include <GLFW/glfw3native.h>
// #endif

#ifndef __Context_h
#include "gVirtualXRay/Context.h"
#endif

#ifndef __Logger_h
#include <gVirtualXRay/Logger.h>
#endif

#ifndef __ResourceManager_h
#include "gVirtualXRay/ResourceManager.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
using namespace gVirtualXRay;


int g_number_of_EGL_devices = 0;

#ifdef HAS_EGL
PFNEGLQUERYDEVICESEXTPROC eglQueryDevicesEXT = nullptr;
PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT = nullptr;
PFNEGLQUERYDEVICESTRINGEXTPROC eglQueryDeviceStringEXT = nullptr;
PFNEGLQUERYDEVICEATTRIBEXTPROC eglQueryDeviceAttribEXT = nullptr;

//typedef const char* eglGetDisplayDriverName_t(EGLDisplay disp);
//typedef char* eglGetDisplayDriverconfig_t(EGLDisplay disp);

//eglGetDisplayDriverName_t eglGetDisplayDriverName = nullptr;
//eglGetDisplayDriverconfig_t eglGetDisplayDriverconfig = nullptr;

void initEGLFunctions()
{
    // eglQueryDevicesEXT
    if (eglQueryDevicesEXT == nullptr)
    {
        eglQueryDevicesEXT = (PFNEGLQUERYDEVICESEXTPROC)
            eglGetProcAddress("eglQueryDevicesEXT");
    }

    if (eglQueryDevicesEXT == nullptr)
    {
        throw gVirtualXRay::Exception(
            __FILE__,
            __FUNCTION__,
            __LINE__,
            "eglQueryDevicesEXT() could not be loaded.");
    }

    // eglGetPlatformDisplayEXT
    if (eglGetPlatformDisplayEXT == nullptr)
    {
        eglGetPlatformDisplayEXT = (PFNEGLGETPLATFORMDISPLAYEXTPROC)
            eglGetProcAddress("eglGetPlatformDisplayEXT");
    }

    if (eglQueryDevicesEXT == nullptr)
    {
        throw gVirtualXRay::Exception(
            __FILE__,
            __FUNCTION__,
            __LINE__,
            "eglGetPlatformDisplayEXT() could not be loaded.");
    }

    // eglQueryDeviceStringEXT
    if (eglQueryDeviceStringEXT == nullptr)
    {
        eglQueryDeviceStringEXT = (PFNEGLQUERYDEVICESTRINGEXTPROC)
            eglGetProcAddress("eglQueryDeviceStringEXT");
    }

    if (eglQueryDevicesEXT == nullptr)
    {
        throw gVirtualXRay::Exception(
            __FILE__,
            __FUNCTION__,
            __LINE__,
            "eglQueryDeviceStringEXT() could not be loaded.");
    }

    // eglQueryDeviceAttribEXT
    if (eglQueryDeviceAttribEXT == nullptr)
    {
        eglQueryDeviceAttribEXT = (PFNEGLQUERYDEVICEATTRIBEXTPROC)
            eglGetProcAddress("eglQueryDeviceAttribEXT");
    }

    if (eglQueryDeviceAttribEXT == nullptr)
    {
        throw gVirtualXRay::Exception(
            __FILE__,
            __FUNCTION__,
            __LINE__,
            "eglQueryDeviceAttribEXT() could not be loaded.");
    }
}

struct device_property_t {
    EGLint name;
    const char* displayName;
    const char* extension;
    enum Type {
        String,
        Attribute
    } type;
};


static const device_property_t deviceProperties[] {
#ifdef EGL_DRM_DEVICE_FILE_EXT
    { EGL_DRM_DEVICE_FILE_EXT, "DRM device file", "EGL_EXT_device_drm", device_property_t::String },
#endif

#ifdef EGL_CUDA_DEVICE_NV
    { EGL_CUDA_DEVICE_NV, "CUDA device", "EGL_NV_device_cuda", device_property_t::Attribute }
#endif
};

static const int devicePropertiesSize = sizeof(deviceProperties) / sizeof(device_property_t);
#endif


//-----------------
Context::Context():
//-----------------
		m_renderer(""),
		m_major_version(0),
		m_minor_version(0),

		m_p_window(0),
        m_software_rendering(false),

#ifdef HAS_EGL
		m_display(EGL_NO_DISPLAY),
		m_config(0),
		m_context(0),
		m_surface(0),
        m_device_id(-1),
#endif

		m_ready(false)
//--------------------------------------------
{
	ResourceManager::getInstance().setGraphicsContext(this);
}


//--------------------------------------------
Context::Context(const std::string& aRenderer,
				 int aRendererMajorVersion,
				 int aRendererMinorVersion):
//--------------------------------------------
		m_renderer(""),
		m_major_version(0),
		m_minor_version(0),

		m_p_window(0),
        m_software_rendering(false),

#ifdef HAS_EGL
		m_display(EGL_NO_DISPLAY),
		m_config(0),
		m_context(0),
		m_surface(0),
        m_device_id(-1),
#endif

		m_ready(false)
//--------------------------------------------
{
	ResourceManager::getInstance().setGraphicsContext(this);
	create(aRenderer, aRendererMajorVersion, aRendererMinorVersion);
}


//-----------------
Context::~Context()
//-----------------
{
	ResourceManager::getInstance().destroyAndRemoveContext();
	release();
}


//------------------------------------------------
void Context::create(const std::string& aRenderer,
					 int aRendererMajorVersion,
					 int aRendererMinorVersion)
//------------------------------------------------
{
	// The renderer has changed
	if (m_renderer != aRenderer ||
			m_major_version != aRendererMajorVersion ||
			m_minor_version != aRendererMinorVersion)
	{
		// Release the previous context if applicable
		release();

		// Store the context details
		m_renderer = aRenderer;
		m_major_version = aRendererMajorVersion;
		m_minor_version = aRendererMinorVersion;

		// Create the context if possible
		std::string renderer = m_renderer;
		std::transform(renderer.begin(), renderer.end(), renderer.begin(), ::toupper);

		if (renderer == "OPENGL") createGLWithGLFW();
		else if (renderer == "VULKAN") createVulkanWithGLFW();
		else if (renderer == "EGL") createGLWithEGL();
		else LOGGER.logWarning("Invalid context: ") << aRenderer <<
				" (" << aRendererMajorVersion << "." << aRendererMinorVersion << ")" << std::endl;
	}
}


//---------------------
void Context::release()
//---------------------
{
	ResourceManager::getInstance().releaseResources();

#ifdef HAS_EGL
	if (m_display != EGL_NO_DISPLAY && m_context)
		{
		eglDestroyContext(m_display, m_context);
		m_context = 0;
		}

	if (m_display != EGL_NO_DISPLAY)
		{
		eglTerminate(m_display);
		m_display = EGL_NO_DISPLAY;
		}

        m_device_id = -1;

#endif // HAS_EGL

	if (m_p_window)
		{
		glfwDestroyWindow(m_p_window);
		m_p_window = 0;
		}

	m_ready = false;
    m_software_rendering = false;
}


//---------------------
void Context::destroy()
//---------------------
{
	release();
}


//---------------------------
bool Context::isReady() const
//---------------------------
{
	return m_ready;
}


//------------------------------------
GLFWwindow* Context::getWindowHandle()
//------------------------------------
{
	return m_p_window;
}


//------------------------------------------------
const GLFWwindow* Context::getWindowHandle() const
//------------------------------------------------
{
	return m_p_window;
}


unsigned int Context::getNumberOfEGLDevices()
{
#ifdef HAS_EGL

#ifdef HAS_GLAD
    gVirtualXRay::initialiseGLAD("EGL");
#endif

    if (!g_number_of_EGL_devices)
        {
        initEGLFunctions();



        LOGGER.logNow("Query the number of EGL devices") << std::endl;

        EGLBoolean result = eglQueryDevicesEXT(0, nullptr, &g_number_of_EGL_devices);

        if (result == EGL_TRUE)
        {
            LOGGER.logNow("Success") << std::endl;
        }
        else
        {
            LOGGER.logWarning("Failed to query EGL devices") << std::endl;
            return 0;
        }

        if (g_number_of_EGL_devices > 0)
        {
            LOGGER.logNow("Detected ") << g_number_of_EGL_devices << " EGL devices." << std::endl;
        }
        else
        {
            LOGGER.logWarning("Failed to find an EGL device") << std::endl;
            return 0;
        }

        // Device extensions
        LOGGER.logNow("Print the details here of every EGL device.") << std::endl;
        std::vector<EGLDeviceEXT> egl_devices_set(g_number_of_EGL_devices);
        result = eglQueryDevicesEXT(g_number_of_EGL_devices, &egl_devices_set[0], &g_number_of_EGL_devices);

        if (result == EGL_TRUE)
        {
            LOGGER.logNow("Success") << std::endl;
        }
        else
        {
            LOGGER.logWarning("Failed to query EGL devices") << std::endl;
            return g_number_of_EGL_devices;
        }

        bool software_rendering = false;
        for (int i = 0; i < g_number_of_EGL_devices; ++i)
        {
            LOGGER.logNow("Device ") << i + 1 << "/" << g_number_of_EGL_devices << ":" << std::endl;

            EGLDeviceEXT device = egl_devices_set[i];

            const char* extensions = eglQueryDeviceStringEXT(device, EGL_EXTENSIONS);
            if (extensions)
            {
                if (strlen(extensions))
                {
                    LOGGER.logNow("\tDevice Extensions: ") << extensions << std::endl;

                    if (strcmp(extensions, "EGL_MESA_device_software") == 0)
                    {
                        LOGGER.logWarning("\tSoftware rendering with Mesa will be used with this device") << std::endl;
                        software_rendering = true;
                    }
                }
                else
                    LOGGER.logNow("\tDevice Extensions: NONE") << std::endl;
            }
            else
            {
                LOGGER.logNow("\tFailed to retrieve device extensions.") << std::endl;
            }

            // const char* renderer = eglQueryDeviceStringEXT(egl_devices_set[i], EGL_RENDERER_EXT);
            // if (renderer)
            // {
            //     if (strlen(renderer))
            //         LOGGER.logNow("\tDevice renderer: ") << renderer << std::endl;
            //     else
            //         LOGGER.logNow("\tDevice renderer: NONE") << std::endl;
            // }
            // else
            // {
            //     LOGGER.logNow("\tFailed to retrieve device renderer.") << std::endl;
            // }

            if (!software_rendering && g_number_of_EGL_devices == 1)
            {
                const char* vendor = eglQueryDeviceStringEXT(egl_devices_set[i], EGL_VENDOR);
                if (vendor)
                {
                    if (strlen(vendor))
                        LOGGER.logNow("\tDevice vendor: ") << vendor << std::endl;
                    else
                        LOGGER.logNow("\tDevice vendor: NONE") << std::endl;
                }
                else
                {
                    LOGGER.logNow("\tFailed to retrieve device vendor.") << std::endl;
                }

                const char* device_file = eglQueryDeviceStringEXT(egl_devices_set[i], EGL_DRM_DEVICE_FILE_EXT);
                if (device_file)
                {
                    if (strlen(device_file))
                        LOGGER.logNow("\tEGL DRM device file: ") << device_file << std::endl;
                    else
                        LOGGER.logNow("\tEGL DRM device file: NONE") << std::endl;
                }
                else
                {
                    LOGGER.logNow("\tFailed to retrieve EGL DRM device file.") << std::endl;
                }
            }
        }
    }
#endif

    return g_number_of_EGL_devices;
}


#ifdef HAS_EGL
//---------------------------------------------------------------
bool Context::initialiseDisplay(bool aForceSoftwareRenderingFlag)
//---------------------------------------------------------------
{
    // Get the number of EGL devices and check if software rendering is used
    EGLint number_of_egl_devices = getNumberOfEGLDevices();

    // Use software rendering?
    m_software_rendering = false;

    // LOGGER.logWarning("\nFAILURE.") << std::endl;
    LOGGER.logWarning("\nGo through all the EGL devices to find a suitable display.") << std::endl;

    std::vector<EGLDeviceEXT> egl_devices_set(number_of_egl_devices);
    eglQueryDevicesEXT(number_of_egl_devices, &egl_devices_set[0], &number_of_egl_devices);

    for (int i = 0; i < number_of_egl_devices; ++i)
    {
        LOGGER.logWarning("Check EGL device ") << i + 1 << "/" << number_of_egl_devices << std::endl;

        EGLDeviceEXT device = egl_devices_set[i];
        const char* extensions = eglQueryDeviceStringEXT(device, EGL_EXTENSIONS);
        EGLDisplay temp_display = EGL_NO_DISPLAY;
        bool temp_software_rendering = false;

        if (extensions)
        {
            if (strlen(extensions))
            {
                if (strcmp(extensions, "EGL_MESA_device_software") == 0)
                {
                    temp_display = eglGetPlatformDisplayEXT(EGL_MESA_platform_surfaceless, EGL_DEFAULT_DISPLAY, NULL);

                    if (temp_display != EGL_NO_DISPLAY)
                    {
                        temp_software_rendering = true;
                        m_device_id = i;
                    }
                    else
                    {
                        LOGGER.logWarning("Failed to create an EGL display for Device ID ") << i << std::endl;
                    }
                }
                else
                {
                    temp_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, egl_devices_set[i], 0);
                }
            }
            else
            {
                temp_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, egl_devices_set[i], 0);
            }
        }
        else
        {
            temp_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, egl_devices_set[i], 0);
        }

        if (temp_display != EGL_NO_DISPLAY)
        {
            LOGGER.logNow("EGL display created for Device ID ") << i << std::endl;
            LOGGER.logNow("\tIs software rendering used? ") << (temp_software_rendering?"YES":"NO") << std::endl;

            if (m_display == EGL_NO_DISPLAY)
            {
                m_software_rendering = temp_software_rendering;
                m_display = temp_display;
                m_device_id = i;
            }
            else
            {
                if (m_software_rendering && !temp_software_rendering || !m_software_rendering && aForceSoftwareRenderingFlag)
                {
                    LOGGER.logNow("Replace EGL display of Device ID ") << m_device_id << " with Device " << i << std::endl;

                    m_software_rendering = temp_software_rendering;
                    m_display = temp_display;
                    m_device_id = i;
                }
            }
        }
    }

    if (m_display == EGL_NO_DISPLAY)
    {
        LOGGER.logWarning("Failed to find an EGL device that provides a display") << std::endl;
        m_ready = false;
        return false;
    }
    else
    {
        LOGGER.logNow("EGL device ID ") << m_device_id << " will be used." << std::endl;
    }

    return true;
}


//---------------------------
bool Context::initialiseEGL()
//---------------------------
{
    EGLint egl_major_ver = 0;
    EGLint egl_minor_ver = 0;

    // Initialise the display
    initialiseDisplay();

    // Initialise EGL
    LOGGER.logNow("Initialise EGL") << std::endl;

    if (eglInitialize(m_display, &egl_major_ver, &egl_minor_ver) == EGL_FALSE)
    {
        LOGGER.logWarning("Failed to eglInitialize") << std::endl;
        EGLint error =  eglGetError();
        switch(error)
        {
            case EGL_BAD_DISPLAY:
                LOGGER.logWarning("display is not an EGL display connection") << std::endl;
                break;
            case EGL_NOT_INITIALIZED:
                LOGGER.logWarning("display cannot be initialized") << std::endl;
                break;
            default:
                LOGGER.logWarning("unkonw EGL error") << std::endl;
                break;
        }

        LOGGER.logWarning("Force software rendering") << std::endl;
        initialiseDisplay(true);

        if (eglInitialize(m_display, &egl_major_ver, &egl_minor_ver) == EGL_FALSE)
        {
            LOGGER.logWarning("Failed to eglInitialize") << std::endl;
            EGLint error =  eglGetError();
            switch(error)
            {
                case EGL_BAD_DISPLAY:
                    LOGGER.logWarning("display is not an EGL display connection") << std::endl;
                    break;
                case EGL_NOT_INITIALIZED:
                    LOGGER.logWarning("display cannot be initialized") << std::endl;
                    break;
                default:
                    LOGGER.logWarning("unkonw EGL error") << std::endl;
                    break;
            }
        }

        m_ready = false;
        return false;
    }

    LOGGER.logNow("EGL version: ") << egl_major_ver << "." << egl_minor_ver << std::endl;

    return true;
}
#endif

//-----------------------------
void Context::createGLWithEGL()
//-----------------------------
{
	// See https://developer.nvidia.com/blog/egl-eye-opengl-visualization-without-x-server/
	// For an example

	// Release the previous context if applicable
	release();

#ifdef HAS_EGL
    initEGLFunctions();


    const char* client_extensions = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);
    if (client_extensions)
        LOGGER.logNow("EGL client extensions: ") << client_extensions << std::endl;
    else
        LOGGER.logNow("No EGL client extensions.") << std::endl;

    // Initialise GLAD
#ifdef HAS_GLAD
    gVirtualXRay::initialiseGLAD("EGL");
#endif

	// Initialise EGL
    if (!initialiseEGL())
    {
        m_ready = false;
		return;
    }

    // Context attributes
	constexpr EGLint attrib_list[] =
		{
		EGL_RED_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_DEPTH_SIZE, 8,
		EGL_LUMINANCE_SIZE, 0,
		EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
		EGL_NONE, EGL_NONE
		};


	// Choose the config supporting the attributes set above
	EGLint    num_config;
	if(eglChooseConfig(m_display, attrib_list, &m_config, 1, &num_config) == EGL_FALSE)
		{
		LOGGER.logWarning("Failed to eglChooseConfig") << std::endl;
		m_ready = false;
		return;
		}

	if(num_config < 1)
		{
		LOGGER.logWarning("No matching EGL frame buffer configuration") << std::endl;
		m_ready = false;
		return;
		}

	// Bind the OpenGL API to the context
    LOGGER.logNow("Bind the OpenGL API to EGL") << std::endl;
	if(eglBindAPI(EGL_OPENGL_API) == EGL_FALSE)
		{
		LOGGER.logWarning("Failed to eglBindAPI(EGL_OPENGL_API)") << std::endl;
		m_ready = false;
		return;
		}

	// OpenGL context
	const EGLint context_attrib[] =
		{
		EGL_CONTEXT_MAJOR_VERSION_KHR,  m_major_version,
		EGL_CONTEXT_MINOR_VERSION_KHR,  m_minor_version,
		EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR,    EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR,
		EGL_CONTEXT_FLAGS_KHR,          EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT_KHR
#ifndef NDEBUG
		| EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR
#endif
		,
		EGL_NONE
		};

	// Create the context
    LOGGER.logNow("Create the context") << std::endl;
	m_context = eglCreateContext(m_display, m_config, EGL_NO_CONTEXT, context_attrib);
	if(m_context == EGL_NO_CONTEXT)
		{
		m_context = eglCreateContext(m_display, m_config, EGL_NO_CONTEXT, nullptr);
		}

	// The context was not created
	if(m_context == EGL_NO_CONTEXT)
		{
		LOGGER.logWarning("Failed to eglCreateContext") << std::endl;
		EGLint error =  eglGetError();
		switch(error)
			{
			case EGL_BAD_CONFIG:
				LOGGER.logWarning("config is not an EGL frame buffer configuration, or does not support the current rendering API") << std::endl;
				break;
			case EGL_BAD_ATTRIBUTE:
				LOGGER.logWarning("attrib_list contains an invalid context attribute or if an attribute is not recognized or out of range") << std::endl;
				break;
			default:
				LOGGER.logWarning("Unknown error") << std::endl;
				break;
			}
		m_ready = false;
		return;
		}

	// Create the surface
    LOGGER.logNow("Create the surface") << std::endl;
	m_surface = eglCreatePbufferSurface(m_display,
			m_config,
			nullptr);

	// The surface was not created
	if (!m_surface)
		{
		LOGGER.logWarning("Error: eglCreatePbufferSurface failed Error") << std::endl;
		return;
		}

	// Make current
    LOGGER.logNow("Make the context current") << std::endl;
	if (eglMakeCurrent(m_display,
			m_surface,
			m_surface,
			m_context) == EGL_FALSE)
		{
		LOGGER.logWarning("Failed to eglMakeCurrent") << std::endl;
		m_ready = false;
		return;
		}

    // Using GLAD works when using a Python wheel created with manyliux2014
    // It does NOT when the wheel was repaired with auditwheel
    // GLEW must be  used!
    // gVirtualXRay::initialiseGLAD("OPENGL");

	// Initialise GLEW
    gVirtualXRay::initialiseGLEW();

	// Still no error?
	if(EGL_SUCCESS != eglGetError())
		{
		m_ready = false;
		return;
		}

	LOGGER.logNow("OpenGL version supported by this platform ") << glGetString(GL_VERSION) << std::endl;

	// Get rid of any OpenGL error at this stage
	glGetError();

	m_ready = true;
#endif // HAS_EGL
}


//-------------------------------------------------
void Context::createGLWithGLFW(int aVisibilityFlag)
//-------------------------------------------------
{
		// Release the previous context if applicable
		release();

		std::string window_title = "gVirtualXRay -- OpenGL Window (GLFW)";

        LOGGER.logNow("Initialise GLFW") << std::endl;
        // Initialize GLFW
        if (!glfwInit())
            {
            throw gVirtualXRay::Exception(
                __FILE__,
                __FUNCTION__,
                __LINE__,
                "ERROR: cannot initialise GLFW.");
            }

        // Set the version of OpenGL that is required
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_major_version);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_minor_version);
		glfwWindowHint(GLFW_DEPTH_BITS, 24);

		if (m_major_version == 3 && m_minor_version >= 2 || m_major_version >= 4)
		{
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		}

		// Enable anti-aliasing
		glfwWindowHint(GLFW_SAMPLES, 4);

        // Set the visibility
        if (aVisibilityFlag)
        {
            glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        }
        else
        {
            glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        }

		// Create a windowed mode window and its OpenGL context
        LOGGER.logNow("Create an OpenGL window with a ") << m_major_version << "." << m_minor_version << " context." << std::endl;
		m_p_window = glfwCreateWindow(1, 1, window_title.data(), 0, 0);

		// Window has be created
		if (m_p_window)
		{
			// Make the window's context current
            LOGGER.logNow("Make the window's context current") << std::endl;
			glfwMakeContextCurrent(m_p_window);
			glfwSetWindowUserPointer(m_p_window, this);

            // Initialise GLAD
			//gVirtualXRay::initialiseGLAD("OPENGL");

            // Initialise GLEW
			gVirtualXRay::initialiseGLEW();

			m_ready = true;
		}
}


//----------------------------------
void Context::createVulkanWithGLFW()
//----------------------------------
{
	// Release the previous context if applicable
	release();

// Compiled with Vulkan
#ifdef HAS_VULKAN
    // Vulkan is supported by the system
    if (glfwVulkanSupported())
    {
        // Vulkan is available, at least for compute
        LOGGER.logNow("Vulkan is available, at least for compute") << std::endl;
    }
    // Vulkan is not supported by the system
    else
    {
        throw gVirtualXRay::Exception(
            __FILE__,
            __FUNCTION__,
            __LINE__,
            "ERROR: gVXR was built with Vulkan support but your system does not support it.");
    }
// Compiled without Vulkan
#else
    throw gVirtualXRay::Exception(
        __FILE__,
        __FUNCTION__,
        __LINE__,
        "ERROR: gVXR was built without Vulkan support.");
#endif

    // Still not finished
    throw gVirtualXRay::Exception(
        __FILE__,
        __FUNCTION__,
        __LINE__,
        "ERROR: not implemented, contact Franck if you need this functionality.");
}
