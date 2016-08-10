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
*	@file		validation_01.cxx
*
*	@brief		Main program to test the value of mass attenuation coefficients
*				for various tissues.
*
*	@version	1.0
*
*	@date		15/10/2014
*
*	@author		Dr Franck P. Vidal
*
*	@section	License
*				BSD 3-Clause License.
*
*				For details on use and redistribution please refer
*				to http://opensource.org/licenses/BSD-3-Clause
*
*	@section	Copyright
*				(c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*				http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
*				BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifdef HAS_GLEW
#include <GL/glew.h>
#endif

#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <cstdlib>

#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>

#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//	Defines
//******************************************************************************
#define PREFIX "../data/validation_01"


//******************************************************************************
//	Global variables
//******************************************************************************
RATIONAL_NUMBER g_incident_energy(80.0 * keV);
Vec2ui g_number_of_pixels(64, 64);
GLfloat g_resolution(0.15 * mm); // 1mm per pixel

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;


//******************************************************************************
//	Function declaration
//******************************************************************************
void initGLEW();
void loadDetector();
void loadSource();
void loadXRaySimulator();
void errorCallback(int error, const char* description);
void printAttenuationCoeffiecient(double& anEnergy, double aThreshold, double anOffset, std::ostream& anOutput);

//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
	try
	{
	    // Set an error callback
	    glfwSetErrorCallback(errorCallback);

	    // Initialize GLFW
	    if (!glfwInit())
	    {
	    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: cannot initialise GLFW.");
	    }

        // Enable OpenGL 2.1 if possible
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	    // Create the window
	    GLFWwindow* p_main_window_id(glfwCreateWindow(64, 64, "", 0, 0));
	    if (!p_main_window_id)
	    {
	        glfwTerminate();
	    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: cannot create a GLFW windowed mode window and its OpenGL context.");
	    }

	    // Make the window's context current
	    glfwMakeContextCurrent(p_main_window_id);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Initialise GLEW
        initGLEW();

        // Load the data
		loadDetector();
		loadSource();
		loadXRaySimulator();

		// Print the energy in MeV and the attenuation coefficient of water in cm2/g
		std::ofstream test_stream(PREFIX"/test_mass_attenuation_coefficients.csv");

        if (!test_stream.is_open())
        {
            std::stringstream error_message;
            error_message << "ERROR: cannot open file:\t" << PREFIX"/test_mass_attenuation_coefficients.csv";

            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
        }

        double energy(0);

        test_stream     << "\"Energy in MeV\"\t" <<
                "\"water_xcom\"\t" <<
                "\"water_HU (linear)\"\t" <<
                "\"water_HU (log)\"\t" <<
                "\"bone_xcom\"\t" <<
                "\"bone_HU (linear)\"\t" <<
                "\"bone_HU (log)\"\t" <<
                "\"brain_xcom\"\t" <<
                "\"brain_HU (linear)\"\t" <<
                "\"brain_HU (log)\"\t" <<
                "\"gas_xcom\"\t" <<
                "\"gas_HU (linear)\"\t" <<
                "\"gas_HU (log)\"\t" <<
                "\"soft_tissue_xcom\"\t" <<
                "\"soft_tissue_HU (linear)\"\t" <<
                "\"soft_tissue_HU (log)\"" <<
                std::endl;

        energy = 1.00E-003 * MeV;

        printAttenuationCoeffiecient(energy, 1e-2 * MeV, 0.00001, test_stream);
        printAttenuationCoeffiecient(energy,  0.5 * MeV, 0.0001,  test_stream);
        printAttenuationCoeffiecient(energy, 10E2 * MeV, 0.1,     test_stream);

		// Close the window and shut GLFW
		if (p_main_window_id)
		{
			glfwDestroyWindow(p_main_window_id);
			p_main_window_id = 0;
			glfwTerminate();
		}
	}
	// Catch exception if any
	catch (const std::exception& error)
	{
		std::cerr << error.what() << std::endl;
	}

	return (EXIT_SUCCESS);
}


//-------------
void initGLEW()
//-------------
{
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::stringstream error_message;
        error_message << "ERROR: cannot initialise GLEW:\t" << glewGetErrorString(err);

        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
    }
}


//-----------------
void loadDetector()
//-----------------
{
	g_xray_detector.setResolutionInUnitsOfLengthPerPixel(g_resolution);
	g_xray_detector.setNumberOfPixels(g_number_of_pixels);
}


//---------------
void loadSource()
//---------------
{
	// Set the energy
	g_xray_beam.initialise(g_incident_energy);
}


//----------------------
void loadXRaySimulator()
//----------------------
{
	// Initialise the X-ray renderer
    g_xray_renderer.initialise(XRayRenderer::OPENGL,
            GL_RGB16F,
            &g_xray_detector,
            &g_xray_beam);
}


//----------------------------------------------------
void errorCallback(int error, const char* description)
//----------------------------------------------------
{
	std::cerr << "GLFW error: " << description << std::endl;
}


//-------------------------------------------------------
void printAttenuationCoeffiecient(double& anEnergy,
								  double aThreshold,
								  double anOffset,
								  std::ostream& anOutput)
//-------------------------------------------------------
{
    double HU_bone(1330);
    double HU_brain(37);
    double HU_gas(-998);
    double HU_soft_tissue(52);
    double HU_water(0);

    double rho_bone(g_xray_renderer.getDensityFromHU(HU_bone));
    double rho_brain(g_xray_renderer.getDensityFromHU(HU_brain));
    double rho_gas(g_xray_renderer.getDensityFromHU(HU_gas));
    double rho_soft_tissue(g_xray_renderer.getDensityFromHU(HU_soft_tissue));
    double rho_water(g_xray_renderer.getDensityFromHU(HU_water));

    double mu_rho_bone_xcom(0);
    double mu_rho_brain_xcom(0);
    double mu_rho_gas_xcom(0);
    double mu_rho_soft_tissue_xcom(0);
    double mu_rho_water_xcom(0);

    double linear_mu_bone_mu_water(0);
    double log_mu_bone_mu_water(0);
    double linear_mu_brain_mu_water(0);
    double log_mu_brain_mu_water(0);
    double linear_mu_gas_mu_water(0);
    double log_mu_gas_mu_water(0);
    double linear_mu_soft_tissue_mu_water(0);
    double log_mu_soft_tissue_mu_water(0);
    double linear_mu_water_mu_water(0);
    double log_mu_water_mu_water(0);

    for (anEnergy; anEnergy <= aThreshold; anEnergy += anOffset)
    {
        anOutput     << anEnergy / MeV;

        mu_rho_water_xcom = g_xray_renderer.getMassAttenuationFromHU(HU_water, anEnergy);
        linear_mu_water_mu_water = g_xray_renderer.getMuWaterLinearInterpolation(anEnergy);
        log_mu_water_mu_water = g_xray_renderer.getMuWaterLogLinearInterpolation(anEnergy);

        anOutput     << "\t" << mu_rho_water_xcom                           / (cm2/g);
        anOutput     << "\t" << (linear_mu_water_mu_water / rho_water)             / (cm2/g);
        anOutput     << "\t" << (log_mu_water_mu_water    / rho_water)             / (cm2/g);

        mu_rho_bone_xcom = g_xray_renderer.getMassAttenuationFromHU(HU_bone, anEnergy);
        linear_mu_bone_mu_water = g_xray_renderer.getMuFromHUAndMuWater(HU_bone, linear_mu_water_mu_water);
        log_mu_bone_mu_water = g_xray_renderer.getMuFromHUAndMuWater(HU_bone, log_mu_water_mu_water);

        anOutput     << "\t" << mu_rho_bone_xcom                            / (cm2/g);
        anOutput     << "\t" << (linear_mu_bone_mu_water / rho_bone)               / (cm2/g);
        anOutput     << "\t" << (log_mu_bone_mu_water / rho_bone)               / (cm2/g);

        mu_rho_brain_xcom = g_xray_renderer.getMassAttenuationFromHU(HU_brain, anEnergy);
        linear_mu_brain_mu_water = g_xray_renderer.getMuFromHUAndMuWater(HU_brain, linear_mu_water_mu_water);
        log_mu_brain_mu_water = g_xray_renderer.getMuFromHUAndMuWater(HU_brain, log_mu_water_mu_water);

        anOutput     << "\t" << mu_rho_brain_xcom                            / (cm2/g);
        anOutput     << "\t" << (linear_mu_brain_mu_water / rho_bone)               / (cm2/g);
        anOutput     << "\t" << (log_mu_brain_mu_water / rho_bone)               / (cm2/g);

        mu_rho_gas_xcom = g_xray_renderer.getMassAttenuationFromHU(HU_gas, anEnergy);
        linear_mu_gas_mu_water = g_xray_renderer.getMuFromHUAndMuWater(HU_gas, linear_mu_water_mu_water);
        log_mu_gas_mu_water = g_xray_renderer.getMuFromHUAndMuWater(HU_gas, log_mu_water_mu_water);

        anOutput     << "\t" << mu_rho_gas_xcom                            / (cm2/g);
        anOutput     << "\t" << (linear_mu_gas_mu_water / rho_bone)               / (cm2/g);
        anOutput     << "\t" << (log_mu_gas_mu_water / rho_bone)               / (cm2/g);

        mu_rho_soft_tissue_xcom = g_xray_renderer.getMassAttenuationFromHU(HU_soft_tissue, anEnergy);
        linear_mu_soft_tissue_mu_water = g_xray_renderer.getMuFromHUAndMuWater(HU_soft_tissue, linear_mu_water_mu_water);
        log_mu_soft_tissue_mu_water = g_xray_renderer.getMuFromHUAndMuWater(HU_soft_tissue, log_mu_water_mu_water);

        anOutput     << "\t" << mu_rho_soft_tissue_xcom                     / (cm2/g);
        anOutput     << "\t" << (linear_mu_soft_tissue_mu_water / rho_soft_tissue) / (cm2/g);
        anOutput     << "\t" << (log_mu_soft_tissue_mu_water / rho_soft_tissue) / (cm2/g);

        anOutput     << std::endl;
    }
}
