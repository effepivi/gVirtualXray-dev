/**
********************************************************************************
*
*	@file		Vec3.h
*
*	@brief		Template class to handle 3D vectors.
*
*	@todo		Add an exception in Length().
*				Add a header for constants, use EPSILON instead of 0.0000001
*
*	@version	1.2
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


#ifndef GVXR_VEC3_H
#define GVXR_VEC3_H


//******************************************************************************
//	Include
//******************************************************************************
#include <iostream>


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//******************************************************************************
//	class declaration
//******************************************************************************
template<typename T> class Vec3;


//******************************************************************************
//	function declaration
//******************************************************************************
template <typename T> std::ostream& operator<<(std::ostream& anOutputStream,
		const Vec3<T>& aVector);


//==============================================================================
/**
*	@class	Vec3
*	@brief	Vec3 is a template class to handle a 3D vector.
*/
//==============================================================================
template<typename T> class Vec3
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	//---------------------------------------------------------------------------
	///	Default Constructor
	/**
	*	@param aX: the position along the x-axis	(default value is 0)
	*	@param aY: the position along the y-axis	(default value is 0)
	*	@param aZ: the position along the z-axis	(default value is 0)
	*/
	//---------------------------------------------------------------------------
	Vec3(const T& aX = 0, const T& aY = 0, const T& aZ = 0);


	//---------------------------------------------------------------------------
	///	Set the position along the x-axis
	/**
	*	@param aValue:	the position along the x-axis
	*/
	//---------------------------------------------------------------------------
	void setX(const T& aValue);


	//---------------------------------------------------------------------------
	///	Set the position along the y-axis
	/**
	*	@param aValue:	the position along the y-axis
	*/
	//---------------------------------------------------------------------------
	void setY(const T& aValue);


	//---------------------------------------------------------------------------
	///	Set the position along the z-axis
	/**
	*	@param aValue:	the position along the z-axis
	*/
	//---------------------------------------------------------------------------
	void setZ(const T& aValue);


	//---------------------------------------------------------------------------
	///	Accessor on the position along the x-axis
	/**
	*	@return the position along the x-axis
	*/
	//---------------------------------------------------------------------------
	T getX() const;


	//---------------------------------------------------------------------------
	///	Accessor on the position along the y-axis
	/**
	*	@return the position along the y-axis
	*/
	//---------------------------------------------------------------------------
	T getY() const;


	//---------------------------------------------------------------------------
	///	Accessor on the position along the z-axis
	/**
	*	@return the position along the z-axis
	*/
	//---------------------------------------------------------------------------
	T getZ() const;


	//---------------------------------------------------------------------------
	///	Get the length of the vector.
	/**
	*	@return the length of the vector
	*/
	//---------------------------------------------------------------------------
	double length() const;
	
	
	//---------------------------------------------------------------------------
	///	Get the unit vector corresponding to the normed current vector.
	/**
	*	@return the unit vector
	*/
	//---------------------------------------------------------------------------
	Vec3 normal() const;


	//---------------------------------------------------------------------------
	///	Normalize the current vector so that its length is 1.
	//---------------------------------------------------------------------------
	void normalize();


	//---------------------------------------------------------------------------
	///	Normalise the current vector so that its length is 1.
	//---------------------------------------------------------------------------
	void normalise();
	

	//---------------------------------------------------------------------------
	///	Get the dot product between the current vector and a given vector.
	/**
	*	@param aVector:	the other vector
	*	@return the dot product
	*/
	//---------------------------------------------------------------------------
	double dotProduct(const Vec3& aVector) const;


	//---------------------------------------------------------------------------
	///	Get the distance between the 3D position represented
	///	by the current vector and another position.
	/**
	*	@param aVector:	the other vector
	*	@return the distance
	*/
	//---------------------------------------------------------------------------
	double distance(const Vec3& aVector) const;


	//---------------------------------------------------------------------------
	///	Operator * to compute the dot product between the current vector and
	///	a given vector.
	/**
	*	@param aVector:	the other vector
	*	@return the dot product
	*/
	//---------------------------------------------------------------------------
	double operator*(const Vec3& aVector) const;


	//---------------------------------------------------------------------------
	///	Operator +
	/**
	*	@param aVector:	the other vector
	*	@return the dot product
	*/
	//---------------------------------------------------------------------------
	const Vec3 operator+(const Vec3& aVector) const;


	//---------------------------------------------------------------------------
	///	Operator +=
	/**
	*	@param aVector:	the other vector
	*	@return the modified vector
	*/
	//---------------------------------------------------------------------------
	Vec3& operator+=(const Vec3& aVector);
	

	//---------------------------------------------------------------------------
	///	Operator -
	/**
	*	@param aVector:	the other vector
	*	@return the modified vector
	*/
	//---------------------------------------------------------------------------
	const Vec3 operator-(const Vec3& aVector) const;


	//---------------------------------------------------------------------------
	///	Operator -=
	/**
	*	@param aVector:	the other vector
	*	@return the modified vector
	*/
	//---------------------------------------------------------------------------
	Vec3& operator-=(const Vec3& aVector);


	//---------------------------------------------------------------------------
	///	Operator * to multiply each component of the vector by a given value.
	/**
	*	@param aValue:	the multiplicative value
	*	@return the modified vector
	*/
	//---------------------------------------------------------------------------
	const Vec3 operator*(const double& aValue) const;


	//---------------------------------------------------------------------------
	///	Operator *= to multiply each component of the vector by a given value,
	///	and modify the current vector.
	/**
	*	@param aValue:	the multiplicative value
	*	@return the modified vector
	*/
	//---------------------------------------------------------------------------
	Vec3& operator*=(const double& aValue);


	//---------------------------------------------------------------------------
	///	Operator * to multiply each component of the vector by a given value.
	/**
	*	@param aValue:	the multiplicative value
	*	@return the modified vector
	*/
	//---------------------------------------------------------------------------
	const Vec3 operator*(const float& aValue) const;


	//---------------------------------------------------------------------------
	///	Operator *= to multiply each component of the vector by a given value,
	///	and modify the current vector.
	/**
	*	@param aValue:	the multiplicative value
	*	@return the modified vector
	*/
	//---------------------------------------------------------------------------
	Vec3& operator*=(const float& aValue);


	//---------------------------------------------------------------------------
	///	Operator / to divide each component of the vector by a given value.
	/**
	*	@param aValue:	the value
	*	@return the modified vector
	*/
	//---------------------------------------------------------------------------
	const Vec3 operator/(const double& aValue) const;


	//---------------------------------------------------------------------------
	///	Operator -=
	/**
	*	@param aValue:	the other vector
	*	@return the modified vector
	*/
	//---------------------------------------------------------------------------
	Vec3& operator/=(const double& aValue);


	//---------------------------------------------------------------------------
	///	Operator / to divide each component of the vector by a given value.
	/**
	*	@param aValue:	the value
	*	@return the modified vector
	*/
	//---------------------------------------------------------------------------
	const Vec3 operator/(const float& aValue) const;


	//---------------------------------------------------------------------------
	///	Operator -=
	/**
	*	@param aValue:	the other vector
	*	@return the modified vector
	*/
	//---------------------------------------------------------------------------
	Vec3& operator/=(const float& aValue);


	//---------------------------------------------------------------------------
	///	Operator ^ to compute the cross product between the current vector and
	///	a given vector.
	/**
	*	@param aVector:	the other vector
	*	@return the cross product
	*/
	//---------------------------------------------------------------------------
	const Vec3 operator^(const Vec3& aVector) const;


	//---------------------------------------------------------------------------
	///	Operator - to get the opposite vector.
	/**
	*	@return the opposite vector
	*/
	//---------------------------------------------------------------------------
	const Vec3 operator-() const;


	//---------------------------------------------------------------------------
	///	Operator ==
	/**
	*	@param aVector:	the vector to compare the current vector with
	*	@return true if the two vectors are the same, false if the two vectors are different
	*/
	//---------------------------------------------------------------------------
	bool operator==(const Vec3& aVector) const;


	//---------------------------------------------------------------------------
	///	Operator !=
	/**
	*	@param aVector:	the vector to compare the current vector with
	*	@return true if the two vectors are different, false if the two vectors are the same
	*/
	//---------------------------------------------------------------------------
	bool operator!=(const Vec3& aVector) const;


//******************************************************************************
protected:
	//**************************************************************************
	T m_x; ///< the position along the x-axis
	T m_y; ///< the position along the y-axi
	T m_z; ///< the position along the z-axi
};


//---------------------------------------------------------------------------
///	Operator * to multiply each component of the vector by a given value.
/**
*	@param aValue:	the multiplicative value
*	@param aVector:	the vector to multiply
*	@return the modified vector
*/
//---------------------------------------------------------------------------
template<typename T> Vec3<T> operator*(const double& aValue, const Vec3<T>& aVector);


} // namespace gVirtualXRay


//******************************************************************************
#include "Vec3.inl"


#endif // VEC3_H
