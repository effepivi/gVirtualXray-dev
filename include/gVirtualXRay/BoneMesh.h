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
    *   @param optimize_threshold: To avoid computing optimized texture coordinates
    *   	when the mesh is too big (defalut: 100 000)
    */
    //--------------------------------------------------------------------------
	BoneMesh(int texture_size = 2048, unsigned int optimize_threshold = 100000);

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
