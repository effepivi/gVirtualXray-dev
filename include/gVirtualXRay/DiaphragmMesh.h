#ifndef DIAPHRAGM_MESH_H
#define DIAPHRAGM_MESH_H

/**
********************************************************************************
*
*	@file		DiaphragmMesh.h
*
*	@brief      Class to handle a polygon mesh for a diaphragm.
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

#ifndef SOFT_TISSUE_MESH_H
#include "gVirtualXRay/SoftTissueMesh.h"
#endif

//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  DiaphragmMesh
*   @brief  DiaphragmMesh is a class to handle a polygon mesh for a diaphragm.
*/
//==============================================================================
class DiaphragmMesh: public SoftTissueMesh
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    /*
    *   @param texture_size: size of the side of the buffer (default: 2048)
    *   @param optimize_threshold: To avoid computing optimized texture coordinates
    *   	when the mesh is too big (defalut: 100 000)
    */
    //--------------------------------------------------------------------------
	DiaphragmMesh(int texture_size = 2048, unsigned int optimize_threshold = 100000);

	//--------------------------------------------------------------------------
	/// Destructor
	//--------------------------------------------------------------------------
	virtual ~DiaphragmMesh();

protected:
	//--------------------------------------------------------------------------
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

#endif // DIAPHRAGM_MESH_H
