/**
********************************************************************************
*
*	@file		test_mu_water.cxx
*
*	@brief		Main test program to test the value of mu water.
*
*	@version	1.0
*
*	@date		24/12/2013
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
*				http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
*				BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifndef XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//	Defines
//******************************************************************************
#define PREFIX ".."


//******************************************************************************
//	Global variables
//******************************************************************************
GLsizei g_main_window_width(1280 / 2.0);
GLsizei g_main_window_height(800 / 2.0);
int g_main_window_id(0);
GLfloat g_zoom(40.0 * cm);

GLfloat g_incident_energy(80.0 * keV);
GLfloat g_resolution(0.075 * mm); // 1mm per pixel
Vec2ui g_number_of_pixels(1200, 1000);
VEC3 g_source_position(   0.0, 0.0, -10.0 * cm);
VEC3 g_detector_position( 0.0, 0.0,   1.0 * cm);
VEC3 g_detector_up_vector(0.0, 1.0,   0.0);

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;


//******************************************************************************
//	Function declaration
//******************************************************************************
void loadDetector();
void loadSource();
void loadXRaySimulator();


//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
	try
	{
		// Initialize GLUT
		glutInit(&argc, argv);

		// Create a GLUT window
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

		glutInitWindowSize(g_main_window_width, g_main_window_height);
		g_main_window_id = glutCreateWindow("GPU X-Ray Demo");

		// Initialize GLEW
		GLenum glew_error(glewInit());
		if (glew_error != GLEW_OK)
		{
			std::stringstream error_message;
			error_message << "Cannot initialize GLEW: " << glewGetErrorString(glew_error);
			throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
		}

		// Check the current FBO
		checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Load the data
		loadDetector();
		loadSource();
		loadXRaySimulator();

		// Print the energy in MeV and the attenuation coefficient of water in cm2/g
		std::ofstream test_linear_interpoation_stream(PREFIX"/examples/data/test_mu_water_linear_interpolation.csv");
		std::ofstream test_log_linear_interpoation_stream(PREFIX"/examples/data/test_mu_water_log_linear_interpolation.csv");
		double energy(0);
		for (energy = 1.00E-003; energy / MeV <= 0.1 * MeV; energy += 0.0001)
		{
			test_linear_interpoation_stream     << energy / MeV << " MeV\t" << g_xray_renderer.getMuWaterLinearInterpolation(energy) / (cm2/g) << " cm2/g" << std::endl;
			test_log_linear_interpoation_stream << energy / MeV << " MeV\t" << g_xray_renderer.getMuWaterLogLinearInterpolation(energy) / (cm2/g) << " cm2/g" << std::endl;
		}

		for (; energy / MeV <= 100 * MeV; energy += 0.1)
		{
			test_linear_interpoation_stream     << energy / MeV << " MeV\t" << g_xray_renderer.getMuWaterLinearInterpolation(energy) / (cm2/g) << " cm2/g" << std::endl;
			test_log_linear_interpoation_stream << energy / MeV << " MeV\t" << g_xray_renderer.getMuWaterLogLinearInterpolation(energy) / (cm2/g) << " cm2/g" << std::endl;
		}

		for (; energy / MeV <= 1.00E+005 / MeV; energy += 5)
		{
			test_linear_interpoation_stream     << energy / MeV << " MeV\t" << g_xray_renderer.getMuWaterLinearInterpolation(energy) / (cm2/g) << " cm2/g" << std::endl;
			test_log_linear_interpoation_stream << energy / MeV << " MeV\t" << g_xray_renderer.getMuWaterLogLinearInterpolation(energy) / (cm2/g) << " cm2/g" << std::endl;
		}

		std::ofstream reference_stream(PREFIX"/examples/data/test_mu_water1.csv");
		g_xray_renderer.printMuWaterTable(reference_stream);

		return (EXIT_SUCCESS);
	}
	// Catch exception if any
	catch (const std::exception& error)
	{
		std::cerr << error.what() << std::endl;
	}
}


//---------
void quit()
//---------
{
	exit(EXIT_SUCCESS);
}


//-----------------
void loadDetector()
//-----------------
{
	g_xray_detector.setResolutionInUnitsOfLengthPerPixel(g_resolution);
	g_xray_detector.setNumberOfPixels(g_number_of_pixels);
	g_xray_detector.setDetectorPosition(g_detector_position);
	g_xray_detector.setUpVector(g_detector_up_vector);

	g_xray_detector.setProjectionZNear(0.1 * mm);
	g_xray_detector.setProjectionZFar((g_detector_position - g_source_position).length());
}


//---------------
void loadSource()
//---------------
{
	// Set the energy
	//g_xray_beam.initialise(g_incident_energy);
	g_xray_beam.initialise(PREFIX"/examples/data/90kv_filtered_5mmCU_5mmAIR_5mmCU.txt", keV, 1);

	// Set the source position
	g_xray_detector.setXrayPointSource(g_source_position);
}


//----------------------
void loadXRaySimulator()
//----------------------
{
	// Initialise the X-ray renderer
	g_xray_renderer.initialise(XRayRenderer::OPENGL,
		GL_RGB16F_ARB,
		&g_xray_detector,
		&g_xray_beam);
}
