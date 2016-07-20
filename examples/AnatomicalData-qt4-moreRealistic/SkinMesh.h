#ifndef SKIN_MESH_H
#define SKIN_MESH_H

/**
********************************************************************************
*
*	@file		SkinMesh.h
*
*	@brief      Class to handle a polygon mesh for skin.
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

namespace gVirtualXRay {


//==============================================================================
/**
*   @class  SkinMesh
*   @brief  SkinMesh is a class to handle a polygon mesh for skin.
*/
//==============================================================================
class SkinMesh: public AnatomicalMesh
//------------------------------------------------------------------------------
{
//******************************************************************************
public:

	SkinMesh();

	virtual ~SkinMesh();
};

} // namespace gVirtualXRay

#endif // SKIN_MESH_H
