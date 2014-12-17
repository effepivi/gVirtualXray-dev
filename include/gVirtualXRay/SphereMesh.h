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


#ifndef GVXR_SPHERE_MESH_H
#define GVXR_SPHERE_MESH_H


/**
********************************************************************************
*
*	@file		SphereMesh.h
*
*	@brief		Class to handle 3D meshes of spheres.
*
*	@version	1.0
*
*	@date		22/03/2014
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
//	Include
//******************************************************************************
#include <string>
#include <vector>

#ifndef GVXR_POLYGON_MESH_H
#include "gVirtualXRay/PolygonMesh.h"
#endif

#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*	@class	SphereMesh
*	@brief	SphereMesh is a class to handle 3D meshes of spheres.
*/
//==============================================================================
class SphereMesh: public PolygonMesh
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	//--------------------------------------------------------------------------
	///	Default constructor.
    /**
    *   @param aNumberOfRings   the number of rings (default value: 10)
    *   @param aNumberOfSectors the number of sectors (default value: 10)
    *   @param aRadius the radius (default value: 0.5 cm)
    */
    //--------------------------------------------------------------------------
	SphereMesh(unsigned int aNumberOfRings = 10,
			unsigned int aNumberOfSectors = 10,
			float aRadius = 0.5 * cm);


	//--------------------------------------------------------------------------
	///	Create a new sphere.
    /**
    *   @param aNumberOfRings   the number of rings (default value: 10)
    *   @param aNumberOfSectors the number of sectors (default value: 10)
    *   @param aRadius the radius (default value: 0.5 cm)
    */
	//--------------------------------------------------------------------------
	void create(unsigned int aNumberOfRings = 10,
			unsigned int aNumberOfSectors = 10,
			float aRadius = 0.5 * cm);
};


} // namespace gVirtualXRay


//******************************************************************************
#include "SphereMesh.inl"


#endif // SPHERE_MESH_H
