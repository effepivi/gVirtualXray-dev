#ifndef DepthMap_H
#define DepthMap_H

/**
*******************************************************************************
*
*  @file       DepthMap.h
*
*  @brief      Class to render to a buffer in order to create, for exemple, a shadow map.
*
*  @version    1.0
*
*  @date       19/07/2016
*
*  @author     Andréas Meuleman
*******************************************************************************
*/

//******************************************************************************
//  Include
//******************************************************************************
#include <vector>

#ifndef GVXR_MATRIX4X4_H
#include "gVirtualXRay/Matrix4x4.h"
#endif

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef ANATOMICAL_MESH_H
#include "AnatomicalMesh.h"
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
*  @class  DepthMap
*  @brief  DepthMap is a class to render to a buffer in order to create, for exemple, a shadow map.
*/
//==============================================================================
class DepthMap {

public:

	DepthMap();

	//--------------------------------------------------------------------------
	///
	//--------------------------------------------------------------------------
	~DepthMap();

	//--------------------------------------------------------------------------
	///Initialize the buffers
	//--------------------------------------------------------------------------
	void initialize();

	//--------------------------------------------------------------------------
	///render to the frame buffer, call initialize() if it has never been called before
	/**
	*  @param apMeshSet: the anatomical mesh set to be textured
	*  @param projMatrix: the projection matrix
	*  @param depthShader: glsl shader to create the depth map
	*/
	//--------------------------------------------------------------------------
	void render(std::vector<AnatomicalMesh*>& apMeshSet,
			Matrix4x4<GLfloat> const& projMatrix, Shader& depthShader);

	//--------------------------------------------------------------------------
	///Get the texture
	//--------------------------------------------------------------------------
	GLuint getMapTexture() const;

	//******************************************************************************
private:
	//No copy constructor
	DepthMap(DepthMap const&);

	static const int MAP_SIZE = 16384; //size of the side of the buffer

	bool m_isInitialized;//to know if initialize() has been called

	//To create a buffer for the scene map
	GLuint	m_mapFrameBuffer,
		m_mapTexture;
};
} // namespace gVirtualXRay
#endif //DepthMap_H
