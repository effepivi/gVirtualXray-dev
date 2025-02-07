/*

Copyright (c) 2024-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk),
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
*   @file       TestRayTracing.cxx
*
*   @brief      Unit testing: ray tracing and artefact correction
*
*   @date       25/09/2024
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
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Sept 2024-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

//#include <cmath>
//#include <iomanip>
//#include <memory>
//#include <cstdlib>
//
#include "gtest/gtest.h"
//
//#ifndef __Types_h
//#include "gVirtualXRay/Types.h"
//#endif
//
//#ifndef __Units_h
//#include "gVirtualXRay/Units.h"
//#endif
//
#ifndef __CylinderMesh_h
#include "gVirtualXRay/CylinderMesh.h"
#endif
//
//#ifndef __CubeMesh_h
//#include "gVirtualXRay/CubeMesh.h"
//#endif
//
//#ifndef __XRayBeam_h
//#include "gVirtualXRay/XRayBeam.h"
//#endif
//
//#ifndef __XRayDetector_h
//#include "gVirtualXRay/XRayDetector.h"
//#endif
//
//#ifndef __OpenGLUtilities_h
//#include "gVirtualXRay/OpenGLUtilities.h"
//#endif
//
#ifndef __Matrix4x4_h
#include "gVirtualXRay/Matrix4x4.h"
#endif

#ifndef __XRayRenderer_h
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef __Context_h
#include "gVirtualXRay/Context.h"
#endif
//
//#ifndef __Utilities_h
//#include "gVirtualXRay/Utilities.h"
//#endif
//
//#ifndef __Exception_h
//#include "gVirtualXRay/Exception.h"
//#endif
//
//#ifndef __Logger_h
//#include "gVirtualXRay/Logger.h"
//#endif

#ifndef __utils_h
#include "utils.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Global variables
//******************************************************************************
Context g_GL_context;

Matrix4x4<GLfloat> g_scene_rotation_matrix;
Matrix4x4<GLfloat> g_detector_rotation_matrix;
Matrix4x4<GLfloat> g_sample_rotation_matrix;

Vec2ui g_number_of_pixels(500, 500);
VEC3 g_detector_up_vector(0.0, 0.0, -1.0);
VEC3 g_detector_position(100.0 * mm, 0.0, 0.0);
VEC3 g_source_position(-400.0 * mm, 0.0, 0.0 * cm);
float g_resolution(250 * um);

GLfloat g_incident_energy(80.0 * keV);

SceneGraphNode g_scene_graph("root", NULL);

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_half_precision_renderer;
XRayRenderer g_xray_full_precision_renderer;
XRayRenderer g_xray_openmp_renderer;


//******************************************************************************
//  Function declaration
//******************************************************************************
void loadSTLFile();
void loadDetector();
void loadSource();
void loadXRaySimulator();
void errorCallback(int error, const char* description);
void quit();


//--------------------------------------------------------------------
TEST(TestRayTracing, TestBeerLambertLawPolyChromatic)
//--------------------------------------------------------------------
{
    // Register the exit callback
    atexit(quit);

    // Initialise OpenGL
    initGL(g_GL_context);

    // Load the data
    loadDetector();
    loadSource();
    loadXRaySimulator();

    // Add the geometry to the X-ray renderer
    loadSTLFile();
    g_xray_half_precision_renderer.addInnerSurface(&g_scene_graph.getChild("inside"));
    g_xray_full_precision_renderer.addInnerSurface(&g_scene_graph.getChild("inside"));
    g_xray_openmp_renderer.addInnerSurface(&g_scene_graph.getChild("inside"));

    // Enable artefact filtering on GPU
    g_xray_half_precision_renderer.enableArtifactFilteringOnCPU();
    g_xray_full_precision_renderer.enableArtifactFilteringOnCPU();

    // Compute the X-ray image
    g_sample_rotation_matrix = Matrix4x4<GLfloat>::buildScaleMatrix(2.5 * mm, 2.5 * mm, 2.5 * mm);
    g_xray_half_precision_renderer.computeImage(g_sample_rotation_matrix);
    g_xray_full_precision_renderer.computeImage(g_sample_rotation_matrix);
//    g_xray_openmp_renderer.computeImage(g_sample_rotation_matrix);

    g_xray_half_precision_renderer.printLBuffer("g_xray_half_precision_renderer.mhd");
    g_xray_full_precision_renderer.printLBuffer("g_xray_full_precision_renderer.mhd");
//    g_xray_openmp_renderer.printLBuffer("g_xray_openmp_renderer.mhd");

    // Close the window and shut EGL
    quit();

}


//---------
void quit()
//---------
{
    g_xray_full_precision_renderer.release();
    g_xray_half_precision_renderer.release();
    g_xray_openmp_renderer.release();
    g_scene_graph.clear();
    g_GL_context.release();
}


//----------------
void loadSTLFile()
//----------------
{
	// Set the geometry
//    g_scene_graph.addChild( "inside", CylinderMesh<GLfloat>(10, 1.0 * cm *  2.5 * mm, 0.5 * cm * 2.5 * mm));
    g_scene_graph.addChild( "inside", CylinderMesh<GLfloat>(10, 1.0 * cm, 0.5 * cm));
    g_scene_graph.getChild( "inside").getPolygonMesh().setElement("C");
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

    // Use a parallel source
//    g_xray_detector.setParallelBeam();
}


//----------------------
void loadXRaySimulator()
//----------------------
{
    // Initialise the X-ray renderer
    g_xray_half_precision_renderer.initialise(XRayRenderer::OPENGL,
        GL_RGB16F,
        &g_xray_detector,
        &g_xray_beam);

    g_xray_full_precision_renderer.initialise(XRayRenderer::OPENGL,
        GL_RGB32F,
        &g_xray_detector,
        &g_xray_beam);

    g_xray_openmp_renderer.initialise(XRayRenderer::OPENMP,
        0,
        &g_xray_detector,
        &g_xray_beam);
}
