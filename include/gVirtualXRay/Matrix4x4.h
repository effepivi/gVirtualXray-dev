/**
********************************************************************************
*
*	@file		Matrix4x4.h
*
*	@brief		Template class to handle a 4x4 matrixs.
*
*	@version	1.0
*
*	@date		11/11/2013
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


#ifndef GVXR_MATRIX4X4_H
#define GVXR_MATRIX4X4_H


//******************************************************************************
//	Include
//******************************************************************************
#ifndef GVXR_VEC3_H
#include "gVirtualXRay/Vec3.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//******************************************************************************
//	class declaration
//******************************************************************************
template<typename T> class Matrix4x4;


//******************************************************************************
//	function declaration
//******************************************************************************
template <typename T> std::ostream& operator<<(std::ostream& anOutputStream,
		const Matrix4x4<T>& aMatrix);


//==============================================================================
/**
*	@class	Matrix4x4
*	@brief	Matrix4x4 is a template class to handle a 4 by 4 matrix.
*/
//==============================================================================
template<typename T> class Matrix4x4
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	//--------------------------------------------------------------------------
	///	Default Constructor (create an identity matrix).
	/**
	*	@param a1:	the  1st element of the matrix
	*	@param a2:	the  2nd element of the matrix
	*	@param a3:	the  3rd element of the matrix
	*	@param a4:	the  4th element of the matrix
	*	@param a5:	the  5th element of the matrix
	*	@param a6:	the  6th element of the matrix
	*	@param a7:	the  7th element of the matrix
	*	@param a8:	the  8th element of the matrix
	*	@param a9:	the  9th element of the matrix
	*	@param a10:	the 10th element of the matrix
	*	@param a11:	the 11th element of the matrix
	*	@param a12:	the 12th element of the matrix
	*	@param a13:	the 13th element of the matrix
	*	@param a14:	the 14th element of the matrix
	*	@param a15:	the 15th element of the matrix
	*	@param a16:	the 16th element of the matrix
	*/
	//--------------------------------------------------------------------------
	Matrix4x4(const T& a1 = 1.0f, const T& a2 = 0.0f, const T& a3 = 0.0f, const T& a4 = 0.0f,
		const T& a5 = 0.0f, const T& a6 = 1.0f, const T& a7 = 0.0f, const T& a8 = 0.0f,
		const T& a9 = 0.0f, const T& a10 = 0.0f, const T& a11 = 1.0f, const T& a12 = 0.0f,
		const T& a13 = 0.0f, const T& a14 = 0.0f, const T& a15 = 0.0f, const T& a16 = 1.0f);


	//--------------------------------------------------------------------------
	///	Copy Constructor.
	/**
	*	@param aMatrix: the matrix to copy
	*/
	//--------------------------------------------------------------------------
	Matrix4x4(const Matrix4x4& aMatrix);


	//--------------------------------------------------------------------------
	///	Copy Constructor.
	/**
	*	@param apMatrix: the matrix to copy
	*/
	//--------------------------------------------------------------------------
	Matrix4x4(const Matrix4x4* apMatrix);


	//--------------------------------------------------------------------------
	///	Copy Constructor.
	/**
	*	@param apMatrix: the matrix to copy
	*/
	//--------------------------------------------------------------------------
	Matrix4x4(const T* apMatrix);


	//--------------------------------------------------------------------------
	///	Load identity matrix.
	//--------------------------------------------------------------------------
	void loadIdentity();


	//--------------------------------------------------------------------------
	///	Multiply the current matrix bya rotation matrix.
	/**
	*	@param anAngle: the angle of rotation in degrees
	*	@param aVector: the axis of rotation
	*/
	//--------------------------------------------------------------------------
	void rotate(const double anAngle, const Vec3<T>& aVector);


	//--------------------------------------------------------------------------
	///	Multiply the current matrix by a translation matrix.
	/**
	*	@param aVector: the translation vector
	*/
	//--------------------------------------------------------------------------
	void translate(const Vec3<T>& aVector);


	//--------------------------------------------------------------------------
	///	Multiply the current matrix by a translation matrix.
	/**
	*	@param x: the x-component of the translation vector
	*	@param y: the y-component of the translation vector
	*	@param z: the z-component of the translation vector
	*/
	//--------------------------------------------------------------------------
	void translate(T x, T y, T z);


	//--------------------------------------------------------------------------
	///	Build a tranlation matrix.
	/**
	*	@param aVector: the translation vector
	*	@return the corresponding translation matrix
	*/
	//--------------------------------------------------------------------------
	static Matrix4x4 buildTranslationMatrix(const Vec3<T>& aVector);


	//--------------------------------------------------------------------------
	///	Build a rotation matrix.
	/**
	*	@param anAngle: the angle of rotation in degrees
	*	@param x: the x-component of the axis of rotation
	*	@param y: the y-component of the axis of rotation
	*	@param z: the z-component of the axis of rotation
	*	@return the corresponding rotation matrix
	*/
	//--------------------------------------------------------------------------
	static Matrix4x4 buildRotationMatrix(T anAngle, T x, T y, T z);


	//--------------------------------------------------------------------------
	///	Build a rotation matrix.
	/**
	*	@param anAngle: the angle of rotation in degrees
	*	@param aVector: the axis of rotation
	*	@return the corresponding translation matrix
	*/
	//--------------------------------------------------------------------------
	static Matrix4x4 buildRotationMatrix(T anAngle, const Vec3<T>& aVector);


	//--------------------------------------------------------------------------
	///	Build a rotation matrix.
	/**
	*	@param x: the x-component of the translation vector
	*	@param y: the y-component of the translation vector
	*	@param z: the z-component of the translation vector
	*	@return the corresponding translation matrix
	*/
	//--------------------------------------------------------------------------
	static Matrix4x4 buildTranslationMatrix(T x, T y, T z);


	//--------------------------------------------------------------------------
	///	Multiply the current matrix by a scaling matrix.
	/**
	*	@param x: the scaling factor along the X-axis
	*	@param y: the scaling factor along the Y-axis
	*	@param z: the scaling factor along the Z-axis
	*/
	//--------------------------------------------------------------------------
	void scale(T x, T y, T z);


	//--------------------------------------------------------------------------
	///	Get the transpose of the current matrix.
	/**
	*	@return the transpose matrix
	*/
	//--------------------------------------------------------------------------
	Matrix4x4 getTranspose() const;


	//--------------------------------------------------------------------------
	///	Compute the determinant of the current matrix.
	/**
	*	@return the determinant
	*/
	//--------------------------------------------------------------------------
	T determinant() const;


	//--------------------------------------------------------------------------
	///	Get the inverse of the current matrix.
	/**
	*	@return the inverse matrix
	*/
	//--------------------------------------------------------------------------
	Matrix4x4 getInverse() const;


	//--------------------------------------------------------------------------
	///	Copy operator.
	/**
	*	@param aMatrix: the matrix to copy
	*	@return the copied matrix
	*/
	//--------------------------------------------------------------------------
	Matrix4x4& operator=(const Matrix4x4& aMatrix);


	//--------------------------------------------------------------------------
	///	Accessor on the 4x4 matrix data.
	/**
	*	@return the matrix raw data
	*/
	//--------------------------------------------------------------------------
	const T* get() const;


	//--------------------------------------------------------------------------
	///	Accessor on the 4x4 matrix data.
	/**
	*	@return the matrix raw data
	*/
	//--------------------------------------------------------------------------
	const T* get4x4() const;


	//--------------------------------------------------------------------------
	///	Accessor on the 3x3 matrix data.
	/**
	*	@return the matrix raw data
	*/
	//--------------------------------------------------------------------------
	const T* get3x3();


	//--------------------------------------------------------------------------
	///	Operator * to multiply the current matrix by a vector.
	/**
	*	@param aVector: the vector to transform
	*	@return the transformed vector
	*/
	//--------------------------------------------------------------------------
	Vec3<T> operator*(const Vec3<T>& aVector) const;
	

	//--------------------------------------------------------------------------
	///	Operator * to multiply the current matrix by another matrix.
	/**
	*	@param aMatrix: the other matrix
	*	@return the transformed matrix
	*/
	//--------------------------------------------------------------------------
	Matrix4x4 operator*(const Matrix4x4& aMatrix) const;


	//--------------------------------------------------------------------------
	///	Operator *= to multiply the current matrix by another matrix,
	///	and modify the current matrix.
	/**
	*	@param aMatrix: the other matrix
	*	@return the transformed matrix
	*/
	//--------------------------------------------------------------------------
	Matrix4x4& operator*=(const Matrix4x4& aMatrix);
	

	//--------------------------------------------------------------------------
	///	Print the matrix in the console.
	//--------------------------------------------------------------------------
	void print() const;


	//--------------------------------------------------------------------------
	///	Print the matrix in the console.
	/**
	*	@param anOutputStream: the stream in which the matrix will be printed
	*/
	//--------------------------------------------------------------------------
	void print(std::ostream& anOutputStream) const;


//******************************************************************************
protected:
	/// the matrix data
	T m_p_4x4_data[4 * 4];


	/// the matrix data
	T m_p_3x3_data[3 * 3];


	/// A flag to check if the 3x3 matrix is up-to-date.
	/// It is used for lazy evaluation.
	bool m_small_matrix_up_to_date;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "Matrix4x4.inl"


#endif // MATRIX4X4_H

