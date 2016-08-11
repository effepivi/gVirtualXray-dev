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

//******************************************************************************
//	Include
//******************************************************************************
#include "gVirtualXRay/LungsMesh.h"

//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {

//----------------------------------------------
LungsMesh::LungsMesh():
//----------------------------------------------
		SoftTissueMesh()
//----------------------------------------------
{
	getMaterial().setDiffuseColour(0.7,0.28,0.28,1.);
	getMaterial().setAmbientColour(0.7,0.28,0.28,1.);
	m_use_second_colour=1;
}


//----------------------------------------------
LungsMesh::~LungsMesh()
//----------------------------------------------
{
	// TODO Auto-generated destructor stub
}

//----------------------------------------------
void LungsMesh::defineBumpMapShaderInput(GLint shader_id)
//----------------------------------------------
{
	SoftTissueMesh::defineBumpMapShaderInput(shader_id);
}

//----------------------------------------------
void LungsMesh::defineColourTextureShaderInput(GLint shader_id)
//----------------------------------------------
{
	GLfloat second_colour[] = {0.4,0.03,0.03,1.};

	GLuint handle(glGetUniformLocation(shader_id, "g_second_colour"));
	glUniform4fv(handle, 1, &second_colour[0]);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	SoftTissueMesh::defineColourTextureShaderInput(shader_id);
}

//----------------------------------------------
void LungsMesh::setMatrix(Matrix4x4<float> const& matrix)
//----------------------------------------------
{
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented.");
}

} // namespace gVirtualXRay
