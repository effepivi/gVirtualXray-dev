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

//  Defines
//******************************************************************************
#define PREFIX ".."


//---------------------------------------------------------
inline unsigned int AnatomicalDataHandler::getWidth() const
//---------------------------------------------------------
{
    return (m_width);
}


//----------------------------------------------------------
inline unsigned int AnatomicalDataHandler::getHeight() const
//----------------------------------------------------------
{
    return (m_height);
}


/*
//--------------------------------------------
inline QSize AnatomicalDataHandler::minimumSizeHint() const
//--------------------------------------------
{
    return QSize(50, 50);
}


//------------------------------------------------------------
inline void AnatomicalDataHandler::mousePressEvent(QMouseEvent *eventPress)
//------------------------------------------------------------
{
    // Use the arc ball
    m_use_arc_ball = true;

    // Get the mouse position
    m_last_x_position = eventPress->x();
    m_last_y_position = eventPress->y();
}


//----------------------------------------------------------------
inline void AnatomicalDataHandler::mouseReleaseEvent(QMouseEvent *releaseEvent)
//----------------------------------------------------------------
{
    // Stop using the arc ball
    m_use_arc_ball = false;
}


//----------------------------------------------------------
inline void AnatomicalDataHandler::mouseMoveEvent(QMouseEvent *eventMove)
//----------------------------------------------------------
{
    // Update the rotation matrix
    computeRotation(m_scene_rotation_matrix, eventMove->x(), eventMove->y());
}


//-----------------------------
inline void AnatomicalDataHandler::animate()
//-----------------------------
{
    QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
}
*/

//----------------------------------
inline void AnatomicalDataHandler::loadSTLFiles()
//----------------------------------
{
    std::cout << "Load P4" << std::endl;
    loadPatient(P4);
    std::cout << "Load P5" << std::endl;
    loadPatient(P5);
}


//---------------------------
inline void AnatomicalDataHandler::reset()
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
inline gVirtualXRay::Vec3<GLfloat> AnatomicalDataHandler::getArcballVector(int x, int y)
//-----------------------------------------------------------
{
	gVirtualXRay::Vec3<GLfloat> P(2.0 * float(x) / float(m_width) - 1.0,
			2.0 * float(y) / float(m_height) - 1.0,
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
