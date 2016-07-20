#ifndef ANATOMICAL_MESH_H
#define ANATOMICAL_MESH_H


/**
********************************************************************************
*
*	@file		AnatomicalMesh.h
*
*	@brief      Class to handle a polygon mesh for tissue.
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

#ifndef GVXR_POLYGON_MESH_H
#include "gVirtualXRay/PolygonMesh.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay{


//==============================================================================
/**
*   @class  AnatomicalMesh
*   @brief  AnatomicalMesh is a class to handle a polygon mesh for tissue.
*/
//==============================================================================
class AnatomicalMesh: public PolygonMesh
//------------------------------------------------------------------------------
{
//******************************************************************************
public:

	AnatomicalMesh();

	virtual ~AnatomicalMesh();

	virtual bool isTransparent();


//******************************************************************************
private:




};
} //namespace gVirtualXRay
#endif //ANATOMICAL_MESH_H
