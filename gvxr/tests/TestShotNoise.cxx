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

3. Neither the name of the UK Research and Innovation nor the names of its contributors
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
*   @file       TestShotNoise.cxx
*
*   @brief      Unit testing: shot noise:
*               Assess our own implementation of the Poisson noise.
*
*   @date       29/08/2025
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
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Aug 2025-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <cmath>
#include <iostream>
#include <iomanip>
#include <memory>
#include <cstdlib>

#include "gtest/gtest.h"

#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#ifndef GVXR_SCENEGRAPH_NODE_H
#include "gVirtualXRay/SceneGraphNode.h"
#endif

#ifndef GVXR_CYLINDER_MESH_H
#include "gVirtualXRay/CylinderMesh.h"
#endif

#ifndef GVXR_CUBE_MESH_H
#include "gVirtualXRay/CubeMesh.h"
#endif

#ifndef GVXR_XRAY_BEAM_H
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef GVXR_XRAY_DETECTOR_H
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef OpenGLUtilities_h
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef GVXR_MATRIX_4X4_H
#include "gVirtualXRay/Matrix4x4.h"
#endif

#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef GVXR_SHADER_H
#include "gVirtualXRay/Shader.h"
#endif

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

#ifndef GVXR_utils_h
#include "utils.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Constant variables
//******************************************************************************


//******************************************************************************
//  Global variables
//******************************************************************************
Context g_GL_context;

Matrix4x4<GLfloat> g_scene_rotation_matrix;
Matrix4x4<GLfloat> g_detector_rotation_matrix;
Matrix4x4<GLfloat> g_sample_rotation_matrix;

GLfloat g_incident_energy(80.0f * keV);
constexpr GLfloat g_scaling_factor(1.0f);
GLfloat g_resolution(0.15f * mm * g_scaling_factor); // 1mm per pixel
Vec2ui g_number_of_pixels(600 / g_scaling_factor, 600 / g_scaling_factor);
VEC3 g_source_position(0.0f, 0.0f, -10.0f * cm);
VEC3 g_detector_position(0.0f, 0.0f, 10.0f * cm);
VEC3 g_detector_up_vector(0.0f, 1.0f, 0.0f);

SceneGraphNode g_scene_graph("root", nullptr);

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;


//******************************************************************************
//  Function declaration
//******************************************************************************
void loadSTLFile();
void loadDetector();
void loadSource();
void loadXRaySimulator();
void errorCallback(int error, const char* description);
void quit();
void runFlatImageTest(unsigned int aLambdaCutoff, unsigned int aNumberOfPhotonsPerPixel);


// Helper to calculate exact Poisson Probability Mass Function (PMF)
double poissonPMF(double lambda, int k) {
    // return std::pow(lambda, k) * std::exp(-lambda) / std::tgamma(k + 1);

    // Compute the natural log of the Poisson PMF
    double log_pmf = k * std::log(lambda) - lambda - std::lgamma(k + 1);

    // Convert back from log-space to regular space
    return std::exp(log_pmf);
}


//---------------------------------------------
TEST(TestShotNoise, InitialiseSimulationEngine)
//---------------------------------------------
{
    // Make sure the detector has an odd number of columns
    if ((g_number_of_pixels.getX() % 2) == 0)
    {
        g_number_of_pixels.setX(g_number_of_pixels.getX() + 1);
    }

    // Make sure the detector has an odd number of rows
    if ((g_number_of_pixels.getY() % 2) == 0)
    {
        g_number_of_pixels.setY(g_number_of_pixels.getY() + 1);
    }

    // Register the exit callback
    atexit(quit);

    // Initialise OpenGL
    initGL(g_GL_context);

    // Load the data
    loadDetector();
    loadSource();
    loadXRaySimulator();
}


//------------------------------------------
TEST(TestShotNoise, LowPhotonPoissonProfile)
//------------------------------------------
{
    unsigned int lambda_cutoff = 50;
    unsigned int number_of_photons_per_pixel = 49;

    runFlatImageTest(lambda_cutoff, number_of_photons_per_pixel);
}


//--------------------------------------------
TEST(TestShotNoise, HighPhotonGaussianProfile)
//--------------------------------------------
{
    unsigned int lambda_cutoff = 50;
    unsigned int number_of_photons_per_pixel = 500;

    runFlatImageTest(lambda_cutoff, number_of_photons_per_pixel);
}

//----------------------------
TEST(TestShotNoise, AddSample)
//----------------------------
{
    // Add the geometry to the X-ray renderer
    loadSTLFile();
    g_xray_renderer.addInnerSurface(&g_scene_graph.getChild("inside"));
    g_xray_renderer.addOuterSurface(&g_scene_graph.getChild("outside"));

    // Close the window and shut EGL
    quit();
}


//---------
void quit()
//---------
{
    g_xray_renderer.release();
    g_scene_graph.clear();
    g_GL_context.release();
}


//----------------
void loadSTLFile()
//----------------
{
    // Set the geometry
    g_scene_graph.addChild("inside", CylinderMesh<GLdouble>(20, 3.0 * cm, 1.0 * cm));
    g_scene_graph.addChild("outside", CubeMesh<GLdouble>(3.0 * cm));

    // HU values
    RATIONAL_NUMBER HU_bone(1329.885683);
    RATIONAL_NUMBER HU_soft_tissue(51.714752);

    g_scene_graph.getChild("inside").getPolygonMesh().setHounsfieldValue(HU_bone);
    g_scene_graph.getChild("outside").getPolygonMesh().setHounsfieldValue(HU_soft_tissue);

    g_scene_graph.getChild("outside").getPolygonMesh().splitFaces(false);
    g_scene_graph.getChild("outside").getPolygonMesh().splitFaces(false);
    g_scene_graph.getChild("outside").getPolygonMesh().splitFaces(false);
    g_scene_graph.getChild("outside").getPolygonMesh().splitFaces(false);
}


//-----------------
void loadDetector()
//-----------------
{
    g_xray_detector.setResolutionInUnitsOfLengthPerPixel(g_resolution);
    g_xray_detector.setNumberOfPixels(g_number_of_pixels);
    g_xray_detector.setDetectorPosition(g_detector_position);
    g_xray_detector.setUpVector(g_detector_up_vector);
}


//---------------
void loadSource()
//---------------
{
    // Set the energy
    g_xray_beam.initialise(g_incident_energy);

    // Set the source position
    g_xray_detector.setXrayPointSource(g_source_position);
}


//----------------------
void loadXRaySimulator()
//----------------------
{
    // Initialise the X-ray renderer
#ifdef HAS_OPENGL
    g_xray_renderer.initialise(XRayRenderer::OPENGL,
        0, // Half-precision floating-point numbers is enough
        &g_xray_detector,
        &g_xray_beam);
#else
    g_xray_renderer.initialise(XRayRenderer::OPENMP,
        0,
        &g_xray_detector,
        &g_xray_beam);
#endif
}


//--------------------------------------------------------------------------------------
void runFlatImageTest(unsigned int aLambdaCutoff, unsigned int aNumberOfPhotonsPerPixel)
//--------------------------------------------------------------------------------------
{
    g_xray_beam.setLambdaCutoff(aLambdaCutoff);
    LOGGER.logNow("Use lambda cutoff value: ") << aLambdaCutoff << std::endl;


    LOGGER.logNow("Use ") << aNumberOfPhotonsPerPixel << " photons per pixel" << std::endl;

    // Compute the projection set
    g_xray_beam.disablePoissonNoise();
    g_xray_beam.setNumberOfPhotonsPerPixelAtSDD(aNumberOfPhotonsPerPixel, false);
    double total_energy_per_pixel = g_xray_renderer.getTotalEnergyWithDetectorResponse(true);

    // Load reference image
    // g_xray_renderer.computeImage(g_sample_rotation_matrix);
    // Image<XRayRenderer::PixelType> image_fluence_no_noise = g_xray_renderer.getEnergyFluence() / total_energy_per_pixel;
    Image<XRayRenderer::PixelType> image_fluence_no_noise = g_xray_renderer.getWhiteImage(true) / total_energy_per_pixel;

    // Save the image
    // image_fluence_no_noise.saveMHD("TestShotNoise-fluence-no_noise.mhd");

    // Convert in a number of photons
    Image<XRayRenderer::PixelType> image_photon_count_no_noise =image_fluence_no_noise *
        aNumberOfPhotonsPerPixel;

    // Save the image
    // image_photon_count_no_noise.saveMHD("TestShotNoise-photon_count-no_noise.mhd");

    // Compute the noisy image
    g_xray_beam.enablePoissonNoise();
    // g_xray_renderer.computeImage(g_sample_rotation_matrix);
    // Image<XRayRenderer::PixelType> image_fluence_noise = g_xray_renderer.getEnergyFluence() / total_energy_per_pixel;
    Image<XRayRenderer::PixelType> image_fluence_noise = g_xray_renderer.getWhiteImage(true) / total_energy_per_pixel;

    // Save the image
    // image_fluence_noise.saveMHD("TestShotNoise-fluence-noise.mhd");

    // Convert in a number of photons
    Image<XRayRenderer::PixelType> image_photon_count_noise =image_fluence_noise *
        aNumberOfPhotonsPerPixel;

    // Save the image
    // image_photon_count_noise.saveMHD("TestShotNoise-photon_count-noise.mhd");

    // Statistical convergence: Mean and Variance should both roughly equal Lambda
    double mean = image_photon_count_noise.getAverage();
    double variance = image_photon_count_noise.getVariance();
    double skewness = image_photon_count_noise.getSkewness();

    // Noise is present
    EXPECT_GT(variance, 0.0);

    // Low number of photons
    if (aNumberOfPhotonsPerPixel < aLambdaCutoff)
    {
        double expected_skewness = 1.0 / std::sqrt(aNumberOfPhotonsPerPixel);

        EXPECT_NEAR(mean, aNumberOfPhotonsPerPixel, 0.5);
        EXPECT_NEAR(variance, aNumberOfPhotonsPerPixel, 1.5);
        EXPECT_NEAR(skewness, expected_skewness, 0.05)
            << "The low-count Poisson profile tail is structurally warped.";
    }
    // High number of photons
    else
    {
        double expected_skewness = 0.0;

        EXPECT_NEAR(mean, aNumberOfPhotonsPerPixel, 1.0);
        EXPECT_NEAR(variance, aNumberOfPhotonsPerPixel, 5.0);
        EXPECT_NEAR(skewness, expected_skewness, 0.01)
            << "The high-count branch failed to yield a symmetric Gaussian distribution.";
    }
}