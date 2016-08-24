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

#include <QtGui>
#include <QGLWidget>

#ifndef GVXR_GL_WIDGET_H
#include "GLWidget.h"
#endif

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

#include "display_realistic_gl3.frag.h"
#include "display_realistic_gl3.vert.h"

//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;

//******************************************************************************
//	Implementation
//******************************************************************************

//----------------------------------
GLWidget::GLWidget(QWidget *parent):
//----------------------------------
    QGLWidget(parent),
	m_blue_cube_mesh(0.5 * cm),
	m_red_cube_mesh(1. * cm),
	m_light_direction(-1, 3, -0.5)
//----------------------------------
{
}


//----------------------------------
GLWidget::GLWidget(QGLFormat& aGLFormat, QWidget *parent):
//----------------------------------
    QGLWidget(aGLFormat, parent),
	m_blue_cube_mesh(0.5 * cm),
	m_red_cube_mesh(1. * cm),
	m_light_direction(-1., 3, -0.5)

//----------------------------------
{
}

//-------------------
GLWidget::~GLWidget()
//-------------------
{}

//---------------------------
void GLWidget::initializeGL()
//---------------------------
{
    // Initialise GLEW
    initialiseGLEW();
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Check the OpenGL version
    std::cout << "GL:\t" << glGetString(GL_VERSION) << std::endl;

    // Check the GLSL version
    std::cout << "GLSL:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // Initialise OpenGL
    std::cout << "OpenGL renderer:   " << glGetString(GL_RENDERER)   << std::endl;
    std::cout << "OpenGL version:    " << glGetString(GL_VERSION)    << std::endl;
    std::cout << "OpenGL vender:     " << glGetString(GL_VENDOR)     << std::endl;

    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Enable the Z-buffer
    glEnable(GL_DEPTH_TEST);
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
    
    // Decompress the shader using the ZLib
    // Modern OpenGL 3.2 or above is supported
    if (useOpenGL3_2OrAbove())
    {
        z_lib_return_code_vertex   = inflate(g_display_realistic_gl3_vert, sizeof(g_display_realistic_gl3_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_display_realistic_gl3_frag, sizeof(g_display_realistic_gl3_frag), &p_fragment_shader);
    }
    // Modern OpenGL 3.2 or above isnot supported
    else
    {
       /* z_lib_return_code_vertex   = inflate(g_display_gl2_vert, sizeof(g_display_gl2_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_display_gl2_frag, sizeof(g_display_gl2_frag), &p_fragment_shader);*/
    }

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

    // Initialise the modelling matrix
    m_scene_rotation_matrix *= Matrix4x4<GLfloat>::buildRotationMatrix(180.0, VEC3(0, 0, 1));
    m_scene_rotation_matrix *= Matrix4x4<GLfloat>::buildRotationMatrix( 60.0, VEC3(1, 0, 0));
    m_scene_rotation_matrix *= Matrix4x4<GLfloat>::buildRotationMatrix( 45.0, VEC3(0, 0, 1));

    //set the direction of the light
    m_light_direction.normalize();

    //set the meshes colours
    Material* material(&(m_blue_cube_mesh.getMaterial()));
    material->setAmbientColour(0.,0.,1.,1.);
    material->setDiffuseColour(0.,0.,1.,1.);
    material->setSpecularColour(0.5,0.5,0.5,1.);

    material = &(m_red_cube_mesh.getMaterial());
    material->setAmbientColour(1.,0.,0.,1.);
    material->setDiffuseColour(1.,0.,0.,1.);
    material->setSpecularColour(0.5,0.5,0.5,1.);

    m_blue_cube_mesh.getMaterial().setShininess(5.);
    m_red_cube_mesh.getMaterial().setShininess(5.);

    //set the translation matrixes of the meshes
    m_meshes_model_matrix.loadIdentity();

    m_blue_cube_mesh.translate(VEC3(-2.,-2.,7.));

    //translate the cube
    m_red_cube_mesh.translate(VEC3(0.,0.,-5.));

    m_blue_cube_mesh.splitFaces();
    m_blue_cube_mesh.splitFaces();
    m_blue_cube_mesh.splitFaces();
    m_blue_cube_mesh.splitFaces();

    m_red_cube_mesh.splitFaces();
    m_red_cube_mesh.splitFaces();
    m_red_cube_mesh.splitFaces();
    m_red_cube_mesh.splitFaces();

    // Set the background colour
    glClearColor(0.5, 0.5, 0.5, 1.0);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
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

    loadPerspectiveProjectionMatrix(45., screen_aspect_ratio, 1. * cm, 500. * cm);

    loadLookAtModelViewMatrix(0.0, 0.0, 5. * cm,
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);

    // Draw the 3D scene
    render();
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Restore the current transformation matrix
    popModelViewMatrix();
    popProjectionMatrix();

    // Make sure all the OpenGL code is done
    glFinish();
}

//---------------------------
void GLWidget::displayMesh(std::vector<PolygonMesh*> polygon_mesh_set, GLint aShaderID)
//---------------------------
{
	for(unsigned int i(0); i<polygon_mesh_set.size(); i++)
	{
	    // Get the material
	    Material& material(polygon_mesh_set[i]->getMaterial());

	    // Define colours inshader
	    GLuint handle(glGetUniformLocation(aShaderID, "material_ambient"));
	    glUniform4fv(handle, 1, &(material.getAmbientColour()[0]));
	    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	    handle = glGetUniformLocation(aShaderID, "material_diffuse");
	    glUniform4fv(handle, 1, &(material.getDiffuseColour()[0]));
	    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	    handle = glGetUniformLocation(aShaderID, "material_specular");
	    glUniform4fv(handle, 1, &(material.getSpecularColour()[0]));
	    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	    handle = glGetUniformLocation(aShaderID, "material_shininess");
	    glUniform1f(handle, material.getShininess());
	    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	    polygon_mesh_set[i]->display();
	}
}

//---------------------
void GLWidget::render()
//---------------------
{
    try
    {
    	std::vector<PolygonMesh*> polygon_mesh_set;
    	polygon_mesh_set.push_back(&m_blue_cube_mesh);
    	polygon_mesh_set.push_back(&m_red_cube_mesh);

    	//set the  matrix from the light's point of view to create the shadow map
    	gVirtualXRay::Matrix4x4<GLfloat> shadowProjectionMatrix(gVirtualXRay::buildOrthoProjectionMatrix(-10.f, 10.f, -10.f, 10.f, -10.f, 10.f));

    	gVirtualXRay::Matrix4x4<GLfloat> shadowViewMatrix(gVirtualXRay::buildLookAtModelViewMatrix(
    			m_light_direction,
    			gVirtualXRay::VEC3(0.f, 0.f, 0.f),
    			gVirtualXRay::VEC3(0.f, 1.f, 0.f)
    		));

    	shadowViewMatrix *= m_scene_rotation_matrix*m_meshes_model_matrix;
    	gVirtualXRay::Matrix4x4<GLfloat> shadow_matrix=shadowProjectionMatrix*shadowViewMatrix;

    	//render to the shadow map
        m_shadow_map.render(polygon_mesh_set, shadow_matrix);

    	//Restore the view point
    	glViewport(0, 0, width(), height());

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

        // Check the status of OpenGL and of the current FBO
        checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Rotate the sample
        g_current_modelview_matrix *= m_scene_rotation_matrix*m_meshes_model_matrix;

        // Handle for shader variables
    	GLuint handle(0);

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
        glUniform3f(handle, m_light_direction.getX(), m_light_direction.getY(), m_light_direction.getZ());
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

        //use lighting
        GLint lighting;
        lighting = 1;
        handle = glGetUniformLocation(shader_id,"g_use_lighting");
        glUniform1iv(handle, 1, &lighting);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        //Bind the shadow map texture in texture unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_shadow_map.getMapTexture());

        handle = glGetUniformLocation(shader_id, "g_shadow_map");
        glUniform1i(handle,0);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "g_shadow_map_matrix");
        glUniformMatrix4fv(handle, 1, GL_FALSE, shadow_matrix.get());
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        applyModelViewMatrix();

        // Display the polygon mesh set
        displayMesh(polygon_mesh_set, shader_id);

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
