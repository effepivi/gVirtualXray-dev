/**
********************************************************************************
*
*	@file		tutorial_01.cxx
*
*	@brief		The simplest test program using the library.
*
*	@version	1.0
*
*	@date		19/03/2014
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
#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>

#include <iostream>
#include <exception>

#include "gVirtualXRay/Types.h"
#include "gVirtualXRay/Units.h"
#include "gVirtualXRay/PolygonMesh.h"
#include "gVirtualXRay/XRayBeam.h"
#include "gVirtualXRay/XRayDetector.h"
#include "gVirtualXRay/XRayRenderer.h"

#include "buildCube.h"


//******************************************************************************
//	Name space
//******************************************************************************
using namespace Graphics;
using namespace gVirtualXRay;
using namespace std;


//******************************************************************************
//	Global variables
//******************************************************************************
GLsizei g_window_width( 640);
GLsizei g_window_height(480);
GLFWwindow* g_p_window_id(0);

// Components needed to simulate X-ray images
XRayDetector g_xray_detector;
XRayBeam     g_xray_beam;
XRayRenderer g_xray_simulator;
PolygonMesh g_polygon_mesh_1;
PolygonMesh g_polygon_mesh_2;

MATRIX4 g_sample_rotation_matrix(MATRIX4::buildRotationMatrix(10.0, VEC3(1, 1, 1)));

std::vector<double> g_p_vertex_set_1;
std::vector<unsigned char> g_p_index_set_1;
std::vector<float> g_p_vertex_set_2;


//******************************************************************************
//	Function declaration
//******************************************************************************
void initGLFW();
void initGL();
void framebufferSizeCallback(GLFWwindow* apWindow, int aWidth, int aHeight);
void display();
void keyCallback(GLFWwindow* apWindow, int aKey, int aScanCode, int anAction, int aModifierKey);
void idle();
void quit();
void errorCallback(int error, const char* description);

void setDetector();
void setSource();
void setBeam();
void setScannedObject();
void setSimulator();


//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
	int return_code(EXIT_SUCCESS);
	try
	{
		// Initialise GLUT
		initGLFW();

		// Initialise OpenGL
		initGL();

		// Initialise the position and shape of the X-ray detector
		setDetector();

		// Initialise the position and shape of the X-ray source
		setSource();

		// Initialise the spectrum of the X-ray beam
		setBeam();

		// Initialise the geometry of the scanned object
		setScannedObject();

		// Initialise the X-ray simulation
		setSimulator();

		// Set the projection matrix
		GLint width(0);
		GLint height(0);
		glfwGetFramebufferSize(g_p_window_id, &width, &height);
		framebufferSizeCallback(g_p_window_id, width, height);

		// Launch the event loop
	    while (!glfwWindowShouldClose(g_p_window_id))
	    {
	    	// Render here
	    	display();

	        // Swap front and back buffers
	        glfwSwapBuffers(g_p_window_id);

	        // Idle callback
	        idle();

	        // Poll for and process events
	        glfwPollEvents();
	    }
	}
	// Catch exception if any
	catch (const std::exception& error)
	{
		std::cerr << error.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	// Close the window and shut GLFW
	if (g_p_window_id)
	{
		glfwDestroyWindow(g_p_window_id);
		g_p_window_id = 0;
		glfwTerminate();
	}

	// Return an exit code
	return (return_code);
}


//-------------
void initGLFW()
//-------------
{
    // Set an error callback
    glfwSetErrorCallback(errorCallback);

    // Register the exit callback
	atexit(quit);

	// Initialize GLFW
    if (!glfwInit())
    {
    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: cannot initialise GLFW.");
    }

    // Enable OpenGL 3.2 if possible
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Enable anti-aliasing
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create a windowed mode window and its OpenGL context
    g_p_window_id = glfwCreateWindow(g_window_width, g_window_height, "gVirtualXRay -- Tutorial 01", NULL, NULL);
    if (!g_p_window_id)
    {
        glfwTerminate();
    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: cannot create a GLFW windowed mode window and its OpenGL context.");
    }

    // Make the window's context current
    glfwMakeContextCurrent(g_p_window_id);

}


//-----------
void initGL()
//-----------
{
	// Enable the Z-buffer
	glEnable(GL_DEPTH_TEST);

	// Set the background colour
	glClearColor(0.5, 0.5, 0.5, 1.0);

	// Choose smooth shading
	glShadeModel(GL_SMOOTH);

	// Set up a light
	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);


	// Check if any OpenGL error has occurred.
	// If any has, an exception is thrown
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//-----------------------------------------------------------------------
void framebufferSizeCallback(GLFWwindow* apWindow, int width, int height)
//-----------------------------------------------------------------------
{
	// Avoid a division by zero
	if (height == 0)
	{
		// Prevent divide by 0
		height = 1;
	}

	int x(0), y(0), w(width), h(height);

	g_window_width = width;
	g_window_height = height;

	double screen_aspect_ratio(double(g_window_width) / double(g_window_height));

	// Update the viewport
	glViewport(x, y, w, h);

	// Set up the projection matrix (g_current_projection_matrix)
	glMatrixMode(GL_PROJECTION);
	loadPerspectiveProjectionMatrix(45.0, screen_aspect_ratio, 5.0 * cm, 500.0 * cm);

	// Update OpenGL's matrix as this example make use of it
	glLoadMatrixf(g_current_projection_matrix.get());

	// Set up the modelling-viewing matrix (g_current_modelview_matrix)
	glMatrixMode(GL_MODELVIEW);
	loadLookAtModelViewMatrix(50.0 * cm, 50.0 * cm, -50.0 * cm,
			0.0, 0.0, 0.0,
			0.0, 1.0, 0.0);

	// Update OpenGL's matrix as this example make use of it
	glLoadMatrixf(g_current_modelview_matrix.get());
}


//------------
void display()
//------------
{
	// Clear the buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the light position
	GLfloat light_position[] = { 10.0 * cm, 50.0 * cm, 10.0 * cm, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	// Enable lighting
	glEnable(GL_LIGHTING);

	// Set the source colour
	glColor4f(1.0f, 0.0f, 1.0f, 0.2f);

	// Display the source
	g_xray_detector.displaySource();

	// Store the current transformation matrix
	glPushMatrix();

	// Apply the transformation matrix
	glMultMatrixf(g_sample_rotation_matrix.get4x4());

	// Set the scanned object colour
	glColor4f(0.0f, 0.0f, 1.0f, 0.2f);

	// Display the scanned object colour
	g_polygon_mesh_2.display();

	// Restore the current transformation matrix
	glPopMatrix();

	// Disable lighting
	glDisable(GL_LIGHTING);

	// Display the detector with the corresponding X-ray image
	g_xray_simulator.display();

	// Set the beam colour (transparency is used, this is why the beam is displayed last)
	glColor4f(0.5f, 0.0f, 0.0f, 0.2f);

	// Display the shape of the beam
	g_xray_detector.displayBeam();
}


//-------------------------------------------------------------------------------
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
//-------------------------------------------------------------------------------
{
	switch(key)
	{
	// Close the program
	case GLFW_KEY_Q:
	case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(g_p_window_id, GL_TRUE);
			break;

	// Toggle the rendering mode
	// (negative image/positive image)
	case GLFW_KEY_N:
		g_xray_simulator.useNegativeFilteringFlag(!g_xray_simulator.getNegativeFilteringFlag());
		break;

	default:
		break;
	}
}


//---------
void idle()
//---------
{
	// Compute the X-Ray image
	if (g_xray_simulator.isReady())
	{
		g_xray_simulator.computeImage(g_sample_rotation_matrix);

		// Normalise the X-ray image
		g_xray_simulator.normalise();
	}
}


//---------
void quit()
//---------
{
	if (g_p_window_id)
	{
		glfwDestroyWindow(g_p_window_id);
		g_p_window_id = 0;
        glfwTerminate();
	}
}


//----------------
void setDetector()
//----------------
{
	// Number of pixels of the detector
	Vec2ui number_of_pixels(640, 640);
	g_xray_detector.setNumberOfPixels(number_of_pixels);

	// Size of pixels (in unit of length)
	RATIONAL_NUMBER resolution(320.0 * mm / number_of_pixels.getX());

	g_xray_detector.setResolutionInUnitsOfLengthPerPixel(resolution);

	// Position of the detector
	VEC3 detector_position( 0.0, 0.0, 10.0 * cm);
	g_xray_detector.setDetectorPosition(detector_position);

	// Orientation of the detector
	VEC3 detector_up_vector(0.0, 1.0, 0.0);
	g_xray_detector.setUpVector(detector_up_vector);
}


//--------------
void setSource()
//--------------
{
	// Set the source position and shape
	VEC3 source_position(0.0, 0.0, -40.0 * cm);
	g_xray_detector.setXrayPointSource(source_position);
}


//------------
void setBeam()
//------------
{
	// Set the energy
	RATIONAL_NUMBER incident_energy(80.0 * keV);
	g_xray_beam.initialise(incident_energy);
}


//---------------------
void setScannedObject()
//---------------------
{
	// Create the cube
	VEC3 cube_centre(0, 0, 0);
	RATIONAL_NUMBER length_1( 5.0 * cm);
	RATIONAL_NUMBER length_2(10.0 * cm);
	buildCube(length_1, cube_centre, g_p_vertex_set_1, g_p_index_set_1);
	buildCube(length_2, cube_centre, g_p_vertex_set_2);

    // Set geometry
	g_polygon_mesh_1.setExternalData(GL_TRIANGLES, &g_p_vertex_set_1, &g_p_index_set_1, true, GL_STATIC_DRAW);
	g_polygon_mesh_2.setExternalData(GL_TRIANGLES, &g_p_vertex_set_2, true, GL_STATIC_DRAW);

	// Set the material properties
	g_polygon_mesh_1.setHounsfieldValue(50.0);
	g_polygon_mesh_2.setHounsfieldValue( 0.0);
}


//-----------------
void setSimulator()
//-----------------
{
	// Initialise the X-ray renderer
	g_xray_simulator.initialise(XRayRenderer::OPENGL,
			GL_RGB16F,
			&g_xray_detector,
			&g_xray_beam);

	// Add the geometry to the X-ray renderer
	g_xray_simulator.addInnerSurface(&g_polygon_mesh_1);
	g_xray_simulator.addOuterSurface(&g_polygon_mesh_2);
}


//----------------------------------------------------
void errorCallback(int error, const char* description)
//----------------------------------------------------
{
	std::cerr << "GLFW error: " << description << std::endl;
}
