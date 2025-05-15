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

#ifndef __ElementSet_h
#include "gVirtualXRay/ElementSet.h"
#endif

#ifndef __FileDoesNotExistException_h
#include "gVirtualXRay/FileDoesNotExistException.h"
#endif

#ifndef __OutOfBoundsException_h
#include "gVirtualXRay/OutOfBoundsException.h"
#endif

#ifndef __Logger_h
#include <gVirtualXRay/Logger.h>
#endif

#ifndef __PythonSingleton_h
#include <gVirtualXRay/PythonSingleton.h>
#endif


//******************************************************************************
//	definitions
//******************************************************************************
#undef USE_PYTHON


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//-------------------------------------------------
inline XRayBeam::XRayBeam():
//-------------------------------------------------
		m_bin_total_energy_lower_threshold(1 * eV),
        m_use_poisson_noise(false),
        m_photon_count_type(PhotonCountType::UNKNOWN),
        m_number_of_photons_per_pixel_at_SDD(0),
        m_number_of_photons_per_cm2_at_1m(0),
        m_voltage(-1),
        m_tube_angle_in_degrees(12),
        m_mAs(-1),
        m_use_spekpy(false),
        m_use_xpecgen(false),
        m_need_to_update_tube_spectrum(false)
//-------------------------------------------------
{
    useSpekpy();
}


//--------------------------
inline XRayBeam::~XRayBeam()
//--------------------------
{
    clear();
}


//---------------------------
inline void XRayBeam::clear()
//---------------------------
{
    // Remove all the energy channels
    clearSpectrumOnly();

    m_voltage = -1;
    m_tube_angle_in_degrees = 12;
    m_mAs = -1;
    
    m_filtration.clear();
    m_need_to_update_tube_spectrum = false;
}


//---------------------------------------
inline void XRayBeam::clearSpectrumOnly()
//---------------------------------------
{
    // Remove all the energy channels
    m_beam_spectrum.clear();
    m_beam_spectrum_for_photon_count_detector.clear();
    m_photon_count_bands.clear();

    m_photon_count_type = PhotonCountType::UNKNOWN;
    m_number_of_photons_per_pixel_at_SDD = 0;
    m_number_of_photons_per_cm2_at_1m = 0;

    m_need_to_update_tube_spectrum = true;
}


//-----------------------------------------------------------------------
inline bool XRayBeam::addChannel(const RATIONAL_NUMBER& aNumberOfPhotons,
                                 const RATIONAL_NUMBER& anIncidentEnergy)
//-----------------------------------------------------------------------
{
    LOGGER.logWarning(std::string(__FUNCTION__) + 
        " is deprecated. It will be removed in a future release. "
        "Choose addChannelPerPixelAtSDD or addChannelPerCm2At1m. "
        "Note that addChannel and addChannelPerPixelAtSDD are equivalent.") << std::endl;

    return addChannelPerPixelAtSDD(aNumberOfPhotons, anIncidentEnergy);
}


//------------------------------------------------------------------------------------
inline bool XRayBeam::addChannelPerPixelAtSDD(const RATIONAL_NUMBER& aNumberOfPhotons,
                                              const RATIONAL_NUMBER& anIncidentEnergy)
//------------------------------------------------------------------------------------
{
    if (m_photon_count_type == PhotonCountType::PER_CM2_AT_1M)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "Cannot mix photon numbers per pixel at SDD with per cm2 at 1m");
    }

    // Record the photon count type
    m_photon_count_type = PhotonCountType::PER_PIXEL_AT_SDD;

    // Temporary structure
    SpectrumRecord temp;

    // Store the number of photons
    temp.m_photon_number = aNumberOfPhotons;

    // Store the energy
    temp.m_energy = anIncidentEnergy;

    // The tube voltage is not used
    m_need_to_update_tube_spectrum = false;

    // The total energy of the bin is above the cut-off threshold
    if (temp.m_photon_number * temp.m_energy >= m_bin_total_energy_lower_threshold)
    {
            // Store the data
            m_beam_spectrum.push_back(temp);

            // Add the number of photons
            m_number_of_photons_per_pixel_at_SDD += aNumberOfPhotons;

            // Sort the spectrum
            sort();

            return true;
    }

    return false;
}


//---------------------------------------------------------------------------------
inline bool XRayBeam::addChannelPerCm2At1m(const RATIONAL_NUMBER& aNumberOfPhotons,
                                           const RATIONAL_NUMBER& anIncidentEnergy)
//---------------------------------------------------------------------------------
{
    if (m_photon_count_type == PhotonCountType::PER_PIXEL_AT_SDD)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "Cannot mix photon numbers per pixel at SDD with per cm2 at 1m");
    }

    // Record the photon count type
    m_photon_count_type = PhotonCountType::PER_CM2_AT_1M;

    // Temporary structure
    SpectrumRecord temp;

    // Store the number of photons
    temp.m_photon_number = aNumberOfPhotons;

    // Store the energy
    temp.m_energy = anIncidentEnergy;

    // The tube voltage is not used
    m_need_to_update_tube_spectrum = false;

    // The total energy of the bin is above the cut-off threshold
    if (temp.m_photon_number * temp.m_energy >= m_bin_total_energy_lower_threshold)
    {
            // Store the data
            m_beam_spectrum.push_back(temp);

            // Add the number of photons
            m_number_of_photons_per_cm2_at_1m += aNumberOfPhotons;

            // Sort the spectrum
            sort();

            return true;
    }

    return false;
}


//-----------------------------------------------------------------------
inline void XRayBeam::initialise(const RATIONAL_NUMBER& anIncidentEnergy)
//-----------------------------------------------------------------------
{
	// Empty the current beam spectrum
	m_beam_spectrum.clear();

	// Add the record
	m_beam_spectrum.push_back(SpectrumRecord(1, anIncidentEnergy));

    m_photon_count_type = PhotonCountType::PER_PIXEL_AT_SDD;
    m_number_of_photons_per_pixel_at_SDD = 1;
    m_number_of_photons_per_cm2_at_1m = 1;

    m_need_to_update_tube_spectrum = false;
}


//------------------------------------------------------------------------------
inline void XRayBeam::setBinTotalEnergyCutoff(const RATIONAL_NUMBER& aThreshold)
//------------------------------------------------------------------------------
{
		m_bin_total_energy_lower_threshold = aThreshold;
}


//----------------------------------------------------
inline unsigned int XRayBeam::getEnergyChannelNumber()
//----------------------------------------------------
{
    if (m_need_to_update_tube_spectrum) createSpectrumFromVoltage();

		return (m_beam_spectrum.size());
}


//------------------------------------------------------------------------------
inline SpectrumRecord XRayBeam::getEnergyChannel(unsigned int anID,
                                                 const XRayDetector* apDetector)
//------------------------------------------------------------------------------
{
    LOGGER.logWarning(std::string(__FUNCTION__) + 
        " is deprecated. It will be removed in a future release. "
        "Choose getEnergyChannelPerPixelAtSDD or setNumberOfPhotonsPerCm2At1m. "
        "Note that getEnergyChannel and getEnergyChannelPerPixelAtSDD are equivalent.") << std::endl;

	return getEnergyChannelPerPixelAtSDD(anID, apDetector);
}


//-------------------------------------------------------------------------------------------
inline SpectrumRecord XRayBeam::getEnergyChannelPerPixelAtSDD(unsigned int anID,
                                                              const XRayDetector* apDetector)
//-------------------------------------------------------------------------------------------
{
    if (m_need_to_update_tube_spectrum) createSpectrumFromVoltage();

    if (anID >= getEnergyChannelNumber())
    {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    if (m_photon_count_type == PhotonCountType::UNKNOWN)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "Unknown photon count type, it should be either per pixel at SDD or cm2 at 1m");
    }

    SpectrumRecord& record = m_beam_spectrum[anID];
    if (m_photon_count_type == PhotonCountType::PER_PIXEL_AT_SDD)
        return record;
    
    if (!apDetector)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "The detector must be known to convert the number of photons");
    }

    return SpectrumRecord(apDetector->convertPerCm2At1m2PerPixelAtSDD(record.getPhotonNumber()),
        record.getPhotonEnergy());
}


//----------------------------------------------------------------------------------------
inline SpectrumRecord XRayBeam::getEnergyChannelPerCm2At1m(unsigned int anID,
                                                           const XRayDetector* apDetector)
//----------------------------------------------------------------------------------------
{
    if (m_need_to_update_tube_spectrum) createSpectrumFromVoltage();

    if (anID >= getEnergyChannelNumber())
    {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    if (m_photon_count_type == PhotonCountType::UNKNOWN)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "Unknown photon count type, it should be either per pixel at SDD or cm2 at 1m");
    }

    SpectrumRecord& record = m_beam_spectrum[anID];
    if (m_photon_count_type == PhotonCountType::PER_CM2_AT_1M)
        return record;
    
    if (!apDetector)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "The detector must be known to convert the number of photons");
    }

    return SpectrumRecord(apDetector->convertPerPixelAtSDD2PerCm2At1m(record.getPhotonNumber()),
        record.getPhotonEnergy());
}


//-----------------------------------------------
inline RATIONAL_NUMBER XRayBeam::getTotalEnergy()
//-----------------------------------------------
{
    if (m_need_to_update_tube_spectrum) createSpectrumFromVoltage();

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
    LOGGER.logWarning(std::string(__FUNCTION__) + 
        " is deprecated. It will be removed in a future release. "
        "Choose setNumberOfPhotonsPerPixelAtSDD or setNumberOfPhotonsPerCm2At1m. "
        "Note that setNumberOfPhotons and setNumberOfPhotonsPerPixelAtSDD are equivalent.") << std::endl;
    setNumberOfPhotonsPerPixelAtSDD(aNumberOfPhotons);
}


//-------------------------------------------------------------------------------------
inline void XRayBeam::setNumberOfPhotonsPerPixelAtSDD(RATIONAL_NUMBER aNumberOfPhotons)
//-------------------------------------------------------------------------------------
{
    // Update m_number_of_photons_per_pixel_at_SDD if needed
    if (m_need_to_update_tube_spectrum) createSpectrumFromVoltage();

    // Change its value
    m_photon_count_type = PhotonCountType::PER_PIXEL_AT_SDD;

    RATIONAL_NUMBER backup_number_of_photons = m_number_of_photons_per_pixel_at_SDD;
    m_number_of_photons_per_pixel_at_SDD = aNumberOfPhotons;

    // Compute a scaling factor
    RATIONAL_NUMBER old_to_new = m_number_of_photons_per_pixel_at_SDD / backup_number_of_photons;

    // Apply this scaling factor on all the bins
    for (std::vector<SpectrumRecord>::iterator ite_beam_spectrum(m_beam_spectrum.begin());
        ite_beam_spectrum != m_beam_spectrum.end();
        ++ite_beam_spectrum)
    {
        ite_beam_spectrum->m_photon_number *= old_to_new;
    }
}


//----------------------------------------------------------------------------------
inline void XRayBeam::setNumberOfPhotonsPerCm2At1m(RATIONAL_NUMBER aNumberOfPhotons)
//----------------------------------------------------------------------------------
{
    // Update m_number_of_photons_per_cm2_at_1m if needed
    if (m_need_to_update_tube_spectrum) createSpectrumFromVoltage();

    // Change its value
    m_photon_count_type = PhotonCountType::PER_CM2_AT_1M;

    RATIONAL_NUMBER backup_number_of_photons = m_number_of_photons_per_cm2_at_1m;
    m_number_of_photons_per_cm2_at_1m = aNumberOfPhotons;

    // Compute a scaling factor
    RATIONAL_NUMBER old_to_new = m_number_of_photons_per_cm2_at_1m / backup_number_of_photons;

    // Apply this scaling factor on all the bins
    for (std::vector<SpectrumRecord>::iterator ite_beam_spectrum(m_beam_spectrum.begin());
        ite_beam_spectrum != m_beam_spectrum.end();
        ++ite_beam_spectrum)
    {
        ite_beam_spectrum->m_photon_number *= old_to_new;
    }
}


//---------------------------------------------------------------------------------
inline RATIONAL_NUMBER XRayBeam::getNumberOfPhotons(const XRayDetector* apDetector)
//---------------------------------------------------------------------------------
{
    LOGGER.logWarning(std::string(__FUNCTION__) + 
        " is deprecated. It will be removed in a future release. "
        "Choose getNumberOfPhotonsPerPixelAtSDD or getNumberOfPhotonsPerCm2At1m. "
        "Note that getNumberOfPhotons and getNumberOfPhotonsPerPixelAtSDD are equivalent.") << std::endl;
    return getNumberOfPhotonsPerPixelAtSDD(apDetector);
}


//----------------------------------------------------------------------------------------------
inline RATIONAL_NUMBER XRayBeam::getNumberOfPhotonsPerPixelAtSDD(const XRayDetector* apDetector)
//----------------------------------------------------------------------------------------------
{
    if (m_need_to_update_tube_spectrum) createSpectrumFromVoltage();

    switch (m_photon_count_type)
    {
        case PhotonCountType::PER_PIXEL_AT_SDD:
            return m_number_of_photons_per_pixel_at_SDD;
        
        case PhotonCountType::PER_CM2_AT_1M:
            {
                // Convert the number of photons per cm2 at 1m into
                // the number of photons per pixel at source-detector distance
                if (!apDetector)
                {
                    throw Exception(__FILE__, __FUNCTION__, __LINE__,
                        "The detector must be known to convert the number of photons");
                }

                m_number_of_photons_per_pixel_at_SDD = apDetector->convertPerCm2At1m2PerPixelAtSDD(m_number_of_photons_per_cm2_at_1m);
            }
            return m_number_of_photons_per_pixel_at_SDD;

        case PhotonCountType::UNKNOWN:
            return 0;

        default:
            return 0;
    }
}


//-------------------------------------------------------------------------------------------
inline RATIONAL_NUMBER XRayBeam::getNumberOfPhotonsPerCm2At1m(const XRayDetector* apDetector)
//-------------------------------------------------------------------------------------------
{
    if (m_need_to_update_tube_spectrum) createSpectrumFromVoltage();

    switch (m_photon_count_type)
    {
        case PhotonCountType::PER_PIXEL_AT_SDD:
            {
                // Convert the number of photons per pixel at source-detector distance into
                // the number of photons per cm2 at 1m into
                if (!apDetector)
                {
                    throw Exception(__FILE__, __FUNCTION__, __LINE__,
                        "The detector must be known to convert the number of photons");
                }

                m_number_of_photons_per_cm2_at_1m = apDetector->convertPerPixelAtSDD2PerCm2At1m(m_number_of_photons_per_pixel_at_SDD);
            }
            return m_number_of_photons_per_cm2_at_1m;
        
        case PhotonCountType::PER_CM2_AT_1M:
            return m_number_of_photons_per_cm2_at_1m;

        case PhotonCountType::UNKNOWN:
            return 0;

        default:
            return 0;
    }
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


//------------------------------------------------------------
inline unsigned int XRayBeam::getNumberOfPhotonCountingBands()
//------------------------------------------------------------
{
    if (m_need_to_update_tube_spectrum) createSpectrumFromVoltage();

    return m_beam_spectrum_for_photon_count_detector.size();
}


//------------------------------------------------------------------
inline void XRayBeam::activatePhotonCountingBand(unsigned int aBand)
//------------------------------------------------------------------
{
    if (m_need_to_update_tube_spectrum) createSpectrumFromVoltage();

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
        m_number_of_photons_per_pixel_at_SDD = 0;
        m_number_of_photons_per_cm2_at_1m = 0;

        if (m_photon_count_type == PhotonCountType::UNKNOWN)
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "Unknown photon count type, it should be either per pixel at SDD or cm2 at 1m");
        }

        for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_beam_spectrum.begin());
            ite_beam_spectrum != m_beam_spectrum.end();
            ++ite_beam_spectrum)
        {
            if (m_photon_count_type == PhotonCountType::PER_PIXEL_AT_SDD)
                m_number_of_photons_per_pixel_at_SDD += ite_beam_spectrum->m_photon_number;
            else if (m_photon_count_type == PhotonCountType::PER_CM2_AT_1M)
                m_number_of_photons_per_cm2_at_1m += ite_beam_spectrum->m_photon_number;
        }
    }
}


//---------------------------------------------------------------------------
inline const std::vector<RATIONAL_NUMBER>& XRayBeam::getPhotonCountingBands()
//---------------------------------------------------------------------------
{
    if (m_need_to_update_tube_spectrum) createSpectrumFromVoltage();

    return m_photon_count_bands;
}


//---------------------------------------------------------------
inline void XRayBeam::setVoltage(const RATIONAL_NUMBER& aVoltage)
//---------------------------------------------------------------
{
    m_voltage = aVoltage;
    m_need_to_update_tube_spectrum = true;

#ifdef USE_PYTHON
#ifndef HAS_PythonLibs
    LOGGER.logWarning("PythonLibs support was disabled when the library was compiled. This function ") <<
        __FUNCTION__ << " in " << __FILE__ << " may have no visible effect." << std::endl;
#endif
#endif
}


//-------------------------------------------------
inline RATIONAL_NUMBER XRayBeam::getVoltage() const
//-------------------------------------------------
{
    return m_voltage;
}


//---------------------------------------------------------
inline void XRayBeam::setmAs(const RATIONAL_NUMBER& an_mAs)
//---------------------------------------------------------
{
    m_mAs = an_mAs;
    m_need_to_update_tube_spectrum = true;

#ifdef USE_PYTHON
#ifndef HAS_PythonLibs
    LOGGER.logWarning("PythonLibs support was disabled when the library was compiled. This function ") <<
        __FUNCTION__ << " in " << __FILE__ << " may have no visible effect." << std::endl;
#endif
#endif
}


//---------------------------------------------
inline RATIONAL_NUMBER XRayBeam::getmAs() const
//---------------------------------------------
{
    return m_mAs;
}


//----------------------------------------------------------------
inline void XRayBeam::setTubeAngle(const RATIONAL_NUMBER& anAngle)
//----------------------------------------------------------------
{
    m_tube_angle_in_degrees = m_tube_angle_in_degrees;
    m_need_to_update_tube_spectrum = true;

#ifdef USE_PYTHON
#ifndef HAS_PythonLibs
    LOGGER.logWarning("PythonLibs support was disabled when the library was compiled. This function ") <<
        __FUNCTION__ << " in " << __FILE__ << " may have no visible effect." << std::endl;
#endif
#endif
}


//---------------------------------------------------
inline RATIONAL_NUMBER XRayBeam::getTubeAngle() const
//---------------------------------------------------
{
    return m_tube_angle_in_degrees;
}


//-------------------------------------
inline void XRayBeam::clearFiltration()
//-------------------------------------
{
    m_filtration.clear();
    m_need_to_update_tube_spectrum = true;

#ifdef USE_PYTHON
#ifndef HAS_PythonLibs
    LOGGER.logWarning("PythonLibs support was disabled when the library was compiled. This function ") <<
        __FUNCTION__ << " in " << __FILE__ << " may have no visible effect." << std::endl;
#endif
#endif
}


//--------------------------------------------------------------------------------------------------------------
inline void XRayBeam::setFiltration(const std::vector<std::pair<unsigned int, RATIONAL_NUMBER> >& aSetOfFilters)
//--------------------------------------------------------------------------------------------------------------
{
    m_filtration = aSetOfFilters;
    m_need_to_update_tube_spectrum = true;

#ifdef USE_PYTHON
#ifndef HAS_PythonLibs
    LOGGER.logWarning("PythonLibs support was disabled when the library was compiled. This function ") <<
        __FUNCTION__ << " in " << __FILE__ << " may have no visible effect." << std::endl;
#endif
#endif
}


//-----------------------------------------------------------
inline void XRayBeam::addFilter(const std::string& aMaterial,
                                const double& aThickness)
//-----------------------------------------------------------
{
    m_filtration.push_back(
        std::pair<unsigned int, RATIONAL_NUMBER>(
            ElementSet::getInstance().getElement(aMaterial).getZ(),
            aThickness
        )
    );
    m_need_to_update_tube_spectrum = true;

#ifdef USE_PYTHON
#ifndef HAS_PythonLibs
    LOGGER.logWarning("PythonLibs support was disabled when the library was compiled. This function ") <<
        __FUNCTION__ << " in " << __FILE__ << " may have no visible effect." << std::endl;
#endif
#endif
}


//-------------------------------------------------------
inline void XRayBeam::addFilter(unsigned int aMaterial,
                                const double& aThickness)
//-------------------------------------------------------
{
    m_filtration.push_back(std::pair<unsigned int, RATIONAL_NUMBER>(aMaterial, aThickness));
    m_need_to_update_tube_spectrum = true;
    
#ifdef USE_PYTHON
#ifndef HAS_PythonLibs
    LOGGER.logWarning("PythonLibs support was disabled when the library was compiled. This function ") <<
        __FUNCTION__ << " in " << __FILE__ << " may have no visible effect." << std::endl;
#endif
#endif
}


//--------------------------------------------------------------------------------------------------
inline const std::vector<std::pair<unsigned int, RATIONAL_NUMBER> >& XRayBeam::getFiltration() const
//--------------------------------------------------------------------------------------------------
{
    return m_filtration;
}


//-------------------------------
inline bool XRayBeam::hasSpekpy()
//-------------------------------
{
    return PythonSingleton::getInstance().hasSpekpy();
}


//--------------------------------
inline bool XRayBeam::hasXpecgen()
//--------------------------------
{
    return PythonSingleton::getInstance().hasXpecgen();
}


//-------------------------------
inline void XRayBeam::useSpekpy()
//-------------------------------
{
#ifdef USE_PYTHON
    if (!m_use_spekpy)
    {
        if (PythonSingleton::getInstance().hasSpekpy())
        {
            LOGGER.logNow("Use Spekpy to generate beam spectrums from tube voltages.") << std::endl;
            m_use_spekpy = true;
            m_use_xpecgen = false;
            m_need_to_update_tube_spectrum = true;
        }
        else
        {
            LOGGER.logWarning("Spekpy is not installed, try Xpecgen instead.") << std::endl;
            m_use_spekpy = false;

            if (PythonSingleton::getInstance().hasXpecgen())
            {
                if (!m_use_xpecgen)
                {
                    LOGGER.logNow("Use Xpecgen to generate beam spectrums from tube voltages.") << std::endl;
                    m_use_xpecgen = true;
                    m_need_to_update_tube_spectrum = true;
                }
            }
            else
            {
                LOGGER.logWarning("Xpecgen is not installed either.") << std::endl;
                m_use_xpecgen = false;
            }
        }
    }
#endif
}


//--------------------------------
inline void XRayBeam::useXpecgen()
//--------------------------------
{
#ifdef USE_PYTHON
    if (!m_use_xpecgen)
    {
        if (PythonSingleton::getInstance().hasXpecgen())
        {
            LOGGER.logNow("Use Xpecgen to generate beam spectrums from tube voltages.") << std::endl;
            m_use_xpecgen = true;
            m_use_spekpy = false;
            m_need_to_update_tube_spectrum = true;
        }
        else
        {
            LOGGER.logWarning("Xpecgen is not installed, try Spekpy instead.") << std::endl;
            m_use_xpecgen = false;

            if (PythonSingleton::getInstance().hasSpekpy())
            {
                if (!m_use_spekpy)
                {
                    LOGGER.logNow("Use Spekpy to generate beam spectrums from tube voltages.") << std::endl;
                    m_use_spekpy = true;
                    m_need_to_update_tube_spectrum = true;
                }
            }
            else
            {
                LOGGER.logWarning("Spekpy is not installed either.") << std::endl;
                m_use_spekpy = false;
            }
        }
    }
#endif
}


//---------------------------------------
inline bool XRayBeam::usingSpekpy() const
//---------------------------------------
{
    return m_use_spekpy;
}


//----------------------------------------
inline bool XRayBeam::usingXpecgen() const
//----------------------------------------
{
    return m_use_xpecgen;
}



} // namespace gVirtualXRay
