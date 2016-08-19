#ifndef INTERNALORGANMESH_H
#define INTERNALORGANMESH_H

/**
********************************************************************************
*
*	@file		InternalOrganMesh.h
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
#include <gVirtualXRay/AnatomicalMesh.h>
#ifndef GVXR_SHADER_H
#include "gVirtualXRay/Shader.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {

//==============================================================================
/**
*   @class  InternalOrganMesh
*   @brief  InternalOrganMesh is a class to handle a polygon mesh for an internal organ.
*   	Internal organs use bump mapping.
*/
//==============================================================================
class InternalOrganMesh: public AnatomicalMesh {
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    //--------------------------------------------------------------------------
	InternalOrganMesh();

	//--------------------------------------------------------------------------
	/// Destructor
	//--------------------------------------------------------------------------
	virtual ~InternalOrganMesh();

	//--------------------------------------------------------------------------
	/// return true if the mesh has to be displayed with transparency
	//--------------------------------------------------------------------------
	virtual bool isTransparent() const;

	//--------------------------------------------------------------------------
	/// return true if the mesh represent a soft tissue
	//--------------------------------------------------------------------------
	virtual bool isSoftTissue() const = 0;

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
	/// initialize the shaders, the buffers, the normal vectors and the textures
	//--------------------------------------------------------------------------
	virtual void initialize();


protected:
	//--------------------------------------------------------------------------
	/// generates the textures
	//--------------------------------------------------------------------------
	virtual void generateTextures();

	//--------------------------------------------------------------------------
	/// Split each face into four faces.
	//--------------------------------------------------------------------------
	virtual void splitFaces();

	//-------------------------------------------------------------------------
	/// Link the attributes to their id in the shader bump map generation
    /*
    *   @param shader_id: The GLSL shader
    */
	//-------------------------------------------------------------------------
	virtual void defineBumpMapShaderInput(int shader_id);

	//--------------------------------------------------------------------------
	/// initialize a frame buffer and bind a texture to it
    /*
    *   @param aTexture: the texture to be generated
    *   @param aFrameBuffer: the framebuffer
    */
	//-------------------------------------------------------------------------
	virtual void initializeBuffer(unsigned int& aTexture, unsigned int& aFrameBuffer);

	//--------------------------------------------------------------------------
	/// Initialize optimized texture coordinates
	//--------------------------------------------------------------------------
	virtual void initializeTextureCoordinates();

	//--------------------------------------------------------------------------
	/// Compute a normal vector per vertex for a smoother display
	//--------------------------------------------------------------------------
	virtual void shareNormalVectors();

	//--------------------------------------------------------------------------
	/// delete the indices and calculate new normal vectors
	//--------------------------------------------------------------------------
	virtual void deleteIndices();

	//--------------------------------------------------------------------------
	/// initialize gradient tab for gnoise
	//--------------------------------------------------------------------------
	virtual void initGradientTab();

	//--------------------------------------------------------------------------
	/// initialize permutation tab for all noises
	//--------------------------------------------------------------------------
	virtual void initPermutation();

	//gradient tab for gnoise
	std::vector<VEC3> m_gradient_tab;

	//permutation tab for noises
	std::vector<int> m_permutation;

	//Coordinates for texture sampling
	std::vector<VEC2> m_textCoord;

	//direction of the margin to add between triangles in the texture
	std::vector<VEC2> m_margin_direction;

	//buffers for the bump map
	unsigned int m_bump_map_texture,
		m_bump_map_frame_buffer;

	//GLSL shaders to create the bump map and the colour texture
	Shader m_bump_map_shader;

	//to know if the internal organ is a muscle with a tendon for texturing and bump mapping, 0 by default
	int m_has_tendon;

	//to know if the internal organ is a lung with two colors and a line for texturing and bump mapping, 0 by default
	int m_is_lung;

	//to know if the internal organ has thin bumps for bump mapping, 0 by default
	int m_has_thin_bumps;

	//to know if the textured has been initialized
	bool m_need_initialize;
};

} // namespace gVirtualXRay

#endif // INTERNALORGANMESH_H
