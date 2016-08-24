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

#include "gVirtualXRay/PolygonMesh.h"

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
    /*
    *   @param texture_size: size of the side of the buffer (default: 8192)
    */
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
	///Use this overload in case there is one model matrixes for all the meshes
	/**
	*  @param mesh_set: the Polygon mesh set to be rendered
	*  @param proj_matrix: the projection matrix
	*/
	//--------------------------------------------------------------------------
	void render(std::vector<PolygonMesh*>& mesh_set,
			Matrix4x4<float> const& proj_matrix);

	//--------------------------------------------------------------------------
	///render to the frame buffer, call initialize() if it has never been called before
	///Use this overload in case there are different model matrixes
	/**
	*  @param mesh_set: the Polygon mesh set to be rendered
	*  @param proj_matrix: the projection matrix
	*  @param model_matrix_set: the model matrix set
	*/
	//--------------------------------------------------------------------------
	void render(std::vector<PolygonMesh*>& mesh_set,
			Matrix4x4<float> const& proj_matrix, std::vector< Matrix4x4<float> > const& model_matrix_set);

	//--------------------------------------------------------------------------
	///Get the texture
	//--------------------------------------------------------------------------
	unsigned int getMapTexture() const;

	//******************************************************************************
private:
	//No copy constructor
	DepthMap(DepthMap const&);

	//to know if initialize() has been called
	bool m_is_initialized;

    Shader m_depth_shader;

	//To create a buffer for the scene map
	unsigned int	m_map_frame_buffer,
		m_map_texture;

	int m_map_size;
};
} // namespace gVirtualXRay
#endif //DepthMap_H
