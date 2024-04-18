/*

Copyright (c) 2013-2023, Dr Franck P. Vidal, Bangor University, All rights reserved.
Copyright (c) 2023-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
UK Research and Innovation, All rights reserved.


Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of Bangor University, UK Research and Innovation nor the 
names of its contributors may be used to endorse or promote products derived 
from this software without specific prior written permission.

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
*   @date       18/04/2024
*
*   @author     Prof Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal, Bangor University, Sept 2020-Sept 2023, version 1.1, BSD 3-Clause License
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Oct 2023-present, version 1.1, BSD 3-Clause License
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
#ifdef HAS_GBM
#include <fcntl.h>
#include <gbm.h>
#endif
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

#ifdef HAS_EGL
		m_display(EGL_NO_DISPLAY),
		m_config(0),
		m_context(0),
		m_surface(0),
        m_device_id(-1),
#endif

#ifdef HAS_GLX
        m_glx_display(0),
        m_glx_cmap(0),
        m_glx_context(0),
        m_glx_window(0),
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

#ifdef HAS_EGL
		m_display(EGL_NO_DISPLAY),
		m_config(0),
		m_context(0),
		m_surface(0),
        m_device_id(-1),
#endif

#ifdef HAS_GLX
        m_glx_display(0),
        m_glx_cmap(0),
        m_glx_context(0),
        m_glx_window(0),
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
	destroy();
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
		else if (renderer == "GLX") createGLWithGLX();
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

#ifdef HAS_GLX
    if (m_glx_display)
    {
        glXMakeCurrent( m_glx_display, 0, 0 );

        if (m_glx_context)
        {
            glXDestroyContext( m_glx_display, m_glx_context );
            m_glx_context = 0;
        }

        if (m_glx_window)
        {
            XDestroyWindow( m_glx_display, m_glx_window );
            m_glx_window = 0;
        }

        if (m_glx_cmap)
        {
            XFreeColormap( m_glx_display, m_glx_cmap );
            m_glx_cmap = 0;
        }

        XCloseDisplay( m_glx_display );
        m_glx_display = nullptr;
    }
#endif // HAS_GLX


    if (m_p_window)
        {
        glfwDestroyWindow(m_p_window);
        m_p_window = 0;
        }

    m_ready = false;
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
//------------------------------------------------------------------
const char* Context::getDisplayExtensions(EGLDisplay aDisplay) const
//------------------------------------------------------------------
{
    if (aDisplay == EGL_NO_DISPLAY)
    {
        LOGGER.logNow("EGL client extensions:") << std::endl;
    }
    else
    {
        LOGGER.logNow("EGL extensions:") << std::endl;
    }

    const char* extensions = eglQueryString(aDisplay, EGL_EXTENSIONS);

    if (!extensions)
    {
        LOGGER.logWarning("\tRetrieving EGL extensions failed") << std::endl;
    }
    else
    {
        LOGGER.logNow(extensions) << std::endl;
    }

    return extensions;
}


//----------------------------------------------------------
void Context::createDisplay(int aPlatform,
                            const std::string& aPlatformStr,
                            int aDeviceID)
//----------------------------------------------------------
{
    LOGGER.logNow("Try to build an EGL display for ") << aPlatformStr << "." << std::endl;

    if (aDeviceID < 0)
    {
        if (aPlatform != EGL_PLATFORM_GBM_MESA)
        {
            m_display = eglGetPlatformDisplayEXT(aPlatform, EGL_DEFAULT_DISPLAY, NULL);
        }
        else
#ifdef HAS_GBM
        {
            const char* device_names[] = {"/dev/dri/card0", "/dev/dri/renderD128", "/dev/nvidia0"};
            int fd = -1;

            for (int i = 0; i < 2; ++i)
            {
                if (fd < 0)
                {
                    fd = open(device_names[i], O_RDWR);

                    if (fd < 0)
                    {
                        LOGGER.logWarning("\tCannot open the device (") << device_names[i] <<
                            "). It's usualy a file permission issue. Check with your adminstrator." <<
                            std::endl;
                    }
                    else
                    {
                        LOGGER.logNow("\tThe device (") << device_names[i] << ") is open." << std::endl;
                    }
                }
            }

            if (fd >= 0)
            {
                LOGGER.logNow("\tOpen the corresponding GBM device.") << std::endl;

                gbm_device* p_gbm_device = gbm_create_device(fd);
                m_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_GBM_MESA,
                                                       p_gbm_device,
                                                       NULL);
            }

            if (m_display == EGL_NO_DISPLAY)
            {
                LOGGER.logWarning("\tGBM was requested and it is enabled, but it failed to create a display. We'll try an alternative, but expect the unexpected!") << std::endl;
                m_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, EGL_DEFAULT_DISPLAY, NULL);
            }
        }
#else // HAS_GBM
        {
            LOGGER.logWarning("\tGBM was requested but it was not enabled at the compilation time. We'll try an alternative, but expect the unexpected!") << std::endl;
            m_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, EGL_DEFAULT_DISPLAY, NULL);
        }
#endif // HAS_GBM
    }
    else
    {
        m_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, m_p_egl_devices_set[aDeviceID], 0);
    }

    if (m_display != EGL_NO_DISPLAY)
    {
        LOGGER.logNow("\tSuccess.") << std::endl;
        const char* extensions = getDisplayExtensions(m_display);

        EGLint egl_major_ver = 0;
        EGLint egl_minor_ver = 0;

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

            m_display = EGL_NO_DISPLAY;
        }
        else
        {
            LOGGER.logNow("EGL version: ") << egl_major_ver << "." << egl_minor_ver << std::endl;
        }
    }
    else
    {
        LOGGER.logWarning("\tFailure.") << std::endl;
    }
}


//------------------------------------------------------
std::vector<std::string> Context::findEGLClients() const
//------------------------------------------------------
{
    std::vector<std::string> client_set;

#ifdef HAS_EGL

    std::string client_extensions = getDisplayExtensions(EGL_NO_DISPLAY);

    if (client_extensions.find("EGL_EXT_platform_base") != std::string::npos)
    {
        if (client_extensions.find("EGL_MESA_platform_surfaceless") != std::string::npos)
        {
            client_set.push_back("Surfaceless platform");
        }

        if (client_extensions.find("EGL_EXT_platform_x11") != std::string::npos ||
            client_extensions.find("EGL_KHR_platform_x11") != std::string::npos)
        {
            client_set.push_back("X11 platform");
        }

        if (client_extensions.find("EGL_EXT_platform_wayland") != std::string::npos ||
            client_extensions.find("EGL_KHR_platform_wayland") != std::string::npos)
        {
            client_set.push_back("Wayland platform");
        }

        if (client_extensions.find("EGL_KHR_platform_android") != std::string::npos)
        {
            client_set.push_back("Android platform");
        }

#ifdef HAS_GBM
        if (client_extensions.find("EGL_EXT_platEGL_MESA_platform_gbmform_wayland") != std::string::npos ||
            client_extensions.find("EGL_KHR_platform_gbm") != std::string::npos)
        {
            client_set.push_back("GBM platform");
        }
#endif // HAS_GBM

        if (client_extensions.find("EGL_EXT_device_enumeration") != std::string::npos ||
            client_extensions.find("EGL_EXT_platform_device") != std::string::npos)
        {
            client_set.push_back("Device platform");
        }
    }


#endif // HAS_EGL

    return client_set;
}


//-------------------------------------------------------------------
bool Context::initialiseEGL(std::vector<std::string>& anEGLClientSet)
//-------------------------------------------------------------------
{
    // Initialise EGL
    LOGGER.logNow("Initialise EGL") << std::endl;

    initEGLFunctions();

    // Print EGL extensions

    // Find all the EGL clients
    if (anEGLClientSet.size() == 0)
        anEGLClientSet = findEGLClients();


    LOGGER.logNow("Try to build an EGL display.") << std::endl;
    while (anEGLClientSet.size())
    {
        std::string EGL_client = anEGLClientSet.back();
        anEGLClientSet.pop_back();

        if (m_display == EGL_NO_DISPLAY && EGL_client == "Android platform")
        {
            createDisplay(EGL_PLATFORM_ANDROID_KHR, EGL_client);
        }
        else if (m_display == EGL_NO_DISPLAY && EGL_client == "Wayland platform")
        {
            createDisplay(EGL_PLATFORM_WAYLAND_EXT, EGL_client);
        }
        else if (m_display == EGL_NO_DISPLAY && EGL_client == "Surfaceless platform")
        {
            createDisplay(EGL_PLATFORM_SURFACELESS_MESA, EGL_client);
        }
#ifdef HAS_GBM
        else if (m_display == EGL_NO_DISPLAY && EGL_client == "GBM platform")
        {
            createDisplay(EGL_PLATFORM_GBM_MESA, EGL_client);
        }
#endif // HAS_GBM
        else if (m_display == EGL_NO_DISPLAY && EGL_client == "X11 platform")
        {
            createDisplay(EGL_PLATFORM_X11_EXT, EGL_client);
        }
        else if (m_display == EGL_NO_DISPLAY && EGL_client == "Device platform")
        {
            LOGGER.logNow("Go through all the EGL devices to find a suitable display.") << std::endl;
            LOGGER.logNow("Surfaceless have the lowest priority.") << std::endl;

            EGLint number_of_egl_devices = getNumberOfEGLDevices();
            m_p_egl_devices_set.resize(number_of_egl_devices);
            eglQueryDevicesEXT(number_of_egl_devices, &m_p_egl_devices_set[0], &number_of_egl_devices);

            std::vector<int> p_priority_queue(number_of_egl_devices);
            int start_id = 0;
            int end_id = number_of_egl_devices - 1;

            for (int i = 0; i < number_of_egl_devices; ++i)
            {
                EGLDeviceEXT device = m_p_egl_devices_set[i];
                const std::string client_extensions = eglQueryDeviceStringEXT(device, EGL_EXTENSIONS);
                LOGGER.logNow("Device ") << i << "'s extensions: " << client_extensions << std::endl;

                if (client_extensions.find("EGL_MESA_platform_surfaceless") != std::string::npos)
                {
                    p_priority_queue[end_id--] = i;
                }
                else
                {
                    p_priority_queue[start_id++] = i;
                }
            }

            for (int i = 0; i < number_of_egl_devices; ++i)
            {
                if (m_display == EGL_NO_DISPLAY)
                {
                    std::string platform = "Platform device " + std::to_string(p_priority_queue[i] + 1) + "/" + std::to_string(number_of_egl_devices);
                    createDisplay(EGL_PLATFORM_DEVICE_EXT, platform, p_priority_queue[i]);
                }
            }
        }

        if (m_display != EGL_NO_DISPLAY) return true;
    }

    return false;
}
#endif


// Code taken from:
// https://www.khronos.org/opengl/wiki/Tutorial:_OpenGL_3.0_Context_Creation_(GLX)
#ifdef HAS_GLX
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
#endif


// Code taken from:
// https://www.khronos.org/opengl/wiki/Tutorial:_OpenGL_3.0_Context_Creation_(GLX)
static bool isExtensionSupported(const char *extList, const char *extension)
{
  const char *start;
  const char *where, *terminator;
  
  /* Extension names should not have spaces. */
  where = strchr(extension, ' ');
  if (where || *extension == '\0')
    return false;

  /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
  for (start=extList;;) {
    where = strstr(start, extension);

    if (!where)
      break;

    terminator = where + strlen(extension);

    if ( where == start || *(where - 1) == ' ' )
      if ( *terminator == ' ' || *terminator == '\0' )
        return true;

    start = terminator;
  }

  return false;
}


//-----------------------------
void Context::createGLWithGLX()
//-----------------------------
{
    // Release the previous context if applicable
	release();

#ifdef HAS_GLX
    // Code adapted from:
    // https://www.khronos.org/opengl/wiki/Tutorial:_OpenGL_3.0_Context_Creation_(GLX)

    m_ready = false;

    // Open the X display
    m_glx_display = XOpenDisplay(NULL);

    if (!m_glx_display)
    {
        LOGGER.logWarning("Failed to open X display") << std::endl;
        return;
    }

    // Get a matching FB config
    static int visual_attribs[] =
    {
        GLX_X_RENDERABLE    , True,
        GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
        GLX_RENDER_TYPE     , GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
        GLX_RED_SIZE        , 8,
        GLX_GREEN_SIZE      , 8,
        GLX_BLUE_SIZE       , 8,
        GLX_ALPHA_SIZE      , 8,
        GLX_DEPTH_SIZE      , 8,
        GLX_STENCIL_SIZE    , 8,
        GLX_DOUBLEBUFFER    , False,
        //GLX_SAMPLE_BUFFERS  , 1,
        //GLX_SAMPLES         , 4,
        None
    };

    int glx_major, glx_minor;
 
    // FBConfigs were added in GLX version 1.3.
    if ( !glXQueryVersion( m_glx_display, &glx_major, &glx_minor ) || 
        ( ( glx_major == 1 ) && ( glx_minor < 3 ) ) || ( glx_major < 1 ) )
    {
        LOGGER.logWarning("Invalid GLX version") << std::endl;
        return;
    }

    LOGGER.logNow("Getting matching framebuffer configs") << std::endl;
    int fbcount;
    GLXFBConfig* fbc = glXChooseFBConfig(m_glx_display, DefaultScreen(m_glx_display), visual_attribs, &fbcount);
    if (!fbc)
    {
        LOGGER.logWarning("Failed to retrieve a framebuffer config") << std::endl;
        return;
    }
    LOGGER.logNow( "Found ") << fbcount << " matching FB configs." << std::endl;

    // Pick the FB config/visual with the most samples per pixel
    LOGGER.logNow("Getting XVisualInfos") << std::endl;
    int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

    for (int i=0; i<fbcount; ++i)
    {
        XVisualInfo *vi = glXGetVisualFromFBConfig( m_glx_display, fbc[i] );
        if ( vi )
        {
            int samp_buf, samples;
            glXGetFBConfigAttrib( m_glx_display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
            glXGetFBConfigAttrib( m_glx_display, fbc[i], GLX_SAMPLES       , &samples  );

            if ( best_fbc < 0 || samp_buf && samples > best_num_samp )
                best_fbc = i, best_num_samp = samples;
            
            if ( worst_fbc < 0 || !samp_buf || samples < worst_num_samp )
                worst_fbc = i, worst_num_samp = samples;
        }
        XFree( vi );
    }

    GLXFBConfig bestFbc = fbc[ best_fbc ];

    // Be sure to free the FBConfig list allocated by glXChooseFBConfig()
    XFree( fbc );

    // Get a visual
    XVisualInfo *vi = glXGetVisualFromFBConfig( m_glx_display, bestFbc );

    LOGGER.logNow("Creating colormap") << std::endl;
    XSetWindowAttributes swa;

    m_glx_cmap = XCreateColormap( m_glx_display,
    RootWindow( m_glx_display, vi->screen ), vi->visual, AllocNone );
    swa.colormap = m_glx_cmap;
    swa.background_pixmap = None ;
    swa.border_pixel      = 0;
    swa.event_mask        = StructureNotifyMask;

    LOGGER.logNow("Creating window") << std::endl;
    m_glx_window = XCreateWindow( m_glx_display, RootWindow( m_glx_display, vi->screen ), 
        0, 0, 100, 100, 0, vi->depth, InputOutput, 
        vi->visual, 
        CWBorderPixel|CWColormap|CWEventMask, &swa );

    // Done with the visual info data
    XFree( vi );

    if ( !m_glx_window )
    {
        LOGGER.logWarning("Failed to create window") << std::endl;
        return;
    }

    XStoreName( m_glx_display, m_glx_window, "gVirtualXray" );

    LOGGER.logNow("Mapping window") << std::endl;
    XMapWindow( m_glx_display, m_glx_window );

    // Get the default screen's GLX extension list
    const char *glxExts = glXQueryExtensionsString( m_glx_display,
    DefaultScreen( m_glx_display ) );

    // NOTE: It is not necessary to create or make current to a context before
    // calling glXGetProcAddressARB
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
        glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );

    // Install an X error handler so the application won't exit if GL 3.0
    // context allocation fails.
    //
    // Note this error handler is global.  All display connections in all threads
    // of a process use the same error handler, so be sure to guard against other
    // threads issuing X commands while this code is running.
    // ctxErrorOccurred = false;
    // int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&ctxErrorHandler);

    // Check for the GLX_ARB_create_context extension string and the function.
    // If either is not present, use GLX 1.3 context creation method.
    if ( !isExtensionSupported( glxExts, "GLX_ARB_create_context" ) || !glXCreateContextAttribsARB )
    {
        LOGGER.logWarning("glXCreateContextAttribsARB() not found ... using old-style GLX context") << std::endl;
        m_glx_context = glXCreateNewContext( m_glx_display, bestFbc, GLX_RGBA_TYPE, 0, True );
    }
    // If it does, try to get a GL m_major_version.m_minor_version context!
    else
    {
        int context_attribs[] =
        {
            GLX_CONTEXT_MAJOR_VERSION_ARB, m_major_version,
            GLX_CONTEXT_MINOR_VERSION_ARB, m_minor_version,
            //GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            None
        };

        LOGGER.logNow("Creating context") << std::endl;
        m_glx_context = glXCreateContextAttribsARB( m_glx_display, bestFbc, 0,
            True, context_attribs );

        // Sync to ensure any errors generated are processed.
        XSync( m_glx_display, False );
        if (  m_glx_context )
            LOGGER.logNow("Creating GL ") << m_major_version << "." << m_minor_version << " context" << std::endl;
        else
        {
            LOGGER.logNow("Couldn't create GL ") << m_major_version << "." << m_minor_version << " context" << std::endl;
            return;
        }
    }

    // Sync to ensure any errors generated are processed.
    XSync( m_glx_display, False );

    // Verifying that context is a direct context
    if ( ! glXIsDirect ( m_glx_display, m_glx_context ) )
    {
        LOGGER.logNow("Indirect GLX rendering context obtained") << std::endl;
        printf( "Indirect GLX rendering context obtained\n" );
    }
    else
    {
        LOGGER.logNow("Direct GLX rendering context obtained") << std::endl;
    }

    LOGGER.logNow("Making context current") << std::endl;
    glXMakeCurrent( m_glx_display, m_glx_window, m_glx_context );

    // Initialise GLEW
    gVirtualXRay::initialiseGLEW();

    LOGGER.logNow("OpenGL version supported by this platform ") << glGetString(GL_VERSION) << std::endl;

    // Get rid of any OpenGL error at this stage
    glGetError();

    int major = 0;
    int minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    if (major == m_major_version && minor == m_minor_version)
    {
        m_ready = true;
    }
    
#endif
}


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

    // Initialise GLAD
#ifdef HAS_GLAD
    gVirtualXRay::initialiseGLAD("EGL");
#endif

	// Initialise EGL
    std::vector<std::string> EGL_client_set = findEGLClients();

    // Go through all the clients until one hopefully works
    m_ready = false;
    while (EGL_client_set.size() && m_ready == false)
    {
        if (initialiseEGL(EGL_client_set))
        {
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
            if (eglChooseConfig(m_display, attrib_list, &m_config, 1, &num_config) == EGL_FALSE)
            {
                LOGGER.logWarning("Failed to eglChooseConfig") << std::endl;
                m_ready = false;
            }
            else if(num_config < 1)
            {
                LOGGER.logWarning("No matching EGL frame buffer configuration") << std::endl;
                m_ready = false;
            }
            else
            {
                // Bind the OpenGL API to the context
                LOGGER.logNow("Bind the OpenGL API to EGL") << std::endl;
                if (eglBindAPI(EGL_OPENGL_API) == EGL_FALSE)
                {
                    LOGGER.logWarning("Failed to eglBindAPI(EGL_OPENGL_API)") << std::endl;
                    m_ready = false;
                }
                else
                {
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
                    if (m_context == EGL_NO_CONTEXT)
                    {
                        m_context = eglCreateContext(m_display, m_config, EGL_NO_CONTEXT, nullptr);
                    }

                    // The context was not created
                    if (m_context == EGL_NO_CONTEXT)
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
                    }
                    else
                    {
                        // Create the surface
                        LOGGER.logNow("Create the surface") << std::endl;
                        m_surface = eglCreatePbufferSurface(m_display,
                                m_config,
                                nullptr);

                        // The surface was not created
                        if (!m_surface)
                        {
                            LOGGER.logWarning("Error: eglCreatePbufferSurface failed Error") << std::endl;
                            m_ready = false;
                        }
                        else
                        {
                            // Make current
                            LOGGER.logNow("Make the context current") << std::endl;
                            if (eglMakeCurrent(m_display,
                                    m_surface,
                                    m_surface,
                                    m_context) == EGL_FALSE)
                            {
                                LOGGER.logWarning("Failed to eglMakeCurrent") << std::endl;
                                m_ready = false;
                            }
                            else
                            {
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
                                }
                                else
                                {
                                    LOGGER.logNow("OpenGL version supported by this platform ") << glGetString(GL_VERSION) << std::endl;

                                    // Get rid of any OpenGL error at this stage
                                    glGetError();

                                    int major = 0;
                                    int minor = 0;
                                    glGetIntegerv(GL_MAJOR_VERSION, &major);
                                    glGetIntegerv(GL_MINOR_VERSION, &minor);

                                    if (major == m_major_version && minor == m_minor_version)
                                    {
                                        m_ready = true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
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

        int major = 0;
        int minor = 0;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);

        if (major == m_major_version && minor == m_minor_version)
        {
            m_ready = true;
        }
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
