#ifndef LIVER_MESH_H
#define LIVER_MESH_H

/**
********************************************************************************
*
*	@file		LiverMesh.h
*
*	@brief      Class to handle a polygon mesh for a liver.
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
    *   @param texture_size: size of the side of the buffer (default: 2048)
    *   @param store_texture: To know if the textures need to be stored (default: true)
    *   @param optimize_texture_coordinates: To know if the texture coordinates need to be optimized (default: true)
    */
    //--------------------------------------------------------------------------
	LiverMesh(bool store_texture = true, bool optimize_texture_coordinates = true,
			unsigned int texture_size = 2048);

	//--------------------------------------------------------------------------
	/// Destructor
	//--------------------------------------------------------------------------
	virtual ~LiverMesh();

};

} // namespace gVirtualXRay

#endif // LIVER_MESH_H
