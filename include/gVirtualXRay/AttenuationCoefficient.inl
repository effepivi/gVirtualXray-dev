/**
********************************************************************************
*
*	@file		AttenuationCoefficient.inl
*
*	@brief		Class to manage an attenuation coefficient
*
*	@version	1.2
*
*	@date		30/11/2013
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
#include <iostream>

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//-----------------------------------------------------------------------------------------------------
inline AttenuationCoefficient::AttenuationCoefficient(const RATIONAL_NUMBER& anEnergy,
													  const RATIONAL_NUMBER& anAttenuationCoefficient):
//-----------------------------------------------------------------------------------------------------
		m_energy(anEnergy),
		m_mu(anAttenuationCoefficient)
//-----------------------------------------------------------------------------------------------------
{}


//--------------------------------------------------------------
inline RATIONAL_NUMBER AttenuationCoefficient::getEnergy() const
//--------------------------------------------------------------
{
	return (m_energy);
}


//------------------------------------------------------------------------------
inline RATIONAL_NUMBER AttenuationCoefficient::getAttenuationCoefficient() const
//------------------------------------------------------------------------------
{
	return (m_mu);
}


//-----------------------------------------------------------
inline std::istream& operator >>(std::istream &is,
								 AttenuationCoefficient &obj)
//-----------------------------------------------------------
{
	is >> obj.m_energy;
	is >> obj.m_mu;
	
	obj.m_energy *= MeV;
	obj.m_mu *= cm2/g;
	
	return (is);
}


//-----------------------------------------------------------------
inline std::ostream& operator <<(std::ostream &os,
								 const AttenuationCoefficient &obj)
//-----------------------------------------------------------------
{
	os << obj.m_energy / MeV << " MeV\t" << obj.m_mu / (cm2/g) << " cm2/g";
	return (os);
}


} // namespace gVirtualXRay
