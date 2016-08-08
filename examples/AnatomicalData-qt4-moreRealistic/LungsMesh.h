#ifndef LUNGS_MESH_H
#define LUNGS_MESH_H

/**
********************************************************************************
*
*	@file		LungsMesh.h
*
*	@brief      Class to handle a polygon mesh for lungs.
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
#include "SoftTissueMesh.h"
#endif

//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  LungsMesh
*   @brief  LungsMesh is a class to handle a polygon mesh for a lung.
*/
//==============================================================================
class LungsMesh: public SoftTissueMesh
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    //--------------------------------------------------------------------------
	LungsMesh();

	//--------------------------------------------------------------------------
	/// Destructor
	//--------------------------------------------------------------------------
	virtual ~LungsMesh();

	//--------------------------------------------------------------------------
	/// Set the matrix. Useful only for the diaphragm
	//--------------------------------------------------------------------------
	virtual void setMatrix(Matrix4x4<float> const& matrix);

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
};

} // namespace gVirtualXRay

#endif // LUNGS_MESH_H
