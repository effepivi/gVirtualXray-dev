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
/*
#include "display_realistic_gl2.frag.h"
#include "display_realistic_gl2.vert.h"

#include "display_realistic_gl3.frag.h"
#include "display_realistic_gl3.vert.h"
*/
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

const float g_scaling_factor(1.0);
const VEC2 g_resolution(0.7 * g_scaling_factor * mm, 1 * g_scaling_factor * mm);
const Vec2ui g_number_of_pixels(500 / g_scaling_factor, 1000 / g_scaling_factor);

const float g_incident_energy(0.08 * MeV);

const double g_initial_fovy(45);        // Field of view along the y-axis
const double g_initial_near(5.0 * cm);  // Near clipping plane
const double g_initial_far(500.0 * cm); // Far clipping plane

const VEC3 g_initial_source_position(   0.0,  50.0 * cm, 0.0);
const VEC3 g_initial_detector_position( 0.0, -50.0 * cm, 0.0);
const VEC3 g_initial_detector_up_vector(0.0, 0.0, 1.0);
const VEC3 g_background_colour(0.5, 0.5, 0.5);


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



ICPRPhamtom::~ICPRPhamtom()
{
}


//------------------------------------------------------------------------
void ICPRPhamtom::load(const std::string& aPath, const std::string& aName)
//------------------------------------------------------------------------
{

    std::string path(aPath);
    path += "/";
    path += aName;
    path += "/";
    path += aName;

    m_volume_dataset.loadASCII(path + ".dat", 299, 137, 348, VEC3(1.775 * mm, 1.775 * mm, 4.84 * mm));

    m_volume_dataset.save(path + ".mhd");

    loadTissueMaterial(path + "_media.dat");

    bool create_meshes_flag(false);

    short i(0);
    for (std::map<short, TissueMaterial>::iterator ite(m_p_material_table.begin());
            ite != m_p_material_table.end() && !create_meshes_flag;
            ++ite, ++i)
    {
        // Set the filename
        std::stringstream stl_file_name;
        stl_file_name << path << "_" << ite->first << ".stl";

        // OPen the file
        std::ifstream input_stream(stl_file_name.str().c_str(),
                std::ios::in | std::ios::binary);

        // The file is not open
        if (!input_stream.is_open())
            {
            create_meshes_flag = true;
            }

        else
        {
            m_p_polygon_set[ite->first].setFileName(stl_file_name.str());

            m_p_polygon_set[ite->first].loadSTLFile(false,
                true,
                false,
                false,
                mm,
                GL_STATIC_DRAW);

            m_p_polygon_set[ite->first].setHounsfieldValue(i);
            //m_p_polygon_set[ite->first].invertNormalVectors();
            }
    }

    if (create_meshes_flag)
    {
        m_p_polygon_set.clear();
    }

    loadMaterialDensity(path + "_organs.dat",
            create_meshes_flag);

    if (create_meshes_flag)
    {
        for (std::map<short, PolygonMesh>::iterator ite(m_p_polygon_set.begin());
                ite != m_p_polygon_set.end();
                ++ite)
        {
            std::stringstream stl_file_name;
            stl_file_name << path << "_" << ite->first << ".stl";

            //ite->second.setFileName(stl_file_name.str());
            ite->second.writeSTLFile();
        }
    }

    for (std::map<short, PolygonMesh>::iterator ite(m_p_polygon_set.begin());
            ite != m_p_polygon_set.end();
            ++ite)
    {
       // ite->second.mergeVertices(true);
    }
}


//--------------------------------------------------------------
void ICPRPhamtom::saveVolume(const std::string& aFileName) const
//--------------------------------------------------------------
{
    m_volume_dataset.save(aFileName);
}



//----------------------------------------------------------------
void ICPRPhamtom::loadTissueMaterial(const std::string& aFileName)
//----------------------------------------------------------------
{
    // Open the input file
    std::ifstream input_file_stream(aFileName.c_str());

    // The file does not exist, throw an error
    if (!input_file_stream.is_open())
    {
        throw FileDoesNotExistException(__FILE__, __FUNCTION__, __LINE__, aFileName);
    }

    // Skip first three lines
    std::string data;
    std::getline(input_file_stream, data);
    std::getline(input_file_stream, data);
    std::getline(input_file_stream, data);

    // Empty the material table
    m_p_material_table.clear();

    // Process every line of the file
    while (input_file_stream.good())
    {
        // Store the material ID
        short material_id;
        input_file_stream >> material_id;

        // Get the rest of the current line
        std::getline(input_file_stream, data);

        // Get the percentages only
        data = data.substr(76, 153 - 77);

        // Store the line in a stream
        std::stringstream line;
        line << data;

        // Decompose the stream
        float H, C, N, O, Na, Mg, P, S, Cl, K, Ca, Fe, I;
        line >> H >> C >> N >> O >> Na >> Mg >> P >> S >> Cl >> K >> Ca >> Fe >> I;

        // Build the material
        TissueMaterial material(0, 0,
                H,
                C,
                N,
                O,
                Na,
                Mg,
                P,
                S,
                Cl,
                0,
                K,
                Ca,
                0,
                0,
                0,
                0,
                0,
                Fe,
                I);

        // Add the material to the table
        m_p_material_table[material_id] = material;
    }
}


//-----------------------------------------------------------------
void ICPRPhamtom::loadMaterialDensity(const std::string& aFileName,
                                      bool aCreatePolygonMeshes)
//-----------------------------------------------------------------
{
    // Open the input file
    std::ifstream input_file_stream(aFileName.c_str());

    // The file does not exist, throw an error
    if (!input_file_stream.is_open())
    {
        throw FileDoesNotExistException(__FILE__, __FUNCTION__, __LINE__, aFileName);
    }

    // Skip first four lines
    std::string data;
    std::getline(input_file_stream, data);
    std::getline(input_file_stream, data);
    std::getline(input_file_stream, data);
    std::getline(input_file_stream, data);

    // Process every line of the file
    while (input_file_stream.good())
    {
        // Get the tissue ID
        short tissue_id;
        input_file_stream >> tissue_id;

        // Get the rest of the current line
        std::getline(input_file_stream, data);

        if (data.size() && input_file_stream.good())
        {
            // Get the relevant data only
           //std::cout << data << std::endl;
            data = data.substr(55 - (67 - data.size()), 67 - 56);

            // Store the rest of the line in a stream
            std::stringstream line;
            line << data;

            // Store the material ID
            short material_id;
            line >> material_id;

            // Store the material density
            double density;
            line >> density;
            density *= g / cm3;

            std::cout << tissue_id << "\t" << material_id << "\t" << density / (g / cm3) << std::endl;

            // Add the density to the material
            m_p_material_table[material_id].setDensity(density);

            // Add the polygon
            if (aCreatePolygonMeshes)
            {
                addToPolygon(tissue_id,
                        m_p_material_table[material_id],
                        m_p_polygon_set[material_id]);
            }
        }
    }
}


//------------------------------------------------
void ICPRPhamtom::addToPolygon(short aThreshold,
                               const TissueMaterial& aMaterial,
                               PolygonMesh& aPolygon)
//------------------------------------------------
{
    aPolygon += m_volume_dataset.marchingCubes(aThreshold, aThreshold, true);
    aPolygon.setTissueMaterial(aMaterial);
}


//------------------------------------------------
void ICPRPhamtom::displaySolid()
//------------------------------------------------
{
    for(std::map<short, gVirtualXRay::PolygonMesh>::iterator ite(m_p_polygon_set.begin());
            ite != m_p_polygon_set.end(); ++ite)
        {
            {
            ite->second.display();
            }
        }
}


//----------------------------------
void ICPRPhamtom::displayWireframe()
//----------------------------------
{
    for(std::map<short, gVirtualXRay::PolygonMesh>::iterator ite(m_p_polygon_set.begin());
            ite != m_p_polygon_set.end(); ++ite)
        {
            {
            ite->second.displayWireFrame();
            }
        }
}


//---------------------------------
VEC3 ICPRPhamtom::getCentre() const
//---------------------------------
{
    return (ICPRPhamtom::getCenter());
}


//---------------------------------
VEC3 ICPRPhamtom::getCenter() const
//---------------------------------
{
    VEC3 global_min_corner(FLT_MAX, FLT_MAX, FLT_MAX);
    VEC3 global_max_corner(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for(std::map<short, gVirtualXRay::PolygonMesh>::const_iterator ite(m_p_polygon_set.begin());
            ite != m_p_polygon_set.end(); ++ite)
        {
            {
                VEC3 local_min_corner(ite->second.getMinCorner());
                VEC3 local_max_corner(ite->second.getMaxCorner());

                if (global_min_corner[0] > local_min_corner[0]) global_min_corner[0] = local_min_corner[0];
                if (global_min_corner[1] > local_min_corner[1]) global_min_corner[1] = local_min_corner[1];
                if (global_min_corner[2] > local_min_corner[2]) global_min_corner[2] = local_min_corner[2];

                if (global_max_corner[0] < local_max_corner[0]) global_max_corner[0] = local_max_corner[0];
                if (global_max_corner[1] < local_max_corner[1]) global_max_corner[1] = local_max_corner[1];
                if (global_max_corner[2] < local_max_corner[2]) global_max_corner[2] = local_max_corner[2];
            }
        }

    return (global_min_corner + (global_max_corner - global_min_corner) / 2.0);
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
//    updateXRayImage();
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
        z_lib_return_code_vertex   = inflate(g_display_gl3_vert,
            sizeof(g_display_gl3_vert),
            &p_vertex_shader);
        
        z_lib_return_code_fragment = inflate(g_display_gl3_frag,
            sizeof(g_display_gl3_frag),
            &p_fragment_shader);
        
        m_display_shader.setLabels("display_gl3.vert",
            "display_gl3.frag");
        }
    else
        {
        z_lib_return_code_vertex   = inflate(g_display_gl2_vert,
            sizeof(g_display_gl2_vert),
            &p_vertex_shader);
        
        z_lib_return_code_fragment = inflate(g_display_gl2_frag,
            sizeof(g_display_gl2_frag),
            &p_fragment_shader);
        
        m_display_shader.setLabels("display_gl2.vert",
            "display_gl2.frag");
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


    m_female_icrp_phantom.load("./P110 data V1.2", "AF");

    // Compute the translation vector
    m_translation_vector = m_female_icrp_phantom.getCenter();

    m_rotation_matrix = Matrix4x4<GLfloat>::buildRotationMatrix(
        180.0, VEC3(1, 0, 0));

    m_rotation_matrix *= Matrix4x4<GLfloat>::buildTranslationMatrix(
        -m_translation_vector);

/*
    if (!m_p_polygon_mesh_set.size())
        {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "No anatomical data provided.");
        }
*/
    m_xray_renderer.removeInnerSurfaces();
    m_xray_renderer.removeOuterSurface();
  /*  for (unsigned int i(0); i < m_p_polygon_mesh_set.size(); ++i)
        {
        if (i != 10)
        m_xray_renderer.addInnerSurface((m_p_polygon_mesh_set[i]));
        }
*/
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
{
    try
        {
        // Handle for shader variables
        GLuint handle(0);

        // Enable back face culling
        pushEnableDisableState(GL_CULL_FACE);
        pushEnableDisableState(GL_DEPTH_TEST);
        pushEnableDisableState(GL_BLEND);

        glEnable(GL_DEPTH_TEST);
        //glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glDisable(GL_BLEND);

        // Enable the shader
        pushShaderProgram();
        m_display_shader.enable();
        GLint shader_id(m_display_shader.getProgramHandle());

        // Define the light colours here
        GLfloat light_global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
        GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
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

        VEC3 light_position(0, 0, m_zoom);
        handle = glGetUniformLocation(shader_id, "light_position");
        glUniform4f(handle,
            light_position.getX(),
            light_position.getY(),
            light_position.getZ(),
            1.0);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Check the status of OpenGL and of the current FBO
        checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Store the current transformation matrix
        pushModelViewMatrix();

        // Rotate the sample
        g_current_modelview_matrix *= m_scene_rotation_matrix;

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

        {
            const GLfloat material_ambient[]  = {1.0, 0.39225,  0.39225,  1.0};
            const GLfloat material_diffuse[]  = {1.0, 0.70754,  0.70754,  1.0};
            const GLfloat material_specular[] = {1.0, 0.708273, 0.708273, 1.0};
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

        // Display the current patient
        if (m_display_wireframe)
            m_female_icrp_phantom.displayWireframe();
        else
            m_female_icrp_phantom.displaySolid();

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


//---------------------------------------------------------
void AnatomicalDataHandler::displayPatient(GLint aShaderID)
//---------------------------------------------------------
{
    if (m_display_wireframe)
        m_female_icrp_phantom.displaySolid();
    else
        m_female_icrp_phantom.displayWireframe();
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
}

//---------------------------------------------
void AnatomicalDataHandler::renderToShadowMap()
//---------------------------------------------
{
    // Set the  matrix from the light's point of view to create the shadow map
/*    Matrix4x4<GLfloat> shadow_projection_matrix(buildOrthoProjectionMatrix(
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
*/
}
