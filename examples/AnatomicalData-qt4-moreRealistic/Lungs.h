#ifndef SOFT_TISSUE_MESH_H
#define SOFT_TISSUE_MESH_H

/**
********************************************************************************
*
*	@file		SoftTissueMesh.h
*
*	@brief      Class to handle a polygon mesh for an internal organ.
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

#ifndef ANATOMICAL_MESH_H
#include "AnatomicalMesh.h"
#endif

//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  SoftTissueMesh
*   @brief  SoftTissueMesh is a class to handle a polygon mesh for an internal organ.
*/
//==============================================================================
class SoftTissueMesh: public AnatomicalMesh
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	SoftTissueMesh();
	virtual ~SoftTissueMesh();
};

} // namespace gVirtualXRay

#endif // SOFT_TISSUE_MESH_H
