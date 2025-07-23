/*

Copyright (c) 2023, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*   @file       TestSpectrum.cxx
*
*   @brief      Unit testing: Make sure the spectrum is sorted in asending energy.
*   			The unit test corresponds to validation_09,
*   			last date of check: Passed test on 10 May 2023.
*
*   @version    1.0
*
*   @date       10/05/2023
*   @author     Dr Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, May 2023, 2023, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include "gtest/gtest.h"
#include <vector>
#include <fstream>

#ifndef __SimpleGVXR_h
#include "SimpleGVXR.h"
#endif


//-----------------------------------
TEST(TestSpectrum, TestSpectrumSort1)
//-----------------------------------
{
    resetBeamSpectrum();

    float p_energy_set[] = {80, 81, 81};

    addEnergyBinToSpectrum(p_energy_set[0], "keV", 1);
    addEnergyBinToSpectrum(p_energy_set[1], "keV", 10);
    addEnergyBinToSpectrum(p_energy_set[2], "keV", 5);

    std::vector<double> p_energy_bins = getEnergyBins("keV");

    for (unsigned int i = 0; i < p_energy_bins.size(); ++i)
    {
        ASSERT_FLOAT_EQ(p_energy_set[i], p_energy_bins[i]);
    }

    resetBeamSpectrum();
}


//-----------------------------------
TEST(TestSpectrum, TestSpectrumSort2)
//-----------------------------------
{
    float p_energy_set[] = {80, 81, 81};

    addEnergyBinToSpectrum(p_energy_set[2], "keV", 1);
    addEnergyBinToSpectrum(p_energy_set[1], "keV", 10);
    addEnergyBinToSpectrum(p_energy_set[0], "keV", 5);

    std::vector<double> p_energy_bins = getEnergyBins("keV");

    for (unsigned int i = 0; i < p_energy_bins.size(); ++i)
    {
        ASSERT_FLOAT_EQ(p_energy_set[i], p_energy_bins[i]);
    }

    resetBeamSpectrum();
}


//-----------------------------------
TEST(TestSpectrum, TestSpectrumSort3)
//-----------------------------------
{
    resetBeamSpectrum();

    std::ifstream spectrum_data_file("data/55kV.tsv");

    float energy, photon_count;

    std::vector<float> p_energy_set1;
    std::vector<float> p_photon_count_set1;

    // *** Add in the same order ***
    while (spectrum_data_file >> energy >> photon_count)
    {
        if (photon_count * energy * getUnitOfEnergy("keV") >= 1 * getUnitOfEnergy("eV"))
        {
            addEnergyBinToSpectrum(energy, "keV", photon_count);
            p_energy_set1.push_back(energy);
            p_photon_count_set1.push_back(photon_count);
        }
    }

    std::vector<double> p_energy_set2 = getEnergyBins("keV");
    std::vector<double> p_photon_count_set2 = getPhotonCountEnergyBins();

    ASSERT_EQ(p_energy_set1.size(), p_energy_set2.size());
    ASSERT_EQ(p_photon_count_set1.size(), p_photon_count_set2.size());

    for (unsigned int i = 0; i < p_energy_set1.size(); ++i)
    {
        ASSERT_FLOAT_EQ(p_energy_set1[i], p_energy_set2[i]);
        ASSERT_FLOAT_EQ(p_photon_count_set1[i], p_photon_count_set2[i]);
    }

    resetBeamSpectrum();
}


//-----------------------------------
TEST(TestSpectrum, TestSpectrumSort4)
//-----------------------------------
{
    resetBeamSpectrum();

    std::ifstream spectrum_data_file("data/55kV.tsv");

    float energy, photon_count;

    std::vector<float> p_energy_set1;
    std::vector<float> p_photon_count_set1;

    while (spectrum_data_file >> energy >> photon_count)
    {
        if (photon_count * energy * getUnitOfEnergy("keV") >= 1 * getUnitOfEnergy("eV"))
        {
            p_energy_set1.push_back(energy);
            p_photon_count_set1.push_back(photon_count);
        }
    }

    // *** Add in revesrse order ***
    for (unsigned int i = 0; i < p_energy_set1.size(); ++i)
    {
        addEnergyBinToSpectrum(p_energy_set1[p_energy_set1.size() - i - 1], "keV",
            p_photon_count_set1[p_photon_count_set1.size() - i - 1] );
    }

    std::vector<double> p_energy_set2 = getEnergyBins("keV");
    std::vector<double> p_photon_count_set2 = getPhotonCountEnergyBins();

    ASSERT_EQ(p_energy_set1.size(), p_energy_set2.size());
    ASSERT_EQ(p_photon_count_set1.size(), p_photon_count_set2.size());

    for (unsigned int i = 0; i < p_energy_set1.size(); ++i)
    {
        ASSERT_FLOAT_EQ(p_energy_set1[i], p_energy_set2[i]);
        ASSERT_FLOAT_EQ(p_photon_count_set1[i], p_photon_count_set2[i]);
    }

    resetBeamSpectrum();
}


//------------------------------------------
TEST(TestSpectrum, MergePhotonCountingBands)
//------------------------------------------
{
    resetBeamSpectrum();

    int number_of_bins = 12;
    int number_of_bands = 4;

    for (unsigned int i = 0; i < number_of_bins; ++i)
    {
        addEnergyBinToSpectrum(i + 1, "keV", i * 10 + 1);
    }
    setPhotonCountingBands(0, number_of_bins+1e-6, "keV", number_of_bands);

    mergePhotonCountingBands();
    std::vector<double> p_energy_set = getEnergyBins("keV");
    std::vector<double> p_photon_count_set = getPhotonCountEnergyBins();

    ASSERT_EQ(0, getNumberOfPhotonCountingBands());
    ASSERT_EQ(number_of_bins, p_energy_set.size());

    for (unsigned int i = 0; i < p_energy_set.size(); ++i)
    {
        ASSERT_FLOAT_EQ(p_energy_set[i], i + 1);
        ASSERT_FLOAT_EQ(p_photon_count_set[i], i * 10 + 1);
    }

    resetBeamSpectrum();
}
