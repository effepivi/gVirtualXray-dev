/*

Copyright (c) 2016, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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

#ifndef GVXR_GL_WIDGET_H
#define GVXR_GL_WIDGET_H

/**
********************************************************************************
*
*	@file		GLWidget.h
*
*	@brief      Qt4 widget to handle our OpenGL window.
*
*	@version	1.0
*
*	@date		13/07/2016
*
*	@author		Dr Franck P. Vidal
*
*	@section	License
*				BSD 3-Clause License.
*
*				For details on use and redistribution please refer
*				to http://opensource.org/licenses/BSD-3-Clause
*
*	@section	Copyright
*				(c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*				http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
*				BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <FL/Fl.H>           // Header for FLTK
#include <FL/gl.h>           // Header for OpenGL via FLTK
#include <FL/Fl_Gl_Window.H> // Header to create an OpenGL window using FLTK

#ifndef GVXR_VEC3_H
#include "gVirtualXRay/Vec3.h"
#endif

#ifndef GVXR_MATRIX4X4_H
#include "gVirtualXRay/Matrix4x4.h"
#endif

#ifndef GVXR_SHADER_H
#include "gVirtualXRay/Shader.h"
#endif

#ifndef DEPTH_MAP_H
#include "gVirtualXRay/DepthMap.h"
#endif

#include "gVirtualXRay/LungsMesh.h"


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//******************************************************************************
//  Constant variables
//******************************************************************************
const GLsizei g_default_window_width( 600);
const GLsizei g_default_window_height(600);


//==============================================================================
/**
*   @class  GLWidget
*   @brief  GLWidget is a class to manage a Qt4 widget that handles our OpenGL window.
*/
//==============================================================================
class GLWidget: public Fl_Gl_Window
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    /*
    *   @param W:   the width of the widget
    *   @param H:   the height of the widget
    *   @param L:   the label of the widget
    */
    //--------------------------------------------------------------------------
    GLWidget(int W, int H, const char* L = 0);


    //--------------------------------------------------------------------------
    /// Default Constructor
    /*
    *   @param X:   the position along the horizontal axis
    *   @param Y:   the position along the vertical axis
    *   @param W:   the width of the widget
    *   @param H:   the height of the widget
    *   @param L:   the label of the widget
    */
    //--------------------------------------------------------------------------
    GLWidget(int X, int Y, int W, int H, const char* L = 0);


    //--------------------------------------------------------------------------
    /// Destructor
    //--------------------------------------------------------------------------
    virtual ~GLWidget();


//*****************************************************************************
private:
    //--------------------------------------------------------------------------
    /// Initialise OpenGL
    //--------------------------------------------------------------------------
    virtual void initializeGL();


    //--------------------------------------------------------------------------
    /// Callback used when an event happens
    /*
    *   @param apEvent:   the event that generated the call
    */
    //--------------------------------------------------------------------------
    virtual int handle(int anEvent);


    //--------------------------------------------------------------------------
    /// Display the 3D scene using OpenGL
    //--------------------------------------------------------------------------
    virtual void draw();


    //--------------------------------------------------------------------------
    /// Load STL files (lung geometrical data)
    //--------------------------------------------------------------------------
    virtual void loadSTLFile();


    //--------------------------------------------------------------------------
    /// Render the 3D scene using OpenGL
    //--------------------------------------------------------------------------
    virtual void render();


    /// Transformation matrix used to rotate the 3D scene
    Matrix4x4<GLfloat> m_scene_rotation_matrix;


    /// model matrix of the lung
    Matrix4x4<GLfloat> m_lung_rotation_matrix;


    /// GLSL shader
    Shader m_display_shader;


    /// polygon mesh for the lung
    LungsMesh m_lung_mesh;


    ///the light direction (from the vertex to the camera)
    VEC3 m_light_direction;


    ///object containing buffers to create the shadow map
    DepthMap m_shadow_map;


    /// Flag set to false once OpenGL has been initialised
    bool m_is_first_time;
};


} // namespace gVirtualXRay

#endif // GVXR_GL_WIDGET_H
