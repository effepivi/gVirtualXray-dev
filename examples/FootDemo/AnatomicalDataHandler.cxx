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
#ifdef HAS_GLEW
#include <GL/glew.h>
#endif // HAS_GLEW

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

#include "mesh_HU45.stl.h"
#include "mesh_HU729.stl.h"
#include "mesh_HU253.stl.h"
#include "mesh_HU113.stl.h"
#include "mesh_HU14.stl.h"
#include "mesh_HU89.stl.h"
#include "mesh_HU156.stl.h"


//******************************************************************************
//  Defines
//******************************************************************************
#define PREFIX ".."


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

const float g_incident_energy(0.12 * MeV);

const double g_initial_fovy(45);        // Field of view along the y-axis
const double g_initial_near(5.0 * cm);  // Near clipping plane
const double g_initial_far(500.0 * cm); // Far clipping plane

const VEC3 g_initial_source_position(   0.0,  25.0 * cm, 0.0);
const VEC3 g_initial_detector_position( 0.0, -25.0 * cm, 0.0);
const VEC3 g_initial_detector_up_vector(0.0, 0.0, 1.0);
const VEC3 g_background_colour(0.5, 0.5, 0.5);


const unsigned char SKIN_ID(0);
const unsigned char HARD_BONE_ID(1);
const unsigned char SOFT_BONE_ID(2);
const unsigned char SOFT_STRUCTURE0_ID(3);
const unsigned char SOFT_STRUCTURE1_ID(4);
const unsigned char SOFT_STRUCTURE2_ID(5);
const unsigned char SOFT_STRUCTURE3_ID(6);


const double g_p_HU_set [] =
{
        45,
        729,
        253,
        113,
        14,
        -89,
        -156
};


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
    m_acquisition_type(ROTATE_SOURCE_DETECTOR),

    m_display_help(true),
    m_has_freetype(true),

    m_use_log_scale(false),

    m_use_power_law(false),

    m_shift_value(0.0),

    m_scale_value(1.0 / g_incident_energy),

    m_gamma(1.0 / 2.5),

    m_light_direction(-1, 3, 1)
//-----------------------------------------------------------------
{
}


//---------------------------------------------
AnatomicalDataHandler::~AnatomicalDataHandler()
//---------------------------------------------
{
    release();
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
    initializePatients();
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
        reset();
        }

    // Update the X-ray image
    updateXRayImage();
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Draw the 3D scene
    render();
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Draw the help message
    if (m_display_help && m_has_freetype)
        {
        displayHelp();

        // Check the status of OpenGL and of the current FBO
        checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        }

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
    try
        {
        m_font_rendered.initialise(PREFIX"/data/arial.ttf", 16 * 27);
        m_display_help = true;
        m_has_freetype = true;
        }
    catch (const std::exception& error)
        {
        m_display_help = false;
        m_has_freetype = false;
        std::cerr << "WARNING:\t" << error.what() << std::endl;
        }
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

    // Use GPU artefact filtering by default
    m_xray_renderer.enableArtefactFilteringOnGPU();
}


//----------------------------------------
void AnatomicalDataHandler::loadSTLFiles()
//----------------------------------------
{
    // Set geometry
    for (unsigned int i(0); i < m_p_polygon_mesh_set.size(); ++i)
        {
        char* p_data(0);
        unsigned int data_size(0);
        decompressMesh(i, p_data, data_size);

        m_p_polygon_mesh_set[i]->loadSTLDataFromStream(
            p_data,
            false,
            true,
            false,
			false,
            mm,
            GL_STATIC_DRAW);

        // Delete data
        if (p_data)
            {
            delete [] p_data;
            p_data = 0;
            }

        m_p_polygon_mesh_set[i]->setHounsfieldValue(g_p_HU_set[i]);
        m_p_polygon_mesh_set[i]->initialize();
        }

    // Compute the translation vector
    m_translation_vector =
        m_p_polygon_mesh_set[SKIN_ID]->getMinCorner() +
        (m_p_polygon_mesh_set[SKIN_ID]->getMaxCorner() -
            m_p_polygon_mesh_set[SKIN_ID]->getMinCorner()) / 2.0;

    m_rotation_matrix = Matrix4x4<GLfloat>::buildRotationMatrix(
        180.0, VEC3(1, 0, 0));

    m_rotation_matrix *= Matrix4x4<GLfloat>::buildTranslationMatrix(
        -m_translation_vector);

    m_xray_renderer.removeInnerSurfaces();
    m_xray_renderer.removeOuterSurface();
    for (unsigned int i(0); i < m_p_polygon_mesh_set.size(); ++i)
        {
        m_p_polygon_mesh_set[i]->updateVBO(GL_STATIC_DRAW, GL_TRIANGLES);

        if (i != SKIN_ID)
            {
            m_xray_renderer.addInnerSurface((m_p_polygon_mesh_set[i]));
            }
        else
            {
             m_xray_renderer.addOuterSurface((m_p_polygon_mesh_set[i]));
            }
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
        m_xray_renderer.getEnergyFluenceMinMax();
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
{
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
        glUniform1i(handle, 0);
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

        GLint lighting(1);
        handle = glGetUniformLocation(shader_id,"g_use_lighting");
        glUniform1iv(handle, 1, &lighting);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Store the current transformation matrix
        pushModelViewMatrix();

        // Set the modelview matrix
        g_current_modelview_matrix *= m_rotation_matrix;

        applyModelViewMatrix();

        // Display the current patient
        displayPatient(m_p_polygon_mesh_set, shader_id);

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
        m_xray_renderer.display(false, m_use_log_scale, m_use_power_law, m_shift_value, m_scale_value, m_gamma);

        // Display the beam
        if (m_display_beam)
            {
            const GLfloat material_ambient[]  = {0.75, 0, 0.5, 0.1};
            handle = glGetUniformLocation(shader_id, "material_ambient");
            glUniform4fv(handle, 1, &material_ambient[0]);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            GLint lighting(0);
            handle = glGetUniformLocation(shader_id,"g_use_lighting");
            glUniform1iv(handle, 1, &lighting);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            m_xray_detector.displayBeam();
            }

        //clean up the texture
        glBindTexture(GL_TEXTURE_2D, 0);

        // Restore the current transformation matrix
        popModelViewMatrix();

        // Disable the shader
        popShaderProgram();

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


//---------------------------------------------------------------------------------
void AnatomicalDataHandler::displayPatient(std::vector<AnatomicalMesh*>& apMeshSet,
                                           GLint aShaderID)
//---------------------------------------------------------------------------------
{
     for(int i(apMeshSet.size() - 1); i >= 0; --i)
        {
        apMeshSet[i]->defineDisplayShaderInput(aShaderID);

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
                    if (i == HARD_BONE_ID || i == SOFT_BONE_ID)
                    {
                    apMeshSet[i]->display();
                    }
                }
            }
        }
}


//---------------------------------------
void AnatomicalDataHandler::displayHelp()
//---------------------------------------
{
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
}


//--------------------------------------------------------------
void AnatomicalDataHandler::decompressMesh(unsigned int aMeshID,
                                           char*& apData,
                                           unsigned int& aSize)
//--------------------------------------------------------------
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
            aSize = inflate(g_mesh_HU45_stl, sizeof(g_mesh_HU45_stl), &apData);
            break;

        case HARD_BONE_ID:
            aSize = inflate(g_mesh_HU729_stl, sizeof(g_mesh_HU729_stl), &apData);
            break;

        case SOFT_BONE_ID:
            aSize = inflate(g_mesh_HU253_stl, sizeof(g_mesh_HU253_stl), &apData);
            break;

        case SOFT_STRUCTURE0_ID:
            aSize = inflate(g_mesh_HU113_stl, sizeof(g_mesh_HU113_stl), &apData);
            break;

        case SOFT_STRUCTURE1_ID:
            aSize = inflate(g_mesh_HU14_stl, sizeof(g_mesh_HU14_stl), &apData);
            break;

        case SOFT_STRUCTURE2_ID:
            aSize = inflate(g_mesh_HU89_stl, sizeof(g_mesh_HU89_stl), &apData);
            break;

        case SOFT_STRUCTURE3_ID:
            aSize = inflate(g_mesh_HU156_stl, sizeof(g_mesh_HU156_stl), &apData);
            break;

        default:
            break;
        }

    if (aSize <= 0)
        {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "Cannot decode the polygon mesh using ZLib.");
        }
}


//-------------------------------------------------------------------
void AnatomicalDataHandler::computeRotation(MATRIX4& aRotationMatrix,
                                            int aXmousePosition,
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

            Matrix4x4<GLfloat> camera2object(aRotationMatrix.getInverse());

            Vec3<GLfloat> axis_in_object_coord(camera2object *
                axis_in_camera_coord);

            Matrix4x4<GLfloat> rotation_matrix;
            rotation_matrix.rotate(angle, axis_in_object_coord);
            aRotationMatrix = aRotationMatrix * rotation_matrix;

            m_last_x_position = aXmousePosition;
            m_last_y_position = aYmousePosition;
            }
        }
}

//------------------------------------------------------
void AnatomicalDataHandler::initializePatients()
//------------------------------------------------------
{
    m_p_polygon_mesh_set.push_back(new SkinMesh());
    m_p_polygon_mesh_set.push_back(new BoneMesh());
    m_p_polygon_mesh_set.push_back(new SkinMesh());
    m_p_polygon_mesh_set.push_back(new SkinMesh());
    m_p_polygon_mesh_set.push_back(new SkinMesh());
    m_p_polygon_mesh_set.push_back(new SkinMesh());
    m_p_polygon_mesh_set.push_back(new SkinMesh());
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
