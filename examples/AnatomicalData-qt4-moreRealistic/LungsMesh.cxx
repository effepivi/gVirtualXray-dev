/**
********************************************************************************
*
*	@file		LungsMesh.cxx
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

#include <LungsMesh.h>

namespace gVirtualXRay {

//----------------------------------------------
LungsMesh::LungsMesh():
//----------------------------------------------
		SoftTissueMesh()
//----------------------------------------------
{
	getMaterial().setDiffuseColour(0.7,0.28,0.28,1.);
	getMaterial().setAmbientColour(0.7,0.28,0.28,1.);
}


//----------------------------------------------
LungsMesh::~LungsMesh()
//----------------------------------------------
{
	// TODO Auto-generated destructor stub
}

} // namespace gVirtualXRay
