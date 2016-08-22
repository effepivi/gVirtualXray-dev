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
    /// Constructor
    /*
    *   @param x_mirror: To know if the mesh is upside down for the x axis (default: false)
    *   @param y_mirror: To know if the mesh is upside down for the y axis (default: false)
    *   @param z_mirror: To know if the mesh is upside down for the z axis (default: false)
    *   @param texture_size: size of the side of the buffer (default: 2048)
    *   @param optimize_threshold: To avoid computing optimized texture coordinates
    *   	when the mesh is too big (defalut: 100 000)
    *
    */
    //--------------------------------------------------------------------------
	LungsMesh(bool x_mirror = false, bool y_mirror = false, bool z_mirror = false, int texture_size = 2048, unsigned int optimize_threshold = 100000);

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
