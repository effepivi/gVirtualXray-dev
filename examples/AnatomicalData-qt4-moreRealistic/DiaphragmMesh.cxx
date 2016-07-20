/**
********************************************************************************
*
*	@file		DiaphragmMesh.cxx
*
*	@brief      Class to handle a polygon mesh for a diaphragm.
*
*	@version	1.0
*
*	@date		18/07/2016
*
*	@author		Andréas Meuleman
*
********************************************************************************
*/


#include <DiaphragmMesh.h>

namespace gVirtualXRay {

//----------------------------------------------
DiaphragmMesh::DiaphragmMesh():
//----------------------------------------------
		SoftTissueMesh()
//----------------------------------------------
{
	getMaterial().setDiffuseColour(0.62,0.12,0.12,1.);
	getMaterial().setAmbientColour(0.62,0.12,0.12,1.);
}

//----------------------------------------------
DiaphragmMesh::~DiaphragmMesh()
//----------------------------------------------
{
	// TODO Auto-generated destructor stub
}

} // namespace gVirtualXRay
