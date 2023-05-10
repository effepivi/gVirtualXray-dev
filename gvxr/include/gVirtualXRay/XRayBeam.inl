/*

Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
http://www.fpvidal.net/
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the Bangor University nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


/**
********************************************************************************
*
*	@file		XRayBeam.inl
*
*	@brief		Class to manage X-Ray beams. The beam spectrum is discretised
*				into energy channels.
*
*	@version    1.0
*
*	@date		29/11/2013
*
*	@author		Dr Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*   http://www.fpvidal.net/, Dec 2014, 2014, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#include <fstream>
#include <sstream>
#include <cfloat>
#include <algorithm>

#ifndef __FileDoesNotExistException_h
#include "gVirtualXRay/FileDoesNotExistException.h"
#endif

#ifndef __OutOfBoundsException_h
#include "gVirtualXRay/OutOfBoundsException.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//-------------------------------------------------
inline XRayBeam::XRayBeam():
//-------------------------------------------------
		m_number_of_photons_lower_threshold(0),
        m_use_poisson_noise(false),
        m_number_of_photons_per_pixel(0)
//-------------------------------------------------
{}


//---------------------------
inline void XRayBeam::clear()
//---------------------------
{
    // Remove all the energy channels
    m_beam_spectrum.clear();
    m_beam_spectrum_for_photon_count_detector.clear();
    m_number_of_photons_per_pixel = 0;
}


//-----------------------------------------------------------------------
inline void XRayBeam::addChannel(const RATIONAL_NUMBER& aNumberOfPhotons,
                                 const RATIONAL_NUMBER& anIncidentEnergy)
//-----------------------------------------------------------------------
{
    // Temporary structure
	SpectrumRecord temp;

    // Store the number of photons
    temp.m_photon_number = aNumberOfPhotons;

    // The number of photons is above the cut-off threshold
    if (temp.m_photon_number > m_number_of_photons_lower_threshold)
    {
        // Store the energy
        temp.m_energy = anIncidentEnergy;

        // Store the data
        m_beam_spectrum.push_back(temp);

        // Add the number of photons
        m_number_of_photons_per_pixel += aNumberOfPhotons;

        // Sort the spectrum
        sort();
    }
}


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
	while (beam_spectrum_stream >> temp)
	{
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

    // Sort the spectrum
    sort();

    // Reset the total number of photons per pixel
    m_number_of_photons_per_pixel = 0;

    // Proceed each energy channel
    for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_beam_spectrum.begin());
        ite_beam_spectrum != m_beam_spectrum.end();
        ++ite_beam_spectrum)
    {
        m_number_of_photons_per_pixel += ite_beam_spectrum->m_photon_number;
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
    m_number_of_photons_per_pixel = 1;
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

    // Reset the total number of photons per pixel
    m_number_of_photons_per_pixel = 0;

    // Proceed each energy channel
    for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_beam_spectrum.begin());
        ite_beam_spectrum != m_beam_spectrum.end();
        ++ite_beam_spectrum)
    {
        m_number_of_photons_per_pixel += ite_beam_spectrum->m_photon_number;
    }
}


//------------------------------------------------------------------------------------
inline void XRayBeam::setNumberOfPhotonsLowerThreshold(const RATIONAL_NUMBER& aThreshold)
//------------------------------------------------------------------------------------
{
	m_number_of_photons_lower_threshold = aThreshold;
}


//----------------------------------------------------------
inline unsigned int XRayBeam::getEnergyChannelNumber() const
//----------------------------------------------------------
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


//-----------------------------------------------------
inline RATIONAL_NUMBER XRayBeam::getTotalEnergy() const
//-----------------------------------------------------
{
    // Compute the total incident energy
    RATIONAL_NUMBER total_energy(0);
    for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_beam_spectrum.begin());
        ite_beam_spectrum != m_beam_spectrum.end();
        ++ite_beam_spectrum)
    {
        total_energy += ite_beam_spectrum->m_energy * ite_beam_spectrum->m_photon_number;
    }

    return (total_energy);
}


//----------------------------------------
inline void XRayBeam::enablePoissonNoise()
//----------------------------------------
{
    m_use_poisson_noise = true;
}


//-----------------------------------------
inline void XRayBeam::disablePoissonNoise()
//-----------------------------------------
{
    m_use_poisson_noise = false;
}


//------------------------------------------------------------------------
inline void XRayBeam::setNumberOfPhotons(RATIONAL_NUMBER aNumberOfPhotons)
//------------------------------------------------------------------------
{
    RATIONAL_NUMBER backup_number_of_photons = m_number_of_photons_per_pixel;
    m_number_of_photons_per_pixel = aNumberOfPhotons;

    RATIONAL_NUMBER old_to_new = m_number_of_photons_per_pixel / backup_number_of_photons;

    for (std::vector<SpectrumRecord>::iterator ite_beam_spectrum(m_beam_spectrum.begin());
        ite_beam_spectrum != m_beam_spectrum.end();
        ++ite_beam_spectrum)
    {
        ite_beam_spectrum->m_photon_number *= old_to_new;
    }
}


//---------------------------------------------------------
inline RATIONAL_NUMBER XRayBeam::getNumberOfPhotons() const
//---------------------------------------------------------
{
    return m_number_of_photons_per_pixel;
}


//-----------------------------------------------
inline void XRayBeam::usePoissonNoise(bool aFlag)
//-----------------------------------------------
{
    m_use_poisson_noise = aFlag;
}


//-------------------------------------------
inline bool XRayBeam::usePoissonNoise() const
//-------------------------------------------
{
    return (m_use_poisson_noise);
}


//--------------------------
inline void XRayBeam::sort()
//--------------------------
{
    std::sort(m_beam_spectrum.begin(), m_beam_spectrum.end());
}


//-----------------------------------------------------------------------
inline void XRayBeam::setPhotonCountingBands(RATIONAL_NUMBER aMinEnergy,
                                             RATIONAL_NUMBER aMaxEnergy,
                                             unsigned int aNumberOfBands)
//-----------------------------------------------------------------------
{
    // Reset the array
    m_beam_spectrum_for_photon_count_detector.clear();

    // Ignore this functionality if there is one band only
    if (aNumberOfBands > 1)
    {
        // Get the min and max energies
        RATIONAL_NUMBER step = (aMaxEnergy - aMinEnergy) / aNumberOfBands;
        RATIONAL_NUMBER max_energy_current_band = aMinEnergy;

        std::vector<SpectrumRecord>::const_iterator next_record_ite = m_beam_spectrum.begin();

        // Process every channel in the spectrum
        for (int i = 0; i < aNumberOfBands; ++i)
        {
            // Add a new band
            m_beam_spectrum_for_photon_count_detector.push_back(std::vector<SpectrumRecord>());
            max_energy_current_band += step;
            RATIONAL_NUMBER last_recorded_energy = 0;

            // Process all the channel of the energy band
            while (next_record_ite != m_beam_spectrum.end() && last_recorded_energy < max_energy_current_band)
            {
                if (next_record_ite->m_energy < max_energy_current_band)
                {
                    m_beam_spectrum_for_photon_count_detector.back().push_back(*next_record_ite);
                    last_recorded_energy = next_record_ite->m_energy;

                    ++next_record_ite;
                }
                // Exit the while loop
                else
                {
                    last_recorded_energy = max_energy_current_band;
                }
            }

            // The last band is empty
            if (m_beam_spectrum_for_photon_count_detector.back().size() == 0)
            {
                // Drop the last band as it's empty
                m_beam_spectrum_for_photon_count_detector.pop_back();
            }
        }
    }
}


//------------------------------------------------------------------
inline unsigned int XRayBeam::getNumberOfPhotonCountingBands() const
//------------------------------------------------------------------
{
    return m_beam_spectrum_for_photon_count_detector.size();
}


//------------------------------------------------------------------
inline void XRayBeam::activatePhotonCountingBand(unsigned int aBand)
//------------------------------------------------------------------
{
    // The functionality is in use
    if (getNumberOfPhotonCountingBands())
    {
        // aBand is valid
        if (aBand > getNumberOfPhotonCountingBands())
        {
            throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }

        // Replace the current spectrum with the corresponding band
        m_beam_spectrum = m_beam_spectrum_for_photon_count_detector[aBand];

        // Update the total number of photons in the band
        m_number_of_photons_per_pixel = 0;
        for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_beam_spectrum.begin());
            ite_beam_spectrum != m_beam_spectrum.end();
            ++ite_beam_spectrum)
        {
            m_number_of_photons_per_pixel += ite_beam_spectrum->m_photon_number;
        }
    }
}


//----------------------------------------------
inline void XRayBeam::mergePhotonCountingBands()
//----------------------------------------------
{
    // The functionality is in use
    if (getNumberOfPhotonCountingBands())
    {
        // Empty the spectrum
        m_beam_spectrum.clear();

        // Update the total number of photons in the spectrum
        m_number_of_photons_per_pixel = 0;

        // Process all the bands
        for (std::vector< std::vector<SpectrumRecord> >::const_iterator ite_band(m_beam_spectrum_for_photon_count_detector.begin());
            ite_band != m_beam_spectrum_for_photon_count_detector.end();
            ++ite_band)
        {
            // Process all the bins
            for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(ite_band->begin());
                ite_beam_spectrum != ite_band->end();
                ++ite_beam_spectrum)
            {
                // Add the bin
                m_beam_spectrum.push_back(*ite_beam_spectrum);

                // Update the total number of photons in the band
                m_number_of_photons_per_pixel += ite_beam_spectrum->m_photon_number;
            }
        }

        // Destroy the bands
        m_beam_spectrum_for_photon_count_detector.clear();
    }
}


} // namespace gVirtualXRay
