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
*   @file       TestAcquisitionFile.cxx
*
*   @brief      Unit testing: Test the functions in ../src/acquisition.cxx.
*
*   @date       14/05/2025
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
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, May 2025-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <string>
#include <sstream>
#include <vector>

#include "gtest/gtest.h"

#ifndef GVXR_LOGGER_H
#include "gVirtualXRay/Logger.h"
#endif

#ifndef GVXR_IMAGE_H
#include "gVirtualXRay/Image.h"
#endif

#ifndef SGVXR_Utils_h
#include "Utils.h"
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

#ifndef SGVXR_Internal_h
#include "internal.h"
#endif


//******************************************************************************
//  Global variables
//******************************************************************************


//******************************************************************************
//  Function declaration
//******************************************************************************


//---------------------------------------------
TEST(TestAcquisitionFile, computeCTAcquisition)
//---------------------------------------------
{
    // # %%
    // tilt = 30
    std::string simulation_name = "test";
    unsigned int number_of_projections = 36;
    unsigned int number_of_rows = 500;
    unsigned int number_of_cols = 600;

    // Create the experiment geometry
    // Set up the source
    createOpenGLContext(-1, 4, 6, 32);

    // Set the sample
    float sphere_radius = 40;
    float cube_edge = 60;
    // removePolygonMeshesFromSceneGraph() To be on the safe side
    makeSphere(simulation_name, 50, 50, sphere_radius, "um");
    translateNode(simulation_name, 0, -50, -50, "um");
    applyCurrentLocalTransformation(simulation_name);

    makeCube("cube", cube_edge, "um");
    translateNode("cube", 0, 50, 50, "um");
    applyCurrentLocalTransformation("cube");

    addMesh(simulation_name, "cube");

    setElement(simulation_name, "Si");

    makeCylinder("cylinder", 50, 10, sphere_radius / 8, "um");

    setElement("cylinder", "W");

    addPolygonMeshAsInnerSurface("cylinder");
    addPolygonMeshAsInnerSurface(simulation_name);

    // Set up the detector
    setDetectorPosition(0.10, 0.0, 0.0, "cm");
    setDetectorUpVector(0, 0, -1);
    setDetectorNumberOfPixels(number_of_cols, number_of_rows);
    float pixel_size_um = 0.5;
    setDetectorPixelSize(pixel_size_um, pixel_size_um, "um");

    // Set up the beam geometry
    setSourcePosition(-0.40, 0.0, 0.0, "cm");
    useParallelBeam();

    // Check the SDD calculations
    float ref_SDD_cm = 0.5;
    ASSERT_NEAR(ref_SDD_cm, getSourceDetectorDistance("cm"), 1e-5);
    ASSERT_NEAR(ref_SDD_cm, getSDD("cm"), 1e-5);

    ASSERT_NEAR(ref_SDD_cm * 10.0, getSourceDetectorDistance("mm"), 1e-4);
    ASSERT_NEAR(ref_SDD_cm * 10.0, getSDD("mm"), 1e-4);

    // Check the SOD calculations
    float ref_SOD_cm = 0.4;
    ASSERT_NEAR(ref_SOD_cm, getSourceObjectDistance("cm"), 1e-5);
    ASSERT_NEAR(ref_SOD_cm, getSOD("cm"), 1e-5);

    ASSERT_NEAR(ref_SOD_cm * 10.0, getSourceObjectDistance("mm"), 1e-4);
    ASSERT_NEAR(ref_SOD_cm * 10.0, getSOD("mm"), 1e-4);

    // Check the ODD calculations
    float ref_ODD_cm = 0.1;
    ASSERT_NEAR(ref_ODD_cm, getObjectDetectorDistance("cm"), 1e-5);
    ASSERT_NEAR(ref_ODD_cm, getODD("cm"), 1e-5);

    ASSERT_NEAR(ref_ODD_cm * 10.0, getObjectDetectorDistance("mm"), 1e-4);
    ASSERT_NEAR(ref_ODD_cm * 10.0, getODD("mm"), 1e-4);

    // Set up the beam spectrum
    float energy = 30;
    std::string energy_units = "keV";
    unsigned int photons = 10000;
    setMonoChromaticPerPixelAtSDD(energy, energy_units, photons);

    // Compare with computeCTAcquisition function
    computeCTAcquisition("",
        "",
        number_of_projections, // The total number of projections to simulate.
        0, // The rotation angle corresponding to the first projection.
        false, // A boolean flag to include or exclude the last angle. It is used to calculate the angular step between successive projections.
        360, // The rotation angle corresponding to the last projection. Note that depending on the value of anIncludeLastAngleFlag, this angle may be included or excluded
        1, // The number of white images used to perform the flat-field correction. If zero, then no correction will be performed.
        0, 0, 0, // The location of the rotation centre.
        // gvxr.getCentreOfRotationPositionCT("mm")[0],
        "mm", // The corresponding unit of length.
        0, 0, 1, // The rotation axis
        true // If true the energy fluence is returned, otherwise the number of photons is returned
             // (default value: true)
    );

    std::vector<std::vector<std::vector<float> > > projection_set = getLastProjectionSet();
    saveLastProjectionSet("TestAcquisitionFile_computeCTAcquisition-RAM.mha");

    ASSERT_EQ(projection_set.size(), number_of_projections);
    ASSERT_EQ(projection_set[0].size(), number_of_rows);
    ASSERT_EQ(projection_set[0][0].size(), number_of_cols);

    // Compare with computeCTAcquisition function
    computeCTAcquisition("TestAcquisitionFile_computeCTAcquisition",
        "",
        number_of_projections, // The total number of projections to simulate.
        0, // The rotation angle corresponding to the first projection.
        false, // A boolean flag to include or exclude the last angle. It is used to calculate the angular step between successive projections.
        360, // The rotation angle corresponding to the last projection. Note that depending on the value of anIncludeLastAngleFlag, this angle may be included or excluded
        1, // The number of white images used to perform the flat-field correction. If zero, then no correction will be performed.
        0, 0, 0, // The location of the rotation centre.
        // gvxr.getCentreOfRotationPositionCT("mm")[0],
        "mm", // The corresponding unit of length.
        0, 0, 1, // The rotation axis
        true // If true the energy fluence is returned, otherwise the number of photons is returned
             // (default value: true)
    );

    for (unsigned int k = 0; k < number_of_projections; ++k)
    {
        // Load the TIFF file
        std::stringstream fname;
        fname << "TestAcquisitionFile_computeCTAcquisition/projection_" << std::setfill('0') << std::setw(4) << k << ".tiff";
        gVirtualXRay::Image<float> projection(fname.str());

        // Make sure the projection size is the same
        ASSERT_EQ(projection.getHeight(), number_of_rows);
        ASSERT_EQ(projection.getWidth(), number_of_cols);

        // Compare the values from the TIFF file with those in the RAM
        for (unsigned int j = 0; j < number_of_rows; ++j)
            for (unsigned int i = 0; i < number_of_cols; ++i)
            {
                float pixel0 = projection_set[k][j][i];
                float pixel1 = projection.getPixel(i,j);

                // Make sure the pixel values are the same
                ASSERT_NEAR(pixel0, pixel1, 1e-6);
            }
    }

    setDetectorGain(250);
    std::vector<std::vector<float> > xray_image_float = computeXRayImage(true);
    std::vector<std::vector<unsigned short> > xray_image_ushort = computeXRayImageWithGain(true);

    for (unsigned int j = 0; j < number_of_rows; ++j)
        for (unsigned int i = 0; i < number_of_cols; ++i)
        {
            float pixel0 = xray_image_float[j][i];
            unsigned short pixel1 = xray_image_ushort[j][i];
            unsigned short pixel2 = 0;

            pixel0 *= getDetectorGain();

            if (pixel0 < 0.0f) pixel2 = 0;
            else if (pixel0 > USHRT_MAX) pixel2 = USHRT_MAX;
            else pixel2 = round(pixel0);

            // Make sure the pixel values are the same
            ASSERT_EQ(pixel1, pixel2);
        }


    {
        std::vector<std::vector<float> > xray_image_1 = computeXRayImage(true);
        std::vector<float> xray_image_2(number_of_rows * number_of_cols);
        computeXRayImage(&xray_image_2[0], number_of_rows, number_of_cols, true);

        for (unsigned int j = 0; j < number_of_rows; ++j)
            for (unsigned int i = 0; i < number_of_cols; ++i)
            {
                float pixel0 = xray_image_1[j][i];
                float pixel1 = xray_image_2[j * number_of_cols + i];

                // Make sure the pixel values are the same
                ASSERT_EQ(pixel0, pixel1);
            }
    }

    {
        std::vector<std::vector<unsigned short> > xray_image_1 = computeXRayImageWithGain(true);
        std::vector<unsigned short> xray_image_2(number_of_rows * number_of_cols);
        computeXRayImageWithGain(&xray_image_2[0], number_of_rows, number_of_cols, true);

        for (unsigned int j = 0; j < number_of_rows; ++j)
            for (unsigned int i = 0; i < number_of_cols; ++i)
            {
                float pixel0 = xray_image_1[j][i];
                float pixel1 = xray_image_2[j * number_of_cols + i];

                // Make sure the pixel values are the same
                ASSERT_EQ(pixel0, pixel1);
            }
    }

    {
        std::vector<std::vector<float> > xray_image_float = computeXRayImage(true);
        std::vector<std::vector<unsigned short> > xray_image_ushort = computeXRayImageWithGain(true);

        for (unsigned int j = 0; j < number_of_rows; ++j)
            for (unsigned int i = 0; i < number_of_cols; ++i)
            {
                float pixel0 = xray_image_float[j][i];
                unsigned short pixel1 = xray_image_ushort[j][i];
                unsigned short pixel2 = 0;

                pixel0 *= getDetectorGain();

                if (pixel0 < 0.0f) pixel2 = 0;
                else if (pixel0 > USHRT_MAX) pixel2 = USHRT_MAX;
                else pixel2 = round(pixel0);

                // Make sure the pixel values are the same
                ASSERT_EQ(pixel1, pixel2);
            }
    }

    {
        std::vector<float> xray_image_float(number_of_rows * number_of_cols);
        std::vector<unsigned short> xray_image_ushort(number_of_rows * number_of_cols);

        computeXRayImage(&xray_image_float[0], number_of_rows, number_of_cols, true);
        computeXRayImageWithGain(&xray_image_ushort[0], number_of_rows, number_of_cols, true);

        for (unsigned int j = 0; j < number_of_rows; ++j)
            for (unsigned int i = 0; i < number_of_cols; ++i)
            {
                float pixel0 = xray_image_float[j * number_of_cols + i];
                unsigned short pixel1 = xray_image_ushort[j * number_of_cols + i];
                unsigned short pixel2 = 0;

                pixel0 *= getDetectorGain();

                if (pixel0 < 0.0f) pixel2 = 0;
                else if (pixel0 > USHRT_MAX) pixel2 = USHRT_MAX;
                else pixel2 = round(pixel0);

                // Make sure the pixel values are the same
                ASSERT_EQ(pixel1, pixel2);
            }
    }
}


//-----------------------------
TEST(TestAcquisitionFile, BHPI)
//-----------------------------
{
    // Use several monochromatic beams
    float monochromatic_bhpi = 0.0;
    for (unsigned int i = 0; i < 5; ++i) {
        setMonoChromaticPerPixelAtSDD(5 + i * 20, "keV", 16000);

        monochromatic_bhpi = 100.0 * getBHPI();
        ASSERT_EQ(monochromatic_bhpi < 0.01, true);
    }

    // Polychromatic, 0 filter
    setVoltage(150, "kV");
    setNumberOfPhotonsPerPixelAtSDD(16000);

    float new_bhpi = getBHPI();
    ASSERT_EQ(new_bhpi > monochromatic_bhpi, true);
    float old_bhpi = new_bhpi;

    // Add 1 mm of Al for every iteration of the loop
    for (unsigned int i = 0; i < 5; i++) {
        addFilter("Al", 1.0 * (i + 1.0), "mm");

        float new_bhpi = getBHPI();
        ASSERT_EQ(old_bhpi > new_bhpi, true);
        old_bhpi = new_bhpi;
    }
}
