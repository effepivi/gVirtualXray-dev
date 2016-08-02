/**
********************************************************************************
*
*	@file		InternalOrganMesh.cxx
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
#include <InternalOrganMesh.h>

#include <iostream>

//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {

//----------------------------------------
InternalOrganMesh::InternalOrganMesh():
//----------------------------------------
		AnatomicalMesh()
//----------------------------------------
{

}

//----------------------------------------
InternalOrganMesh::~InternalOrganMesh()
//----------------------------------------
{
	// TODO Auto-generated destructor stub
}

//----------------------------------------
bool InternalOrganMesh::isTransparent() const
//----------------------------------------
{
	return false;
}

//----------------------------------------------
void InternalOrganMesh::defineShaderInput(GLint aShaderID)
//----------------------------------------------
{
	AnatomicalMesh::defineShaderInput(aShaderID);
}

} // namespace gVirtualXRay
