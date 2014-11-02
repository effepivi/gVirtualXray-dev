/**
********************************************************************************
*
*	@file		buildCube.h
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


#ifndef GVXR_BUILD_CUBE_H
#define GVXR_BUILD_CUBE_H


//******************************************************************************
//	Include
//******************************************************************************
#include <vector>

#ifndef GVXR_GVXR_TYPES_H
#include <gVirtualXRay/Types.h>
#endif


//******************************************************************************
//	function declaration
//******************************************************************************


//------------------------------------------------------------------------------
///	Create a cube.
/**
*	@param 	aLenght:	the length of the cube
*	@param 	aCentre:	the centre of the cube
*	@param 	apVertexSet:	the vertices of the cube
*/
//------------------------------------------------------------------------------
void buildCube(gVirtualXRay::RATIONAL_NUMBER aLength,
		gVirtualXRay::VEC3 aCentre,
		std::vector<double>& apVertexSet);


//------------------------------------------------------------------------------
///	Create a cube.
/**
*	@param 	aLenght:	the length of the cube
*	@param 	aCentre:	the centre of the cube
*	@param 	apVertexSet:	the vertices of the cube
*/
//------------------------------------------------------------------------------
void buildCube(gVirtualXRay::RATIONAL_NUMBER aLength,
		gVirtualXRay::VEC3 aCentre,
		std::vector<double>& apVertexSet,
		std::vector<unsigned char>& apIndexSet);


//------------------------------------------------------------------------------
///	Create a cube.
/**
*	@param 	aLenght:	the length of the cube
*	@param 	aCentre:	the centre of the cube
*	@param 	apVertexSet:	the vertices of the cube
*/
//------------------------------------------------------------------------------
void buildCube(gVirtualXRay::RATIONAL_NUMBER aLength,
		gVirtualXRay::VEC3 aCentre,
		std::vector<float>& apVertexSet);


//------------------------------------------------------------------------------
///	Create a cube.
/**
*	@param 	aLenght:	the length of the cube
*	@param 	aCentre:	the centre of the cube
*	@param 	apVertexSet:	the vertices of the cube
*/
//------------------------------------------------------------------------------
void buildCube(gVirtualXRay::RATIONAL_NUMBER aLength,
		gVirtualXRay::VEC3 aCentre,
		std::vector<float>& apVertexSet,
		std::vector<unsigned char>& apIndexSet);


#endif // BUILD_CUBE_H
