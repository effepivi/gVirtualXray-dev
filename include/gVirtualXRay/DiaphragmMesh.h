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
    //--------------------------------------------------------------------------
	DiaphragmMesh();

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
	virtual void defineBumpMapShaderInput(GLint shader_id);

	//--------------------------------------------------------------------------
	/// Link the attributes to their id in the shader for colour texture generation
    /*
    *   @param shader_id: The GLSL shader
    */
	//-------------------------------------------------------------------------
	virtual void defineColourTextureShaderInput(GLint shader_id);

	//matrix for the bump map (to know where the center is)
	Matrix4x4<float> m_matrix;
};

} // namespace gVirtualXRay

#endif // DIAPHRAGM_MESH_H
