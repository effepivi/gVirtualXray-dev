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
#include "InternalOrganMesh.h"
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
	virtual void defineShaderInput(GLint aShaderID);

	//--------------------------------------------------------------------------
	/// generates the texture if needed
	//--------------------------------------------------------------------------
	virtual void generateTextures();


protected:

	virtual void splitFaces();

	virtual void defineBumpMapShaderInput();

	virtual void defineColourShaderInput();

	//--------------------------------------------------------------------------
	/// initialize the shaders and the buffers
	//--------------------------------------------------------------------------
	virtual void initialize();

	virtual void initializeBuffer(GLuint& aTexture, GLuint& aFrameBuffer);

	//--------------------------------------------------------------------------
	/// initialize tab for gnoise
	//--------------------------------------------------------------------------
	virtual std::vector<VEC3> gradientTabInit();

	//size of the side of the buffer
	static const int TEXTURE_SIZE = 4096;

	//Coordinates for texture sampling
	std::vector<VEC2> m_textCoord;

	GLuint m_bumpMapTexture,
		m_bumpMapFrameBuffer,
		m_colourTexture,
		m_colourFrameBuffer;

	Shader m_bumpMapShader,
		m_textureShader;
};

} // namespace gVirtualXRay

#endif // SOFT_TISSUE_MESH_H
