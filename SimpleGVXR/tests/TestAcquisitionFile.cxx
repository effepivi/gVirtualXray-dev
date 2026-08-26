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

#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

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

#ifndef SGVXR_Sample_h
#include "sample.h"
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
extern gVirtualXRay::XRayRenderer g_xray_renderer; // X-ray graphics engine

const unsigned int g_number_of_rows = 50;
const unsigned int g_number_of_cols = 60;


//******************************************************************************
//  Function declaration
//******************************************************************************


//----------------------------------
TEST(TestAcquisitionFile, distances)
//----------------------------------
{
    // Create an OpenGL context
#ifdef HAS_OPENGL
#ifdef HAS_EGL
    createWindow(-1, 0,
        "EGL",
        3, 2,
        255); // Full-precision floating-point numbers
#else // HAS_EGL
#ifdef __APPLE__
 // OpenGL 4.2 and above are not supported on MacOS (at least on my compute with MacOS Big Sur)
    createWindow(-1, 0,
        "OPENGL",
        4, 1,
        255); // Full-precision floating-point numbers
#else // __APPLE__
    createWindow(-1, 0,
        "OPENGL",
        4, 6,
        0); // Half-precision floating-point numbers is enough
#endif // __APPLE__
#endif // HAS_EGL
#else // HAS_OPENGL
    createWindow(-1, 0, "OPENMP");
#endif // HAS_OPENGL

    // Make sure the sample is reproducible from one run to another
    setSeed(0);

    // Set the sample
    double cylinder_radius_in_mm = 20.0;
    double cylinder_height_in_mm = 20.0;
    double percent_sphere_volume = 0.3;
    double sphere_radius_in_mm = 2.0;

    double cylinder_volume_in_mm3 = M_PI * cylinder_radius_in_mm * cylinder_radius_in_mm * cylinder_height_in_mm;
    double sphere_volume_in_mm3 = 4.0 / 3.0 * M_PI * sphere_radius_in_mm * sphere_radius_in_mm * sphere_radius_in_mm;
    double target_sphere_volume_in_mm3 = percent_sphere_volume * cylinder_volume_in_mm3;

    size_t number_of_spheres = makeSpheresInCylinder(
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

    // Error within 5% of expected value
    double total_sphere_volume_in_mm3 = sphere_volume_in_mm3 * static_cast<double>(number_of_spheres);
    ASSERT_NEAR(target_sphere_volume_in_mm3, total_sphere_volume_in_mm3, 5.0 / 100.0 * target_sphere_volume_in_mm3);


    setElement("cylinder", "Si");
    setElement("spheres", "Fe");

    addPolygonMeshAsInnerSurface("cylinder");
    addPolygonMeshAsInnerSurface("spheres");

    // The SOD and ODD are measured from the centre of the sample's bounding
    // box. The spheres are randomly positioned, so this centre is not exactly
    // at the origin: the detector and the source are positioned relatively to
    // it so that the expected distances are known exactly.
    std::vector<double> object_position = getNodeAndChildrenBoundingBoxCentre("cylinder", "mm");

    // Set up the detector, 1 mm away from the centre of the sample
    setDetectorPosition(object_position[0] + 1.0, object_position[1], object_position[2], "mm");
    setDetectorUpVector(0, 0, -1);
    setDetectorNumberOfPixels(g_number_of_cols, g_number_of_rows);
    float pixel_size_mm = 1.0;
    setDetectorPixelSize(pixel_size_mm, pixel_size_mm, "mm");

    // Set up the beam geometry, 4 mm away from the centre of the sample along the X-axis
    setSourcePosition(object_position[0] - 4.0, object_position[1], object_position[2], "mm");
    useParallelBeam();

    // // Check the SDD calculations
    // float ref_SDD_cm = 0.5;
    // ASSERT_NEAR(ref_SDD_cm, getSourceDetectorDistance("cm"), 1e-5);
    // ASSERT_NEAR(ref_SDD_cm, getSDD("cm"), 1e-5);
    //
    // ASSERT_NEAR(ref_SDD_cm * 10.0, getSourceDetectorDistance("mm"), 1e-4);
    // ASSERT_NEAR(ref_SDD_cm * 10.0, getSDD("mm"), 1e-4);
    //
    // // Check the SOD calculations
    // float ref_SOD_cm = 0.4;
    // ASSERT_NEAR(ref_SOD_cm, getSourceObjectDistance("cm"), 1e-5);
    // ASSERT_NEAR(ref_SOD_cm, getSOD("cm"), 1e-5);
    //
    // ASSERT_NEAR(ref_SOD_cm * 10.0, getSourceObjectDistance("mm"), 1e-4);
    // ASSERT_NEAR(ref_SOD_cm * 10.0, getSOD("mm"), 1e-4);
    //
    // // Check the ODD calculations
    // float ref_ODD_cm = 0.1;
    // ASSERT_NEAR(ref_ODD_cm, getObjectDetectorDistance("cm"), 1e-5);
    // ASSERT_NEAR(ref_ODD_cm, getODD("cm"), 1e-5);
    //
    // ASSERT_NEAR(ref_ODD_cm * 10.0, getObjectDetectorDistance("mm"), 1e-4);
    // ASSERT_NEAR(ref_ODD_cm * 10.0, getODD("mm"), 1e-4);

    // Set up the beam spectrum
    float energy = 30;
    std::string energy_units = "keV";
    unsigned int photons = 10000;
    setMonoChromaticPerPixelAtSDD(energy, energy_units, photons);

    std::string unit = "mm";
    float sdd = getSDD(unit);
    ASSERT_NEAR(sdd, 5, 1e-5);
    ASSERT_EQ(getSourceDetectorDistance(unit), sdd);

    float odd = getODD(unit);
    ASSERT_NEAR(odd, 1, 1e-5);
    ASSERT_EQ(getObjectDetectorDistance(unit), odd);

    float sod = getSOD(unit);
    ASSERT_NEAR(sod, 4, 1e-5);
    ASSERT_EQ(getSourceObjectDistance(unit), sod);

    unit = "cm";
    sdd = getSDD(unit);
    ASSERT_NEAR(sdd, 0.5, 1e-5);
    ASSERT_EQ(getSourceDetectorDistance(unit), sdd);

    odd = getODD(unit);
    ASSERT_NEAR(odd, 0.1, 1e-5);
    ASSERT_EQ(getObjectDetectorDistance(unit), odd);

    sod = getSOD(unit);
    ASSERT_NEAR(sod, 0.4, 1e-5);
    ASSERT_EQ(getSourceObjectDistance(unit), sod);
}


//---------------------------------------------
TEST(TestAcquisitionFile, computeCTAcquisition)
//---------------------------------------------
{
    unsigned int number_of_projections = 4;


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
    ASSERT_EQ(projection_set[0].size(), g_number_of_rows);
    ASSERT_EQ(projection_set[0][0].size(), g_number_of_cols);

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
        ASSERT_EQ(projection.getHeight(), g_number_of_rows);
        ASSERT_EQ(projection.getWidth(), g_number_of_cols);

        // Compare the values from the TIFF file with those in the RAM
        for (unsigned int j = 0; j < g_number_of_rows; ++j)
            for (unsigned int i = 0; i < g_number_of_cols; ++i)
            {
                float pixel0 = projection_set[k][j][i];
                float pixel1 = projection.getPixel(i,j);

                // Make sure the pixel values are the same
                ASSERT_NEAR(pixel0, pixel1, 1e-3);
            }
    }

    setDetectorGain(250);
    std::vector<std::vector<float> > xray_image_float = computeXRayImage(true);
    std::vector<std::vector<unsigned short> > xray_image_ushort = computeXRayImageWithGain(true);

    for (unsigned int j = 0; j < g_number_of_rows; ++j)
        for (unsigned int i = 0; i < g_number_of_cols; ++i)
        {
            float pixel0 = xray_image_float[j][i];
            unsigned short pixel1 = xray_image_ushort[j][i];
            unsigned short pixel2 = 0;

            pixel0 *= getDetectorGain();

            if (pixel0 < 0.0f) pixel2 = 0;
            else if (pixel0 > USHRT_MAX) pixel2 = USHRT_MAX;
            else pixel2 = static_cast<unsigned short>(round(pixel0));

            // Make sure the pixel values are the same
            ASSERT_EQ(pixel1, pixel2);
        }


    {
        std::vector<std::vector<float> > xray_image_1 = computeXRayImage(true);
        std::vector<float> xray_image_2(g_number_of_rows * g_number_of_cols);
        computeXRayImage(&xray_image_2[0], g_number_of_rows, g_number_of_cols, true);

        for (unsigned int j = 0; j < g_number_of_rows; ++j)
            for (unsigned int i = 0; i < g_number_of_cols; ++i)
            {
                float pixel0 = xray_image_1[j][i];
                float pixel1 = xray_image_2[j * g_number_of_cols + i];

                // Make sure the pixel values are the same
                ASSERT_EQ(pixel0, pixel1);
            }
    }

    {
        std::vector<std::vector<unsigned short> > xray_image_1 = computeXRayImageWithGain(true);
        std::vector<unsigned short> xray_image_2(g_number_of_rows * g_number_of_cols);
        computeXRayImageWithGain(&xray_image_2[0], g_number_of_rows, g_number_of_cols, true);

        for (unsigned int j = 0; j < g_number_of_rows; ++j)
            for (unsigned int i = 0; i < g_number_of_cols; ++i)
            {
                float pixel0 = xray_image_1[j][i];
                float pixel1 = xray_image_2[j * g_number_of_cols + i];

                // Make sure the pixel values are the same
                ASSERT_EQ(pixel0, pixel1);
            }
    }

    {
        std::vector<std::vector<float> > xray_image_float = computeXRayImage(true);
        std::vector<std::vector<unsigned short> > xray_image_ushort = computeXRayImageWithGain(true);

        for (unsigned int j = 0; j < g_number_of_rows; ++j)
            for (unsigned int i = 0; i < g_number_of_cols; ++i)
            {
                float pixel0 = xray_image_float[j][i];
                unsigned short pixel1 = xray_image_ushort[j][i];
                unsigned short pixel2 = 0;

                pixel0 *= getDetectorGain();

                if (pixel0 < 0.0f) pixel2 = 0;
                else if (pixel0 > USHRT_MAX) pixel2 = USHRT_MAX;
                else pixel2 = static_cast<unsigned short>(round(pixel0));

                // Make sure the pixel values are the same
                ASSERT_EQ(pixel1, pixel2);
            }
    }

    {
        std::vector<float> xray_image_float(g_number_of_rows * g_number_of_cols);
        std::vector<unsigned short> xray_image_ushort(g_number_of_rows * g_number_of_cols);

        computeXRayImage(&xray_image_float[0], g_number_of_rows, g_number_of_cols, true);
        computeXRayImageWithGain(&xray_image_ushort[0], g_number_of_rows, g_number_of_cols, true);

        for (unsigned int j = 0; j < g_number_of_rows; ++j)
            for (unsigned int i = 0; i < g_number_of_cols; ++i)
            {
                float pixel0 = xray_image_float[j * g_number_of_cols + i];
                unsigned short pixel1 = xray_image_ushort[j * g_number_of_cols + i];
                unsigned short pixel2 = 0;

                pixel0 *= getDetectorGain();

                if (pixel0 < 0.0f) pixel2 = 0;
                else if (pixel0 > USHRT_MAX) pixel2 = USHRT_MAX;
                else pixel2 = static_cast<unsigned short>(round(pixel0));

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
    gVirtualXRay::Image<gVirtualXRay::XRayRenderer::PixelType> image_before;
    gVirtualXRay::Image<gVirtualXRay::XRayRenderer::PixelType> image_after;

    for (unsigned int i = 0; i < 5; ++i) {
        setMonoChromaticPerPixelAtSDD(5 + i * 20, "keV", 16000);

        g_xray_renderer.computeImage(gVirtualXRay::MATRIX4(), true);
        image_before = g_xray_renderer.getEnergyFluence();

        monochromatic_bhpi = getBHPI();
        ASSERT_EQ(monochromatic_bhpi < 1e-3, true);

        g_xray_renderer.computeImage(gVirtualXRay::MATRIX4(), true);
        image_after = g_xray_renderer.getEnergyFluence();

        ASSERT_EQ(image_before.computeMAE(image_after) < 1e-5, true);
        ASSERT_EQ(image_before.computeRMSE(image_after) < 1e-5, true);
        ASSERT_EQ(image_before.computeNCC(image_after) > 0.999, true);
    }

    // Polychromatic, 0 filter
    setVoltage(200, "kV");
    clearFiltration();
    setNumberOfPhotonsPerPixelAtSDD(16000);

    g_xray_renderer.computeImage(gVirtualXRay::MATRIX4(), true);
    image_before = g_xray_renderer.getEnergyFluence();

    float new_bhpi = getBHPI();
    std::cout << "Filtration" << "\t" << "BHPI" << std::endl;
    std::cout << 0.0 << "\t" << new_bhpi << std::endl;
    ASSERT_EQ(new_bhpi > monochromatic_bhpi, true);
    float old_bhpi = new_bhpi;

    g_xray_renderer.computeImage(gVirtualXRay::MATRIX4(), true);
    image_after = g_xray_renderer.getEnergyFluence();

    ASSERT_EQ(image_before.computeMAE(image_after) < 1e-5, true);
    ASSERT_EQ(image_before.computeRMSE(image_after) < 1e-5, true);
    ASSERT_EQ(image_before.computeNCC(image_after) > 0.99999, true);

    // Add 0.1 mm of Al for every iteration of the loop
    for (unsigned int i = 1; i < 21; i++) {
        clearFiltration();
        addFilter("Al", 0.1 * i, "mm");

        g_xray_renderer.computeImage(gVirtualXRay::MATRIX4(), true);
        image_before = g_xray_renderer.getEnergyFluence();

        float new_bhpi = getBHPI();
        std::cout << 0.1 * i << "\t" << new_bhpi << std::endl;

        ASSERT_EQ(old_bhpi > new_bhpi, true);
        old_bhpi = new_bhpi;

        g_xray_renderer.computeImage(gVirtualXRay::MATRIX4(), true);
        image_after = g_xray_renderer.getEnergyFluence();

        double mae = image_before.computeMAE(image_after);
        double rmse = image_before.computeRMSE(image_after);
        double zncc = image_before.computeNCC(image_after);
        ASSERT_EQ(mae < 1e-5, true);
        ASSERT_EQ(rmse < 1e-5, true);
        ASSERT_EQ(zncc > 0.99999, true);
    }
}


//------------------------------
TEST(TestAcquisitionFile, PSNR1)
//------------------------------
{
    clearFiltration();
    setVoltage(150, "kV");
    setNumberOfPhotonsPerPixelAtSDD(16);

    float new_psnr = getPSNR();
    ASSERT_EQ(new_psnr > 0.0, true);
    float old_psnr = new_psnr;

    // Multiply the flux by 10 for every iteration of the loop
    // The PSNR should improve (less and less noise)
    for (unsigned int i = 0; i < 3; i++) {
        setNumberOfPhotonsPerPixelAtSDD(getNumberOfPhotonsPerPixelAtSDD() * 10);

        float new_psnr = getPSNR();
        ASSERT_EQ(new_psnr > old_psnr, true);
        old_psnr = new_psnr;
    }
}


//------------------------------
TEST(TestAcquisitionFile, PSNR2)
//------------------------------
{
    clearFiltration();
    setVoltage(150, "kV");
    setNumberOfPhotonsPerPixelAtSDD(16000);

    float new_psnr = getPSNR();
    ASSERT_EQ(new_psnr > 0.0, true);
    float old_psnr = new_psnr;

    // Multiply the flux by 10 for every iteration of the loop
    // The PSNR should improve (less and less noise)
    for (unsigned int i = 1; i < 10; i++) {
        addFilter("Cu", 0.1 * i, "mm");

        float new_psnr = getPSNR();
        ASSERT_EQ(new_psnr < old_psnr, true);
        old_psnr = new_psnr;
    }
}


//--------------------------------------
TEST(TestAcquisitionFile, Magnification)
//--------------------------------------
{
    std::vector<double> object_position = getNodeAndChildrenBoundingBoxCentre("cylinder", "mm");
    setDetectorNumberOfPixels(400, 400);
    setDetectorPixelSize(1.0, 1., "mm");

    setSourcePosition(object_position[0] - 1000, object_position[1], object_position[2], "mm");
    setDetectorPosition(object_position[0] + 500, object_position[1], object_position[2], "mm");

    ASSERT_NEAR(getSDD("mm"), 1500.0, 1e-6);
    ASSERT_NEAR(getSOD("mm"), 1000.0, 1e-6);

    useParallelBeam();
    ASSERT_NEAR(getMagnification(), 1.0, 1e-6);

    useConeBeam();
    ASSERT_NEAR(getMagnification(), 1500.0 / 1000.0, 1e-6);
}


//----------------------------
TEST(TestAcquisitionFile, FOV)
//----------------------------
{
    float magnification = 1500.0 / 1000.0;
    ASSERT_NEAR(getFOV("mm"), 400.0 / magnification, 1e-4);
    ASSERT_NEAR(getFieldOfView("mm"), 400.0 / magnification, 1e-4);

    ASSERT_NEAR(getFOV("cm"), 40.0 / magnification, 1e-5);
    ASSERT_NEAR(getFieldOfView("cm"), 40.0 / magnification, 1e-5);
}


//-------------------------------
TEST(TestAcquisitionFile, N_proj)
//-------------------------------
{
    float magnification = 1500.0F / 1000.0F;
    float FOV = 400.0F / magnification;
    float atan_ratio = atan(2.0F * static_cast<float>(getDetectorPixelSpacing("mm")[0]) / FOV);
    unsigned int n_proj_ref = static_cast<unsigned int>(round(2.0F * M_PI / atan_ratio));
    unsigned int n_proj_test = getOptimalNumberOfProjectionsCT();
    ASSERT_EQ(n_proj_ref, n_proj_test);
}
