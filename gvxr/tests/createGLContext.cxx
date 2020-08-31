/*

Copyright (c) 2019, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*   @file       createGLContext.cxx
*
*   @brief      Create an OpenGL constext using EGL
*               (see https://www.khronos.org/egl for details)
*
*   @version    1.0
*
*   @date       11/08/2020
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
*               http://www.fpvidal.net/, Aug 2020, 2020, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************

#include <iostream>


#include <fcntl.h> // For O_RDWR
//#include <unistd.h>
#include <stdlib.h> // For malloc/free

#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>


/*
#ifdef HAS_GBM
#include <gbm.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
*/
#include "gtest/gtest.h"

#ifndef __OpenGLUtilities_h
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef __createGLContext_h
#include "createGLContext.h"
#endif



/*
void checkExtensions(void)
{
#ifdef HAS_GBM
#ifdef EGL_MESA_platform_gbm
    const char* client_extensions = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);

    if (!client_extensions) {
        // EGL_EXT_client_extensions is unsupported.
        abort();
    }
    if (!strstr(client_extensions, "EGL_MESA_platform_gbm")) {
        abort();
    }
#endif
#endif
}


//---------------------------------------------
void getDisplay(contextData& aWholeContextData)
//---------------------------------------------
{
    EGLint error_code;

#ifdef HAS_GBM
    // Open the device
    int fd = open("/dev/dri/card0", O_RDWR | FD_CLOEXEC);
    if (fd >= 0)
    {
		aWholeContextData.p_device = gbm_create_device(fd);
		if (!aWholeContextData.p_device)
        {
			abort();
		}

#ifdef EGL_MESA_platform_gbm
		aWholeContextData.egl_display = eglGetPlatformDisplay(EGL_PLATFORM_GBM_MESA, aWholeContextData.p_device, NULL);
		error_code = eglGetError();
		ASSERT_EQ(error_code, EGL_SUCCESS) << "Cannot run eglGetPlatformDisplay.";
#else // EGL_MESA_platform_gbm
		aWholeContextData.egl_display = eglGetDisplay(aWholeContextData.p_device);
		error_code = eglGetError();
		ASSERT_EQ(error_code, EGL_SUCCESS) << "Cannot run eglGetDisplay.";
#endif // EGL_MESA_platform_gbm
    }
    else
#endif // HAS_GBM
    {
		aWholeContextData.egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		error_code = eglGetError();
		ASSERT_EQ(error_code, EGL_SUCCESS) << "Cannot run eglGetDisplay.";
    }

    if (aWholeContextData.egl_display == EGL_NO_DISPLAY) {
        abort();
    }

    EGLint major, minor;
    eglInitialize(aWholeContextData.egl_display, &major, &minor);
    error_code = eglGetError();
    ASSERT_EQ(error_code, EGL_SUCCESS) << "Cannot run eglInitialize.";
}

void getConfig(contextData& aWholeContextData)
{
    EGLint error_code;
    EGLint num_configs;

    EGLint egl_config_attribs[] = {
        EGL_BUFFER_SIZE,        32,
        EGL_DEPTH_SIZE,         EGL_DONT_CARE,
        EGL_STENCIL_SIZE,       EGL_DONT_CARE,
        EGL_RENDERABLE_TYPE,    EGL_OPENGL_BIT,
        EGL_SURFACE_TYPE,       EGL_PIXMAP_BIT,
        EGL_NONE,
    };

#ifdef HAS_GBM
    eglGetConfigs(aWholeContextData.egl_display, NULL, 0, &num_configs);
    error_code = eglGetError();
    ASSERT_EQ(error_code, EGL_SUCCESS) << "Cannot run eglGetConfigs.";

    EGLConfig *configs = (EGLConfig *) malloc(num_configs * sizeof(EGLConfig));
    eglChooseConfig(aWholeContextData.egl_display, egl_config_attribs,
                         configs, num_configs, &num_configs);
    error_code = eglGetError();
    ASSERT_EQ(error_code, EGL_SUCCESS) << "Cannot run eglChooseConfig.";

    if (num_configs == 0) {
        abort();
    }

    // Find a config whose native visual ID is the desired GBM format.
    for (int i = 0; i < num_configs; ++i) {
        EGLint gbm_format;

        eglGetConfigAttrib(aWholeContextData.egl_display, configs[i],
                                EGL_NATIVE_VISUAL_ID, &gbm_format);
        error_code = eglGetError();
        ASSERT_EQ(error_code, EGL_SUCCESS) << "Cannot run eglGetConfigAttrib.";

        if (gbm_format == GBM_FORMAT_XRGB8888) {
            aWholeContextData.egl_config = configs[i];
            free(configs);
            return;
        }
    }
#endif

    // Failed to find a config with matching GBM format.
    eglChooseConfig(aWholeContextData.egl_display, nullptr, &aWholeContextData.egl_config, 1, &num_configs);
    error_code = eglGetError();
    ASSERT_EQ(error_code, EGL_SUCCESS) << "Cannot run eglChooseConfig.";
}

void getSurface(contextData& aWholeContextData)
{
#ifdef HAS_GBM
    if (aWholeContextData.p_device)
    {
		aWholeContextData.p_surface = gbm_surface_create(aWholeContextData.p_device,
										256, 256,
										GBM_FORMAT_XRGB8888,
										GBM_BO_USE_RENDERING);
		if (!aWholeContextData.p_surface) {
			abort();
		}

#ifdef EGL_MESA_platform_gbm
		aWholeContextData.egl_surface = eglCreatePlatformWindowSurface(aWholeContextData.egl_display,
													   aWholeContextData.egl_config,
													   aWholeContextData.p_surface,
													   NULL);
#else // EGL_MESA_platform_gbm
		aWholeContextData.egl_surface = eglCreateWindowSurface(aWholeContextData.egl_display,
											aWholeContextData.egl_config,
											aWholeContextData.p_surface,
											NULL);
#endif // EGL_MESA_platform_gbm
    }
    else
#endif // HAS_GBM
    {
    	aWholeContextData.egl_surface = eglCreatePbufferSurface(aWholeContextData.egl_display, aWholeContextData.egl_config, nullptr);
        //assertEGLError("eglCreatePbufferSurface");
    }

    if (aWholeContextData.egl_surface == EGL_NO_SURFACE) {
        abort();
    }
}
*/


//--------------------------------------------------------------
void createGLContext(contextData& aWholeContextData)
//--------------------------------------------------------------
{
    aWholeContextData.ready = false;

    // Initialize GLFW
    ASSERT_EQ(glfwInit(), true) << "Cannot initialise GLFW.";

#ifndef WIN32
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
#endif
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    // Enable anti-aliasing
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create a windowed mode window and its OpenGL context
    aWholeContextData.m_p_window = glfwCreateWindow(1, 1, "Unit test", 0, 0);
    ASSERT_EQ(aWholeContextData.m_p_window != nullptr, true) << "Cannot create a window.";

    // Make the window's context current
    glfwMakeContextCurrent(aWholeContextData.m_p_window);

    // Initialise GLEW
    gVirtualXRay::initialiseGLEW();

/*    aWholeContextData.ready = false;
    checkExtensions();

    EGLint num_config;

    EGLint error_code;

    getDisplay(aWholeContextData);
    getConfig(aWholeContextData);
    getSurface(aWholeContextData);

    eglBindAPI(EGL_OPENGL_API);
    error_code = eglGetError();
    ASSERT_EQ(error_code, EGL_SUCCESS) << "Cannot run eglBindAPI.";

    EGLint opengl_major_version [] = {
        4, 3, 0, 2, 2
    };

    EGLint opengl_minor_version [] = {
        5, 1, 0, 2, 1
    };

    bool context_created = false;
    for (int i = 0; i < sizeof(opengl_major_version) / sizeof(EGLint); ++i)
    {
        if (!context_created)
        {
            EGLint ctxattr[] = {
                EGL_CONTEXT_MAJOR_VERSION, opengl_major_version[i],
                EGL_CONTEXT_MINOR_VERSION, opengl_minor_version[i],
                EGL_NONE
            };

            aWholeContextData.egl_context = eglCreateContext(aWholeContextData.egl_display, aWholeContextData.egl_config, EGL_NO_CONTEXT, ctxattr);
            error_code = eglGetError();

            if (error_code == EGL_SUCCESS)
            {
                std::cout << "Create OpenGL " << opengl_major_version[i] << "." << opengl_minor_version[i] << " context" << std::endl;
                context_created = true;
            }
        }
    }

    if (!context_created)
    {
        aWholeContextData.egl_context = eglCreateContext(aWholeContextData.egl_display, aWholeContextData.egl_config, EGL_NO_CONTEXT, nullptr);
        error_code = eglGetError();
        ASSERT_EQ(error_code, EGL_SUCCESS) << "Cannot run eglCreateContext.";
    }

    //surface = eglCreatePbufferSurface(display, config, nullptr);
    //assertEGLError("eglCreatePbufferSurface");

    eglMakeCurrent(aWholeContextData.egl_display, aWholeContextData.egl_surface, aWholeContextData.egl_surface, aWholeContextData.egl_context);
    error_code = eglGetError();
    ASSERT_EQ(error_code, EGL_SUCCESS) << "Cannot run eglMakeCurrent.";

    printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));
    aWholeContextData.ready = true;*/
}


//-----------------------------------------------
void destroyGLContext(contextData& aWholeContextData)
//-----------------------------------------------
{/*
    EGLint error_code;

    //eglDestroyContext(aWholeContextData.egl_display, aWholeContextData.egl_context);
    //error_code = eglGetError();
    //ASSERT_EQ(error_code, EGL_SUCCESS) << "Cannot run eglDestroyContext.";

    if (aWholeContextData.ready)
    {
		error_code = eglTerminate(aWholeContextData.egl_display);
		if (error_code == EGL_FALSE)
		{
			std::cerr << "Error running eglTerminate" << std::endl;
		}

		error_code = eglGetError();
		if (error_code == EGL_BAD_DISPLAY)
		{
			std::cerr << " display is not an EGL display connection" << std::endl;
		}

	    aWholeContextData.ready = false;
    }

    //ASSERT_EQ(error_code, EGL_SUCCESS) << "Cannot run eglTerminate.";*/

}
