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


/**
********************************************************************************
*
*	@file		GLWidget.inl
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
#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif


//******************************************************************************
//  Defines
//******************************************************************************
#define PREFIX ".."


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//--------------------------
inline GLWidget::~GLWidget()
//--------------------------
{}


//--------------------------------------------
inline QSize GLWidget::minimumSizeHint() const
//--------------------------------------------
{
    return QSize(50, 50);
}


//---------------------------------------------------
inline void GLWidget::resizeGL(int width, int height)
//---------------------------------------------------
{
    // Avoid a division by zero
    if (height == 0)
    {
        // Prevent divide by 0
        height = 1;
    }

    int x(0), y(0), w(width), h(height);

    // Update the viewport
    glViewport(x, y, w, h);

    m_text_2D_projection_matrix = buildOrthoProjectionMatrix(0,
            width,
            height,
            0,
            -1.0,
            1.0);
}


//------------------------------------------------------------
inline void GLWidget::mousePressEvent(QMouseEvent *eventPress)
//------------------------------------------------------------
{
    // Use the arc ball
    m_use_arc_ball = true;

    // Get the mouse position
    m_last_x_position = eventPress->x();
    m_last_y_position = eventPress->y();
}


//----------------------------------------------------------------
inline void GLWidget::mouseReleaseEvent(QMouseEvent *releaseEvent)
//----------------------------------------------------------------
{
    // Stop using the arc ball
    m_use_arc_ball = false;
}


//-------------------------------------------------------
inline void GLWidget::wheelEvent(QWheelEvent *wheelEvent)
//-------------------------------------------------------
{
    // Scrolling along the Y-axis
    if (fabs(wheelEvent->delta()) > EPSILON)
    {
        // Stop using the arc ball
        m_use_arc_ball = false;

        // Change the zoom
        m_zoom += 0.1 * wheelEvent->delta() * cm;

        // Update the projection matrix
        resizeGL(width(), height());
    }

}


//----------------------------------------------------------
inline void GLWidget::mouseMoveEvent(QMouseEvent *eventMove)
//----------------------------------------------------------
{
    // Update the rotation matrix
    computeRotation(m_scene_rotation_matrix, eventMove->x(), eventMove->y());
}


//-----------------------------
inline void GLWidget::animate()
//-----------------------------
{
    QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
}


//----------------------------------
inline void GLWidget::initFreeType()
//----------------------------------
{
#ifdef HAS_FREETYPE
    m_font_rendered.initialise(PREFIX"/data/arial.ttf", 16*27);
    m_display_help = true;
#endif
}


//---------------------------------------
inline void GLWidget::loadXRaySimulator()
//---------------------------------------
{
    // Initialise the X-ray renderer
    m_xray_renderer.initialise(XRayRenderer::OPENGL,
            GL_RGB16F,
            &m_xray_detector,
            &m_xray_beam);
}


//----------------------------------
inline void GLWidget::loadSTLFiles()
//----------------------------------
{
    std::cout << "Load P4" << std::endl;
    loadPatient(P4);
    std::cout << "Load P5" << std::endl;
    loadPatient(P5);
}


//---------------------------
inline void GLWidget::reset()
//---------------------------
{
    m_next_angle_to_process = 0;
    m_current_angle = 0;

    if (m_acquisition_type == ROTATE_SOURCE_DETECTOR)
    {
        m_acquisition_type = ROTATE_SAMPLE;
    }
    else
    {
        m_acquisition_type = ROTATE_SOURCE_DETECTOR;
    }
}


//-----------------------------------------------------------
inline Vec3<GLfloat> GLWidget::getArcballVector(int x, int y)
//-----------------------------------------------------------
{
	Vec3<GLfloat> P(2.0 * float(x) / float(width()) - 1.0,
			2.0 * float(y) / float(height()) - 1.0,
			0);

	P.setY(-P.getY());

	float OP_squared = P.getX() * P.getX() + P.getY() * P.getY();
	if (OP_squared <= 1.0)
	{
		P.setZ(sqrt(1.0 - OP_squared));  // Pythagore
	}
	else
	{
		P.normalise();  // nearest point
	}

	return (P);
}


} // namespace gVirtualXRay
