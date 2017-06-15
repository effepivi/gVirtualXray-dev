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
*   @file       AnatomicalDataHandler.cxx
*
*   @brief      Class to manage the anatomical data corresponding a patient.
*
*   @version    1.0
*
*   @date       13/07/2016
*
*   @author     Dr Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <streambuf>
#include <ctime>
#include <algorithm>

#define GLFW_INCLUDE_GLCOREARB 1

#include <GLFW/glfw3.h>

#ifndef __Units_h
#include "gVirtualXRay/Units.h"
#endif

#ifndef __CubeMesh_h
#include "gVirtualXRay/CubeMesh.h"
#endif

#ifndef __OpenGLUtilities_h
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef __Utilities_h
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef __Exception_h
#include "gVirtualXRay/Exception.h"
#endif

#ifndef __BoneMesh_h
#include "gVirtualXRay/BoneMesh.h"
#endif

#ifndef __DiaphragmMesh_h
#include "gVirtualXRay/DiaphragmMesh.h"
#endif

#ifndef __LiverMesh_h
#include "gVirtualXRay/LiverMesh.h"
#endif

#ifndef __LungsMesh_h
#include "gVirtualXRay/LungsMesh.h"
#endif

#ifndef __SkinMesh_h
#include "gVirtualXRay/SkinMesh.h"
#endif

#ifndef __AnatomicalDataHandler_h
#include "AnatomicalDataHandler.h"
#endif

#include "display_gl2.frag.h"
#include "display_gl2.vert.h"

#include "display_gl3.frag.h"
#include "display_gl3.vert.h"

#include "display_realistic_gl2.frag.h"
#include "display_realistic_gl2.vert.h"

#include "display_realistic_gl3.frag.h"
#include "display_realistic_gl3.vert.h"
/*
#include "P4_skin.stl.h"
#include "P4_diaph.stl.h"
#include "P4_spine.stl.h"
#include "P4_ribs.stl.h"
*/
//#include "kidney.stl.h"
//#include "lung.stl.h"
//#include "liver.stl.h"
//#include "diaphragm.stl.h"


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Constant variables
//******************************************************************************
const GLfloat g_rotation_speed(2.0);

const float g_scaling_factor(0.25);
const VEC2 g_resolution(0.7 * g_scaling_factor * mm, 1 * g_scaling_factor * mm);
const Vec2ui g_number_of_pixels(500 / g_scaling_factor, 225 / g_scaling_factor);

const float g_incident_energy(0.08 * MeV);

const double g_initial_fovy(45);        // Field of view along the y-axis
const double g_initial_near(5.0 * cm);  // Near clipping plane
const double g_initial_far(500.0 * cm); // Far clipping plane

const VEC3 g_initial_source_position(   0.0,  25.0 * cm, 0.0);
const VEC3 g_initial_detector_position( 0.0, -25.0 * cm, 0.0);
const VEC3 g_initial_detector_up_vector(0.0, 0.0, 1.0);
const VEC3 g_background_colour(0.5, 0.5, 0.5);
/*
const unsigned char SKIN_ID(0);
const unsigned char LIVER_ID(1);
const unsigned char LUNG_ID(2);
const unsigned char DIAPH_ID(3);
const unsigned char SPINE_ID(4);
const unsigned char RIBS_ID(5);
const unsigned char KIDNEY_ID(6);
*/

/*
4593: "right palatine tonsil"
4595: "thyroid gland"
4596: "left palatine tonsil"
4598: "grey matter"
4632: "lingual tonsil"
5149: "cranium"
5152: "mandible"
5153: "left styloid process"
5154: "right styloid process"
5156: "left eye"
5157: "right eye"
5158: "brain"
5161: "larynx"
5176: "right lens"
5177: "left lens"
5178: "frontal sinus"
5179: "right maxillary sinus"
5180: "left maxillary sinus"
6507: "atlas C1"
6508: "axis C2"
6509: "cervical vertebra C3"
6510: "cervical vertebra C4"
6511: "cervical vertebra C5"
6512: "cervical vertebra C6"
6545: "hyoid bone"
*/


const unsigned char VERTEBRA_ID(0);
const unsigned char HYOID_BONE_ID(1);
const unsigned char SINUS_ID(2);
const unsigned char EYE_ID(3);
const unsigned char TONGUE_ID(4);
const unsigned char TONSIL_ID(5);
const unsigned char GREY_MATTER_ID(6);
const unsigned char LENS_ID(7);
const unsigned char THYROID_ID(8);
const unsigned char BRAIN_ID(9);
const unsigned char LARYNX_ID(10);

const unsigned char CRANIUM_ID(11);


/*
const double g_p_HU_set [] =
{
    -70.202,
    36.9,
    -687.297,
    -41.284,
    181.08,
    181.08,
};*/


//******************************************************************************
//  Function declaration
//******************************************************************************
float radian2degree(float anAngle);


//******************************************************************************
//  Implementation
//******************************************************************************

//---------------------------------------
inline float radian2degree(float anAngle)
//---------------------------------------
{
    return (180.0 * anAngle / gVirtualXRay::PI);
}


//-----------------------------------------------------------------
AnatomicalDataHandler::AnatomicalDataHandler(unsigned int aWidth,
                                             unsigned int aHeight):
//-----------------------------------------------------------------
    m_width(aWidth),
    m_height(aHeight),

    m_zoom(100.0 * cm),

    m_use_arc_ball(false),
    m_last_x_position(0),
    m_last_y_position(0),

    m_display_beam(true),

    m_display_wireframe(false),

    m_angle_step(1),
    m_next_angle_to_process(0),
    m_current_angle(0),
    m_simulation_total_time(0),
    m_number_of_rotations(0),
    m_acquisition_type(ROTATE_SAMPLE),

    m_shadow_map(),

    #undef HAS_FREETYPE

    #ifdef HAS_FREETYPE
    m_display_help(true),
    #endif

    m_light_direction(-1, 3, 1)
//-----------------------------------------------------------------
{
}


//---------------------------------------------
AnatomicalDataHandler::~AnatomicalDataHandler()
//---------------------------------------------
{
    for(unsigned int i(0); i < m_p_polygon_mesh_set.size(); i++)
        {
        delete m_p_polygon_mesh_set[i];
        }
}


//----------------------------------------
void AnatomicalDataHandler::initializeGL()
//----------------------------------------
{
    // Initialise GLEW
    initialiseGLEW();
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Check the OpenGL version
    std::cout << "GL:\t" <<
        glGetString(GL_VERSION) << std::endl;

    // Check the GLSL version
    std::cout << "GLSL:\t" <<
        glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // Initialise OpenGL
    initGL();
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Initialise FreeType
    initFreeType();

    // Load the data
    loadDetector();
    loadSource();
    loadXRaySimulator();

    // Add the geometry to the X-ray renderer
    initializePatient();
    loadSTLFiles();

    //initialize the buffers
    m_shadow_map.initialize();

    //set the direction of the light
    m_light_direction.normalize();

    // Initialise the modelling matrix
    m_scene_rotation_matrix *=
        Matrix4x4<GLfloat>::buildRotationMatrix(180.0, VEC3(0, 0, 1));
    
    m_scene_rotation_matrix *=
        Matrix4x4<GLfloat>::buildRotationMatrix( 60.0, VEC3(1, 0, 0));
}


//--------------------------------
void AnatomicalDataHandler::draw()
//--------------------------------
{
    // Clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Display the 3D elements
    glViewport(0, 0, m_width, m_height);

    // Store the current transformation matrix
    pushModelViewMatrix();
    pushProjectionMatrix();

    double screen_aspect_ratio(double(m_width) / double(m_height));

    loadPerspectiveProjectionMatrix(
        g_initial_fovy,
        screen_aspect_ratio,
        g_initial_near,
        g_initial_far);

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
                    Matrix4x4<GLfloat> rotation_matrix;
                    rotation_matrix *=
                        Matrix4x4<GLfloat>::buildTranslationMatrix(
                            m_translation_vector);
                
                    rotation_matrix *=
                        Matrix4x4<GLfloat>::buildRotationMatrix(
                            -m_angle_step,
                            g_initial_detector_up_vector);
                
                    rotation_matrix *=
                        Matrix4x4<GLfloat>::buildTranslationMatrix(
                            -m_translation_vector);

                
                    m_rotation_matrix *= rotation_matrix;
                }
                break;

            case ROTATE_SOURCE_DETECTOR:
                {
                    Matrix4x4<GLfloat> rotation_matrix;
                    rotation_matrix *=
                        Matrix4x4<GLfloat>::buildRotationMatrix(m_angle_step,
                            g_initial_detector_up_vector);

                    VEC3 source_position(
                        rotation_matrix * m_xray_detector.getXraySourceCentre());
                
                    VEC3 detector_position(
                        rotation_matrix * m_xray_detector.getDetectorPosition());

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
            {
            reset();
            }
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
}


//-----------------------------------------------------------
void AnatomicalDataHandler::resizeGL(int aWidth, int aHeight)
//-----------------------------------------------------------
{
    m_width = aWidth;
    m_height = aHeight;

    // Avoid a division by zero
    if (m_height == 0)
        {
        // Prevent divide by 0
        m_height = 1;
        }

    // Update the viewport
    glViewport(0, 0, m_width, m_height);

    m_text_2D_projection_matrix = buildOrthoProjectionMatrix(0,
        m_width,
        m_height,
        0,
        -1.0,
        1.0);
}


//----------------------------------
void AnatomicalDataHandler::initGL()
//----------------------------------
{
    std::cout << "OpenGL renderer:   " <<
        glGetString(GL_RENDERER)   << std::endl;
    
    std::cout << "OpenGL version:    " <<
        glGetString(GL_VERSION)    << std::endl;
    
    std::cout << "OpenGL vender:     " <<
        glGetString(GL_VENDOR)     << std::endl;

    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Enable the Z-buffer
    glEnable(GL_DEPTH_TEST);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Set the background colour
    glClearColor(g_background_colour.getX(),
        g_background_colour.getY(),
         g_background_colour.getZ(), 1.0);
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
    if (useOpenGL3_2OrAbove())
        {
        z_lib_return_code_vertex   = inflate(g_display_realistic_gl3_vert,
            sizeof(g_display_realistic_gl3_vert),
            &p_vertex_shader);
        
        z_lib_return_code_fragment = inflate(g_display_realistic_gl3_frag,
            sizeof(g_display_realistic_gl3_frag),
            &p_fragment_shader);
        
        m_display_shader.setLabels("display_realistic_gl3.vert",
            "display_realistic_gl3.frag");
        }
    else
        {
        z_lib_return_code_vertex   = inflate(g_display_realistic_gl2_vert,
            sizeof(g_display_realistic_gl2_vert),
            &p_vertex_shader);
        
        z_lib_return_code_fragment = inflate(g_display_realistic_gl2_frag,
            sizeof(g_display_realistic_gl2_frag),
            &p_fragment_shader);
        
        m_display_shader.setLabels("display_realistic_gl2.vert",
            "display_realistic_gl2.frag");
        }

    vertex_shader   = p_vertex_shader;
    fragment_shader = p_fragment_shader;

    delete [] p_vertex_shader;
    delete [] p_fragment_shader;

    p_vertex_shader = 0;
    p_fragment_shader = 0;

    if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 ||
            !vertex_shader.size() || !fragment_shader.size())
        {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "Cannot decode the shader using ZLib.");
        }
    
    m_display_shader.loadSource(vertex_shader, fragment_shader);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//----------------------------------------
void AnatomicalDataHandler::initFreeType()
//----------------------------------------
{
#ifdef HAS_FREETYPE
    m_font_rendered.initialise(PREFIX"/data/arial.ttf", 16 * 27);
    m_display_help = true;
#endif
}


//----------------------------------------
void AnatomicalDataHandler::loadDetector()
//----------------------------------------
{
    m_xray_detector.setResolutionInUnitsOfLengthPerPixel(g_resolution);
    m_xray_detector.setNumberOfPixels(g_number_of_pixels);
    m_xray_detector.setDetectorPosition(g_initial_detector_position);
    m_xray_detector.setUpVector(g_initial_detector_up_vector);
}


//--------------------------------------
void AnatomicalDataHandler::loadSource()
//--------------------------------------
{
    // Set the energy
    m_xray_beam.initialise(g_incident_energy);

    // Set the source position
    m_xray_detector.setXrayPointSource(g_initial_source_position);

    // Use a parallel source
    m_xray_detector.setParallelBeam();
}


//---------------------------------------------
void AnatomicalDataHandler::loadXRaySimulator()
//---------------------------------------------
{
    // Initialise the X-ray renderer
    m_xray_renderer.initialise(XRayRenderer::OPENGL,
        GL_RGB32F,
        &m_xray_detector,
        &m_xray_beam);
}


//---------------------------------------
void AnatomicalDataHandler::loadPatient()
//---------------------------------------
{
    // Set geometry
    for (unsigned int i(0); i < m_p_polygon_mesh_set.size(); ++i)
        {
        //char* p_data(0);
        //unsigned int data_size(0);
        //decompressMesh(i, p_data, data_size);
        double HU_value(-1000);

        switch (i)
        {
        case VERTEBRA_ID:
            {
            BoneMesh p1;
            BoneMesh p2;
            BoneMesh p3;
            BoneMesh p4;
            BoneMesh p5;
            BoneMesh p6;

            p1.setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/cervical_vertebra_C3.stl");
            p2.setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/cervical_vertebra_C4.stl");
            p3.setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/cervical_vertebra_C5.stl");
            p4.setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/cervical_vertebra_C6.stl");
            p5.setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/atlas_C1.stl");
            p6.setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/axis_C2.stl");

            p1.loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);

            p2.loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);

            p3.loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);

            p4.loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);

            p5.loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);

            p6.loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);

            *(m_p_polygon_mesh_set[i]) = p1 + p2 + p3 + p4 + p5 + p6;

            HU_value =  (221.71129682829866 + 224.66567090742953 + 224.431593261313   + 220.99315552175565 + 221.96307037896366 + 221.50775979344235) / 6.0;
            }
            break;

        case HYOID_BONE_ID:
            {
            m_p_polygon_mesh_set[i]->setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/hyoid_bone.stl");

            m_p_polygon_mesh_set[i]->loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);

            HU_value = 208.4762484774665;
            }
            break;

        case SINUS_ID:
            {
            BoneMesh p1;
            BoneMesh p2;
            BoneMesh p3;

            p1.setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/left_maxillary_sinus.stl");
            p2.setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/right_maxillary_sinus.stl");
            p3.setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/frontal_sinus.stl");

            p1.loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);

            p2.loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);

            p3.loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);


            *(m_p_polygon_mesh_set[i]) = p1 + p2 + p3;

            HU_value = (0.5605566870332654 + 0.6805360496793516 + 1.2631457976862375) / 3.0;
            }
            break;

        case EYE_ID:
            {
            BoneMesh p1;
            BoneMesh p2;

            p1.setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/left_eye.stl");
            p2.setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/right_eye.stl");

            p1.loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);

            p2.loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);

            *(m_p_polygon_mesh_set[i]) = p1 + p2;

            HU_value = (77.58484270734033 + 78.10232558139535) / 2.0;
            }
            break;

        case TONGUE_ID:
            {
            m_p_polygon_mesh_set[i]->setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/hyoid_bone.stl");

            m_p_polygon_mesh_set[i]->loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);

            HU_value = 79.86510437179992;
            }
            break;

        case TONSIL_ID:
            {
            BoneMesh p1;
            BoneMesh p2;

            p1.setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/right_palatine_tonsil.stl");
            p2.setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/left_palatine_tonsil.stl");

            p1.loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);

            p2.loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);

            *(m_p_polygon_mesh_set[i]) = p1 + p2;

            HU_value = (82.83418988368948 + 89.67550839964633) / 2.0;
            }
            break;



        case GREY_MATTER_ID:
            {
            m_p_polygon_mesh_set[i]->setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/grey_matter.stl");

            m_p_polygon_mesh_set[i]->loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);

            HU_value = 88.03497942386831;
            }
            break;

        case LARYNX_ID:
            {
            m_p_polygon_mesh_set[i]->setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/larynx.stl");

            m_p_polygon_mesh_set[i]->loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);

            HU_value = 151.2662734818698;
            }
            break;

        case LENS_ID:
            {
            BoneMesh p1;
            BoneMesh p2;

            p1.setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/left_lens.stl");
            p2.setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/right_lens.stl");

            p1.loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);

            p2.loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);

            *(m_p_polygon_mesh_set[i]) = p1 + p2;

            HU_value = (90.068359375 + 92.84291187739464) / 2.0;
            }
            break;

        case THYROID_ID:
            {
            m_p_polygon_mesh_set[i]->setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/thyroid_gland.stl");

            m_p_polygon_mesh_set[i]->loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);

            HU_value = 94.07185628742515;
            }
            break;


        case BRAIN_ID:
            {
            m_p_polygon_mesh_set[i]->setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/brain.stl");

            m_p_polygon_mesh_set[i]->loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);

            HU_value = 94.97935295425961;
            }
            break;

        case CRANIUM_ID:
            {
            BoneMesh p1;
            BoneMesh p2;
            BoneMesh p3;
            BoneMesh p4;

            p1.setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/cranium.stl");
            p2.setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/mandible.stl");
            p3.setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/left_styloid_process.stl");
            p4.setFileName("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/STL/right_styloid_process.stl");

            p1.loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);

            p2.loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);

            p3.loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);

            p4.loadSTLFile(
                false,
                true,
                false,
                true,
                mm,
                GL_STATIC_DRAW);

            *(m_p_polygon_mesh_set[i]) = p1 + p2 + p3 + p4;

            HU_value = (233.32136682188195 + 232.73651678763204 + 232.71139102667863 + 227.32326283987916) / 4.0;
            }
            break;
        }
/*
        // Delete data
        if (p_data)
            {
            delete [] p_data;
            p_data = 0;
            }
*/
       // if (i == LEFT_LUNG_ID)
        {
            /*char* p_data(0);
            unsigned int data_size(0);
            decompressMesh(RIGHT_LUNG_ID, p_data, data_size);

            AnatomicalMesh right_lung;
            right_lung.loadSTLDataFromStream(
                    p_data,
                    false,
                    true,
                    false,
                    true,
                    mm,
                    GL_STATIC_DRAW);


            *(m_p_polygon_mesh_set[i]) = *(m_p_polygon_mesh_set[i]) + right_lung;

            // Delete data
            if (p_data)
                {
                delete [] p_data;
                p_data = 0;
                }*/
        }



        HU_value = -1000.0 + 2357 * (HU_value / 255.0);
        m_p_polygon_mesh_set[i]->setHounsfieldValue(HU_value);
        m_p_polygon_mesh_set[i]->initialize();
        }

    // Compute the translation vector
    m_translation_vector =
            m_p_polygon_mesh_set[CRANIUM_ID]->getMinCorner() +
        (m_p_polygon_mesh_set[CRANIUM_ID]->getMaxCorner() -
                m_p_polygon_mesh_set[CRANIUM_ID]->getMinCorner()) / 2.0;

    m_rotation_matrix = Matrix4x4<GLfloat>::buildRotationMatrix(
        180.0, VEC3(1, 0, 0));

    m_rotation_matrix *= Matrix4x4<GLfloat>::buildTranslationMatrix(
        -m_translation_vector);


    if (!m_p_polygon_mesh_set.size())
        {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "No anatomical data provided.");
        }

    m_xray_renderer.removeInnerSurfaces();
    m_xray_renderer.removeOuterSurface();
    for (unsigned int i(0); i < m_p_polygon_mesh_set.size(); ++i)
        {
        if (i != 10)
        m_xray_renderer.addInnerSurface((m_p_polygon_mesh_set[i]));
        }

    m_next_angle_to_process = 0;
    m_current_angle = 0;
}


//-------------------------------------------
void AnatomicalDataHandler::updateXRayImage()
//-------------------------------------------
{
    float start_time(glfwGetTime());

    // Compute the X-Ray image
    m_xray_renderer.computeImage(m_rotation_matrix);

    // Normalise the X-ray image
    if (fabs(m_current_angle) < EPSILON)
        {
        m_xray_renderer.normalise();
        }

    m_simulation_total_time += glfwGetTime() - start_time;
    ++m_number_of_rotations;

    // Update the rotation angle
    if (m_number_of_rotations)
        {
        m_angle_step = (m_simulation_total_time / float(m_number_of_rotations)) *
            180.0 / 10.0;
        }
}


//----------------------------------
void AnatomicalDataHandler::render()
//----------------------------------
{/*
    try
        {
        renderToShadowMap();

        //Restore the view point
        glViewport(0, 0, m_width, m_height);

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
        GLfloat light_global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
        GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
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
        glUniform3f(handle,
            m_light_direction.getX(),
            m_light_direction.getY(),
            m_light_direction.getZ());
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
        glBindTexture(GL_TEXTURE_2D, m_shadow_map.getMapTexture());

        handle = glGetUniformLocation(shader_id, "g_shadow_map");
        glUniform1i(handle,0);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "g_shadow_map_matrix");
        glUniformMatrix4fv(handle, 1, GL_FALSE, m_shadow_matrix.get());
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "g_projection_matrix");
        glUniformMatrix4fv(handle,
            1,
            GL_FALSE,
            g_current_projection_matrix.get());
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "g_modelview_matrix");
        glUniformMatrix4fv(handle,
            1,
            GL_FALSE,
            g_current_modelview_matrix.get());
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
        g_current_modelview_matrix *= m_rotation_matrix;


        applyModelViewMatrix();

        // Display the current patient
        displayPatient(shader_id);

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
        m_xray_renderer.display(true, false, true);

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
        }*/
}


//---------------------------------------------------------
void AnatomicalDataHandler::displayPatient(GLint aShaderID)
//---------------------------------------------------------
{
    for(int i(0); i < m_p_polygon_mesh_set.size() - 1; ++i)
        {
        m_p_polygon_mesh_set[i]->defineDisplayShaderInput(aShaderID);

        if (m_display_wireframe)
            {
            m_p_polygon_mesh_set[i]->displayWireFrame();
            }
        else
            {
            if(m_p_polygon_mesh_set[i]->isTransparent())
                 {
                pushEnableDisableState(GL_CULL_FACE);
                pushEnableDisableState(GL_BLEND);

                glDisable(GL_CULL_FACE);
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE, GL_ONE);

                m_p_polygon_mesh_set[i]->display();

                popEnableDisableState();
                popEnableDisableState();
              }
            else
                {
                m_p_polygon_mesh_set[i]->display();
                }
            }
        }
}


//--------------------------
void AnatomicalDataHandler::displayHelp()
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
    m_font_rendered.renderText(
        std::string("Help"),
        20,   x_position += offset,
        m_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    m_font_rendered.renderText(
        std::string("\'h\': display|hide help message"),
        40,   x_position += offset,
        m_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    m_font_rendered.renderText(
        std::string("\'n\': toggle negative image"),
        40,   x_position += offset,
        m_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    m_font_rendered.renderText(
        std::string("\'p\': change patient"),
        40,   x_position += offset,
        m_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    m_font_rendered.renderText(
        std::string("\'b\': display|hide X-ray beam"),
        40,   x_position += offset,
        m_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    m_font_rendered.renderText(
        std::string("\'w\': wireframe|solid rendering"),
        40,   x_position += offset,
        m_text_2D_projection_matrix,
        p_background_colour,
        p_font_colour);

    m_font_rendered.renderText(
        std::string("\'space bar\': change tomography mode"),
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


//--------------------------------------------------------------
void AnatomicalDataHandler::decompressMesh(unsigned int aMeshID,
                                           char*& apData,
                                           unsigned int& aSize)
//--------------------------------------------------------------
{
  /*  // Initialise the data
    if (apData)
        {
        delete [] apData;
        apData = 0;
        }

    aSize = 0;

    switch (aMeshID)
        {
        case SKIN_ID:
            aSize = inflate(g_P4_skin_stl, sizeof(g_P4_skin_stl), &apData);
            break;

        case LIVER_ID:
            aSize = inflate(bin2c_liver_stl_gz, sizeof(bin2c_liver_stl_gz), &apData);
            break;

        case LEFT_LUNG_ID:
            aSize = inflate(bin2c_left_lung_stl_gz, sizeof(bin2c_left_lung_stl_gz), &apData);
            break;

        case RIGHT_LUNG_ID:
            aSize = inflate(bin2c_right_lung_stl_gz, sizeof(bin2c_right_lung_stl_gz), &apData);
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

    if (aSize <= 0)
        {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "Cannot decode the shader using ZLib.");
        }*/
}


//-------------------------------------------------------------------
void AnatomicalDataHandler::computeRotation(int aXmousePosition,
                                            int aYmousePosition)
//-------------------------------------------------------------------
{
    if (m_use_arc_ball)
        {
        if (aXmousePosition != m_last_x_position ||
            aYmousePosition != m_last_y_position)
            {
            Vec3<GLfloat> va(getArcballVector(m_last_x_position,
                m_last_y_position));

            Vec3<GLfloat> vb(getArcballVector(aXmousePosition,
                aYmousePosition));

            float angle(g_rotation_speed *
                radian2degree(acos((std::min)(1.0, va.dotProduct(vb)))));

            Vec3<GLfloat> axis_in_camera_coord(va ^ vb);

            Matrix4x4<GLfloat> camera2object(m_scene_rotation_matrix.getInverse());

            Vec3<GLfloat> axis_in_object_coord(camera2object *
                axis_in_camera_coord);

            Matrix4x4<GLfloat> rotation_matrix;
            rotation_matrix.rotate(angle, axis_in_object_coord);
            m_scene_rotation_matrix = m_scene_rotation_matrix * rotation_matrix;

            }
        }

    m_last_x_position = aXmousePosition;
    m_last_y_position = aYmousePosition;
}


//------------------------------------------------------------
void AnatomicalDataHandler::enableArcBall(int aXmousePosition,
                                          int aYmousePosition)
//------------------------------------------------------------
{
    m_use_arc_ball = true;
    m_last_x_position = aXmousePosition;
    m_last_y_position = aYmousePosition;
}


//------------------------------------------
void AnatomicalDataHandler::disableArcBall()
//------------------------------------------
{
    m_use_arc_ball = false;
}


//----------------------------------
void AnatomicalDataHandler::zoomIn()
//----------------------------------
{
    if (!m_use_arc_ball)
        {
        m_zoom += 1.0 * cm;
        }
}


//-----------------------------------
void AnatomicalDataHandler::zoomOut()
//-----------------------------------
{
    if (!m_use_arc_ball)
        {
        m_zoom -= 1.0 * cm;
        }
}


//---------------------------------------------
void AnatomicalDataHandler::initializePatient()
//---------------------------------------------
{
    m_p_polygon_mesh_set.push_back(new BoneMesh());
    m_p_polygon_mesh_set.push_back(new BoneMesh());

    m_p_polygon_mesh_set.push_back(new SkinMesh());
    m_p_polygon_mesh_set.push_back(new SkinMesh());
    m_p_polygon_mesh_set.push_back(new SkinMesh());
    m_p_polygon_mesh_set.push_back(new SkinMesh());
    m_p_polygon_mesh_set.push_back(new SkinMesh());
    m_p_polygon_mesh_set.push_back(new SkinMesh());
    m_p_polygon_mesh_set.push_back(new SkinMesh());
    m_p_polygon_mesh_set.push_back(new SkinMesh());
    m_p_polygon_mesh_set.push_back(new SkinMesh());

    m_p_polygon_mesh_set.push_back(new BoneMesh());
/*
    m_p_polygon_mesh_set.push_back(new SkinMesh());
    m_p_polygon_mesh_set.push_back(new LiverMesh());
    m_p_polygon_mesh_set.push_back(new LungsMesh());
    m_p_polygon_mesh_set.push_back(new DiaphragmMesh());
    m_p_polygon_mesh_set.push_back(new BoneMesh());
    m_p_polygon_mesh_set.push_back(new BoneMesh());
*/
}

//---------------------------------------------
void AnatomicalDataHandler::renderToShadowMap()
//---------------------------------------------
{
    // Set the  matrix from the light's point of view to create the shadow map
    Matrix4x4<GLfloat> shadow_projection_matrix(buildOrthoProjectionMatrix(
        -300.0,
         300.0,
        -300.0,
         300.0,
        -300.0,
         300.0));

    Matrix4x4<GLfloat> shadow_view_matrix(buildLookAtModelViewMatrix(
        m_light_direction,
        VEC3(0.f, 0.f, 0.f),
        VEC3(0.f, 1.f, 0.f)
    ));

    // Create the shadow map
    shadow_view_matrix *= m_scene_rotation_matrix * m_rotation_matrix;
    m_shadow_matrix = shadow_projection_matrix * shadow_view_matrix;

    std::vector<PolygonMesh*> polygon_mesh_set;

    for(unsigned int i(0); i < m_p_polygon_mesh_set.size(); i++)
        {
        if (!m_p_polygon_mesh_set[i]->isTransparent())
            {
            polygon_mesh_set.push_back((m_p_polygon_mesh_set[i]));
            }
        }

    m_shadow_map.render(polygon_mesh_set, m_shadow_matrix);

}
