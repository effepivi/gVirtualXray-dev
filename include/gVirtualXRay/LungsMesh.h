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
#include "gVirtualXRay/SoftTissueMesh.h"
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
	LungsMesh(bool x_mirror = false, bool y_mirror = false, bool z_mirror = false );

	//--------------------------------------------------------------------------
	/// Destructor
	//--------------------------------------------------------------------------
	virtual ~LungsMesh();

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

	//to know if the model is upside down to draw the lines
	bool m_x_mirror,
		m_y_mirror,
		m_z_mirror;

};

} // namespace gVirtualXRay

#endif // LUNGS_MESH_H
