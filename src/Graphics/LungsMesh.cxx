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
#ifdef HAS_GLEW
#include "GL/glew.h"
#endif

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
	m_is_lung=1;
}


//----------------------------------------------
LungsMesh::~LungsMesh()
//----------------------------------------------
{
	// TODO Auto-generated destructor stub
}

//----------------------------------------------
void LungsMesh::defineBumpMapShaderInput(int shader_id)
//----------------------------------------------
{
	float max_height(m_max_corner.getZ());
	GLuint handle(glGetUniformLocation(shader_id, "g_max_height"));
	glUniform1fv(handle, 1, &max_height);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	float min_height(m_min_corner.getZ());
	handle = glGetUniformLocation(shader_id, "g_min_height");
	glUniform1fv(handle, 1, &min_height);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	SoftTissueMesh::defineBumpMapShaderInput(shader_id);
}

//----------------------------------------------
void LungsMesh::defineColourTextureShaderInput(int shader_id)
//----------------------------------------------
{
	GLfloat second_colour[] = {0.4,0.03,0.03,1.};

	GLuint handle(glGetUniformLocation(shader_id, "g_second_colour"));
	glUniform4fv(handle, 1, &second_colour[0]);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	SoftTissueMesh::defineColourTextureShaderInput(shader_id);
}

} // namespace gVirtualXRay
