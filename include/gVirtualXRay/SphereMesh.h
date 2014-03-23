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


#ifndef SPHERE_MESH_H
#define SPHERE_MESH_H


//******************************************************************************
//	Include
//******************************************************************************
#include <string>
#include <vector>

#ifndef TRIANGLE_MESH_H
#include "gVirtualXRay/TriangleMesh.h"
#endif

#ifndef TYPES_H
#include "gVirtualXRay/Types.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace Graphics {
	using namespace Utilities;


//==============================================================================
/**
*	@class	SphereMesh
*	@brief	SphereMesh is a class to handle 3D meshes of spheres.
*/
//==============================================================================
class SphereMesh: public TriangleMesh
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


	//--------------------------------------------------------------------------
	///	Update the VBO.
	/**
	*	@param aTypeOfVBO: the VBO usage hint to set; valid values are GL_STATIC_DRAW, GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	*/
	//--------------------------------------------------------------------------
	void updateVBO(int aTypeOfVBO, int aTypeOfPrimitive);
};


} // namespace Graphics


//******************************************************************************
#include "SphereMesh.inl"


#endif // SPHERE_MESH_H
