/**
********************************************************************************
*
*	@file		CubeMesh.h
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


#ifndef CUBE_MESH_H
#define CUBE_MESH_H


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
*	@class	CubeMesh
*	@brief	CubeMesh is a class to handle 3D meshes of cubes.
*/
//==============================================================================
class CubeMesh: public PolygonMesh
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	//--------------------------------------------------------------------------
	///	Default constructor.
	//--------------------------------------------------------------------------
	CubeMesh(float aLength = 1.0 * cm);


	//--------------------------------------------------------------------------
	///	Create a new sphere.
	//--------------------------------------------------------------------------
	void create(float aLength = 1.0 * cm);
};


} // namespace gVirtualXRay


//******************************************************************************
#include "CubeMesh.inl"


#endif // CUBE_MESH_H
