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

#include <stdlib.h>

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

#ifdef HAS_LINUX
#include "gVirtualXRay/stereo_helper.h"
#endif

//#include "gVirtualXRayConfig.h"


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
	   // std::cout << argv[0] << " Version " << gVirtualXRay_VERSION_MAJOR << "." << gVirtualXRay_VERSION_MINOR << std::endl;

	    // Register the exit callback
		atexit(quit);

		// Initialize GLUT
		glutInit(&argc, argv);

		// Is stereo enable
#ifdef USE_STEREO
	    glGetBooleanv(GL_STEREO, &g_can_use_stereo);

	    if (g_can_use_stereo)
	    {
	    	g_use_stereo = true;
	    	std::cout << "Stereo is enable." << std::endl;
	    }
	    else
	    {
	    	g_use_stereo = false;
	    	std::cout << "Stereo is not enable." << std::endl;
	    }
#endif

		// Stereo is enable on the system
	    g_can_use_stereo = true;
    	g_use_stereo = true;

    	if (g_can_use_stereo)
		{
			glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_STEREO);
		}
		// Stereo is not enable on the system
		else
		{
			glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
		}

		// Create a GLUT window
		glutInitWindowSize(g_main_window_width, g_main_window_height);
		g_main_window_id = glutCreateWindow("GPU X-Ray Demo");

		// Initialize GLEW
		GLenum glew_error_0(glewInit());
		if (glew_error_0 != GLEW_OK)
		{
			if (g_can_use_stereo || g_use_stereo)
			{
			    g_can_use_stereo = false;
		    	g_use_stereo = false;

		    	// Stereo is not enable on the system
				glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

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
			else
			{
				std::stringstream error_message;
				error_message << "Cannot initialize GLEW: " << glewGetErrorString(glew_error_0);
				throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
			}
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

	// Rescale normal if needed
	//glEnable(GL_RESCALE_NORMAL);

	// Two-sided lighting
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	// Set the background colour
	glClearColor(0.5, 0.5, 0.5, 1.0);
	
	// Choose smooth shading
	//glShadeModel(GL_SMOOTH);
	
	// Use colour and texture coordinate interpolation
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Use antialiased polygons
	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	// Use antialiasing
	//glEnable(GL_POLYGON_SMOOTH);


    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

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


//-----------
void render()
//-----------
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

		MATRIX4 normal_matrix(g_current_modelview_matrix);
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		render();
	}

	// Display the image
	glutSwapBuffers();
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

	case 's':
		if (g_can_use_stereo)
		{
	    	g_use_stereo = !g_use_stereo;
		}
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

	case '+':
		g_zoom += 1.0 * cm;
		reshape(g_main_window_width, g_main_window_height);
		std::cout << "Zoom in cm)" << g_zoom / cm << std::endl;
		break;

	case '-':
		g_zoom -= 1.0 * cm;
		reshape(g_main_window_width, g_main_window_height);
		std::cout << "Zoom in cm)" << g_zoom / cm << std::endl;
		break;

	case 'o':
		g_IOD += 5.0 * mm;
		reshape(g_main_window_width, g_main_window_height);
		std::cout << "IOD in cm" << g_IOD / cm << std::endl;
		break;

	case 'p':
		g_IOD -= 5.0 * mm;
		reshape(g_main_window_width, g_main_window_height);
		std::cout << "IOD in cm" << g_IOD / cm << std::endl;
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
	if (height == 0)
	{
		// Prevent divide by 0
		height = 1;
	}

	int x(0), y(0), w(width), h(height);
    g_aspect = double(w) / double(h);

	g_main_window_width = width;
	g_main_window_height = height;

	glViewport(x, y, w, h);

	glMatrixMode(GL_PROJECTION);
	loadPerspectiveProjectionMatrix(g_fovy, g_aspect, g_nearZ, g_farZ);
	glLoadMatrixf(g_current_projection_matrix.get());

    setFrustum();

	glMatrixMode(GL_MODELVIEW);
	loadLookAtModelViewMatrix(0.0, 0.0, g_zoom,
			0.0, 0.0, 0.0,
			0.0, 1.0, 0.0);
	glLoadMatrixf(g_current_modelview_matrix.get());
}


//----------------------------------------------------
void mouse(int button, int button_state, int x, int y)
//----------------------------------------------------
{
	g_button = button;
	g_button_state = button_state;

	std::cout << g_button << "\t" << g_button_state << std::endl;

	if (g_button == 3)
	{
		g_use_arc_ball = false;
		g_zoom += 1.0 * cm;
		reshape(g_main_window_width, g_main_window_height);
		glutPostRedisplay();
	}
	else if (g_button == 4)
	{
		g_use_arc_ball = false;
		g_zoom -= 1.0 * cm;
		reshape(g_main_window_width, g_main_window_height);
		glutPostRedisplay();
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
	//g_inside_polygon_data.mergeVertices();
	g_inside_polygon_data.setHounsfieldValue(0.0);

	g_outside_polygon_data.setFilename(stl_filename_1);
	g_outside_polygon_data.loadSTLFile(true, true, true, true, cm);
	//g_outside_polygon_data.mergeVertices();
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
