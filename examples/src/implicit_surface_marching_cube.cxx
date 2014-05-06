/**
********************************************************************************
*
*	@file		implicit_surface_marching_cube.cxx
*
*	@brief		Main test program for implicit surfaces.
*
*	@version	1.0
*
*	@date		10/03/2014
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
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <GL/glcorearb.h>

#define GLFW_INCLUDE_NONE 1
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

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef GVXR_MATRIX4X4_H
#include "gVirtualXRay/Matrix4x4.h"
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

#ifndef GVXR_IMPLICIT_SURFACE_H
#include "gVirtualXRay/ImplicitSurface.h"
#endif

#include "gVirtualXRayConfig.h"

#include "display.frag.h"
#include "display.vert.h"


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//	Constant variables
//******************************************************************************
const GLfloat g_rotation_speed(2.0);

const double g_initial_fovy(45);                              // Field of view along the y-axis
const double g_initial_near(5.0 * cm);                        // Near clipping plane
const double g_initial_far(500.0 * cm);                       // Far clipping plane
const double g_initial_screen_projection_plane(10000.0 * cm); // Screen projection plane


//******************************************************************************
//	Global variables
//******************************************************************************
GLsizei g_current_main_window_width(1280 / 2.0);
GLsizei g_current_main_window_height(800 / 2.0);
GLsizei g_original_main_window_width(g_current_main_window_width);
GLsizei g_original_main_window_height(g_current_main_window_height);
GLFWwindow* g_p_main_window_id(0);
GLfloat g_zoom(100.0 * cm);

int g_button(-1);
int g_button_state(-1);
bool g_use_arc_ball(false);
GLint g_last_x_position(0);
GLint g_last_y_position(0);
GLint g_current_x_position(0);
GLint g_current_y_position(0);

Matrix4x4<GLfloat> g_rotation_matrix;

PolygonMesh g_polygon_data;
std::vector<GLdouble> g_p_vertex_set;

Shader g_display_shader;

bool g_display_wireframe(false);

bool g_use_lighing(true);


clock_t g_start(0);
int g_image_computed(0);
double g_number_of_seconds(0);
double g_fps(0);
bool g_use_fullscreen(true);
bool g_make_threshold_bigger(true);

float g_threshold(0.5);
int g_selected_iso_surface(0);

RATIONAL_NUMBER g_scaling_factor(0.25);
Vec3ui g_number_of_voxel(g_scaling_factor * 128, g_scaling_factor * 128, g_scaling_factor * 128);

ImplicitSurface g_sphere_1;
ImplicitSurface g_blobby_molecule_1;
ImplicitSurface g_meta_ball_1;
ImplicitSurface g_soft_object_1;

ImplicitSurface g_sphere_2;
ImplicitSurface g_blobby_molecule_2;
ImplicitSurface g_meta_ball_2;
ImplicitSurface g_soft_object_2;


//******************************************************************************
//	Function declaration
//******************************************************************************
void display();
void initGL();
void framebufferSizeCallback(GLFWwindow* apWindow, int aWidth, int aHeight);
void keyCallback(GLFWwindow* apWindow, int aKey, int aScanCode, int anAction, int aModifierKey);
void mouseButtonCallback(GLFWwindow* apWindow, int aButton, int aButtonState, int aModifierKey);
void cursorPosCallback(GLFWwindow* apWindow, double x, double y);
void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset);
void errorCallback(int error, const char* description);
void quit();
void idle();
void extractIsoSurface();

void computeRotation(MATRIX4& aRotationMatrix);
void loadImplicitSurface();
void render();
Vec3<GLfloat> getArcballVector(int x, int y);


//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
    try
    {
		std::cout << argv[0] << " Version " << gVirtualXRay_VERSION_MAJOR << "." << gVirtualXRay_VERSION_MINOR << std::endl;

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

		// Create the window title
		std::stringstream window_title;
		window_title << "gVirtualXRay -- Implicit surface/Marching cubes test (" << gVirtualXRay_VERSION_MAJOR << "." << gVirtualXRay_VERSION_MINOR << ")";

	    // Create a windowed mode window and its OpenGL context
	    g_p_main_window_id = glfwCreateWindow(g_current_main_window_width, g_current_main_window_height, window_title.str().data(), NULL, NULL);
	    if (!g_p_main_window_id)
	    {
	        glfwTerminate();
	    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: cannot create a GLFW windowed mode window and its OpenGL context.");
	    }

	    // Make the window's context current
	    glfwMakeContextCurrent(g_p_main_window_id);

     	// Initialise OpenGL
		initGL();
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Check the current FBO
		checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Initialize GLFW callback
		glfwSetKeyCallback(g_p_main_window_id, keyCallback);
		glfwSetFramebufferSizeCallback(g_p_main_window_id, framebufferSizeCallback);
		glfwSetMouseButtonCallback(g_p_main_window_id, mouseButtonCallback);
		glfwSetCursorPosCallback(g_p_main_window_id, cursorPosCallback);
		glfwSetScrollCallback(g_p_main_window_id, scrollCallback);

		// Load the data
		loadImplicitSurface();
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Make fullscreen

		// Set the projection matrix
		GLint width(0);
		GLint height(0);
		glfwGetFramebufferSize(g_p_main_window_id, &width, &height);
		framebufferSizeCallback(g_p_main_window_id, width, height);

		// Launch the event loop
	    while (!glfwWindowShouldClose(g_p_main_window_id))
	    {
	    	// Render here
	    	display();

	        // Swap front and back buffers
	        glfwSwapBuffers(g_p_main_window_id);

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


//-----------
void initGL()
//-----------
{
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

	// L-buffer
	z_lib_return_code_vertex   = inflate(g_display_vert, sizeof(g_display_vert),   &p_vertex_shader);
	z_lib_return_code_fragment = inflate(g_display_frag, sizeof(g_display_frag), &p_fragment_shader);

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

	// Enable the shader
	g_display_shader.enable();
	GLint shader_id(g_display_shader.getProgramHandle());

	GLfloat light_global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	// Handle for shader variables
	GLuint handle(0);

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

	// Disable the shader
	g_display_shader.disable();
}


//-----------
void render()
//-----------
{
	try
	{
		// Enable back face culling
		pushEnableDisableState(GL_CULL_FACE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// Enable the shader
		pushShaderProgram();
		g_display_shader.enable();
		GLint shader_id(g_display_shader.getProgramHandle());

		// Check the status of OpenGL and of the current FBO
		checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Store the current transformation matrix
		pushModelViewMatrix();

		// Rotate the sample
		g_current_modelview_matrix *= g_rotation_matrix;

		GLuint handle(0);
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
		if (g_use_lighing)
		{
			lighting = 1;
		}
		else
		{
			lighting = 0;
		}
		handle = glGetUniformLocation(shader_id,"g_use_lighting");
		glUniform1iv(handle, 1, &lighting);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Set the colour of the sample
		const GLfloat material_ambient[]  = {0.19225,  0.0, 0.0, 1.0};
		const GLfloat material_diffuse[]  = {0.50754,  0.0, 0.0, 1.0};
		const GLfloat material_specular[] = {0.508273, 0.0, 0.0, 1.0};
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

		// Display the sample
		if (g_display_wireframe)
		{
			g_polygon_data.displayWireFrame();
		}
		else
		{
			g_polygon_data.display();
		}

		// Disable the shader
		popShaderProgram();

		// Restore the current transformation matrix
		popModelViewMatrix();

		// Restore the attributes
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
	if (!g_image_computed)
	{
		g_start = clock();
	}

	// Store the current transformation matrix
	pushModelViewMatrix();
	pushProjectionMatrix();

	// Clear the buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Display the 3D scene
	render();

	// Make sure all the OpenGL code is done
	glFinish();

	// Restore the current transformation matrix
	popModelViewMatrix();
	popProjectionMatrix();

	clock_t end(clock());
	clock_t duration(end - g_start);

	g_number_of_seconds = double(duration) / CLOCKS_PER_SEC;

	g_fps = ++g_image_computed / g_number_of_seconds;
}


//-------------------------------------------------------------------------------
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
//-------------------------------------------------------------------------------
{
	if (action == GLFW_PRESS)
	{
		switch(key)
		{
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(g_p_main_window_id, GL_TRUE);
			break;

		case GLFW_KEY_F:
			g_use_fullscreen = !g_use_fullscreen;

			if (g_use_fullscreen)
			{
			}
			else
			{
			}
			break;

		case GLFW_KEY_L:
			g_use_lighing = !g_use_lighing;
			break;

		case GLFW_KEY_0:
			g_selected_iso_surface = 0;
			break;

		case GLFW_KEY_1:
			g_selected_iso_surface = 1;
			break;

		case GLFW_KEY_2:
			g_selected_iso_surface = 2;
			break;

		case GLFW_KEY_3:
			g_selected_iso_surface = 3;
			break;

		case GLFW_KEY_4:
			g_selected_iso_surface = 4;
			break;

		case GLFW_KEY_5:
			g_selected_iso_surface = 5;
			break;

		case GLFW_KEY_6:
			g_selected_iso_surface = 6;
			break;

		case GLFW_KEY_7:
			g_selected_iso_surface = 7;
			break;

		case GLFW_KEY_W:
			g_display_wireframe = !g_display_wireframe;
			break;

		case GLFW_KEY_I:
			g_zoom -= 1.0 * cm;
			framebufferSizeCallback(g_p_main_window_id, g_current_main_window_width, g_current_main_window_height);
			break;

		case GLFW_KEY_O:
			g_zoom += 1.0 * cm;
			framebufferSizeCallback(g_p_main_window_id, g_current_main_window_width, g_current_main_window_height);
			break;

		default:
			break;
		}
	}
}


//-----------------------------------------------------------------------
void framebufferSizeCallback(GLFWwindow* apWindow, int width, int height)
//-----------------------------------------------------------------------
{
	if (height == 0)
	{
		// Prevent divide by 0
		height = 1;
	}

	int x(0), y(0), w(width), h(height);

	g_current_main_window_width = width;
	g_current_main_window_height = height;

	double screen_aspect_ratio(double(g_current_main_window_width) / double(g_current_main_window_height));

	glViewport(x, y, w, h);

	loadPerspectiveProjectionMatrix(g_initial_fovy, screen_aspect_ratio, g_initial_near, g_initial_far);

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


//-----------------------------------------------------------------------
void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset)
//-----------------------------------------------------------------------
{
	// Scrolling along the Y-axis
	if (fabs(yoffset) > EPSILON)
	{
		g_use_arc_ball = false;
		g_zoom += yoffset * cm;
		framebufferSizeCallback(apWindow, g_current_main_window_width, g_current_main_window_height);
	}
}


//--------------------------------------------------------------
void cursorPosCallback(GLFWwindow* apWindow, double x, double y)
//--------------------------------------------------------------
{
	g_current_x_position = x;
	g_current_y_position = y;

	// Compute the rotation
	computeRotation(g_rotation_matrix);
}


//------------------------
void loadImplicitSurface()
//------------------------
{
	ImplicitSurface::DensityFunctionParameterType density_function_parameter;

	density_function_parameter.m_density_function = ImplicitSurface::SPHERE;
	g_sphere_1.addControlPoint(density_function_parameter, VEC3(-2.0, 0, 0));
	g_sphere_1.addControlPoint(density_function_parameter, VEC3( 2.0, 0, 0));
	g_sphere_2.addControlLine(density_function_parameter, VEC3(-2.0, 0, 0), VEC3( 2.0, 0, 0));

	density_function_parameter.m_density_function = ImplicitSurface::BLOBBY_MOLECULE;
	density_function_parameter.m_a = 2;
	density_function_parameter.m_b = 1;
	g_blobby_molecule_1.addControlPoint(density_function_parameter, VEC3(-2.0, 0, 0));
	g_blobby_molecule_1.addControlPoint(density_function_parameter, VEC3( 2.0, 0, 0));
	g_blobby_molecule_2.addControlLine(density_function_parameter, VEC3(-2.0, 0, 0), VEC3( 2.0, 0, 0));

	density_function_parameter.m_density_function = ImplicitSurface::META_BALL;
	density_function_parameter.m_a = 1;
	density_function_parameter.m_b = 3;
	g_meta_ball_1.addControlPoint(density_function_parameter, VEC3(-2.0, 0, 0));
	g_meta_ball_1.addControlPoint(density_function_parameter, VEC3( 2.0, 0, 0));
	g_meta_ball_2.addControlLine(density_function_parameter, VEC3(-2.0, 0, 0), VEC3( 2.0, 0, 0));

	density_function_parameter.m_density_function = ImplicitSurface::SOFT_OBJECT;
	density_function_parameter.m_a = 1;
	density_function_parameter.m_b = 3;
	g_soft_object_1.addControlPoint(density_function_parameter, VEC3(-2.0, 0, 0));
	g_soft_object_1.addControlPoint(density_function_parameter, VEC3( 2.0, 0, 0));
	g_soft_object_2.addControlLine(density_function_parameter, VEC3(-2.0, 0, 0), VEC3( 2.0, 0, 0));

    // Set geometry
	extractIsoSurface();
}


//------------------------------------------
Vec3<GLfloat> getArcballVector(int x, int y)
//------------------------------------------
{
	Vec3<GLfloat> P(2.0 * float(x) / float(g_current_main_window_width) - 1.0,
			2.0 * float(y) / float(g_current_main_window_height) - 1.0,
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

#if (defined(_WIN32) || defined(_WIN64)) && !defined(__GNUC__)
			float angle(g_rotation_speed * radian2degree(acos(std::min(1.0, va.dotProduct(vb)))));
#else
			float angle(g_rotation_speed * radian2degree(acos(std::min(1.0, va.dotProduct(vb)))));
#endif

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


//----------------------------------------------------
void errorCallback(int error, const char* description)
//----------------------------------------------------
{
	std::cerr << "GLFW error: " << description << std::endl;
}


//---------
void idle()
//---------
{
	if ((g_image_computed % 25) == 0)
	{
		std::cout << "FPS:\t" << g_fps << std::endl;
		g_image_computed = 0;
	}

	if (g_make_threshold_bigger)
	{
		g_threshold += 0.02;
	}
	else
	{
		g_threshold -= 0.02;
	}

	if (g_threshold > 1 - 0.01)
	{
		g_make_threshold_bigger = false;
		g_threshold = 1 - 0.01;
	}
	else if (g_threshold < 0 + 0.01)
	{
		g_make_threshold_bigger = true;
		g_threshold = 0 + 0.01;
	}

	extractIsoSurface();
}


//----------------------
void extractIsoSurface()
//----------------------
{
	//g_meta_ball.writeVoxelData("meta_ball.mhd");
	//g_meta_ball.extractIsoSurface(g_threshold);

	g_number_of_voxel = Vec3ui(g_scaling_factor * 128, g_scaling_factor * 128, g_scaling_factor * 128);
	VEC3 voxel_size(0.1 / g_scaling_factor, 0.1 / g_scaling_factor, 0.1 / g_scaling_factor);
	VEC3 centre(0.0, 0.0, 0.0);

	switch (g_selected_iso_surface)
	{
	// Sphere
	case 0:
		g_sphere_1.voxelise(g_number_of_voxel,
				centre,
				voxel_size);
		g_sphere_1.extractIsoSurface(g_threshold);
		g_polygon_data.setExternalData(GL_TRIANGLES, &g_sphere_1.getVertexData(), &g_sphere_1.getIndexData(), true, GL_DYNAMIC_DRAW);
		break;

	case 1:
		g_sphere_2.voxelise(g_number_of_voxel,
				centre,
				voxel_size);
		g_sphere_2.extractIsoSurface(g_threshold);
		g_polygon_data.setExternalData(GL_TRIANGLES, &g_sphere_2.getVertexData(), &g_sphere_2.getIndexData(), true, GL_DYNAMIC_DRAW);
		break;

	// Blobby molecule
	case 2:
		g_blobby_molecule_1.voxelise(g_number_of_voxel,
				centre,
				voxel_size);
		g_blobby_molecule_1.extractIsoSurface(g_threshold);
		g_polygon_data.setExternalData(GL_TRIANGLES, &g_blobby_molecule_1.getVertexData(), &g_blobby_molecule_1.getIndexData(), true, GL_DYNAMIC_DRAW);
		break;

	case 3:
		g_blobby_molecule_2.voxelise(g_number_of_voxel,
				centre,
				voxel_size);
		g_blobby_molecule_2.extractIsoSurface(g_threshold);
		g_polygon_data.setExternalData(GL_TRIANGLES, &g_blobby_molecule_2.getVertexData(), &g_blobby_molecule_2.getIndexData(), true, GL_DYNAMIC_DRAW);
		break;

	// Meta ball
	case 4:
		g_meta_ball_1.voxelise(g_number_of_voxel,
				centre,
				voxel_size);
		g_meta_ball_1.extractIsoSurface(g_threshold);
		g_polygon_data.setExternalData(GL_TRIANGLES, &g_meta_ball_1.getVertexData(), &g_meta_ball_1.getIndexData(), true, GL_DYNAMIC_DRAW);
		break;

	case 5:
		g_meta_ball_2.voxelise(g_number_of_voxel,
				centre,
				voxel_size);
		g_meta_ball_2.extractIsoSurface(g_threshold);
		g_polygon_data.setExternalData(GL_TRIANGLES, &g_meta_ball_2.getVertexData(), &g_meta_ball_2.getIndexData(), true, GL_DYNAMIC_DRAW);
		break;

	// Soft object
	case 6:
		g_soft_object_1.voxelise(g_number_of_voxel,
				centre,
				voxel_size);
		g_soft_object_1.extractIsoSurface(g_threshold);
		g_polygon_data.setExternalData(GL_TRIANGLES, &g_soft_object_1.getVertexData(), &g_soft_object_1.getIndexData(), true, GL_DYNAMIC_DRAW);
		break;

	case 7:
		g_soft_object_2.voxelise(g_number_of_voxel,
				centre,
				voxel_size);
		g_soft_object_2.extractIsoSurface(g_threshold);
		g_polygon_data.setExternalData(GL_TRIANGLES, &g_soft_object_2.getVertexData(), &g_soft_object_2.getIndexData(), true, GL_DYNAMIC_DRAW);
		break;
	}

    // Set geometry
	g_polygon_data.applyScale(15);

	// Updat ethe VBO
	g_polygon_data.updateVBO(GL_DYNAMIC_DRAW, GL_TRIANGLES);
}
