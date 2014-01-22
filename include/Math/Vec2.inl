/**
********************************************************************************
*
*	@file		Vec2.inl
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


//******************************************************************************
//	namespace
//******************************************************************************
namespace Math {


//-----------------------------------------------------------
template<typename T> Vec2<T>::Vec2(const T& aX, const T& aY):
//-----------------------------------------------------------
	m_x(aX),
	m_y(aY)
//-----------------------------------------------------------
{}


//------------------------------------------------------
template<typename T> void Vec2<T>::setX(const T& aValue)
//------------------------------------------------------
{
	m_x = aValue;
}


//------------------------------------------------------
template<typename T> void Vec2<T>::setY(const T& aValue)
//------------------------------------------------------
{
	m_y = aValue;
}


//-------------------------------------------
 template<typename T> T Vec2<T>::getX() const
//-------------------------------------------
{
	return (m_x);
}


//-------------------------------------------
 template<typename T> T Vec2<T>::getY() const
//-------------------------------------------
{
	return (m_y);
}


//--------------------------------------------------
template<typename T> double Vec2<T>::length() const
//--------------------------------------------------
{
	return (sqrt(m_x * m_x + m_y * m_y));
}


//---------------------------------------------------
template<typename T> Vec2<T> Vec2<T>::normal() const
//---------------------------------------------------
{
	return (Vec2(m_x, m_y) / length());
}


//---------------------------------------------
template<typename T> void Vec2<T>::normalize()
//---------------------------------------------
{
	double vector_length(length());
	m_x /= vector_length;
	m_y /= vector_length;
}


//---------------------------------------------
template<typename T> void Vec2<T>::normalise()
//---------------------------------------------
{
	return (normalize());
}


//------------------------------------------------------------------------
template<typename T> double Vec2<T>::dotProduct(const Vec2& aVector) const
//------------------------------------------------------------------------
{
	return (m_x * aVector.m_x + m_y * aVector.m_y);
}


//----------------------------------------------------------------------
template<typename T> double Vec2<T>::distance(const Vec2& aVector) const
//----------------------------------------------------------------------
{
	return (sqrt(
			pow(m_x - aVector.m_x, 2.0) +
			pow(m_y - aVector.m_y, 2.0))
	);
}



//-----------------------------------------------------------------------------
template<typename T> const Vec2<T> Vec2<T>::operator+(const Vec2& aValue) const
//-----------------------------------------------------------------------------
{
	return (Vec2(m_x + aValue.m_x, m_y + aValue.m_y));
}


//--------------------------------------------------------------------
 template<typename T> Vec2<T>& Vec2<T>::operator+=(const Vec2& aValue)
//--------------------------------------------------------------------
{
	m_x += aValue.m_x;
	m_y += aValue.m_y;

	return (*this);
}


//-----------------------------------------------------------------------------
template<typename T> const Vec2<T> Vec2<T>::operator-(const Vec2& aValue) const
//-----------------------------------------------------------------------------
{
	return (Vec2(m_x - aValue.m_x, m_y - aValue.m_y));
}


//-------------------------------------------------------------------
template<typename T> Vec2<T>& Vec2<T>::operator-=(const Vec2& aValue)
//-------------------------------------------------------------------
{
	m_x -= aValue.m_x;
	m_y -= aValue.m_y;

	return (*this);
}


//-------------------------------------------------------------------------------
template<typename T> const Vec2<T> Vec2<T>::operator*(const double& aValue) const
//-------------------------------------------------------------------------------
{
	return (Vec2(m_x * aValue, m_y * aValue));
}


//---------------------------------------------------------------------
template<typename T> Vec2<T>& Vec2<T>::operator*=(const double& aValue)
//---------------------------------------------------------------------
{
	m_x *= aValue;
	m_y *= aValue;

	return (*this);
}


//------------------------------------------------------------------------------
template<typename T> const Vec2<T> Vec2<T>::operator*(const float& aValue) const
//------------------------------------------------------------------------------
{
	return (Vec2(m_x * aValue, m_y * aValue));
}


//--------------------------------------------------------------------
template<typename T> Vec2<T>& Vec2<T>::operator*=(const float& aValue)
//--------------------------------------------------------------------
{
	m_x *= aValue;
	m_y *= aValue;

	return (*this);
}


//-------------------------------------------------------------------------------
template<typename T> const Vec2<T> Vec2<T>::operator/(const double& aValue) const
//-------------------------------------------------------------------------------
{
	return (Vec2(m_x / aValue, m_y / aValue));
}


//---------------------------------------------------------------------
template<typename T> Vec2<T>& Vec2<T>::operator/=(const double& aValue)
//---------------------------------------------------------------------
{
	m_x /= aValue;
	m_y /= aValue;

	return (*this);
}


//------------------------------------------------------------------------------
template<typename T> const Vec2<T> Vec2<T>::operator/(const float& aValue) const
//------------------------------------------------------------------------------
{
	return (Vec2(m_x / aValue, m_y / aValue));
}


//--------------------------------------------------------------------
template<typename T> Vec2<T>& Vec2<T>::operator/=(const float& aValue)
//--------------------------------------------------------------------
{
	m_x /= aValue;
	m_y /= aValue;

	return (*this);
}


//-----------------------------------------------------------
template<typename T> const Vec2<T> Vec2<T>::operator-() const
//-----------------------------------------------------------
{
	return (Vec2(-m_x, -m_y));
}


//--------------------------------------------------------------------------
template <typename T> std::ostream& operator<<(std::ostream& anOutputStream,
											   const Vec2<T>& aVector)
//--------------------------------------------------------------------------
{
	anOutputStream << aVector.getX() << ", " << aVector.getY();
	return (anOutputStream);
}


} // namespace Math
