#ifndef LIVER_MESH_H
#define LIVER_MESH_H

/**
********************************************************************************
*
*	@file		LiverMesh.h
*
*	@brief      Class to handle a polygon mesh for a liver. It uses 3D texture for the
*				bump map and 2D texture for the colour
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
*   @class  LiverMesh
*   @brief  LiverMesh is a class to handle a polygon mesh for a liver.
*   		It uses 3D texture for the bump map and 2D texture for the colour
*/
//==============================================================================
class LiverMesh: public SoftTissueMesh
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    /*
    *   @param texture_2D_size: size of the side of the buffer of the 2D texture (default: 2048)
    *   @param texture_3D_size: size of the side of the buffer of the 3D texture (default: 64)
    *   @param optimize_texture_coordinates: To know if the texture coordinates need to be optimized (default: true)
    */
    //--------------------------------------------------------------------------
	LiverMesh(bool optimize_texture_coordinates = true,
			unsigned int texture_2D_size = 2048, unsigned int texture_3D_size = 64);

	//--------------------------------------------------------------------------
	/// Destructor
	//--------------------------------------------------------------------------
	virtual ~LiverMesh();

};

} // namespace gVirtualXRay

#endif // LIVER_MESH_H
