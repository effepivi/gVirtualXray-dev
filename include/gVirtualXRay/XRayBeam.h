/**
********************************************************************************
*
*	@file		XRayBeam.h
*
*	@brief		Class to manage X-Ray beams. The beam spectrum is discretised
*				into energy channels.
*
*	@version	1.0
*
*	@date		29/11/2013
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


#ifndef XRAY_BEAM_H
#define XRAY_BEAM_H


//******************************************************************************
//	Include
//******************************************************************************
#include <GL/glew.h>

#include <vector>
#include <iostream>

#ifndef TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef SPECTRUM_RECORD_H
#include "gVirtualXRay/SpectrumRecord.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*	@class	XRayBeam
*	@brief	XRayBeam is a class to handle the X-Ray beam.
*			The beam spectrum is discretised into energy channels.
*/
//==============================================================================
class XRayBeam
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	//--------------------------------------------------------------------------
	///	Default constructor.
	//--------------------------------------------------------------------------
	XRayBeam();


	//--------------------------------------------------------------------------
	///	Initialise the X-ray beam specturm with a polychromatic spectrum.
	/**
	*	@param aFileName:	name of the file that stores the spectrum
	*	@param aUnit:	unit in which the data is stored, e.g. meV, eV, keV, or MeV
	*	@param aMergeChannelFlag:	the number of records to store in a single bin (default value: 1)
	*/
	//--------------------------------------------------------------------------
	void initialise(const char* aFileName,
			const RATIONAL_NUMBER& aUnit,
			unsigned int aMergeChannelFlag = 1);


	//--------------------------------------------------------------------------
	///	Initialise the X-ray beam specturm with a monochromatic spectrum.
	/**
	*	@param anIncidentEnergy:	the energy of these photons
	*/
	//--------------------------------------------------------------------------
	void initialise(const RATIONAL_NUMBER& anIncidentEnergy);


	//--------------------------------------------------------------------------
	///	Normalise the number of photons so that the total incident energy is 1.0.
	//--------------------------------------------------------------------------
	void normalise();


	//--------------------------------------------------------------------------
	///	Set a threshold to discard energy bins with a tiny number of photons.
	/**
	*	@param aThreshold:	the threshold
	*/
	//--------------------------------------------------------------------------
	void setNumberOfPhotonsLowerThreshold(const RATIONAL_NUMBER& aThreshold);

	
	//--------------------------------------------------------------------------
	///	Accessor on the number of bins in the spectrum.
	/**
	*	@return the number of energy bins in the spectrum
	*/
	//--------------------------------------------------------------------------
	unsigned int getEnergyChannelNumber() const;

	
	//--------------------------------------------------------------------------
	///	Accessor on a given energy bin.
	/**
	*	@param anID:	the ID of the bin
	*	@return the energy bin
	*/
	//--------------------------------------------------------------------------
	const SpectrumRecord& getEnergyChannel(unsigned int anID) const;
	

//******************************************************************************
protected:
	/// Set of photon bins
	std::vector<SpectrumRecord> m_beam_spectrum;


	/// All the photon bins with a number of photons lower than
	/// m_epsilon_number_of_photons will be ignored
	RATIONAL_NUMBER m_number_of_photons_lower_threshold;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "XRayBeam.inl"


#endif // XRAY_BEAM_H

