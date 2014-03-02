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
*				http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
*				BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifndef BUILD_CUBE_H
#include "buildCube.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace Utilities;


//******************************************************************************
//	Defines
//******************************************************************************


//******************************************************************************
//	Constant variables
//******************************************************************************


//******************************************************************************
//	Global variables
//******************************************************************************


//******************************************************************************
//	Function declaration
//******************************************************************************


//-------------------------------------------------------
void buildCube(RATIONAL_NUMBER aLength,
			   VEC3 aCentre,
			   std::vector<RATIONAL_NUMBER>& apVertexSet)
//-------------------------------------------------------
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
