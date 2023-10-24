/*

Copyright (c) 2017, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


#ifndef __XRayBeam_h
#define __XRayBeam_h


/**
********************************************************************************
*
*   @file       XRayBeam.h
*
*   @brief      Class to manage X-Ray beams. The beam spectrum is discretised
*               into energy channels.
*
*   @version    1.0
*
*   @date       13/01/2017
*
*   @author     Dr Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*   http://www.fpvidal.net/, Jan 2017, 2017, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <vector>
#include <string>
#include <iostream>

#ifdef HAS_PythonLibs
#include <Python.h>
#endif


#ifndef __Types_h
#include "gVirtualXRay/Types.h"
#endif

#ifndef __SpectrumRecord_h
#include "gVirtualXRay/SpectrumRecord.h"
#endif


#include <memory>
#include <mutex>

//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  XRayBeam
*   @brief  XRayBeam is a class to handle the X-Ray beam.
*           The beam spectrum is discretised into energy channels.
*/
//==============================================================================
class XRayBeam
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default constructor.
    //--------------------------------------------------------------------------
    XRayBeam();


    //--------------------------------------------------------------------------
    /// Destructor.
    //--------------------------------------------------------------------------
    ~XRayBeam();


    //--------------------------------------------------------------------------
    /// Remove all the energy channels from the beam.
    //--------------------------------------------------------------------------
    void clear();


    //--------------------------------------------------------------------------
    /// Add an energy channel to the beam.
    /**
     *  @param aNumberOfPhotons:    the number of photons in this energy channel
     *  @param anIncidentEnergy:    the energy of the photons in
     *                              this energy channel
     *  @return true if the channel was added, i.e.
     *          if aNumberOfPhotons * anIncidentEnergy >= m_bin_total_energy_lower_threshold,
     *          false otherwise
     */
    //--------------------------------------------------------------------------
    bool addChannel(const RATIONAL_NUMBER& aNumberOfPhotons,
                    const RATIONAL_NUMBER& anIncidentEnergy);


    //--------------------------------------------------------------------------
    /// Initialise the X-ray beam specturm with a polychromatic spectrum.
    /**
     *  @param aFileName:           name of the file that stores the spectrum
     *  @param aUnit:               unit in which the data is stored,
     *                              e.g. meV, eV, keV, or MeV
     *  @param aMergeChannelFlag:   the number of records to store in a single
     *                              bin (default value: 1)
     */
    //--------------------------------------------------------------------------
    void initialise(const char* aFileName,
                    const RATIONAL_NUMBER& aUnit,
                    unsigned int aMergeChannelFlag = 1);


    //--------------------------------------------------------------------------
    /// Initialise the X-ray beam specturm with a monochromatic spectrum.
    /**
     *  @param anIncidentEnergy:    the energy of these photons
     */
    //--------------------------------------------------------------------------
    void initialise(const RATIONAL_NUMBER& anIncidentEnergy);


    //--------------------------------------------------------------------------
    /// Normalise the number of photons so that the total incident energy is 1.0.
    //--------------------------------------------------------------------------
    void normalise();


    //--------------------------------------------------------------------------
    /// Set a threshold to discard energy bins with a tiny amount of energy.
    /**
     *  @param aThreshold:  the threshold
     */
    //--------------------------------------------------------------------------
    void setBinTotalEnergyCutoff(const RATIONAL_NUMBER& aThreshold);


    //--------------------------------------------------------------------------
    /// Accessor on the number of bins in the spectrum.
    /**
     *  @return the number of energy bins in the spectrum
     */
    //--------------------------------------------------------------------------
    unsigned int getEnergyChannelNumber();


    //--------------------------------------------------------------------------
    /// Accessor on a given energy bin.
    /**
     *  @param anID:    the ID of the bin
     *  @return the energy bin
     */
    //--------------------------------------------------------------------------
    const SpectrumRecord& getEnergyChannel(unsigned int anID);


    //--------------------------------------------------------------------------
    /// Accessor on a total energy of the beam.
    /**
     *  @return the total energy
     */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER getTotalEnergy();


    //--------------------------------------------------------------------------
    /// Enable Poisson noise for the energy fluence.
    //--------------------------------------------------------------------------
    void enablePoissonNoise();


    //--------------------------------------------------------------------------
    /// Disable Poisson noise for the energy fluence.
    //--------------------------------------------------------------------------
    void disablePoissonNoise();


    //--------------------------------------------------------------------------
    /// Set the total number of photons per pixel when Poisson noise is enable.
    /**
    *  @param aNumberOfPhotons: the number of photons
    */
    //--------------------------------------------------------------------------
    void setNumberOfPhotons(RATIONAL_NUMBER aNumberOfPhotons);


    //--------------------------------------------------------------------------
    /// Accessor on the total number of photons per pixel when Poisson noise is enable.
    /**
    *  @return the number of photons
    */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER getNumberOfPhotons();


    //--------------------------------------------------------------------------
    /// Enable or disable Poisson noise for the energy fluence.
    /**
    *  @param aFlag: if true, then enable Poisson noise. If false, then disable it
    */
    //--------------------------------------------------------------------------
    void usePoissonNoise(bool aFlag);


    //--------------------------------------------------------------------------
    /// Check if Poisson noise for the energy fluence is enable or disable.
    /**
    *  @return true if Poisson noise is enable, or false if Poisson noise is disable
    */
    //--------------------------------------------------------------------------
    bool usePoissonNoise() const;


    //--------------------------------------------------------------------------
    /// Sort the spectrum in ascending energy.
    //--------------------------------------------------------------------------
    void sort();


    void setPhotonCountingBands(RATIONAL_NUMBER aMinEnergy,
        RATIONAL_NUMBER aMaxEnergy,
        unsigned int aNumberOfBands);


    unsigned int getNumberOfPhotonCountingBands();
    void activatePhotonCountingBand(unsigned int aBand);
    void mergePhotonCountingBands();
    const std::vector<RATIONAL_NUMBER>& getPhotonCountingBands();

    void setVoltage(const RATIONAL_NUMBER& aVoltage);
    RATIONAL_NUMBER getVoltage() const;
    void clearFiltration();
    void setFiltration(const std::vector<std::pair<unsigned int, RATIONAL_NUMBER> >& aSetOfFilters);
    void addFilter(const std::string& aMaterial, const double& aThickness);
    void addFilter(unsigned int aMaterial, const double& aThickness);
    const std::vector<std::pair<unsigned int, RATIONAL_NUMBER> >& getFiltration() const;
    bool hasSpekpy();
    bool hasXpecgen();
    void useSpekpy();
    void useXpecgen();
    bool usingSpekpy() const;
    bool usingXpecgen() const;


//******************************************************************************
protected:
    void createSpectrumFromVoltage();
    std::string getInterpretor() const;

    /// Set of photon bins
    std::vector<SpectrumRecord> m_beam_spectrum;


    /// All bins with a total energy below
    /// m_bin_total_energy_lower_threshold will be ignored
    RATIONAL_NUMBER m_bin_total_energy_lower_threshold;


    bool m_use_poisson_noise;


    RATIONAL_NUMBER m_number_of_photons_per_pixel;

    /// Set of photon bins
    std::vector<std::vector<SpectrumRecord> > m_beam_spectrum_for_photon_count_detector;
    std::vector<RATIONAL_NUMBER>  m_photon_count_bands;

    RATIONAL_NUMBER m_voltage;
    std::vector<std::pair<unsigned int, RATIONAL_NUMBER> > m_filtration;
    bool m_use_spekpy;
    bool m_use_xpecgen;
    bool m_need_to_update_tube_spectrum;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "XRayBeam.inl"


#endif // __XRayBeam_h
