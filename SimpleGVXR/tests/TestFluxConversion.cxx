/*

Copyright (c) 2025-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
UK Research and Innovation, All rights reserved.

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
*   @file       TestFluxConversion.cxx
*
*   @brief      Unit testing: Test if the flux conversion function works.
*
*   @date       14/11/2025
*
*   @author     Prof Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Nov 2025-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <cmath>

#include "gtest/gtest.h"

#ifndef SGVXR_Source_h
#include "source.h"
#endif

#ifndef SGVXR_Detector_h
#include "detector.h"
#endif


//******************************************************************************
//  Global variables
//******************************************************************************
int g_main_window_width(1280 / 2.0);
int g_main_window_height(800 / 2.0);


//******************************************************************************
//  Function declaration
//******************************************************************************


//-------------------------------------------
TEST(TestFluxConversion, SDD1m_PixelArea1cm2)
//-------------------------------------------
{
    // SDD: 1m
    // pixel area: 1cm^2
    // Note: all the values should be the same
    double pixel_area = 1.0;
    setSourcePosition(0.5, 0, 0, "m");
    setDetectorPosition(-0.5, 0, 0, "m");
    setDetectorUpVector(0, 1, 0);
    setDetectorPixelSize(sqrt(pixel_area), sqrt(pixel_area), "cm");
    setDetectorNumberOfPixels(1, 1);

    int number_of_photons_per_pixel_at_SDD;
    int number_of_photons_per_cm2_at_1m;

    // Test monochromatic case
    // Set as per pixel at SDD
    number_of_photons_per_pixel_at_SDD = 10000;
    setMonoChromaticPerPixelAtSDD(14, "keV", number_of_photons_per_pixel_at_SDD);
    ASSERT_EQ(number_of_photons_per_pixel_at_SDD, getNumberOfPhotonsPerPixelAtSDD());

    number_of_photons_per_cm2_at_1m = getNumberOfPhotonsPerCm2At1m();
    ASSERT_EQ(number_of_photons_per_pixel_at_SDD, number_of_photons_per_cm2_at_1m);

    // Test monochromatic case
    // Set as per cm2 at 1m
    number_of_photons_per_cm2_at_1m = 10000;
    setMonoChromaticPerCm2At1m(14, "keV", number_of_photons_per_cm2_at_1m);
    ASSERT_EQ(number_of_photons_per_cm2_at_1m, getNumberOfPhotonsPerCm2At1m());

    number_of_photons_per_pixel_at_SDD = getNumberOfPhotonsPerPixelAtSDD();
    ASSERT_EQ(number_of_photons_per_pixel_at_SDD, number_of_photons_per_cm2_at_1m);

    // Polychromatic
    setVoltage(100, "kV");

    // Set as per pixel at SDD
    number_of_photons_per_pixel_at_SDD = 10000;
    setNumberOfPhotonsPerPixelAtSDD(number_of_photons_per_pixel_at_SDD);
    ASSERT_EQ(number_of_photons_per_pixel_at_SDD, getNumberOfPhotonsPerPixelAtSDD());

    number_of_photons_per_cm2_at_1m = getNumberOfPhotonsPerCm2At1m();
    ASSERT_EQ(number_of_photons_per_pixel_at_SDD, number_of_photons_per_cm2_at_1m);

    // Set as per cm2 at 1m
    number_of_photons_per_cm2_at_1m = 10000;
    setNumberOfPhotonsPerCm2At1m(number_of_photons_per_cm2_at_1m);
    ASSERT_EQ(number_of_photons_per_cm2_at_1m, getNumberOfPhotonsPerCm2At1m());

    number_of_photons_per_pixel_at_SDD = getNumberOfPhotonsPerPixelAtSDD();
    ASSERT_EQ(number_of_photons_per_pixel_at_SDD, number_of_photons_per_cm2_at_1m);
}


//-------------------------------------------
TEST(TestFluxConversion, SDD1m_PixelArea3cm2)
//-------------------------------------------
{
    // SDD: 1m
    // pixel area: 3cm^2
    // Note: the values per pixel at SDD should be 3x larger than per 1cm2 at 1m
    double pixel_area = 3.0;
    setSourcePosition(0.5, 0, 0, "m");
    setDetectorPosition(-0.5, 0, 0, "m");
    setDetectorUpVector(0, 1, 0);
    setDetectorPixelSize(sqrt(pixel_area), sqrt(pixel_area), "cm");
    setDetectorNumberOfPixels(1, 1);

    int number_of_photons_per_pixel_at_SDD;
    int number_of_photons_per_cm2_at_1m;
    float max_error_in_percent = 0.5 / 100.0;

    // Test monochromatic case
    // Set as per pixel at SDD
    number_of_photons_per_pixel_at_SDD = 10000;
    setMonoChromaticPerPixelAtSDD(14, "keV", number_of_photons_per_pixel_at_SDD);
    ASSERT_NEAR(number_of_photons_per_pixel_at_SDD, getNumberOfPhotonsPerPixelAtSDD(), 2);

    number_of_photons_per_cm2_at_1m = getNumberOfPhotonsPerCm2At1m();
    ASSERT_LT(abs(number_of_photons_per_pixel_at_SDD - round(pixel_area * number_of_photons_per_cm2_at_1m)) / number_of_photons_per_pixel_at_SDD, max_error_in_percent);

    // Test monochromatic case
    // Set as per cm2 at 1m
    number_of_photons_per_cm2_at_1m = 10000;
    setMonoChromaticPerCm2At1m(14, "keV", number_of_photons_per_cm2_at_1m);
    ASSERT_NEAR(number_of_photons_per_cm2_at_1m, getNumberOfPhotonsPerCm2At1m(), 2);

    number_of_photons_per_pixel_at_SDD = getNumberOfPhotonsPerPixelAtSDD();
    ASSERT_LT(abs(round(number_of_photons_per_pixel_at_SDD / pixel_area) - number_of_photons_per_cm2_at_1m) / number_of_photons_per_cm2_at_1m, max_error_in_percent);

    // Polychromatic
    setVoltage(100, "kV");

    // Set as per pixel at SDD
    number_of_photons_per_pixel_at_SDD = 10000;
    setNumberOfPhotonsPerPixelAtSDD(number_of_photons_per_pixel_at_SDD);
    ASSERT_NEAR(number_of_photons_per_pixel_at_SDD, getNumberOfPhotonsPerPixelAtSDD(), 2);

    number_of_photons_per_cm2_at_1m = getNumberOfPhotonsPerCm2At1m();
    ASSERT_NEAR(number_of_photons_per_pixel_at_SDD, round(pixel_area * number_of_photons_per_cm2_at_1m), 2);
    ASSERT_LT(abs(number_of_photons_per_pixel_at_SDD - round(pixel_area * number_of_photons_per_cm2_at_1m)) / number_of_photons_per_pixel_at_SDD, max_error_in_percent);

    // Set as per cm2 at 1m
    number_of_photons_per_cm2_at_1m = 10000;
    setNumberOfPhotonsPerCm2At1m(number_of_photons_per_cm2_at_1m);
    ASSERT_NEAR(number_of_photons_per_cm2_at_1m, getNumberOfPhotonsPerCm2At1m(), 2);

    number_of_photons_per_pixel_at_SDD = getNumberOfPhotonsPerPixelAtSDD();
    ASSERT_LT(abs(round(number_of_photons_per_pixel_at_SDD / pixel_area) - number_of_photons_per_cm2_at_1m) / number_of_photons_per_cm2_at_1m, max_error_in_percent);
}
