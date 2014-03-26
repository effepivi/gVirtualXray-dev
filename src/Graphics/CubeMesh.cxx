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
#ifndef CUBE_MESH_H
#include "gVirtualXRay/CubeMesh.h"
#endif

#ifndef OUT_OF_MEMORY_EXCEPTION_H
#include "gVirtualXRay/OutOfMemoryException.h"
#endif

#include <GL/glew.h>


//******************************************************************************
//	Name space
//******************************************************************************
using namespace Graphics;


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

    GLubyte* p_indices = new GLubyte[4 * 6];
    if (!p_indices)
    {
    	throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
    }
    m_number_of_indices = 4 * 6;
    m_p_index_set = p_indices;
    m_index_data_type = GL_UNSIGNED_BYTE;

	// Set the kind of polygon
	m_polygon_type = GL_QUADS;

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
	p_indices[0 * 4 + 0] = 0;
	p_indices[0 * 4 + 1] = 1;
	p_indices[0 * 4 + 2] = 2;
	p_indices[0 * 4 + 3] = 3;

	// Bottom face
	p_indices[1 * 4 + 0] = 7;
	p_indices[1 * 4 + 1] = 6;
	p_indices[1 * 4 + 2] = 5;
	p_indices[1 * 4 + 3] = 4;

	// Front face
	p_indices[2 * 4 + 0] = 4;
	p_indices[2 * 4 + 1] = 5;
	p_indices[2 * 4 + 2] = 1;
	p_indices[2 * 4 + 3] = 0;

	// Back face
	p_indices[3 * 4 + 0] = 6;
	p_indices[3 * 4 + 1] = 7;
	p_indices[3 * 4 + 2] = 3;
	p_indices[3 * 4 + 3] = 2;

	// Left face
	p_indices[4 * 4 + 0] = 4;
	p_indices[4 * 4 + 1] = 0;
	p_indices[4 * 4 + 2] = 3;
	p_indices[4 * 4 + 3] = 7;

	// Right face
	p_indices[5 * 4 + 0] = 5;
	p_indices[5 * 4 + 1] = 6;
	p_indices[5 * 4 + 2] = 2;
	p_indices[5 * 4 + 3] = 1;
}
