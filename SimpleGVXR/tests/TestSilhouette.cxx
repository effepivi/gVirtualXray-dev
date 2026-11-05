/*

Copyright (c) 2026, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*   @file       TestSilhouette.cxx
*
*   @brief      Unit testing: Silhouette.
*               A cube is projected onto the detector using a point source.
*               The silhouette must be 255 where the cube is visible, and 0
*               everywhere else. It is compared with the L-buffer of the same
*               cube, which is non-zero over the same area.
*
*   @version    1.0
*
*   @date       14/09/2026
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
*               http://www.fpvidal.net/, Sep 2026, 2026, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <cmath>
#include <vector>

#include "gtest/gtest.h"

#ifndef SGVXR_SimpleGVXR_h
#include "SimpleGVXR.h"
#endif


//******************************************************************************
//  Global variables
//******************************************************************************
const int g_main_window_width(1280 / 2);
const int g_main_window_height(800 / 2);

const int g_number_of_pixels[] = {301, 301};
const double g_resolution_in_mm(0.5);

const double g_cube_length_in_cm(2.0);

const double g_source_position_in_cm[]   = {0.0, 0.0, -10.0};
const double g_detector_position_in_cm[] = {0.0, 0.0,  10.0};
const double g_detector_up_vector[]      = {0.0, 1.0,  0.0};


//******************************************************************************
//  Function declaration
//******************************************************************************
void loadDetector();
void loadSource();


//----------------------------------
TEST(TestSilhouette, TestSilhouette)
//----------------------------------
{
    // Create an OpenGL context
#ifdef HAS_OPENGL
#ifdef HAS_EGL
    createWindow(-1, 0, "EGL", 3, 2, 0);
#else // HAS_EGL
#ifdef __APPLE__
    // OpenGL 4.2 and above are not supported on MacOS
    createWindow(-1, 0, "OPENGL", 4, 1, 0);
#else // __APPLE__
    createWindow(-1, 0, "OPENGL", 4, 6, 0);
#endif // __APPLE__
#endif // HAS_EGL
#else // HAS_OPENGL
    createWindow(-1, 0, "OPENMP");
#endif // HAS_OPENGL

    setWindowSize(g_main_window_width, g_main_window_height);

    // Load the data
    loadSource();
    loadDetector();

    // Add the cube to the X-ray renderer
    makeCube("cube", g_cube_length_in_cm, "cm");
    setElement("cube", "Al");
    moveToCentre("cube");
    addPolygonMeshAsInnerSurface("cube");

    // Enable artefact filtering on GPU
    enableArtifactFilteringOnGPU();

    // Compute the L-buffer and the silhouette of the same cube
    std::vector<std::vector<float> > lbuffer = computeLBuffer("cube");
    std::vector<std::vector<unsigned char> > silhouette =
        computeSilhouette("cube");

    // The accessor returns the same silhouette
    std::vector<std::vector<unsigned char> > silhouette_again =
        getLastSilhouette();

    // Close the window
    destroyAllWindows();

    // The silhouette has the same size as the L-buffer
    ASSERT_EQ(lbuffer.size(), silhouette.size());
    ASSERT_EQ(std::size_t(g_number_of_pixels[1]), silhouette.size());
    ASSERT_EQ(std::size_t(g_number_of_pixels[0]), silhouette[0].size());
    ASSERT_EQ(silhouette, silhouette_again);

    // The cube is centred on the beam axis, it covers the centre of the
    // detector but not its corners
    ASSERT_EQ(255, int(silhouette[g_number_of_pixels[1] / 2][g_number_of_pixels[0] / 2]));
    ASSERT_EQ(0, int(silhouette[0][0]));
    ASSERT_EQ(0, int(silhouette[g_number_of_pixels[1] - 1][g_number_of_pixels[0] - 1]));

    // Compare the silhouette with the L-buffer
    const float epsilon_in_cm(1e-3);

    unsigned int number_of_pixels_in_silhouette(0);
    unsigned int number_of_pixels_in_lbuffer(0);

    for (int j = 0; j < g_number_of_pixels[1]; ++j)
    {
        for (int i = 0; i < g_number_of_pixels[0]; ++i)
        {
            // Every pixel is either 0 or 255
            ASSERT_TRUE(silhouette[j][i] == 0 || silhouette[j][i] == 255) <<
                "silhouette[" << j << "][" << i << "] = " <<
                int(silhouette[j][i]);

            if (silhouette[j][i] == 255) ++number_of_pixels_in_silhouette;

            // A pixel of the L-buffer that holds a path length must belong
            // to the silhouette
            if (lbuffer[j][i] > epsilon_in_cm)
            {
                ++number_of_pixels_in_lbuffer;
                ASSERT_EQ(255, int(silhouette[j][i])) <<
                    "lbuffer[" << j << "][" << i << "] = " << lbuffer[j][i];
            }

            // There should be the same number in both, maybe less in the L-buffer
            // due to epsilon
            ASSERT_LE(number_of_pixels_in_lbuffer, number_of_pixels_in_silhouette);
        }
    }

    // The L-buffer is zero on the edges of the cube, where the entry and the
    // exit points are the same, so the silhouette cannot be smaller than it
    ASSERT_LE(number_of_pixels_in_lbuffer, number_of_pixels_in_silhouette);

    // The cube is aligned with the detector, its silhouette is a square whose
    // side is its length magnified by the cone beam. The widest projection is
    // the one of the face that faces the source
    const double magnification(
        (g_detector_position_in_cm[2] - g_source_position_in_cm[2]) /
        (-0.5 * g_cube_length_in_cm - g_source_position_in_cm[2]));

    const double side_in_pixels(
        magnification * g_cube_length_in_cm * 10.0 / g_resolution_in_mm);

    const double expected_number_of_pixels(side_in_pixels * side_in_pixels);

    // The tolerance accounts for the pixels of the border of the silhouette,
    // whose centre may fall on either side of the edge of the square
    ASSERT_NEAR(1.0,
        number_of_pixels_in_silhouette / expected_number_of_pixels,
        0.05);
}


//-------------------
void loadDetector()
//-------------------
{
    setDetectorPosition(g_detector_position_in_cm[0],
        g_detector_position_in_cm[1],
        g_detector_position_in_cm[2],
        "cm");

    setDetectorUpVector(g_detector_up_vector[0],
        g_detector_up_vector[1],
        g_detector_up_vector[2]);

    setDetectorNumberOfPixels(g_number_of_pixels[0], g_number_of_pixels[1]);

    setDetectorPixelSize(g_resolution_in_mm, g_resolution_in_mm, "mm");
}


//-----------------
void loadSource()
//-----------------
{
    setSourcePosition(g_source_position_in_cm[0],
        g_source_position_in_cm[1],
        g_source_position_in_cm[2],
        "cm");

    usePointSource();

    setMonoChromatic(80.0, "keV", 1000);
}
