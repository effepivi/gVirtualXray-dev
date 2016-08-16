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
	float min_corner[] = {m_min_corner.getX(), m_min_corner.getY(), m_min_corner.getZ()};

	GLuint handle(glGetUniformLocation(shader_id, "g_min_corner"));
	glUniform3fv(handle, 1, &min_corner[0]);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	float max_corner[] = {m_max_corner.getX(), m_max_corner.getY(), m_max_corner.getZ()};

	handle = glGetUniformLocation(shader_id, "g_max_corner");
	glUniform3fv(handle, 1, &max_corner[0]);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);


	//matrix for the bump map (to create the lines on the sides)
	VEC3 translation_vector = getMinCorner() + (getMaxCorner() - getMinCorner()) / 2.0;
	Matrix4x4<GLfloat> matrix(Matrix4x4<GLfloat>::buildTranslationMatrix(translation_vector));
	matrix *= Matrix4x4<GLfloat>::buildRotationMatrix(30,VEC3(1.,0.,0.));
	matrix *= Matrix4x4<GLfloat>::buildTranslationMatrix(-translation_vector);

	handle = glGetUniformLocation(shader_id, "g_matrix");
	glUniformMatrix4fv(handle, 1, GL_FALSE, matrix.get());
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
