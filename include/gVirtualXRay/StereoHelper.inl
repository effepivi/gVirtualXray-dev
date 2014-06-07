/**
********************************************************************************
*
*	@file		StereoHelper.inl
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
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//----------------------------------------------------------------
inline void StereoHelper::setIntraocularDistance(double aDistance)
//----------------------------------------------------------------
{
	m_intraocular_distance = aDistance;
	m_matrices_are_up_to_date = false;
}


//--------------------------------------------------------
inline double StereoHelper::getIntraocularDistance() const
//--------------------------------------------------------
{
	return (m_intraocular_distance);
}


//-----------------------------------------------------------
inline void StereoHelper::setScreenAspectRatio(double aRatio)
//-----------------------------------------------------------
{
	m_screen_aspect_ratio = aRatio;
	m_matrices_are_up_to_date = false;
}


//-----------------------------------------------------
inline void StereoHelper::setFieldOfViewY(double aFOVY)
//-----------------------------------------------------
{
	m_fovy = aFOVY;
	m_matrices_are_up_to_date = false;
}


//----------------------------------------------
inline void StereoHelper::setNear(double aPlane)
//----------------------------------------------
{
	m_near = aPlane;
	m_matrices_are_up_to_date = false;
}


//---------------------------------------------
inline void StereoHelper::setFar(double aPlane)
//---------------------------------------------
{
	m_far = aPlane;
	m_matrices_are_up_to_date = false;
}


//---------------------------------------------------------------
inline void StereoHelper::setScreenProjectionPlane(double aPlane)
//---------------------------------------------------------------
{
	m_screen_projection_plane = aPlane;
	m_matrices_are_up_to_date = false;
}


//--------------------------------------------------------------
inline const MATRIX4& StereoHelper::getLeftEyeProjectionMatrix()
//--------------------------------------------------------------
{
	if (!m_matrices_are_up_to_date)
	{
		update();
	}

	return (m_left_eye_projection_matrix);
}


//---------------------------------------------------------------
inline const MATRIX4& StereoHelper::getRightEyeProjectionMatrix()
//---------------------------------------------------------------
{
	if (!m_matrices_are_up_to_date)
	{
		update();
	}

	return (m_right_eye_projection_matrix);
}


//----------------------------------------
inline bool StereoHelper::isActive() const
//----------------------------------------
{
	return (m_stereo_currently_in_use);
}


} // namespace gVirtualXRay
