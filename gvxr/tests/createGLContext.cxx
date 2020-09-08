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


contextData::contextData():
	m_ready(false),
	m_display(0),
	m_config(0),
	m_context(0),
	m_surface(0)
{}


contextData::~contextData()
{
	destroyGLContext(*this);
}


//--------------------------------------------------------------
void createGLContext(contextData& aWholeContextData,

        int gl_req_major_ver,
        int gl_req_minor_ver)
//--------------------------------------------------------------
{
	if (!aWholeContextData.m_ready)
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

		// Get the display
		aWholeContextData.m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		if(aWholeContextData.m_display == EGL_NO_DISPLAY)
			{
			cerr << "Failed to eglGetDisplay(EGL_DEFAULT_DISPLAY)" << endl;
			aWholeContextData.m_ready = false;
			return;
			}

		// Initialise EGL
		EGLint egl_major_ver;
		EGLint egl_minor_ver;
		if(eglInitialize(aWholeContextData.m_display, &egl_major_ver, &egl_minor_ver) == EGL_FALSE)
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
			aWholeContextData.m_ready = false;
			return;
			}
		cout << "EGL version: " << egl_major_ver << "." << egl_minor_ver << endl;

		// Choose the config supporting the attributes set above
		EGLint    num_config;
		if(eglChooseConfig(aWholeContextData.m_display, attrib_list, &aWholeContextData.m_config, 1, &num_config) == EGL_FALSE)
			{
			cerr << "Failed to eglChooseConfig" << endl;
			aWholeContextData.m_ready = false;
			return;
			}

		if(num_config < 1)
			{
			cerr << "No matching EGL frame buffer configuration" << endl;
			aWholeContextData.m_ready = false;
			return;
			}

		// Bind the OpenGL API to the context
		if(eglBindAPI(EGL_OPENGL_API) == EGL_FALSE)
			{
			cerr << "Failed to eglBindAPI(EGL_OPENGL_API)" << endl;
			aWholeContextData.m_ready = false;
			return;
			}

		// OpenGL context
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

		// Create the context
		aWholeContextData.m_context = eglCreateContext(aWholeContextData.m_display, aWholeContextData.m_config, EGL_NO_CONTEXT, context_attrib);
		if(aWholeContextData.m_context == EGL_NO_CONTEXT)
			{
			aWholeContextData.m_context = eglCreateContext(aWholeContextData.m_display, aWholeContextData.m_config, EGL_NO_CONTEXT, nullptr);
			}

		// The context was not created
		if(aWholeContextData.m_context == EGL_NO_CONTEXT)
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
			aWholeContextData.m_ready = false;
			return;
			}

		// Create the surface
		aWholeContextData.m_surface = eglCreatePbufferSurface(aWholeContextData.m_display,
				aWholeContextData.m_config,
				nullptr);

		// The surface was not created
		if (!aWholeContextData.m_surface)
			{
			cerr << "Error: eglCreatePbufferSurface failed Error" << endl;
			return;
			}

		// Make current
		if (eglMakeCurrent(aWholeContextData.m_display,
				aWholeContextData.m_surface,
				aWholeContextData.m_surface,
				aWholeContextData.m_context) == EGL_FALSE)
			{
			cerr << "Failed to eglMakeCurrent" << endl;
			aWholeContextData.m_ready = false;
			return;
			}

		// Initialise GLEW
		gVirtualXRay::initialiseGLEW();

		// Still no error?
		if(EGL_SUCCESS != eglGetError())
			{
			aWholeContextData.m_ready = false;
			return;
			}

		cout << "OpenGL version supported by this platform " << glGetString(GL_VERSION) << endl;

		aWholeContextData.m_ready = true;
	}
}

//-----------------------------------------------
void destroyGLContext(contextData& aWholeContextData)
//-----------------------------------------------
{
	if (aWholeContextData.m_display && aWholeContextData.m_context)
		{
		eglDestroyContext(aWholeContextData.m_display, aWholeContextData.m_context);
		aWholeContextData.m_context = 0;
		}

	if (aWholeContextData.m_display)
		{
		eglTerminate(aWholeContextData.m_display);
		aWholeContextData.m_display = 0;
		}

	aWholeContextData.m_ready = false;
}
