/*

Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*   @file       TestDensity.cxx
*
*   @brief      Unit testing: density.
*
*   @version    1.0
*
*   @date       26/12/2014
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
*               http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#define BOOST_TEST_MODULE TestDensity
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#ifdef HAS_GLEW
#include <GL/glew.h>
#endif

#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef __XRayRenderer_h
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef __InvalidInternalFormalException_h
#include "gVirtualXRay/InvalidInternalFormalException.h"
#endif

#ifndef __InvalidImageSizeException_h
#include "gVirtualXRay/InvalidImageSizeException.h"
#endif

#ifndef __NoDetectorException_h
#include "gVirtualXRay/NoDetectorException.h"
#endif

#ifndef __NoBeamException_h
#include "gVirtualXRay/NoBeamException.h"
#endif



//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Const
//******************************************************************************
const short HU_bone(1330);
const short HU_brain(37);
const short HU_gas(-998);
const short HU_soft_tissue(52);
const short HU_water(0);


//-------------------------------
BOOST_AUTO_TEST_CASE(TestDensity)
//-------------------------------
{
    // Initialise the X-ray renderer
    XRayRenderer g_xray_renderer;

    // Computed density values in g/cm3
    double rho_bone(       g_xray_renderer.getDensityFromHU(HU_bone)        / (g/cm3));
    double rho_brain(      g_xray_renderer.getDensityFromHU(HU_brain)       / (g/cm3));
    double rho_gas(        g_xray_renderer.getDensityFromHU(HU_gas)         / (g/cm3));
    double rho_soft_tissue(g_xray_renderer.getDensityFromHU(HU_soft_tissue) / (g/cm3));
    double rho_water(      g_xray_renderer.getDensityFromHU(HU_water)       / (g/cm3));

    BOOST_CHECK_CLOSE(rho_brain,       1.040E+00, 5.0);
    BOOST_CHECK_CLOSE(rho_soft_tissue, 1.060E+00, 5.0);
    BOOST_CHECK_CLOSE(rho_water,       1.000E+00, 5.0);
}


//--------------------------------------------------
BOOST_AUTO_TEST_CASE(TestMassAttenuationCoefficient)
//--------------------------------------------------
{
    // Initialise the X-ray renderer
    XRayRenderer g_xray_renderer;

    // Computed mass attenuation coefficients in cm2/g
    double mass_attenuation_coefficient_bone(       g_xray_renderer.getMassAttenuationFromHU(HU_bone,        80.0 * keV) / (cm2 / g));
    double mass_attenuation_coefficient_brain(      g_xray_renderer.getMassAttenuationFromHU(HU_brain,       80.0 * keV) / (cm2 / g));
    double mass_attenuation_coefficient_gas(        g_xray_renderer.getMassAttenuationFromHU(HU_gas,         80.0 * keV) / (cm2 / g));
    double mass_attenuation_coefficient_soft_tissue(g_xray_renderer.getMassAttenuationFromHU(HU_soft_tissue, 80.0 * keV) / (cm2 / g));
    double mass_attenuation_coefficient_water(      g_xray_renderer.getMassAttenuationFromHU(HU_water,       80.0 * keV) / (cm2 / g));

    BOOST_CHECK_CLOSE(mass_attenuation_coefficient_brain,       1.831E-01, 5.0);
    BOOST_CHECK_CLOSE(mass_attenuation_coefficient_soft_tissue, 1.823E-01, 5.0);
    BOOST_CHECK_CLOSE(mass_attenuation_coefficient_water,       1.837E-01, 5.0);
}


//----------------------------------------------------
BOOST_AUTO_TEST_CASE(TestLinearAttenuationCoefficient)
//----------------------------------------------------
{
    // Initialise the X-ray renderer
    XRayRenderer g_xray_renderer;

    // Computed linear attenuation coefficients in cm-1
    double linear_attenuation_coefficient_bone(       g_xray_renderer.getMuFromHU(HU_bone,        80.0 * keV) / (1.0 / cm));
    double linear_attenuation_coefficient_brain(      g_xray_renderer.getMuFromHU(HU_brain,       80.0 * keV) / (1.0 / cm));
    double linear_attenuation_coefficient_gas(        g_xray_renderer.getMuFromHU(HU_gas,         80.0 * keV) / (1.0 / cm));
    double linear_attenuation_coefficient_soft_tissue(g_xray_renderer.getMuFromHU(HU_soft_tissue, 80.0 * keV) / (1.0 / cm));
    double linear_attenuation_coefficient_water(      g_xray_renderer.getMuFromHU(HU_water,       80.0 * keV) / (1.0 / cm));

    BOOST_CHECK_CLOSE(linear_attenuation_coefficient_brain,       1.831E-01, 5.0);
    BOOST_CHECK_CLOSE(linear_attenuation_coefficient_soft_tissue, 1.932E-01, 5.0);
    BOOST_CHECK_CLOSE(linear_attenuation_coefficient_water,       1.837E-01, 5.0);
}





















/*
BOOST_AUTO_TEST_CASE(InvalidInternalFormal)
{
    // Initialise the X-ray renderer
    XRayRenderer g_xray_renderer;
    XRayDetector g_xray_detector;
    XRayBeam g_xray_beam;
    
    BOOST_CHECK_THROW(g_xray_renderer.initialise(XRayRenderer::OPENGL,
        GL_RGB,
        &g_xray_detector,
        &g_xray_beam), InvalidInternalFormalException);
}


BOOST_AUTO_TEST_CASE(InvalidaImageSize)
{
    // Initialise the X-ray renderer
    XRayRenderer g_xray_renderer;
    XRayDetector g_xray_detector;
    XRayBeam g_xray_beam;

    BOOST_CHECK_THROW(g_xray_renderer.initialise(XRayRenderer::OPENGL,
            GL_RGB16F,
            &g_xray_detector,
            &g_xray_beam), InvalidImageSizeException);
}


BOOST_AUTO_TEST_CASE(NoDetector)
{
    // Initialise the X-ray renderer
    XRayRenderer g_xray_renderer;
    XRayBeam g_xray_beam;
    
    BOOST_CHECK_THROW(g_xray_renderer.initialise(XRayRenderer::OPENGL,
        GL_RGB16F,
        0,
        &g_xray_beam), NoDetectorException);
}


BOOST_AUTO_TEST_CASE(NoBeam)
{
    // Initialise the X-ray renderer
    XRayRenderer g_xray_renderer;
    XRayDetector g_xray_detector;
    BOOST_CHECK_THROW(g_xray_renderer.initialise(XRayRenderer::OPENGL,
            GL_RGB16F,
            &g_xray_detector,
            0), NoBeamException);
}


*/

/*
BOOST_AUTO_TEST_CASE(AtomicNumber)
{
    XRayRenderer g_xray_renderer;

	// Initialise the X-ray renderer
    g_xray_renderer.initialise(XRayRenderer::OPENGL,
            GL_RGB16F,
            0,
            0);
*/
  /*  BOOST_CHECK_EQUAL(4, 2 * 2);
}

BOOST_AUTO_TEST_CASE(FailTest)
{
    BOOST_CHECK_EQUAL(4, 2 * 2);
}

int add( int i, int j ) { return i+j; }

BOOST_AUTO_TEST_CASE( my_test )
{
    // seven ways to detect and report the same error:
    BOOST_CHECK( add( 2,2 ) == 4 );        // #1 continues on error

    BOOST_REQUIRE( add( 2,2 ) == 4 );      // #2 throws on error

    if( add( 2,2 ) != 4 )
      BOOST_ERROR( "Ouch..." );            // #3 continues on error

    if( add( 2,2 ) != 4 )
      BOOST_FAIL( "Ouch..." );             // #4 throws on error

    if( add( 2,2 ) != 4 ) throw "Ouch..."; // #5 throws on error

    BOOST_CHECK_MESSAGE( add( 2,2 ) == 4,  // #6 continues on error
                         "add(..) result: " << add( 2,2 ) );

    BOOST_CHECK_EQUAL( add( 2,2 ), 4 );	  // #7 continues on error
}

BOOST_AUTO_TEST_CASE(testMissingServer)
{
  /*MyClient client;
  BOOST_CHECK_THROW(
    client.connect(someNonexistentServerIP, somePort), 
    boost::asio::error::Connection_refused
  );*/
//}
