/**
********************************************************************************
*
*	@file		Types.h
*
*	@brief		Type declarations.
*
*	@version	1.0
*
*	@date		08/11/2013
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


#ifndef GVXR_TYPES_H
#define GVXR_TYPES_H


//******************************************************************************
//	Include
//******************************************************************************
#include <limits>

#ifndef GVXR_VEC3_H
#include "gVirtualXRay/Vec3.h"
#endif

#ifndef GVXR_VEC2_H
#include "gVirtualXRay/Vec2.h"
#endif

#ifndef GVXR_MATRIX4X4_H
#include "gVirtualXRay/Matrix4x4.h"
#endif


//******************************************************************************
//	define
//******************************************************************************


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//******************************************************************************
//	defines
//******************************************************************************
/// Biggest value that can be stored with a real number
#define RATIONAL_NUMBER_MAX   FLT_MAX


/// Smallest value that can be stored with a real number
#define EPSILON               0.0000001


//******************************************************************************
//	type declaration
//******************************************************************************
/// Type of data used to store sinograms
typedef float                    SinogramType;


/// Type of data used to store real numbers
typedef float                    RATIONAL_NUMBER;


/// Type of data used to store 2D vectors
typedef Vec2<RATIONAL_NUMBER>      VEC2;


/// Type of data used to store 3D vectors
typedef Vec3<RATIONAL_NUMBER>      VEC3;


/// Type of data used to store 4x4 matrices
typedef Matrix4x4<RATIONAL_NUMBER> MATRIX4;


//******************************************************************************
//	class declaration
//******************************************************************************
//==============================================================================
/**
 *	@typedef Vec2d
 *	@brief	Vec2d is a class to handle a 2D vector using double precision
 *				floating point numbers.
 */
//==============================================================================
typedef Vec2<double> Vec2d;


//==============================================================================
/**
 *	@typedef Vec2f
 *	@brief	Vec2f is a class to handle a 2D vector using double precision
 *				floating point numbers.
 */
//==============================================================================
typedef Vec2<float> Vec2f;


//==============================================================================
/**
 *	@typedef Vec2i
 *	@brief	Vec2i is a class to handle a 2D vector using 32 bits integers.
 */
//==============================================================================
typedef Vec2<int> Vec2i;


//==============================================================================
/**
 *	@typedef Vec2ui
 *	@brief	Vec2ui is a class to handle a 2D vector using
 *				unsigned 32 bits integers.
 */
//==============================================================================
typedef Vec2<unsigned int> Vec2ui;


//==============================================================================
/**
 *	@typedef Vec3d
 *	@brief	Vec3d is a class to handle a 3D vector using double precision
 *				floating point numbers.
 */
//==============================================================================
typedef Vec3<double> Vec3d;


//==============================================================================
/**
 *	@typedef Vec3f
 *	@brief	Vec3f is a class to handle a 3D vector using double precision
 *				floating point numbers.
 */
//==============================================================================
typedef Vec3<float> Vec3f;


//==============================================================================
/**
 *	@typedef Vec3i
 *	@brief	Vec3i is a class to handle a 3D vector using
 *				signed 32 bits integers.
 */
//==============================================================================
typedef Vec3<int> Vec3i;


//==============================================================================
/**
 *	@typedef Vec3ui
 *	@brief	Vec3ui is a class to handle a 3D vector using
 *				unsigned 32 bits integers.
 */
//==============================================================================
typedef Vec3<unsigned int> Vec3ui;


//==============================================================================
/**
 *	@typedef Matrix4x4d
 *	@brief	Matrix4x4d is a class to handle a 4x4 matrix
 *				using double precision floating point numbers.
 */
//==============================================================================
typedef Matrix4x4<double> Matrix4x4d;


//==============================================================================
/**
 *	@typedef Matrix4x4f
 *	@brief	Matrix4x4f is a class to handle a 4x4 matrix
 *				using single precision floating point numbers.
 */
//==============================================================================
typedef Matrix4x4<float> Matrix4x4f;


//******************************************************************************
//	function declaration
//******************************************************************************


} // namespace gVirtualXRay


#endif // TYPES_H
