/**
********************************************************************************
*
*	@file		StereoHelper.cxx
*
*	@brief		Class to handle stereo vision using OpenGL's quad buffer.
*				The code is relying on // http://www.orthostereo.com/geometryopengl.html
*
*	@version	1.0
*
*	@date		14/02/2014
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
*				http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
*				BSD 3-Clause License
*
********************************************************************************
*/

//******************************************************************************
//	Include
//******************************************************************************
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#define GL_GLEXT_PROTOTYPES 1
#include <GL/glcorearb.h>
#endif

#ifndef GVXR_STEREO_HELPER_H
#include "gVirtualXRay/StereoHelper.h"
#endif

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace Graphics;


//---------------------------------------
StereoHelper::StereoHelper():
//---------------------------------------
		m_intraocular_distance(4.5),
		m_near(0.01),
		m_far(5000.0),
		m_fovy(45),
		m_screen_aspect_ratio(1),
		m_screen_projection_plane(10000),
		m_matrices_are_up_to_date(false),
		m_current_eye(MONO),
		m_stereo_currently_in_use(false)
//---------------------------------------
{
	m_left_eye_frustum[0] = 0;
	m_left_eye_frustum[1] = 0;
	m_left_eye_frustum[2] = 0;
	m_left_eye_frustum[3] = 0;

	m_right_eye_frustum[0] = 0;
	m_right_eye_frustum[1] = 0;
	m_right_eye_frustum[2] = 0;
	m_right_eye_frustum[3] = 0;
}


//-------------------------
void StereoHelper::update()
//-------------------------
{
	const double degree_to_radian(PI / 180.0);

	// Sets top of frustum based on fovy and near clipping plane
	double top = m_near * tan(degree_to_radian * m_fovy / 2.0);
	double right = m_screen_aspect_ratio * top;                             //sets right of frustum based on aspect ratio
	double frustum_shift = (m_intraocular_distance / 2.0) * m_near / m_screen_projection_plane;

	m_left_eye_frustum[0] = -right + frustum_shift;
	m_left_eye_frustum[1] =  right + frustum_shift;
	m_left_eye_frustum[2] = -top;
	m_left_eye_frustum[3] =  top;

	double left_camera_translation(m_intraocular_distance / 2.0);

	m_left_eye_projection_matrix = buildFrustumProjectionMatrix(m_left_eye_frustum[0],
			m_left_eye_frustum[1],
			m_left_eye_frustum[2],
			m_left_eye_frustum[3],
			m_near,
			m_far);

	m_left_eye_projection_matrix *= MATRIX4::buildTranslationMatrix(left_camera_translation, 0, 0);

	m_right_eye_frustum[0] = -right - frustum_shift;
	m_right_eye_frustum[1] =  right - frustum_shift;
	m_right_eye_frustum[2] = -top;
	m_right_eye_frustum[3] =  top;

	double right_camera_translation(-left_camera_translation);

	m_right_eye_projection_matrix = buildFrustumProjectionMatrix(m_right_eye_frustum[0],
			m_right_eye_frustum[1],
			m_right_eye_frustum[2],
			m_right_eye_frustum[3],
			m_near,
			m_far);

	m_right_eye_projection_matrix *= MATRIX4::buildTranslationMatrix(right_camera_translation, 0, 0);

	m_matrices_are_up_to_date = true;
}


//--------------------------
void StereoHelper::swapEye()
//--------------------------
{
	// Change the eye
	switch (m_current_eye)
	{
	case LEFT_EYE:
		m_current_eye = RIGHT_EYE;
		break;

	case RIGHT_EYE:
		m_current_eye = LEFT_EYE;
		break;

	default:
		if (m_stereo_currently_in_use)
		{
			m_current_eye = RIGHT_EYE;
		}
		else
		{
			m_current_eye = MONO;
		}
		break;
	}

	// Change the back buffer and the projection matrix
	switch (m_current_eye)
	{
	case LEFT_EYE:
		g_current_projection_matrix = getLeftEyeProjectionMatrix();
		if (m_stereo_currently_in_use)
		{
			glDrawBuffer(GL_BACK_LEFT);
		}
		break;

	case RIGHT_EYE:
		g_current_projection_matrix = getRightEyeProjectionMatrix();
		if (m_stereo_currently_in_use)
		{
			glDrawBuffer(GL_BACK_RIGHT);
		}
		break;

	case MONO:
	default:
		glDrawBuffer(GL_BACK);
		break;
	}
}


//-------------------------
bool StereoHelper::enable()
//-------------------------
{
	// Stereo is disable
	if (!m_stereo_currently_in_use)
	{
		// Check the stereo
		GLboolean can_use_stereo;
		glGetBooleanv(GL_STEREO, &can_use_stereo);

		// It is enable on the system
		if (can_use_stereo)
		{
			// Enable the use of stereo
			m_stereo_currently_in_use = true;
		}
		// It is not enable on the system
		else
		{
			disable();
		}
	}

	return (m_stereo_currently_in_use);
}


//--------------------------
void StereoHelper::disable()
//--------------------------
{
	// Stereo is enable
	if (m_stereo_currently_in_use)
	{
		m_current_eye = MONO;
		m_stereo_currently_in_use = false;
	}
}
