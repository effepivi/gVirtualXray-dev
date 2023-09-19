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

#ifndef __Logger_h
#include <gVirtualXRay/Logger.h>
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//-------------------------------------------------
inline XRayBeam::XRayBeam():
//-------------------------------------------------
		m_number_of_photons_lower_threshold(EPSILON),
        m_use_poisson_noise(false),
        m_number_of_photons_per_pixel(0),
        m_voltage(-1),
        m_has_spekpy(false),
        m_has_xpecgen(false),
        m_use_spekpy(false),
        m_use_xpecgen(false),
        m_p_global(0),
        m_p_module(0),
        m_p_local(0),
        m_p_func_spekpy(0),
        m_p_func_xpecgen(0)
//-------------------------------------------------
{
    initPython();
}


//--------------------------
inline XRayBeam::~XRayBeam()
//--------------------------
{
    clear();
    releasePython();
}


//---------------------------
inline void XRayBeam::clear()
//---------------------------
{
    // Remove all the energy channels
    m_beam_spectrum.clear();
    m_beam_spectrum_for_photon_count_detector.clear();
    m_photon_count_bands.clear();
    m_number_of_photons_per_pixel = 0;

    m_voltage = -1;
    m_filtration.clear();
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



//---------------------------------------------------------------------------------
inline const std::vector<RATIONAL_NUMBER>& XRayBeam::getPhotonCountingBands() const
//---------------------------------------------------------------------------------
{
    return m_photon_count_bands;
}


//---------------------------------------------------------------
inline void XRayBeam::setVoltage(const RATIONAL_NUMBER& aVoltage)
//---------------------------------------------------------------
{
#ifdef HAS_PythonLibs    
    m_voltage = aVoltage;
    createSpectrumFromVoltage();
#else
		LOGGER.logWarning("PythonLibs support was disabled when the library was compiled. This function ") << 
            __FUNCTION__ << " in " << __FILE__ << " won't have any effect." << std::endl;
#endif
}


//-------------------------------------------------
inline RATIONAL_NUMBER XRayBeam::getVoltage() const
//-------------------------------------------------
{
    return m_voltage;
}


//-------------------------------------------------------------------------------------------------------------
inline void XRayBeam::setFiltration(const std::vector<std::pair<std::string, RATIONAL_NUMBER> >& aSetOfFilters)
//-------------------------------------------------------------------------------------------------------------
{
#ifdef HAS_PythonLibs    
    m_filtration = aSetOfFilters;
    createSpectrumFromVoltage();
#else
		LOGGER.logWarning("PythonLibs support was disabled when the library was compiled. This function ") << 
            __FUNCTION__ << " in " << __FILE__ << " won't have any effect." << std::endl;
#endif
}


//-------------------------------------------------------------------------------------------------
inline const std::vector<std::pair<std::string, RATIONAL_NUMBER> >& XRayBeam::getFiltration() const
//-------------------------------------------------------------------------------------------------
{
    return m_filtration;
}


//-------------------------------
inline bool XRayBeam::hasSpekpy()
//-------------------------------
{
    return m_has_spekpy;
}


//--------------------------------
inline bool XRayBeam::hasXpecgen()
//--------------------------------
{
    return m_has_xpecgen;
}


//-------------------------------
inline void XRayBeam::useSpekpy()
//-------------------------------
{
    if (m_has_spekpy)
    {
		LOGGER.logNow("Use Spekpy to generate beam spectrums from tube voltages.") << std::endl;
        m_use_spekpy = true;
    }
    else
    {
		LOGGER.logWarning("Spekpy is not installed, try Xpecgen instead.") << std::endl;
        m_use_spekpy = false;

        if (m_has_xpecgen)
        {
            m_use_xpecgen = true;
        }
        else
        {
    		LOGGER.logWarning("Xpecgen is not installed either.") << std::endl;
            m_use_xpecgen = false;
        }
    }
}


//--------------------------------
inline void XRayBeam::useXpecgen()
//--------------------------------
{
    if (m_has_xpecgen)
    {
		LOGGER.logNow("Use Xpecgen to generate beam spectrums from tube voltages.") << std::endl;
        m_use_xpecgen = true;
    }
    else
    {
		LOGGER.logWarning("Xpecgen is not installed, try Spekpy instead.") << std::endl;
        m_use_xpecgen = false;

        if (m_has_spekpy)
        {
            m_use_spekpy = true;
        }
        else
        {
    		LOGGER.logWarning("Spekpy is not installed either.") << std::endl;
            m_use_spekpy = false;
        }
    }
}


//-------------------------------------
inline bool XRayBeam::useSpekpy() const
//-------------------------------------
{
    return m_use_spekpy;
}


//--------------------------------------
inline bool XRayBeam::useXpecgen() const
//--------------------------------------
{
    return m_use_xpecgen;
}



} // namespace gVirtualXRay
