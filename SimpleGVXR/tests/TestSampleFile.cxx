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
*   @file       TestSampleFile.cxx
*
*   @brief      Unit testing: Test the functions in ../src/sample.cxx.
*
*   @date       27/04/2026
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
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Apr 2026-present, version 1.1,
*   BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <cmath>
#include <string>

#include "SimpleGVXR.h"
#include "gtest/gtest.h"

#ifndef SGVXR_Sample_h
#include "sample.h"
#endif


//******************************************************************************
//  Global variables
//******************************************************************************


//******************************************************************************
//  Function declaration
//******************************************************************************




//------------------------------------
TEST(TestSampleFile, elementMolarMass)
//------------------------------------
{
    // Create the sample
    emptyMesh("sample_element");

    setElement("sample_element", "Si");
    ASSERT_NEAR(getElementMolarMass(("Si")), 28.085, 1e-3);
    ASSERT_NEAR(getEffectiveMolarMass(("sample_element")), 28.085, 1e-3);

    setElement("sample_element", "Fe");
    ASSERT_NEAR(getElementMolarMass(("Fe")), 55.845, 1e-3);
    ASSERT_NEAR(getEffectiveMolarMass(("sample_element")), 55.845, 1e-3);
}


//----------------------------------------------
TEST(TestSampleFile, compoundEffectiveMolarMass)
//----------------------------------------------
{
    // Create the sample
    emptyMesh("sample_compound");

    setCompound("sample_compound", "H2O");
    ASSERT_NEAR(getEffectiveMolarMass(("sample_compound")), 18.015, 1e-3);

    setCompound("sample_compound", "SiC");
    ASSERT_NEAR(getEffectiveMolarMass(("sample_compound")), 40.096, 1e-3);
}


//---------------------------------------
TEST(TestSampleFile, elementAtomicNumber)
//---------------------------------------
{
    // Create the sample
    emptyMesh("sample_element");

    setElement("sample_element", "Si");
    ASSERT_EQ(getElementAtomicNumber(("Si")), 14);
    ASSERT_EQ(std::round(getEffectiveAtomicNumber(("sample_element"))), 14);

    setElement("sample_element", "Fe");
    ASSERT_EQ(getElementAtomicNumber(("Fe")), 26);
    ASSERT_EQ(std::round(getEffectiveAtomicNumber(("sample_element"))), 26);
}


//-------------------------------------------------
TEST(TestSampleFile, compoundEffectiveAtomicNumber)
//-------------------------------------------------
{
    // Create the sample
    emptyMesh("sample_compound");

    setCompound("sample_compound", "H2O");
    float Z_H2O = getEffectiveAtomicNumber(("sample_compound"));

    setCompound("sample_compound", "SiC");
    float Z_SiC = getEffectiveAtomicNumber(("sample_compound"));

    ASSERT_EQ(Z_H2O >= 7.42 && Z_H2O <= 7.5, true);
    ASSERT_EQ(Z_SiC >= 9.5 && Z_SiC <= 10.0, true);
}
//
//
// //------------------------------------------------
// TEST(TestSampleFile, mixtureEffectiveAtomicNumber)
// //------------------------------------------------
// {
//     // Create the sample
//     emptyMesh("sample_mixture");
//
//     setMixture("sample_mixture", "Ti90Al6V4");
//     float Z_Ti90Al6V4 = getEffectiveAtomicNumber(("sample_mixture"));
//
//     ASSERT_NEAR(Z_Ti90Al6V4, 21.5, 1e-2);
// }
