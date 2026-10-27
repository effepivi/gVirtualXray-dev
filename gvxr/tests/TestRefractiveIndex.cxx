/*

Copyright (c) 2026 - present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), United Kingdom Research and Innovation, all rights reserved.

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
 *   @file       TestMixture.cxx
 *
 *   @brief      Unit testing: Make sure the code in gvxr/src/Utilities/Mixture.cxx is tested.
/**
 ********************************************************************************
 *
 *   @file       TestRefractiveIndex.cxx
 *
 *   @brief      Unit testing: Make sure the refractive index (delta and beta)
 *               used for phase contrast is consistent with analytical values.
 *
 *   @version    1.0
 *
 *   @date       26/10/2026
 *   @author     Prof Franck P. Vidal
 *
 *   @section    License
 *               BSD 3-Clause License.
 *
 *               For details on use and redistribution please refer
 *               to http://opensource.org/licenses/BSD-3-Clause
 *
 ********************************************************************************
 */


//******************************************************************************
//  Include
//******************************************************************************
#include <cmath>
#include <map>
#include <string>

#include "gtest/gtest.h"

#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef GVXR_PHOTON_CROSS_SECTION_H
#include "gVirtualXRay/PhotonCrossSection.h"
#endif

#ifndef GVXR_MIXTURE_H
#include "gVirtualXRay/Mixture.h"
#endif

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Constants
//******************************************************************************
namespace
{
    const double classical_electron_radius = 2.8179403262e-12 * mm; // r_e
    const double avogadro = 6.02214076e23;                          // 1/mol
    const double pi = 3.14159265358979323846;

    // Free-electron approximation: delta = r_e lambda^2 n_e / (2 pi)
    // where n_e is the electron density in 1/mm3.
    double deltaFromElectronDensity(double anElectronDensity, double anEnergy)
    {
        const double lambda = getWavelength(anEnergy);
        return classical_electron_radius * lambda * lambda * anElectronDensity / (2.0 * pi);
    }
}


//---------------------------------------
TEST(TestRefractiveIndex, TestWavelength)
//---------------------------------------
{
    // 12.398 keV <-> 1 Angstrom = 1e-7 mm
    double wavelength_in_mm = getWavelength(12.3984198 * keV) / mm;
    ASSERT_NEAR(wavelength_in_mm / 1e-7, 1.0, 1e-6);

    // The wavelength is inversely proportional to photon energy
    ASSERT_NEAR(getWavelength(25.0 * keV) / getWavelength(50.0 * keV), 2.0, 1e-12);
    ASSERT_NEAR(getWavelength(50.0 * keV) / getWavelength(100.0 * keV), 2.0, 1e-12);

    // Verify k = 2*pi/lambda for a 1 Angstrom photon (E = 12.3984198 keV).
    ASSERT_NEAR(getWaveNumber(12.3984198 * keV) * 1e-7, 2.0 * pi, 1e-5);
}


//--------------------------------------------------
TEST(TestRefractiveIndex, TestDeltaElementAluminium)
//--------------------------------------------------
{
    // Cross-check X-ray refractive-index decrement against electron-density theory.
    const double Z = 13.0; // atomic number of Al
    const double A = 26.9815385; // molar mass of Al in g/mol

    // Creates a cross-section object for aluminium (atomic number 13).
    PhotonCrossSection aluminium(Z);

    for (double energy : {20.0 * keV, 40.0 * keV, 80.0 * keV})
    {
        // electrons per mm3
        const double electron_density = (2.699) * avogadro * Z / A / 1000.0;

        const double expected = deltaFromElectronDensity(electron_density, energy);
        const double delta = aluminium.getRefractiveIndexDecrement(energy);

        ASSERT_GT(delta, 0.0);
        ASSERT_NEAR(delta / expected, 1.0, 0.03) << "Energy: " << energy / keV << " keV";
    }
}


//-----------------------------------------------
TEST(TestRefractiveIndex, TestDeltaWaterCompound)
//-----------------------------------------------
{
    // Cross-check X-ray refractive-index decrement against electron-density theory.

    // Creates a cross-section object for water (here a compound).
    PhotonCrossSection photon_cross_section;
    photon_cross_section.useCompound("H2O");
    photon_cross_section.setDensity(1.0 * g / cm3);

    // Electrons per mm3 of water at 1 g/cm3 (0.5551 mol of electrons/g ... 3.343e23 / cm3)
    const double electron_density = 3.343e23 / 1000.0;

    for (double energy : {20.0 * keV, 30.0 * keV, 60.0 * keV})
    {
        const double expected = deltaFromElectronDensity(electron_density, energy);
        const double delta = photon_cross_section.getRefractiveIndexDecrement(energy);

        ASSERT_NEAR(delta / expected, 1.0, 0.03) << "Energy: " << energy / keV << " keV";
    }
}

//----------------------------------------------
TEST(TestRefractiveIndex, TestDeltaWaterMixture)
//----------------------------------------------
{
    // Cross-check X-ray refractive-index decrement against electron-density theory.
    std::map<std::string, double> weight_set;
    weight_set["H"] = 0.111894;
    weight_set["O"] = 0.888106;

    // Creates a cross-section object for water (here a mixture).
    Mixture water(weight_set);
    water.setDensity(1.0 * g / cm3);
    PhotonCrossSection photon_cross_section(water);

    // Electrons per mm3 of water at 1 g/cm3 (0.5551 mol of electrons/g ... 3.343e23 / cm3)
    const double electron_density = 3.343e23 / 1000.0;

    for (double energy : {20.0 * keV, 30.0 * keV, 60.0 * keV})
    {
        const double expected = deltaFromElectronDensity(electron_density, energy);
        const double delta = photon_cross_section.getRefractiveIndexDecrement(energy);

        ASSERT_NEAR(delta / expected, 1.0, 0.03) << "Energy: " << energy / keV << " keV";
    }
}


//---------------------------------------------------
TEST(TestRefractiveIndex, TestDeltaScalesWithDensity)
//--------------------------------------------------
{
    // Verify that the refractive-index decrement scales linearly with density.
    std::map<std::string, double> weight_set;
    weight_set["H"] = 0.111894;
    weight_set["O"] = 0.888106;

    Mixture water(weight_set);
    water.setDensity(1.0 * g / cm3);
    const double delta_1 = PhotonCrossSection(water).getRefractiveIndexDecrement(30.0 * keV);

    water.setDensity(2.0 * g / cm3);
    const double delta_2 = PhotonCrossSection(water).getRefractiveIndexDecrement(30.0 * keV);

    ASSERT_NEAR(delta_2 / delta_1, 2.0, 1e-6);
}


//---------------------------------------------------
TEST(TestRefractiveIndex, TestBetaMatchesAttenuation)
//---------------------------------------------------
{
    // Cross-check absorption index against the attenuation coefficient.
    std::map<std::string, double> weight_set;
    weight_set["H"] = 0.111894;
    weight_set["O"] = 0.888106;

    Mixture water(weight_set);
    water.setDensity(1.0 * g / cm3);
    PhotonCrossSection photon_cross_section(water);

    for (double energy : {20.0 * keV, 30.0 * keV, 60.0 * keV})
    {
        // beta = mu lambda / (4 pi), with mu the linear attenuation coefficient
        double mu = water.getMassAttenuationTotal(energy) * water.getDensity();
        double expected = mu * getWavelength(energy) / (4.0 * pi);
        const double beta = photon_cross_section.getAbsorptionIndex(energy);

        ASSERT_GT(beta, 0.0);
        ASSERT_NEAR(beta / expected, 1.0, 0.01) << "Energy: " << energy / keV << " keV";

        // beta = mu lambda / (4 pi), with mu the linear attenuation coefficient
        mu = water.getLinearAttenuationTotal(energy);
        expected = mu * getWavelength(energy) / (4.0 * pi);

        ASSERT_NEAR(beta / expected, 1.0, 0.01) << "Energy: " << energy / keV << " keV";
    }
}
