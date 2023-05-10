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
*   @file       TestSpectrumSort.cxx
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


//---------------------------------------
TEST(TestSpectrumSort, TestSpectrumSort1)
//---------------------------------------
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


//---------------------------------------
TEST(TestSpectrumSort, TestSpectrumSort2)
//---------------------------------------
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


// //---------------------------------------
// TEST(TestSpectrumSort, TestSpectrumSort3)
// //---------------------------------------
// {
//     resetBeamSpectrum();
//
//     std::ifstream spectrum_data_file("data/55kV.tsv");
//
//     float energy, photon_count;
//
//     std::vector<float> p_energy_set;
//     std::vector<float> p_photon_count_set;
//
//     while (spectrum_data_file >> energy >> photon_count)
//     {
//         if (photon_count > 1e-15)
//         {
//             g_xray_beam.addChannel(photon_count, energy * keV);
//             p_energy_set.push_back(energy * keV);
//             p_photon_count_set.push_back(photon_count);
//         }
//     }
//
//     ASSERT_EQ(p_energy_set.size(), g_xray_beam.getEnergyChannelNumber());
//     ASSERT_EQ(p_photon_count_set.size(), g_xray_beam.getEnergyChannelNumber());
//
//     for (unsigned int i = 0; i < g_xray_beam.getEnergyChannelNumber(); ++i)
//     {
//         ASSERT_FLOAT_EQ(p_energy_set[i], g_xray_beam.getEnergyChannel(i).getPhotonEnergy());
//         ASSERT_FLOAT_EQ(p_photon_count_set[i], g_xray_beam.getEnergyChannel(i).getPhotonNumber());
//     }
//
//     resetBeamSpectrum();
// }
//
//
// //---------------------------------------
// TEST(TestSpectrumSort, TestSpectrumSort4)
// //---------------------------------------
// {
//     resetBeamSpectrum();
//
//     std::ifstream spectrum_data_file("data/55kV.tsv");
//
//     float energy, photon_count;
//
//     std::vector<float> p_energy_set;
//     std::vector<float> p_photon_count_set;
//
//     while (spectrum_data_file >> energy >> photon_count)
//     {
//         if (photon_count > 1e-15)
//         {
//             p_energy_set.push_back(energy * keV);
//             p_photon_count_set.push_back(photon_count);
//         }
//     }
//
//     // Add in revesrse order
//     for (unsigned int i = 0; i < p_energy_set.size(); ++i)
//     {
//         g_xray_beam.addChannel(p_photon_count_set[p_energy_set.size() - i - 1],
//             p_energy_set[p_energy_set.size() - i - 1]);
//     }
//
//     ASSERT_EQ(p_energy_set.size(), g_xray_beam.getEnergyChannelNumber());
//     ASSERT_EQ(p_photon_count_set.size(), g_xray_beam.getEnergyChannelNumber());
//
//     for (unsigned int i = 0; i < g_xray_beam.getEnergyChannelNumber(); ++i)
//     {
//         ASSERT_FLOAT_EQ(p_energy_set[i], g_xray_beam.getEnergyChannel(i).getPhotonEnergy());
//         ASSERT_FLOAT_EQ(p_photon_count_set[i], g_xray_beam.getEnergyChannel(i).getPhotonNumber());
//     }
//
//     resetBeamSpectrum();
// }
//
//
// //-----------------------------------------------
// TEST(TestSpectrumSort, CreatePhotonCountingBands)
// //-----------------------------------------------
// {
//     resetBeamSpectrum();
//
//     int number_of_bins = 12;
//     int number_of_bands = 4;
//
//     for (unsigned int i = 0; i < number_of_bins; ++i)
//     {
//         g_xray_beam.addChannel(i * 10 + 1, i * keV);
//     }
//     ASSERT_EQ(number_of_bins, g_xray_beam.getEnergyChannelNumber());
//
//     g_xray_beam.setPhotonCountingBands(0 * keV, number_of_bins * keV, number_of_bands);
//     ASSERT_EQ(number_of_bands, g_xray_beam.getNumberOfPhotonCountingBands());
//
//     for (unsigned int i = 0; i < g_xray_beam.getNumberOfPhotonCountingBands(); ++i)
//     {
//         g_xray_beam.activatePhotonCountingBand(i);
//
//         for (int j = 0; j < g_xray_beam.getEnergyChannelNumber(); ++j)
//         {
//             ASSERT_FLOAT_EQ(g_xray_beam.getEnergyChannel(j).getPhotonEnergy() / keV, i * number_of_bins / number_of_bands + j);
//             ASSERT_EQ(number_of_bins / number_of_bands, g_xray_beam.getEnergyChannelNumber());
//         }
//     }
//
//     resetBeamSpectrum();
// }
//
//
// //----------------------------------------------
// TEST(TestSpectrumSort, MergePhotonCountingBands)
// //----------------------------------------------
// {
//     resetBeamSpectrum();
//
//     int number_of_bins = 12;
//     int number_of_bands = 4;
//
//     for (unsigned int i = 0; i < number_of_bins; ++i)
//     {
//         g_xray_beam.addChannel(i * 10 + 1, i * keV);
//     }
//     g_xray_beam.setPhotonCountingBands(0 * keV, number_of_bins * keV, number_of_bands);
//
//     g_xray_beam.mergePhotonCountingBands();
//     ASSERT_EQ(0, g_xray_beam.getNumberOfPhotonCountingBands());
//     ASSERT_EQ(number_of_bins, g_xray_beam.getEnergyChannelNumber());
//
//     for (unsigned int i = 0; i < g_xray_beam.getEnergyChannelNumber(); ++i)
//     {
//         ASSERT_FLOAT_EQ(g_xray_beam.getEnergyChannel(i).getPhotonEnergy() / keV, i);
//         ASSERT_FLOAT_EQ(g_xray_beam.getEnergyChannel(i).getPhotonNumber(), i * 10 + 1);
//     }
//
//     resetBeamSpectrum();
// }
