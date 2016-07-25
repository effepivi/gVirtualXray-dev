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

#include <SoftTissueMesh.h>

namespace gVirtualXRay {

//----------------------------------------------
SoftTissueMesh::SoftTissueMesh()
:
//----------------------------------------------
		AnatomicalMesh()
//----------------------------------------------
{
	getMaterial().setShininess(50);
}

//----------------------------------------------
SoftTissueMesh::~SoftTissueMesh()
//----------------------------------------------
{
	// TODO Auto-generated destructor stub
}

//----------------------------------------------
bool SoftTissueMesh::isSoftTissue() const
//----------------------------------------------
{
	return true;
}

//----------------------------------------------
void SoftTissueMesh::display()
//----------------------------------------------
{
	AnatomicalMesh::display();
}

} // namespace gVirtualXRay

