/**
********************************************************************************
*
*	@file		BoneMesh.cxx
*
*	@brief      Class to handle a polygon mesh for Bone.
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
#include "gVirtualXRay/BoneMesh.h"


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {

//----------------------------------------------
BoneMesh::BoneMesh(bool store_texture, bool optimize_texture_coordinates,
		unsigned int texture_size):
//----------------------------------------------
		InternalOrganMesh(store_texture, optimize_texture_coordinates, texture_size)
//----------------------------------------------
{
	m_has_thin_bumps = 1;
	getMaterial().setShininess(4);
	getMaterial().setSpecularColour(0.2,0.2,0.2,1.);
	getMaterial().setDiffuseColour(0.84,0.74,0.6,1.);
	getMaterial().setAmbientColour(0.84,0.74,0.6,1.);
}

//----------------------------------------
BoneMesh::~BoneMesh()
//----------------------------------------
{
	// TODO Auto-generated destructor stub
}

//----------------------------------------
bool BoneMesh::isSoftTissue() const
//----------------------------------------
{
	return false;
}

//----------------------------------------------
void BoneMesh::initialize()
//----------------------------------------------
{
	InternalOrganMesh::initialize();
}

} // namespace gVirtualXRay
