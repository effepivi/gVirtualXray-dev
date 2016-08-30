/**
********************************************************************************
*
*	@file		LiverMesh.cxx
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
#include "gVirtualXRay/LiverMesh.h"

//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {

//----------------------------------------------
LiverMesh::LiverMesh(bool optimize_texture_coordinates,
		unsigned int texture_size):
//----------------------------------------------
		SoftTissueMesh(optimize_texture_coordinates, texture_size)
//----------------------------------------------
{
	getMaterial().setDiffuseColour(0.3,0.03,0.03,1.);
	getMaterial().setAmbientColour(0.3,0.03,0.03,1.);
}

//----------------------------------------------
LiverMesh::~LiverMesh()
//----------------------------------------------
{
	// TODO Auto-generated destructor stub
}


} // namespace gVirtualXRay
