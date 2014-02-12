/**
********************************************************************************
*
*	@file		XRayBeam.inl
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


//******************************************************************************
//	Include
//******************************************************************************
#include <fstream>
#include <sstream>
#include <cfloat>

#ifndef FILE_DOES_NOT_EXIST_EXCEPTION_H
#include "gVirtualXRay/FileDoesNotExistException.h"
#endif

#ifndef OUT_OF_BOUNDS_EXCEPTION_H
#include "gVirtualXRay/OutOfBoundsException.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//--------------------------------------------
inline XRayBeam::XRayBeam():
//--------------------------------------------
		m_number_of_photons_lower_threshold(0)
//--------------------------------------------
{}



//-------------------------------------------------------------
inline void XRayBeam::initialise(const char* aFileName,
								 const RATIONAL_NUMBER& anUnit,
								 unsigned int aMergeChannelFlag)
//-------------------------------------------------------------
{
	// Empty the current beam spectrum
	m_beam_spectrum.clear();
	
	// Open the file
	std::ifstream beam_spectrum_stream(aFileName);
	
	// The file is not open
	if (!beam_spectrum_stream.is_open())
	{
		throw FileDoesNotExistException(__FILE__, __FUNCTION__, __LINE__, aFileName);
	}

	// Load the file
	SpectrumRecord temp;
	while (!beam_spectrum_stream.eof())
	{
		// Read a record
		beam_spectrum_stream >> temp;
		
		// The record is valid
		if (!beam_spectrum_stream.eof())
		{
			// The number of photons is greater than the threshold
			if (temp.m_photon_number > m_number_of_photons_lower_threshold)
			{
				// Apply the unit
				temp.m_energy *= anUnit;
				
				// Store the data
				m_beam_spectrum.push_back(temp);
			}
		}
	}
	
	// Merge channels
	if (aMergeChannelFlag > 1)
	{
		// Merge channels
		unsigned int counter(0);
		temp.m_photon_number = 0;
		temp.m_energy = 0;
		
		// The set of new records
		std::vector<SpectrumRecord> new_beam_spectrum;
		
		// Process all the spectrum
		for (std::vector<SpectrumRecord>::iterator ite(m_beam_spectrum.begin());
			ite != m_beam_spectrum.end();
			++ite)
		{
			// Add the current 'old' bin to the 'new' bin
			temp.m_photon_number += ite->m_photon_number;
			temp.m_energy += ite->m_energy * ite->m_photon_number;
			counter++;
		
			// Store the new bin
			if (counter == aMergeChannelFlag)
			{
				// Compute the average the energy
				temp.m_energy /= temp.m_photon_number;
				
				// Add the bin
				new_beam_spectrum.push_back(temp);
				
				// Reset counters
				counter = 0;
				temp.m_photon_number = 0;
				temp.m_energy = 0;
			}
		}
		
		// Add remaining channels if any
		if (counter)
		{
			temp.m_energy /= temp.m_photon_number;				
			new_beam_spectrum.push_back(temp);
		}

		// Store the new spectrum
		m_beam_spectrum = new_beam_spectrum;
	}
	
	// Get the min and max energy
	double min_energy(FLT_MAX);
	double max_energy(-FLT_MAX);
	for (std::vector<SpectrumRecord>::const_iterator ite(m_beam_spectrum.begin());
		ite != m_beam_spectrum.end();
		++ite)
	{
		double photon_number(ite->m_photon_number);
		double photon_energy(ite->m_energy);
		double bin_energy(photon_energy * photon_number);
		
		if (min_energy > bin_energy) min_energy = bin_energy;
		if (max_energy < bin_energy) max_energy = bin_energy;
	}
	
	// Normalise the beam energy
	double range(max_energy - min_energy);
	for (std::vector<SpectrumRecord>::iterator ite(m_beam_spectrum.begin());
		ite != m_beam_spectrum.end();
		++ite)
	{
		ite->m_photon_number /= range;
	}
}


//-----------------------------------------------------------------------
inline void XRayBeam::initialise(const RATIONAL_NUMBER& anIncidentEnergy)
//-----------------------------------------------------------------------
{
	// Empty the current beam spectrum
	m_beam_spectrum.clear();

	// Add the record
	m_beam_spectrum.push_back(SpectrumRecord(1, anIncidentEnergy));
}


//-------------------------------
inline void XRayBeam::normalise()
//-------------------------------
{
	// Compute the total incident energy
	RATIONAL_NUMBER total_energy(0);
	for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_beam_spectrum.begin());
		ite_beam_spectrum != m_beam_spectrum.end();
		++ite_beam_spectrum)
	{
		total_energy += ite_beam_spectrum->m_energy * ite_beam_spectrum->m_photon_number;
	}

	// The energy is positive
	if (total_energy > 0)
	{
		// Normalise the number of photons
		for (std::vector<SpectrumRecord>::iterator ite_beam_spectrum(m_beam_spectrum.begin());
			ite_beam_spectrum != m_beam_spectrum.end();
			++ite_beam_spectrum)
		{
			ite_beam_spectrum->m_photon_number /= total_energy;
		}
	}
}


//------------------------------------------------------------------------------------
inline void XRayBeam::setNumberOfPhotonsLowerThreshold(const RATIONAL_NUMBER& aThreshold)
//------------------------------------------------------------------------------------
{
	m_number_of_photons_lower_threshold = aThreshold;
}


//--------------------------------------------------
inline UINT XRayBeam::getEnergyChannelNumber() const
//--------------------------------------------------
{
	return (m_beam_spectrum.size());
}


//------------------------------------------------------------------------------
inline const SpectrumRecord& XRayBeam::getEnergyChannel(unsigned int anID) const
//------------------------------------------------------------------------------
{
	if (anID >= getEnergyChannelNumber())
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}
	
	return (m_beam_spectrum[anID]);
}


} // namespace gVirtualXRay
