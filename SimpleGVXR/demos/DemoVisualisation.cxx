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
#include <iostream>

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

    const std::string particle_label = "Pores";
    const std::string matrix_label = "Matrix";
    const std::string foam_label = "root";

    emptyMesh(foam_label);
    size_t number_of_pores =     makeSpheresInCylinder(
            matrix_label,
            150,
            5,
            20,
            particle_label,
            40,
            40,
            1,
            0.05,
            "mm",
            foam_label);
    std::cout << "Number of pores: " << number_of_pores << std::endl;

    // Add the cylinder to the renderer
    addPolygonMeshAsInnerSurface(matrix_label);

    // Material properties

    // Change the sample's colour
    // By default the object is white, which is not always pretty.Let's change it to shiny steel.
    setDiffuseColour(matrix_label, 0.45F, 0.45F, 0.45F, 1.0F);
    setSpecularColour(matrix_label, 0.95F, 0.95F, 0.95F, 1.0F);
    setAmbiantColour(matrix_label, 0.15F, 0.15F, 0.15F, 1.0F);
    setShininess(matrix_label, 20.0F);

    // Steel water
    setElement(matrix_label, "Fe");
    // setMixture(matrix_label, "Fe80C20");
    // setDensity(matrix_label, 8.85, "g/cm3");
    
    // Set the simulation parameters
    gVirtualXRay::LOGGER.logNow("Load the XTEKCT file:\ttensile.xtekct") << std::endl;
    loadXtekct("tensile.xtekct");
    // addFilter("Fe", 0.5, "mm");

    float best_sod = getBestSOD(matrix_label, "mm");
    applySOD(matrix_label, best_sod, "mm");

    // Change to a white background if you prefer (it could be useful for putting screenshots in papers)
    setWindowBackGroundColour(0.5F, 0.5F, 0.5F, -1);
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
             true, // Cleaned L-buffer
             true, // Cumulated L-buffer
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
