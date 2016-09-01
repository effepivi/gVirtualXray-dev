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
    /*
    *   @param texture_2D_size: size of the side of the buffer of the 2D texture (default: 1024)
    *   @param texture_3D_size: size of the side of the buffer of the 3D texture (default: 64)
    *   @param optimize_texture_coordinates: To know if the texture coordinates need to be optimized (default: true)
    */
    //--------------------------------------------------------------------------
	InternalOrganMesh(bool optimize_texture_coordinates = true,
			unsigned int texture_2D_size = 1024, unsigned int texture_3D_size = 64);

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
	/// Link the attributes to their id in the shader
    /*
    *   @param aShaderID: The GLSL shader used to display the anatomical mesh
    */
	//-------------------------------------------------------------------------
	virtual void defineDisplayShaderInput(int shader_id);

	//--------------------------------------------------------------------------
	/// initialise the shaders, the buffers, the normal vectors and the textures.
	/// Merge vertices for a smoother display if only using 3D textures.
	//--------------------------------------------------------------------------
	virtual void initialize() = 0;

	//--------------------------------------------------------------------------
	/// Split each face into four faces.
	//--------------------------------------------------------------------------
	virtual void splitFaces();

protected:
	///To know if a bump map or a colour texture has to be created
	enum ToCreate{TO_CREATE_BUMP_MAP, TO_CREATE_COLOUR_TEXTURE};

	//--------------------------------------------------------------------------
	/// generates the textures
	//--------------------------------------------------------------------------
	virtual void generateTextures() = 0;

	//--------------------------------------------------------------------------
	/// generates a 2D texture
    /*
    *   @param shader: The GLSL shader
    *   @param frame_buffer: The frame buffer the texture will be rendered to
    *   @param to_create: To know if a bump map or a colour texture has to be created
    */
	//--------------------------------------------------------------------------
	virtual void generate2DTexture(Shader const& shader, unsigned int& frame_buffer,
			ToCreate to_create);

	//--------------------------------------------------------------------------
	/// generates a 3D texture
    /*
    *   @param shader: The GLSL shader
    *   @param frame_buffer: The frame buffer the texture will be rendered to
    *   @param texture: the texture storing the data
    *   @param to_create: To know if a bump map or a colour texture has to be created
    */
	//--------------------------------------------------------------------------
	virtual void generate3DTexture(Shader const& shader,
			unsigned int& frame_buffer, unsigned int& texture, ToCreate to_create);

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
	virtual void defineColourTextureShaderInput(int shader_id) = 0;

	//--------------------------------------------------------------------------
	/// initialize a 2D frame buffer and bind a texture to it
    /*
    *   @param aTexture: the texture to be generated
    *   @param aFrameBuffer: the framebuffer
    */
	//-------------------------------------------------------------------------
	virtual void initialize2DBuffer(unsigned int& aTexture, unsigned int& aFrameBuffer);

	//--------------------------------------------------------------------------
	/// initialize a 3D frame buffer and bind a texture to its layer 0
    /*
    *   @param aTexture: the texture to be generated
    *   @param aFrameBuffer: the framebuffer
    */
	//-------------------------------------------------------------------------
	virtual void initialize3DBuffer(unsigned int& aTexture, unsigned int& aFrameBuffer, bool has_colours);

	//--------------------------------------------------------------------------
	/// Load the shader to create the 2D bump map
	//-------------------------------------------------------------------------
	virtual void load2DBumpShader();

	//--------------------------------------------------------------------------
	/// Load the shader to create the 3D bump map
	//-------------------------------------------------------------------------
	virtual void load3DBumpShader();

	//--------------------------------------------------------------------------
	/// Initialise optimised texture coordinates by sharing an hypothenuse in the texture
	/// for two adjacent polygons.
	//--------------------------------------------------------------------------
	virtual void initialize2DTextureCoordinates();

	//--------------------------------------------------------------------------
	/// Initialise 3D texture coordinates
	//--------------------------------------------------------------------------
	virtual void initialize3DTextureCoordinates();

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
	std::vector<VEC2> m_2D_texture_coordinates;

	std::vector<VEC3> m_3D_texture_coordinates;

	//direction of the margin to add between triangles in the 2D texture
	std::vector<VEC2> m_margin_direction;

	//buffers for the bump map
	unsigned int m_bump_map_texture,
		m_bump_map_frame_buffer;

	//GLSL shader to create the bump map
	Shader m_bump_map_shader;

	//to know if the internal organ is a muscle with a tendon for texturing and bump mapping, 0 by default
	int m_has_tendon;

	//to know if the internal organ is a lung with two colors and a line for texturing and bump mapping, 0 by default
	int m_is_lung;

	//to know if the internal organ has thin bumps for bump mapping, 0 by default
	int m_has_thin_bumps;

	int m_use_3D_bump_map;

	//size of the side of the buffer
	unsigned int m_texture_2D_size,
		m_texture_3D_size;

	//to know if the textured has been initialized
	bool m_need_initialize;

	//to know if the generation texture coordinates has been set
	bool m_generation_texture_coord_are_set;

	///To know if the texture coordinates need to be optimized
	bool m_optimize_texture_coordinates;
};

} // namespace gVirtualXRay

#endif // INTERNALORGANMESH_H
