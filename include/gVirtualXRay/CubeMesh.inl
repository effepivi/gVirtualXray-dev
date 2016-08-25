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
*	@file		CubeMesh.inl
*
*	@brief		Class to handle 3D meshes of cubes.
*
*	@version	1.0
*
*	@date		23/03/2014
*
*	@author		Dr Franck P. Vidal
*
*	@todo		Make it possible to merge vertices if an array of indices exists
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net), 
*   http://www.fpvidal.net/, Dec 2014, 2014, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//---------------------------------------------------------
template<typename T> CubeMesh<T>::CubeMesh(double aLength):
//---------------------------------------------------------
		PolygonMesh()
//---------------------------------------------------------
{
	create(aLength);
}


//-----------------------------------------------------------
template<typename T> void CubeMesh<T>::create(double aLength)
//-----------------------------------------------------------
{
    // Reset the mesh
    reset();
    m_external_data_flag = false;

    // Cache the half length
    double half_length(aLength / 2.0);

    // Find the 8 corners

    // Top
    VEC3 top_far_left(  -half_length, half_length, -half_length);
    VEC3 top_near_left( -half_length, half_length,  half_length);
    VEC3 top_far_right(  half_length, half_length, -half_length);
    VEC3 top_near_right( half_length, half_length,  half_length);

    // Bottom
    VEC3 bottom_far_left(  -half_length, -half_length, -half_length);
    VEC3 bottom_near_left( -half_length, -half_length,  half_length);
    VEC3 bottom_far_right(  half_length, -half_length, -half_length);
    VEC3 bottom_near_right( half_length, -half_length,  half_length);

    // Store vertices
    std::vector<T> p_vertex_set;
    p_vertex_set.push_back(top_far_left.getX());
    p_vertex_set.push_back(top_far_left.getY());
    p_vertex_set.push_back(top_far_left.getZ());

    p_vertex_set.push_back(top_near_left.getX());
    p_vertex_set.push_back(top_near_left.getY());
    p_vertex_set.push_back(top_near_left.getZ());

    p_vertex_set.push_back(top_far_right.getX());
    p_vertex_set.push_back(top_far_right.getY());
    p_vertex_set.push_back(top_far_right.getZ());

    p_vertex_set.push_back(top_near_right.getX());
    p_vertex_set.push_back(top_near_right.getY());
    p_vertex_set.push_back(top_near_right.getZ());

    p_vertex_set.push_back(bottom_far_left.getX());
    p_vertex_set.push_back(bottom_far_left.getY());
    p_vertex_set.push_back(bottom_far_left.getZ());

    p_vertex_set.push_back(bottom_near_left.getX());
    p_vertex_set.push_back(bottom_near_left.getY());
    p_vertex_set.push_back(bottom_near_left.getZ());

    p_vertex_set.push_back(bottom_far_right.getX());
    p_vertex_set.push_back(bottom_far_right.getY());
    p_vertex_set.push_back(bottom_far_right.getZ());

    p_vertex_set.push_back(bottom_near_right.getX());
    p_vertex_set.push_back(bottom_near_right.getY());
    p_vertex_set.push_back(bottom_near_right.getZ());

    // Store the current vertex
    VEC3 current_vertex;

    // Top face
    std::vector<GLubyte> p_index_set;

    p_index_set.push_back(1);
    p_index_set.push_back(2);
    p_index_set.push_back(0);

    p_index_set.push_back(2);
    p_index_set.push_back(1);
    p_index_set.push_back(3);

    // Bottom face
    p_index_set.push_back(6);
    p_index_set.push_back(5);
    p_index_set.push_back(4);

    p_index_set.push_back(5);
    p_index_set.push_back(6);
    p_index_set.push_back(7);

    // Front face
    p_index_set.push_back(5);
    p_index_set.push_back(3);
    p_index_set.push_back(1);

    p_index_set.push_back(3);
    p_index_set.push_back(5);
    p_index_set.push_back(7);

    // Back face
    p_index_set.push_back(2);
    p_index_set.push_back(4);
    p_index_set.push_back(0);

    p_index_set.push_back(4);
    p_index_set.push_back(2);
    p_index_set.push_back(6);

    // Left face
    p_index_set.push_back(5);
    p_index_set.push_back(0);
    p_index_set.push_back(4);

    p_index_set.push_back(0);
    p_index_set.push_back(5);
    p_index_set.push_back(1);

    // Right face
    p_index_set.push_back(2);
    p_index_set.push_back(7);
    p_index_set.push_back(6);

    p_index_set.push_back(7);
    p_index_set.push_back(2);
    p_index_set.push_back(3);

    setInternalData(GL_TRIANGLES,
            &p_vertex_set,
            &p_index_set,
            false,
            GL_STATIC_DRAW);
}

} // namespace gVirtualXRay
