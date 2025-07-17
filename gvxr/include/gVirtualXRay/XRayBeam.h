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
*   @todo       Make sure the filtration functionality works with photon counting
*               detectors.
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

#ifndef __XRayDetector_h
#include "gVirtualXRay/XRayDetector.h"
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
    /// Remove all the energy channels from the beam and all its parameters.
    //--------------------------------------------------------------------------
    void clear();


    //--------------------------------------------------------------------------
    /// Remove all the energy channels from the beam.
    //--------------------------------------------------------------------------
    void clearSpectrumOnly();


    //--------------------------------------------------------------------------
    /// Add an energy channel to the beam.
    /**
     *  @param aNumberOfPhotons:    the number of photons in this energy channel per pixel at SDD
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
    /// Add an energy channel to the beam.
    /**
     *  @param aNumberOfPhotons:    the number of photons in this energy channel per pixel at SDD
     *  @param anIncidentEnergy:    the energy of the photons in
     *                              this energy channel
     *  @return true if the channel was added, i.e.
     *          if aNumberOfPhotons * anIncidentEnergy >= m_bin_total_energy_lower_threshold,
     *          false otherwise
     */
    //--------------------------------------------------------------------------
    bool addChannelPerPixelAtSDD(const RATIONAL_NUMBER& aNumberOfPhotons,
                    const RATIONAL_NUMBER& anIncidentEnergy);


    //--------------------------------------------------------------------------
    /// Add an energy channel to the beam.
    /**
     *  @param aNumberOfPhotons:    the number of photons in this energy channel per cm2 at 1m
     *  @param anIncidentEnergy:    the energy of the photons in
     *                              this energy channel
     *  @return true if the channel was added, i.e.
     *          if aNumberOfPhotons * anIncidentEnergy >= m_bin_total_energy_lower_threshold,
     *          false otherwise
     */
    //--------------------------------------------------------------------------
    bool addChannelPerCm2At1m(const RATIONAL_NUMBER& aNumberOfPhotons,
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
    /// Initialise the X-ray beam specturm with a polychromatic spectrum.
    /// The number of photons is given per pixel at the source-detector distance.
    /**
     *  @param aFileName:           name of the file that stores the spectrum
     *  @param aUnit:               unit in which the data is stored,
     *                              e.g. meV, eV, keV, or MeV
     *  @param aMergeChannelFlag:   the number of records to store in a single
     *                              bin (default value: 1)
     */
    //--------------------------------------------------------------------------
    void initialisePerPixelAtSDD(const char* aFileName,
                    const RATIONAL_NUMBER& aUnit,
                    unsigned int aMergeChannelFlag = 1);


    //--------------------------------------------------------------------------
    /// Initialise the X-ray beam specturm with a polychromatic spectrum.
    /// The number of photons is given per cm2 at 1m.
    /**
     *  @param aFileName:           name of the file that stores the spectrum
     *  @param aUnit:               unit in which the data is stored,
     *                              e.g. meV, eV, keV, or MeV
     *  @param aMergeChannelFlag:   the number of records to store in a single
     *                              bin (default value: 1)
     */
    //--------------------------------------------------------------------------
    void initialisePerCm2At1m(const char* aFileName,
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
     *  @param apDetector: the detector to retrieve the pixel pitch and the source-detector distance if needed (default value: nullptr)
     *  @return the energy bin
     */
    //--------------------------------------------------------------------------
    SpectrumRecord getEnergyChannel(unsigned int anID,
        const XRayDetector* apDetector = nullptr);


    //--------------------------------------------------------------------------
    /// Accessor on a given energy bin.
    /**
     *  @param anID:    the ID of the bin
     *  @param apDetector: the detector to retrieve the pixel pitch and the source-detector distance if needed
     *  @return the energy bin
     */
    //--------------------------------------------------------------------------
    SpectrumRecord getEnergyChannelPerPixelAtSDD(unsigned int anID,
        const XRayDetector* apDetector);


    //--------------------------------------------------------------------------
    /// Accessor on a given energy bin.
    /**
     *  @param anID:    the ID of the bin
     *  @param apDetector: the detector to retrieve the pixel pitch and the source-detector distance if needed
     *  @return the energy bin
     */
    //--------------------------------------------------------------------------
    SpectrumRecord getEnergyChannelPerCm2At1m(unsigned int anID,
        const XRayDetector* apDetector);


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
    /// Set the total number of photons per pixel when Poisson noise is enable.
    /**
    *  @param aNumberOfPhotons: the number of photons
    */
    //--------------------------------------------------------------------------
    void setNumberOfPhotonsPerPixelAtSDD(RATIONAL_NUMBER aNumberOfPhotons);


    //--------------------------------------------------------------------------
    /// Set the total number of photons per cm2 at 1m when Poisson noise is enable.
    /**
    *  @param aNumberOfPhotons: the number of photons
    */
    //--------------------------------------------------------------------------
    void setNumberOfPhotonsPerCm2At1m(RATIONAL_NUMBER aNumberOfPhotons);


    //--------------------------------------------------------------------------
    /// Accessor on the total number of photons per pixel when Poisson noise is enable.
    /**
    *  @param apDetector: the detector to retrieve the pixel pitch and the source-detector distance if needed (default value: nullptr)
    *  @return the number of photons
    */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER getNumberOfPhotons(const XRayDetector* apDetector = nullptr);


    //--------------------------------------------------------------------------
    /// Accessor on the total number of photons per pixel when Poisson noise is enable.
    /**
    *  @param apDetector: the detector to retrieve the pixel pitch and the source-detector distance if needed
    *  @return the number of photons
    */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER getNumberOfPhotonsPerPixelAtSDD(const XRayDetector* apDetector);


    //--------------------------------------------------------------------------
    /// Accessor on the total number of photons per cm2 at 1m when Poisson noise is enable.
    /**
    *  @param apDetector: the detector to retrieve the pixel pitch and the source-detector distance if needed
    *  @return the number of photons
    */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER getNumberOfPhotonsPerCm2At1m(const XRayDetector* apDetector);


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
    void setmAs(const RATIONAL_NUMBER& an_mAs);
    void setTubeAngle(const RATIONAL_NUMBER& anAngle);
    RATIONAL_NUMBER getVoltage() const;
    RATIONAL_NUMBER getmAs() const;
    RATIONAL_NUMBER getTubeAngle() const;
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
    typedef enum PhotonCountTypeTag {
        PER_PIXEL_AT_SDD = 0,
        PER_CM2_AT_1M = 1,
        UNKNOWN = 2
    } PhotonCountType;


    

    void createSpectrumFromVoltage();
    std::string getInterpretor() const;


    //--------------------------------------------------------------------------
    /// Initialise the X-ray beam specturm with a polychromatic spectrum.
    /**
     *  @param aFileName:           name of the file that stores the spectrum
     *  @param aUnit:               unit in which the data is stored,
     *                              e.g. meV, eV, keV, or MeV
     *  @param aMergeChannelFlag:   the number of records to store in a single
     *                              bin
     */
    //--------------------------------------------------------------------------
    void loadSpectrumFromFile(const char* aFileName,
                    const RATIONAL_NUMBER& aUnit,
                    unsigned int aMergeChannelFlag);


    void updateTotalNumberOfPhotons(PhotonCountType aPhotonCountType);


    void applyFiltration();
    

    /// Set of photon bins with no filter applied
    std::vector<SpectrumRecord> m_unfiltered_beam_spectrum;
    
    /// Set of photon bins with filters applied
    std::vector<SpectrumRecord> m_current_beam_spectrum;

    /// All bins with a total energy below
    /// m_bin_total_energy_lower_threshold will be ignored
    RATIONAL_NUMBER m_bin_total_energy_lower_threshold;


    bool m_use_poisson_noise;

    PhotonCountType m_photon_count_type;
    RATIONAL_NUMBER m_number_of_photons_per_pixel_at_SDD;
    RATIONAL_NUMBER m_number_of_photons_per_cm2_at_1m;

    /// Set of photon bins
    std::vector<std::vector<SpectrumRecord> > m_beam_spectrum_for_photon_count_detector;
    std::vector<RATIONAL_NUMBER>  m_photon_count_bands;

    RATIONAL_NUMBER m_voltage;
    RATIONAL_NUMBER m_tube_angle_in_degrees;
    RATIONAL_NUMBER m_mAs; // Not currently in use
    std::vector<std::pair<unsigned int, RATIONAL_NUMBER> > m_filtration;
    bool m_use_spekpy;
    bool m_use_xpecgen;
    bool m_need_to_update_tube_spectrum;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "XRayBeam.inl"


#endif // __XRayBeam_h
