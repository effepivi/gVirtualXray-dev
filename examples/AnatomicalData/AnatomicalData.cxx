/**
********************************************************************************
*
*	@file		AnatomicalData.cxx
*
*	@brief      Demo to illustrate the tomography acquisition and reconstruction
*	             processesusing anatomical data.
*
*	@version	1.0
*
*	@date		11/11/2014
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
#include <GL/glew.h>

#include <boost/scoped_ptr.hpp>

#include <cstdlib>
#include <sstream>
#include <streambuf>
#include <ctime>

#ifdef __APPLE__
#define GL3_PROTOTYPES 1
#else
#define GL_GLEXT_PROTOTYPES 1
#endif

#define GLFW_INCLUDE_GLCOREARB 1

#include <GLFW/glfw3.h>

#include <stdlib.h>

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

#include "display_gl2.frag.h"
#include "display_gl2.vert.h"

#include "display_gl3.frag.h"
#include "display_gl3.vert.h"

#include "skin.stl.h"
#include "liver.stl.h"
#include "lung.stl.h"
#include "diaph.stl.h"
#include "spine.stl.h"
#include "rib1L.stl.h"
#include "rib1R.stl.h"
#include "rib2L.stl.h"
#include "rib2R.stl.h"
#include "rib3L.stl.h"
#include "rib3R.stl.h"
#include "rib4L.stl.h"
#include "rib4R.stl.h"
#include "rib5L.stl.h"
#include "rib5R.stl.h"
#include "rib6L.stl.h"
#include "rib6R.stl.h"
#include "rib7L.stl.h"
#include "rib7R.stl.h"
#include "rib8L.stl.h"
#include "rib8R.stl.h"
#include "rib9L.stl.h"
#include "rib9R.stl.h"
#include "rib10L.stl.h"
#include "rib10R.stl.h"
#include "rib11L.stl.h"
#include "rib11R.stl.h"


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Defines
//******************************************************************************
#define PREFIX ".."


//******************************************************************************
//	Constant variables
//******************************************************************************
const GLfloat g_rotation_speed(2.0);

const float g_incident_energy(0.08 * MeV);
const float g_scaling_factor(2);
const VEC2 g_resolution(0.7 * g_scaling_factor * mm, 1 * g_scaling_factor * mm);
const Vec2ui g_number_of_pixels(500 / g_scaling_factor, 225 / g_scaling_factor);

const double g_initial_fovy(45);                              // Field of view along the y-axis
const double g_initial_near(5.0 * cm);                        // Near clipping plane
const double g_initial_far(500.0 * cm);                       // Far clipping plane

const VEC3 g_initial_source_position(   0.0,  25.0 * cm, 0.0);
const VEC3 g_initial_detector_position( 0.0, -25.0 * cm, 0.0);
const VEC3 g_initial_detector_up_vector(0.0, 0.0, 1.0);

typedef enum AcquisitionTypeTag
{
    ROTATE_SAMPLE,
    ROTATE_SOURCE_DETECTOR
} AcquisitionType;

std::vector<PolygonMesh> g_p_polygon_mesh_set(27);

const unsigned char SKIN_ID(0);
const unsigned char LIVER_ID(1);
const unsigned char LUNG_ID(2);
const unsigned char DIAPH_ID(3);
const unsigned char SPINE_ID(4);
const unsigned char RIB1L_ID(5);
const unsigned char RIB1R_ID(6);
const unsigned char RIB2L_ID(7);
const unsigned char RIB2R_ID(8);
const unsigned char RIB3L_ID(9);
const unsigned char RIB3R_ID(10);
const unsigned char RIB4L_ID(11);
const unsigned char RIB4R_ID(12);
const unsigned char RIB5L_ID(13);
const unsigned char RIB5R_ID(14);
const unsigned char RIB6L_ID(15);
const unsigned char RIB6R_ID(16);
const unsigned char RIB7L_ID(17);
const unsigned char RIB7R_ID(18);
const unsigned char RIB8L_ID(19);
const unsigned char RIB8R_ID(20);
const unsigned char RIB9L_ID(21);
const unsigned char RIB9R_ID(22);
const unsigned char RIB10L_ID(23);
const unsigned char RIB10R_ID(24);
const unsigned char RIB11L_ID(25);
const unsigned char RIB11R_ID(26);

const double g_p_HU_set [] =
{
        -36.6789,
        33.267,
        -803.976,
        -19.3418,
        323.115,
        323.115,
        323.115,
        323.115,
        323.115,
        323.115,
        323.115,
        323.115,
        323.115,
        323.115,
        323.115,
        323.115,
        323.115,
        323.115,
        323.115,
        323.115,
        323.115,
        323.115,
        323.115,
        323.115,
        323.115,
        323.115,
        323.115,
};


//******************************************************************************
//	Global variables
//******************************************************************************
GLsizei g_main_window_width( 600);
GLsizei g_main_window_height(600);
GLFWwindow* g_p_main_window_id(0);
GLfloat g_zoom(100.0 * cm);
GLfloat g_eye_offset(2.0 * cm);

int g_button(-1);
int g_button_state(-1);
bool g_use_arc_ball(false);
GLint g_last_x_position(0);
GLint g_last_y_position(0);
GLint g_current_x_position(0);
GLint g_current_y_position(0);

bool g_display_beam(true);

Matrix4x4<GLfloat> g_scene_rotation_matrix;
Matrix4x4<GLfloat> g_sample_rotation_matrix;
VEC3 g_translation_vector;

std::vector<float> g_p_vertex_data;
std::vector<unsigned int> g_p_index_data;

boost::scoped_ptr<VBO> g_sinogram_rectangle;
boost::scoped_ptr<VBO> g_tomography_rectangle;

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;
Shader g_display_shader;
Shader g_display_image_shader;

bool g_is_xray_image_up_to_date(false);
bool g_display_wireframe(false);
bool g_display_outside_polygon(true);

bool g_run_tomography_acquisition(true);

unsigned int g_angle_number(180);
RATIONAL_NUMBER g_angle_step(1);
RATIONAL_NUMBER g_first_angle(0);
RATIONAL_NUMBER g_last_angle(g_first_angle + g_angle_step * (g_angle_number - 1));
unsigned int g_next_angle_to_process(0);
unsigned int g_current_angle(0);
AcquisitionType g_acquisition_type(ROTATE_SOURCE_DETECTOR);
VEC3 g_rotation_axis(g_initial_detector_up_vector);
VEC3 g_rotation_centre(0, 0, 0);

VEC3 g_source_position(g_initial_source_position);
VEC3 g_detector_position(g_initial_detector_position);
VEC3 g_detector_up_vector(g_initial_detector_up_vector);


//******************************************************************************
//	Function declaration
//******************************************************************************
void initGLEW();
void initGL();
void display();
void framebufferSizeCallback(GLFWwindow* apWindow, int aWidth, int aHeight);
void keyCallback(GLFWwindow* apWindow, int aKey, int aScanCode, int anAction, int aModifierKey);
void mouseButtonCallback(GLFWwindow* apWindow, int aButton, int aButtonState, int aModifierKey);
void cursorPosCallback(GLFWwindow* apWindow, double x, double y);
void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset);
void quit();
void loadSTLFile();
void loadDetector();
void loadSource();
void loadXRaySimulator();
Vec3<GLfloat> getArcballVector(int x, int y);
void computeRotation(MATRIX4& aRotationMatrix);
void updateXRayImage();
void errorCallback(int error, const char* description);
void draw();
void reset();
void drawLine(const Vec2i& aVector0, const Vec2i& aVector1, int width, int height, float* apData, float intensity);


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

	    // Enable OpenGL 2.1 if possible
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	    // Enable quad-buffering if possible
	    glfwWindowHint(GLFW_STEREO, GL_TRUE);

	    // Enable anti-aliasing
	    glfwWindowHint(GLFW_SAMPLES, 4);

	    // Create a windowed mode window and its OpenGL context
	    g_p_main_window_id = glfwCreateWindow(g_main_window_width, g_main_window_height, "GPU X-Ray Demo", NULL, NULL);
	    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	   
        // Window cannot be created 
        if (!g_p_main_window_id)
	    {
            // Disable quad-buffering
            glfwWindowHint(GLFW_STEREO, GL_FALSE);

            // Create the window
	        g_p_main_window_id = glfwCreateWindow(g_main_window_width, g_main_window_height, "GPU X-Ray Demo", NULL, NULL);
        }
        
        // Window cannot be created 
        if (!g_p_main_window_id)
	    {
	        glfwTerminate();
	    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: cannot create a GLFW windowed mode window and its OpenGL context.");
	    }

	    // Make the window's context current
	    glfwMakeContextCurrent(g_p_main_window_id);

        // Initialise GLEW
        initGLEW();
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Check the OpenGL version
        std::cout << "GL:\t" << glGetString(GL_VERSION) << std::endl;

        // Check the GLSL version
        std::cout << "GLSL:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

        // Initialise OpenGL
		initGL();
	    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Check the current FBO
		checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Initialize GLFL callback
		glfwSetKeyCallback(g_p_main_window_id, keyCallback);
		glfwSetFramebufferSizeCallback(g_p_main_window_id, framebufferSizeCallback);
		glfwSetMouseButtonCallback(g_p_main_window_id, mouseButtonCallback);
		glfwSetCursorPosCallback(g_p_main_window_id, cursorPosCallback);
		glfwSetScrollCallback(g_p_main_window_id, scrollCallback);

		// Load the data
		loadDetector();
		loadSource();
		loadXRaySimulator();

		// Add the geometry to the X-ray renderer
		loadSTLFile();

		if (!g_p_polygon_mesh_set.size())
		{
		    throw "No anatomical data provided.";
		}

		for (unsigned int i(0); i < g_p_polygon_mesh_set.size(); ++i)
		{
		    g_xray_renderer.addInnerSurface(&g_p_polygon_mesh_set[i]);
		}

		// Set the projection matrix
		GLint width(0);
		GLint height(0);
		glfwGetFramebufferSize(g_p_main_window_id, &width, &height);
		framebufferSizeCallback(g_p_main_window_id, width, height);

        g_scene_rotation_matrix *= Matrix4x4<GLfloat>::buildRotationMatrix(180.0, VEC3(0, 0, 1));
        g_scene_rotation_matrix *= Matrix4x4<GLfloat>::buildRotationMatrix( 60.0, VEC3(1, 0, 0));

		// Launch the event loop
	    while (!glfwWindowShouldClose(g_p_main_window_id))
	    {

	        if (g_run_tomography_acquisition)
	        {
	            // Rotation
	            if (g_current_angle != g_next_angle_to_process)
	            {
	                switch (g_acquisition_type)
	                {
                    case ROTATE_SAMPLE:
                        {
                            Matrix4x4<GLfloat> rotation_matrix;
                            rotation_matrix *= Matrix4x4<GLfloat>::buildTranslationMatrix( g_translation_vector);
                            rotation_matrix *= Matrix4x4<GLfloat>::buildRotationMatrix(-g_angle_step, g_rotation_axis);
                            rotation_matrix *= Matrix4x4<GLfloat>::buildTranslationMatrix(-g_translation_vector);

                            g_sample_rotation_matrix *= rotation_matrix;
                        }
                        break;

                    case ROTATE_SOURCE_DETECTOR:
                        {
                            Matrix4x4<GLfloat> rotation_matrix;
                            rotation_matrix *= Matrix4x4<GLfloat>::buildTranslationMatrix( g_rotation_centre);
                            rotation_matrix *= Matrix4x4<GLfloat>::buildRotationMatrix(g_angle_step, g_rotation_axis);
                            rotation_matrix *= Matrix4x4<GLfloat>::buildTranslationMatrix(-g_rotation_centre);

                            g_source_position   = rotation_matrix * g_source_position;
                            g_detector_position = rotation_matrix * g_detector_position;

                            // Set the source position
                            g_xray_detector.setXrayPointSource(g_source_position);

                            // Use a parallel source
                            g_xray_detector.setParallelBeam();

                            // Set the detector position
                            g_xray_detector.setDetectorPosition(g_detector_position);
                        }
                        break;
	                }
	            }

	            // Update the X-ray image
	            g_is_xray_image_up_to_date = false;

	            // Update the current angle
                g_current_angle = g_next_angle_to_process;
                ++g_next_angle_to_process;

                // Stop the acquisition
                if (g_next_angle_to_process == g_angle_number)
                {
                    g_run_tomography_acquisition = false;
                }
	        }

	        // Update the X-ray image
	        updateXRayImage();
	        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	        // Draw the 3D scene
	        draw();
	        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	        // Swap front and back buffers
	        glfwSwapBuffers(g_p_main_window_id);

	        // Poll for and process events
	        glfwPollEvents();
	    }
	}
	// Catch exception if any
	catch(itk::ExceptionObject& error)
	{
		std::cerr << error << std::endl;
	}
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

    try
    {
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    }
    // Catch exception if any
    catch (const std::exception& error)
    {
        std::cerr << error.what() << std::endl;
    }
}


//-----------
void initGL()
//-----------
{
	std::cout << "OpenGL renderer:   " << glGetString(GL_RENDERER)   << std::endl;
	std::cout << "OpenGL version:    " << glGetString(GL_VERSION)    << std::endl;
	std::cout << "OpenGL vender:     " << glGetString(GL_VENDOR)     << std::endl;
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Enable the Z-buffer
	glEnable(GL_DEPTH_TEST);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Set the background colour
	glClearColor(0.5, 0.5, 0.5, 1.0);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	glEnable(GL_MULTISAMPLE);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Initialise the shaders
	char* p_vertex_shader(0);
	char* p_fragment_shader(0);

	int z_lib_return_code_vertex(0);
	int z_lib_return_code_fragment(0);

	std::string vertex_shader;
	std::string fragment_shader;

	// Display shader
    // Display shader
    if (useOpenGL3_2OrAbove())
    {
        z_lib_return_code_vertex   = inflate(g_display_gl3_vert, sizeof(g_display_gl3_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_display_gl3_frag, sizeof(g_display_gl3_frag), &p_fragment_shader);
    }
    else
    {
        z_lib_return_code_vertex   = inflate(g_display_gl2_vert, sizeof(g_display_gl2_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_display_gl2_frag, sizeof(g_display_gl2_frag), &p_fragment_shader);
    }

	vertex_shader   = p_vertex_shader;
	fragment_shader = p_fragment_shader;
	delete [] p_vertex_shader;     p_vertex_shader = 0;
	delete [] p_fragment_shader; p_fragment_shader = 0;

	if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
	}
	g_display_shader.setLabels("display.vert", "display.frag");
	g_display_shader.loadSource(vertex_shader, fragment_shader);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//-----------
void render()
//-----------
{
	try
	{
	    // Display the 3D elements
	    glViewport(0, 0, g_main_window_width, g_main_window_height);

		// Handle for shader variables
		GLuint handle(0);

		// Enable back face culling
		pushEnableDisableState(GL_CULL_FACE);
	    pushEnableDisableState(GL_DEPTH_TEST);
	    pushEnableDisableState(GL_BLEND);

	    glEnable(GL_DEPTH_TEST);
	    glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// Enable the shader
		pushShaderProgram();
		g_display_shader.enable();
		GLint shader_id(g_display_shader.getProgramHandle());

		// Define the light colours here
		GLfloat light_global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
		GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
		GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

		handle = glGetUniformLocation(shader_id, "light_global_ambient");
		glUniform4fv(handle, 1, &light_global_ambient[0]);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		handle = glGetUniformLocation(shader_id, "light_ambient");
		glUniform4fv(handle, 1, &light_ambient[0]);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		handle = glGetUniformLocation(shader_id, "light_diffuse");
		glUniform4fv(handle, 1, &light_diffuse[0]);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		handle = glGetUniformLocation(shader_id, "light_specular");
		glUniform4fv(handle, 1, &light_specular[0]);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		VEC3 light_position(0, 0, g_zoom);
		handle = glGetUniformLocation(shader_id, "light_position");
		glUniform4f(handle, light_position.getX(), light_position.getY(), light_position.getZ(), 1.0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Check the status of OpenGL and of the current FBO
		checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Store the current transformation matrix
		pushModelViewMatrix();

		// Rotate the sample
		g_current_modelview_matrix *= g_scene_rotation_matrix;

		handle = glGetUniformLocation(shader_id, "g_projection_matrix");
		glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_projection_matrix.get());
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		handle = glGetUniformLocation(shader_id, "g_modelview_matrix");
		glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_modelview_matrix.get());
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		MATRIX4 normal_matrix(g_current_modelview_matrix);
		handle = glGetUniformLocation(shader_id, "g_normal_matrix");
		glUniformMatrix3fv(handle, 1, GL_FALSE, normal_matrix.get3x3());
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		GLint lighting;
		lighting = 1;
		handle = glGetUniformLocation(shader_id,"g_use_lighting");
		glUniform1iv(handle, 1, &lighting);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Store the current transformation matrix
		pushModelViewMatrix();

		g_current_modelview_matrix *= g_sample_rotation_matrix;
		applyModelViewMatrix();

		for (unsigned int i(1); i < g_p_polygon_mesh_set.size(); ++i)
		{
            // Get the material
            Material& material(g_p_polygon_mesh_set[i].getMaterial());

            // Define colours inshader
            handle = glGetUniformLocation(shader_id, "material_ambient");
            glUniform4fv(handle, 1, &(material.getAmbientColour()[0]));
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(shader_id, "material_diffuse");
            glUniform4fv(handle, 1, &(material.getDiffuseColour()[0]));
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(shader_id, "material_specular");
            glUniform4fv(handle, 1, &(material.getSpecularColour()[0]));
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(shader_id, "material_shininess");
            glUniform1f(handle, material.getShininess());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            if (g_display_wireframe)
            {
                g_p_polygon_mesh_set[i].displayWireFrame();
            }
            else
            {
                g_p_polygon_mesh_set[i].display();
            }
		}

		// Display the skin with transparency
        if (!g_display_wireframe)
        {
            pushEnableDisableState(GL_BLEND);

            glEnable (GL_BLEND);
            glBlendFunc (GL_ONE, GL_ONE);

            g_p_polygon_mesh_set[SKIN_ID].display();

            popEnableDisableState();
        }

        // Restore the current transformation matrix
		popModelViewMatrix();

		// Define colours
		{
			const GLfloat material_ambient[]  = {0.0, 0.39225,  0.39225,  1.0};
			const GLfloat material_diffuse[]  = {0.0, 0.70754,  0.70754,  1.0};
			const GLfloat material_specular[] = {0.0, 0.708273, 0.708273, 1.0};
			const GLfloat material_shininess = 50.2;

			handle = glGetUniformLocation(shader_id, "material_ambient");
			glUniform4fv(handle, 1, &material_ambient[0]);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			handle = glGetUniformLocation(shader_id, "material_diffuse");
			glUniform4fv(handle, 1, &material_diffuse[0]);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			handle = glGetUniformLocation(shader_id, "material_specular");
			glUniform4fv(handle, 1, &material_specular[0]);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			handle = glGetUniformLocation(shader_id, "material_shininess");
			glUniform1fv(handle, 1, &material_shininess);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		}

		// Display the source
		g_xray_detector.displaySource();

		// Display the X-Ray image
	    glDisable(GL_CULL_FACE);
		g_xray_renderer.display(true);

		// Display the beam
		if (g_display_beam)
		{
			const GLfloat material_ambient[]  = {0.75, 0, 0.5, 0.1};
			handle = glGetUniformLocation(shader_id, "material_ambient");
			glUniform4fv(handle, 1, &material_ambient[0]);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			lighting = 0;
			handle = glGetUniformLocation(shader_id,"g_use_lighting");
			glUniform1iv(handle, 1, &lighting);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			g_xray_detector.displayBeam();
		}

		// Disable the shader
		popShaderProgram();

		// Restore the current transformation matrix
		popModelViewMatrix();

		// Restore the attributes
        popEnableDisableState();
        popEnableDisableState();
        popEnableDisableState();

		// Check the status of OpenGL and of the current FBO
		checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
	// Catch exception if any
	catch (const std::exception& error)
	{
		std::cerr << error.what() << std::endl;
	}
}


//------------
void display()
//------------
{
	// Store the current transformation matrix
	pushModelViewMatrix();
	pushProjectionMatrix();

	// Update the X-ray image
	updateXRayImage();

    draw();

	// Restore the current transformation matrix
	popModelViewMatrix();
	popProjectionMatrix();
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

		case GLFW_KEY_SPACE:
		    g_run_tomography_acquisition = !g_run_tomography_acquisition;

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

		case GLFW_KEY_V:
			g_display_outside_polygon = !g_display_outside_polygon;
			break;

        case GLFW_KEY_X:
        case GLFW_KEY_F:
        case GLFW_KEY_P:
            g_xray_renderer.printEnergyFluence(PREFIX"/data/energy_fluence.mhd");
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

	double screen_aspect_ratio(double(g_main_window_width) / double(g_main_window_height));

	loadPerspectiveProjectionMatrix(g_initial_fovy, screen_aspect_ratio, g_initial_near, g_initial_far);
	/*loadOrthoProjectionMatrix(-width / 2.0, width / 2.0,
			-height / 2.0, height / 2.0,
			0.01, g_initial_far);
	 */

	loadLookAtModelViewMatrix(0.0, 0.0, g_zoom,
			0.0, 0.0, 0.0,
			0.0, 1.0, 0.0);
}


//--------------------------------------------
void mouseButtonCallback(GLFWwindow* apWindow,
						 int aButton,
						 int aButtonState,
						 int aModifierKey)
//--------------------------------------------
{
	g_button = aButton;
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
	g_current_y_position = ypos;
}


//--------------------------------------------------------------
void cursorPosCallback(GLFWwindow* apWindow, double x, double y)
//--------------------------------------------------------------
{
	g_current_x_position = x;
	g_current_y_position = y;

	// Update the rotation matrix if needed
	computeRotation(g_scene_rotation_matrix);
}


//-----------------------------------------------------------------------
void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset)
//-----------------------------------------------------------------------
{
	// Scrolling along the Y-axis
	if (fabs(yoffset) > EPSILON)
	{
		g_use_arc_ball = false;
		g_zoom += yoffset * cm;
		framebufferSizeCallback(apWindow, g_main_window_width, g_main_window_height);
	}
}


//---------------------------------------------------------------------------
void decompressMesh(unsigned int aMeshID, char*& apData, unsigned int& aSize)
//---------------------------------------------------------------------------
{
	// Initialise the data
	if (apData)
	{
		delete [] apData;
		apData = 0;
	}

	aSize = 0;

	switch (aMeshID)
	{
	case SKIN_ID:
		aSize = inflate(g_skin_stl, sizeof(g_skin_stl), &apData);
		break;

	case LIVER_ID:
		aSize = inflate(g_liver_stl, sizeof(g_liver_stl), &apData);
		break;

	case LUNG_ID:
		aSize = inflate(g_lung_stl, sizeof(g_lung_stl), &apData);
		break;

	case DIAPH_ID:
		aSize = inflate(g_diaph_stl, sizeof(g_diaph_stl), &apData);
		break;

	case SPINE_ID:
		aSize = inflate(g_spine_stl, sizeof(g_spine_stl), &apData);
		break;

	case RIB1R_ID:
		aSize = inflate(g_rib1R_stl, sizeof(g_rib1R_stl), &apData);
		break;

	case RIB1L_ID:
		aSize = inflate(g_rib1L_stl, sizeof(g_rib1L_stl), &apData);
		break;

	case RIB2L_ID:
		aSize = inflate(g_rib2L_stl, sizeof(g_rib2L_stl), &apData);
		break;

	case RIB2R_ID:
		aSize = inflate(g_rib2R_stl, sizeof(g_rib2R_stl), &apData);
		break;

	case RIB3L_ID:
		aSize = inflate(g_rib3L_stl, sizeof(g_rib3L_stl), &apData);
		break;

	case RIB3R_ID:
		aSize = inflate(g_rib3R_stl, sizeof(g_rib3R_stl), &apData);
		break;

	case RIB4L_ID:
		aSize = inflate(g_rib4L_stl, sizeof(g_rib4L_stl), &apData);
		break;

	case RIB4R_ID:
		aSize = inflate(g_rib4R_stl, sizeof(g_rib4R_stl), &apData);
		break;

	case RIB5L_ID:
		aSize = inflate(g_rib5L_stl, sizeof(g_rib5L_stl), &apData);
		break;

	case RIB5R_ID:
		aSize = inflate(g_rib5R_stl, sizeof(g_rib5R_stl), &apData);
		break;

	case RIB6L_ID:
		aSize = inflate(g_rib6L_stl, sizeof(g_rib6L_stl), &apData);
		break;

	case RIB6R_ID:
		aSize = inflate(g_rib6R_stl, sizeof(g_rib6R_stl), &apData);
		break;

	case RIB7L_ID:
		aSize = inflate(g_rib7L_stl, sizeof(g_rib7L_stl), &apData);
		break;

	case RIB7R_ID:
		aSize = inflate(g_rib7R_stl, sizeof(g_rib7R_stl), &apData);
		break;

	case RIB8L_ID:
		aSize = inflate(g_rib8L_stl, sizeof(g_rib8L_stl), &apData);
		break;

	case RIB8R_ID:
		aSize = inflate(g_rib8R_stl, sizeof(g_rib8R_stl), &apData);
		break;

	case RIB9L_ID:
		aSize = inflate(g_rib9L_stl, sizeof(g_rib9L_stl), &apData);
		break;

	case RIB9R_ID:
		aSize = inflate(g_rib9R_stl, sizeof(g_rib9R_stl), &apData);
		break;

	case RIB10L_ID:
		aSize = inflate(g_rib10L_stl, sizeof(g_rib10L_stl), &apData);
		break;

	case RIB10R_ID:
		aSize = inflate(g_rib10R_stl, sizeof(g_rib10R_stl), &apData);
		break;

	case RIB11L_ID:
		aSize = inflate(g_rib11L_stl, sizeof(g_rib11L_stl), &apData);
		break;

	case RIB11R_ID:
		aSize = inflate(g_rib11R_stl, sizeof(g_rib11R_stl), &apData);
		break;

	default:
		break;
	}

	if (aSize <= 0)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
	}
}


//----------------
void loadSTLFile()
//----------------
{
    // Set geometry
    for (unsigned int i(0); i < g_p_polygon_mesh_set.size(); ++i)
    {
    	char* p_data(0);
    	unsigned int data_size(0);
    	decompressMesh(i, p_data, data_size);

    	g_p_polygon_mesh_set[i].loadSTLDataFromStream(p_data, false, true, false, true, mm, GL_STATIC_DRAW);

    	// Delete data
    	if (p_data)
    	{
    		delete [] p_data;
    		p_data = 0;
    	}

    	//g_p_polygon_mesh_set[i].mergeVertices();
        g_p_polygon_mesh_set[i].setHounsfieldValue(g_p_HU_set[i]);

        float red((float(rand()) / (RAND_MAX)) + 1);
        float green((float(rand()) / (RAND_MAX)) + 1);
        float blue((float(rand()) / (RAND_MAX)) + 1);
        float alpha(1.0);

        if (i == SKIN_ID)
        {
            alpha = 0.25;
        }

        VEC3 base_colour(red, green, blue);
        base_colour.normalise();

        g_p_polygon_mesh_set[i].getMaterial().setAmbientColour( base_colour.getX() * 0.19225, base_colour.getY() * 0.19225, base_colour.getZ() * 0.19225, alpha);
        g_p_polygon_mesh_set[i].getMaterial().setDiffuseColour( base_colour.getX() * 0.50754, base_colour.getY() * 0.50754, base_colour.getZ() * 0.50754, alpha);
        g_p_polygon_mesh_set[i].getMaterial().setSpecularColour(base_colour.getX() * 0.50827, base_colour.getY() * 0.50827, base_colour.getZ() * 0.50827, alpha);
        g_p_polygon_mesh_set[i].getMaterial().setShininess(50);
    }

    // Compute the translation vector
    g_translation_vector = g_p_polygon_mesh_set[SKIN_ID].getMinCorner() + (g_p_polygon_mesh_set[SKIN_ID].getMaxCorner() - g_p_polygon_mesh_set[SKIN_ID].getMinCorner()) / 2.0;
    g_sample_rotation_matrix = Matrix4x4<GLfloat>::buildTranslationMatrix(-g_translation_vector);

    // The X-ray image is not up-to-date
    g_is_xray_image_up_to_date = false;
}


//-----------------
void loadDetector()
//-----------------
{
	g_xray_detector.setResolutionInUnitsOfLengthPerPixel(g_resolution);
	g_xray_detector.setNumberOfPixels(g_number_of_pixels);
	g_xray_detector.setDetectorPosition(g_detector_position);
	g_xray_detector.setUpVector(g_detector_up_vector);

	// The X-ray image is not up-to-date
	g_is_xray_image_up_to_date = false;
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
	g_xray_detector.setParallelBeam();

	// The X-ray image is not up-to-date
	g_is_xray_image_up_to_date = false;
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

	// The X-ray image is not up-to-date
	g_is_xray_image_up_to_date = false;
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


//--------------------------------
float radian2degree(float anAngle)
//--------------------------------
{
	return (180.0 * anAngle / gVirtualXRay::PI);
}


//--------------------------------------------
void computeRotation(MATRIX4& aRotationMatrix)
//--------------------------------------------
{
	if (g_use_arc_ball)
	{
		if (g_current_x_position != g_last_x_position || g_current_y_position != g_last_y_position)
		{
			Vec3<GLfloat> va(getArcballVector(g_last_x_position,    g_last_y_position));
			Vec3<GLfloat> vb(getArcballVector(g_current_x_position, g_current_y_position));

			float angle(g_rotation_speed * radian2degree(acos(std::min(1.0, va.dotProduct(vb)))));

			Vec3<GLfloat> axis_in_camera_coord(va ^ vb);
			//axis_in_camera_coord.normalize();

			Matrix4x4<GLfloat> camera2object(aRotationMatrix.getInverse());
			Vec3<GLfloat> axis_in_object_coord = camera2object * axis_in_camera_coord;
			//axis_in_object_coord.normalize();

			Matrix4x4<GLfloat> rotation_matrix;
			rotation_matrix.rotate(angle, axis_in_object_coord);
			aRotationMatrix = aRotationMatrix * rotation_matrix;

			g_last_x_position = g_current_x_position;
			g_last_y_position = g_current_y_position;
		}
	}
}


//--------------------
void updateXRayImage()
//--------------------
{
	// The X-ray image is not up-to-date
	if (!g_is_xray_image_up_to_date)
	{
		// Compute the X-Ray image
		g_xray_renderer.computeImage(g_sample_rotation_matrix);

		// Normalise the X-ray image
		if (g_current_angle == 0)
		{
			g_xray_renderer.normalise();
		}

		// The X-ray image is up-to-date
		g_is_xray_image_up_to_date = true;
	}
}


//---------
void draw()
//---------
{
	// Get the current draw buffer if needed
	GLint back_buffer(0);
	glGetIntegerv(GL_DRAW_BUFFER, &back_buffer);

	// Clear the buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Display the 3D scene
	render();

	// Make sure all the OpenGL code is done
	glFinish();

	// Restore the draw buffer if needed
	glDrawBuffer(back_buffer);
}


//------------------------------------
void createTexture(GLuint& aTextureID,
                   unsigned int aWidth,
                   unsigned int aHeight,
                   float** apImage)
//------------------------------------
{
    unsigned int number_of_pixels(aWidth * aHeight);

    glActiveTexture(GL_TEXTURE0);

    if (aTextureID)
    {
        glDeleteTextures(1, &aTextureID);
        aTextureID = 0;
    }

    if (*apImage)
    {
        delete [] *apImage;
        *apImage = 0;
    }

    *apImage = new float[number_of_pixels];
    for (unsigned int i(0); i < number_of_pixels; ++i)
    {
        (*apImage)[i] = 0;
    }

    glGenTextures(1, &aTextureID);

    pushTexture2D();
    glBindTexture(GL_TEXTURE_2D, aTextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, aWidth, aHeight, 0, GL_RED, GL_FLOAT, *apImage);

    popTexture();

    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}



//----------
void reset()
//----------
{
/*    g_sample_rotation_matrix = Matrix4x4<GLfloat>::buildTranslationMatrix(-g_translation_vector);

    g_source_position = g_initial_source_position;
    g_detector_position = g_initial_detector_position;
    g_detector_up_vector = g_initial_detector_up_vector;
*/
    g_next_angle_to_process = 0;
    g_current_angle = 0;

    if (g_acquisition_type == ROTATE_SOURCE_DETECTOR)
    {
    	g_acquisition_type = ROTATE_SAMPLE;
    }
    else
    {
    	g_acquisition_type = ROTATE_SOURCE_DETECTOR;
    }
}


//----------------------------------
void drawLine(const Vec2i& aVector0,
		      const Vec2i& aVector1,
		      int width,
		      int height,
		      float* apData,
		      float intensity)
//----------------------------------
{
	// Draw a line using Xiaolin Wu's algorithm for Anti-Aliased line
	// For an example, see http://rosettacode.org/wiki/Xiaolin_Wu%27s_line_algorithm
	int x1(aVector0.getX());
	int y1(aVector0.getY());
	int x2(aVector1.getX());
	int y2(aVector1.getY());

	double dx = (double)x2 - (double)x1;
	double dy = (double)y2 - (double)y1;
	if ( fabs(dx) > fabs(dy) )
	{
		if ( x2 < x1 )
		{
			swap(x1, x2);
			swap(y1, y2);
		}
		double gradient = dy / dx;
		double xend = round(x1);
		double yend = y1 + gradient*(xend - x1);
		double xgap = rfpart(x1 + 0.5);
		int xpxl1 = xend;
		int ypxl1 = ipart(yend);
		plot(xpxl1, ypxl1, rfpart(yend)*xgap, width, height, apData, intensity);
		plot(xpxl1, ypxl1+1, fpart(yend)*xgap, width, height, apData, intensity);
		double intery = yend + gradient;

		xend = round(x2);
		yend = y2 + gradient*(xend - x2);
		xgap = fpart(x2+0.5);
		int xpxl2 = xend;
		int ypxl2 = ipart(yend);
		plot(xpxl2, ypxl2, rfpart(yend) * xgap, width, height, apData, intensity);
		plot(xpxl2, ypxl2 + 1, fpart(yend) * xgap, width, height, apData, intensity);

		int x;
		for(x=xpxl1+1; x <= (xpxl2-1); x++)
		{
			plot(x, ipart(intery), rfpart(intery), width, height, apData, intensity);
			plot(x, ipart(intery) + 1, fpart(intery), width, height, apData, intensity);
			intery += gradient;
		}
	}
	else
	{
		if ( y2 < y1 )
		{
			swap(x1, x2);
			swap(y1, y2);
		}
		double gradient = dx / dy;
		double yend = round(y1);
		double xend = x1 + gradient*(yend - y1);
		double ygap = rfpart(y1 + 0.5);
		int ypxl1 = yend;
		int xpxl1 = ipart(xend);
		plot(xpxl1, ypxl1, rfpart(xend)*ygap, width, height, apData, intensity);
		plot(xpxl1, ypxl1+1, fpart(xend)*ygap, width, height, apData, intensity);
		double interx = xend + gradient;

		yend = round(y2);
		xend = x2 + gradient*(yend - y2);
		ygap = fpart(y2+0.5);
		int ypxl2 = yend;
		int xpxl2 = ipart(xend);
		plot(xpxl2, ypxl2, rfpart(xend) * ygap, width, height, apData, intensity);
		plot(xpxl2, ypxl2 + 1, fpart(xend) * ygap, width, height, apData, intensity);

		int y;
		for(y=ypxl1+1; y <= (ypxl2-1); y++)
		{
			plot(ipart(interx), y, rfpart(interx), width, height, apData, intensity);
			plot(ipart(interx) + 1, y, fpart(interx), width, height, apData, intensity);
			interx += gradient;
		}
	}
}
