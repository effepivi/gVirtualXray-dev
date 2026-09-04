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
*   @file       TestReloadSimulation.cxx
*
*   @brief      Unit testing: Test if we can reload the simulation.
*
*   @date       04/09/2026
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
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Sep 2026-present, version 1.1,
*   BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include "gtest/gtest.h"

#ifndef SGVXR_Utils_h
#include "Utils.h"
#endif

#ifndef SGVXR_Sample_h
#include "sample.h"
#endif

#ifndef SGVXR_Source_h
#include "source.h"
#endif

#ifndef SGVXR_Detector_h
#include "detector.h"
#endif

#ifndef SGVXR_SimpleGVXR_h
#include "SimpleGVXR.h"
#endif


//******************************************************************************
//  Function declaration
//******************************************************************************
std::vector<std::vector<float>> crack(double incidence_angle = 0.0);


//******************************************************************************
//  Function definition
//******************************************************************************

//--------------------------------
TEST(TestReloadSimulation, reload)
//--------------------------------
{
    std::vector<std::vector<float>> xray_image;

    EXPECT_NO_THROW(xray_image = crack());
    EXPECT_EQ(xray_image.size(), 512);

    EXPECT_NO_THROW(xray_image = crack());
    EXPECT_EQ(xray_image.size(), 512);
}

//-----------------------------------------------------------
std::vector<std::vector<float>> crack(double incidence_angle)
//-----------------------------------------------------------
{
    // Create OpenGL context
    createOpenGLContext();
    removePolygonMeshesFromXRayRenderer();

    // Source
    setSourcePosition(-150.0, 0.0, 0.0, "cm");
    usePointSource();

    // Spectrum
    setVoltage(100.0F, "kV");

    // Noise parameters
    constexpr float nbPhotonsPerPixel = 15000.0F;
    setNumberOfPhotonsPerPixelAtSDD(nbPhotonsPerPixel);

    // Detector
    constexpr int sizeX = 512;
    constexpr int sizeY = 512;
    constexpr double pixelSize = 0.194;

    setDetectorPosition(20.0, 0.0, 0.0, "cm");
    setDetectorUpVector(0.0, 0.0, -1.0);
    setDetectorNumberOfPixels(sizeX, sizeY);
    setDetectorPixelSize(pixelSize, pixelSize, "mm");

    makeCylinder("CylinderWithCrack",
        10,
        60.0,30.0,
        "mm");

    // Material
    const std::vector<int> elements = {1, 8, 16, 20};
    const std::vector<double> weights = {
        0.023416f,
        0.557572f,
        0.18621f,
        0.232797f
    };

    setMixture("CylinderWithCrack", elements, weights);
    setDensity("CylinderWithCrack", 2.32, "g/cm3");

    // Rotate object
    rotateNode(
        "CylinderWithCrack",
        incidence_angle,
        0.0,
        0.0,
        1.0
    );

    addPolygonMeshAsInnerSurface("CylinderWithCrack");

    // Compute image
    return computeXRayImage();
}