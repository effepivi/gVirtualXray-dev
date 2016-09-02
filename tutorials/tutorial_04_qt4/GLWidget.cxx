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

#include "P4_lung.stl.h"

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
    QGLWidget(parent)
//----------------------------------
{
}


//--------------------------------------
GLWidget::GLWidget(QGLFormat& aGLFormat,
                   QWidget *parent):
//--------------------------------------
    QGLWidget(aGLFormat, parent)
//--------------------------------------
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

    // Set the background colour
    glClearColor(0.5, 0.5, 0.5, 1.0);
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

    // load the geometry
    loadSTLFile();

    // Initialise the modelling matrix
    m_scene_rotation_matrix *= Matrix4x4<GLfloat>::buildRotationMatrix(180.0, VEC3(0, 0, 1));
    m_scene_rotation_matrix *= Matrix4x4<GLfloat>::buildRotationMatrix( 60.0, VEC3(1, 0, 0));
}


//----------------------------------------------
void GLWidget::keyPressEvent(QKeyEvent* apEvent)
//----------------------------------------------
{
    switch(apEvent->key())
    {
    // Close the program
    case Qt::Key_Q:
    case Qt::Key_Escape:
        close();
        break;

    default:
    std::cout << apEvent->key() << std::endl;
        break;
    }
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

    loadPerspectiveProjectionMatrix(45., screen_aspect_ratio, 5. * cm, 500. * cm);

    loadLookAtModelViewMatrix(0.0, 0.0, 35. * cm,
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


//--------------------------
void GLWidget::loadSTLFile()
//--------------------------
{
	// Set geometry
	char* p_data(0);

    int aSize(0);
    aSize = inflate(g_P4_lung_stl, sizeof(g_P4_lung_stl), &p_data);

    if (aSize <= 0)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode using ZLib.");
    }

	m_lung_mesh.loadSTLDataFromStream(p_data, false, true, false, true, mm, GL_STATIC_DRAW);

	// Delete data
	if (p_data)
	{
		delete [] p_data;
		p_data = 0;
	}

	// Compute the translation vector
	VEC3 lung_translation_vector = m_lung_mesh.getMinCorner() + (m_lung_mesh.getMaxCorner() - m_lung_mesh.getMinCorner()) / 2.0;
	// compute the lung model matrix
	m_lung_rotation_matrix = Matrix4x4<GLfloat>::buildRotationMatrix(180, VEC3(1, 0, 0));
	m_lung_rotation_matrix *= Matrix4x4<GLfloat>::buildTranslationMatrix(-lung_translation_vector);

	m_lung_mesh.initialize();
}

//---------------------
void GLWidget::render()
//---------------------
{
    try
    {
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
        g_current_modelview_matrix *= m_scene_rotation_matrix;
        g_current_modelview_matrix *= m_lung_rotation_matrix;

        // Handle for shader variables
    	GLuint handle(0);

    	handle = glGetUniformLocation(shader_id, "g_projection_matrix");
        glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_projection_matrix.get());
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "g_modelview_matrix");
        glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_modelview_matrix.get());
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        //Don't use lighting
        GLint lighting;
        lighting = 0;
        handle = glGetUniformLocation(shader_id,"g_use_lighting");
        glUniform1iv(handle, 1, &lighting);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        m_lung_mesh.defineDisplayShaderInput(shader_id);

        // Display the current patient
   		m_lung_mesh.display();

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
