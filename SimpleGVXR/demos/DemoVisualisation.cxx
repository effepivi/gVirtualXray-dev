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

3. Neither the name of UK Research and Innovation nor the names of its contributors
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
*   @file       DemoVisualisation.cxx
*
*   @brief      Showcase the renderLoop functionalities.
*
*   @version    1.0
*
*   @date       21/07/2026
*
*   @author     Prof Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, July 2026-present, version 1.1,
*   BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_LOGGER_H
#include "gVirtualXRay/Logger.h"
#endif

#ifndef SGVXR_SimpleGVXR_h
#include "SimpleGVXR.h"
#endif


//******************************************************************************
//  Namespaces
//******************************************************************************

//******************************************************************************
//  Function declarations
//******************************************************************************
void createEnvironment(const std::string& aRenderer,
    unsigned int aMixedPrecisionFlag);


//******************************************************************************
//  Global variables
//******************************************************************************


//******************************************************************************
//  Function definitions
//******************************************************************************

//------------------------------------------------------
void createEnvironment(const std::string& aRenderer,
                       unsigned int aMixedPrecisionFlag)
//------------------------------------------------------
{
    // Create a simulation environment
    gVirtualXRay::LOGGER.logNow("Create a simulation environment") << std::endl;

    createWindow(-1,
        1,
        aRenderer,
        4, 6, aMixedPrecisionFlag);

    gVirtualXRay::LOGGER.logNow("OpenGL renderer:\t") << getOpenGlRenderer() << std::endl;
    gVirtualXRay::LOGGER.logNow("OpenGL version:\t") << getOpenGlVersion() << std::endl;
    gVirtualXRay::LOGGER.logNow("OpenGL vendor:\t") << getOpenGlVendor() << std::endl;
    gVirtualXRay::LOGGER.logNow("Shading language version:\t") << getShadingLanguageVersion() << std::endl;
    gVirtualXRay::LOGGER.logNow("Poisson noise implementation:\t") << getNoiseGenerator() << std::endl;

    setEnergyBinSize(5, "keV");

    // The sample
    std::string fname = "welsh-dragon-small.stl";

    loadMeshFile("Dragon", fname, "mm");
    // moveToCentre("Dragon");
    // applyCurrentLocalTransformation("Dragon");

    // Change the sample's colour
    // By default the object is white, which is not always pretty.Let's change it to gold.
    float red = 1.0F;
    float green = 215.0F / 255.0F;
    float blue = 0.0F;
    setColour("Dragon", red, green, blue, 1.0F);

    // Material properties

    // Liquid water
    setCompound("Dragon", "H2O");
    setDensity("Dragon", 1.0, "g/cm3");
    // setDensity("Dragon", 1.0, "g.cm-3");
    //

    // Set the simulation parameters
    gVirtualXRay::LOGGER.logNow("Load the XTEKCT file:\ttensile.xtekct") << std::endl;
    loadXtekct("tensile.xtekct");

    float best_sod = getBestSOD("Dragon", "mm");
    applySOD("Dragon", best_sod, "mm");


    // Set up the detector
//     gVirtualXRay::LOGGER.logNow("Set up the detector");
//     setDetectorPosition(0.0F, 0.0F, 40.0F, "cm");
//     setDetectorUpVector(0.0F, 1.0F, -1.0F);
//
// #ifdef _DEBUG
//     setDetectorNumberOfPixels(512 / 8, 512 / 8);
//     setDetectorPixelSize(500 * 12, 500 * 8, "um");
// #else
//     setDetectorNumberOfPixels(256, 128);
//     setDetectorPixelSize(1000.0F, 1000.0F, "um");
// #endif // _DEBUG
//
//     // Create a source
//     gVirtualXRay::LOGGER.logNow("Set up the beam");
//     setSourcePosition(0.0F, 0.0F, -80.0F, "cm");
    // usePointSource();
    //  For a parallel source, use useParallelBeam();

    // Set its spectrum, here a monochromatic beam with 1000 photons of 80 keV(i.e. 0.08 MeV) per ray
    // setMonoChromatic(15.0, "keV", 1000.0F);



    // Change to a white background(it could be useful for putting screenshots in papers)
    setWindowBackGroundColour(1.0F, 1.0F, 1.0F, -1);
}


//--------
int main()
//--------
{
    int return_code = 0;

    try {
        gVirtualXRay::LOGGER.logNow("gVXR:\t") << getVersionOfCoreGVXR() << std::endl;
        gVirtualXRay::LOGGER.logNow("SimpleGVXR:\t") << getVersionOfSimpleGVXR() << std::endl;

         // OpenGL mixed precision
         uint8_t mixed_precision_flag = computeMixedPrecisionFlag(true, // Uncleaned L-buffer
             false, // Cleaned L-buffer
             false, // Cumulated L-buffer
             true, // Energy fluence
             false, // Sum muX dX
             false); // Z-buffer

         createEnvironment("OPENGL", mixed_precision_flag);

         // Compute an X - ray image
         std::vector<std::vector<float> > x_ray_image_mixed = computeXRayImage();

         // Interactive visualisation
         renderLoop();

         destroy();

    }
    catch (std::exception& e) {
        gVirtualXRay::LOGGER.logError("") << e.what() << std::endl;
        return_code = 1;
    }

    return return_code;
}
