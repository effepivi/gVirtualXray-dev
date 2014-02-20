/**
********************************************************************************
*
*	@file		welsh_dragon.cxx
*
*	@brief		Main test program.
*
*	@version	1.0
*
*	@date		11/11/2013
*
*	@todo		Fix bug when using the special key.
*				There is a problem with the internal vectors of the detector.
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

#include <ctime>
#include <sstream>

#include<GL/glew.h>

#ifdef HAS_FLTK
#include <FL/glut.H>
#else // HAS_FLTK
#ifdef HAS_GLUT
#ifdef __APPLE__
#include <GLUT/glut.h>
#else // __APPLE__
#include <GL/glut.h>
#endif // __APPLE__
#endif // HAS_GLUT
#endif // HAS_FLTK

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else // __APPLE__
#include <GL/gl.h>
#include <GL/glu.h>
#endif // __APPLE__

#ifdef __APPLE__
#include <boost/filesystem.hpp>
#endif

#ifndef TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#ifndef TRIANGLE_MESH_H
#include "gVirtualXRay/TriangleMesh.h"
#endif

#ifndef XRAY_BEAM_H
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef XRAY_DETECTOR_H
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef MATRIX4X4_H
#include "gVirtualXRay/Matrix4x4.h"
#endif

#ifndef SHADER_H
#include "gVirtualXRay/Graphics/Shader.h"
#endif

#ifndef UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef STEREO_HELPER_H
#include "gVirtualXRay/StereoHelper.h"
#endif

#include "gVirtualXRayConfig.h"

#include "display.frag.h"
#include "display.vert.h"


//******************************************************************************
//	Name space
//******************************************************************************
using namespace Graphics;
using namespace gVirtualXRay;


//******************************************************************************
//	Defines
//******************************************************************************
#define PREFIX ".."


//******************************************************************************
//	Constant variables
//******************************************************************************
const GLfloat g_rotation_speed(2.0);

const int NONE(0);
const int SCENE(1);
const int OBJECT(2);
const int DETECTOR(3);

const double g_initial_intraocular_distance(1.5 * cm);        // Intraocular distance
const double g_initial_fovy(45);                              // Field of view along the y-axis
const double g_initial_near(5.0 * cm);                        // Near clipping plane
const double g_initial_far(500.0 * cm);                       // Far clipping plane
const double g_initial_screen_projection_plane(10000.0 * cm); // Screen projection plane


//******************************************************************************
//	Global variables
//******************************************************************************
GLsizei g_main_window_width(1280 / 2.0);
GLsizei g_main_window_height(800 / 2.0);
int g_main_window_id(0);
GLfloat g_zoom(100.0 * cm);

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
VEC2 g_detector_size(320.0 * mm, 320.0 * mm);
Vec2ui g_number_of_pixels(640, 640);
GLfloat g_resolution(g_detector_size.getX() / g_number_of_pixels.getX());

VEC3 g_source_position(   0.0, 0.0, -40.0 * cm);
VEC3 g_detector_position( 0.0, 0.0,  10.0 * cm);
VEC3 g_detector_up_vector(0.0, 1.0,   0.0);

TriangleMesh g_polygon_data;

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;
Shader g_display_shader;

StereoHelper g_stereo_helper;

bool g_is_xray_image_up_to_date(false);
int g_rotation_mode(SCENE);
bool g_display_wireframe(false);
//bool g_display_outside_polygon(true);

bool g_use_lighing(true);
bool g_display_detector(true);


clock_t g_start(0);
int g_image_computed(0);
double g_number_of_seconds(0);
double g_fps(0);


//******************************************************************************
//	Function declaration
//******************************************************************************
void initGL();
void reshape(int width, int height);
void display();
void mouse(int button, int button_state, int x, int y);
void motion(int x, int y);
void keyboard(unsigned char key, int x, int y);
void specialKeyboard(int key, int x, int y);
void idle();
void quit();

void computeRotation(MATRIX4& aRotationMatrix);
void loadDetector();
void loadSource();
void loadXRaySimulator();
void loadSTLFile(const std::string& aPrefix);
void updateXRayImage();
void render();
void draw();
void drawMono();
void drawStereo();
void setCurrentEye();
Vec3<GLfloat> getArcballVector(int x, int y);


//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
    try
    {
		std::cout << argv[0] << " Version " << gVirtualXRay_VERSION_MAJOR << "." << gVirtualXRay_VERSION_MINOR << std::endl;

		// Register the exit callback
		atexit(quit);

		// Initialize GLUT
		glutInit(&argc, argv);

#ifdef USE_STEREO
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_STEREO);
#else
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
#endif
		// Create the window title
		std::stringstream window_title;
		window_title << "gVirtualXRay -- DragonDemo (" << gVirtualXRay_VERSION_MAJOR << "." << gVirtualXRay_VERSION_MINOR << ")";

    	// Create a GLUT window
		glutInitWindowSize(g_main_window_width, g_main_window_height);
		g_main_window_id = glutCreateWindow(window_title.str().data());

		// Initialize GLEW
		GLenum glew_error_0(glewInit());
		if (glew_error_0 != GLEW_OK)
		{
#ifdef USE_STEREO
			glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
#else
			glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif

			// Create a GLUT window
			if (g_main_window_id) glutDestroyWindow(g_main_window_id);
			glutInitWindowSize(g_main_window_width, g_main_window_height);
			g_main_window_id = glutCreateWindow("GPU X-Ray Demo");

			GLenum glew_error_1(glewInit());
			if (glew_error_1 != GLEW_OK)
			{
				std::stringstream error_message;
				error_message << "Cannot initialize GLEW: " << glewGetErrorString(glew_error_1);
				throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
			}
		}

 		// Is stereo enable
		if (g_stereo_helper.enable())
		{
			std::cout << "Stereo is enable." << std::endl;
		}
		else
		{
			std::cout << "Stereo is not enable." << std::endl;
		}

     	// Initialise OpenGL
		initGL();

		// Check the current FBO
		checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Set the stereo parameters
		g_stereo_helper.setIntraocularDistance(g_initial_intraocular_distance);
		g_stereo_helper.setFieldOfViewY(g_initial_fovy);
		g_stereo_helper.setNear(g_initial_near);
		g_stereo_helper.setFar(g_initial_far);
		g_stereo_helper.setScreenProjectionPlane(g_initial_screen_projection_plane);

		// Initialize GLUT callback
		glutDisplayFunc(display);
		glutReshapeFunc(reshape);
		glutMouseFunc(mouse);
		glutMotionFunc(motion);
		glutKeyboardFunc(keyboard);
		glutSpecialFunc(specialKeyboard);
		glutIdleFunc(idle);

		// Load the data
		loadDetector();
		loadSource();
		loadXRaySimulator();

		// Add the geometry to the X-ray renderer
        std::string prefix;
#ifdef HAS_BUNDLE
        namespace fs = boost::filesystem;
        fs::path p(argv[0]);
        fs::path full_p = fs::complete(p);
        if (full_p.has_parent_path())
        {
        	prefix = full_p.parent_path().string();

        	if (prefix.length())
        	{
        		prefix += "/";
        	}
        }
#endif
        prefix += PREFIX;
		loadSTLFile(prefix);
		g_xray_renderer.addInnerSurface(&g_polygon_data);

		// Update the X-ray image
		updateXRayImage();

        // Launch the event loop
		glutMainLoop();

		return (EXIT_SUCCESS);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return (EXIT_FAILURE);
    }
}


//---------
void quit()
//---------
{
	if (g_main_window_id)
	{
		glutDestroyWindow(g_main_window_id);
		g_main_window_id = 0;
	}
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
	
	// Use colour and texture coordinate interpolation
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Use antialiased polygons
	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	// Use antialiasing
	//glEnable(GL_POLYGON_SMOOTH);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

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

		const GLfloat p_ambiant_silver_material_1[]  = {0.0, 0.19225,  0.19225,  1.0};
		const GLfloat p_diffuse_silver_material_1[]  = {0.0, 0.50754,  0.50754,  1.0};
		const GLfloat p_specular_silver_material_1[] = {0.0, 0.508273, 0.508273, 1.0};

		const GLfloat shininess_silver_material = 51.2;

		// Check the status of OpenGL and of the current FBO
		checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Store the current attributes
		pushEnableDisableState(GL_LIGHT0);
		pushEnableDisableState(GL_LIGHTING);

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
		if (g_use_lighing && !g_display_wireframe)
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

		// Display the sample
		// Set the colour of the sample
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   p_ambiant_silver_material_0);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   p_diffuse_silver_material_0);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  p_specular_silver_material_0);
		glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, shininess_silver_material);

		// Store the current transformation matrix
		pushModelViewMatrix();

		g_current_modelview_matrix *= g_sample_rotation_matrix;
		applyModelViewMatrix();

		if (g_display_wireframe)
		{
			g_polygon_data.displayWireFrame();
		}
		else
		{
			g_polygon_data.display(false);
		}

		// Restore the current transformation matrix
		popModelViewMatrix();

		// Display the detector
		// Set the colour of the sample
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   p_ambiant_silver_material_1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   p_diffuse_silver_material_1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  p_specular_silver_material_1);
		glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, shininess_silver_material);

		// Display the detector and the source
		g_xray_detector.displaySource();

		// Display the X-Ray image
		if (g_display_detector)
		{
			g_xray_renderer.display();
		}

		// Display the beam
		if (g_display_beam)
		{
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


//---------
void draw()
//---------
{
	// Get the current draw buffer if needed
	GLint back_buffer(0);
	glGetIntegerv(GL_DRAW_BUFFER, &back_buffer);

	g_stereo_helper.swapEye();

	// Clear the buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Display the 3D scene
	render();

	// Restore the draw buffer if needed
	glDrawBuffer(back_buffer);
}


//---------------
void drawStereo()
//---------------
{
	if (g_stereo_helper.enable())
	{
		draw();
		draw();

		// Display the images
		glutSwapBuffers();
	}
}


//-------------
void drawMono()
//-------------
{
	draw();

	// Display the image
	glutSwapBuffers();
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

	// Update the X-ray image
	updateXRayImage();

    // Use stereo
	if (g_stereo_helper.isActive())
	{
		drawStereo();
	}
    // Cannot use stereo
	else
	{
		drawMono();
	}

	// Restore the current transformation matrix
	popModelViewMatrix();
	popProjectionMatrix();

	clock_t end(clock());
	clock_t duration(end - g_start);

	g_number_of_seconds = double(duration) / CLOCKS_PER_SEC;

	g_fps = ++g_image_computed / g_number_of_seconds;
}


//--------------------------------------------
void keyboard(unsigned char key, int x, int y)
//--------------------------------------------
{
	switch(key)
	{
	case 'q':
		case 27:
		exit(EXIT_SUCCESS);
		break;

	case 's':
		if (g_stereo_helper.isActive())
		{
			g_stereo_helper.disable();
		}
		else
		{
			g_stereo_helper.enable();
		}
		break;

	case 'l':
		g_use_lighing = !g_use_lighing;
		break;

	case 'b':
		g_display_beam = !g_display_beam;
		break;

	case 'n':
		g_xray_renderer.useNegativeFilteringFlag(!g_xray_renderer.getNegativeFilteringFlag());
		break;

	case '1':
		// This is a point source
		if (g_xray_detector.getSourceType() != XRayDetector::PARALLEL)
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

	case '2':
		// This is not a line source
		if (g_xray_detector.getSourceType() != XRayDetector::LINE)
		{
			g_xray_detector.setLineSource(g_xray_detector.getXraySourceCentre(), VEC3(1, 0, 0), 16, 10.0*mm);
		}
		// This is a cubic source
		else
		{
			g_xray_detector.setPointSource();
		}

		// The X-ray image is not up-to-date
		g_is_xray_image_up_to_date = false;
		break;

	case '3':
		// This is not a square source
		if (g_xray_detector.getSourceType() != XRayDetector::SQUARE)
		{
			g_xray_detector.setSquareSource(g_xray_detector.getXraySourceCentre(), 10, 10.0*mm);
		}
		// This is a cubic source
		else
		{
			g_xray_detector.setPointSource();
		}
		// The X-ray image is not up-to-date
		g_is_xray_image_up_to_date = false;

		break;

	case '4':
		// This is not a cubic source
		if (g_xray_detector.getSourceType() != XRayDetector::CUBE)
		{
			g_xray_detector.setCubicSource(g_xray_detector.getXraySourceCentre(), 5, 10.0*mm);
		}
		// This is a cubic source
		else
		{
			g_xray_detector.setPointSource();
		}

		// The X-ray image is not up-to-date
		g_is_xray_image_up_to_date = false;
		break;

	case 't':
		if (g_rotation_mode == SCENE)
		{
				std::cout << "Rotation: select the object." << std::endl;
				g_rotation_mode = OBJECT;
		}
		else if (g_rotation_mode == OBJECT)
		{
			std::cout << "Rotation: select the detector." << std::endl;
			g_rotation_mode = DETECTOR;
		}
		else if (g_rotation_mode == DETECTOR)
		{
			std::cout << "Rotation: select the scene." << std::endl;
			g_rotation_mode = SCENE;
		}
		break;

	case 'w':
		g_display_wireframe = !g_display_wireframe;
		break;

	case 'i':
		g_zoom -= 1.0 * cm;
		reshape(g_main_window_width, g_main_window_height);
		break;

	case 'o':
		g_zoom += 1.0 * cm;
		reshape(g_main_window_width, g_main_window_height);
		break;

	case 'd':
		g_display_detector = !g_display_detector;
		break;

	case '+':
		g_stereo_helper.setIntraocularDistance(g_stereo_helper.getIntraocularDistance() + 5.0 * mm);
		std::cout << "IOD in cm" << g_stereo_helper.getIntraocularDistance() / cm << std::endl;
		break;

	case '-':
		g_stereo_helper.setIntraocularDistance(g_stereo_helper.getIntraocularDistance() - 5.0 * mm);
		std::cout << "IOD in cm" << g_stereo_helper.getIntraocularDistance() / cm << std::endl;
		break;

	default:
		break;
	}

	glutPostRedisplay();
}


//-----------------------------------------
void specialKeyboard(int key, int x, int y)
//-----------------------------------------
{
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		{
			VEC3 translation(g_detector_position - g_source_position);
			translation.normalise();
			g_source_position += translation * 1.0 * cm;

			// Set the source position
			g_xray_detector.setXrayPointSource(g_source_position);

			// The X-ray image is not up-to-date
			g_is_xray_image_up_to_date = false;
		}
		break;

	case GLUT_KEY_LEFT:
		{
			VEC3 translation(g_detector_position - g_source_position);
			translation.normalise();
			g_source_position -= translation * 1.0 * cm;

			// Set the source position
			g_xray_detector.setXrayPointSource(g_source_position);

			// The X-ray image is not up-to-date
			g_is_xray_image_up_to_date = false;
		}
		break;

	case GLUT_KEY_UP:
		{
			g_source_position += VEC3(0, 1.0 * cm, 0);

			// Set the source position
			g_xray_detector.setXrayPointSource(g_source_position);

			// The X-ray image is not up-to-date
			g_is_xray_image_up_to_date = false;
		}
		break;

	case GLUT_KEY_DOWN:
		{
			g_source_position -= VEC3(0, 1.0 * cm, 0);

			// Set the source position
			g_xray_detector.setXrayPointSource(g_source_position);

			// The X-ray image is not up-to-date
			g_is_xray_image_up_to_date = false;
		}
		break;

	default:
		std::cout << "Special key: " << key << std::endl;
		break;
	}

	glutPostRedisplay();
}


//---------------------------------
void reshape(int width, int height)
//---------------------------------
{
	if (height == 0)
	{
		// Prevent divide by 0
		height = 1;
	}

	int x(0), y(0), w(width), h(height);

	g_main_window_width = width;
	g_main_window_height = height;

	double screen_aspect_ratio(double(g_main_window_width) / double(g_main_window_height));
	//g_stereo_helper.setScreenAspectRatio(screen_aspect_ratio);

	glViewport(x, y, w, h);

	glMatrixMode(GL_PROJECTION);
	loadPerspectiveProjectionMatrix(g_initial_fovy, screen_aspect_ratio, g_initial_near, g_initial_far);
	glLoadMatrixf(g_current_projection_matrix.get());

	glMatrixMode(GL_MODELVIEW);
	loadLookAtModelViewMatrix(0.0, 0.0, g_zoom,
			0.0, 0.0, 0.0,
			0.0, 1.0, 0.0);

	// Rotate the scene
	g_scene_rotation_matrix.loadIdentity();
	g_scene_rotation_matrix.rotate(-110, VEC3(0, 1, 0));

	glLoadMatrixf(g_current_modelview_matrix.get());
}


//----------------------------------------------------
void mouse(int button, int button_state, int x, int y)
//----------------------------------------------------
{
	g_button = button;
	g_button_state = button_state;

	// This is a wheel event
	if (g_button == 3)
	{
		g_use_arc_ball = false;
		g_zoom += 1.0 * cm;
		reshape(g_main_window_width, g_main_window_height);
	}
	// This is a wheel event
	else if (g_button == 4)
	{
		g_use_arc_ball = false;
		g_zoom -= 1.0 * cm;
		reshape(g_main_window_width, g_main_window_height);
	}
	// Use the arc ball
	else if (g_button_state == GLUT_DOWN)
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
	case GLUT_LEFT_BUTTON:
		g_rotation_mode = SCENE;
		break;

	case GLUT_RIGHT_BUTTON:
		g_rotation_mode = OBJECT;
		break;

	case GLUT_MIDDLE_BUTTON:
		g_rotation_mode = DETECTOR;
		break;

	default:
		g_rotation_mode = NONE;
		break;
	}

	g_last_x_position = x;
	g_last_y_position = y;

	g_current_x_position = x;
	g_current_y_position = y;

	// No need to redisplay the screen as yet
	glutPostRedisplay();
}


//-----------------------
void motion(int x, int y)
//-----------------------
{
	g_current_x_position = x;
	g_current_y_position = y;

	// Update the rotation matrix if needed
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

	glutPostRedisplay();
}


//---------
void idle()
//---------
{
	if (glutGetWindow() != g_main_window_id)
	{
		glutSetWindow(g_main_window_id);
	}

	if ((g_image_computed % 50) == 0)
	{
		std::cout << "FPS:\t" << g_fps << std::endl;
		g_image_computed = 0;
	}

	glutPostRedisplay();
}


//------------------------------------------
void loadSTLFile(const std::string& aPrefix)
//------------------------------------------
{
    std::string stl_filename(aPrefix);
    stl_filename += "/examples/data/welsh-dragon-small.stl";

	// Set geometry
	g_polygon_data.setFilename(stl_filename.data());
	g_polygon_data.loadSTLFile(true, true, true, true, mm);
	//g_polygon_data.mergeVertices();
	g_polygon_data.setHounsfieldValue(500.0);

	g_sample_rotation_matrix.rotate(90, VEC3(0, 0, 1));
	g_sample_rotation_matrix.rotate(90, VEC3(0, 1, 0));

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
	//g_xray_detector.setParallelBeam();
	g_xray_detector.setPointSource();

	// The X-ray image is not up-to-date
	g_is_xray_image_up_to_date = false;
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

	// The X-ray image is not up-to-date
	g_is_xray_image_up_to_date = false;
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

