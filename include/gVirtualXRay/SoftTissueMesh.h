#ifndef SOFT_TISSUE_MESH_H
#define SOFT_TISSUE_MESH_H

/**
********************************************************************************
*
*	@file		SoftTissueMesh.h
*
*	@brief      Class to handle a polygon mesh for an internal organ.
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
*   @class  SoftTissueMesh
*   @brief  SoftTissueMesh is a class to handle a polygon mesh for an internal organ.
*   		It uses procedural texturing.
*/
//==============================================================================
class SoftTissueMesh: public InternalOrganMesh
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    /*
    *   @param texture_size: size of the side of the buffer (default: 1024)
    *   @param texture_3D_size: size of the side of the buffer of the 3D texture (default: 64)
    *   @param optimize_texture_coordinates: To know if the texture coordinates need to be optimized (default: true)
    */
    //--------------------------------------------------------------------------
	SoftTissueMesh(bool optimize_texture_coordinates = true,
			unsigned int texture_size = 1024,  unsigned int texture_3D_size = 64);

	//--------------------------------------------------------------------------
	/// Destructor
	//--------------------------------------------------------------------------
	virtual ~SoftTissueMesh();

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
	virtual void defineDisplayShaderInput(int aShaderID);

	//--------------------------------------------------------------------------
	/// initialize the shaders, the buffers, the normal vectors and the textures
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

	//buffers for the colour texture
	unsigned int m_colour_texture,
		m_colour_texture_frame_buffer;

	//to know if we use a 3D colour texture
	int m_use_3D_colour_texture;

	//GLSL shaders to create the colour texture
	Shader m_colour_texture_shader;
};

} // namespace gVirtualXRay

#endif // SOFT_TISSUE_MESH_H
