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
*	@file		buildCube.cxx
*
*	@brief		Generate the triangle mesh corresponding to a cube.
*
*	@version	1.0
*
*	@date		25/02/2014
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
#ifndef GVXR_BUILD_CUBE_H
#include "buildCube.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//----------------------------------------------
void buildCube(RATIONAL_NUMBER aLength,
			   VEC3 aCentre,
			   std::vector<double>& apVertexSet)
//----------------------------------------------
{
	// Clear the vertex set
	apVertexSet.clear();

	// Cache the half length
	RATIONAL_NUMBER half_length(aLength / 2.0);

	// Find the 8 corners

	// Top
	VEC3 top_far_left(  aCentre.getX() - half_length, aCentre.getY() + half_length, aCentre.getZ() - half_length);
	VEC3 top_near_left( aCentre.getX() - half_length, aCentre.getY() + half_length, aCentre.getZ() + half_length);
	VEC3 top_far_right( aCentre.getX() + half_length, aCentre.getY() + half_length, aCentre.getZ() - half_length);
	VEC3 top_near_right(aCentre.getX() + half_length, aCentre.getY() + half_length, aCentre.getZ() + half_length);

	// Bottom
	VEC3 bottom_far_left(  aCentre.getX() - half_length, aCentre.getY() - half_length, aCentre.getZ() - half_length);
	VEC3 bottom_near_left( aCentre.getX() - half_length, aCentre.getY() - half_length, aCentre.getZ() + half_length);
	VEC3 bottom_far_right( aCentre.getX() + half_length, aCentre.getY() - half_length, aCentre.getZ() - half_length);
	VEC3 bottom_near_right(aCentre.getX() + half_length, aCentre.getY() - half_length, aCentre.getZ() + half_length);

	// Store the current vertex
	VEC3 current_vertex;

	// Top face
	current_vertex = top_near_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_far_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_far_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_far_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_near_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_near_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	// Bottom face
	current_vertex = bottom_far_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_near_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_far_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_near_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_far_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_near_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	// Front face
	current_vertex = bottom_near_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_near_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_near_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_near_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_near_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_near_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	// Back face
	current_vertex = top_far_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_far_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_far_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_far_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_far_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_far_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	// Left face
	current_vertex = bottom_near_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_far_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_far_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_far_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_near_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_near_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	// Right face
	current_vertex = top_far_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_near_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_far_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_near_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_far_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_near_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());
}


//---------------------------------------------------
void buildCube(gVirtualXRay::RATIONAL_NUMBER aLength,
			   gVirtualXRay::VEC3 aCentre,
			   std::vector<double>& apVertexSet,
			   std::vector<unsigned char>& apIndexSet)
//---------------------------------------------------
{
	// Clear the vertex set
	apVertexSet.clear();

	// Clear the index set
	apIndexSet.clear();

	// Cache the half length
	RATIONAL_NUMBER half_length(aLength / 2.0);

	// Find the 8 corners

	// Top
	VEC3 top_far_left(  aCentre.getX() - half_length, aCentre.getY() + half_length, aCentre.getZ() - half_length);
	VEC3 top_near_left( aCentre.getX() - half_length, aCentre.getY() + half_length, aCentre.getZ() + half_length);
	VEC3 top_far_right( aCentre.getX() + half_length, aCentre.getY() + half_length, aCentre.getZ() - half_length);
	VEC3 top_near_right(aCentre.getX() + half_length, aCentre.getY() + half_length, aCentre.getZ() + half_length);

	// Bottom
	VEC3 bottom_far_left(  aCentre.getX() - half_length, aCentre.getY() - half_length, aCentre.getZ() - half_length);
	VEC3 bottom_near_left( aCentre.getX() - half_length, aCentre.getY() - half_length, aCentre.getZ() + half_length);
	VEC3 bottom_far_right( aCentre.getX() + half_length, aCentre.getY() - half_length, aCentre.getZ() - half_length);
	VEC3 bottom_near_right(aCentre.getX() + half_length, aCentre.getY() - half_length, aCentre.getZ() + half_length);

	// Store vertices
	apVertexSet.push_back(top_far_left.getX());
	apVertexSet.push_back(top_far_left.getY());
	apVertexSet.push_back(top_far_left.getZ());

	apVertexSet.push_back(top_near_left.getX());
	apVertexSet.push_back(top_near_left.getY());
	apVertexSet.push_back(top_near_left.getZ());

	apVertexSet.push_back(top_far_right.getX());
	apVertexSet.push_back(top_far_right.getY());
	apVertexSet.push_back(top_far_right.getZ());

	apVertexSet.push_back(top_near_right.getX());
	apVertexSet.push_back(top_near_right.getY());
	apVertexSet.push_back(top_near_right.getZ());

	apVertexSet.push_back(bottom_far_left.getX());
	apVertexSet.push_back(bottom_far_left.getY());
	apVertexSet.push_back(bottom_far_left.getZ());

	apVertexSet.push_back(bottom_near_left.getX());
	apVertexSet.push_back(bottom_near_left.getY());
	apVertexSet.push_back(bottom_near_left.getZ());

	apVertexSet.push_back(bottom_far_right.getX());
	apVertexSet.push_back(bottom_far_right.getY());
	apVertexSet.push_back(bottom_far_right.getZ());

	apVertexSet.push_back(bottom_near_right.getX());
	apVertexSet.push_back(bottom_near_right.getY());
	apVertexSet.push_back(bottom_near_right.getZ());

	// Store the current vertex
	VEC3 current_vertex;

	// Top face
	apIndexSet.push_back(1);
	apIndexSet.push_back(2);
	apIndexSet.push_back(0);

	apIndexSet.push_back(2);
	apIndexSet.push_back(1);
	apIndexSet.push_back(3);

	// Bottom face
	apIndexSet.push_back(6);
	apIndexSet.push_back(5);
	apIndexSet.push_back(4);

	apIndexSet.push_back(5);
	apIndexSet.push_back(6);
	apIndexSet.push_back(7);

	// Front face
	apIndexSet.push_back(5);
	apIndexSet.push_back(3);
	apIndexSet.push_back(1);

	apIndexSet.push_back(3);
	apIndexSet.push_back(5);
	apIndexSet.push_back(7);

	// Back face
	apIndexSet.push_back(2);
	apIndexSet.push_back(4);
	apIndexSet.push_back(0);

	apIndexSet.push_back(4);
	apIndexSet.push_back(2);
	apIndexSet.push_back(6);

	// Left face
	apIndexSet.push_back(5);
	apIndexSet.push_back(0);
	apIndexSet.push_back(4);

	apIndexSet.push_back(0);
	apIndexSet.push_back(5);
	apIndexSet.push_back(1);

	// Right face
	apIndexSet.push_back(2);
	apIndexSet.push_back(7);
	apIndexSet.push_back(6);

	apIndexSet.push_back(7);
	apIndexSet.push_back(2);
	apIndexSet.push_back(3);
}


//----------------------------------------------
void buildCube(RATIONAL_NUMBER aLength,
			   VEC3 aCentre,
			   std::vector<float>& apVertexSet)
//----------------------------------------------
{
	// Clear the vertex set
	apVertexSet.clear();

	// Cache the half length
	RATIONAL_NUMBER half_length(aLength / 2.0);

	// Find the 8 corners

	// Top
	VEC3 top_far_left(  aCentre.getX() - half_length, aCentre.getY() + half_length, aCentre.getZ() - half_length);
	VEC3 top_near_left( aCentre.getX() - half_length, aCentre.getY() + half_length, aCentre.getZ() + half_length);
	VEC3 top_far_right( aCentre.getX() + half_length, aCentre.getY() + half_length, aCentre.getZ() - half_length);
	VEC3 top_near_right(aCentre.getX() + half_length, aCentre.getY() + half_length, aCentre.getZ() + half_length);

	// Bottom
	VEC3 bottom_far_left(  aCentre.getX() - half_length, aCentre.getY() - half_length, aCentre.getZ() - half_length);
	VEC3 bottom_near_left( aCentre.getX() - half_length, aCentre.getY() - half_length, aCentre.getZ() + half_length);
	VEC3 bottom_far_right( aCentre.getX() + half_length, aCentre.getY() - half_length, aCentre.getZ() - half_length);
	VEC3 bottom_near_right(aCentre.getX() + half_length, aCentre.getY() - half_length, aCentre.getZ() + half_length);

	// Store the current vertex
	VEC3 current_vertex;

	// Top face
	current_vertex = top_near_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_far_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_far_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_far_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_near_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_near_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	// Bottom face
	current_vertex = bottom_far_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_near_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_far_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_near_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_far_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_near_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	// Front face
	current_vertex = bottom_near_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_near_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_near_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_near_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_near_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_near_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	// Back face
	current_vertex = top_far_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_far_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_far_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_far_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_far_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_far_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	// Left face
	current_vertex = bottom_near_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_far_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_far_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_far_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_near_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_near_left;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	// Right face
	current_vertex = top_far_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_near_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_far_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = bottom_near_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_far_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());

	current_vertex = top_near_right;
	apVertexSet.push_back(current_vertex.getX());
	apVertexSet.push_back(current_vertex.getY());
	apVertexSet.push_back(current_vertex.getZ());
}


//---------------------------------------------------
void buildCube(gVirtualXRay::RATIONAL_NUMBER aLength,
			   gVirtualXRay::VEC3 aCentre,
			   std::vector<float>& apVertexSet,
			   std::vector<unsigned char>& apIndexSet)
//---------------------------------------------------
{
	// Clear the vertex set
	apVertexSet.clear();

	// Clear the index set
	apIndexSet.clear();

	// Cache the half length
	RATIONAL_NUMBER half_length(aLength / 2.0);

	// Find the 8 corners

	// Top
	VEC3 top_far_left(  aCentre.getX() - half_length, aCentre.getY() + half_length, aCentre.getZ() - half_length);
	VEC3 top_near_left( aCentre.getX() - half_length, aCentre.getY() + half_length, aCentre.getZ() + half_length);
	VEC3 top_far_right( aCentre.getX() + half_length, aCentre.getY() + half_length, aCentre.getZ() - half_length);
	VEC3 top_near_right(aCentre.getX() + half_length, aCentre.getY() + half_length, aCentre.getZ() + half_length);

	// Bottom
	VEC3 bottom_far_left(  aCentre.getX() - half_length, aCentre.getY() - half_length, aCentre.getZ() - half_length);
	VEC3 bottom_near_left( aCentre.getX() - half_length, aCentre.getY() - half_length, aCentre.getZ() + half_length);
	VEC3 bottom_far_right( aCentre.getX() + half_length, aCentre.getY() - half_length, aCentre.getZ() - half_length);
	VEC3 bottom_near_right(aCentre.getX() + half_length, aCentre.getY() - half_length, aCentre.getZ() + half_length);

	// Store vertices
	apVertexSet.push_back(top_far_left.getX());
	apVertexSet.push_back(top_far_left.getY());
	apVertexSet.push_back(top_far_left.getZ());

	apVertexSet.push_back(top_near_left.getX());
	apVertexSet.push_back(top_near_left.getY());
	apVertexSet.push_back(top_near_left.getZ());

	apVertexSet.push_back(top_far_right.getX());
	apVertexSet.push_back(top_far_right.getY());
	apVertexSet.push_back(top_far_right.getZ());

	apVertexSet.push_back(top_near_right.getX());
	apVertexSet.push_back(top_near_right.getY());
	apVertexSet.push_back(top_near_right.getZ());

	apVertexSet.push_back(bottom_far_left.getX());
	apVertexSet.push_back(bottom_far_left.getY());
	apVertexSet.push_back(bottom_far_left.getZ());

	apVertexSet.push_back(bottom_near_left.getX());
	apVertexSet.push_back(bottom_near_left.getY());
	apVertexSet.push_back(bottom_near_left.getZ());

	apVertexSet.push_back(bottom_far_right.getX());
	apVertexSet.push_back(bottom_far_right.getY());
	apVertexSet.push_back(bottom_far_right.getZ());

	apVertexSet.push_back(bottom_near_right.getX());
	apVertexSet.push_back(bottom_near_right.getY());
	apVertexSet.push_back(bottom_near_right.getZ());

	// Store the current vertex
	VEC3 current_vertex;

	// Top face
	apIndexSet.push_back(1);
	apIndexSet.push_back(2);
	apIndexSet.push_back(0);

	apIndexSet.push_back(2);
	apIndexSet.push_back(1);
	apIndexSet.push_back(3);

	// Bottom face
	apIndexSet.push_back(6);
	apIndexSet.push_back(5);
	apIndexSet.push_back(4);

	apIndexSet.push_back(5);
	apIndexSet.push_back(6);
	apIndexSet.push_back(7);

	// Front face
	apIndexSet.push_back(5);
	apIndexSet.push_back(3);
	apIndexSet.push_back(1);

	apIndexSet.push_back(3);
	apIndexSet.push_back(5);
	apIndexSet.push_back(7);

	// Back face
	apIndexSet.push_back(2);
	apIndexSet.push_back(4);
	apIndexSet.push_back(0);

	apIndexSet.push_back(4);
	apIndexSet.push_back(2);
	apIndexSet.push_back(6);

	// Left face
	apIndexSet.push_back(5);
	apIndexSet.push_back(0);
	apIndexSet.push_back(4);

	apIndexSet.push_back(0);
	apIndexSet.push_back(5);
	apIndexSet.push_back(1);

	// Right face
	apIndexSet.push_back(2);
	apIndexSet.push_back(7);
	apIndexSet.push_back(6);

	apIndexSet.push_back(7);
	apIndexSet.push_back(2);
	apIndexSet.push_back(3);
}
