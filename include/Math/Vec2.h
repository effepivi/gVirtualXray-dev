/**
********************************************************************************
*
*	@file		Vec2.h
*
*	@brief		Template class to handle 2D vectors.
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


#ifndef VEC2_H
#define VEC2_H


//******************************************************************************
//	namespace
//******************************************************************************
namespace Math {


//******************************************************************************
//	class declaration
//******************************************************************************
template<typename T> class Vec2;


//******************************************************************************
//	function declaration
//******************************************************************************
template <typename T> std::ostream& operator<<(std::ostream& anOutputStream,
		const Vec2<T>& aVector);


//==============================================================================
/**
*	@class	Vec2
*	@brief	Vec2 is a template class to handle a 2D vector.
*/
//==============================================================================
template<typename T> class Vec2
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	//---------------------------------------------------------------------------
	///	Default Constructor
	/**
	*	@param aX: the position along the x-axis
	*	@param aY: the position along the y-axis
	*/
	//---------------------------------------------------------------------------
	Vec2(const T& aX = 0.0f, const T& aY = 0.0f);


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
	Vec2 normal() const;


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
	double dotProduct(const Vec2& aVector) const;


	//---------------------------------------------------------------------------
	///	Get the distance between the 2D position represented
	///	by the current vector and another position.
	/**
	*	@param aVector:	the other vector
	*	@return the distance
	*/
	//---------------------------------------------------------------------------
	double distance(const Vec2& aVector) const;


	//---------------------------------------------------------------------------
	///	Operator +
	/**
	*	@param aVector:	the other vector
	*	@return the modified vector
	*/
	//---------------------------------------------------------------------------
	const Vec2 operator+(const Vec2& aVector) const;


	//---------------------------------------------------------------------------
	///	Operator +=
	/**
	*	@param aVector:	the other vector
	*	@return the modified vector
	*/
	//---------------------------------------------------------------------------
	Vec2& operator+=(const Vec2& aVector);
	

	//---------------------------------------------------------------------------
	///	Operator -
	/**
	*	@param aVector:	the other vector
	*	@return the modified vector
	*/
	//---------------------------------------------------------------------------
	const Vec2 operator-(const Vec2& aVector) const;


	//---------------------------------------------------------------------------
	///	Operator -=
	/**
	*	@param aVector:	the other vector
	*	@return the modified vector
	*/
	//---------------------------------------------------------------------------
	Vec2& operator-=(const Vec2& aVector);


	//---------------------------------------------------------------------------
	///	Operator * to multiply each component of the vector by a given value.
	/**
	*	@param aValue:	the multiplicative value
	*	@return the modified vector
	*/
	//---------------------------------------------------------------------------
	const Vec2 operator*(const double& aValue) const;


	//---------------------------------------------------------------------------
	///	Operator *= to multiply each component of the vector by a given value,
	///	and modify the current vector.
	/**
	*	@param aValue:	the multiplicative value
	*	@return the modified vector
	*/
	//---------------------------------------------------------------------------
	Vec2& operator*=(const double& aValue);


	//---------------------------------------------------------------------------
	///	Operator * to multiply each component of the vector by a given value.
	/**
	*	@param aValue:	the multiplicative value
	*	@return the modified vector
	*/
	//---------------------------------------------------------------------------
	const Vec2 operator*(const float& aValue) const;


	//---------------------------------------------------------------------------
	///	Operator *= to multiply each component of the vector by a given value,
	///	and modify the current vector.
	/**
	*	@param aValue:	the multiplicative value
	*	@return the modified vector
	*/
	//---------------------------------------------------------------------------
	Vec2& operator*=(const float& aValue);


	//---------------------------------------------------------------------------
	///	Operator / to divide each component of the vector by a given value.
	/**
	*	@param aValue:	the value
	*	@return the modified vector
	*/
	//---------------------------------------------------------------------------
	const Vec2 operator/(const double& aValue) const;


	//---------------------------------------------------------------------------
	///	Operator /= to divide each component of the vector by a given value,
	///	and modify the current vector.
	/**
	*	@param aValue:	the value
	*	@return the modified vector
	*/
	//---------------------------------------------------------------------------
	Vec2& operator/=(const double& aValue);


	//---------------------------------------------------------------------------
	///	Operator / to divide each component of the vector by a given value.
	/**
	*	@param aValue:	the value
	*	@return the modified vector
	*/
	//---------------------------------------------------------------------------
	const Vec2 operator/(const float& aValue) const;


	//---------------------------------------------------------------------------
	///	Operator /= to divide each component of the vector by a given value,
	///	and modify the current vector.
	/**
	*	@param aValue:	the value
	*	@return the modified vector
	*/
	//---------------------------------------------------------------------------
	Vec2& operator/=(const float& aValue);


	//---------------------------------------------------------------------------
	///	Operator - to get the opposite vector.
	/**
	*	@return the opposite vector
	*/
	//---------------------------------------------------------------------------
	const Vec2 operator-() const;


//******************************************************************************
protected:
	//**************************************************************************
	T m_x; ///< the position along the x-axis
	T m_y; ///< the position along the y-axi
};


} // namespace Math


//******************************************************************************
#include "Vec2.inl"


#endif // VEC2_H
