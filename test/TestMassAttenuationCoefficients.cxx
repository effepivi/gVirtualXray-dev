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


/**
********************************************************************************
*
*   @file       TestMassAttenuationCoefficients.cxx
*
*   @brief      Unit testing: mass attenuation coefficients (validation_01).
*
*   @version    1.0
*
*   @date       22/03/2017
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
*               http://www.fpvidal.net/, Mar 2017, 2017, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <numeric>

#define BOOST_TEST_MODULE TestMassAttenuationCoefficients
#include <boost/test/unit_test.hpp>

#ifdef HAS_GLEW
#include <GL/glew.h>
#endif

#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef __XRayRenderer_h
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef __InvalidInternalFormalException_h
#include "gVirtualXRay/InvalidInternalFormalException.h"
#endif

#ifndef __InvalidImageSizeException_h
#include "gVirtualXRay/InvalidImageSizeException.h"
#endif

#ifndef __NoDetectorException_h
#include "gVirtualXRay/NoDetectorException.h"
#endif

#ifndef __NoBeamException_h
#include "gVirtualXRay/NoBeamException.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;
//using namespace std;


//******************************************************************************
//  Function declaration
//******************************************************************************
void runTest(const char* aFileName, short anHounsfieldValue);
double getSum(const std::vector<double>& aVector);
double getAverage(const std::vector<double>& aVector);
double getVariance(const std::vector<double>& aVector);
double getStdDev(const std::vector<double>& aVector);
double getNCC(const std::vector<double>& aVector1, const std::vector<double>& aVector2);


//******************************************************************************
//  Const
//******************************************************************************
const short HU_bone(1330);
const short HU_brain(37);
const short HU_gas(-998);
const short HU_soft_tissue(52);
const short HU_water(0);


//-------------------------------------------------------
BOOST_AUTO_TEST_CASE(TestBoneMassAttenuationCoefficients)
//-------------------------------------------------------
{
    runTest("data/bone.csv", HU_bone);
}


//--------------------------------------------------------
BOOST_AUTO_TEST_CASE(TestBrainMassAttenuationCoefficients)
//--------------------------------------------------------
{
    runTest("data/brain.csv", HU_brain);
}


//------------------------------------------------------
BOOST_AUTO_TEST_CASE(TestGasMassAttenuationCoefficients)
//------------------------------------------------------
{
    runTest("data/gas.csv", HU_gas);
}


//--------------------------------------------------------
BOOST_AUTO_TEST_CASE(TestTissueMassAttenuationCoefficients)
//--------------------------------------------------------
{
    runTest("data/tissue.csv", HU_soft_tissue);
}


//--------------------------------------------------------
BOOST_AUTO_TEST_CASE(TestWaterMassAttenuationCoefficients)
//--------------------------------------------------------
{
    runTest("data/water.csv", HU_water);
}


//----------------------------------------------------------
void runTest(const char* aFileName, short anHounsfieldValue)
//----------------------------------------------------------
{
    // Open the file
    std::ifstream reference_data_stream(aFileName);

    // The file is not open
    if (!reference_data_stream.is_open())
    {
        std::string error_message;
        error_message += "\"";
        error_message += aFileName;
        error_message += "\" does not exist";

        BOOST_FAIL(error_message);
    }

    // Initialise the X-ray renderer
    XRayRenderer g_xray_renderer;
    
    std::vector<double> reference_vector;
    std::vector<double> test_vector;

    double energy;
    double mass_attenuation_coefficient;
    double temp;

    while (reference_data_stream.good())
    {
        reference_data_stream >> energy >> mass_attenuation_coefficient >> temp;
        energy *= MeV;

        reference_vector.push_back(mass_attenuation_coefficient);
        test_vector.push_back(g_xray_renderer.getMassAttenuationFromHU(anHounsfieldValue, energy)  / (cm2 / g));
    }

    BOOST_CHECK_CLOSE(getNCC(reference_vector, test_vector), 1.0, 0.5);
}


//-----------------------------------------------
double getSum(const std::vector<double>& aVector)
//-----------------------------------------------
{
    return (std::accumulate(aVector.begin(), aVector.end(), 0.0));
}


//---------------------------------------------------
double getAverage(const std::vector<double>& aVector)
//---------------------------------------------------
{
    return (getSum(aVector) / double(aVector.size()));
}


//----------------------------------------------------
double getVariance(const std::vector<double>& aVector)
//----------------------------------------------------
{
    double variance(0.0);
    double average(getAverage(aVector));

    for (std::vector<double>::const_iterator ite(aVector.begin());
            ite != aVector.end();
            ++ite)
    {
        variance += std::pow(*ite - average, 2);
    }

    return (variance / double(aVector.size()));
}


//--------------------------------------------------
double getStdDev(const std::vector<double>& aVector)
//--------------------------------------------------
{
    return (std::sqrt(getVariance(aVector)));
}


//------------------------------------------------
double getNCC(const std::vector<double>& aVector1,
              const std::vector<double>& aVector2)
//------------------------------------------------
{
    double ncc(0.0);

    double average1(getAverage(aVector1));
    double average2(getAverage(aVector2));

    double std_dev(getStdDev(aVector1) * getStdDev(aVector2));

    std::vector<double>::const_iterator ite1(aVector1.begin());
    std::vector<double>::const_iterator ite2(aVector2.begin());

    while (ite1 != aVector1.end())
    {
        ncc += (*ite1 - average1) * (*ite2 - average2);
        ++ite1;
        ++ite2;
    }

    ncc /= std_dev;
    ncc /= aVector1.size();

    return (ncc);
}
