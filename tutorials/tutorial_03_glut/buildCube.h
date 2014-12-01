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
*				http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
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
