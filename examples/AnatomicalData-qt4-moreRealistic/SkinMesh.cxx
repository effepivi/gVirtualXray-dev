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

#include <SkinMesh.h>

namespace gVirtualXRay {

//----------------------------------------
SkinMesh::SkinMesh():
//----------------------------------------
		AnatomicalMesh()
//----------------------------------------
{
	getMaterial().setShininess(1);
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
bool SkinMesh::isTransparent()
//----------------------------------------
{
	return true;
}

} // namespace gVirtualXRay
