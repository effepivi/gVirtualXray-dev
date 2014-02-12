/**
********************************************************************************
*
*	@file		SpectrumRecord.inl
*
*	@brief		Class to manage a record of the X-Ray beam,
*				i.e. an energy bin (number of photons, energy of the photons in
*				the bin).
*
*	@version	1.0
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

#ifndef UNITS_H
#include "gVirtualXRay/Units.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//-----------------------------------------------------------------------------
inline SpectrumRecord::SpectrumRecord(const RATIONAL_NUMBER& aPhotonNumber,
									  const RATIONAL_NUMBER& anIncidentEnergy):
//-----------------------------------------------------------------------------
		m_photon_number(aPhotonNumber),
		m_energy(anIncidentEnergy)
//-----------------------------------------------------------------------------
{}


//------------------------------------------------------------
inline RATIONAL_NUMBER SpectrumRecord::getPhotonNumber() const
//------------------------------------------------------------
{
	return (m_photon_number);
}


//------------------------------------------------------------
inline RATIONAL_NUMBER SpectrumRecord::getPhotonEnergy() const
//------------------------------------------------------------
{
	return (m_energy);
}


//---------------------------------------------------
inline std::istream& operator >>(std::istream &is,
								 SpectrumRecord &obj)
//---------------------------------------------------
{
	is >> obj.m_energy;
	is >> obj.m_photon_number;
		
	return (is);
}


//---------------------------------------------------------
inline std::ostream& operator <<(std::ostream &os,
								 const SpectrumRecord &obj)
//---------------------------------------------------------
{
	os << obj.getPhotonNumber() << " photons of " << obj.getPhotonEnergy() / keV << " keV";
	return (os);
}


} // namespace gVirtualXRay
