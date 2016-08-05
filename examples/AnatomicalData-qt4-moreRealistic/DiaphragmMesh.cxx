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
	m_has_tendon=1;
}

//----------------------------------------------
DiaphragmMesh::~DiaphragmMesh()
//----------------------------------------------
{
	// TODO Auto-generated destructor stub
}

//----------------------------------------------
void DiaphragmMesh::defineBumpMapShaderInput()
//----------------------------------------------
{
	GLint shader_id(m_bumpMapShader.getProgramHandle());

	SoftTissueMesh::defineBumpMapShaderInput();
}

//----------------------------------------------
void DiaphragmMesh::defineColourShaderInput()
 //----------------------------------------------
{
	GLint shader_id(m_textureShader.getProgramHandle());

	GLfloat second_colour[] = {0.84,0.77,0.6,1.};
	GLuint handle(glGetUniformLocation(shader_id, "g_second_colour"));
	glUniform4fv(handle, 1, &second_colour[0]);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);


	//Bind the texture for bump mapping in texture unit 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_bumpMapTexture);

	handle = glGetUniformLocation(shader_id, "bumpMap");
	glUniform1i(handle,1);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	SoftTissueMesh::defineColourShaderInput();
}

} // namespace gVirtualXRay
