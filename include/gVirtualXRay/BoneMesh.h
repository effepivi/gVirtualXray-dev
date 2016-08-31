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
    *   @param texture_size: size of the side of the buffer (default: 32)
    */
    //--------------------------------------------------------------------------
	BoneMesh(unsigned int texture_size = 32);

	//--------------------------------------------------------------------------
	/// Destructor
	//--------------------------------------------------------------------------
	virtual ~BoneMesh();

	//--------------------------------------------------------------------------
	/// return true if the mesh represent a soft tissue
	//--------------------------------------------------------------------------
	virtual bool isSoftTissue() const;

	//--------------------------------------------------------------------------
	///	Update the VBO.
	/**
	*	@param aTypeOfVBO: 			VBO usage hint to set; valid values are GL_STATIC_DRAW, GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	*	@param aTypeOfPrimitive:	type of primitive; valid values are GL_TRIANGLES
	*/
	//--------------------------------------------------------------------------
	virtual void updateVBO(int aTypeOfVBO, int aTypeOfPrimitive);

	//--------------------------------------------------------------------------
	/// Link the attributes to their id in the shader
    /*
    *   @param aShaderID: The GLSL shader used to display the anatomical mesh
    */
	//-------------------------------------------------------------------------
	virtual void defineDisplayShaderInput(int shader_id);


	//--------------------------------------------------------------------------
	/// Merge the vertices for optimization and a smoother display and create the bump map
	//--------------------------------------------------------------------------
	virtual void initialize();

protected:
	//--------------------------------------------------------------------------
	/// generates the textures
	//--------------------------------------------------------------------------
	virtual void generateTextures();

};

} // namespace gVirtualXRay

#endif // BONE_MESH_H
