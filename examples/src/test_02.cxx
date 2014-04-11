/**
********************************************************************************
*	@mainpage	gVirtualX -- Virtual X-Ray Imaging Library on GPU
*
*	@file			test_03.cxx
*
*	@brief		Main test program with GUI.
*
*	@version		1.0
*
*	@date			06/05/2012
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
#include <cstdlib>
#include <list>
#include <vector>
#include <sstream>

#include<GL/glew.h>
#include<GL/glui.h>

/*
#include<math.h>*/
#ifdef HAS_FREEIMAGE
#include <FreeImage.h>
#endif // HAS_FREEIMAGE

#include <iostream>
/*#include <sstream>
*/
#ifndef GVXR_TYPES_H
#include "Types.h"
#endif

/*#ifndef GVXR_UTILITIES_H
#include "Utilities.h"
#endif
*/
#ifndef GVXR_POLYGON_MESH_H
#include "PolygonMesh.h"
#endif
/*
#ifndef GVXR_VOLUMEDATASET_H
#include "VolumeDataset.h"
#endif
*/
#ifndef GVXR_XRAY_DETECTOR_H
#include "XRayDetector.h"
#endif

#ifndef GVXR_XRAY_RENDERER_H
#include "XRayRenderer.h"
#endif

#ifndef GVXR_SHADER_H
#include "Shader.h"
#endif

#ifndef GVXR_OPENGL_UTILITIES_H
#include "OpenGLUtilities.h"
#endif

#include "framebufferObject.h"


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//	Defines
//******************************************************************************
#define BEAM_SPECTRUM       1
#define NEGATIVE_DISPLAY    2
#define INPUT_INTENSITY     3
#define SHIFT_FILER         4
#define SCALE_FILER         5
#define ARTEFACT_CORRECTION 6
#define LOAD_SAMPLE         7
#define AUTO_ADJUST         8
#define SOURCE_POINTS       9
#define SOURCE_SIZE        10
#define ADD_PROFILE        11
#define DELETE_PROFILES    12
#define BEAM_GEOMETRY      13
#define BUILD_SINOGRAM     14

#define PELVIS 0
#define DRAGON 1
#define CUBES  2
#define CUBE_AND_CYLINDER 3
#define INSIDE 4
//#define CUBE   4

int g_sample_id(PELVIS);

#define IRON_HU       24416.847826086956522
#define WATER_HU         0.0
#define BONES_HU       750.0
#define SOFT_TISSUE_HU  40.0
#define MUSCLE_HU       31.0
#define FAT_HU        -125.0
#define SKIN_HU       -391.0
#define BOWEL_HU     -1000.0
#define LUNG_HU      -1000.0
#define DIAPHRAGM_HU  -200.0
#define LIVER_HU       100.0
#define LIGAMENT_HU    500.0


//#ifdef __APPLE__
//#define PREFIX "../../.."
//#else
#define PREFIX ".."
//#endif

#define XRAY_DETECTOR_LBUFFER_SHADER_ID 0
#define XRAY_DETECTOR_LBUFFER_TEXTURE_NAME_ID            0
#define XRAY_DETECTOR_LBUFFER_FBO_ID            0


//******************************************************************************
//	Constant variables
//******************************************************************************
const GLfloat g_p_red_material[] = {1.0, 0.0, 0.0, 1.0};
const GLfloat g_p_white_material[] = {1.0, 1.0, 1.0, 1.0};

const GLfloat g_p_ambiant_chrome_material[] = {0.25, 0.25, 0.25, 1.0};
const GLfloat g_p_diffuse_chrome_material[] = {0.4, 0.4, 0.4, 1.0};
const GLfloat g_p_specular_chrome_material[] = {0.774597, 0.774597, 0.774597, 1.0};
const GLfloat g_shininess_chrome_material = 76.8;

const GLfloat g_p_ambiant_silver_material[] = {0.19225, 0.19225, 0.19225, 1.0};
const GLfloat g_p_diffuse_silver_material[] = {0.50754, 0.50754, 0.50754, 1.0};
const GLfloat g_p_specular_silver_material[] = {0.508273, 0.508273, 0.508273, 1.0};
const GLfloat g_shininess_silver_material = 51.2;

const GLfloat g_p_ambiant_polished_silver_material[] = {0.23125, 0.23125, 0.23125, 1.0};
const GLfloat g_p_diffuse_polished_silver_material[] = {0.2775, 0.2775, 0.2775, 1.0};
const GLfloat g_p_specular_polished_silver_material[] = {0.773911, 0.773911, 0.773911, 1.0};
const GLfloat g_shininess_polished_silver_material = 89.6;

const GLfloat* g_p_ambiant_material(g_p_ambiant_silver_material);
const GLfloat* g_p_diffuse_material(g_p_diffuse_silver_material);
const GLfloat* g_p_specular_material(g_p_specular_silver_material);
const GLfloat g_shininess_material(g_shininess_silver_material);

int g_u_source_point(1);
int g_v_source_point(1);
Vec2ui g_xray_source_sample_number(g_u_source_point, g_v_source_point);
//Vec2ui g_xray_source_sample_number(5, 5);

float g_u_source_size(1.0*mm);
float g_v_source_size(1.0*mm);
VEC3 g_xray_source_sample_u_offset((g_u_source_size / g_xray_source_sample_number.getX()), 0.0*mm, 0.0*mm);
VEC3 g_xray_source_sample_v_offset(0.0*mm, (g_v_source_size / g_xray_source_sample_number.getY()), 0.0*mm);

VEC3 g_detector_rotation(0, 0, 0);

bool g_adjust_shift_scale(false);
bool g_display_ready(false);

float g_lowest_profile_intensity(0);
float g_highest_profile_intensity(0);


VEC3            g_xray_source_position(258.5*mm, -1000*cm, 200*mm);
VEC3            g_detector_position(   258.5*mm,     70*cm, 200*mm);
VEC3            g_up_vector(0, 0, 1);

float g_incident_photon_number(300);
float g_shift(0.0);
float g_scale(1);

RATIONAL_NUMBER g_projection_z_near(0.01);
RATIONAL_NUMBER g_projection_z_far(50000);

RATIONAL_NUMBER g_resolution_in_centimetres_per_pixel(2.0 * 1.75 / 2.25);
Vec2ui          g_size_in_pixels(256, 256);
GLfloat g_zoom(GLfloat(120 * cm));

int g_parallel_beam(0);

bool g_build_sinogram(false);
int  g_number_of_angles(180);
int g_current_angle(0);



/*#elif defined(CUBE)
VEC3            g_xray_source_position(0, 0, -7.5*cm);
VEC3            g_detector_position(   0, 0,  5*cm);
VEC3            g_up_vector(0, -1, 0);

RATIONAL_NUMBER g_incident_photon_number(1);
RATIONAL_NUMBER g_shift(100.0);
RATIONAL_NUMBER g_scale(1);

RATIONAL_NUMBER g_projection_z_near(0.01);
RATIONAL_NUMBER g_projection_z_far(5000);

RATIONAL_NUMBER g_resolution_in_centimetres_per_pixel(0.6);
Vec2ui          g_size_in_pixels(512, 512);
GLfloat g_zoom(GLfloat(30 * cm));
#elif defined(INSIDE)
#define TEST_NAME "inside"
VEC3            g_xray_source_position(0*mm, 10*cm, 1500*mm);
VEC3            g_detector_position(   0*mm, 10*cm, -250*mm);
VEC3            g_up_vector(0, 1, 0);

RATIONAL_NUMBER g_incident_photon_number(50);
RATIONAL_NUMBER g_shift(0.0);
RATIONAL_NUMBER g_scale(1);

RATIONAL_NUMBER g_projection_z_near(0.01);
RATIONAL_NUMBER g_projection_z_far(5000);

RATIONAL_NUMBER g_resolution_in_centimetres_per_pixel(1.5);
Vec2ui          g_size_in_pixels(512, 512);
GLfloat g_zoom(GLfloat(120 * cm));
#elif defined(CUBE_AND_CYLINDER)
#endif
*/

const RATIONAL_NUMBER g_epsilon(0.00001);

const char* g_mu_water_table_filename(PREFIX"/data/mu_water.csv");

const char* g_unfiltered_beam_spectrum_table_filename(PREFIX"/data/90kv_unfiltered.txt");
const char* g_filtered_beam_spectrum_table_filename(PREFIX"/data/90kv_filtered_5mmCU_5mmAIR_5mmCU.txt");

const char* g_LBuffer_vertex_shader_filename(PREFIX"/shaders/l_buffer.vert");
const char* g_LBuffer_fragment_shader_filename(PREFIX"/shaders/l_buffer.frag");

const char* g_MuX_Dx_vertex_shader_filename(PREFIX"/shaders/MUx_Dx.vert");
const char* g_MuX_Dx_fragment_shader_filename(PREFIX"/shaders/MUx_Dx.frag");

const char* g_attenuation_vertex_shader_filename(PREFIX"/shaders/xray_attenuation.vert");
const char* g_attenuation_fragment_shader_filename(PREFIX"/shaders/xray_attenuation.frag");

const char* g_display_vertex_shader_filename(PREFIX"/shaders/xray_display.vert");
const char* g_display_fragment_shader_filename(PREFIX"/shaders/xray_display.frag");

const char* g_screenshot_prefix(PREFIX"/data/screenshot/screenshot_");

const char* g_sinogram_prefix(PREFIX"/data/sinogram");

const GLfloat g_rotation_speed(2.0);

/*
#ifdef CUBE
const char* g_cube_filename(PREFIX"/data/cube_bin.stl");
#endif

#ifdef INSIDE
const char* g_skin_filename(PREFIX"/data/"TEST_NAME"/skin.stl");
const char* g_bones_filename(PREFIX"/data/"TEST_NAME"/bones.stl");
const char* g_diaphragm_filename(PREFIX"/data/"TEST_NAME"/diaphragm.stl");
const char* g_liver_filename(PREFIX"/data/"TEST_NAME"/liver.stl");
const char* g_lungs_filename(PREFIX"/data/"TEST_NAME"/lungs.stl");
const char* g_ligaments_filename(PREFIX"/data/"TEST_NAME"/ligaments.stl");
#endif

*/

//******************************************************************************
//	Global variables
//******************************************************************************
GLsizei g_main_window_width(1280);
GLsizei g_main_window_height(800);
int g_main_window_id(0);
GLsizei g_profile_window_width(256);
GLsizei g_profile_window_height(256);
int g_profile_window_id(0);

GLfloat g_sample_rotation_x(0.0);
GLfloat g_sample_rotation_y(0.0);
GLfloat g_sample_rotation_z(0.0);

int g_button(-1);
int g_button_state(-1);
bool g_use_arc_ball(false);
GLint g_last_x_position(0);
GLint g_last_y_position(0);
GLint g_current_x_position(0);
GLint g_current_y_position(0);

Matrix4x4<GLfloat> g_scene_rotation_matrix;
Matrix4x4<GLfloat> g_detector_rotation_matrix;
Matrix4x4<GLfloat> g_sample_rotation_matrix;

GLuint g_screenshot_counter(0);

//Shader g_l_buffer_computation_shader_program;
//FramebufferObject* g_L_buffer_fbo(0);
//GLuint l_buffer_texture(0);

int g_negative_display(1);

TriangleMesh g_pelvis_skin;
TriangleMesh g_pelvis_bones;
TriangleMesh g_pelvis_bowels;
TriangleMesh g_pelvis_muscle;
TriangleMesh g_pelvis_fat;

TriangleMesh g_dragon;

TriangleMesh g_cubes;

/*#elif defined (CUBE)
int g_negative_display(1);
TriangleMesh g_cube;
#elif defined(INSIDE)
int g_negative_display(1);
*/

TriangleMesh g_cube;
TriangleMesh g_cylinder;

TriangleMesh g_inside_bones;
TriangleMesh g_inside_diaphragm;
TriangleMesh g_inside_ligaments;
TriangleMesh g_inside_liver;
TriangleMesh g_inside_lungs;
TriangleMesh g_inside_skin;

Shader m_shader_set[1];
GLuint m_p_texture_name_set[] = {0};
GLuint m_p_fbo_set[] = {0};

XRayDetector g_detector;
XRayBeam g_xray_beam;
XRayRenderer g_xray_renderer;

float g_p_view_rotatation[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
float g_object_position[] = {0.0, 0.0, 0.0};


int g_beam_spectrum(4);

int g_artefact_correction(1);

std::list<std::vector<float> > g_profiles;
/*
GLenum m_proxy_texture_target(GL_PROXY_TEXTURE_2D);
GLenum m_texture_target(GL_TEXTURE_2D);
GLenum g_texture_32f(GL_RGBA32F_ARB);
GLenum g_texture_16f(GL_RGBA16F_ARB);
GLenum g_internal_texture_format(GL_RGBA32F_ARB);
*/
GLUI_Spinner* g_p_input_intensity(0);
GLUI_Spinner* g_p_shift_filter(0);
GLUI_Spinner* g_p_scale_filter(0);

GLUI_Spinner* g_p_u_source_size(0);
GLUI_Spinner* g_p_v_source_size(0);
FILE* g_p_file(0);
float* g_p_sinogram(0);
bool g_first_time(true);


//******************************************************************************
//	Function declaration
//******************************************************************************
void init();
void initSample(int aSampleId);
void initBeam();
void initDetector();
void display();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int button_state, int x, int y);
void motion(int x, int y);
void idle();
void quit();
void buildGUI();
void controlCallback(int aWidgetId);

void displayProfile();
void reshapeProfile(int width, int height);

void loadPelvis();
void loadDragon();
void loadCubes();
void loadCubeAndCylinder();
void loadInside();
void computeRotation(MATRIX4& aRotationMatrix);
Vec3<GLfloat> getArcballVector(int x, int y);

void showAxis(GLfloat aSize = 1.0);
#ifdef HAS_FREEIMAGE
void screenshot();
#endif // HAS_FREEIMAGE
/*
inline GLuint CreateTexture()
{
	GLuint texture_name(0);
	glGenTextures(1, &texture_name);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	glBindTexture(m_texture_target, texture_name);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	glTexParameteri(m_texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(m_texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(m_texture_target, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(m_texture_target, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	glTexImage2D(m_texture_target, 0, g_internal_texture_format, g_detector.getWidthInPixels(), g_detector.getHeightInPixels(), 0, GL_RGB, GL_FLOAT, 0);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	return texture_name;
}


inline 	FramebufferObject* CreateFbo(GLuint aTextureName)
{
	FramebufferObject* p_temp(new FramebufferObject());
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	p_temp->Bind();
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	p_temp->AttachTexture(m_texture_target, aTextureName, GL_COLOR_ATTACHMENT0_EXT);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	
	return p_temp;
}
*/

//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
	// Register the exit callback
	atexit(quit);
	
	// Initialize GLUT
	glutInit(&argc, argv);

	// Create a GLUT window
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

	glutInitWindowSize(g_main_window_width, g_main_window_height);
	g_main_window_id = glutCreateWindow("GPU X-Ray Demo");

	// Initialize GLEW
    glewExperimental = GL_TRUE;
	GLenum glew_error(glewInit());
	if (glew_error != GLEW_OK)
	{
		std::cerr << "OpenGL error: cannot initialize GLEW: " << glewGetErrorString(glew_error) << std::endl;
		//return EXIT_FAILURE;
	}

	// Initialise OpenGL
	init();
	buildGUI();

	checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Initialize GLUT callback
	glutDisplayFunc(display);
	GLUI_Master.set_glutReshapeFunc(reshape);
	GLUI_Master.set_glutMouseFunc(mouse);
	glutMotionFunc(motion);
	GLUI_Master.set_glutKeyboardFunc(keyboard);
	GLUI_Master.set_glutIdleFunc(idle);
	

	// Set the X-ray detector properties
	initDetector();
	checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Load beam spectrum
	initBeam();
	checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Initialise the X-ray renderer
	if (!g_xray_renderer.initialise(XRayRenderer::OPENGL,
		GL_RGB16F_ARB,
		&g_detector,
		g_mu_water_table_filename,
		&g_xray_beam,
		g_LBuffer_vertex_shader_filename, g_LBuffer_fragment_shader_filename,
		g_MuX_Dx_vertex_shader_filename, g_MuX_Dx_fragment_shader_filename,
		g_attenuation_vertex_shader_filename, g_attenuation_fragment_shader_filename,
		g_display_vertex_shader_filename, g_display_fragment_shader_filename))
	{
		std::cerr << "Cannot initialise the X-ray renderer." << std::endl;
		return (-1);
	}
	checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	controlCallback(LOAD_SAMPLE);

	controlCallback(ARTEFACT_CORRECTION);
    
	g_xray_renderer.setXRaySourceSampleUOffset(g_xray_source_sample_u_offset);
	g_xray_renderer.setXRaySourceSampleVOffset(g_xray_source_sample_v_offset);
	g_xray_renderer.setXRaySourceSampleNumber(g_xray_source_sample_number);

	g_xray_renderer.setInputPhotonNumberScalingFactor(g_incident_photon_number);
	g_xray_renderer.setShiftFilter(g_shift);
	g_xray_renderer.setScaleFilter(g_scale);
	g_xray_renderer.setNegativeDisplayFlag(g_negative_display);

    controlCallback(BEAM_GEOMETRY);
	checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	glFlush();
	glutSwapBuffers();
	
	glutMainLoop();
	
	return (EXIT_SUCCESS);
}


//---------
void quit()
//---------
{
#ifdef HAS_FREEIMAGE
	FreeImage_DeInitialise();
#endif // HAS_FREEIMAGE

	exit(EXIT_SUCCESS);
}


//---------
void init()
//---------
{
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE, GL_ONE);

	glDisable(GL_TEXTURE_1D);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_3D);
	glDisable(GL_TEXTURE_RECTANGLE_NV);
	glDisable(GL_TEXTURE_RECTANGLE_ARB);
	glDisable(GL_TEXTURE_RECTANGLE_EXT);
	
	glEnable(GL_RESCALE_NORMAL);
	glClearColor(0.5, 0.5, 0.5, 1.0);
//	glClearColor(1.0, 1.0, 1.0, 1.0);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	glEnable(GL_DEPTH_TEST);
	
	glShadeModel(GL_SMOOTH);
	
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	
//	glPointSize(1.5);
	
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
/*
	//affichage d'un ecran noir
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	glutSwapBuffers();
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
*/
}


void initDetector()
{
	g_detector.setResolutionInUnitsOfLengthPerPixel(g_resolution_in_centimetres_per_pixel);

	g_detector.setNumberOfPixels(g_size_in_pixels);

	g_detector.setXraySourcePosition(g_xray_source_position);
	g_detector.setDetectorPosition(g_detector_position);
	g_detector.setUpVector(g_up_vector);

	g_detector.setProjectionZNear(g_projection_z_near);
	g_detector.setProjectionZFar(g_projection_z_far);
	
	//g_detector.SetRotationAngles(g_detector_rotation);

	if (g_p_shift_filter) g_p_shift_filter->set_float_val(g_shift);
	if (g_p_scale_filter) g_p_scale_filter->set_float_val(g_scale);
	if (g_p_input_intensity) g_p_input_intensity->set_float_val(g_incident_photon_number);

	g_xray_renderer.setShiftFilter(g_shift);
	g_xray_renderer.setScaleFilter(g_scale);
	g_xray_renderer.setInputPhotonNumberScalingFactor(g_incident_photon_number);
}


//----------------------------
void initSample(int aSampleId)
//----------------------------
{
	try
	{
		g_xray_renderer.ClearSurface();
	#ifdef HAS_ITK
		g_xray_renderer.ClearVolume();
	#endif // HAS_ITK
		g_profiles.clear();
		if (g_profile_window_id)
		{
			glutSetWindow(g_profile_window_id);
			glutPostRedisplay();

			glutSetWindow(g_main_window_id);
		}

		switch (aSampleId)
		{
		case PELVIS:
			loadPelvis();
			break;


		case DRAGON:
			loadDragon();
			break;

		case CUBES:
			loadCubes();
			break;
		/*
		case CUBE:
			g_cube.setFilename(g_cube_filename);
			if (!g_cube.loadSTLFile(false, true, false, true, cm))
			{
				std::cerr << "Cannot load STL file \"" << g_cube.getFilename() << "\"." << std::endl;
				return EXIT_FAILURE;
			}
			g_cube.setHounsfieldValue(BONES_HU);
			g_xray_renderer.AddInnerSurface(&g_cube);
			break;

		case INSIDE:
			g_skin.setFilename(g_skin_filename);
			if (!g_skin.loadSTLFile(false, true, false, true, cm))
			{
				std::cerr << "Cannot load STL file \"" << g_skin.getFilename() << "\"." << std::endl;
				return -1;
			}
			g_skin.setHounsfieldValue(SKIN_HU);

			g_bones.setFilename(g_bones_filename);
			if (!g_bones.loadSTLFile(false, true, false, true, cm))
			{
				std::cerr << "Cannot load STL file \"" << g_bones.getFilename() << "\"." << std::endl;
				return -1;
			}
			g_bones.setHounsfieldValue(BONES_HU);

			g_diaphragm.setFilename(g_diaphragm_filename);
			if (!g_diaphragm.loadSTLFile(false, true, false, true, cm))
			{
				std::cerr << "Cannot load STL file \"" << g_diaphragm.getFilename() << "\"." << std::endl;
				return -1;
			}
			g_diaphragm.setHounsfieldValue(DIAPHRAGM_HU);


			g_liver.setFilename(g_liver_filename);
			if (!g_liver.loadSTLFile(false, true, false, true, cm))
			{
				std::cerr << "Cannot load STL file \"" << g_liver.getFilename() << "\"." << std::endl;
				return -1;
			}
			g_liver.setHounsfieldValue(LIVER_HU);

			g_lungs.setFilename(g_lungs_filename);
			if (!g_lungs.loadSTLFile(false, true, false, true, cm))
			{
				std::cerr << "Cannot load STL file \"" << g_lungs.getFilename() << "\"." << std::endl;
				return -1;
			}
			g_lungs.setHounsfieldValue(LUNG_HU);

			g_ligaments.setFilename(g_ligaments_filename);
			if (!g_ligaments.loadSTLFile(false, true, false, true, cm))
			{
				std::cerr << "Cannot load STL file \"" << g_ligaments.getFilename() << "\"." << std::endl;
				return -1;
			}
			g_ligaments.setHounsfieldValue(LIGAMENT_HU);
			g_xray_renderer.AddOuterSurface(&g_skin);
			g_xray_renderer.AddInnerSurface(&g_bones);
			g_xray_renderer.AddInnerSurface(&g_diaphragm);
			g_xray_renderer.AddInnerSurface(&g_liver);
			g_xray_renderer.AddInnerSurface(&g_lungs);
			g_xray_renderer.AddInnerSurface(&g_ligaments);
			break;
			*/
		case CUBE_AND_CYLINDER:
			loadCubeAndCylinder();
			break;

		case INSIDE:
			loadInside();
			break;
		}
		
		g_shift = g_xray_renderer.GetShiftFilter();
		if (g_p_shift_filter) g_p_shift_filter->set_float_val(g_shift);
		
		g_scale = g_xray_renderer.GetScaleFilter();
		if (g_p_scale_filter) g_p_scale_filter->set_float_val(g_scale);

		reshape(g_main_window_width, g_main_window_height);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << "The program will terminate." << std::endl;
		exit(EXIT_FAILURE);
	}
}


void initBeam()
{
	g_xray_beam.setNumberOfPhotonsLowerThreshold(g_epsilon);

	if (g_beam_spectrum == 0)
	{
		g_xray_beam.initialise(g_unfiltered_beam_spectrum_table_filename, keV);
	}
	else if (g_beam_spectrum == 1)
	{
		g_xray_beam.initialise(33 * keV);
	}
	else if (g_beam_spectrum == 2)
	{
		g_xray_beam.initialise(44 * keV);
	}
	else if (g_beam_spectrum == 3)
	{
		g_xray_beam.initialise(g_filtered_beam_spectrum_table_filename, keV);
	}
	else if (g_beam_spectrum == 4)
	{
		g_xray_beam.initialise(81 * keV);
	}
	else if (g_beam_spectrum == 5)
	{
		g_xray_beam.initialise(80 * keV);
	}
	g_xray_beam.normalise();
}


//------------
void display()
//------------
{
	try
	{
		glFlush();
		
		if (!g_first_time)
		{
			//if (!g_display_ready)
			{
				g_xray_renderer.ComputeImage(g_sample_rotation_x, g_sample_rotation_y, g_sample_rotation_z);

				if (g_adjust_shift_scale)
				{
					g_xray_renderer.AutoShiftScale();

					g_shift = g_xray_renderer.GetShiftFilter();
					g_p_shift_filter->set_float_val(g_shift);

					g_scale = g_xray_renderer.GetScaleFilter();
					g_p_scale_filter->set_float_val(g_scale);

					g_adjust_shift_scale = false;
				}
				checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
			}

			// Bind the default buffer
			glFlush();

			checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			glPushAttrib(GL_ENABLE_BIT);
			glPushMatrix();

			glTranslatef(g_object_position[0], g_object_position[1], -g_object_position[2]);
			glMultMatrixf(g_p_view_rotatation);

			glMultMatrixf(g_scene_rotation_matrix.get());



			//ShowAxis(50);

			glPushMatrix();
			glRotatef(g_sample_rotation_z, 0.0, 0.0, 1.0);
			glRotatef(g_sample_rotation_y, 0.0, 1.0, 0.0);
			glRotatef(g_sample_rotation_x, 1.0, 0.0, 0.0);

			glEnable(GL_LIGHTING);

			switch (g_sample_id)
			{
			case PELVIS:
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
				g_pelvis_skin.display(false);
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
				break;
	
			case DRAGON:
				glMaterialfv(GL_FRONT, GL_AMBIENT, g_p_ambiant_material);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, g_p_diffuse_material);
				glMaterialfv(GL_FRONT, GL_SPECULAR, g_p_specular_material);
				glMaterialf(GL_FRONT, GL_SHININESS, g_shininess_material);

				g_dragon.display(false);
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
				break;
	
			case CUBES:
				g_cubes.display(false);
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
				break;
	
			case CUBE_AND_CYLINDER:
				glPushAttrib(GL_ENABLE_BIT);
				glPushAttrib(GL_DEPTH_BUFFER_BIT);
				glPushAttrib(GL_COLOR_BUFFER_BIT);
				glPushAttrib(GL_CURRENT_BIT);

				glEnable(GL_COLOR_MATERIAL);
				glColor4f(0.0, 1.0, 1.0, 0.75);
				g_cylinder.display(false);
		
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		
			//	glDisable(GL_LIGHTING);
				glDepthFunc(GL_ALWAYS);
				glEnable(GL_BLEND);
				glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glColor4f(1, 0.0, 0.0, 0.25);
				g_cube.display(false);
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

				glPopAttrib();
				glPopAttrib();
				glPopAttrib();
				glPopAttrib();
				break;

			case INSIDE:
				g_inside_skin.display(false);
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
				break;
			}
		
		/*
	
		#elif defined(CUBE)
			g_cube.Render(false);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		#elif defined(INSIDE)
			g_skin.Render(false);
			//g_bones.Render(false);
			//g_diaphragm.Render(false);
			//g_liver.Render(false);
			//g_lungs.Render(false);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		#elif defined(CUBE_AND_CYLINDER)
		#endif*/
			glDisable(GL_LIGHTING);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
			glPopMatrix();
	
			glColor3f(1, 0, 0);
			glFlush();
			if (g_display_ready)
			{
				g_xray_renderer.Render();
				g_detector.displaySource();
				g_detector.displayBeam();
			}
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			glPopMatrix();
			glPopAttrib();
	
	
		/*	glPushAttrib(GL_VIEWPORT);
			int tx, ty, tw, th;
			GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th);
			glViewport(tx, ty, g_size_in_pixels.getX(), g_size_in_pixels.getY());
	
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0, 1, 0, 1, -1, 1);
	
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			if (g_display_ready) g_xray_renderer.Display();
			glPopMatrix();
	
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();

			glMatrixMode(GL_MODELVIEW);

			glPopAttrib();
		*/
			glFlush();
			glutSwapBuffers();

			if (!g_display_ready)
			{
				g_display_ready = true;
				glutPostRedisplay();
			}
		}
	}
	catch (const OpenGLException& error)
	{
		std::cerr << error << std::endl;
	}
	catch (const Exception& error)
	{
		std::cerr << error << std::endl;
	}
	catch (const std::exception& error)
	{
		std::cerr << error.what() << std::endl;
	}

	g_first_time = false;
}


//-------------------
void displayProfile()
//-------------------
{
	glClear(GL_COLOR_BUFFER_BIT);

	float red = 0.0f;
	float green = 0.0f;
	float blue = 0.0f;
		
	for (std::list<std::vector<float> >::const_iterator ite_profile(g_profiles.begin()); ite_profile != g_profiles.end(); ++ite_profile)
	{
		int i(0);

		red -= .27f;
		green -= .36f;
		blue -= .45f;

		if (red < 0.15) red = 1.0f;
		if (green < 0.15) green = 1.0f;
		if (blue < 0.15) blue = 1.0f;

		glColor3f(red, green, blue);

		glBegin(GL_LINE_STRIP);
		for (std::vector<float>::const_iterator ite(ite_profile->begin()); ite != ite_profile->end(); ++ite, ++i)
		{
			glVertex3f(i, log(*ite), 0);
		}
		glEnd();
	}
	
	glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		g_xray_renderer.Reset();
		exit(EXIT_SUCCESS);
		break;
/*
	case 'r':
		{
			GLubyte* p_data(new GLubyte[g_main_window_width * g_main_window_height * 4]);

			glReadPixels(0, 0, g_main_window_width, g_main_window_height, GL_RGBA, GL_UNSIGNED_BYTE, p_data);

			int bpp(4 * sizeof(GLubyte) * 8);
			int pitch = ((((bpp * g_main_window_width) + 31) / 32) * 4);
			FIBITMAP* bitmap = FreeImage_ConvertFromRawBits(p_data, g_main_window_width, g_main_window_height, pitch, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, false);

			// save as a regular file
			std::string screen_shot_file_name(GetFileName(g_screenshot_prefix, g_screenshot_counter++, 6, ".png"));
#if (FREEIMAGE_MAJOR_VERSION == 3) && (FREEIMAGE_MINOR_VERSION >= 11)
			if (!FreeImage_Save(FIF_PNG, bitmap, screen_shot_file_name.c_str(), PNG_Z_BEST_COMPRESSION))
#else
			if (!FreeImage_Save(FIF_PNG, bitmap, screen_shot_file_name.c_str(), PNG_DEFAULT))
#endif
			{
				std::cerr << "WARNING: could not write \"" << screen_shot_file_name << "\"" << std::endl;
			}

		//	if (!FreeImage_Save(FIF_PNG | PNG_Z_BEST_COMPRESSION, bitmap, screen_shot_file_name, 0))
			FreeImage_Unload(bitmap);

			delete [] p_data;
		}
		break;
		
	case 'f':
		g_xray_renderer.PrintFramebuffer();
		break;
	
	case 'n':
		g_negative_display = !g_negative_display;
		g_xray_renderer.SetNegativeDisplayFlag(g_negative_display);
		break;
		
	case 'b':
		g_xray_renderer.SetUseFilter(!g_xray_renderer.getUseFilter());
		break;
*/
	default:
		break;
	}
	
	glutPostRedisplay();
}


//--------------------------
void showAxis(GLfloat aSize)
//--------------------------
{
	glPushAttrib(GL_CURRENT_BIT);
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(aSize, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, aSize, 0.0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, aSize);
	glEnd();
	glPopAttrib();
	glPopAttrib();
}


//---------------------------------
void reshape(int width, int height)
//---------------------------------
{
	int tx, ty, tw, th;
	GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th);

	g_main_window_width = width;
	g_main_window_height = height;

	g_xray_renderer.SetPosition(Vec2ui(5, th - 5 - g_detector.getNumberOfPixels().getY()));

	glViewport(tx, ty, tw, th);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, float(tw) / float(th), 10.0, 5000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, g_zoom,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
}


//----------------------------------------
void reshapeProfile(int width, int height)
//----------------------------------------
{
	g_profile_window_width  = width;
	g_profile_window_height = height;

	glViewport(0, 0, g_profile_window_width, g_profile_window_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, g_size_in_pixels.getX() - 1, log(g_lowest_profile_intensity), log(g_highest_profile_intensity), -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


//----------------------------------------------------
void mouse(int button, int button_state, int x, int y)
//----------------------------------------------------
{
	g_button = button;
	g_button_state = button_state;

	// Use the arc ball
	if (((g_button == GLUT_LEFT_BUTTON) || (g_button == GLUT_RIGHT_BUTTON) || (g_button == GLUT_MIDDLE_BUTTON)) &&
			(g_button_state == GLUT_DOWN))
	{
		g_last_x_position = x;
		g_last_y_position = y;

		g_current_x_position = x;
		g_current_y_position = y;

		g_use_arc_ball = true;
	}
	else
	{
		g_use_arc_ball = false;
	}

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
	switch (g_button)
	{
	case GLUT_LEFT_BUTTON:
		computeRotation(g_scene_rotation_matrix);
		break;

	case GLUT_RIGHT_BUTTON:
		computeRotation(g_detector_rotation_matrix);
		g_detector.setRotationMatrix(g_detector_rotation_matrix);
		break;

	case GLUT_MIDDLE_BUTTON:
		computeRotation(g_sample_rotation_matrix);
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

//	g_xray_renderer.ComputeImage(g_sample_rotation_x, g_sample_rotation_y, g_sample_rotation_z);

    if (g_build_sinogram)
    {
        // Record the data
        if (g_current_angle < g_number_of_angles)
        {
            // Initialise the sinogram
            if (g_current_angle == 0)
            {
                std::stringstream filename;
                filename << g_sinogram_prefix;
                filename << "_" << g_detector.getNumberOfPixels().getX() << "x" << g_number_of_angles << "x" << g_detector.getNumberOfPixels().getY() << ".raw";
                
                // Close the file
                if (g_p_file)
                {
                    fclose(g_p_file);
                    g_p_file = 0;
                    
                }
                
                // Open the file
                g_p_file = fopen(filename.str().c_str(), "wb");
                
                // Release the memory
                if (g_p_sinogram)
                {
                    delete [] g_p_sinogram;
                    g_p_sinogram = 0;
                }
                
                // Allocate memory
                g_p_sinogram = new float[g_detector.getNumberOfPixels().getX() * g_number_of_angles * g_detector.getNumberOfPixels().getY()];
            }
            
            // Record the data
            if (g_xray_renderer.LoadAttenuationToRAM())
            {
                for (int k(0); k < g_detector.getNumberOfPixels().getY(); ++k)
                {
                    memcpy(g_p_sinogram + k * g_number_of_angles * g_detector.getNumberOfPixels().getX() + g_current_angle * g_detector.getNumberOfPixels().getX(), g_xray_renderer.GetAttenuation() + k * g_detector.getNumberOfPixels().getX(), g_detector.getNumberOfPixels().getX() * sizeof(float));
                }
            }
            
            MATRIX4 rotation;
            rotation.rotate(180.0 / RATIONAL_NUMBER(g_number_of_angles), g_detector.getUpVector());
            
            // Move source
            VEC3 source_position(g_detector.getXraySourcePosition());
            source_position = rotation * source_position;
            g_detector.setXraySourcePosition(source_position);
            
            // Move detector
            VEC3 detector_position(g_detector.getDetectorPosition());
            detector_position = rotation * detector_position;
            g_detector.setDetectorPosition(detector_position);

            g_current_angle++;
        }
        else
        {
            g_build_sinogram = false;
            
            // Record the data
            float* p_data(g_p_sinogram);
            for (int i(0); i < g_detector.getNumberOfPixels().getY() * g_number_of_angles; ++i, p_data += g_detector.getNumberOfPixels().getX())
            {
                fwrite(p_data, g_detector.getNumberOfPixels().getX(), sizeof(float), g_p_file);
            }
            
            g_xray_renderer.PrintAttenuation();
            
            // Close the file
            if (g_p_file)
            {
                fclose(g_p_file);
                g_p_file = 0;
            }
            
            // Release the memory
            if (g_p_sinogram)
            {
                delete [] g_p_sinogram;
                g_p_sinogram = 0;
            }
            
            g_current_angle = 0;
        }
    }
    
	glutPostRedisplay();
}


//-------------
void buildGUI()
//-------------
{
	GLUI* p_glui_01(GLUI_Master.create_glui_subwindow(g_main_window_id, GLUI_SUBWINDOW_RIGHT));
	p_glui_01->set_main_gfx_window(g_main_window_id);

	p_glui_01->add_checkbox("Negative X-ray image", &g_negative_display, NEGATIVE_DISPLAY, controlCallback);
	p_glui_01->add_checkbox("Artefact correction", &g_artefact_correction, ARTEFACT_CORRECTION, controlCallback);

    p_glui_01->add_checkbox("Parallel beam", &g_parallel_beam, BEAM_GEOMETRY, controlCallback);

	GLUI_Panel* p_beam_panel(p_glui_01->add_rollout("Beam", true));
   GLUI_RadioGroup* p_radio_button_group(p_glui_01->add_radiogroup_to_panel(p_beam_panel, &g_beam_spectrum, BEAM_SPECTRUM, controlCallback));
	p_glui_01->add_radiobutton_to_group(p_radio_button_group, "Use unfiltered polychromatism");
///	new GLUI_StaticText(p_radio_button_group, "(peak energy 33keV, mean energy 44keV)");	
	p_glui_01->add_radiobutton_to_group(p_radio_button_group, "Use monochromatism (33keV)");
	p_glui_01->add_radiobutton_to_group(p_radio_button_group, "Use monochromatism (44keV)");

	p_glui_01->add_radiobutton_to_group(p_radio_button_group, "Use filtered polychromatism");
///	new GLUI_StaticText(p_radio_button_group, "(peak energy 81keV, mean energy 80keV)");	
	p_glui_01->add_radiobutton_to_group(p_radio_button_group, "Use monochromatism (81keV)");
	p_glui_01->add_radiobutton_to_group(p_radio_button_group, "Use monochromatism (80keV)");


	p_glui_01->add_button("Auto adjust", AUTO_ADJUST, controlCallback);
	
    p_glui_01->add_button("Build sinogram", BUILD_SINOGRAM, controlCallback);

	GLUI_Panel* p_sample_panel(p_glui_01->add_rollout("Scanned object", true));
   p_radio_button_group = p_glui_01->add_radiogroup_to_panel(p_sample_panel, &g_sample_id, LOAD_SAMPLE, controlCallback);
	p_glui_01->add_radiobutton_to_group(p_radio_button_group, "Pelvis");
	p_glui_01->add_radiobutton_to_group(p_radio_button_group, "Dragon");
	p_glui_01->add_radiobutton_to_group(p_radio_button_group, "Cubes");
	p_glui_01->add_radiobutton_to_group(p_radio_button_group, "Cube and cylinder");
	p_glui_01->add_radiobutton_to_group(p_radio_button_group, "Inside");

	GLUI_Panel* p_source_panel(p_glui_01->add_rollout("Source", true));
	GLUI_Spinner* p_u_source_point = p_glui_01->add_spinner_to_panel(p_source_panel, "Source point along U axis", GLUI_SPINNER_INT, &g_u_source_point, SOURCE_POINTS, controlCallback);
	p_u_source_point->set_int_limits(1, 25);
	GLUI_Spinner* p_v_source_point = p_glui_01->add_spinner_to_panel(p_source_panel, "Source point along Y axis", GLUI_SPINNER_INT, &g_v_source_point, SOURCE_POINTS, controlCallback);
	p_v_source_point->set_int_limits(1, 25);

	g_p_u_source_size = p_glui_01->add_spinner_to_panel(p_source_panel, "Source size along U axis (in mm)", GLUI_SPINNER_FLOAT, &g_u_source_size, SOURCE_SIZE, controlCallback);
	g_p_v_source_size = p_glui_01->add_spinner_to_panel(p_source_panel, "Source size along V axis (in mm)", GLUI_SPINNER_FLOAT, &g_v_source_size, SOURCE_SIZE, controlCallback);
	g_p_u_source_size->set_float_limits(0.001, 50);
	g_p_v_source_size->set_float_limits(0.001, 50);


	g_p_input_intensity = p_glui_01->add_spinner_to_panel(p_source_panel, "Input intensity", GLUI_SPINNER_FLOAT, &g_incident_photon_number, INPUT_INTENSITY, controlCallback);
	g_p_input_intensity->set_float_limits(0.001, 10000);

	g_p_shift_filter = p_glui_01->add_spinner("Shift filter", GLUI_SPINNER_FLOAT, &g_shift, SHIFT_FILER, controlCallback);

	g_p_scale_filter = p_glui_01->add_spinner("Scale filter", GLUI_SPINNER_FLOAT, &g_scale, SCALE_FILER, controlCallback);


	p_glui_01->add_button("Show profle", ADD_PROFILE, controlCallback);
	p_glui_01->add_button("Clear profle", DELETE_PROFILES, controlCallback);

#ifdef HAS_FREEIMAGE
	p_glui_01->add_button("Save screenshot", 0, GLUI_Update_CB(Screenshot));
#endif // HAS_FREEIMAGE

	p_glui_01->add_button("Quit", 0, GLUI_Update_CB(exit));

	GLUI* p_glui_02(GLUI_Master.create_glui_subwindow(g_main_window_id, GLUI_SUBWINDOW_BOTTOM));
	p_glui_02->set_main_gfx_window(g_main_window_id);

	GLUI_Rotation* p_view_rotation(p_glui_02->add_rotation("Objects", g_p_view_rotatation));
	p_view_rotation->set_spin(1.0);

	p_glui_02->add_column(false);

	GLUI_Translation* p_trans_xy(p_glui_02->add_translation("Objects XY", GLUI_TRANSLATION_XY, g_object_position));
	p_trans_xy->set_speed(0.5);

	p_glui_02->add_column(false);

	GLUI_Translation* p_trans_x(p_glui_02->add_translation("Objects X", GLUI_TRANSLATION_X, g_object_position));
	p_trans_x->set_speed(0.5);

	p_glui_02->add_column(false);

	GLUI_Translation* p_trans_y(p_glui_02->add_translation("Objects Y", GLUI_TRANSLATION_Y, &g_object_position[1]));
	p_trans_y->set_speed(0.5);

	p_glui_02->add_column(false);

	GLUI_Translation* p_trans_z(p_glui_02->add_translation("Objects Z", GLUI_TRANSLATION_Z, &g_object_position[2]));
	p_trans_z->set_speed(1.5);
}

void controlCallback(int aWidgetId)
{
	switch (aWidgetId)
	{
    case BUILD_SINOGRAM:
        g_build_sinogram = true;
        g_current_angle = 0;
        break;
            
	case BEAM_SPECTRUM:
		initBeam();
		if (!g_xray_renderer.InitialiseBeamSpectrum(&g_xray_beam, GL_RGB16F_ARB))
		{
			std::cerr << "Cannot initialise the X-ray renderer." << std::endl;
		}
		break;
    
    case BEAM_GEOMETRY:
        if (g_parallel_beam)
        {
        	g_detector.setParallelBeam();
        }
        else
        {
        	g_detector.setPointSource();
        }
        break;

	case NEGATIVE_DISPLAY:
		g_xray_renderer.SetNegativeDisplayFlag(g_negative_display);
		break;
		
	case INPUT_INTENSITY:
		g_xray_renderer.SetInputPhotonNumberScalingFactor(g_incident_photon_number);
		break;

	case SHIFT_FILER:
		g_xray_renderer.SetShiftFilter(g_shift);
		break;

	case SCALE_FILER:
		g_xray_renderer.SetScaleFilter(g_scale);
		break;
		
	case ARTEFACT_CORRECTION:
		g_xray_renderer.SetUseFilter(g_artefact_correction);
		break;
		
	case LOAD_SAMPLE:
		initSample(g_sample_id);
		break;
		
	case AUTO_ADJUST:
		g_adjust_shift_scale = true;
		break;
	
	case SOURCE_POINTS:
		if (g_u_source_point == 1) g_p_u_source_size->disable();
		else g_p_u_source_size->enable();

		if (g_v_source_point == 1) g_p_v_source_size->disable();
		else g_p_v_source_size->enable();

		g_xray_source_sample_number.setX(g_u_source_point);
		g_xray_source_sample_number.setY(g_v_source_point);
		g_xray_renderer.SetXRaySourceSampleNumber(g_xray_source_sample_number);

		if (g_xray_source_sample_number.getX() - 1)
			g_xray_source_sample_u_offset.setX((g_u_source_size / (g_xray_source_sample_number.getX() - 1))*mm);

		if (g_xray_source_sample_number.getY() - 1)
			g_xray_source_sample_v_offset.setY((g_v_source_size / (g_xray_source_sample_number.getY() - 1))*mm);

		g_xray_renderer.SetXRaySourceSampleUOffset(g_xray_source_sample_u_offset);
		g_xray_renderer.SetXRaySourceSampleVOffset(g_xray_source_sample_v_offset);
		break;
		
	case SOURCE_SIZE:
		if (g_xray_source_sample_number.getX() - 1)
			g_xray_source_sample_u_offset.setX((g_u_source_size / (g_xray_source_sample_number.getX() - 1))*mm);

		if (g_xray_source_sample_number.getY() - 1)
			g_xray_source_sample_v_offset.setY((g_v_source_size / (g_xray_source_sample_number.getY() - 1))*mm);

		g_xray_renderer.SetXRaySourceSampleUOffset(g_xray_source_sample_u_offset);
		g_xray_renderer.SetXRaySourceSampleVOffset(g_xray_source_sample_v_offset);
		break;
		
	case ADD_PROFILE:
		{
			// Compute image
			g_xray_renderer.ComputeImage(g_sample_rotation_x, g_sample_rotation_y, g_sample_rotation_z);

			// Extract profile
			g_xray_renderer.LoadAttenuationToRAM();
			float* p_attenuation(g_xray_renderer.GetAttenuation());
			
			// Normalize
			float min_value(p_attenuation[0]);
			float max_value(p_attenuation[0]);
			
			for (unsigned int i(0); i < g_size_in_pixels.getX() * g_size_in_pixels.getY(); ++i)
			{
				if (min_value > p_attenuation[i]) min_value = p_attenuation[i];
				if (max_value < p_attenuation[i]) max_value = p_attenuation[i];
			}
			
			float range(max_value - min_value);
			for (unsigned int i(0); i < g_size_in_pixels.getX() * g_size_in_pixels.getY(); ++i)
			{
				p_attenuation[i] = (p_attenuation[i] - min_value) / range;
			}
			
			// Get the center line
			p_attenuation = g_xray_renderer.GetAttenuation() + g_size_in_pixels.getX() * g_size_in_pixels.getY() / 2;

			float old_lowest_profile_intensity(g_lowest_profile_intensity);
			float old_highest_profile_intensity(g_highest_profile_intensity);
			std::vector<float> profile;
			
			if (g_profiles.size() == 0)
			{
				g_lowest_profile_intensity = p_attenuation[0];
			}
			
			for (unsigned int i(0); i < g_size_in_pixels.getX(); ++i)
			{
				profile.push_back(p_attenuation[i]);
				if (g_lowest_profile_intensity  > p_attenuation[i]) g_lowest_profile_intensity  = p_attenuation[i];
				if (g_highest_profile_intensity < p_attenuation[i]) g_highest_profile_intensity = p_attenuation[i];
			}
			
			/*for (std::vector<double>::iterator ite(profile.begin()); ite != profile.end(); ++ite)
			{
				*ite /= highest_intensity;
			}*/
			g_profiles.push_back(profile);
			
			if (!g_profile_window_id)
			{
				// Create a GLUT window
				glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

				glutInitWindowSize(256, 256);
				g_profile_window_id = glutCreateWindow("Profile");
				
				glutDisplayFunc(displayProfile);
				glutReshapeFunc(reshapeProfile);
			}

			glutSetWindow(g_profile_window_id);
			if (g_highest_profile_intensity != old_highest_profile_intensity || g_lowest_profile_intensity != old_lowest_profile_intensity)
			{
				reshapeProfile(g_profile_window_width, g_profile_window_height);
			}
			glutPostRedisplay();

			glutSetWindow(g_main_window_id);
		}
		break;
		
	case DELETE_PROFILES:
		g_profiles.clear();
		g_lowest_profile_intensity  = 0;
		g_highest_profile_intensity = 0;
		
		if (g_profile_window_id)
		{
			glutSetWindow(g_profile_window_id);
			glutPostRedisplay();

			glutSetWindow(g_main_window_id);
		}
		break;
	}
	
	glutPostRedisplay();
}


//---------------
void loadPelvis()
//---------------
{
	const char* g_pelvis_skin_filename(PREFIX"/data/hip_pelvis/skin_bin.stl");
	const char* g_pelvis_bones_filename(PREFIX"/data/hip_pelvis/bones_bin.stl");
	const char* g_pelvis_bowels_filename(PREFIX"/data/hip_pelvis/bowels_bin.stl");
	const char* g_pelvis_muscle_filename(PREFIX"/data/hip_pelvis/muscle_bin.stl");
	const char* g_pelvis_fat_filename(PREFIX"/data/hip_pelvis/fat_bin.stl");

	g_xray_source_position = VEC3(0*mm, -100*cm, 0*mm);
	g_detector_position = VEC3(   0*mm,  100*cm, 0*mm);
	g_up_vector = VEC3(0, 0, 1);

	g_incident_photon_number = 10;
	g_shift = -0.02;
	g_scale =  2;

	g_projection_z_near = 0.01;
	g_projection_z_far = 5000;

	g_resolution_in_centimetres_per_pixel = 2.0 * 1.;
	g_size_in_pixels = Vec2ui(256, 256);
	g_zoom = GLdouble(200 * cm);

	// Set detector
	initDetector();

	// Set geometry
	g_pelvis_skin.setFilename(g_pelvis_skin_filename);
	g_pelvis_skin.loadSTLFile(false, true, false, true, mm);
	g_pelvis_skin.setHounsfieldValue(SKIN_HU);

	g_pelvis_bones.setFilename(g_pelvis_bones_filename);
	g_pelvis_bones.loadSTLFile(false, true, false, true, mm);
	g_pelvis_bones.setHounsfieldValue(BONES_HU);
	
	g_pelvis_bowels.setFilename(g_pelvis_bowels_filename);
	g_pelvis_bowels.loadSTLFile(false, true, false, true, mm);
	g_pelvis_bowels.setHounsfieldValue(BOWEL_HU);
	
	g_pelvis_muscle.setFilename(g_pelvis_muscle_filename);
	g_pelvis_muscle.loadSTLFile(false, true, false, true, mm);
	g_pelvis_muscle.setHounsfieldValue(MUSCLE_HU);
	
	g_pelvis_fat.setFilename(g_pelvis_fat_filename);
	g_pelvis_fat.loadSTLFile(false, true, false, true, mm);
	g_pelvis_fat.setHounsfieldValue(FAT_HU);

    VEC3 translation_vector(-(g_pelvis_skin.getMinCorner() + g_pelvis_skin.getMaxCorner()) / 2.0);
    g_pelvis_skin.translate(translation_vector);
    g_pelvis_bones.translate(translation_vector);
    g_pelvis_bowels.translate(translation_vector);
    g_pelvis_muscle.translate(translation_vector);
    g_pelvis_fat.translate(translation_vector);

    g_xray_renderer.AddOuterSurface(&g_pelvis_skin);
	g_xray_renderer.AddInnerSurface(&g_pelvis_bones);
	g_xray_renderer.AddInnerSurface(&g_pelvis_bowels);
	g_xray_renderer.AddInnerSurface(&g_pelvis_muscle);
	g_xray_renderer.AddInnerSurface(&g_pelvis_fat);

	//g_scene_rotation_x =  -90.0;
	//g_scene_rotation_y =   60.0;
}


void loadDragon()
{
	const char* g_dragon_filename(PREFIX"/data/dragon_vrip_res2.ply_binary.stl");
	//const char* g_dragon_filename(PREFIX"/data/dragon_vrip.ply_binary.stl");

	g_xray_source_position = VEC3(0, 0, -15*cm);
	g_detector_position = VEC3(   0, 0,  15*cm);
	g_up_vector = VEC3(0, 1, 0);

	g_incident_photon_number = 1;
	g_adjust_shift_scale = true;

	g_projection_z_near = 0.01;
	g_projection_z_far = 5000;

	g_resolution_in_centimetres_per_pixel = 1.3;
	g_size_in_pixels = Vec2ui(256, 256);
	g_zoom = GLdouble(80 * cm);

	// Set detector
	initDetector();

	// Set geometry
	g_dragon.setFilename(g_dragon_filename);
	g_dragon.loadSTLFile(true, true, false, true, m);

	g_dragon.setHounsfieldValue(BONES_HU);
	g_xray_renderer.AddInnerSurface(&g_dragon);

	//g_scene_rotation_x =   0.0;
	//g_scene_rotation_y = 130.0;
}

void loadCubes()
{
	const char* g_cubes_filename(PREFIX"/data/cubes_bin.stl");

	g_xray_source_position = VEC3(0, 0, -7.5*cm);
	g_detector_position = VEC3(   0, 0,  7.5*cm);
	g_up_vector = VEC3(0, -1, 0);

	g_incident_photon_number = 1;
	g_shift = -0.06;
	g_scale = 1.0;

	g_projection_z_near = 0.01;
	g_projection_z_far = 5000;

	g_resolution_in_centimetres_per_pixel = 0.6;
	g_size_in_pixels = Vec2ui(256, 256);
	g_zoom = 30 * cm;

	// Set detector
	initDetector();

	// Set geometry
	g_cubes.setFilename(g_cubes_filename);
	g_cubes.loadSTLFile(true, true, false, true, cm);

	g_cubes.setHounsfieldValue(IRON_HU);
	g_xray_renderer.AddInnerSurface(&g_cubes);

	//g_scene_rotation_x =   0.0;
	//g_scene_rotation_y = 130.0;
}

void loadCubeAndCylinder()
{
	const char* g_cube_filename(PREFIX"/data/cube_bin.stl");
	const char* g_cylinder_filename(PREFIX"/data/cylinder_bin.stl");

	g_xray_source_position = VEC3(0, 0, -10*cm);
	g_detector_position = VEC3(   0, 0,  10*cm);
	g_up_vector = VEC3(0, -1, 0);

	g_incident_photon_number = 1000;
	g_shift = -0.045;
	g_scale = 0.001;

	g_projection_z_near = 0.01;
	g_projection_z_far = 5000;

	g_resolution_in_centimetres_per_pixel = 0.6;
	g_size_in_pixels = Vec2ui(256, 256);
	g_zoom = 30 * cm;

	// Set detector
	initDetector();

	g_cube.setFilename(g_cube_filename);
	g_cube.loadSTLFile(false, true, false, true, cm);

	g_cube.setHounsfieldValue(WATER_HU);

	g_cylinder.setFilename(g_cylinder_filename);
	g_cylinder.loadSTLFile(false, true, false, true, cm);

	g_cylinder.setHounsfieldValue(IRON_HU);

    VEC3 translation_vector(-(g_cube.getMinCorner() + g_cube.getMaxCorner()) / 2.0);
    g_cube.translate(translation_vector);
    g_cylinder.translate(translation_vector);

    g_xray_renderer.AddOuterSurface(&g_cube);
	g_xray_renderer.AddInnerSurface(&g_cylinder);
	
	//g_scene_rotation_x =   0.0;
	//g_scene_rotation_y = 130.0;
}


//---------------
void loadInside()
//---------------
{
	const char* g_inside_bones_filename(PREFIX"/data/inside/bones.stl");
	const char* g_inside_diaphragm_filename(PREFIX"/data/inside/diaphragm.stl");
	const char* g_inside_ligaments_filename(PREFIX"/data/inside/ligaments.stl");
	const char* g_inside_liver_filename(PREFIX"/data/inside/liver.stl");
	const char* g_inside_lungs_filename(PREFIX"/data/inside/lungs.stl");
	const char* g_inside_skin_filename(PREFIX"/data/inside/skin.stl");

	g_xray_source_position = VEC3(0*mm, 0*mm, -100*cm);
	g_detector_position = VEC3(   0*mm, 0*mm,  100*cm);
	g_up_vector = VEC3(0, 1, 0);

	g_incident_photon_number = 10;
	g_shift = -0.18;
	g_scale =  0.11;

	g_projection_z_near = 0.01;
	g_projection_z_far = 5000;

	g_resolution_in_centimetres_per_pixel = 2.0 * 1.;
	g_size_in_pixels = Vec2ui(300, 300);
	g_zoom = GLdouble(200 * cm);

	// Set detector
	initDetector();

	// Set geometry
	g_inside_bones.setFilename(g_inside_bones_filename);
	g_inside_bones.loadSTLFile(false, true, true, true, cm);
	g_inside_bones.setHounsfieldValue(BONES_HU);

	g_inside_diaphragm.setFilename(g_inside_diaphragm_filename);
	g_inside_diaphragm.loadSTLFile(false, true, true, true, cm);
	g_inside_diaphragm.setHounsfieldValue(DIAPHRAGM_HU);

	g_inside_ligaments.setFilename(g_inside_ligaments_filename);
	g_inside_ligaments.loadSTLFile(false, true, true, true, cm);
	g_inside_diaphragm.setHounsfieldValue(LIGAMENT_HU);

	g_inside_liver.setFilename(g_inside_liver_filename);
	g_inside_liver.loadSTLFile(false, true, true, true, cm);
	g_inside_liver.setHounsfieldValue(LIVER_HU);

	g_inside_lungs.setFilename(g_inside_lungs_filename);
	g_inside_lungs.loadSTLFile(false, true, true, true, cm);
	g_inside_lungs.setHounsfieldValue(LUNG_HU);

	g_inside_skin.setFilename(g_inside_skin_filename);
	g_inside_skin.loadSTLFile(false, true, true, true, cm);
	g_inside_skin.setHounsfieldValue(SKIN_HU);

    VEC3 translation_vector(-(g_inside_skin.getMinCorner() + g_cube.getMaxCorner()) / 2.0);
    g_inside_skin.translate(translation_vector);
    g_inside_lungs.translate(translation_vector);
    g_inside_liver.translate(translation_vector);
    g_inside_ligaments.translate(translation_vector);
    g_inside_diaphragm.translate(translation_vector);
    g_inside_bones.translate(translation_vector);
    
	g_xray_renderer.AddOuterSurface(&g_inside_skin);
	g_xray_renderer.AddInnerSurface(&g_inside_lungs);
	g_xray_renderer.AddInnerSurface(&g_inside_liver);
	g_xray_renderer.AddInnerSurface(&g_inside_ligaments);
	g_xray_renderer.AddInnerSurface(&g_inside_diaphragm);
	g_xray_renderer.AddInnerSurface(&g_inside_bones);

//	g_scene_rotation_x =  -90.0;
//	g_scene_rotation_y =   60.0;
}


#ifdef HAS_FREEIMAGE
void Screenshot()
{
			std::string screen_shot_file_name(GetFileName(g_screenshot_prefix, g_screenshot_counter, 6, ".raw"));
			g_xray_renderer.LoadAttenuationToRAM();
			float* p_attenuation(g_xray_renderer.GetAttenuation());
			FILE* p_file(fopen(screen_shot_file_name.c_str(), "wb"));
			fwrite(p_attenuation, sizeof(float) * g_size_in_pixels.getX(), g_size_in_pixels.getY(), p_file);
			fclose(p_file);
	
			GLubyte* p_data(new GLubyte[g_main_window_width * g_main_window_height * 4]);

			glReadPixels(0, 0, g_main_window_width, g_main_window_height, GL_RGBA, GL_UNSIGNED_BYTE, p_data);

			int bpp(4 * sizeof(GLubyte) * 8);
			int pitch = ((((bpp * g_main_window_width) + 31) / 32) * 4);
			FIBITMAP* bitmap = FreeImage_ConvertFromRawBits(p_data, g_main_window_width, g_main_window_height, pitch, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, false);

			// save as a regular file
			screen_shot_file_name = GetFileName(g_screenshot_prefix, g_screenshot_counter++, 6, ".png");
#if (FREEIMAGE_MAJOR_VERSION == 3) && (FREEIMAGE_MINOR_VERSION >= 11)
			if (!FreeImage_Save(FIF_PNG, bitmap, screen_shot_file_name.c_str(), PNG_Z_BEST_COMPRESSION))
#else
			if (!FreeImage_Save(FIF_PNG, bitmap, screen_shot_file_name.c_str(), PNG_DEFAULT))
#endif
			{
				std::cerr << "WARNING: could not write \"" << screen_shot_file_name << "\"" << std::endl;
			}

		//	if (!FreeImage_Save(FIF_PNG | PNG_Z_BEST_COMPRESSION, bitmap, screen_shot_file_name, 0))
			FreeImage_Unload(bitmap);

			delete [] p_data;
}
#endif // HAS_FREEIMAGE


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

			float angle(g_rotation_speed * radian2degree(acos(min(1.0, va.dotProduct(vb)))));

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
