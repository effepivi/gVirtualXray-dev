#ifndef BONE_MESH_H
#define BONE_MESH_H

/**
********************************************************************************
*
*	@file		BoneMesh.h
*
*	@brief      Class to handle a polygon mesh for Bone with thin bumps stored using a 3D texture.
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
*   @brief  BoneMesh is a class to handle a polygon mesh for Bone, with thin bumps
*   stored using a 3D texture.
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
    *   @param texture_3D_size: size of the side of the buffer of the 3D texture (default: 32)
    */
    //--------------------------------------------------------------------------
	BoneMesh(unsigned int texture_3D_size = 32);

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

	//-------------------------------------------------------------------------
	/// Link the attributes to their id in the shader bump map generation
    /*
    *   @param shader_id: The GLSL shader
    */
	//-------------------------------------------------------------------------
	virtual void defineBumpMapShaderInput(int shader_id);

	//--------------------------------------------------------------------------
	/// Link the attributes to their id in the shader for colour texture generation
    /*
    *   @param shader_id: The GLSL shader
    */
	//-------------------------------------------------------------------------
	virtual void defineColourTextureShaderInput(int shader_id);
};

} // namespace gVirtualXRay

#endif // BONE_MESH_H
