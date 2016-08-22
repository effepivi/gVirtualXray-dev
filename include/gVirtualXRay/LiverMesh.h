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
    *   @param optimize_threshold: To avoid computing optimized texture coordinates
    *   	when the mesh is too big (default: 100 000)
    */
    //--------------------------------------------------------------------------
	LiverMesh(int texture_size = 2048, unsigned int optimize_threshold = 100000);

	//--------------------------------------------------------------------------
	/// Destructor
	//--------------------------------------------------------------------------
	virtual ~LiverMesh();

};

} // namespace gVirtualXRay

#endif // LIVER_MESH_H
