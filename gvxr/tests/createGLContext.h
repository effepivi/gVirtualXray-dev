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
*   @file       createGLContext.h
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
#ifndef EGL_NO_X11
#define EGL_NO_X11
#endif

#include <EGL/egl.h>

#ifdef HAS_GBM
class gbm_device;
class gbm_surface;
#endif


struct eglData {
    bool ready;

#ifdef HAS_GBM
    gbm_device* p_device;
    gbm_surface* p_surface;
#endif

    EGLDisplay egl_display;
    EGLConfig egl_config;
    EGLContext egl_context;
    EGLSurface egl_surface;
};


//------------------------------------------------------------------------------
/// Create an OpenGL context using EGL.
/*
*   @param aContext:   the context
*   @param aDisplay:   the display
*/
//------------------------------------------------------------------------------
void createGLContext(eglData& aWholeContextData);


//------------------------------------------------------------------------------
/// Destroy an OpenGL context using EGL.
/*
*   @param aContext:   the context
*   @param aDisplay:   the display
*/
//------------------------------------------------------------------------------
void destroyGLContext(eglData& aWholeContextData);
