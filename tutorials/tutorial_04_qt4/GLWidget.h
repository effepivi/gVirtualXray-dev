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

#include <QtGui>

#include <QGLWidget>

#ifndef GVXR_VEC3_H
#include "gVirtualXRay/Vec3.h"
#endif

#ifndef GVXR_MATRIX4X4_H
#include "gVirtualXRay/Matrix4x4.h"
#endif

#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef GVXR_ATOMIC_ELEMENT_H
#include "gVirtualXRay/AtomicElement.h"
#endif

#ifndef GVXR_XRAY_BEAM_H
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef GVXR_XRAY_DETECTOR_H
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef GVXR_SHADER_H
#include "gVirtualXRay/Shader.h"
#endif

#ifndef GVXR_TEXT_RENDERER_H
#include "gVirtualXRay/TextRenderer.h"
#endif

#ifndef ANATOMICAL_MESH_H
#include "LungMesh.h"
#endif


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
class GLWidget: public QGLWidget
//------------------------------------------------------------------------------
{
//******************************************************************************
private:
    /// New type defining the type of CT acquisition
    typedef enum
    {
        /// The sample is rotating, the couple X-ray source/detector is static
        ROTATE_SAMPLE,

        /// The sample is static, the couple X-ray source/detector is rotating
        ROTATE_SOURCE_DETECTOR
    } AcquisitionType;


//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    /*
    *   @param apPparent:   the parent widget
    */
    //--------------------------------------------------------------------------
    GLWidget(QWidget* apPparent = 0);


    //--------------------------------------------------------------------------
    /// Default Constructor
    /*
    *   @param aGLFormat:   the OpenGL context
    *   @param apPparent:   the parent widget
    */
    //--------------------------------------------------------------------------
    GLWidget(QGLFormat& aGLFormat, QWidget *apPparent = 0);


    //--------------------------------------------------------------------------
    /// Destructor
    //--------------------------------------------------------------------------
    virtual ~GLWidget();


//*****************************************************************************
private:
    //--------------------------------------------------------------------------
    /// Set the minimum window size
    //--------------------------------------------------------------------------
    virtual QSize minimumSizeHint() const;


    //--------------------------------------------------------------------------
    /// Set the default window size
    //--------------------------------------------------------------------------
    virtual QSize sizeHint() const;


    //--------------------------------------------------------------------------
    /// Initialise OpenGL
    //--------------------------------------------------------------------------
    virtual void initializeGL();


    //--------------------------------------------------------------------------
    /// Display the 3D scene using OpenGL
    //--------------------------------------------------------------------------
    virtual void paintGL();

    //--------------------------------------------------------------------------
    /// Initialise OpenGL
    //--------------------------------------------------------------------------
    virtual void initGL();

    //--------------------------------------------------------------------------
    /// Load STL files (lung geometrical data)
    //--------------------------------------------------------------------------
    virtual void loadSTLFiles();

    //--------------------------------------------------------------------------
    /// Activate the properties of the mesh (geometry and Hounsfield values)
    //--------------------------------------------------------------------------
    virtual void loadLung();


    //--------------------------------------------------------------------------
    /// Render the 3D scene using OpenGL
    //--------------------------------------------------------------------------
    virtual void render();


    /// The zooming factor
    GLfloat m_zoom;

    /// Transformation matrix used to rotate the 3D scene
    Matrix4x4<GLfloat> m_scene_rotation_matrix;

    /// Transformation matrix used to rotate the lung
    Matrix4x4<GLfloat> m_lung_rotation_matrix;

    /// Translation vector used to centre the lung
    VEC3 m_lung_translation_vector;

    /// GLSL shader
    Shader m_display_shader;

    /// polygon mesh for the lung
    LungMesh m_lung_mesh;

    /// Flag set to true when the meshes are displayed using wireframe, false otherwise
    bool m_display_wireframe;

    /// The angular step between two successive rotations
    RATIONAL_NUMBER m_angle_step;

    /// The next angle to use
    float m_next_angle_to_process;

    /// The current angle
    float m_current_angle;

    /// The total simulation time
    float m_simulation_total_time;

    /// The number of rotations that have been performed
    unsigned int m_number_of_rotations;

    /// The current acquisition type
    GLWidget::AcquisitionType m_acquisition_type;

    ///the light direction (from the vertex to the camera
    VEC3 m_light_direction;
};

} // namespace gVirtualXRay


//******************************************************************************
#include "GLWidget.inl"


#endif // GVXR_GL_WIDGET_H
