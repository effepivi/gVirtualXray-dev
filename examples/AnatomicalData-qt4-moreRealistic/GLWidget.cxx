/*

Copyright (c) 2016, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*	@file		GLWidget.cxx
*
*	@brief      Qt4 widget to handle our OpenGL window.
*
*	@version	1.0
*
*	@date		13/07/2016
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

#include <QtGui>
#include <QGLWidget>

#include <algorithm>


#ifndef GVXR_GL_WIDGET_H
#include "GLWidget.h"
#endif

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#ifndef GVXR_CUBE_MESH_H
#include "gVirtualXRay/CubeMesh.h"
#endif

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

#ifndef ANATOMICAL_MESH_H
#include "AnatomicalMesh.h"
#endif


#include "BoneMesh.h"

#include "DiaphragmMesh.h"

#include "LiverMesh.h"

#include "LungsMesh.h"

#include "SkinMesh.h"

#include "display_gl2.frag.h"
#include "display_gl2.vert.h"

#include "display_realistic_gl3.frag.h"
#include "display_realistic_gl3.vert.h"

#include "depth_map_gl3.frag.h"
#include "depth_map_gl3.vert.h"

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


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//	Constant variables
//******************************************************************************
const GLsizei g_default_window_width( 600);
const GLsizei g_default_window_height(600);

const GLfloat g_rotation_speed(2.0);

const float g_scaling_factor(2);
const VEC2 g_resolution(0.7 * g_scaling_factor * mm, 1 * g_scaling_factor * mm);
const Vec2ui g_number_of_pixels(500 / g_scaling_factor, 225 / g_scaling_factor);

const float g_incident_energy(0.08 * MeV);

const double g_initial_fovy(45);                              // Field of view along the y-axis
const double g_initial_near(5.0 * cm);                        // Near clipping plane
const double g_initial_far(500.0 * cm);                       // Far clipping plane

const VEC3 g_initial_source_position(   0.0,  25.0 * cm, 0.0);
const VEC3 g_initial_detector_position( 0.0, -25.0 * cm, 0.0);
const VEC3 g_initial_detector_up_vector(0.0, 0.0, 1.0);
const VEC3 g_background_colour(0.5, 0.5, 0.5);

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


//******************************************************************************
//  Function declaration
//******************************************************************************
float radian2degree(float anAngle);


//******************************************************************************
//	Implementation
//******************************************************************************

//---------------------------------------
inline float radian2degree(float anAngle)
//---------------------------------------
{
    return (180.0 * anAngle / gVirtualXRay::PI);
}


//----------------------------------
GLWidget::GLWidget(QWidget *parent):
//----------------------------------
    QGLWidget(parent),


    m_zoom(100.0 * cm),

    m_use_arc_ball(false),
    m_last_x_position(0),
    m_last_y_position(0),

    m_display_beam(true),

    m_p_P4_polygon_mesh_set(),
    m_p_P5_polygon_mesh_set(),
    m_current_patient(P4),

    m_display_wireframe(false),

    m_angle_step(1),
    m_next_angle_to_process(0),
    m_current_angle(0),
    m_simulation_total_time(0),
    m_number_of_rotations(0),
    m_acquisition_type(ROTATE_SOURCE_DETECTOR),

	m_shadowMap(),

	#undef HAS_FREETYPE

    #ifdef HAS_FREETYPE
    m_display_help(true),
    #endif

    m_swap_patient(false),

	m_light_direction(-1, 3, 1)

//----------------------------------
{
	//Initialize the polygon meshes
	initializePatient(m_p_P4_polygon_mesh_set);
	initializePatient(m_p_P5_polygon_mesh_set);

    // Initialize Qt
    // and create a windowed mode window and its OpenGL context

    resize(g_default_window_width, g_default_window_height);
    show();
    setWindowTitle(QApplication::translate("toplevel", "gVirtualXRay -- Anatomical Data Demo -- Qt4"));

    // Make the window's context current
    makeCurrent();
}


//-------------------
GLWidget::~GLWidget()
//-------------------
{}


//------------------------------
QSize GLWidget::sizeHint() const
//------------------------------
{
    return QSize(g_default_window_width, g_default_window_height);
}


//---------------------------
void GLWidget::initializeGL()
//---------------------------
{
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

    // Initialise FreeType
    initFreeType();

    // Load the data
    loadDetector();
    loadSource();
    loadXRaySimulator();

    // Add the geometry to the X-ray renderer
    loadSTLFiles();

    //initialize the buffers
    m_shadowMap.initialize();

    //set the dirextion of the light
    m_light_direction.normalize();

    // Set the patient
    usePatient(m_current_patient);

    // Initialise the modelling matrix
    m_scene_rotation_matrix *= Matrix4x4<GLfloat>::buildRotationMatrix(180.0, VEC3(0, 0, 1));
    m_scene_rotation_matrix *= Matrix4x4<GLfloat>::buildRotationMatrix( 60.0, VEC3(1, 0, 0));
}


//----------------------
void GLWidget::paintGL()
//----------------------
{
    // Clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Display the 3D elements
    glViewport(0, 0, width(), height());

    // Store the current transformation matrix
    pushModelViewMatrix();
    pushProjectionMatrix();

    double screen_aspect_ratio(double(width()) / double(height()));

    loadPerspectiveProjectionMatrix(g_initial_fovy, screen_aspect_ratio, g_initial_near, g_initial_far);

    loadLookAtModelViewMatrix(0.0, 0.0, m_zoom,
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);

    // Rotation
    if (m_current_angle != m_next_angle_to_process)
    {
        switch (m_acquisition_type)
        {
        case ROTATE_SAMPLE:
            {
                Matrix4x4<GLfloat> P4_rotation_matrix;
                P4_rotation_matrix *= Matrix4x4<GLfloat>::buildTranslationMatrix( m_P4_translation_vector);
                P4_rotation_matrix *= Matrix4x4<GLfloat>::buildRotationMatrix(-m_angle_step, g_initial_detector_up_vector);
                P4_rotation_matrix *= Matrix4x4<GLfloat>::buildTranslationMatrix(-m_P4_translation_vector);

                Matrix4x4<GLfloat> P5_rotation_matrix;
                P5_rotation_matrix *= Matrix4x4<GLfloat>::buildTranslationMatrix( m_P5_translation_vector);
                P5_rotation_matrix *= Matrix4x4<GLfloat>::buildRotationMatrix(-m_angle_step, g_initial_detector_up_vector);
                P5_rotation_matrix *= Matrix4x4<GLfloat>::buildTranslationMatrix(-m_P5_translation_vector);

                m_P4_rotation_matrix *= P4_rotation_matrix;
                m_P5_rotation_matrix *= P5_rotation_matrix;
            }
            break;

        case ROTATE_SOURCE_DETECTOR:
            {
                Matrix4x4<GLfloat> rotation_matrix;
                rotation_matrix *= Matrix4x4<GLfloat>::buildRotationMatrix(m_angle_step, g_initial_detector_up_vector);

                VEC3 source_position(  rotation_matrix * m_xray_detector.getXraySourceCentre());
                VEC3 detector_position(rotation_matrix * m_xray_detector.getDetectorPosition());

                // Set the source position
                m_xray_detector.setXrayPointSource(source_position);

                // Use a parallel source
                m_xray_detector.setParallelBeam();

                // Set the detector position
                m_xray_detector.setDetectorPosition(detector_position);
            }
            break;
        }
    }

    // Update the current angle
    m_current_angle = m_next_angle_to_process;
    m_next_angle_to_process += m_angle_step;

    // Stop the acquisition
    if (m_next_angle_to_process > 180.0)
    {
        if (m_swap_patient)
        {
            if (m_current_patient == P4)
            {
                usePatient(P5);
            }
            else
            {
                usePatient(P4);
            }
        }
        else
        {
            reset();
        }

        m_swap_patient = !m_swap_patient;
    }

    // Update the X-ray image
    updateXRayImage();
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Draw the 3D scene
    render();
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Draw the help message
#ifdef HAS_FREETYPE
    if (m_display_help)
    {
        displayHelp();

        // Check the status of OpenGL and of the current FBO
        checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    }
#endif

    // Restore the current transformation matrix
    popModelViewMatrix();
    popProjectionMatrix();

    // Make sure all the OpenGL code is done
    glFinish();

    animate();
}


//----------------------------------------------
void GLWidget::resizeGL(int aWidth, int aHeight)
//----------------------------------------------
{
    // Avoid a division by zero
    if (aHeight == 0)
    {
        // Prevent divide by 0
        aHeight = 1;
    }

    int x(0), y(0), w(aWidth), h(aHeight);

    // Update the viewport
    glViewport(x, y, w, h);

    m_text_2D_projection_matrix = buildOrthoProjectionMatrix(0,
            aWidth,
            aHeight,
            0,
            -1.0,
            1.0);
}


//---------------------------------------------
void GLWidget::keyPressEvent(QKeyEvent * event)
//---------------------------------------------
{
    switch(event->key())
    {
    // Close the program
    case Qt::Key_Q:
    case Qt::Key_Escape:
        close();
        break;

    case Qt::Key_Space:
        reset();
        break;

    case Qt::Key_B:
        m_display_beam = !m_display_beam;
        break;

    case Qt::Key_N:
        m_xray_renderer.useNegativeFilteringFlag(!m_xray_renderer.getNegativeFilteringFlag());
        break;

    case Qt::Key_W:
        m_display_wireframe = !m_display_wireframe;
        break;


#ifdef HAS_FREETYPE
    case Qt::Key_H:
        m_display_help = !m_display_help;
        break;
#endif

    case Qt::Key_P:
        if (m_current_patient == P4) m_current_patient = P5;
        else m_current_patient = P4;
        usePatient(m_current_patient);
        break;

    case Qt::Key_Plus:
        m_zoom += 1.0 * cm;
        resizeGL(width(), height());
        std::cout << "Zoom in cm)" << m_zoom / cm << std::endl;
        break;

    case Qt::Key_Minus:
        m_zoom -= 1.0 * cm;
        resizeGL(width(), height());
        std::cout << "Zoom in cm)" << m_zoom / cm << std::endl;
        break;

    default:
        std::cout << event->key() << std::endl;
        break;
    }
}


//------------------------------------------------
void GLWidget::wheelEvent(QWheelEvent *wheelEvent)
//------------------------------------------------
{
    // Scrolling along the Y-axis
    if (fabs(wheelEvent->delta()) > EPSILON)
    {
        // Stop using the arc ball
        m_use_arc_ball = false;

        // Change the zoom
        m_zoom += 0.1 * wheelEvent->delta() * cm;

        // Update the projection matrix
        resizeGL(width(), height());
    }

}


//-----------------------
void GLWidget::initGLEW()
//-----------------------
{
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::stringstream error_message;
        error_message << "ERROR: cannot initialise GLEW:\t" << glewGetErrorString(err);

        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
    }

    do
    {
    } while (glGetError() != GL_NO_ERROR);
}


//---------------------
void GLWidget::initGL()
//---------------------
{
    std::cout << "OpenGL renderer:   " << glGetString(GL_RENDERER)   << std::endl;
    std::cout << "OpenGL version:    " << glGetString(GL_VERSION)    << std::endl;
    std::cout << "OpenGL vender:     " << glGetString(GL_VENDOR)     << std::endl;

    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Enable the Z-buffer
    glEnable(GL_DEPTH_TEST);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Set the background colour
    glClearColor(g_background_colour.getX(), g_background_colour.getY(), g_background_colour.getZ(), 1.0);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glEnable(GL_MULTISAMPLE);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	//load the shaders
    char* p_vertex_shader(0);
    char* p_fragment_shader(0);

    int z_lib_return_code_vertex(0);
    int z_lib_return_code_fragment(0);

    std::string vertex_shader;
    std::string fragment_shader;

    // Display shader
    z_lib_return_code_vertex   = inflate(g_display_realistic_gl3_vert, sizeof(g_display_realistic_gl3_vert),   &p_vertex_shader);
    z_lib_return_code_fragment = inflate(g_display_realistic_gl3_frag, sizeof(g_display_realistic_gl3_frag), &p_fragment_shader);


    vertex_shader   = p_vertex_shader;
    fragment_shader = p_fragment_shader;
    delete [] p_vertex_shader;     p_vertex_shader = 0;
    delete [] p_fragment_shader; p_fragment_shader = 0;

    if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
    {
    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
    }
    m_display_shader.setLabels("display.vert", "display.frag");
    m_display_shader.loadSource(vertex_shader, fragment_shader);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    //depth shader
    z_lib_return_code_vertex   = inflate(g_depth_map_gl3_vert, sizeof(g_depth_map_gl3_vert),   &p_vertex_shader);
    z_lib_return_code_fragment = inflate(g_depth_map_gl3_frag, sizeof(g_depth_map_gl3_frag), &p_fragment_shader);


    vertex_shader   = p_vertex_shader;
    fragment_shader = p_fragment_shader;
    delete [] p_vertex_shader;     p_vertex_shader = 0;
    delete [] p_fragment_shader; p_fragment_shader = 0;

    if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
    {
    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
    }
    m_depthShader.setLabels("depth.vert", "depth.frag");
    m_depthShader.loadSource(vertex_shader, fragment_shader);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//---------------------------
void GLWidget::initFreeType()
//---------------------------
{
#ifdef HAS_FREETYPE
    m_font_rendered.initialise(PREFIX"/data/arial.ttf", 16*27);
    m_display_help = true;
#endif
}


//---------------------------
void GLWidget::loadDetector()
//---------------------------
{
    m_xray_detector.setResolutionInUnitsOfLengthPerPixel(g_resolution);
    m_xray_detector.setNumberOfPixels(g_number_of_pixels);
    m_xray_detector.setDetectorPosition(g_initial_detector_position);
    m_xray_detector.setUpVector(g_initial_detector_up_vector);
}


//-------------------------
void GLWidget::loadSource()
//-------------------------
{
    // Set the energy
    m_xray_beam.initialise(g_incident_energy);

    // Set the source position
    m_xray_detector.setXrayPointSource(g_initial_source_position);

    // Use a parallel source
    m_xray_detector.setParallelBeam();
}


//--------------------------------
void GLWidget::loadXRaySimulator()
//--------------------------------
{
    // Initialise the X-ray renderer
    m_xray_renderer.initialise(XRayRenderer::OPENGL,
            GL_RGB16F,
            &m_xray_detector,
            &m_xray_beam);
}


//----------------------------------------------
void GLWidget::loadPatient(PatientID aPatientID)
//----------------------------------------------
{
	std::vector<AnatomicalMesh*>* p_mesh_set(0);
	    double const* p_HU_set(0);

	    switch (aPatientID)
	    {
	        case P4:
	            p_mesh_set = &m_p_P4_polygon_mesh_set;
	            p_HU_set = g_p_P4_HU_set;
	            break;

	        case P5:
	            p_mesh_set = &m_p_P5_polygon_mesh_set;
	            p_HU_set = g_p_P5_HU_set;
	            break;

	        default:
	            p_mesh_set = &m_p_P5_polygon_mesh_set;
	            p_HU_set = g_p_P5_HU_set;
	    }

	    // Set geometry
	    for (unsigned int i(0); i < p_mesh_set->size(); ++i)
	    {
	        char* p_data(0);
	        unsigned int data_size(0);
	        decompressMesh(i, aPatientID, p_data, data_size);

	        (*p_mesh_set)[i]->loadSTLDataFromStream(p_data, false, true, false, true, mm, GL_STATIC_DRAW);

	        // Delete data
	        if (p_data)
	        {
	            delete [] p_data;
	            p_data = 0;
	        }

	        (*p_mesh_set)[i]->setHounsfieldValue(p_HU_set[i]);
	        (*p_mesh_set)[i]->initialize();
	    }

	    // Compute the translation vector
	    switch (aPatientID)
	    {
	        case P4:
	            m_P4_translation_vector = (*p_mesh_set)[SKIN_ID]->getMinCorner() + ((*p_mesh_set)[SKIN_ID]->getMaxCorner() - (*p_mesh_set)[SKIN_ID]->getMinCorner()) / 2.0;
	            m_P4_rotation_matrix = Matrix4x4<GLfloat>::buildRotationMatrix(180, VEC3(1, 0, 0));
	            m_P4_rotation_matrix *= Matrix4x4<GLfloat>::buildTranslationMatrix(-m_P4_translation_vector);
	            break;

	        case P5:
	            m_P5_translation_vector = (*p_mesh_set)[SKIN_ID]->getMinCorner() + ((*p_mesh_set)[SKIN_ID]->getMaxCorner() - (*p_mesh_set)[SKIN_ID]->getMinCorner()) / 2.0;
	            m_P5_rotation_matrix = Matrix4x4<GLfloat>::buildTranslationMatrix(-m_P5_translation_vector);
	            break;

	        default:
	            break;
	    }
}

//----------------------------------------------------
void GLWidget::usePatient(const PatientID& aPatientID)
//----------------------------------------------------
{
    std::vector<AnatomicalMesh*>* p_mesh_set(0);
    switch (aPatientID)
    {
        case P4:
            m_current_patient = P4;
            p_mesh_set = &m_p_P4_polygon_mesh_set;
            break;

        case P5:
            m_current_patient = P5;
            p_mesh_set = &m_p_P5_polygon_mesh_set;
            break;

        default:
            m_current_patient = P5;
            p_mesh_set = &m_p_P5_polygon_mesh_set;
    }

    if (!p_mesh_set->size())
    {
        throw "No anatomical data provided.";
    }

    m_xray_renderer.removeInnerSurfaces();
    m_xray_renderer.removeOuterSurface();
    for (unsigned int i(0); i < p_mesh_set->size(); ++i)
    {
        m_xray_renderer.addInnerSurface(((*p_mesh_set)[i]));
    }

    // Reset the simulation state
    //reset();

    m_next_angle_to_process = 0;
    m_current_angle = 0;
}


//------------------------------
void GLWidget::updateXRayImage()
//------------------------------
{
    QDateTime start_time = QDateTime::currentDateTime();

    // Compute the X-Ray image
    if (m_current_patient == P4)
    {
        m_xray_renderer.computeImage(m_P4_rotation_matrix);
    }
    else
    {
        m_xray_renderer.computeImage(m_P5_rotation_matrix);
    }

    // Normalise the X-ray image
    if (fabs(m_current_angle) < EPSILON)
    {
        m_xray_renderer.normalise();
    }

    QDateTime stop_time = QDateTime::currentDateTime();
    m_simulation_total_time += float(start_time.time().msecsTo(stop_time.time())) / 1000.0;
    ++m_number_of_rotations;

    // Update the rotation angle
    if (m_number_of_rotations)
    {
        m_angle_step = (m_simulation_total_time / float(m_number_of_rotations)) * 180.0 / 10.0;
    }
}


//---------------------
void GLWidget::render()
//---------------------
{
    try
    {
    	renderToShadowMap();

    	//Restore the view point
    	glViewport(0, 0, width(), height());

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
        m_display_shader.enable();
        GLint shader_id(m_display_shader.getProgramHandle());

        // Define the light colours here
        GLfloat light_global_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
        GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
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

        handle = glGetUniformLocation(shader_id, "light_direction");
        glUniform3f(handle, m_light_direction.getX(), m_light_direction.getY(), m_light_direction.getZ());
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Check the status of OpenGL and of the current FBO
        checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Store the current transformation matrix
        pushModelViewMatrix();

        // Rotate the sample
        g_current_modelview_matrix *= m_scene_rotation_matrix;

        //Bind the shadow map texture in texture unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_shadowMap.getMapTexture());

        handle = glGetUniformLocation(shader_id, "shadowMap");
        glUniform1i(handle,0);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "g_shadowMapMatrix");
        glUniformMatrix4fv(handle, 1, GL_FALSE, m_shadowMatrix.get());
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

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

        // Set the modelview matrix
        if (m_current_patient == P4)
        {
            g_current_modelview_matrix *= m_P4_rotation_matrix;
        }
        else
        {
            g_current_modelview_matrix *= m_P5_rotation_matrix;
        }

        applyModelViewMatrix();

        // Display the current patient
        if (m_current_patient == P4)
        {
            displayPatient(m_p_P4_polygon_mesh_set, shader_id);
        }
        else
        {
            displayPatient(m_p_P5_polygon_mesh_set, shader_id);
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
        m_xray_detector.displaySource();

        // Display the X-Ray image
        glDisable(GL_CULL_FACE);
        m_xray_renderer.display(true);

        // Display the beam
        if (m_display_beam)
        {
            const GLfloat material_ambient[]  = {0.75, 0, 0.5, 0.1};
            handle = glGetUniformLocation(shader_id, "material_ambient");
            glUniform4fv(handle, 1, &material_ambient[0]);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            lighting = 0;
            handle = glGetUniformLocation(shader_id,"g_use_lighting");
            glUniform1iv(handle, 1, &lighting);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            m_xray_detector.displayBeam();
        }

        //clean up the texture
		glBindTexture(GL_TEXTURE_2D, 0);

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


//----------------------------------------------------------------
void GLWidget::displayPatient(std::vector<AnatomicalMesh*>& apMeshSet,
                              GLint aShaderID)
//----------------------------------------------------------------
{
	for(int i(apMeshSet.size()-1); i>=0; --i)
	{
		apMeshSet[i]->defineShaderInput(aShaderID);

        if (m_display_wireframe)
        {
            apMeshSet[i]->displayWireFrame();
        }
        else
        {
        	if(apMeshSet[i]->isTransparent())
     	    {
        		pushEnableDisableState(GL_CULL_FACE);
        		pushEnableDisableState(GL_BLEND);

        		glDisable(GL_CULL_FACE);
        		glEnable(GL_BLEND);
        		glBlendFunc(GL_ONE, GL_ONE);

        		apMeshSet[i]->display();

        		popEnableDisableState();
        		popEnableDisableState();
    	    }
        	else
        	{
                apMeshSet[i]->display();
        	}
        }
    }
}


//--------------------------
void GLWidget::displayHelp()
//--------------------------
{
#ifdef HAS_FREETYPE
    GLfloat p_background_colour [] = {
            g_background_colour.getX(),
            g_background_colour.getY(),
            g_background_colour.getZ(),
            0.0
    };

    GLfloat p_font_colour [] = {
            1.0,
            1.0,
            0.0,
            1.0
    };

    pushEnableDisableState(GL_BLEND);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int x_position(15);
    int offset(29);
    m_font_rendered.renderText(std::string("Help"),
            20,   x_position += offset,
            m_text_2D_projection_matrix,
            p_background_colour,
            p_font_colour);

    m_font_rendered.renderText(std::string("\'h\': display|hide help message"),
            40,   x_position += offset,
            m_text_2D_projection_matrix,
            p_background_colour,
            p_font_colour);

    m_font_rendered.renderText(std::string("\'n\': toggle negative image"),
            40,   x_position += offset,
            m_text_2D_projection_matrix,
            p_background_colour,
            p_font_colour);

    m_font_rendered.renderText(std::string("\'p\': change patient"),
            40,   x_position += offset,
            m_text_2D_projection_matrix,
            p_background_colour,
            p_font_colour);

    m_font_rendered.renderText(std::string("\'b\': display|hide X-ray beam"),
            40,   x_position += offset,
            m_text_2D_projection_matrix,
            p_background_colour,
            p_font_colour);

    m_font_rendered.renderText(std::string("\'w\': wireframe|solid rendering"),
            40,   x_position += offset,
            m_text_2D_projection_matrix,
            p_background_colour,
            p_font_colour);

    m_font_rendered.renderText(std::string("\'space bar\': change tomography mode"),
            40, x_position += offset,
            m_text_2D_projection_matrix,
            p_background_colour,
            p_font_colour);

    m_font_rendered.renderText(std::string("\'q\', 'Esc': quit"),
            40,   x_position += offset,
            m_text_2D_projection_matrix,
            p_background_colour,
            p_font_colour);

    popEnableDisableState();
#endif
}


//-------------------------------------------------
void GLWidget::decompressMesh(unsigned int aMeshID,
                              PatientID aPatientID,
                              char*& apData,
                              unsigned int& aSize)
//-------------------------------------------------
{
    // Initialise the data
    if (apData)
    {
        delete [] apData;
        apData = 0;
    }

    aSize = 0;

    if (aPatientID == P4)
    {
        switch (aMeshID)
        {
        case SKIN_ID:
            aSize = inflate(g_P4_skin_stl, sizeof(g_P4_skin_stl), &apData);
            break;

        case LIVER_ID:
            aSize = inflate(g_P4_liver_stl, sizeof(g_P4_liver_stl), &apData);
            break;

        case LUNG_ID:
            aSize = inflate(g_P4_lung_stl, sizeof(g_P4_lung_stl), &apData);
            break;

        case DIAPH_ID:
            aSize = inflate(g_P4_diaph_stl, sizeof(g_P4_diaph_stl), &apData);
            break;

        case SPINE_ID:
            aSize = inflate(g_P4_spine_stl, sizeof(g_P4_spine_stl), &apData);
            break;

        case RIBS_ID:
            aSize = inflate(g_P4_ribs_stl, sizeof(g_P4_ribs_stl), &apData);
            break;

        default:
            break;
        }
    }
    else
    {
        switch (aMeshID)
        {
        case SKIN_ID:
            aSize = inflate(g_P5_skin_stl, sizeof(g_P5_skin_stl), &apData);
            break;

        case LIVER_ID:
            aSize = inflate(g_P5_liver_stl, sizeof(g_P5_liver_stl), &apData);
            break;

        case LUNG_ID:
            aSize = inflate(g_P5_lung_stl, sizeof(g_P5_lung_stl), &apData);
            break;

        case DIAPH_ID:
            aSize = inflate(g_P5_diaph_stl, sizeof(g_P5_diaph_stl), &apData);
            break;

        case SPINE_ID:
            aSize = inflate(g_P5_spine_stl, sizeof(g_P5_spine_stl), &apData);
            break;

        case RIBS_ID:
            aSize = inflate(g_P5_ribs_stl, sizeof(g_P5_ribs_stl), &apData);
            break;

        default:
            break;
        }
    }

    if (aSize <= 0)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
    }
}


//------------------------------------------------------
void GLWidget::computeRotation(MATRIX4& aRotationMatrix,
                               int aXmousePosition,
                               int aYmousePosition)
//------------------------------------------------------
{
    if (m_use_arc_ball)
    {
        if (aXmousePosition != m_last_x_position || aYmousePosition != m_last_y_position)
        {
            Vec3<GLfloat> va(getArcballVector(m_last_x_position,    m_last_y_position));
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

            m_last_x_position = aXmousePosition;
            m_last_y_position = aYmousePosition;
        }
    }
}

//------------------------------------------------------
void GLWidget::initializePatient(std::vector<AnatomicalMesh*>& apMeshSet)
//------------------------------------------------------
{
	apMeshSet.push_back(new SkinMesh());
	apMeshSet.push_back(new LiverMesh());
	apMeshSet.push_back(new LungsMesh());
	apMeshSet.push_back(new DiaphragmMesh());
	apMeshSet.push_back(new BoneMesh());
	apMeshSet.push_back(new BoneMesh());
}

//------------------------------------------------------
void GLWidget::renderToShadowMap()
//------------------------------------------------------
{
	//set the  matrix from the light's point of view to create the shadow map
	Matrix4x4<GLfloat> shadowProjectionMatrix(buildOrthoProjectionMatrix(-300.f, 300.f, -300.f, 300.f, -300.f, 300.f));

	Matrix4x4<GLfloat> shadowViewMatrix(buildLookAtModelViewMatrix(
			m_light_direction,
			VEC3(0.f, 0.f, 0.f),
			VEC3(0.f, 1.f, 0.f)
		));

	//create the shadow map
	if (m_current_patient == P4)
	{
		shadowViewMatrix *= m_scene_rotation_matrix*m_P4_rotation_matrix;
		m_shadowMatrix=shadowProjectionMatrix*shadowViewMatrix;
		m_shadowMap.render(m_p_P4_polygon_mesh_set, m_shadowMatrix, m_depthShader);
	}
	else
	{
		shadowViewMatrix *= m_scene_rotation_matrix*m_P5_rotation_matrix;
		m_shadowMatrix=shadowProjectionMatrix*shadowViewMatrix;
		m_shadowMap.render(m_p_P5_polygon_mesh_set, m_shadowMatrix, m_depthShader);
	}
}


#if 0
//------------------------------------------------------
void GLWidget::loadShader(const void *srcVert, const void *srcFrag, std::string const& name, Shader& shader)
//------------------------------------------------------
{
	// Initialise the shaders
	char* p_vertex_shader(0);
	char* p_fragment_shader(0);

	int z_lib_return_code_vertex(0);
	int z_lib_return_code_fragment(0);

	std::string vertex_shader;
	std::string fragment_shader;

	// Display shader

	z_lib_return_code_vertex   = inflate(srcVert, sizeof(srcVert),   &p_vertex_shader);
	z_lib_return_code_fragment = inflate(srcFrag, sizeof(srcFrag), &p_fragment_shader);


	vertex_shader   = p_vertex_shader;
	fragment_shader = p_fragment_shader;
	delete [] p_vertex_shader;     p_vertex_shader = 0;
	delete [] p_fragment_shader; p_fragment_shader = 0;

	if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
	}
	shader.setLabels(name+".vert", name+".frag");
	shader.loadSource(vertex_shader, fragment_shader);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}
#endif //0
