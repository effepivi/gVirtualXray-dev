/**
********************************************************************************
*
*	@file		OpenGLWindow.cxx
*
*	@brief		Class to handle the OpenGL rendering in FLTK.
*
*	@version	1.0
*
*	@date		26/02/2014
*
*	@author		Steven Hampson
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
#if (defined(_WIN32) || defined(_WIN64))
#define _USE_MATH_DEFINES
#endif

#include <algorithm>
#include <cmath>

#include <GL/glew.h>

#include <FL/gl.h>
#include <FL/Fl.H>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else // __APPLE__
#include <GL/glu.h>
#endif // __APPLE__

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem.hpp>

#ifndef OPENGL_WINDOW_H
#include "OpenGLWindow.h"
#endif

#ifndef UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#ifdef min
#undef min
#endif

#include "display_gui.frag.h"
#include "display_gui.vert.h"

#include "gui.h"


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//	Constant variables
//******************************************************************************
const GLfloat g_incident_energy(80.0 * keV);
const VEC2 g_detector_size(320.0 * mm, 320.0 * mm);
const Vec2ui g_number_of_pixels(640, 640);
const GLfloat g_resolution(g_detector_size.getX() / g_number_of_pixels.getX());

const VEC3 g_source_position(   0.0, 0.0, -40.0 * cm);
const VEC3 g_detector_position( 0.0, 0.0,  10.0 * cm);
const VEC3 g_detector_up_vector(0.0, 1.0,   0.0);

const double g_initial_intraocular_distance(1.5 * cm);        // Intraocular distance
const double g_initial_near(5.0 * cm);                        // Near clipping plane
const double g_initial_far(500.0 * cm);                       // Far clipping plane
const double g_initial_fovy(45);                              // Field of view along the y-axis
const double g_initial_screen_projection_plane(10000.0 * cm); // Screen projection plane

const VEC3 g_rotation_axis(0, 0, 1);
const GLfloat g_rotation_speed(2.0);

//******************************************************************************
//	Global variables
//******************************************************************************
Matrix4x4<GLfloat> g_detector_rotation_matrix;


void OpenGLWindow::setExecutable(char *argv)
{
    m_executable_path.clear();

    namespace fs = boost::filesystem;
    fs::path p(argv);
    fs::path full_p = fs::complete(p);
    if (full_p.has_parent_path())
    {
        m_executable_path = full_p.parent_path().string();
    }

    std::cout << "Executable path:\t" << m_executable_path << std::endl;
}


//-----------------------------
void idleCallback(void* apData)
//-----------------------------
{
   if (apData)
   {
      OpenGLWindow* p_window(reinterpret_cast<OpenGLWindow*>(apData));
      
      if (p_window->getAnimationFlag())
      {
         p_window->setObjectRotationAngle(p_window->getRotationIncrement() / 100.0);
      }
      p_window->redraw();
   }
}


//-------------------------------------------------------------------------------
OpenGLWindow::OpenGLWindow(int X, int Y, int aWidth, int aHeight, char* apTitle):
//-------------------------------------------------------------------------------
    Fl_Gl_Window(X, Y, aWidth, aHeight, apTitle),
    m_scene_rotation_angle(0),
    m_object_rotation_angle(0),
    m_rotation_increment(10),
    m_animating(false),
    m_first_time(true),
    m_display_wireframe(false),
    m_display_beam(true),
    m_display_detector(true),
    m_is_xray_image_up_to_date(false),
    m_zoom(115.0 * cm),
    m_use_arc_ball(false),
    m_rotation_speed(2)
//-------------------------------------------------------------------------------
{
	if (can_do(FL_RGB | FL_ALPHA | FL_DEPTH | FL_DOUBLE | FL_STEREO))
	{
		mode(FL_RGB | FL_ALPHA | FL_DEPTH | FL_DOUBLE | FL_STEREO);
	}
	else
	{
		mode(FL_RGB | FL_ALPHA | FL_DEPTH | FL_DOUBLE);
	}

	Fl::add_idle(idleCallback, this);
}


//-------------------------------
void OpenGLWindow::initializeGL()
//-------------------------------
{
	try
	{
		GLenum glew_error_1(glewInit());
		if (glew_error_1 != GLEW_OK)
		{
			std::stringstream error_message;
			error_message << "Cannot initialize GLEW: " << glewGetErrorString(glew_error_1);
			throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
		}

		glClearColor(.1f, .1f, .1f, 1);
		glEnable(GL_DEPTH_TEST);

		// Choose smooth shading
		glShadeModel(GL_SMOOTH);

		// Use colour and texture coordinate interpolation
		//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		// Use antialiased polygons
		//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

		// Use antialiasing
		//glEnable(GL_POLYGON_SMOOTH);

		//glEnable(GL_MULTISAMPLE);
		//glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

		// Add the geometry to the X-ray renderer
        std::string prefix(m_executable_path);
        if (prefix.length())
        {
            prefix += "/";
        }
        prefix += "..";

		std::string stl_filename(prefix);
        stl_filename += "/examples/data/welsh-dragon-small.stl";

		loadSTLFile(stl_filename);

        loadDetector();
		loadSource();
		loadXRaySimulator();
		loadShader();

		m_xray_renderer.addInnerSurface(&m_polygon_data);

		// Rotate the sample
		m_sample_rotation_matrix.rotate( 180, VEC3(0, 1, 0));
		m_sample_rotation_matrix.rotate( 90, VEC3(0, 0, 1));
		m_sample_rotation_matrix.rotate( 90, VEC3(0, 1, 0));

		// Rotate the scene
		m_scene_rotation_matrix.rotate(-110, VEC3(0, 1, 0));

		// Enable the stereo if possible
		// Set the stereo parameters
		m_stereo_helper.setIntraocularDistance(g_initial_intraocular_distance);
		g_p_intraocular_distance_slider->value(g_initial_intraocular_distance);
		g_p_intraocular_distance_slider->range(0,
				g_initial_intraocular_distance + g_initial_intraocular_distance / 1.5);
		m_stereo_helper.setFieldOfViewY(g_initial_fovy);
		m_stereo_helper.setNear(g_initial_near);
		m_stereo_helper.setFar(g_initial_far);
		m_stereo_helper.setScreenProjectionPlane(g_initial_screen_projection_plane);
		m_stereo_helper.enable();

		// The stereo cannot be activated
		if (!m_stereo_helper.isActive())
		{
			// Disable widgets
			g_p_stereo_button->deactivate();
			g_p_intraocular_distance_slider->deactivate();
		}
		else
		{
			g_p_stereo_button->value(true);
		}
	}
	catch (const std::exception& e)
	{
		const char* p_error_message(e.what());
		fl_alert(p_error_message);
		//exit (EXIT_FAILURE);
	}
}


//------------------------------------------------
void OpenGLWindow::resize(int X,int Y,int W,int H)
//------------------------------------------------
{
  Fl_Gl_Window::resize(X,Y,W,H);
  reshape(W, H);
  redraw();
}


//-----------------------
void OpenGLWindow::draw()
//-----------------------
{
	try
	{
		if (m_first_time)
		{
			initializeGL();
			reshape(w(), h());
			m_first_time = false;
		}

		// Compute the X-ray image
		updateXRayImage();

		// Get the current draw buffer if needed
		GLint back_buffer(0);
		glGetIntegerv(GL_DRAW_BUFFER, &back_buffer);

		if (m_stereo_helper.isActive())
		{
			m_stereo_helper.swapEye();

			// Clear the buffers
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Display the 3D scene
			render();

			// Make sure all the OpenGL code is done
			glFinish();

			m_stereo_helper.swapEye();
		}

		// Clear the buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Display the 3D scene
		render();

		// Make sure all the OpenGL code is done
		glFinish();

		// Restore the draw buffer if needed
		glDrawBuffer(back_buffer);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}


//-----------------------------------
int OpenGLWindow::handle(int anEvent)
//-----------------------------------
{
	switch (anEvent)
	{
	case FL_FOCUS:
	case FL_UNFOCUS:
		return (1);

	case FL_KEYBOARD:
		{
			int key = Fl::event_key();
			switch (key)
			{
			case FL_Left:
				setObjectRotationAngle(-m_rotation_increment);
				redraw();
				return (1);

			case FL_Right:
				setObjectRotationAngle(+m_rotation_increment);
				redraw();
				return (1);

			case ' ':
				m_animating = !m_animating;
				m_is_xray_image_up_to_date = false;
				return (1);
			}
		}
		break;
		

	case FL_PUSH:

		m_use_arc_ball = true;

        m_last_x_position = m_current_x_position = Fl::event_x();

        if ( Fl::event_button() == FL_LEFT_MOUSE)
		{
	        m_last_y_position = m_current_y_position = Fl::event_y();
		}
		else
		{
	        m_last_y_position = m_current_y_position = h() - Fl::event_y();
		}
		return (1);

	case FL_RELEASE:
		m_use_arc_ball = false;
		return (1);

    case FL_DRAG:
        if ( Fl::event_button() == FL_LEFT_MOUSE)
        {
            m_current_x_position = Fl::event_x();
            m_current_y_position = Fl::event_y();
            // left click rotates environment
            computeRotation(m_scene_rotation_matrix);
            m_is_xray_image_up_to_date = false;
            redraw();
            return (1);
        }

        if ( Fl::event_button() == FL_RIGHT_MOUSE)
        {
            m_current_x_position = Fl::event_x();
            m_current_y_position = h() - Fl::event_y();
            // right click rotates object
            computeRotation(m_sample_rotation_matrix);
            m_is_xray_image_up_to_date = false;
            redraw();
            return (1);
        }

        if ( Fl::event_button() == FL_MIDDLE_MOUSE)
        {
            std::cout << "MIDDLE BUTTON." << std::endl;
            m_current_x_position = Fl::event_x();
            m_current_y_position = h() - Fl::event_y();
            // middle click rotates x-ray detector
            computeRotation(m_detector_rotation_matrix);
            m_xray_detector.setRotationMatrix(m_detector_rotation_matrix);
            m_is_xray_image_up_to_date = false;
            redraw();
            return (1);
        }
        break;

	case FL_MOUSEWHEEL:
		m_zoom += float(Fl::event_dy()) * cm;
		if (m_zoom > g_zoom_slider->maximum()) m_zoom = g_zoom_slider->maximum();
		if (m_zoom < g_zoom_slider->minimum()) m_zoom = g_zoom_slider->minimum();
		g_zoom_slider->value(m_zoom);
		reshape(w(), h());
		return (1);
    }


	return (Fl_Gl_Window::handle(anEvent));
}
   
//-------------------------
void OpenGLWindow::render()
//-------------------------
{
	try
	{
		// Enable the shader
		pushShaderProgram();
		m_display_shader.enable();
		GLint shader_id(m_display_shader.getProgramHandle());

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

		// Store the current attributes
		pushEnableDisableState(GL_LIGHT0);
		pushEnableDisableState(GL_LIGHTING);

		// Store the current transformation matrix
		pushModelViewMatrix();

		// Rotate the sample
		g_current_modelview_matrix *= m_scene_rotation_matrix;

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

		GLint lighting(1);
		if (!m_display_wireframe)
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

		g_current_modelview_matrix *= m_sample_rotation_matrix;
		applyModelViewMatrix();

		if (m_display_wireframe)
		{
			m_polygon_data.displayWireFrame();
		}
		else
		{
			m_polygon_data.display();
		}

		// Restore the current transformation matrix
		popModelViewMatrix();

		// Display the source
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   p_ambiant_silver_material_1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   p_diffuse_silver_material_1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  p_specular_silver_material_1);
		glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, shininess_silver_material);
		
		lighting = 1;
		handle = glGetUniformLocation(shader_id,"g_use_lighting");
		glUniform1iv(handle, 1, &lighting);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		
		m_xray_detector.displaySource();

		// Display the X-Ray image
		if (m_display_detector)
		{
			m_xray_renderer.display();
		}

		// Display the beam
		if (m_display_beam)
		{
			glColor4f(0.75, 0, 0.5, 0.3);

			lighting = 0;
			handle = glGetUniformLocation(shader_id,"g_use_lighting");
			glUniform1iv(handle, 1, &lighting);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			m_xray_detector.displayBeam();

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


//----------------------------------------------------------
void OpenGLWindow::loadSTLFile(const std::string& aFileName)
//----------------------------------------------------------
{
	// Set geometry
    bool move_to_centre(true);
    bool auto_compute_normal(true);
    bool print_debug_info(false);
    bool use_VBO(true);

	m_polygon_data.setFilename(aFileName.data());
	m_polygon_data.loadSTLFile(move_to_centre, auto_compute_normal, print_debug_info, use_VBO, mm, GL_STATIC_DRAW);
	m_polygon_data.mergeVertices();
	m_polygon_data.setHounsfieldValue(500.0);

	// The X-ray image is not up-to-date
	m_is_xray_image_up_to_date = false;
}


//-------------------------------
void OpenGLWindow::loadDetector()
//-------------------------------
{
	m_xray_detector.setResolutionInUnitsOfLengthPerPixel(g_resolution);
	m_xray_detector.setNumberOfPixels(g_number_of_pixels);
	m_xray_detector.setDetectorPosition(g_detector_position);
	m_xray_detector.setUpVector(g_detector_up_vector);

	// The X-ray image is not up-to-date
	m_is_xray_image_up_to_date = false;
}


//-----------------------------
void OpenGLWindow::loadSource()
//-----------------------------
{
	// Set the energy
	m_xray_beam.initialise(g_incident_energy);

	// Set the source position
	m_xray_detector.setXrayPointSource(g_source_position);
	//m_xray_detector.setParallelBeam();
	m_xray_detector.setPointSource();

	// The X-ray image is not up-to-date
	m_is_xray_image_up_to_date = false;

    switch (m_xray_detector.getSourceShape())
    {
        case XRayDetector::POINT:
            g_line_source_widget->value(0);
            g_cube_source_widget->value(0);
            g_point_source_widget->value(1);
            g_parallel_source_widget->value(0);
            g_square_source_widget->value(0);
            break;

        case XRayDetector::SQUARE:
            g_line_source_widget->value(0);
            g_cube_source_widget->value(0);
            g_point_source_widget->value(0);
            g_parallel_source_widget->value(0);
            g_square_source_widget->value(1);
            break;

        case XRayDetector::CUBE:
            g_line_source_widget->value(0);
            g_cube_source_widget->value(1);
            g_point_source_widget->value(0);
            g_parallel_source_widget->value(0);
            g_square_source_widget->value(0);
            break;

        case XRayDetector::LINE:
            g_line_source_widget->value(1);
            g_cube_source_widget->value(0);
            g_point_source_widget->value(0);
            g_parallel_source_widget->value(0);
            g_square_source_widget->value(0);
            break;

        case XRayDetector::PARALLEL:
            g_line_source_widget->value(0);
            g_cube_source_widget->value(0);
            g_point_source_widget->value(0);
            g_parallel_source_widget->value(1);
            g_square_source_widget->value(0);
            break;
    }
}


//------------------------------------
void OpenGLWindow::loadXRaySimulator()
//------------------------------------
{
	// Initialise the X-ray renderer
	m_xray_renderer.initialise(XRayRenderer::OPENGL,
		GL_RGB16F_ARB,
		&m_xray_detector,
		&m_xray_beam);

	m_xray_renderer.useNegativeFilteringFlag(!m_xray_renderer.getNegativeFilteringFlag());

	// The X-ray image is not up-to-date
	m_is_xray_image_up_to_date = false;
}


//-----------------------------
void OpenGLWindow::loadShader()
//-----------------------------
{
	// Initialise the shaders
	char* p_vertex_shader(0);
	char* p_fragment_shader(0);

	int z_lib_return_code_vertex(0);
	int z_lib_return_code_fragment(0);

	std::string vertex_shader;
	std::string fragment_shader;

	// L-buffer
	z_lib_return_code_vertex   = inflate(g_display_gui_vert, sizeof(g_display_gui_vert),   &p_vertex_shader);
	z_lib_return_code_fragment = inflate(g_display_gui_frag, sizeof(g_display_gui_frag), &p_fragment_shader);

	vertex_shader   = p_vertex_shader;
	fragment_shader = p_fragment_shader;
	delete [] p_vertex_shader;     p_vertex_shader = 0;
	delete [] p_fragment_shader; p_fragment_shader = 0;

	if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
	}
	m_display_shader.loadSource(vertex_shader, fragment_shader);
}


//------------------------------------------------
void OpenGLWindow::reshape(int aWidth,int aHeight)
//------------------------------------------------
{
	int x(0), y(0), width(aWidth), height(aHeight);

	if (height == 0)
	{
		// Prevent divide by 0
		height = 1;
	}

	double screen_aspect_ratio(double(width) / double(height));

	glViewport(x, y, width, height);

	loadPerspectiveProjectionMatrix(g_initial_fovy, screen_aspect_ratio, g_initial_near, g_initial_far);

	loadLookAtModelViewMatrix(0.0, 0.0, m_zoom,
			0.0, 0.0, 0.0,
			0.0, 1.0, 0.0);
}


//----------------------------------
void OpenGLWindow::updateXRayImage()
//----------------------------------
{
	// The X-ray image is not up-to-date
	if (!m_is_xray_image_up_to_date)
	{
		// Compute the X-Ray image
		m_xray_renderer.computeImage(m_sample_rotation_matrix);

		// Normalise the X-ray image
		m_xray_renderer.normalise();

		// The X-ray image is up-to-date
		m_is_xray_image_up_to_date = true;
	}
}

//------------------------------------------------
void OpenGLWindow::loadObject(double aValue)
//------------------------------------------------
{
	
}

//---------------------------------------------
void OpenGLWindow::setPointSource(double aValue)
//---------------------------------------------
{
	m_xray_detector.setPointSource();
	m_is_xray_image_up_to_date = false;
	redraw();
}

//------------------------------------------------
void OpenGLWindow::setSourceParallel(double aValue)
//------------------------------------------------
{
	if (m_xray_detector.getSourceShape() != XRayDetector::PARALLEL)
		{
			// Use a parallel source
			m_xray_detector.setParallelBeam();
		}
		// This is a cubic source
		else
		{
			m_xray_detector.setPointSource();
		}


	m_is_xray_image_up_to_date = false;
	redraw();
}

//--------------------------------------------
void OpenGLWindow::setSourceLine(double aValue)
//--------------------------------------------
{
	if (m_xray_detector.getSourceShape() != XRayDetector::LINE)
		{
			m_xray_detector.setLineSource(m_xray_detector.getXraySourceCentre(), VEC3(1, 0, 0), 16, 10.0*mm);
		}
		// This is a Line source
		else
		{
			m_xray_detector.setPointSource();
		}

	m_is_xray_image_up_to_date = false;
	redraw();
}

//----------------------------------------------
void OpenGLWindow::setSourceSquare(double aValue)
//----------------------------------------------
{
	if (m_xray_detector.getSourceShape() != XRayDetector::SQUARE)
		{
			m_xray_detector.setSquareSource(m_xray_detector.getXraySourceCentre(), 10, 10.0*mm);
		}
		// This is a square source
		else
		{
			m_xray_detector.setPointSource();
		}
		// The X-ray image is not up-to-date

	m_is_xray_image_up_to_date = false;
	redraw();
}

//--------------------------------------------
void OpenGLWindow::setSourceCube(double aValue)
//--------------------------------------------
{
	if (m_xray_detector.getSourceShape() != XRayDetector::CUBE)
		{
			m_xray_detector.setCubicSource(m_xray_detector.getXraySourceCentre(), 5, 10.0*mm);
		}
		// This is a cubic source
		else
		{
			m_xray_detector.setPointSource();
		}

		// The X-ray image is not up-to-date
		m_is_xray_image_up_to_date = false;
	redraw();
}


//---------------------------------------
void OpenGLWindow::setStereo(bool aValue)
//---------------------------------------
{
	if (aValue)
	{
		m_stereo_helper.enable();
	}
	else
	{
		m_stereo_helper.disable();
	}
	redraw();
}

//------------------------------------------------------
void OpenGLWindow::setObjectRotationAngle(float anAngle)
//------------------------------------------------------
{
    // Update the transformation matrix
	m_sample_rotation_matrix.rotate(anAngle - m_object_rotation_angle, g_rotation_axis);

	// Update the angle
	m_object_rotation_angle = anAngle;

    m_is_xray_image_up_to_date = false;
}

//------------------------------------------------
void OpenGLWindow::setNegative(int aValue)
//------------------------------------------------
{
	// renders the xray in negative
	m_xray_renderer.useNegativeFilteringFlag(!m_xray_renderer.getNegativeFilteringFlag());
	// The X-ray image is not up-to-date
	m_is_xray_image_up_to_date = false;
	redraw();
}

//--------------------------------------------
void OpenGLWindow::setWireframe(double aValue)
//--------------------------------------------
{
	// The X-ray image is not up-to-date
	// wireframe currently doesn't turn off
	m_display_wireframe = !m_display_wireframe;
	m_is_xray_image_up_to_date = false;
	redraw();
}

//--------------------------------------
void OpenGLWindow::setBeam(double aValue)
//--------------------------------------
{

	m_display_beam = !m_display_beam;
	m_is_xray_image_up_to_date = false;
	redraw();

}

//---------------------------------------------------
void OpenGLWindow::rotateObject(const double anAngle)
//---------------------------------------------------
{
	setObjectRotationAngle(anAngle);
    redraw();
}

//-----------------------------------------------------
void OpenGLWindow::rotateDetector(const double anAngle)
//-----------------------------------------------------
{

    m_is_xray_image_up_to_date = false;
	m_detector_rotation_matrix.loadIdentity();
	m_detector_rotation_matrix.rotate(anAngle, m_display_detector);
	m_xray_detector.setRotationMatrix(m_detector_rotation_matrix);
	redraw();
}

//--------------------------------------------------------
void OpenGLWindow::rotateEnvironment(const double anAngle)
//--------------------------------------------------------
{
    m_is_xray_image_up_to_date = false;
	m_scene_rotation_matrix.rotate(anAngle - m_scene_rotation_angle, g_detector_up_vector);
	redraw();
	
	// Update the angle
    m_scene_rotation_angle = anAngle;
}


//------------------------------------
void OpenGLWindow::setZoom(int aValue)
//------------------------------------
{
	m_zoom = aValue;
	loadLookAtModelViewMatrix(0.0, 0.0, m_zoom,
			0.0, 0.0, 0.0,
			0.0, 1.0, 0.0);

	redraw();
}


//-----------------------------------------------------
void OpenGLWindow::setIntraocularDistance(float aValue)
//-----------------------------------------------------
{
	m_stereo_helper.setIntraocularDistance(aValue);

	redraw();
}

//-----------------------------------------------
VEC3 OpenGLWindow::getArcballVector(int x, int y)
//-----------------------------------------------
{
    VEC3 P(2.0 * float(x) / float(w()) - 1.0,
            2.0 * float(y) / float(h()) - 1.0,
            0);

    P.setY(-P.getY());
	
    float OP_squared = P.getX() * P.getX() + P.getY() * P.getY();
    if (OP_squared <= 1.0)
    {
        P.setZ(sqrt(1.0 - OP_squared));  // Pythagore
    }
    else
    {
        P.normalize();  // nearest point
    }

    return (P);
}

//---------------------------------------
inline float radian2degree(float anAngle)
//---------------------------------------
{
    return (180.0 * anAngle / M_PI);
}

//----------------------------------------------------------
void OpenGLWindow::computeRotation(MATRIX4& aRotationMatrix)
//----------------------------------------------------------
{
    if (m_use_arc_ball)
    {
        if (m_current_x_position != m_last_x_position || m_current_y_position != m_last_y_position)
        {
            VEC3 va(getArcballVector(m_last_x_position,    m_last_y_position));
            VEC3 vb(getArcballVector(m_current_x_position, m_current_y_position));

#if (defined(_WIN32) || defined(_WIN64)) && !defined(__GNUC__)
            float angle(m_rotation_speed * radian2degree(acos(std::min(1.0, va.dotProduct(vb)))));
#else
            float angle(m_rotation_speed * radian2degree(acos(std::min(1.0, va.dotProduct(vb)))));
#endif

            VEC3 axis_in_camera_coord(va ^ vb);
            //axis_in_camera_coord.normalize();

            MATRIX4 camera2object(aRotationMatrix.getInverse());
            Vec3<GLfloat> axis_in_object_coord = camera2object * axis_in_camera_coord;
            //axis_in_object_coord.normalize();

            MATRIX4 rotation_matrix;
            rotation_matrix.rotate(angle, axis_in_object_coord);
            aRotationMatrix = aRotationMatrix * rotation_matrix;

            m_last_x_position = m_current_x_position;
            m_last_y_position = m_current_y_position;
        }
    }
}

