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
*	@file		AnatomicalData-glfw-moreRealistic.cxx
*
*	@brief      Demo to illustrate the tomography acquisition and reconstruction
*	             processesusing anatomical data.
*
*	@version	1.0
*
*	@date		12/08/2016
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

#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <streambuf>
#include <ctime>

#define GLFW_INCLUDE_GLCOREARB 1

#include <GLFW/glfw3.h>

#include <stdlib.h>

/*
#ifndef GVXR_ATOMIC_ELEMENT_H
#include "gVirtualXRay/AtomicElement.h"
#endif

#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#ifndef GVXR_POLYGON_MESH_H
#include "gVirtualXRay/PolygonMesh.h"
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

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef GVXR_MATRIX4X4_H
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

#ifndef GVXR_TEXT_RENDERER_H
#include "gVirtualXRay/TextRenderer.h"
#endif

#ifndef ANATOMICAL_MESH_H
#include "gVirtualXRay/AnatomicalMesh.h"
#endif

#ifndef DEPTH_MAP_H
#include "gVirtualXRay/DepthMap.h"
#endif

#include "gVirtualXRay/BoneMesh.h"

#include "gVirtualXRay/DiaphragmMesh.h"

#include "gVirtualXRay/LiverMesh.h"

#include "gVirtualXRay/LungsMesh.h"

#include "gVirtualXRay/SkinMesh.h"

#include "display_gl2.frag.h"
#include "display_gl2.vert.h"

#include "display_gl3.frag.h"
#include "display_gl3.vert.h"

#include "display_realistic_gl3.frag.h"
#include "display_realistic_gl3.vert.h"


#include "P4_skin.stl.h"
#include "P4_liver.stl.h"
#include "P4_lung.stl.h"
#include "P4_diaph.stl.h"
#include "P4_spine.stl.h"
#include "P4_ribs.stl.h"

#include "P5_skin.stl.h"
#include "P5_liver.stl.h"
#include "P5_lung.stl.h"
#include "P5_diaph.stl.h"
#include "P5_spine.stl.h"
#include "P5_ribs.stl.h"

*/

#ifndef GVXR_ANATOMICAL_DATA_HANDLER_H
#include "AnatomicalDataHandler.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//	Constant variables
//******************************************************************************
const GLfloat g_rotation_speed(2.0);
/*
const float g_incident_energy(0.08 * MeV);
const float g_scaling_factor(2);
const VEC2 g_resolution(0.7 * g_scaling_factor * mm, 1 * g_scaling_factor * mm);
const Vec2ui g_number_of_pixels(500 / g_scaling_factor, 225 / g_scaling_factor);
*/
const double g_initial_fovy(45);                              // Field of view along the y-axis
const double g_initial_near(5.0 * cm);                        // Near clipping plane
const double g_initial_far(500.0 * cm);                       // Far clipping plane
/*
const VEC3 g_initial_source_position(   0.0,  25.0 * cm, 0.0);
const VEC3 g_initial_detector_position( 0.0, -25.0 * cm, 0.0);
const VEC3 g_initial_detector_up_vector(0.0, 0.0, 1.0);
const VEC3 g_background_colour(0.5, 0.5, 0.5);


typedef enum AcquisitionTypeTag
{
    ROTATE_SAMPLE,
    ROTATE_SOURCE_DETECTOR
} AcquisitionType;

typedef enum PatientIDTag
{
    P4,
    P5
} PatientID;


std::vector<PolygonMesh*> g_p_P4_polygon_mesh_set(6);
std::vector<PolygonMesh*> g_p_P5_polygon_mesh_set(6);
PatientID g_current_patient(P4);

const unsigned char SKIN_ID(0);
const unsigned char LIVER_ID(1);
const unsigned char LUNG_ID(2);
const unsigned char DIAPH_ID(3);
const unsigned char SPINE_ID(4);
const unsigned char RIBS_ID(5);

const double g_p_P4_HU_set [] =
{
        -70.202,
        36.9,
        -687.297,
        -41.284,
        181.08,
        181.08,
};

const double g_p_P5_HU_set [] =
{
        -36.6789,
        33.267,
        -803.976,
        -19.3418,
        323.115,
        323.115,
};
*/

//******************************************************************************
//	Global variables
//******************************************************************************
GLsizei g_main_window_width( 600);
GLsizei g_main_window_height(600);
GLFWwindow* g_p_main_window_id(0);

GLfloat g_zoom(100.0 * cm);

int g_button(-1);
int g_button_state(-1);
bool g_use_arc_ball(false);
GLint g_last_x_position(0);
GLint g_last_y_position(0);
GLint g_current_x_position(0);
GLint g_current_y_position(0);
/*
bool g_display_beam(true);
*/
Matrix4x4<GLfloat> g_text_2D_projection_matrix;
Matrix4x4<GLfloat> g_scene_rotation_matrix;
/*Matrix4x4<GLfloat> g_P4_rotation_matrix;
Matrix4x4<GLfloat> g_P5_rotation_matrix;
VEC3 g_P4_translation_vector;
VEC3 g_P5_translation_vector;

std::vector<float> g_p_vertex_data;
std::vector<unsigned int> g_p_index_data;

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;
Shader g_display_shader;

bool g_is_xray_image_up_to_date(false);
bool g_display_wireframe(false);
bool g_display_outside_polygon(true);

bool g_run_tomography_acquisition(true);

unsigned int g_angle_number(180);
RATIONAL_NUMBER g_angle_step(1);
RATIONAL_NUMBER g_first_angle(0);
RATIONAL_NUMBER g_last_angle(g_first_angle + g_angle_step * (g_angle_number - 1));
float g_next_angle_to_process(0);
float g_current_angle(0);
float g_angle_offset(1);
float g_simulation_total_time(0);
unsigned int g_number_of_rotations(0);
AcquisitionType g_acquisition_type(ROTATE_SOURCE_DETECTOR);
VEC3 g_rotation_axis(g_initial_detector_up_vector);
VEC3 g_rotation_centre(0, 0, 0);

VEC3 g_source_position(g_initial_source_position);
VEC3 g_detector_position(g_initial_detector_position);
VEC3 g_detector_up_vector(g_initial_detector_up_vector);

#ifdef HAS_FREETYPE
bool g_display_help(true);
TextRenderer g_font_rendered;
#endif

bool g_swap_patient(false);
*/
AnatomicalDataHandler g_anatomical_data_handler;


//******************************************************************************
//	Function declaration
//******************************************************************************
/*void initGL();
void initFreeType();*/
void framebufferSizeCallback(GLFWwindow* apWindow, int aWidth, int aHeight);
void keyCallback(GLFWwindow* apWindow, int aKey, int aScanCode, int anAction, int aModifierKey);
void mouseButtonCallback(GLFWwindow* apWindow, int aButton, int aButtonState, int aModifierKey);
void cursorPosCallback(GLFWwindow* apWindow, double x, double y);
void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset);
void draw();
//void render();
void quit();/*
void loadSTLFile();
void loadDetector();
void loadSource();
void loadXRaySimulator();
Vec3<GLfloat> getArcballVector(int x, int y);
void computeRotation(MATRIX4& aRotationMatrix);
void updateXRayImage();*/
void errorCallback(int error, const char* description);
/*void reset();
void displayHelp();
void usePatient(const PatientID& aPatientID);
*/

//------------------------
inline int ipart(double X)
//------------------------
{
	return (int(X));
}


//---------------------------
inline double fpart(double X)
//---------------------------
{
	return (double(X) - double(ipart(X)));
}


//----------------------------
inline double rfpart(double X)
//----------------------------
{
	return (1.0 - fpart(X));
}


//------------------------------
inline void swap(int& a, int& b)
//------------------------------
{
	int temp(b);
	b = a;
	a = temp;
}


//-------------------------------
inline void plot(int x,
				 int y,
				 float distance,
				 int width,
				 int height,
				 float* apData,
				 float intensity)
//-------------------------------
{
	int index(x + y * width);

	if (index >= 0 && index < width * height)
	{
		apData[x + y * width] += fabs(distance * intensity);
	}
}


//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
	try
	{
	    // Initialize random seed
	    srand(time(0));

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
        g_p_main_window_id = glfwCreateWindow(g_main_window_width, g_main_window_height, "gVirtualXRay -- Anatomical Data Demo -- GLFW3", NULL, NULL);
        
        // Window cannot be created 
        if (!g_p_main_window_id)
        {
            glfwTerminate();
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: cannot create a GLFW windowed mode window and its OpenGL context.");
        }

        // Make the window's context current
        glfwMakeContextCurrent(g_p_main_window_id);

        // Initialise the anatomical data handler
        g_anatomical_data_handler.initialise();
        
		// Initialize GLFL callback
		glfwSetKeyCallback(g_p_main_window_id, keyCallback);
		glfwSetFramebufferSizeCallback(g_p_main_window_id, framebufferSizeCallback);
		glfwSetMouseButtonCallback(g_p_main_window_id, mouseButtonCallback);
		glfwSetCursorPosCallback(g_p_main_window_id, cursorPosCallback);
		glfwSetScrollCallback(g_p_main_window_id, scrollCallback);

		// Set the projection matrix
		GLint width(0);
		GLint height(0);
		glfwGetFramebufferSize(g_p_main_window_id, &width, &height);
		framebufferSizeCallback(g_p_main_window_id, width, height);

		// Launch the event loop
	    while (!glfwWindowShouldClose(g_p_main_window_id))
	    {
	        draw();

            // Swap front and back buffers
	        glfwSwapBuffers(g_p_main_window_id);

	        // Poll for and process events
	        glfwPollEvents();
	    }
	}
	// Catch exception if any
    catch (const std::exception& error)
    {
        std::cerr << error.what() << std::endl;
    }
    catch (const std::string& error)
    {
        std::cerr << error << std::endl;
    }
    catch (const char* error)
    {
        std::cerr << error << std::endl;
    }

	// Close the window and shut GLFW
	if (g_p_main_window_id)
	{
		glfwDestroyWindow(g_p_main_window_id);
		g_p_main_window_id = 0;
		glfwTerminate();
	}

	// Return an exit code
	return (EXIT_SUCCESS);
}


//----------------------------------------------------
void errorCallback(int error, const char* description)
//----------------------------------------------------
{
	std::cerr << "GLFW error: " << description << std::endl;
}


//---------
void quit()
//---------
{
    if (g_p_main_window_id)
    {
        glfwDestroyWindow(g_p_main_window_id);
        g_p_main_window_id = 0;
        glfwTerminate();
    }
}


//---------
void draw()
//---------
{
    // Display the 3D elements
    glViewport(0, 0, g_main_window_width, g_main_window_height);

    // Clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Store the current transformation matrix
    pushModelViewMatrix();
    pushProjectionMatrix();

    double screen_aspect_ratio(double(g_main_window_width) / double(g_main_window_height));

    loadPerspectiveProjectionMatrix(g_initial_fovy, screen_aspect_ratio, g_initial_near, g_initial_far);

    loadLookAtModelViewMatrix(0.0, 0.0, g_zoom,
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);
    
    // Display the data
    g_anatomical_data_handler.paintGL();
    
    // Restore the current transformation matrix
    popModelViewMatrix();
    popProjectionMatrix();

    // Make sure all the OpenGL code is done
    glFinish();
}


//-------------------------------------------------------------------------------
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
//-------------------------------------------------------------------------------
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(g_p_main_window_id, GL_TRUE);
			break;
/*
		case GLFW_KEY_SPACE:
		    if (g_run_tomography_acquisition)
		    {
		        reset();
		    }
		    break;

		case GLFW_KEY_B:
			g_display_beam = !g_display_beam;
			break;

		case GLFW_KEY_N:
			g_xray_renderer.useNegativeFilteringFlag(!g_xray_renderer.getNegativeFilteringFlag());
			break;

        case GLFW_KEY_W:
            g_display_wireframe = !g_display_wireframe;
            break;

#ifdef HAS_FREETYPE
        case GLFW_KEY_H:
            g_display_help = !g_display_help;
            break;
#endif

        case GLFW_KEY_P:
            if (g_current_patient == P4) g_current_patient = P5;
            else g_current_patient = P4;
            usePatient(g_current_patient);
            break;

		case GLFW_KEY_V:
			g_display_outside_polygon = !g_display_outside_polygon;
			break;

		case GLFW_KEY_KP_ADD:
			g_zoom += 1.0 * cm;
			framebufferSizeCallback(g_p_main_window_id, g_main_window_width, g_main_window_height);
			std::cout << "Zoom in cm)" << g_zoom / cm << std::endl;
			break;

		case GLFW_KEY_MINUS:
		case GLFW_KEY_KP_SUBTRACT:
			g_zoom -= 1.0 * cm;
			framebufferSizeCallback(g_p_main_window_id, g_main_window_width, g_main_window_height);
			std::cout << "Zoom in cm)" << g_zoom / cm << std::endl;
			break;
*/
		default:
			std::cout << "Key: " << key << std::endl;
			break;
		}
	}
}


//-----------------------------------------------------------------------
void framebufferSizeCallback(GLFWwindow* apWindow, int width, int height)
//-----------------------------------------------------------------------
{
	glViewport(0, 0, width, height);

	if (height == 0)
	{
		// Prevent divide by 0
		height = 1;
	}

	g_main_window_width = width;
	g_main_window_height = height;

    g_text_2D_projection_matrix = buildOrthoProjectionMatrix(0,
            g_main_window_width,
            g_main_window_height,
            0,
            -1.0,
            1.0);
}


//--------------------------------------------
void mouseButtonCallback(GLFWwindow* apWindow,
						 int aButton,
						 int aButtonState,
						 int aModifierKey)
//--------------------------------------------
{
/*	g_button = aButton;
	g_button_state = aButtonState;

	// Use the arc ball
	if (g_button_state == GLFW_PRESS)
	{
		g_use_arc_ball = true;
	}
	// Stop using the arc ball
	else
	{
		g_use_arc_ball = false;
	}

	double xpos(0);
	double ypos(0);
	glfwGetCursorPos (apWindow, &xpos, &ypos);

	g_last_x_position = xpos;
	g_last_y_position = ypos;

	g_current_x_position = xpos;
	g_current_y_position = ypos;*/
}


//--------------------------------------------------------------
void cursorPosCallback(GLFWwindow* apWindow, double x, double y)
//--------------------------------------------------------------
{
/*	g_current_x_position = x;
	g_current_y_position = y;

	// Update the rotation matrix if needed
	computeRotation(g_scene_rotation_matrix);*/
}


//-----------------------------------------------------------------------
void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset)
//-----------------------------------------------------------------------
{
/*	// Scrolling along the Y-axis
	if (fabs(yoffset) > EPSILON)
	{
		g_use_arc_ball = false;
		g_zoom += yoffset * cm;
		framebufferSizeCallback(apWindow, g_main_window_width, g_main_window_height);
	}*/
}


//---------------------------------------
inline float radian2degree(float anAngle)
//---------------------------------------
{
    return (180.0 * anAngle / gVirtualXRay::PI);
}


//------------------------------------------
Vec3<GLfloat> getArcballVector(int x, int y)
//------------------------------------------
{
	Vec3<GLfloat> P(2.0 * float(x) / float(g_main_window_width) - 1.0,
			2.0 * float(y) / float(g_main_window_height) - 1.0,
			0);

	P.setY(-P.getY());

	float OP_squared = P.getX() * P.getX() + P.getY() * P.getY();
	if (OP_squared <= 1.0)
	{
		P.setZ(sqrt(1.0 - OP_squared));  // Pythagore
	}
	else
	{
		P.normalise();  // nearest point
	}

	return (P);
}


//-------------------------------------------------------------------
void computeRotation(MATRIX4& aRotationMatrix,
                     int aXmousePosition,
                     int aYmousePosition)
//-------------------------------------------------------------------
{
    if (g_use_arc_ball)
    {
        if (aXmousePosition != g_last_x_position || aYmousePosition != g_last_y_position)
        {
            Vec3<GLfloat> va(getArcballVector(g_last_x_position,    g_last_y_position));
            Vec3<GLfloat> vb(getArcballVector(aXmousePosition, aYmousePosition));

            float angle(g_rotation_speed * radian2degree(acos((std::min)(1.0, va.dotProduct(vb)))));

            Vec3<GLfloat> axis_in_camera_coord(va ^ vb);
            //axis_in_camera_coord.normalize();

            Matrix4x4<GLfloat> camera2object(aRotationMatrix.getInverse());
            Vec3<GLfloat> axis_in_object_coord = camera2object * axis_in_camera_coord;
            //axis_in_object_coord.normalize();

            Matrix4x4<GLfloat> rotation_matrix;
            rotation_matrix.rotate(angle, axis_in_object_coord);
            aRotationMatrix = aRotationMatrix * rotation_matrix;

            g_last_x_position = aXmousePosition;
            g_last_y_position = aYmousePosition;
        }
    }
}

