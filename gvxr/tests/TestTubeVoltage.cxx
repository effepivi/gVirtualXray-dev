/*

Copyright (c) 2023, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*   @file       TestTubeVoltage.cxx
*
*   @brief      Unit testing: test the tube voltage,
*   			last date of check: Passed test on ?? ??? 202?.
*
*   @version    1.0
*
*   @date       19/09/2023
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
*               http://www.fpvidal.net/, Sept 2023, 2023, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

// #include <cmath>
// #include <iomanip>
// #include <memory>
// #include <cstdlib>

#include "gtest/gtest.h"

// #ifndef __Types_h
// #include "gVirtualXRay/Types.h"
// #endif

#ifndef __Units_h
#include "gVirtualXRay/Units.h"
#endif

// #ifndef __SceneGraphNode_h
// #include "gVirtualXRay/SceneGraphNode.h"
// #endif

// #ifndef __CylinderMesh_h
// #include "gVirtualXRay/CylinderMesh.h"
// #endif

// #ifndef __CubeMesh_h
// #include "gVirtualXRay/CubeMesh.h"
// #endif

// #ifndef __XRayBeam_h
// #include "gVirtualXRay/XRayBeam.h"
// #endif

// #ifndef __XRayDetector_h
// #include "gVirtualXRay/XRayDetector.h"
// #endif

// #ifndef __OpenGLUtilities_h
// #include "gVirtualXRay/OpenGLUtilities.h"
// #endif

// #ifndef __Matrix4x4_h
// #include "gVirtualXRay/Matrix4x4.h"
// #endif

// #ifndef __XRayRenderer_h
// #include "gVirtualXRay/XRayRenderer.h"
// #endif

// #ifndef __Shader_h
// #include "gVirtualXRay/Shader.h"
// #endif

// #ifndef __Utilities_h
// #include "gVirtualXRay/Utilities.h"
// #endif

// #ifndef __Exception_h
// #include "gVirtualXRay/Exception.h"
// #endif

// #ifndef __Context_h
// #include "gVirtualXRay/Context.h"
// #endif

#ifndef __XRayBeam_h
#include <gVirtualXRay/XRayBeam.h>
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;
//using namespace std;


//******************************************************************************
//  Global variables
//******************************************************************************
// Context g_GL_context;

// Matrix4x4<GLfloat> g_scene_rotation_matrix;
// Matrix4x4<GLfloat> g_detector_rotation_matrix;
// Matrix4x4<GLfloat> g_sample_rotation_matrix;

// GLfloat g_incident_energy_0(100.0 * keV);
// GLfloat g_incident_energy_1(200.0 * keV);
// GLfloat g_incident_energy_2(300.0 * keV);

// GLfloat g_number_of_photon_in_energy_channel_0(10.0);
// GLfloat g_number_of_photon_in_energy_channel_1(20.0);
// GLfloat g_number_of_photon_in_energy_channel_2(10.0);

// const GLfloat g_scaling_factor(2.0);
// GLfloat g_resolution(0.15 * mm * g_scaling_factor); // 1mm per pixel
// Vec2ui g_number_of_pixels(600 / g_scaling_factor, 600 / g_scaling_factor);
// VEC3 g_source_position(   0.0, 0.0, -10.0 * cm);
// VEC3 g_detector_position( 0.0, 0.0,  10.0 * cm);
// VEC3 g_detector_up_vector(0.0, 1.0,   0.0);

// SceneGraphNode g_scene_graph("root", NULL);

XRayBeam g_xray_beam;
// XRayDetector g_xray_detector;
// XRayRenderer g_xray_half_precision_renderer;
// XRayRenderer g_xray_full_precision_renderer;
// XRayRenderer g_xray_openmp_renderer;

// RATIONAL_NUMBER g_literature_result(0.0);
// RATIONAL_NUMBER g_expected_result(0.0);

//******************************************************************************
//  Function declaration
//******************************************************************************
// void initGL();
// void loadSTLFile();
// void loadDetector();
// void loadSource();
// void loadXRaySimulator();
// void errorCallback(int error, const char* description);
void quit();


//------------------------------------
TEST (TestTubeVoltage, TestSetVoltage)
//------------------------------------
{
    unsigned int number_of_bins = 0;

    std::cout << "Is Spekpy installed: " << (g_xray_beam.hasSpekpy() ? "YES" : "NO") << std::endl;
    std::cout << "Is Xpecgen installed: " << (g_xray_beam.hasXpecgen() ? "YES" : "NO") << std::endl;

    number_of_bins = g_xray_beam.getEnergyChannelNumber();
    std::cout << "Number of bins: " << number_of_bins << std::endl << std::endl;

    std::cout << "Use Spekpy? " << (g_xray_beam.usingSpekpy() ? "YES" : "NO") << std::endl;
    std::cout << "Use Xpecgen?: " << (g_xray_beam.usingXpecgen() ? "YES" : "NO") << std::endl;

    number_of_bins = g_xray_beam.getEnergyChannelNumber();
    std::cout << "Number of bins: " << number_of_bins << std::endl << std::endl;

    // g_xray_beam.useSpekpy();
    // std::cout << "Use Spekpy? " << (g_xray_beam.usingSpekpy() ? "YES" : "NO") << std::endl;
    // std::cout << "Use Xpecgen?: " << (g_xray_beam.usingXpecgen() ? "YES" : "NO") << std::endl;

    // g_xray_beam.setVoltage(80 * kV);

    // number_of_bins = g_xray_beam.getEnergyChannelNumber();
    // std::cout << "Number of bins: " << number_of_bins << std::endl << std::endl;

    // g_xray_beam.useXpecgen();
    std::cout << "Use Spekpy? " << (g_xray_beam.usingSpekpy() ? "YES" : "NO") << std::endl;
    std::cout << "Use Xpecgen?: " << (g_xray_beam.usingXpecgen() ? "YES" : "NO") << std::endl;

    number_of_bins = g_xray_beam.getEnergyChannelNumber();
    std::cout << "Number of bins: " << number_of_bins << std::endl << std::endl;

    g_xray_beam.setVoltage(80 * kV);
    number_of_bins = g_xray_beam.getEnergyChannelNumber();
    std::cout << "Number of bins: " << number_of_bins << std::endl << std::endl;

    g_xray_beam.setVoltage(150 * kV);
    number_of_bins = g_xray_beam.getEnergyChannelNumber();
    std::cout << "Number of bins: " << number_of_bins << std::endl << std::endl;


    // for (unsigned int i = 0; i < number_of_bins; ++i)
    // {
    //     SpectrumRecord record = g_xray_beam.getEnergyChannel(i);
    //     std::cout << "(" << i << ")\t" << record.getPhotonNumber() << " photons of " << record.getPhotonEnergy() / keV << " keV" << std::endl;
    // }

    // Register the exit callback
    atexit(quit);
}


//---------
void quit()
//---------
{
}

