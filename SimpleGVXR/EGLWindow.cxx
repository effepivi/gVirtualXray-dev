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
*   @file       EGLWindow.cxx
*
*   @brief      EGLWindow is a class to handle an OpenGL context create
*   			using EGL.
*
*   @version    1.0
*
*   @date       08/09/2020
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
*   http://www.fpvidal.net/, Sept 2020, 2020, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <iostream>

#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

// To initialise GLEW, to define matrix stacks, to define state stacks, etc.
#ifndef __OpenGLUtilities_h
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef __EGLWindow_h
#include "EGLWindow.h"
#endif


//******************************************************************************
//  Namespaces
//******************************************************************************
using namespace std;


//------------------------------------------------
EGLWindow::EGLWindow(int anOpenGLMajorVersion,
                       int anOpenGLMinorVersion):
//------------------------------------------------
    Window(false),
    m_opengl_majour_version(anOpenGLMajorVersion),
    m_opengl_minor_version(anOpenGLMinorVersion),
#ifdef HAS_EGL
	m_display(0),
	m_config(0),
	m_context(0),
	m_surface(0),
#endif // HAS_EGL
	m_ready(false)
//------------------------------------------------
{
    // Create the window
    createWindow(false);

    // Window has not been created
    if (!m_ready)
    {
        // Display a warning
        cerr << "gvxrWarning:\tEGLWindow " <<
            " cannot be created." <<
            endl;
    }
}


//---------------------
EGLWindow::~EGLWindow()
//---------------------
{
	eglDestroyContext(m_display, m_context);
	eglTerminate(m_display);
}


//-----------------------------------------------------
void EGLWindow::setWindowSize(int aWidth, int aHeight)
//-----------------------------------------------------
{
	// Do nothing
}

//----------------------------------------------------
void EGLWindow::displayScene(bool aSceneRotationFlag)
//----------------------------------------------------
{
	// Do nothing
}


//---------------------------------------------------
void EGLWindow::createWindow(bool aVisibilityFlag)
//---------------------------------------------------
{
#ifdef HAS_EGL
	if (!m_ready)
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
		m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		if(m_display == EGL_NO_DISPLAY)
			{
			m_ready = false;
			return;
			}

		// Initialise EGL
		EGLint m_major_ver;
		EGLint m_minor_ver;
		if(eglInitialize(m_display, &m_major_ver, &m_minor_ver) == EGL_FALSE)
			{
			cerr << "ERROR: Failed to eglInitialize" << endl;
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
			m_ready = false;
			return;
			}
		cout << "EGL version: " << m_major_ver << "." << m_minor_ver << endl;

		// Choose the config supporting the attributes set above
		EGLint    num_config;
		if(eglChooseConfig(m_display, attrib_list, &m_config, 1, &num_config) == EGL_FALSE)
			{
			cerr << "Failed to eglChooseConfig" << endl;
			m_ready = false;
			return;
			}

		if(num_config < 1)
			{
			cerr << "No matching EGL frame buffer configuration" << endl;
			m_ready = false;
			return;
			}

		// Bind the OpenGL API to the context
		if(eglBindAPI(EGL_OPENGL_API) == EGL_FALSE)
			{
			cerr << "Failed to eglBindAPI(EGL_OPENGL_API)" << endl;
			m_ready = false;
			return;
			}

		// OpenGL context
		const EGLint context_attrib[] =
			{
			EGL_CONTEXT_MAJOR_VERSION_KHR,  m_opengl_majour_version,
			EGL_CONTEXT_MINOR_VERSION_KHR,  m_opengl_minor_version,
			EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR,    EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR,
			EGL_CONTEXT_FLAGS_KHR,          EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT_KHR
#ifndef NDEBUG
			| EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR
#endif
			,
			EGL_NONE
			};

		// Create the context
		m_context = eglCreateContext(m_display, m_config, EGL_NO_CONTEXT, context_attrib);
		if(m_context == EGL_NO_CONTEXT)
			{
			m_context = eglCreateContext(m_display, m_config, EGL_NO_CONTEXT, nullptr);
			}

		// The context was not created
		if(m_context == EGL_NO_CONTEXT)
		{
			cerr << "Failed to eglCreateContext" << endl;
			switch(eglGetError())
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

			m_ready = false;
			return;
		}

		// Create the surface
		m_surface = eglCreatePbufferSurface(m_display, m_config, nullptr);
		if (!m_surface)
			{
			cerr << "Error: eglCreatePbufferSurface failed Error" << endl;
			return;
			}

		// Make current
		if(eglMakeCurrent(m_display, m_surface, m_surface, m_context) == EGL_FALSE)
			{
			cerr << "Failed to eglMakeCurrent" << endl;
			m_ready = false;
			return;
			}

		// Initialise GLEW
		gVirtualXRay::initialiseGLEW();

		// Still no error?
		if(EGL_SUCCESS != eglGetError())
			{
			m_ready = false;
			return;
			}

		cout << "OpenGL version supported by this platform " << glGetString(GL_VERSION) << endl;

		m_ready = true;
		}

#endif // HAS_EGL
}


//---------------------------
void EGLWindow::initShader()
//---------------------------
{
	// Do nothing
}


//----------------------------------------------
void EGLWindow::render(bool aSceneRotationFlag)
//----------------------------------------------
{
	// Do nothing
}
