/**
********************************************************************************
*
*	@file		SkinMesh.h
*
*	@brief      Class to handle a polygon mesh for Skin.
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
#include "gVirtualXRay/SkinMesh.h"

//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {

//----------------------------------------
SkinMesh::SkinMesh():
//----------------------------------------
		AnatomicalMesh()
//----------------------------------------
{
	getMaterial().setShininess(5);
	getMaterial().setSpecularColour(0.1,0.1,0.1,0.05);
	getMaterial().setDiffuseColour(0.3,0.3,0.3,0.05);
	getMaterial().setAmbientColour(0.3,0.3,0.3,0.05);
}

//----------------------------------------
SkinMesh::~SkinMesh()
//----------------------------------------
{
	// TODO Auto-generated destructor stub
}

//----------------------------------------
bool SkinMesh::isTransparent() const
//----------------------------------------
{
	return true;
}

//----------------------------------------
bool SkinMesh::isSoftTissue() const
//----------------------------------------
{
	return true;
}

//----------------------------------------------
void SkinMesh::initialize()
//----------------------------------------------
{
	mergeVertices();
}

//----------------------------------------------
void SkinMesh::setMatrix(Matrix4x4<float> const& matrix)
//----------------------------------------------
{
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented.");
}



} // namespace gVirtualXRay
