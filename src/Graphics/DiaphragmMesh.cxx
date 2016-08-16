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

//******************************************************************************
//	Include
//******************************************************************************
#ifdef HAS_GLEW
#include "GL/glew.h"
#endif

#include "gVirtualXRay/DiaphragmMesh.h"


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {

//----------------------------------------------
DiaphragmMesh::DiaphragmMesh():
//----------------------------------------------
		SoftTissueMesh()
//----------------------------------------------
{
	getMaterial().setDiffuseColour(0.58,0.2,0.15,1.);
	getMaterial().setAmbientColour(0.58,0.22,0.17,1.);
	m_has_tendon=1;
}

//----------------------------------------------
DiaphragmMesh::~DiaphragmMesh()
//----------------------------------------------
{
	// TODO Auto-generated destructor stub
}

//----------------------------------------------
void DiaphragmMesh::defineBumpMapShaderInput(int shader_id)
//----------------------------------------------
{
	//matrix for the bump map (to know where the center is)
	VEC3 translation_vector = getMinCorner() + (getMaxCorner() - getMinCorner()) / 2.0;
	Matrix4x4<GLfloat> matrix(Matrix4x4<GLfloat>::buildTranslationMatrix(-translation_vector));

	GLuint handle(glGetUniformLocation(shader_id, "g_matrix"));
    glUniformMatrix4fv(handle, 1, GL_FALSE, matrix.get());
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	SoftTissueMesh::defineBumpMapShaderInput(shader_id);
}

//----------------------------------------------
void DiaphragmMesh::defineColourTextureShaderInput(int shader_id)
 //----------------------------------------------
{
	GLfloat second_colour[] = {1.,0.73,0.6,1.};
	GLuint handle(glGetUniformLocation(shader_id, "g_second_colour"));
	glUniform4fv(handle, 1, &second_colour[0]);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);


	//Bind the texture for bump mapping in texture unit 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_bump_map_texture);

	handle = glGetUniformLocation(shader_id, "g_bump_map");
	glUniform1i(handle,1);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	SoftTissueMesh::defineColourTextureShaderInput(shader_id);
}


} // namespace gVirtualXRay
