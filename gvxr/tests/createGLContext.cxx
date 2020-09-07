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

#include "gtest/gtest.h"

#ifndef __OpenGLUtilities_h
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>

#ifndef __createGLContext_h
#include "createGLContext.h"
#endif


using namespace std;


//--------------------------------------------------------------
void createGLContext(contextData& aWholeContextData,

        int gl_req_major_ver,
        int gl_req_minor_ver)
//--------------------------------------------------------------
{
    aWholeContextData.ready = false;

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

    aWholeContextData.egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if(aWholeContextData.egl_display == EGL_NO_DISPLAY)
    {
        cerr << "Failed to eglGetDisplay(EGL_DEFAULT_DISPLAY)" << endl;
        aWholeContextData.ready = false;
	return;
    }

    EGLint egl_major_ver;
    EGLint egl_minor_ver;
    if(eglInitialize(aWholeContextData.egl_display, &egl_major_ver, &egl_minor_ver) == EGL_FALSE)
    {
        cerr << "Failed to eglInitialize" << endl;
        EGLint error =  eglGetError();
        switch(error)
        {
        case EGL_BAD_DISPLAY:
            cerr << "display is not an EGL display connection" << endl;
            break;
        case EGL_NOT_INITIALIZED:
            cerr << "display cannot be initialized" << endl;
            break;
        default:
            break;
        }
        aWholeContextData.ready = false;
	return;
    }
    cout << "EGL version: " << egl_major_ver << "." << egl_minor_ver << endl;

    char const * client_apis = eglQueryString(aWholeContextData.egl_display, EGL_CLIENT_APIS);
    if(!client_apis)
    {
        cerr << "Failed to eglQueryString(display, EGL_CLIENT_APIS)" << endl;
        aWholeContextData.ready = false;
	return;

    }
    cout << "Supported client rendering APIs: " << client_apis << endl;

    EGLint    num_config;
    if(eglChooseConfig(aWholeContextData.egl_display, attrib_list, &aWholeContextData.egl_config, 1, &num_config) == EGL_FALSE)
    {
        cerr << "Failed to eglChooseConfig" << endl;
        aWholeContextData.ready = false;
	return;

    }
    if(num_config < 1)
    {
        cerr << "No matching EGL frame buffer configuration" << endl;
        aWholeContextData.ready = false;
	return;
    }

    if(eglBindAPI(EGL_OPENGL_API) == EGL_FALSE)
    {
        cerr << "Failed to eglBindAPI(EGL_OPENGL_API)" << endl;
        aWholeContextData.ready = false;
	return;

    }

    const EGLint context_attrib[] =
    {
        EGL_CONTEXT_MAJOR_VERSION_KHR,  gl_req_major_ver,
        EGL_CONTEXT_MINOR_VERSION_KHR,  gl_req_minor_ver,
        EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR,    EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR,
        EGL_CONTEXT_FLAGS_KHR,          EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT_KHR
#ifndef NDEBUG
        | EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR
#endif
        ,
        EGL_NONE
    };

    aWholeContextData.egl_context = eglCreateContext(aWholeContextData.egl_display, aWholeContextData.egl_config, EGL_NO_CONTEXT, context_attrib);
    if(aWholeContextData.egl_context == EGL_NO_CONTEXT)
    {
        aWholeContextData.egl_context = eglCreateContext(aWholeContextData.egl_display, aWholeContextData.egl_config, EGL_NO_CONTEXT, nullptr);
    }

    if(aWholeContextData.egl_context == EGL_NO_CONTEXT)
    {
        cerr << "Failed to eglCreateContext" << endl;
        EGLint error =  eglGetError();
        switch(error)
        {
        case EGL_BAD_CONFIG:
            cerr << "config is not an EGL frame buffer configuration, or does not support the current rendering API" << endl;
            break;
        case EGL_BAD_ATTRIBUTE:
            cerr << "attrib_list contains an invalid context attribute or if an attribute is not recognized or out of range" << endl;
            break;
        default:
            cerr << "Unknown error" << endl;
            break;
        }
        aWholeContextData.ready = false;
	return;

    }

   
	printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));


 aWholeContextData.egl_surface = eglCreatePbufferSurface(aWholeContextData.egl_display,
  	aWholeContextData.egl_config,
  	nullptr);
 


   if (!aWholeContextData.egl_surface) {
          cerr << "Error: eglCreatePbufferSurface failed Error" << endl;
          return;
   }

    if(eglMakeCurrent(aWholeContextData.egl_display, aWholeContextData.egl_surface, aWholeContextData.egl_surface, aWholeContextData.egl_context) == EGL_FALSE)
    {
        cerr << "Failed to eglMakeCurrent" << endl;
        aWholeContextData.ready = false;
	return;

    }

    /*GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        cerr << "Failed to glewInit\n";
        return false;
    }*/

    if(EGL_SUCCESS != eglGetError())
    {
        aWholeContextData.ready = false;
	return;

    }

    aWholeContextData.ready = true;

	// Create an OpenGL framebuffer as render target.
	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	//assertOpenGLError("glBindFramebuffer");

	printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));


	// Create a texture as color attachment.
	GLuint t;
	glGenTextures(1, &t);

	glBindTexture(GL_TEXTURE_2D, t);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 500, 500, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	//assertOpenGLError("glTexImage2D");
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	
	// Attach the texture to the framebuffer.
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, t, 0);
	//assertOpenGLError("glFramebufferTexture2D");

	
	// Render something.
	glClearColor(0.9, 0.8, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();

	
	// Read the framebuffer's color attachment and save it as a PNG file.
        GLubyte* buffer = new GLubyte[500 * 500 * 3];
        if (buffer)
	{
		FILE* p_file = fopen("img.raw", "wb");

		if (p_file)
		{
			glReadBuffer(GL_COLOR_ATTACHMENT0);
			glReadPixels(0, 0, 500, 500, GL_RGB, GL_UNSIGNED_BYTE, buffer);
			//assertOpenGLError("glReadPixels");

			fwrite(buffer, sizeof(GLubyte), 500 * 500 * 3, p_file); 
			fclose(p_file);
		}

		delete [] buffer;
	}	
	
	// Destroy context.
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteTextures(1, &t);	
}


//-----------------------------------------------
void destroyGLContext(contextData& aWholeContextData)
//-----------------------------------------------
{
	eglDestroyContext(aWholeContextData.egl_display, aWholeContextData.egl_context);
	eglTerminate(aWholeContextData.egl_display);
}
