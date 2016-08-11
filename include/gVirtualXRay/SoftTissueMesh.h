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

#ifndef GVXR_SHADER_H
#include "gVirtualXRay/Shader.h"
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
    //--------------------------------------------------------------------------
	SoftTissueMesh();

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
	virtual void defineDisplayShaderInput(GLint aShaderID);

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
	virtual void defineBumpMapShaderInput(GLint shader_id);

	//--------------------------------------------------------------------------
	/// Link the attributes to their id in the shader for colour texture generation
    /*
    *   @param shader_id: The GLSL shader
    */
	//-------------------------------------------------------------------------
	virtual void defineColourTextureShaderInput(GLint shader_id);


	virtual void initializeBuffer(GLuint& aTexture, GLuint& aFrameBuffer);


	//--------------------------------------------------------------------------
	/// Compute a normal vector per vertex for a smoother display
	//--------------------------------------------------------------------------
	virtual void shareNormalVectors();

	//--------------------------------------------------------------------------
	/// initialize gradient tab for gnoise
	//--------------------------------------------------------------------------
	virtual std::vector<VEC3> initGradientTab();

	//--------------------------------------------------------------------------
	/// initialize permutation tab for all noises
	//--------------------------------------------------------------------------
	virtual std::vector<int> initPermutation();

	//Coordinates for texture sampling
	std::vector<VEC2> m_textCoord;

	//buffers for the bump map and the colour texture
	GLuint m_bump_map_texture,
		m_bump_map_frame_buffer,
		m_colour_texture,
		m_colour_texture_frame_buffer;

	//GLSL shaders to create the bump map and the colour texture
	Shader m_bump_map_shader,
		m_colour_texture_shader;

	//to know if the soft tissue texture generation uses two colours
	GLint m_use_second_colour;

	//to know if the soft tissue is a muscle with a tendon for texturing
	GLint m_has_tendon;
};

} // namespace gVirtualXRay

#endif // SOFT_TISSUE_MESH_H
