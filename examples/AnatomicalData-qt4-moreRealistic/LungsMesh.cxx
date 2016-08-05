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
	m_use_second_colour=1;
}


//----------------------------------------------
LungsMesh::~LungsMesh()
//----------------------------------------------
{
	// TODO Auto-generated destructor stub
}

//----------------------------------------------
void LungsMesh::defineBumpMapShaderInput()
//----------------------------------------------
{
	SoftTissueMesh::defineBumpMapShaderInput();
}

//----------------------------------------------
void LungsMesh::defineColourShaderInput()
//----------------------------------------------
{
	GLfloat second_colour[] = {0.4,0.03,0.03,1.};

	GLint shader_id(m_textureShader.getProgramHandle());
	GLuint handle(glGetUniformLocation(shader_id, "g_second_colour"));
	glUniform4fv(handle, 1, &second_colour[0]);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	SoftTissueMesh::defineColourShaderInput();
}


} // namespace gVirtualXRay
