#ifndef LUNGS_MESH_H
#define LUNGS_MESH_H

/**
********************************************************************************
*
*	@file		LungsMesh.h
*
*	@brief      Class to handle a polygon mesh for lungs.
*  			 It uses 2D texture for the bump map and 3D texture for the colour
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
*  			 It uses 2D texture for the bump map and 3D texture for the colour
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
    *   @param texture_2D_size: size of the side of the buffer of the 2D texture (default: 2048)
    *   @param texture_3D_size: size of the side of the buffer of the 3D texture (default: 32)
    *   @param optimize_texture_coordinates: To know if the texture coordinates need to be optimized (default: true)
    */
    //--------------------------------------------------------------------------
	LungsMesh(bool x_mirror = false, bool y_mirror = false, bool z_mirror = false,
			bool optimize_texture_coordinates = true, unsigned int  texture_2D_size = 2048,
			unsigned int  texture_3D_size = 32);

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
