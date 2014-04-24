/**
********************************************************************************
*
*	@file		SpectrumRecord.h
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


#ifndef GVXR_SPECTRUM_RECORD_H
#define GVXR_SPECTRUM_RECORD_H


//******************************************************************************
//	Include
//******************************************************************************
#include <iostream>

#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//******************************************************************************
//	Class declaration
//******************************************************************************
class	SpectrumRecord;


//******************************************************************************
//	Function declaration
//******************************************************************************
//------------------------------------------------------------------------------
///	operator>>
/**
*	@param is: the input stream
*	@param obj: the record to load from the stream
*	@return the input stream
*/
//------------------------------------------------------------------------------
std::istream& operator>>(std::istream& is, SpectrumRecord& obj);


//------------------------------------------------------------------------------
///	operator<<
/**
*	@param os: the output stream
*	@param obj: the record to store in the stream
*	@return the output stream
*/
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const SpectrumRecord& obj);


//==============================================================================
/**
*	@class	SpectrumRecord
*	@brief	SpectrumRecord is a class to handle a record of the X-Ray beam,
*			i.e. an energy bin (number of photons, energy of the photons in
*			the bin).
*/
//==============================================================================
class SpectrumRecord
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	//--------------------------------------------------------------------------
	///	Default constructor.
	/**
	*	@param aPhotonNumber: number of photons in the bin (default value: 0)
	*	@param anIncidentEnergy: energy of the photons (default value: 0)
	*/
	//--------------------------------------------------------------------------
	SpectrumRecord(const RATIONAL_NUMBER& aPhotonNumber = 0,
			const RATIONAL_NUMBER& anIncidentEnergy = 0);


	//--------------------------------------------------------------------------
	///	Accessor on the number of photons in the bin.
	/**
	*	@return the number of photons
	*/
	//--------------------------------------------------------------------------
	RATIONAL_NUMBER getPhotonNumber() const;


	//--------------------------------------------------------------------------
	///	Accessor on the energy of photons.
	/**
	*	@return the energy of photons
	*/
	//--------------------------------------------------------------------------
	RATIONAL_NUMBER getPhotonEnergy() const;


//******************************************************************************
protected:
	/// The number of photons
	RATIONAL_NUMBER m_photon_number;


	/// The energy of the photons
	RATIONAL_NUMBER m_energy;


	//**************************************************************************
	//	Friend class
	//**************************************************************************
	friend class XRayBeam;


	//**************************************************************************
	//	Friend operators
	//**************************************************************************
	friend std::istream& operator>>(std::istream& is, SpectrumRecord& obj);
	friend std::ostream& operator<<(std::ostream& os, const SpectrumRecord& obj);
};


} // namespace gVirtualXRay


//******************************************************************************
#include "SpectrumRecord.inl"


#endif // SPECTRUM_RECORD_H

