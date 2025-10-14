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
*   @file       TestGain.cxx
*
*   @brief      Unit testing: detector gain.
*
*   @version    1.0
*
*   @date       14/10/2025
*   @author     Prof Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk),
*   https://gvirtualxray.sourceforge.io/, Oct 2025, 2025, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <cmath>
#include <iomanip>
#include <memory>

#include "gtest/gtest.h"
#include "gVirtualXRay/XRayRenderer.h"

#ifndef __Logger_h
#include "gVirtualXRay/Logger.h"
#endif

#ifndef __Image_h
#include "gVirtualXRay/Image.h"
#endif

#ifndef __SimpleGVXR_h
#include "SimpleGVXR.h"
#endif


//******************************************************************************
//  Global variables
//******************************************************************************
int g_main_window_width(1280 / 2.0);
int g_main_window_height(800 / 2.0);

double g_incident_energy_in_keV(80.0);
const double g_scaling_factor(2.0);
double g_resolution_in_mm = (0.15 * g_scaling_factor); // 1mm per pixel
int g_number_of_pixels[] = {int(600 / g_scaling_factor), int(600 / g_scaling_factor)};
double g_source_position_in_cm[]   = {0.0, 0.0, -10.0};
double g_detector_position_in_cm[] = {0.0, 0.0,  10.0};
double g_detector_up_vector[]      = {0.0, 1.0,  0.0};
double g_detector_gain(3000.0);
double g_source_size_in_cm(0.5);


//******************************************************************************
//  Function declaration
//******************************************************************************
void loadSTLFile();
void loadDetector();
void loadSource();


//--------------------------------------
TEST(TestGain, computeXRayImageWithGain)
//--------------------------------------
{
    // Make sure the detector has an odd number of columns
    if ((g_number_of_pixels[0] % 2) == 0)
    {
        g_number_of_pixels[0] = g_number_of_pixels[0] + 1;
    }

    // Make sure the detector has an odd number of rows
    if ((g_number_of_pixels[1] % 2) == 0)
    {
        g_number_of_pixels[1] = g_number_of_pixels[1] + 1;
    }

    // Create an OpenGL context
#ifdef HAS_EGL
    createWindow(-1, 0, "EGL", 3, 2, 0);
#else // HAS_EGL
#ifdef __APPLE__
    // OpenGL 4.2 and above are not supported on MacOS (at least on my compute with MacOS Big Sur)
    createWindow(-1, 0, "OPENGL", 4, 1, 0);
#else // __APPLE__
    createWindow(-1, 0, "OPENGL", 4, 6, 0);
#endif // __APPLE__
#endif // HAS_EGL

    setWindowSize(g_main_window_width, g_main_window_height);

    // Load the data
    loadDetector();
    loadSource();

    // Add the geometry to the X-ray renderer
    loadSTLFile();
    addPolygonMeshAsInnerSurface("inside");
    addPolygonMeshAsOuterSurface("outside");

    // Enable artefact filtering on GPU
    enableArtifactFilteringOnGPU();

    // Compute the images without and with gain
    auto x_ray_image_float = computeXRayImage();
    auto x_ray_image_ushort = computeXRayImageWithGain();

    // Go through every row of the images
    auto ite_row_float = x_ray_image_float.begin();
    for (auto ite_row_ushort = x_ray_image_ushort.begin(); ite_row_ushort != x_ray_image_ushort.end(); ++ite_row_float, ++ite_row_ushort) {

        // Go through every column of the images
        auto ite_col_float = ite_row_float->begin();
        for (auto ite_col_ushort = ite_row_ushort->begin(); ite_col_ushort != ite_row_ushort->end(); ++ite_col_float, ++ite_col_ushort) {
            ASSERT_NEAR(*ite_col_ushort / *ite_col_float, g_detector_gain, 0.5);
        }
    }
}


//---------------------------------------
TEST(TestGain, saveLastXRayImageWithGain)
//---------------------------------------
{
    saveLastXRayImage("test_gain-float.tif");
    saveLastXRayImageWithGain("test_gain-ushort.tif");

    gVirtualXRay::Image<float> image_float("test_gain-float.tif");
    gVirtualXRay::Image<unsigned short> image_ushort("test_gain-ushort.tif");

    ASSERT_EQ(image_float.getWidth(), image_ushort.getWidth());
    ASSERT_EQ(image_float.getHeight(), image_ushort.getHeight());
    ASSERT_EQ(image_float.getDepth(), image_ushort.getDepth());

    for (int i = 0; i < image_float.getWidth() * image_float.getHeight() * image_float.getDepth(); ++i) {
        ASSERT_NEAR(image_ushort.getRawData()[i] / image_float.getRawData()[i], g_detector_gain, 0.5);
    }

    // Close the window
    destroyAllWindows();
}


//----------------
void loadSTLFile()
//----------------
{
	// Set the geometry
  // Set the geometry
  makeCube("outside",
          3.0,
          "cm",
          "root");

  makeCylinder("inside",
          20,
          3.0,
          1.0,
          "cm",
          "root");

    // HU values
    double HU_bone(1329.885683);
    double HU_soft_tissue(51.714752);

    setHU( "inside", HU_bone);
    setHU("outside", HU_soft_tissue);
}


//-----------------
void loadDetector()
//-----------------
{
    setDetectorPixelSize(g_resolution_in_mm, g_resolution_in_mm , "mm");

    setDetectorNumberOfPixels(g_number_of_pixels[0], g_number_of_pixels[1]);

    setDetectorPosition(
        g_detector_position_in_cm[0],
        g_detector_position_in_cm[1],
        g_detector_position_in_cm[2],
        "cm");

    setDetectorUpVector(
        g_detector_up_vector[0],
        g_detector_up_vector[1],
        g_detector_up_vector[2]);

    setDetectorGain(g_detector_gain);
}


//---------------
void loadSource()
//---------------
{
    // Set the energy
    setMonoChromaticPerPixelAtSDD(g_incident_energy_in_keV, "keV", 100);

    // No noise
    disablePoissonNoise();

    // Set the source position
    usePointSource();
    setSourcePosition(g_source_position_in_cm[0],
        g_source_position_in_cm[1],
        g_source_position_in_cm[2],
        "cm");
}
