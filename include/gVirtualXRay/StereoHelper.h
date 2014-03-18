/**
********************************************************************************
*
*	@file		StereoHelper.h
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


#ifndef STEREO_HELPER_H
#define STEREO_HELPER_H


//******************************************************************************
//	Include
//******************************************************************************
#ifndef MATRIX4X4_H
#include "gVirtualXRay/Matrix4x4.h"
#endif

#ifndef TYPES_H
#include "gVirtualXRay/Types.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace Graphics {
using namespace Utilities;


//==============================================================================
/**
*	@class	StereoHelper
*	@brief	StereoHelper is a class to handle stereo vision using OpenGL's
*			quad buffer.
*/
//==============================================================================
class StereoHelper
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	//--------------------------------------------------------------------------
	///	Enum defining eyes.
	//--------------------------------------------------------------------------
	typedef enum EyeTypeTag
	{
		LEFT_EYE  = 0, //< Left eye (stereo vision)
		RIGHT_EYE = 1, //< Right eye (stereo vision)
		MONO      = 2  //< Monovision
	} EyeType;


	//--------------------------------------------------------------------------
	///	Default constructor.
	//--------------------------------------------------------------------------
	StereoHelper();
	

	//--------------------------------------------------------------------------
	///	Set the intraocular distance
	/**
	*	@param aDistance: the intraocular distance
	*/
	//--------------------------------------------------------------------------
	void setIntraocularDistance(double aDistance);


	//--------------------------------------------------------------------------
	///	Accessor on the intraocular distance
	/**
	*	@return the intraocular distance
	*/
	//--------------------------------------------------------------------------
	double getIntraocularDistance() const;


	//--------------------------------------------------------------------------
	///	Set the screen aspect ratio
	/**
	*	@param aRatio: the screen aspect ratio
	*/
	//--------------------------------------------------------------------------
	void setScreenAspectRatio(double aRatio);


	//--------------------------------------------------------------------------
	///	Set the field of view along the y-axis
	/**
	*	@param aFOVY: the field of view along the y-axis
	*/
	//--------------------------------------------------------------------------
	void setFieldOfViewY(double aFOVY);


	//--------------------------------------------------------------------------
	///	Set the near clipping plane
	/**
	*	@param aPlane: the near clipping plane
	*/
	//--------------------------------------------------------------------------
	void setNear(double aPlane);


	//--------------------------------------------------------------------------
	///	Set the far clipping plane
	/**
	*	@param aPlane: the far clipping plane
	*/
	//--------------------------------------------------------------------------
	void setFar(double aPlane);


	//--------------------------------------------------------------------------
	///	Set the screen projection plane
	/**
	*	@param aPlane: the screen projection plane
	*/
	//--------------------------------------------------------------------------
	void setScreenProjectionPlane(double aPlane);


	//--------------------------------------------------------------------------
	///	Accessor on the projection matrix corresponding to the left eye.
	/**
	*	@return the projection matrix
	*/
	//--------------------------------------------------------------------------
	const MATRIX4& getLeftEyeProjectionMatrix();


	//--------------------------------------------------------------------------
	///	Accessor on the projection matrix corresponding to the right eye.
	/**
	*	@return the projection matrix
	*/
	//--------------------------------------------------------------------------
	const MATRIX4& getRightEyeProjectionMatrix();


	//--------------------------------------------------------------------------
	///	Swap eye.
	//--------------------------------------------------------------------------
	void swapEye();


	//--------------------------------------------------------------------------
	///	Enable the stereo if possible.
	//--------------------------------------------------------------------------
	bool enable();


	//--------------------------------------------------------------------------
	///	Disable the stereo.
	//--------------------------------------------------------------------------
	void disable();


	//--------------------------------------------------------------------------
	///	Accessor on the stereo flag.
	//--------------------------------------------------------------------------
	bool isActive() const;


//******************************************************************************
protected:
	/// Compute the projection matrices
	void update();


	/// Intraocular distance
	double m_intraocular_distance;


	/// The projection matrix corresponding to the left eye
	MATRIX4 m_left_eye_projection_matrix;


	/// The projection matrix corresponding to the right eye
	MATRIX4 m_right_eye_projection_matrix;


	/// Frustum of the left eye
	/// m_left_eye_frustum[0] corresponds to the left clipping plane
	/// m_left_eye_frustum[1] corresponds to the right clipping plane
	/// m_left_eye_frustum[2] corresponds to the bottom clipping plane
	/// m_left_eye_frustum[3] corresponds to the top clipping plane
	double m_left_eye_frustum[4];


	/// Frustum of the right eye
	/// m_right_eye_frustum[0] corresponds to the left clipping plane
	/// m_right_eye_frustum[1] corresponds to the right clipping plane
	/// m_right_eye_frustum[2] corresponds to the bottom clipping plane
	/// m_right_eye_frustum[3] corresponds to the top clipping plane
	double m_right_eye_frustum[4];


	/// Near clipping plane
	double m_near;


	/// Far clipping plane
	double m_far;


	/// Field of view along the y-axis
	double m_fovy;


	/// Screen aspect ratio
	double m_screen_aspect_ratio;


	/// Screen projection plane
	double m_screen_projection_plane;


	/// Flag set to true when the matrices are up-to-date
	bool m_matrices_are_up_to_date;


	/// Current eye type
	EyeType m_current_eye;


	/// Flag set to true when the stereo is in used
	bool m_stereo_currently_in_use;
};


} // namespace Graphics


//******************************************************************************
#include "StereoHelper.inl"


#endif // STEREO_HELPER_H

