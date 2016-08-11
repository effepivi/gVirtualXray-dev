/*

Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*				http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
*				BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifdef HAS_GLEW
#include "GL/glew.h"
#endif

#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
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
