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


#ifndef GVXR_SPHERE_MESH_H
#define GVXR_SPHERE_MESH_H


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
	//--------------------------------------------------------------------------
	SphereMesh(unsigned int aNumberOfRings = 10,
			unsigned int aNumberOfSectors = 10,
			float aRadius = 0.5 * cm);


	//--------------------------------------------------------------------------
	///	Create a new sphere.
	//--------------------------------------------------------------------------
	void create(unsigned int aNumberOfRings = 10,
			unsigned int aNumberOfSectors = 10,
			float aRadius = 0.5 * cm);
};


} // namespace gVirtualXRay


//******************************************************************************
#include "SphereMesh.inl"


#endif // SPHERE_MESH_H
