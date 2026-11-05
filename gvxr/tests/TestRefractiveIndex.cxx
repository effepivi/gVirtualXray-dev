/*

Copyright (c) 2026-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk),
UK Research and Innovation, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the UK Research and Innovation nor the names of its contributors
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
*   Copyright
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk),
*   UK Research and Innovation, Oct 2026-present, version 1.0,
*   BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <cmath>
#include <complex>
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
    const unsigned short Z = 13; // atomic number of Al
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


//--------------------------------------------------
TEST(TestRefractiveIndex, TestBetaScalesWithDensity)
//--------------------------------------------------
{
    // Verify that the absorption index scales linearly with density.
    std::map<std::string, double> weight_set;
    weight_set["H"] = 0.111894;
    weight_set["O"] = 0.888106;

    Mixture water(weight_set);
    water.setDensity(1.0 * g / cm3);
    const double beta_1 = PhotonCrossSection(water).getAbsorptionIndex(30.0 * keV);

    water.setDensity(2.0 * g / cm3);
    const double beta_2 = PhotonCrossSection(water).getAbsorptionIndex(30.0 * keV);

    ASSERT_NEAR(beta_2 / beta_1, 2.0, 1e-6);
}


//-------------------------------------------------
TEST(TestRefractiveIndex, TestBetaElementAluminium)
//-------------------------------------------------
{
    // Cross-check the absorption index of an element against its attenuation
    // coefficient: beta = mu lambda / (4 pi)
    PhotonCrossSection aluminium(13);

    for (double energy : {20.0 * keV, 40.0 * keV, 80.0 * keV})
    {
        const double mu = aluminium.getMassAttenuationCoefficient(energy) * aluminium.getDensity();
        const double expected = mu * getWavelength(energy) / (4.0 * pi);
        const double beta = aluminium.getAbsorptionIndex(energy);

        ASSERT_GT(beta, 0.0);
        ASSERT_NEAR(beta / expected, 1.0, 0.01) << "Energy: " << energy / keV << " keV";
    }
}


//----------------------------------------------------------------
TEST(TestRefractiveIndex, TestDeltaScalesWithInverseEnergySquared)
//----------------------------------------------------------------
{
    // Away from absorption edges, delta is proportional to 1 / E^2.
    PhotonCrossSection aluminium(13);

    for (double energy : {20.0 * keV, 30.0 * keV, 50.0 * keV})
    {
        const double ratio = aluminium.getRefractiveIndexDecrement(energy) /
                             aluminium.getRefractiveIndexDecrement(2.0 * energy);

        ASSERT_NEAR(ratio, 4.0, 0.04) << "Energy: " << energy / keV << " keV";
    }
}


//-------------------------------------------------------
TEST(TestRefractiveIndex, TestDeltaDominatesBetaForWater)
//-------------------------------------------------------
{
    // For light materials at hard X-ray energies, phase effects dominate
    // absorption, i.e. delta >> beta.
    PhotonCrossSection photon_cross_section;
    photon_cross_section.useCompound("H2O");
    photon_cross_section.setDensity(1.0 * g / cm3);

    for (double energy : {30.0 * keV, 60.0 * keV, 100.0 * keV})
    {
        const double delta = photon_cross_section.getRefractiveIndexDecrement(energy);
        const double beta  = photon_cross_section.getAbsorptionIndex(energy);

        ASSERT_GT(beta, 0.0);
        ASSERT_GT(delta / beta, 100.0) << "Energy: " << energy / keV << " keV";
    }
}


//--------------------------------------------------------------------
TEST(TestRefractiveIndex, TestDecrementAndAbsorptionMatchComplexIndex)
//--------------------------------------------------------------------
{
    // n = 1 - delta + i beta
    PhotonCrossSection aluminium(13);

    for (double energy : {20.0 * keV, 60.0 * keV})
    {
        const std::complex<double> n = aluminium.getRefractiveIndex(energy);

        ASSERT_LT(n.real(), 1.0);
        ASSERT_GT(n.imag(), 0.0);
        ASSERT_DOUBLE_EQ(aluminium.getRefractiveIndexDecrement(energy), 1.0 - n.real());
        ASSERT_DOUBLE_EQ(aluminium.getAbsorptionIndex(energy), n.imag());
    }
}


//---------------------------------------------------------------
TEST(TestRefractiveIndex, TestElementMatchesSingleElementMixture)
//---------------------------------------------------------------
{
    // An element and a mixture made of the same single element must agree.
    PhotonCrossSection element(13);

    PhotonCrossSection mixture(std::map<int, double>{{13, 1.0}});
    mixture.setDensity(element.getDensity());

    for (double energy : {20.0 * keV, 40.0 * keV, 80.0 * keV})
    {
        ASSERT_NEAR(mixture.getRefractiveIndexDecrement(energy) /
                    element.getRefractiveIndexDecrement(energy), 1.0, 1e-3)
            << "Energy: " << energy / keV << " keV";

        ASSERT_NEAR(mixture.getAbsorptionIndex(energy) /
                    element.getAbsorptionIndex(energy), 1.0, 1e-3)
            << "Energy: " << energy / keV << " keV";
    }
}


//---------------------------------------------------
TEST(TestRefractiveIndex, TestCompoundMatchesMixture)
//---------------------------------------------------
{
    // Water defined as a compound or as a mixture must agree.
    PhotonCrossSection compound;
    compound.useCompound("H2O");
    compound.setDensity(1.0 * g / cm3);

    std::map<std::string, double> weight_set;
    weight_set["H"] = 0.111894;
    weight_set["O"] = 0.888106;

    Mixture water(weight_set);
    water.setDensity(1.0 * g / cm3);
    PhotonCrossSection mixture(water);

    for (double energy : {20.0 * keV, 30.0 * keV, 60.0 * keV})
    {
        ASSERT_NEAR(mixture.getRefractiveIndexDecrement(energy) /
                    compound.getRefractiveIndexDecrement(energy), 1.0, 1e-3)
            << "Energy: " << energy / keV << " keV";

        ASSERT_NEAR(mixture.getAbsorptionIndex(energy) /
                    compound.getAbsorptionIndex(energy), 1.0, 1e-3)
            << "Energy: " << energy / keV << " keV";
    }
}


//------------------------------------------------
TEST(TestRefractiveIndex, TestHounsfieldUnitWater)
//------------------------------------------------
{
    // 0 HU is water, whose delta must be close to the one of water.
    PhotonCrossSection hu;
    hu.useHU(0);

    PhotonCrossSection water;
    water.useCompound("H2O");
    water.setDensity(1.0 * g / cm3);

    for (double energy : {30.0 * keV, 60.0 * keV})
    {
        ASSERT_GT(hu.getRefractiveIndexDecrement(energy), 0.0);
        ASSERT_GT(hu.getAbsorptionIndex(energy), 0.0);

        ASSERT_NEAR(hu.getRefractiveIndexDecrement(energy) /
                    water.getRefractiveIndexDecrement(energy), 1.0, 0.05)
            << "Energy: " << energy / keV << " keV";

        ASSERT_NEAR(hu.getAbsorptionIndex(energy) /
                    water.getAbsorptionIndex(energy), 1.0, 0.1)
            << "Energy: " << energy / keV << " keV";
    }
}


//--------------------------------------------------------
TEST(TestRefractiveIndex, TestHounsfieldUnitDensityIsUsed)
//--------------------------------------------------------
{
    // The refractive index of a HU material follows the density that
    // corresponds to its HU value, and the user-defined one if any.
    PhotonCrossSection air;
    air.useHU(-1000);

    PhotonCrossSection water;
    water.useHU(0);

    PhotonCrossSection bone;
    bone.useHU(1000);

    const double energy = 60.0 * keV;

    ASSERT_GT(air.getRefractiveIndexDecrement(energy), 0.0);
    ASSERT_LT(air.getRefractiveIndexDecrement(energy),
              water.getRefractiveIndexDecrement(energy) / 100.0);
    ASSERT_GT(bone.getRefractiveIndexDecrement(energy),
              water.getRefractiveIndexDecrement(energy));

    // Override the density
    const double delta_1 = water.getRefractiveIndexDecrement(energy);
    water.setDensity(2.0 * water.getDensity());
    ASSERT_NEAR(water.getRefractiveIndexDecrement(energy) / delta_1, 2.0, 1e-6);
}


//--------------------------------------------------------
TEST(TestRefractiveIndex, TestNoPropertiesThrowsException)
//--------------------------------------------------------
{
    // Nothing has been set: it is impossible to compute the refractive index.
    PhotonCrossSection photon_cross_section;

    ASSERT_THROW((void) photon_cross_section.getRefractiveIndexDecrement(30.0 * keV), Exception);
    ASSERT_THROW((void) photon_cross_section.getAbsorptionIndex(30.0 * keV), Exception);
}


//----------------------------------------------------------------------
TEST(TestRefractiveIndex, TestAttenuationCoefficientOnlyThrowsException)
//----------------------------------------------------------------------
{
    // The refractive index cannot be derived from a user-defined attenuation
    // coefficient: the chemical composition is unknown.
    PhotonCrossSection photon_cross_section;
    photon_cross_section.useLinearAttenuationCoefficient(0.2 / cm);

    ASSERT_THROW((void) photon_cross_section.getRefractiveIndexDecrement(30.0 * keV), Exception);
    ASSERT_THROW((void) photon_cross_section.getAbsorptionIndex(30.0 * keV), Exception);
}


//---------------------------------------------------------
TEST(TestRefractiveIndex, TestInvalidEnergyThrowsException)
//---------------------------------------------------------
{
    PhotonCrossSection aluminium(13);

    ASSERT_THROW((void) aluminium.getRefractiveIndexDecrement(0.0 * keV), Exception);
    ASSERT_THROW((void) aluminium.getAbsorptionIndex(-10.0 * keV), Exception);
}
