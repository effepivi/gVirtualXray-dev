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
#include<GL/glew.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else // __APPLE__
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif // __APPLE__

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

#include "gVirtualXRayConfig.h"


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

const char* g_display_scene_vertex_shader_filename(PREFIX"/examples/shaders/display.vert");
const char* g_display_scene_fragment_shader_filename(PREFIX"/examples/shaders/display.frag");

const int SCENE(0);
const int OBJECT(1);
const int DETECTOR(2);


//******************************************************************************
//	Global variables
//******************************************************************************
GLsizei g_main_window_width(1280 / 2.0);
GLsizei g_main_window_height(800 / 2.0);
int g_main_window_id(0);
GLfloat g_zoom(40.0 * cm);

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

TriangleMesh g_inside_polygon_data;
TriangleMesh g_outside_polygon_data;

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;
Shader g_display_shader;

bool g_is_xray_image_up_to_date(false);
int g_rotation_mode(SCENE);
bool g_display_wireframe(false);
bool g_display_outside_polygon(true);


//******************************************************************************
//	Function declaration
//******************************************************************************
void initGL();
void display();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void specialKeyboard(int key, int x, int y);
void mouse(int button, int button_state, int x, int y);
void motion(int x, int y);
void idle();
void quit();
void loadSTLFile();
void loadDetector();
void loadSource();
void loadXRaySimulator();
Vec3<GLfloat> getArcballVector(int x, int y);
void computeRotation(MATRIX4& aRotationMatrix);
void updateXRayImage();


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

		// Initialise OpenGL
		initGL();

		// Check the current FBO
		checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);

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
		loadSTLFile();
		g_xray_renderer.addInnerSurface(&g_inside_polygon_data);
		g_xray_renderer.addOuterSurface(&g_outside_polygon_data);

		// Update the X-ray image
		updateXRayImage();

		// Launch the event loop
		glutMainLoop();

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


//-----------
void initGL()
//-----------
{
	// Enable the Z-buffer
	glEnable(GL_DEPTH_TEST);

	// Rescale normal if needed
	//glEnable(GL_RESCALE_NORMAL);

	// Two-sided lighting
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	// Set the background colour
	glClearColor(0.5, 0.5, 0.5, 1.0);
	
	// Choose smooth shading
	glShadeModel(GL_SMOOTH);
	
	// Use colour and texture coordinate interpolation
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Use antialiased polygons
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	// Use antialiasing
	glEnable(GL_POLYGON_SMOOTH);

	/*size_t fragment_shader_size = (size_t)((void *)display_frag_size);
	size_t vertex_shader_size   = (size_t)((void *)display_vert_size);

	char* p_fragment_shader = new char[fragment_shader_size + 1];
	char* p_vertex_shader   = new char[vertex_shader_size   + 1];

	memcpy(p_fragment_shader, display_frag_data, fragment_shader_size);
	memcpy(p_vertex_shader,   display_vert_data, vertex_shader_size);

	p_fragment_shader[fragment_shader_size] = '\0';
	p_vertex_shader[vertex_shader_size] = '\0';

	g_display_shader.loadSource(p_vertex_shader, p_fragment_shader);*/

	g_display_shader.loadFiles(g_display_scene_vertex_shader_filename, g_display_scene_fragment_shader_filename);
}


//------------
void display()
//------------
{
	try
	{
		// Define colours
		const GLfloat p_ambiant_silver_material_0[]  = {0.19225,  0.0, 0.19225,  1.0};
		const GLfloat p_diffuse_silver_material_0[]  = {0.50754,  0.0, 0.50754,  1.0};
		const GLfloat p_specular_silver_material_0[] = {0.508273, 0.0, 0.508273, 1.0};

		const GLfloat p_ambiant_silver_material_1[]  = {0.0, 0.19225,  0.19225,  1.0};
		const GLfloat p_diffuse_silver_material_1[]  = {0.0, 0.50754,  0.50754,  1.0};
		const GLfloat p_specular_silver_material_1[] = {0.0, 0.508273, 0.508273, 1.0};

		const GLfloat shininess_silver_material = 51.2;

		// Check the status of OpenGL and of the current FBO
		checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Clear the buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Store the current attributes
		pushEnableDisableState(GL_LIGHT0);
		pushEnableDisableState(GL_LIGHTING);

		// Enable lighting
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);

		// Store the current transformation matrix
		pushModelViewMatrix();

		// Rotate the sample
		g_current_modelview_matrix *= g_scene_rotation_matrix;
		//glPushMatrix();
		//glLoadIdentity();
		//glMultMatrixf(g_current_modelview_matrix.get());

		g_display_shader.enable();

		GLuint projection_matrix_handle(glGetUniformLocation(g_display_shader.getProgramHandle(), "g_projection_matrix"));
		glUniformMatrix4fv(projection_matrix_handle, 1, GL_FALSE, g_current_projection_matrix.get());
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		GLuint modelview_matrix_handle(glGetUniformLocation(g_display_shader.getProgramHandle(), "g_modelview_matrix"));
		glUniformMatrix4fv(modelview_matrix_handle, 1, GL_FALSE, g_current_modelview_matrix.get());
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		MATRIX4 normal_matrix(g_current_modelview_matrix.inverse().transpose());
		GLuint normal_matrix_handle(glGetUniformLocation(g_display_shader.getProgramHandle(), "g_normal_matrix"));
		glUniformMatrix3fv(normal_matrix_handle, 1, GL_FALSE, normal_matrix.get3x3());
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		GLint lighting(1);
		GLuint lighting_handle(glGetUniformLocation(g_display_shader.getProgramHandle(),"g_use_lighting"));
		glUniform1iv(lighting_handle, 1, &lighting);
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

/*		g_outside_polygon_data.display(true);

		if (g_outside_polygon_data.getVertexNormalNumber())
			g_outside_polygon_data.displayVertexNormal();
*/
		TriangleMesh* p_mesh;

		if (g_display_outside_polygon)
		{
			p_mesh = &g_outside_polygon_data;
		}
		else
		{
			p_mesh = &g_inside_polygon_data;
		}

		if (g_display_wireframe)
		{
			//p_mesh->displayWireFrame();
		}
		else
		{
			p_mesh->display(true);
		}

		if (p_mesh->getVertexNormalNumber())
		{
			p_mesh->displayVertexNormal();
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
		//g_xray_detector.displayDetector();
		g_xray_detector.displaySource();

		//g_display_shader.disable();

		// Enable lighting
		//glEnable(GL_LIGHTING);

		// Update the X-ray image
		updateXRayImage();

		// Display the X-Ray image
		g_xray_renderer.display();

		// Display the beam
		if (g_display_beam)
		{
			g_display_shader.enable();

			lighting = 0;
			glUniform1iv(lighting_handle, 1, &lighting);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			g_xray_detector.displayBeam();
		}

		// Disable the shader
		g_display_shader.disable();

		// Restore the current transformation matrix
		popModelViewMatrix();

		// Restore the attributes
		popEnableDisableState();
		popEnableDisableState();

		// Display the image
		glutSwapBuffers();

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


//--------------------------------------------
void keyboard(unsigned char key, int x, int y)
//--------------------------------------------
{
	switch (key)
	{
	case 'q':
	case 27:
		exit(EXIT_SUCCESS);
		break;

	case 'b':
		g_display_beam = !g_display_beam;
		break;

	case 'l':
//		g_xray_renderer.computeImage(g_sample_rotation_matrix);

		g_xray_renderer.computeLBuffer(g_inside_polygon_data, VEC3(), g_sample_rotation_matrix);
		g_xray_renderer.printLBuffer(PREFIX"/examples/data/inside_lbuffer.mhd");

		g_xray_renderer.computeLBuffer(g_outside_polygon_data, VEC3(), g_sample_rotation_matrix);
		g_xray_renderer.printLBuffer(PREFIX"/examples/data/outside_lbuffer.mhd");
		break;

	case 'c':
		g_xray_renderer.computeImage(g_sample_rotation_matrix);
		g_xray_renderer.printCumulatedLBuffer(PREFIX"/examples/data/cumulated_lbuffer.mhd");
		break;

	case 'a':
	case 'f':
		g_xray_renderer.computeImage(g_sample_rotation_matrix);
		g_xray_renderer.printEnergyFluence(PREFIX"/examples/data/energy_fluence.mhd");
		break;

	case 'm':
		g_xray_renderer.computeImage(g_sample_rotation_matrix);
		g_xray_renderer.printSumMuxDx(PREFIX"/examples/data/Mux_Dx.mhd");
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

	case '3':
		{
			// This is not a square source
			if (g_xray_detector.getSourceType() != XRayDetector::SQUARE)
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

	case '4':
		{
			// This is not a cubic source
			if (g_xray_detector.getSourceType() != XRayDetector::CUBE)
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

	case 't':
		{
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
		}
		break;

	case 'w':
		g_display_wireframe = !g_display_wireframe;
		break;

	case 'v':
		g_display_outside_polygon = !g_display_outside_polygon;
		break;

	default:
		std::cout << "Key: " << key << std::endl;
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
			g_source_position += translation * 1.5 * mm;

			// Set the source position
			g_xray_detector.setXrayPointSource(g_source_position);
			g_xray_detector.setProjectionZFar((g_detector_position - g_source_position).length());

			// The X-ray image is not up-to-date
			g_is_xray_image_up_to_date = false;
		}
		break;

	case GLUT_KEY_LEFT:
		{
			VEC3 translation(g_detector_position - g_source_position);
			translation.normalise();
			g_source_position -= translation * 1.5 * mm;

			// Set the source position
			g_xray_detector.setXrayPointSource(g_source_position);
			g_xray_detector.setProjectionZFar((g_detector_position - g_source_position).length());

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
	int x(0), y(0), w(width), h(height);

	g_main_window_width = width;
	g_main_window_height = height;

	glViewport(x, y, w, h);

	glMatrixMode(GL_PROJECTION);
	loadPerspectiveProjectionMatrix(45, float(w) / float(h), 0.01 * cm, 5000.0 * cm);
	glLoadMatrixf(g_current_projection_matrix.get());

	glMatrixMode(GL_MODELVIEW);
	loadLookAtModelViewMatrix(0.0, 0.0, g_zoom,
			0.0, 0.0, 0.0,
			0.0, 1.0, 0.0);
}


//----------------------------------------------------
void mouse(int button, int button_state, int x, int y)
//----------------------------------------------------
{
	g_button = button;
	g_button_state = button_state;

	// Use the arc ball
	if (g_button_state == GLUT_DOWN)
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
	}

	g_last_x_position = x;
	g_last_y_position = y;

	g_current_x_position = x;
	g_current_y_position = y;

	// No need to redisplay the screen as yet
	//glutPostRedisplay();
}


//-----------------------
void motion(int x, int y)
//-----------------------
{
	g_current_x_position = x;
	g_current_y_position = y;

	// Update the rotation matrix if needed
/*	switch (g_button)
	{
	case GLUT_LEFT_BUTTON:
		computeRotation(g_scene_rotation_matrix);
		break;

	case GLUT_RIGHT_BUTTON:
		computeRotation(g_detector_rotation_matrix);
		g_xray_detector.setRotationMatrix(g_detector_rotation_matrix);

		// The X-ray image is not up-to-date
		g_is_xray_image_up_to_date = false;
		break;

	case GLUT_MIDDLE_BUTTON:
		computeRotation(g_sample_rotation_matrix);

		// The X-ray image is not up-to-date
		g_is_xray_image_up_to_date = false;
		break;

	default:
		break;
	}*/


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

	glutPostRedisplay();
}


//----------------
void loadSTLFile()
//----------------
{
	const char* stl_filename_0(PREFIX"/examples/data/cylinder_bin.stl");
	const char* stl_filename_1(PREFIX"/examples/data/cube_bin.stl");
	//const char* g_STL_filename(PREFIX"/examples/data/dragon_vrip_res2.ply_binary.stl");

	// Set geometry
	g_inside_polygon_data.setFilename(stl_filename_0);
	g_inside_polygon_data.loadSTLFile(true, true, true, true, cm);
	g_inside_polygon_data.mergeVertices();
	g_inside_polygon_data.setHounsfieldValue(0.0);

	g_outside_polygon_data.setFilename(stl_filename_1);
	g_outside_polygon_data.loadSTLFile(true, true, true, true, cm);
	g_outside_polygon_data.mergeVertices();
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

	g_xray_detector.setProjectionZNear(0.1 * mm);
	g_xray_detector.setProjectionZFar((g_detector_position - g_source_position).length());

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
		GL_RGB16F_ARB,
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
	return (180.0 * anAngle / M_PI);
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

			Matrix4x4<GLfloat> camera2object(aRotationMatrix.inverse());
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
