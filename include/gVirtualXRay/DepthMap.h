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
*  @class  DepthMap
*  @brief  DepthMap is a class to render to a buffer in order to create, for exemple, a shadow map.
*/
//==============================================================================
class DepthMap {

public:
	//--------------------------------------------------------------------------
	/// default constructor
	//--------------------------------------------------------------------------
	DepthMap(int map_size = 8192);

	//--------------------------------------------------------------------------
	/// destructor
	//--------------------------------------------------------------------------
	~DepthMap();

	//--------------------------------------------------------------------------
	///Initialize the buffers
	//--------------------------------------------------------------------------
	void initialize();

	//--------------------------------------------------------------------------
	///render to the frame buffer, call initialize() if it has never been called before
	/**
	*  @param apMeshSet: the Polygon mesh set to be textured
	*  @param projMatrix: the projection matrix
	*/
	//--------------------------------------------------------------------------
	void render(std::vector<PolygonMesh*>& apMeshSet,
			Matrix4x4<GLfloat> const& projMatrix);

	//--------------------------------------------------------------------------
	///Get the texture
	//--------------------------------------------------------------------------
	unsigned int getMapTexture() const;

	//******************************************************************************
private:
	//No copy constructor
	DepthMap(DepthMap const&);

	//to know if initialize() has been called
	bool m_isInitialized;

    Shader m_depth_shader;

	//To create a buffer for the scene map
	unsigned int	m_mapFrameBuffer,
		m_mapTexture;

	int m_map_size;
};
} // namespace gVirtualXRay
#endif //DepthMap_H
