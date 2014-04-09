/**
********************************************************************************
*
*	@file		test.cxx
*
*	@brief		Main test program.
*
*	@version	1.0
*
*	@date		11/11/2013
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

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include <stdlib.h>

#ifndef TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#ifndef POLYGON_MESH_H
#include "gVirtualXRay/PolygonMesh.h"
#endif

#ifndef CUBE_MESH_H
#include "gVirtualXRay/CubeMesh.h"
#endif

#ifndef XRAY_BEAM_H
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef XRAY_DETECTOR_H
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef MATRIX4X4_H
#include "gVirtualXRay/Matrix4x4.h"
#endif

#ifndef XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef SHADER_H
#include "gVirtualXRay/Graphics/Shader.h"
#endif

#ifndef UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

#include "gVirtualXRayConfig.h"

#include "display.frag.h"
#include "display.vert.h"


//******************************************************************************
//	Name space
//******************************************************************************
using namespace Graphics;
using namespace gVirtualXRay;
using namespace std;


//******************************************************************************
//	Defines
//******************************************************************************
#define PREFIX ".."


//******************************************************************************
//	Constant variables
//******************************************************************************
const GLfloat g_rotation_speed(2.0);

const int SCENE(0);
const int OBJECT(1);
const int DETECTOR(2);


//******************************************************************************
//	Global variables
//******************************************************************************
GLsizei g_main_window_width(1280 / 2.0);
GLsizei g_main_window_height(800 / 2.0);
GLFWwindow* g_p_main_window_id(0);
GLfloat g_zoom(60.0 * cm);
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
Matrix4x4<GLfloat> g_detector_rotation_matrix;
Matrix4x4<GLfloat> g_sample_rotation_matrix;

GLfloat g_incident_energy(80.0 * keV);
GLfloat g_resolution(0.15 * mm); // 1mm per pixel
Vec2ui g_number_of_pixels(1200, 1000);
VEC3 g_source_position(   0.0, 0.0, -10.0 * cm);
VEC3 g_detector_position( 0.0, 0.0,  10.0 * cm);
VEC3 g_detector_up_vector(0.0, 1.0,   0.0);

PolygonMesh g_inside_polygon_data;
CubeMesh g_outside_polygon_data(3.0 * cm);

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;
Shader g_display_shader;

bool g_is_xray_image_up_to_date(false);
int g_rotation_mode(SCENE);
bool g_display_wireframe(false);
bool g_display_outside_polygon(true);

GLboolean g_can_use_stereo(false);
bool g_use_stereo(false);

// http://www.orthostereo.com/geometryopengl.html
const float g_DTR(Utilities::PI / 180.0);  // one degree = 0.0174532925 Radians


struct camera
{
	GLdouble m_left_frustum;
	GLdouble m_right_frustum;
	GLdouble m_bottom_frustum;
	GLdouble m_top_frustum;
	GLfloat m_model_translation;
} g_left_camera, g_right_camera;

double g_fovy = 45;                                          //field of view in y-axis
double g_aspect = double(g_main_window_width)/double(g_main_window_height);  //screen aspect ratio
double g_nearZ = 5.0 * cm;                                        //near clipping plane
double g_farZ = 500.0 * cm;                                        //far clipping plane
double g_screenZ = 10000.0 * cm;                                     //screen projection plane
double g_IOD = 1.5 * cm;                                          //intraocular distance

MATRIX4 g_left_eye_projection_matrix;
MATRIX4 g_right_eye_projection_matrix;

void setFrustum(void)
{
   double top = g_nearZ * tan(g_DTR * g_fovy / 2.0);                    //sets top of frustum based on fovy and near clipping plane
   double right = g_aspect * top;                             //sets right of frustum based on aspect ratio
   double frustum_shift = (g_IOD / 2.0) * g_nearZ / g_screenZ;

   g_left_camera.m_top_frustum        = top;
   g_left_camera.m_bottom_frustum     = -top;
   g_left_camera.m_left_frustum       = -right + frustum_shift;
   g_left_camera.m_right_frustum      = right + frustum_shift;
   g_left_camera.m_model_translation  = g_IOD / 2.0;

   g_left_eye_projection_matrix = buildFrustumProjectionMatrix(g_left_camera.m_left_frustum,
		   g_left_camera.m_right_frustum,
		   g_left_camera.m_bottom_frustum,
		   g_left_camera.m_top_frustum,
		   g_nearZ,
		   g_farZ);

   g_left_eye_projection_matrix *= MATRIX4::buildTranslationMatrix(g_left_camera.m_model_translation, 0, 0);

   g_right_camera.m_top_frustum       = top;
   g_right_camera.m_bottom_frustum    = -top;
   g_right_camera.m_left_frustum      = -right - frustum_shift;
   g_right_camera.m_right_frustum     = right - frustum_shift;
   g_right_camera.m_model_translation = -g_IOD / 2.0;

   g_right_eye_projection_matrix = buildFrustumProjectionMatrix(g_right_camera.m_left_frustum,
		   g_right_camera.m_right_frustum,
		   g_right_camera.m_bottom_frustum,
		   g_right_camera.m_top_frustum,
		   g_nearZ,
		   g_farZ);

   g_right_eye_projection_matrix *= MATRIX4::buildTranslationMatrix(g_right_camera.m_model_translation, 0, 0);
}


//******************************************************************************
//	Function declaration
//******************************************************************************
void initGL();
void display();
void framebufferSizeCallback(GLFWwindow* apWindow, int aWidth, int aHeight);
void keyCallback(GLFWwindow* apWindow, int aKey, int aScanCode, int anAction, int aModifierKey);
void mouseButtonCallback(GLFWwindow* apWindow, int aButton, int aButtonState, int aModifierKey);
void cursorPosCallback(GLFWwindow* apWindow, double x, double y);
void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset);
void idle();
void quit();
void loadSTLFile();
void loadDetector();
void loadSource();
void loadXRaySimulator();
Vec3<GLfloat> getArcballVector(int x, int y);
void computeRotation(MATRIX4& aRotationMatrix);
void updateXRayImage();
void errorCallback(int error, const char* description);


//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
	try
	{
		cout << argv[0] << " Version " << gVirtualXRay_VERSION_MAJOR << "." << gVirtualXRay_VERSION_MINOR << endl;

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

	    // Enable quad-buffering if possible
	    glfwWindowHint(GLFW_STEREO, GL_TRUE);

	    // Enable anti-aliasing
	    glfwWindowHint(GLFW_SAMPLES, 4);

	    // Create a windowed mode window and its OpenGL context
	    //g_p_main_window_id = glfwCreateWindow(g_main_window_width, g_main_window_height, "GPU X-Ray Demo", glfwGetPrimaryMonitor(), NULL);
	    g_p_main_window_id = glfwCreateWindow(g_main_window_width, g_main_window_height, "GPU X-Ray Demo", NULL, NULL);
	    if (!g_p_main_window_id)
	    {
	        glfwTerminate();
	    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: cannot create a GLFW windowed mode window and its OpenGL context.");
	    }

	    // Make the window's context current
	    glfwMakeContextCurrent(g_p_main_window_id);

		// Is stereo enable
	    glGetBooleanv(GL_STEREO, &g_can_use_stereo);

	    if (g_can_use_stereo)
	    {
	    	g_use_stereo = true;
	    	cout << "Stereo is enable." << endl;
	    }
	    else
	    {
	    	g_use_stereo = false;
	    	cout << "Stereo is not enable." << endl;
	    }

		// Initialise OpenGL
		initGL();

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
		g_xray_renderer.addInnerSurface(&g_inside_polygon_data);
		g_xray_renderer.addOuterSurface(&g_outside_polygon_data);

		// Update the X-ray image
		updateXRayImage();

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

	        // Poll for and process events
	        glfwPollEvents();
	    }
	}
	// Catch exception if any
	catch (const exception& error)
	{
		cerr << error.what() << endl;
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
/*
	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	//GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	*/

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
	g_display_shader.loadSource(vertex_shader, fragment_shader);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//-----------
void render()
//-----------
{
	try
	{
		// Enable the shader
		pushShaderProgram();
		g_display_shader.enable();
		GLint shader_id(g_display_shader.getProgramHandle());

		// Define colours
		const GLfloat p_ambiant_silver_material_0[]  = {0.19225,  0.0, 0.0, 1.0};
		const GLfloat p_diffuse_silver_material_0[]  = {0.50754,  0.0, 0.0, 1.0};
		const GLfloat p_specular_silver_material_0[] = {0.508273, 0.0, 0.0, 1.0};

		const GLfloat p_ambiant_silver_material_1[]  = {0.0, 0.39225,  0.39225,  1.0};
		const GLfloat p_diffuse_silver_material_1[]  = {0.0, 0.70754,  0.70754,  1.0};
		const GLfloat p_specular_silver_material_1[] = {0.0, 0.708273, 0.708273, 1.0};

		const GLfloat shininess_silver_material = 50.2;

		// Check the status of OpenGL and of the current FBO
		checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Store the current transformation matrix
		pushModelViewMatrix();

		// Rotate the sample
		g_current_modelview_matrix *= g_scene_rotation_matrix;

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
		lighting = 1;
		handle = glGetUniformLocation(shader_id,"g_use_lighting");
		glUniform1iv(handle, 1, &lighting);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Location binding code here
		glBindAttribLocation (shader_id, 0, "in_Vertex");
		glBindAttribLocation (shader_id, 1, "in_TexCoord");
		glBindAttribLocation (shader_id, 2, "in_Normal");
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Display the sample
		// Set the colour of the sample
		/*glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   p_ambiant_silver_material_0);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   p_diffuse_silver_material_0);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  p_specular_silver_material_0);
		glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, shininess_silver_material);
*/
		// Store the current transformation matrix
		pushModelViewMatrix();

		g_current_modelview_matrix *= g_sample_rotation_matrix;
		applyModelViewMatrix();

		if (g_display_wireframe)
		{
			g_outside_polygon_data.displayWireFrame();
			g_inside_polygon_data.displayWireFrame();
		}
		else
		{
			g_outside_polygon_data.display();
		}

		// Restore the current transformation matrix
		popModelViewMatrix();

		// Display the source
	/*	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   p_ambiant_silver_material_1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   p_diffuse_silver_material_1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  p_specular_silver_material_1);
		glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, shininess_silver_material);
	*/	g_xray_detector.displaySource();

		// Display the X-Ray image
		//g_xray_renderer.display();

		// Display the beam
		if (g_display_beam)
		{
			//glColor4f(0.75, 0, 0.5, 0.3);

			lighting = 0;
			handle = glGetUniformLocation(shader_id,"g_use_lighting");
			glUniform1iv(handle, 1, &lighting);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			//g_xray_detector.displayBeam();
		}

		// Disable the shader
		popShaderProgram();

		// Restore the current transformation matrix
		popModelViewMatrix();

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
	// Update the X-ray image
	updateXRayImage();

    // Use stereo
	if (g_use_stereo)
	{
		// Store the current transformation matrix
		pushModelViewMatrix();
		pushProjectionMatrix();

		// Set left eye
		glDrawBuffer(GL_BACK_LEFT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		g_current_projection_matrix = g_left_eye_projection_matrix;
		render();

		// Set right eye
		glDrawBuffer(GL_BACK_RIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		g_current_projection_matrix = g_right_eye_projection_matrix;
		render();

		// Restore the current transformation matrix
		popModelViewMatrix();
		popProjectionMatrix();
	}
    // Cannot use stereo
	else
	{
		// Clear the buffers
		glDrawBuffer(GL_BACK);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		render();
	}
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

		case GLFW_KEY_RIGHT:
			{
				VEC3 translation(g_detector_position - g_source_position);
				translation.normalise();
				g_source_position += translation * 1.5 * mm;

				// Set the source position
				g_xray_detector.setXrayPointSource(g_source_position);

				// The X-ray image is not up-to-date
				g_is_xray_image_up_to_date = false;
			}
			break;

		case GLFW_KEY_LEFT:
			{
				VEC3 translation(g_detector_position - g_source_position);
				translation.normalise();
				g_source_position -= translation * 1.5 * mm;

				// Set the source position
				g_xray_detector.setXrayPointSource(g_source_position);

				// The X-ray image is not up-to-date
				g_is_xray_image_up_to_date = false;
			}
			break;

		case GLFW_KEY_S:
			if (g_can_use_stereo)
			{
		    	g_use_stereo = !g_use_stereo;
			}
			break;

		case GLFW_KEY_B:
			g_display_beam = !g_display_beam;
			break;

		case GLFW_KEY_L:
	//		g_xray_renderer.computeImage(g_sample_rotation_matrix);

			g_xray_renderer.computeLBuffer(g_inside_polygon_data, VEC3(), g_sample_rotation_matrix);
			g_xray_renderer.printLBuffer(PREFIX"/examples/data/inside_lbuffer.mhd");

			g_xray_renderer.computeLBuffer(g_outside_polygon_data, VEC3(), g_sample_rotation_matrix);
			g_xray_renderer.printLBuffer(PREFIX"/examples/data/outside_lbuffer.mhd");
			break;

		case GLFW_KEY_C:
			g_xray_renderer.computeImage(g_sample_rotation_matrix);
			g_xray_renderer.printCumulatedLBuffer(PREFIX"/examples/data/cumulated_lbuffer.mhd");
			break;

		case GLFW_KEY_F:
			g_xray_renderer.computeImage(g_sample_rotation_matrix);
			g_xray_renderer.printEnergyFluence(PREFIX"/examples/data/energy_fluence.mhd");
			break;

		case GLFW_KEY_M:
			g_xray_renderer.computeImage(g_sample_rotation_matrix);
			g_xray_renderer.printSumMuxDx(PREFIX"/examples/data/Mux_Dx.mhd");
			break;

		case GLFW_KEY_N:
			g_xray_renderer.useNegativeFilteringFlag(!g_xray_renderer.getNegativeFilteringFlag());
			break;

		case GLFW_KEY_A:
			g_outside_polygon_data.splitFaces(true);

			// The X-ray image is not up-to-date
			g_is_xray_image_up_to_date = false;
			break;

		case GLFW_KEY_1:
			// This is a point source
			if (g_xray_detector.getSourceShape() != XRayDetector::PARALLEL)
			{
				// Use a parallel source
				g_xray_detector.setParallelBeam();
			}
			// This is a cubic source
			else
			{
				g_xray_detector.setPointSource();
			}

			// The X-ray image is not up-to-date
			g_is_xray_image_up_to_date = false;
			break;

		case GLFW_KEY_2:
			// This is not a line source
			if (g_xray_detector.getSourceShape() != XRayDetector::LINE)
			{
				g_xray_detector.setLineSource(g_xray_detector.getXraySourceCentre(), VEC3(1, 0, 0), 16, 5.0*mm);
			}
			// This is a cubic source
			else
			{
				g_xray_detector.setPointSource();
			}

			// The X-ray image is not up-to-date
			g_is_xray_image_up_to_date = false;
			break;

		case GLFW_KEY_3:
			{
				// This is not a square source
				if (g_xray_detector.getSourceShape() != XRayDetector::SQUARE)
				{
					g_xray_detector.setSquareSource(g_xray_detector.getXraySourceCentre(), 4, 5.0*mm);
				}
				// This is a cubic source
				else
				{
					g_xray_detector.setPointSource();
				}
				// The X-ray image is not up-to-date
				g_is_xray_image_up_to_date = false;
			}
			break;

		case GLFW_KEY_4:
			{
				// This is not a cubic source
				if (g_xray_detector.getSourceShape() != XRayDetector::CUBE)
				{
					g_xray_detector.setCubicSource(g_xray_detector.getXraySourceCentre(), 3, 5.0*mm);
				}
				// This is a cubic source
				else
				{
					g_xray_detector.setPointSource();
				}

				// The X-ray image is not up-to-date
				g_is_xray_image_up_to_date = false;
			}
			break;

		case GLFW_KEY_T:
			{
				if (g_rotation_mode == SCENE)
				{
					cout << "Rotation: select the object." << endl;
					g_rotation_mode = OBJECT;
				}
				else if (g_rotation_mode == OBJECT)
				{
					cout << "Rotation: select the detector." << endl;
					g_rotation_mode = DETECTOR;
				}
				else if (g_rotation_mode == DETECTOR)
				{
					cout << "Rotation: select the scene." << endl;
					g_rotation_mode = SCENE;
				}
			}
			break;

		case GLFW_KEY_W:
			g_display_wireframe = !g_display_wireframe;
			break;

		case GLFW_KEY_V:
			g_display_outside_polygon = !g_display_outside_polygon;
			break;

		case GLFW_KEY_KP_ADD:
			g_zoom += 1.0 * cm;
			framebufferSizeCallback(g_p_main_window_id, g_main_window_width, g_main_window_height);
			cout << "Zoom in cm)" << g_zoom / cm << endl;
			break;

		case GLFW_KEY_MINUS:
		case GLFW_KEY_KP_SUBTRACT:
			g_zoom -= 1.0 * cm;
			framebufferSizeCallback(g_p_main_window_id, g_main_window_width, g_main_window_height);
			cout << "Zoom in cm)" << g_zoom / cm << endl;
			break;

		case GLFW_KEY_O:
			g_IOD += 5.0 * mm;
			framebufferSizeCallback(g_p_main_window_id, g_main_window_width, g_main_window_height);
			cout << "IOD in cm" << g_IOD / cm << endl;
			break;

		case GLFW_KEY_P:
			g_IOD -= 5.0 * mm;
			framebufferSizeCallback(g_p_main_window_id, g_main_window_width, g_main_window_height);
			cout << "IOD in cm" << g_IOD / cm << endl;
			break;

		default:
			cout << "Key: " << key << endl;
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

    g_aspect = double(g_main_window_width) / double(g_main_window_height);

	loadPerspectiveProjectionMatrix(g_fovy, g_aspect, g_nearZ, g_farZ);

    setFrustum();

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

	cout << g_button << "\t" << g_button_state << endl;

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

	// Select the right transformation
	switch (g_button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		g_rotation_mode = SCENE;
		break;

	case GLFW_MOUSE_BUTTON_RIGHT:
		g_rotation_mode = OBJECT;
		break;

	case GLFW_MOUSE_BUTTON_MIDDLE:
		g_rotation_mode = DETECTOR;
		break;
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
	switch (g_button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		computeRotation(g_scene_rotation_matrix);
		break;

	case GLFW_MOUSE_BUTTON_RIGHT:
		computeRotation(g_detector_rotation_matrix);
		g_xray_detector.setRotationMatrix(g_detector_rotation_matrix);

		// The X-ray image is not up-to-date
		g_is_xray_image_up_to_date = false;
		break;

	case GLFW_MOUSE_BUTTON_MIDDLE:
		computeRotation(g_sample_rotation_matrix);

		// The X-ray image is not up-to-date
		g_is_xray_image_up_to_date = false;
		break;

	default:
		break;
	}


	switch (g_rotation_mode)
	{
	case SCENE:
		// Compute the rotation
		computeRotation(g_scene_rotation_matrix);
		break;

	case OBJECT:
		// Compute the rotation
		computeRotation(g_sample_rotation_matrix);

		// The X-ray image is not up-to-date
		g_is_xray_image_up_to_date = false;
		break;

	case DETECTOR:
		// Compute the rotation
		computeRotation(g_detector_rotation_matrix);
		g_xray_detector.setRotationMatrix(g_detector_rotation_matrix);

		// The X-ray image is not up-to-date
		g_is_xray_image_up_to_date = false;
		break;

	default:
		break;
	}
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


//----------------
void loadSTLFile()
//----------------
{
	string stl_filename_0("cylinder_bin.stl");
	string stl_filename_1("cube_bin.stl");

	// Open the file
	FILE* p_file_descriptor_0(fopen(stl_filename_0.data(), "rb"));
	FILE* p_file_descriptor_1(fopen(stl_filename_1.data(), "rb"));

	// The file is not in the same directory as the executable
	if (!p_file_descriptor_0)
	{
		// Use the prefix
#if defined(_WIN32) || defined(_WIN64)
		stl_filename_0 = "../../examples/data/" + stl_filename_0;
#else
		stl_filename_0 = "../examples/data/" + stl_filename_0;
#endif
	}
	else
	{
		fclose(p_file_descriptor_0);
	}

	// The file is not in the same directory as the executable
	if (!p_file_descriptor_1)
	{
		// Use the prefix
#if defined(_WIN32) || defined(_WIN64)
		stl_filename_1 = "../../examples/data/" + stl_filename_1;
#else
		stl_filename_1 = "../examples/data/" + stl_filename_1;
#endif
	}
	else
	{
		fclose(p_file_descriptor_1);
	}

	// Set geometry
	g_inside_polygon_data.setFilename(stl_filename_0.data());
	g_inside_polygon_data.loadSTLFile(true, true, true, true, cm, GL_STATIC_DRAW);
	//g_inside_polygon_data.mergeVertices();
	g_inside_polygon_data.setHounsfieldValue(0.0);

	//g_outside_polygon_data.setFilename(stl_filename_1.data());
	//g_outside_polygon_data.loadSTLFile(true, true, true, true, cm, GL_STATIC_DRAW);
	//g_outside_polygon_data.mergeVertices(true);
	g_outside_polygon_data.setHounsfieldValue(500.0);

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
	//g_xray_beam.initialise(PREFIX"/examples/data/90kv_filtered_5mmCU_5mmAIR_5mmCU.txt", keV, 1);

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
	return (180.0 * anAngle / Utilities::PI);
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

			float angle(g_rotation_speed * radian2degree(acos(min(1.0, va.dotProduct(vb)))));

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
		g_xray_renderer.normalise();

		// The X-ray image is up-to-date
		g_is_xray_image_up_to_date = true;
	}
}
