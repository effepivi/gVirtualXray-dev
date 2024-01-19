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
*   @file       TestScintilator.cxx
*
*   @brief      Unit testing: scintillator and energy response,
*               Last date of check: Passed test on 27 Jun 2023.
*
*   @version    1.0
*
*   @date       27/06/2023
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
*               http://www.fpvidal.net/, Jun 27th, 2023, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <numeric>

#include "gtest/gtest.h"

#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef __Scintillator_h
#include "gVirtualXRay/Scintillator.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Function declaration
//******************************************************************************
void runTest(const char* aFileName,
    const std::vector<std::pair<RATIONAL_NUMBER, RATIONAL_NUMBER> >& response);
double getSum(const std::vector<double>& aVector);
double getAverage(const std::vector<double>& aVector);
double getVariance(const std::vector<double>& aVector);
double getStdDev(const std::vector<double>& aVector);
double getNCC(const std::vector<double>& aVector1, const std::vector<double>& aVector2);
double maxRelativeError(const std::vector<double>& aVector1, const std::vector<double>& aVector2);
double meanRelativeError(const std::vector<double>& aVector1, const std::vector<double>& aVector2);


//---------------------------------
void runTest(const char* aFileName,
             const std::vector<std::pair<RATIONAL_NUMBER, RATIONAL_NUMBER> >& response)
//---------------------------------
{
    // Open the files
    std::ifstream reference_data_stream(aFileName);

    // The file is not open
    if (!reference_data_stream.is_open())
    {
        std::string error_message;
        error_message += "\"";
        error_message += aFileName;
        error_message += "\" does not exist";

        FAIL() << error_message;
    }

    std::vector<double> reference_vector;
    std::vector<double> test_vector;

    double current_ref_energy = 0;
    double last_ref_energy    = 0;
    double ref_response = 0;

    int i = 0;

    while (reference_data_stream.good())
    {
        reference_data_stream >> current_ref_energy >> ref_response;

        if (i == 0 || last_ref_energy != current_ref_energy)
        {
          reference_vector.push_back(ref_response);
          test_vector.push_back(response[i].second);
      }

      last_ref_energy = current_ref_energy;

        ++i;
    }

    // Tolerence 0.25%
    ASSERT_NEAR(100.0 * getNCC(reference_vector, test_vector), 100.0, 0.25);

    // Tolerence 0.25%
    ASSERT_NEAR(100.0 * maxRelativeError(reference_vector, test_vector), 0.0, 0.25);
}


//----------------------------
TEST(TestScintilator, TestCsI)
//----------------------------
{
    const std::string& mat = "CsI";
    double rho = 4.51; // g.cm^-3
    double thickness = 0.06; // cm

    Scintillator scintillator;
    scintillator.setMaterial(mat);
    scintillator.setThickness(0.06, cm);

    std::vector<std::pair<RATIONAL_NUMBER, RATIONAL_NUMBER> > response = scintillator.getEnergyResponse(keV);

    runTest("data/responseCsIDetector.tsv", response);
}


//------------------------------
TEST(TestScintilator, TestGdO2S)
//------------------------------
{
    const std::string& mat = "GdO2S";
    double rho = 7.32; // g.cm^-3
    double thickness = 0.06; // cm

    Scintillator scintillator;
    scintillator.setMaterial(mat);
    scintillator.setThickness(0.06, cm);

    std::vector<std::pair<RATIONAL_NUMBER, RATIONAL_NUMBER> > response = scintillator.getEnergyResponse(keV);

    runTest("data/responseGdO2SDetector.tsv", response);
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


//----------------------------------------------------------
double maxRelativeError(const std::vector<double>& aVector1,
                        const std::vector<double>& aVector2)
//----------------------------------------------------------
{
    double max_relative_error(0.0);
    double relative_error(0.0);

    std::vector<double>::const_iterator ite1(aVector1.begin());
    std::vector<double>::const_iterator ite2(aVector2.begin());

    while (ite1 != aVector1.end())
    {
        relative_error = std::abs(*ite1 - *ite2) / *ite1;
        max_relative_error = std::max(relative_error, max_relative_error);

        ++ite1;
        ++ite2;
    }

    return (max_relative_error);
}


//-----------------------------------------------------------
double meanRelativeError(const std::vector<double>& aVector1,
                         const std::vector<double>& aVector2)
//-----------------------------------------------------------
{
    double mean_relative_error(0.0);

    std::vector<double>::const_iterator ite1(aVector1.begin());
    std::vector<double>::const_iterator ite2(aVector2.begin());

    while (ite1 != aVector1.end())
    {
        mean_relative_error += std::abs(*ite1 - *ite2) / *ite1;

        ++ite1;
        ++ite2;
    }

    return (mean_relative_error / aVector1.size());
}
