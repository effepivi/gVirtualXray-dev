/**
********************************************************************************
*
*	@file		VBO.cxx
*
*	@brief		Class to handle vertex buffer objects (VBOs).
*
*	@version	1.0
*
*	@date		07/11/2013
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
#ifdef __APPLE__
#define GL3_PROTOTYPES 1
#include <OpenGL/gl3.h>
#else
#define GL_GLEXT_PROTOTYPES 1
#include <GL/glcorearb.h>
#endif

#include <iostream>

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef GVXR_VBO_H
#include "gVirtualXRay/VBO.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//----------------------------------------------
VBO::VBO():
//----------------------------------------------
        m_number_of_normals(0),
        m_number_of_compontents_per_normal(0),
        m_number_of_texture_coordinates(0),
        m_number_of_compontents_per_texture_coord(0),
        m_number_of_vertices(0),
        m_number_of_compontents_per_vertex(0),
        m_number_of_indices(0),
        m_normal_size(0),
        m_texture_coordinate_size(0),
        m_vertex_size(0),
        m_index_size(0),
        m_normal_data_type(GL_NONE),
        m_texture_coordinate_data_type(GL_NONE),
        m_vertex_data_type(GL_NONE),
        m_index_data_type(GL_NONE),
        m_primitive_type(GL_NONE),
        m_storage_type(GL_DYNAMIC_DRAW)
//----------------------------------------------
{
    m_id_set[VERTEX_SET_ID]     = 0;
    m_id_set[TEXT_COORD_SET_ID] = 0;
    m_id_set[NORMAL_SET_ID]     = 0;
    m_id_set[INDEX_SET_ID]      = 0;
}


//-----------------
void VBO::destroy()
//-----------------
{
    if (m_id_set[VERTEX_SET_ID])     glDeleteBuffers(1, &m_id_set[VERTEX_SET_ID]);
    if (m_id_set[TEXT_COORD_SET_ID]) glDeleteBuffers(1, &m_id_set[TEXT_COORD_SET_ID]);
    if (m_id_set[NORMAL_SET_ID])     glDeleteBuffers(1, &m_id_set[NORMAL_SET_ID]);
    if (m_id_set[INDEX_SET_ID])      glDeleteBuffers(1, &m_id_set[INDEX_SET_ID]);

    m_id_set[VERTEX_SET_ID]     = 0;
    m_id_set[TEXT_COORD_SET_ID] = 0;
    m_id_set[NORMAL_SET_ID]     = 0;
    m_id_set[INDEX_SET_ID]      = 0;

    reset();
}


//---------------
void VBO::reset()
//---------------
{
    m_number_of_normals                       = 0;
    m_number_of_compontents_per_normal        = 0;

    m_number_of_texture_coordinates           = 0;
    m_number_of_compontents_per_texture_coord = 0;

    m_number_of_vertices                      = 0;
    m_number_of_compontents_per_vertex        = 0;

    m_number_of_indices                       = 0;

    m_normal_size             = 0;
    m_texture_coordinate_size = 0;
    m_vertex_size             = 0;
    m_index_size              = 0;

    m_normal_data_type             = GL_NONE;
    m_texture_coordinate_data_type = GL_NONE;
    m_vertex_data_type             = GL_NONE;
    m_index_data_type              = GL_NONE;

    m_primitive_type = GL_NONE;

    m_storage_type = GL_DYNAMIC_DRAW;
}
