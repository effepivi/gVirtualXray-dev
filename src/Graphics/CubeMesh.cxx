/**
********************************************************************************
*
*	@file		CubeMesh.cxx
*
*	@brief		Class to handle 3D meshes of cubes.
*
*	@version	1.0
*
*	@date		23/03/2014
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
#ifdef HAS_OPENGL2
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#endif

#ifdef HAS_OPENGL3
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glcorearb.h>
#endif
#endif

#ifndef GVXR_CUBE_MESH_H
#include "gVirtualXRay/CubeMesh.h"
#endif

#ifndef GVXR_OUT_OF_MEMORY_EXCEPTION_H
#include "gVirtualXRay/OutOfMemoryException.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//	Constant variables
//******************************************************************************


//******************************************************************************
//	Global variables
//******************************************************************************


//******************************************************************************
//	Function declaration
//******************************************************************************


//----------------------------------
void CubeMesh::create(float aLength)
//----------------------------------
{
	// Reset the mesh
	reset();
	m_external_data_flag = false;

    GLfloat* p_vertices = new GLfloat[8 * 3];
    if (!p_vertices)
    {
    	throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
    }
    m_number_of_vertices = 8;
    m_p_vertex_set = p_vertices;
    m_vertex_data_type = GL_FLOAT;

    m_p_vertex_normal_set.resize(m_number_of_vertices * 3);

    GLubyte* p_indices = new GLubyte[6 * 6];
    if (!p_indices)
    {
    	throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
    }
    m_number_of_indices = 6 * 6;
    m_p_index_set = p_indices;
    m_index_data_type = GL_UNSIGNED_BYTE;

	// Set the kind of polygon
	m_polygon_type = GL_TRIANGLES;

    // Cache the half length
	RATIONAL_NUMBER half_length(aLength / 2.0);

	// Find the 8 corners

	// Store the current vertex
	VEC3 current_vertex;
	VEC3 normal_vector;

	// Top
	VEC3 top_near_left(-half_length, half_length,  half_length);
	current_vertex = top_near_left;
	p_vertices[ 0 * 3 + 0] = current_vertex.getX();
	p_vertices[ 0 * 3 + 1] = current_vertex.getY();
	p_vertices[ 0 * 3 + 2] = current_vertex.getZ();

	normal_vector = current_vertex.normal();
	m_p_vertex_normal_set[0 * 3 + 0] = normal_vector.getX();
	m_p_vertex_normal_set[0 * 3 + 1] = normal_vector.getY();
	m_p_vertex_normal_set[0 * 3 + 2] = normal_vector.getZ();

	VEC3 top_near_right(half_length, half_length,  half_length);
	current_vertex = top_near_right;
	p_vertices[ 1 * 3 + 0] = current_vertex.getX();
	p_vertices[ 1 * 3 + 1] = current_vertex.getY();
	p_vertices[ 1 * 3 + 2] = current_vertex.getZ();

	normal_vector = current_vertex.normal();
	m_p_vertex_normal_set[1 * 3 + 0] = normal_vector.getX();
	m_p_vertex_normal_set[1 * 3 + 1] = normal_vector.getY();
	m_p_vertex_normal_set[1 * 3 + 2] = normal_vector.getZ();

	VEC3 top_far_right( half_length, half_length, -half_length);
	current_vertex = top_far_right;
	p_vertices[ 2 * 3 + 0] = current_vertex.getX();
	p_vertices[ 2 * 3 + 1] = current_vertex.getY();
	p_vertices[ 2 * 3 + 2] = current_vertex.getZ();

	normal_vector = current_vertex.normal();
	m_p_vertex_normal_set[2 * 3 + 0] = normal_vector.getX();
	m_p_vertex_normal_set[2 * 3 + 1] = normal_vector.getY();
	m_p_vertex_normal_set[2 * 3 + 2] = normal_vector.getZ();

	VEC3 top_far_left( -half_length, half_length, -half_length);
	current_vertex = top_far_left;
	p_vertices[ 3 * 3 + 0] = current_vertex.getX();
	p_vertices[ 3 * 3 + 1] = current_vertex.getY();
	p_vertices[ 3 * 3 + 2] = current_vertex.getZ();

	normal_vector = current_vertex.normal();
	m_p_vertex_normal_set[3 * 3 + 0] = normal_vector.getX();
	m_p_vertex_normal_set[3 * 3 + 1] = normal_vector.getY();
	m_p_vertex_normal_set[3 * 3 + 2] = normal_vector.getZ();

	// Bottom
	VEC3 bottom_near_left(-half_length, -half_length,  half_length);
	current_vertex = bottom_near_left;
	p_vertices[ 4 * 3 + 0] = current_vertex.getX();
	p_vertices[ 4 * 3 + 1] = current_vertex.getY();
	p_vertices[ 4 * 3 + 2] = current_vertex.getZ();

	normal_vector = current_vertex.normal();
	m_p_vertex_normal_set[4 * 3 + 0] = normal_vector.getX();
	m_p_vertex_normal_set[4 * 3 + 1] = normal_vector.getY();
	m_p_vertex_normal_set[4 * 3 + 2] = normal_vector.getZ();

	VEC3 bottom_near_right(half_length, -half_length,  half_length);
	current_vertex = bottom_near_right;
	p_vertices[ 5 * 3 + 0] = current_vertex.getX();
	p_vertices[ 5 * 3 + 1] = current_vertex.getY();
	p_vertices[ 5 * 3 + 2] = current_vertex.getZ();

	normal_vector = current_vertex.normal();
	m_p_vertex_normal_set[5 * 3 + 0] = normal_vector.getX();
	m_p_vertex_normal_set[5 * 3 + 1] = normal_vector.getY();
	m_p_vertex_normal_set[5 * 3 + 2] = normal_vector.getZ();

	VEC3 bottom_far_right( half_length, -half_length, -half_length);
	current_vertex = bottom_far_right;
	p_vertices[ 6 * 3 + 0] = current_vertex.getX();
	p_vertices[ 6 * 3 + 1] = current_vertex.getY();
	p_vertices[ 6 * 3 + 2] = current_vertex.getZ();

	normal_vector = current_vertex.normal();
	m_p_vertex_normal_set[6 * 3 + 0] = normal_vector.getX();
	m_p_vertex_normal_set[6 * 3 + 1] = normal_vector.getY();
	m_p_vertex_normal_set[6 * 3 + 2] = normal_vector.getZ();

	VEC3 bottom_far_left( -half_length, -half_length, -half_length);
	current_vertex = bottom_far_left;
	p_vertices[ 7 * 3 + 0] = current_vertex.getX();
	p_vertices[ 7 * 3 + 1] = current_vertex.getY();
	p_vertices[ 7 * 3 + 2] = current_vertex.getZ();

	normal_vector = current_vertex.normal();
	m_p_vertex_normal_set[7 * 3 + 0] = normal_vector.getX();
	m_p_vertex_normal_set[7 * 3 + 1] = normal_vector.getY();
	m_p_vertex_normal_set[7 * 3 + 2] = normal_vector.getZ();

	// Top face
	p_indices[0 * 6 + 0] = 0;
	p_indices[0 * 6 + 1] = 1;
	p_indices[0 * 6 + 2] = 2;
	p_indices[0 * 6 + 3] = 0;
	p_indices[0 * 6 + 4] = 2;
    p_indices[0 * 6 + 5] = 3;

	// Bottom face
	p_indices[1 * 6 + 0] = 7;
	p_indices[1 * 6 + 1] = 6;
	p_indices[1 * 6 + 2] = 5;
	p_indices[1 * 6 + 3] = 7;
	p_indices[1 * 6 + 4] = 5;
	p_indices[1 * 6 + 5] = 4;

	// Front face
	p_indices[2 * 6 + 0] = 4;
	p_indices[2 * 6 + 1] = 5;
	p_indices[2 * 6 + 2] = 1;
	p_indices[2 * 6 + 3] = 4;
	p_indices[2 * 6 + 4] = 1;
	p_indices[2 * 6 + 5] = 0;

	// Back face
	p_indices[3 * 6 + 0] = 6;
	p_indices[3 * 6 + 1] = 7;
	p_indices[3 * 6 + 2] = 3;
	p_indices[3 * 6 + 3] = 6;
	p_indices[3 * 6 + 4] = 3;
	p_indices[3 * 6 + 5] = 2;

	// Left face
	p_indices[4 * 6 + 0] = 4;
	p_indices[4 * 6 + 1] = 0;
	p_indices[4 * 6 + 2] = 3;
	p_indices[4 * 6 + 3] = 4;
	p_indices[4 * 6 + 4] = 3;
	p_indices[4 * 6 + 5] = 7;

	// Right face
	p_indices[5 * 6 + 0] = 5;
	p_indices[5 * 6 + 1] = 6;
	p_indices[5 * 6 + 2] = 2;
	p_indices[5 * 6 + 3] = 5;
	p_indices[5 * 6 + 4] = 2;
	p_indices[5 * 6 + 5] = 1;
}
