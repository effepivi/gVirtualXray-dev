#ifndef BONE_MESH_H
#define BONE_MESH_H

/**
********************************************************************************
*
*	@file		BoneMesh.h
*
*	@brief      Class to handle a polygon mesh for Bone with thin bumps.
*
*	@version	1.0
*
*	@date		18/07/2016
*
*	@author		Andréas Meuleman
*
********************************************************************************
*/

//******************************************************************************
//	Include
//******************************************************************************

#ifndef INTERNALORGANMESH_H
#include "gVirtualXRay/InternalOrganMesh.h"
#endif

//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  BoneMesh
*   @brief  BoneMesh is a class to handle a polygon mesh for Bone, with thin bumps.
*/
//==============================================================================
class BoneMesh: public InternalOrganMesh
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    /*
    *   @param texture_size: size of the side of the buffer (default: 2048)
    *   @param optimize_texture_coordinates: To know if the texture coordinates need to be optimized (default: true)
    */
    //--------------------------------------------------------------------------
	BoneMesh(bool optimize_texture_coordinates = true,
			unsigned int texture_size = 2048);

	//--------------------------------------------------------------------------
	/// Destructor
	//--------------------------------------------------------------------------
	virtual ~BoneMesh();

	//--------------------------------------------------------------------------
	/// return true if the mesh represent a soft tissue
	//--------------------------------------------------------------------------
	virtual bool isSoftTissue() const;

	//--------------------------------------------------------------------------
	/// Merge the vertices for optimization and a smoother display
	//--------------------------------------------------------------------------
	virtual void initialize();

};

} // namespace gVirtualXRay

#endif // BONE_MESH_H
