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
*   @file       TestRunDestroyRun.cxx
*
*   @brief      Unit testing: Make sure the simulation can be set, run,
*               (including computing the 3D rendering with text) and destroyed
*               several times in a row.
*
*   @date       17/11/2026
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
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Nov 2026-present, version 1.1,
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
//
#ifndef SGVXR_Source_h
#include "source.h"
#endif

#ifndef SGVXR_Sample_h
#include "sample.h"
#endif

#ifndef SGVXR_Detector_h
#include "detector.h"
#endif
//
#ifndef SGVXR_SimpleGVXR_h
#include "SimpleGVXR.h"
#endif


//******************************************************************************
//  Global variables
//******************************************************************************


//******************************************************************************
//  Function declaration
//******************************************************************************
void runAndDestroy();


//******************************************************************************
//  Unit test
//******************************************************************************

//---------------------------------------
TEST(TestRunDestroyRun, TestRunDestroy3x)
//---------------------------------------
{
    EXPECT_NO_THROW({
        runAndDestroy();
    });

    EXPECT_NO_THROW({
        runAndDestroy();
    });

    EXPECT_NO_THROW({
        runAndDestroy();
    });
}


//******************************************************************************
//  Function definition
//******************************************************************************

//------------------
void runAndDestroy()
//------------------
{
    createOpenGLContext();

    setSourcePosition(-40.0,  0.0, 0.0, "cm");
    usePointSource();
    setMonoChromaticPerPixelAtSDD(0.08, "MeV", 1000);

    setDetectorPosition(10.0, 0.0, 0.0, "cm");
    setDetectorUpVector(0, 0, -1);
    setDetectorNumberOfPixels(640, 320);
    setDetectorPixelSize(0.5, 0.5, "mm");

    // Set the sample
    double cylinder_radius_in_mm = 20.0;
    double cylinder_height_in_mm = 20.0;
    double percent_sphere_volume = 0.025;
    double sphere_radius_in_mm = 2.0;

    makeSpheresInCylinder(
            "cylinder",
            150,
            cylinder_radius_in_mm,
            cylinder_height_in_mm,
            "spheres",
            40,
            40,
            sphere_radius_in_mm,
            percent_sphere_volume,
            "mm");

    // Rotate the sample. The spheres are within the cylinder, only rotate the later.
    rotateNode("cylinder", 90, 1, 0, 0);

    applySOD("cylinder",
        getBestSOD("cylinder", "mm"),
        "mm");

    setElement("cylinder", "Si");
    setElement("spheres", "Fe");

    addPolygonMeshAsInnerSurface("cylinder");
    addPolygonMeshAsInnerSurface("spheres");

    computeXRayImage();
    displayScene();

    renderLoop();
    destroy();
}
