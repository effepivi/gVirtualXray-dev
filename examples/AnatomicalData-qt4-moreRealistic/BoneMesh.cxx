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

#include <BoneMesh.h>


namespace gVirtualXRay {

//----------------------------------------------
BoneMesh::BoneMesh():
//----------------------------------------------
		InternalOrganMesh()
//----------------------------------------------
{
	getMaterial().setShininess(4);
	getMaterial().setSpecularColour(0.2,0.2,0.2,1.);
	getMaterial().setDiffuseColour(0.84,0.77,0.6,1.);
	getMaterial().setAmbientColour(0.84,0.77,0.6,1.);
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
void BoneMesh::defineShaderInput(GLint aShaderID)
//----------------------------------------------
{
	GLint useTexture(0);
	GLuint handle = glGetUniformLocation(aShaderID, "g_useTexture");
	glUniform1iv(handle, 1, &useTexture);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	AnatomicalMesh::defineShaderInput(aShaderID);
}


//----------------------------------------------
void BoneMesh::generateTextures()
//----------------------------------------------
{
	//Don't generate textures for bones
}

} // namespace gVirtualXRay
