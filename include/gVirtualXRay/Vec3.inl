/**
********************************************************************************
*
*	@file		Vec3.inl
*
*	@brief		Template class to handle 3D vectors.
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


//******************************************************************************
//	Include
//******************************************************************************
#include <math.h>


//******************************************************************************
//	namespace
//******************************************************************************
namespace Math {


//------------------------------------------------------------------------
template<typename T> Vec3<T>::Vec3(const T& aX, const T& aY, const T& aZ):
//------------------------------------------------------------------------
	m_x(aX),
	m_y(aY),
	m_z(aZ)
//------------------------------------------------------------------------
{}


//------------------------------------------------------
template<typename T> void Vec3<T>::setX(const T& aValue)
//------------------------------------------------------
{
	m_x = aValue;
}


//------------------------------------------------------
template<typename T> void Vec3<T>::setY(const T& aValue)
//------------------------------------------------------
{
	m_y = aValue;
}


//------------------------------------------------------
template<typename T> void Vec3<T>::setZ(const T& aValue)
//------------------------------------------------------
{
	m_z = aValue;
}


//-------------------------------------------
template<typename T> T Vec3<T>::getX() const
//-------------------------------------------
{
	return (m_x);
}


//-------------------------------------------
template<typename T> T Vec3<T>::getY() const
//-------------------------------------------
{
	return (m_y);
}


//-------------------------------------------
template<typename T> T Vec3<T>::getZ() const
//-------------------------------------------
{
	return (m_z);
}


//--------------------------------------------------
template<typename T> double Vec3<T>::length() const
//--------------------------------------------------
{
	return (sqrt(m_x * m_x + m_y * m_y + m_z * m_z));
}


//---------------------------------------------------
template<typename T> Vec3<T> Vec3<T>::normal() const
//---------------------------------------------------
{
	return (Vec3(m_x, m_y, m_z) / length());
}


//---------------------------------------------
template<typename T> void Vec3<T>::normalize()
//---------------------------------------------
{
	double vector_length(length());
	m_x /= vector_length;
	m_y /= vector_length;
	m_z /= vector_length;
}


//---------------------------------------------
template<typename T> void Vec3<T>::normalise()
//---------------------------------------------
{
	return (normalize());
}


//------------------------------------------------------------------------
template<typename T> double Vec3<T>::dotProduct(const Vec3& aVector) const
//------------------------------------------------------------------------
{
	return (m_x * aVector.m_x + m_y * aVector.m_y + m_z * aVector.m_z);
}


//-----------------------------------------------------------------------
template<typename T> double Vec3<T>::operator*(const Vec3& aVector) const
//-----------------------------------------------------------------------
{
	return (m_x * aVector.m_x + m_y * aVector.m_y + m_z * aVector.m_z);
}


//----------------------------------------------------------------------
template<typename T> double Vec3<T>::distance(const Vec3& aVector) const
//----------------------------------------------------------------------
{
	return (sqrt(
			pow(m_x - aVector.m_x, 2.0) +
			pow(m_y - aVector.m_y, 2.0) +
			pow(m_z - aVector.m_z, 2.0))
	); 
}


//-----------------------------------------------------------------------------
template<typename T> const Vec3<T> Vec3<T>::operator+(const Vec3& aValue) const
//-----------------------------------------------------------------------------
{
	return (Vec3(m_x + aValue.m_x, m_y + aValue.m_y, m_z + aValue.m_z));
}


//--------------------------------------------------------------------
template<typename T> Vec3<T>& Vec3<T>::operator+=(const Vec3& aValue)
//--------------------------------------------------------------------
{
	m_x += aValue.m_x;
	m_y += aValue.m_y;
	m_z += aValue.m_z;

	return (*this);
}


//-----------------------------------------------------------------------------
template<typename T> const Vec3<T> Vec3<T>::operator-(const Vec3& aValue) const
//-----------------------------------------------------------------------------
{
	return (Vec3(m_x - aValue.m_x, m_y - aValue.m_y, m_z - aValue.m_z));
}


//-------------------------------------------------------------------
template<typename T> Vec3<T>& Vec3<T>::operator-=(const Vec3& aValue)
//-------------------------------------------------------------------
{
	m_x -= aValue.m_x;
	m_y -= aValue.m_y;
	m_z -= aValue.m_z;

	return (*this);
}


//-------------------------------------------------------------------------------
template<typename T> const Vec3<T> Vec3<T>::operator*(const double& aValue) const
//-------------------------------------------------------------------------------
{
	return (Vec3(m_x * aValue, m_y * aValue, m_z * aValue));
}


//---------------------------------------------------------------------
template<typename T> Vec3<T>& Vec3<T>::operator*=(const double& aValue)
//---------------------------------------------------------------------
{
	m_x *= aValue;
	m_y *= aValue;
	m_z *= aValue;

	return (*this);
}


//------------------------------------------------------------------------------
template<typename T> const Vec3<T> Vec3<T>::operator*(const float& aValue) const
//------------------------------------------------------------------------------
{
	return (Vec3(m_x * aValue, m_y * aValue, m_z * aValue));
}


//--------------------------------------------------------------------
template<typename T> Vec3<T>& Vec3<T>::operator*=(const float& aValue)
//--------------------------------------------------------------------
{
	m_x *= aValue;
	m_y *= aValue;
	m_z *= aValue;

	return (*this);
}


//-------------------------------------------------------------------------------
template<typename T> const Vec3<T> Vec3<T>::operator/(const double& aValue) const
//-------------------------------------------------------------------------------
{
	return (Vec3(m_x / aValue, m_y / aValue, m_z / aValue));
}


//---------------------------------------------------------------------
template<typename T> Vec3<T>& Vec3<T>::operator/=(const double& aValue)
//---------------------------------------------------------------------
{
	m_x /= aValue;
	m_y /= aValue;
	m_z /= aValue;

	return (*this);
}


//------------------------------------------------------------------------------
template<typename T> const Vec3<T> Vec3<T>::operator/(const float& aValue) const
//------------------------------------------------------------------------------
{
	return (Vec3(m_x / aValue, m_y / aValue, m_z / aValue));
}


//--------------------------------------------------------------------
template<typename T> Vec3<T>& Vec3<T>::operator/=(const float& aValue)
//--------------------------------------------------------------------
{
	m_x /= aValue;
	m_y /= aValue;
	m_z /= aValue;

	return (*this);
}


//-----------------------------------------------------------------------------
template<typename T> const Vec3<T> Vec3<T>::operator^(const Vec3& aValue) const
//-----------------------------------------------------------------------------
{
	return (Vec3(m_y * aValue.m_z - m_z * aValue.m_y,
			-(m_x * aValue.m_z - m_z * aValue.m_x),
			m_x * aValue.m_y - m_y * aValue.m_x));
}


//-----------------------------------------------------------
template<typename T> const Vec3<T> Vec3<T>::operator-() const
//-----------------------------------------------------------
{
	return (Vec3(-m_x, -m_y, -m_z));
}


//-------------------------------------------------------------------------
template<typename T> bool Vec3<T>::operator==(const Vec3<T>& aVector) const
//-------------------------------------------------------------------------
{
	bool return_value(fabs(m_x - aVector.m_x) < 0.0000001 &&
			fabs(m_y - aVector.m_y) < 0.0000001 &&
			fabs(m_z - aVector.m_z) < 0.0000001);
	
	return (return_value);
}


//-------------------------------------------------------------------------
template<typename T> bool Vec3<T>::operator!=(const Vec3<T>& aVector) const
//-------------------------------------------------------------------------
{
	bool return_value(fabs(m_x - aVector.m_x) >= 0.0000001 ||
			fabs(m_y - aVector.m_y) >= 0.0000001 ||
			fabs(m_z - aVector.m_z) >= 0.0000001);
	
	return (return_value);
}


//-------------------------------------------------------------
template <typename T> Vec3<T> operator*(const double& aValue,
										const Vec3<T>& aVector)
//-------------------------------------------------------------
{
	return Vec3<T>(aVector.getX() * aValue,
			aVector.getY() * aValue,
			aVector.getZ() * aValue);
}


//--------------------------------------------------------------------------
template <typename T> std::ostream& operator<<(std::ostream& anOutputStream,
											   const Vec3<T>& aVector)
//--------------------------------------------------------------------------
{
	anOutputStream << aVector.getX() << ", " << aVector.getY() << ", " << aVector.getZ();
	return (anOutputStream);
}
 

} // namespace Math
