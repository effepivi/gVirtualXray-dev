/**
********************************************************************************
*
*	@file		ImplicitSurface.inl
*
*	@brief		Class to build 3D implicit surfaces. To know what implicit
*				surfaces are, please visit Paul Bourke's tutorial available at
*				http://paulbourke.net/geometry/implicitsurf/
*
*	@version	1.0
*
*	@date		08/03/2014
*
*	@author		Dr Franck P. Vidal
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
#include <cmath>

#ifndef OUT_OF_BOUNDS_EXCEPTION_H
#include "gVirtualXRay/OutOfBoundsException.h"
#endif


//******************************************************************************
//	define
//******************************************************************************


//******************************************************************************
//	namespace
//******************************************************************************
namespace Graphics {
	

//-----------------------------------------------------------------------
inline RATIONAL_NUMBER ImplicitSurface::evaluate(RATIONAL_NUMBER r) const
//-----------------------------------------------------------------------
{
	switch (m_density_function)
	{
	case SPHERE:
		return evaluateSphere(r);
		
	case BLOBBY_MOLECULE:
		return evaluateBlobbyMolecule(r);
		
	case META_BALL:
		return evaluateMetaBall(r);
		
	case SOFT_OBJECT:
		return evaluateSoftObject(r);
	}
}


//--------------------------------------
inline void ImplicitSurface::setSphere()
//--------------------------------------
{
	m_density_function = SPHERE;
}


//---------------------------------------------------------------
inline void ImplicitSurface::setBlobbyMolecule(RATIONAL_NUMBER a,
											   RATIONAL_NUMBER b)
//---------------------------------------------------------------
{
	m_density_function = BLOBBY_MOLECULE;
	m_a = a;
	m_b = b;	
}


//---------------------------------------------------------
inline void ImplicitSurface::setMetaBall(RATIONAL_NUMBER a,
										 RATIONAL_NUMBER b)
//---------------------------------------------------------
{
	m_density_function = META_BALL;
	m_a = a;
	m_b = b;	
}


//-----------------------------------------------------------
inline void ImplicitSurface::setSoftObject(RATIONAL_NUMBER a,
										   RATIONAL_NUMBER b)
//-----------------------------------------------------------
{
	m_density_function = SOFT_OBJECT;
	m_a = a;
	m_b = b;	
}


//-----------------------------------------------
inline void ImplicitSurface::resetControlPoints()
//-----------------------------------------------
{
	m_control_point_set.clear();
}


//----------------------------------------------
inline void ImplicitSurface::resetControlLines()
//----------------------------------------------
{
	m_control_line_set.clear();	
}


//---------------------------------------------------------------------
inline void ImplicitSurface::addControlPoint(const VEC3& aControlPoint)
//---------------------------------------------------------------------
{
	m_control_point_set.push_back(aControlPoint);
}


//---------------------------------------------------------------------
inline void ImplicitSurface::addControlLine(const VEC3& aControlPoint1,
											const VEC3& aControlPoint2)
//---------------------------------------------------------------------
{
	m_control_line_set.push_back(std::pair<VEC3, VEC3>(aControlPoint1, aControlPoint2));
}


//-----------------------------------------------------------------------------
inline const VEC3& ImplicitSurface::getControlPoint(unsigned int anIndex) const
//-----------------------------------------------------------------------------
{
	if (anIndex >= m_control_point_set.size())
	{
		OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	return (m_control_point_set[anIndex]);
}


//-----------------------------------------------------------------
inline VEC3& ImplicitSurface::getControlPoint(unsigned int anIndex)
//-----------------------------------------------------------------
{
	if (anIndex >= m_control_point_set.size())
	{
		OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	return (m_control_point_set[anIndex]);
}


//---------------------------------------------------------------------------------------------
inline const std::pair<VEC3, VEC3>& ImplicitSurface::getControlLine(unsigned int anIndex) const
//---------------------------------------------------------------------------------------------
{
	if (anIndex >= m_control_line_set.size())
	{
		OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}
	
	return (m_control_line_set[anIndex]);
}


//---------------------------------------------------------------------------------
inline std::pair<VEC3, VEC3>& ImplicitSurface::getControlLine(unsigned int anIndex)
//---------------------------------------------------------------------------------
{
	if (anIndex >= m_control_line_set.size())
	{
		OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}
	
	return (m_control_line_set[anIndex]);
}


//-----------------------------------------------------------------------------
inline RATIONAL_NUMBER ImplicitSurface::evaluateSphere(RATIONAL_NUMBER r) const
//-----------------------------------------------------------------------------
{
	if (0 <= r)
	{
		return (1.0 / (r * r));		
	}
	else
	{
		return (0);
	}
}


//-------------------------------------------------------------------------------------
inline RATIONAL_NUMBER ImplicitSurface::evaluateBlobbyMolecule(RATIONAL_NUMBER r) const
//-------------------------------------------------------------------------------------
{
	if (0 <= r)
	{
		return (m_a * exp(-m_b * r * r));		
	}
	else
	{
		return (0);
	}
}


//-------------------------------------------------------------------------------
inline RATIONAL_NUMBER ImplicitSurface::evaluateMetaBall(RATIONAL_NUMBER r) const
//-------------------------------------------------------------------------------
{
	if (0 <= r && r <= m_b / 3.0)
	{
		return (m_a * (1.0 - (3.0 * r * r) / (m_b * m_b)));
	}
	else if (m_b / 3.0 <= r && r <= m_b)
	{
		return (((3.0 * m_a) / 2.0) * pow((1.0 - (r / m_b)), 2.0));
	}
	else if (m_b <= r)
	{
		return (0);
	}
}


//---------------------------------------------------------------------------------
inline RATIONAL_NUMBER ImplicitSurface::evaluateSoftObject(RATIONAL_NUMBER r) const
//---------------------------------------------------------------------------------
{
	if (0 <= r && r <= m_b)
	{
		return (m_a * 
				(1.0 - 
						(( 4.0 * pow(r, 6.0)) / (9.0 * pow(m_b, 6.0))) + 
						((17.0 * pow(r, 4.0)) / (9.0 * pow(m_b, 4.0))) - 
						((22.0 * pow(r, 2.0)) / (9.0 * pow(m_b, 2.0)))));
	}
	else
	{
		return (0);
	}
}


} // namespace Graphics
