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
	// TODO Auto-generated constructor stub

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
void InternalOrganMesh::display()
//----------------------------------------------
{
	AnatomicalMesh::display();
}

} // namespace gVirtualXRay
