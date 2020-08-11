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
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>

#include "gtest/gtest.h"

#ifndef __OpenGLUtilities_h
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef __createGLContext_h
#include "createGLContext.h"
#endif


//--------------------------------------------------------------
void createGLContext(EGLContext& aContext, EGLDisplay& aDisplay)
//--------------------------------------------------------------
{
    EGLConfig config;
    EGLSurface surface;
    EGLint num_config;

    EGLint error_code;

    aDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    error_code = eglGetError();
    ASSERT_EQ(error_code, EGL_SUCCESS) << "Cannot run eglGetDisplay.";

    eglInitialize(aDisplay, nullptr, nullptr);
    error_code = eglGetError();
    ASSERT_EQ(error_code, EGL_SUCCESS) << "Cannot run eglInitialize.";

    eglChooseConfig(aDisplay, nullptr, &config, 1, &num_config);
    error_code = eglGetError();
    ASSERT_EQ(error_code, EGL_SUCCESS) << "Cannot run eglChooseConfig.";

    eglBindAPI(EGL_OPENGL_API);
    error_code = eglGetError();
    ASSERT_EQ(error_code, EGL_SUCCESS) << "Cannot run eglBindAPI.";

    aContext = eglCreateContext(aDisplay, config, EGL_NO_CONTEXT, NULL);
    error_code = eglGetError();
    ASSERT_EQ(error_code, EGL_SUCCESS) << "Cannot run eglCreateContext.";

    //surface = eglCreatePbufferSurface(display, config, nullptr);
    //assertEGLError("eglCreatePbufferSurface");

    eglMakeCurrent(aDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, aContext);
    error_code = eglGetError();
    ASSERT_EQ(error_code, EGL_SUCCESS) << "Cannot run eglMakeCurrent.";


    GLuint frameBuffer;
  	glGenFramebuffers(1, &frameBuffer);
    error_code = glGetError();
    ASSERT_EQ(error_code, GL_NO_ERROR) << "Cannot run glGenFramebuffers.";

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    error_code = glGetError();
    ASSERT_EQ(error_code, GL_NO_ERROR) << "Cannot run glBindFramebuffer.";

	GLuint t;
	glGenTextures(1, &t);
    error_code = glGetError();
    ASSERT_EQ(error_code, GL_NO_ERROR) << "Cannot run glGenTextures.";

	glBindTexture(GL_TEXTURE_2D, t);
    error_code = glGetError();
    ASSERT_EQ(error_code, GL_NO_ERROR) << "Cannot run glBindTexture.";

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    error_code = glGetError();
    ASSERT_EQ(error_code, GL_NO_ERROR) << "Cannot run glTexImage2D.";

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    error_code = glGetError();
    ASSERT_EQ(error_code, GL_NO_ERROR) << "Cannot run glTexParameteri.";

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, t, 0);
    error_code = glGetError();
    ASSERT_EQ(error_code, GL_NO_ERROR) << "Cannot run glFramebufferTexture2D.";

  	printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));
}


//---------------------------------------------------------------
void destroyGLContext(EGLContext& aContext, EGLDisplay& aDisplay)
//---------------------------------------------------------------
{
    EGLint error_code;

    eglDestroyContext(aDisplay, aContext);
    error_code = eglGetError();
    ASSERT_EQ(error_code, EGL_SUCCESS) << "Cannot run eglDestroyContext.";

    eglTerminate(aDisplay);
    error_code = eglGetError();
    ASSERT_EQ(error_code, EGL_SUCCESS) << "Cannot run eglTerminate.";
}
