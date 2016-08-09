#ifndef LIVER_MESH_H
#define LIVER_MESH_H

/**
********************************************************************************
*
*	@file		LungMesh.h
*
*	@brief      Class to handle a polygon mesh for a lung.
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
#ifndef GVXR_POLYGON_MESH_H
#include "gVirtualXRay/PolygonMesh.h"
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
*   @class  LungMesh
*   @brief  LungMesh is a class to handle a polygon mesh for a lung.
*/
//==============================================================================
class LungMesh: public PolygonMesh
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    //--------------------------------------------------------------------------
	LungMesh();

	//--------------------------------------------------------------------------
	/// Destructor
	//--------------------------------------------------------------------------
	virtual ~LungMesh();

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
    *   @param shader_id: The GLSL shader used to display the anatomical mesh
    */
	//-------------------------------------------------------------------------
	virtual void defineDisplayShaderInput(GLint shader_id);

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

	//size of the arrays to initialize perlin's noise
	const static int TAB_SIZE = 256;

	//size of the side of the buffer
	static const int TEXTURE_SIZE = 4096;

	//Coordinates for texture sampling
	std::vector<VEC2> m_textCoord;

	//buffers for the bump map and the colour texture
	GLuint m_colour_texture,
		m_colour_texture_frame_buffer;

	//GLSL shader to create the colour texture
	Shader m_colour_texture_shader;


};

} // namespace gVirtualXRay

#endif // LIVER_MESH_H
