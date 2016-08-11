/**
********************************************************************************
*
*	@file		AnatomicalMesh.cxx
*
*	@brief      Class to handle a polygon mesh for tissue.
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
#ifndef ANATOMICAL_MESH_H
#include "gVirtualXRay/AnatomicalMesh.h"
#endif

//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay{

//----------------------------------------
AnatomicalMesh::AnatomicalMesh():
//----------------------------------------
		PolygonMesh(),
		m_use_texture(0)
//----------------------------------------
{
	getMaterial().setSpecularColour(1.,1.,1.,1.);
}

//----------------------------------------
AnatomicalMesh::~AnatomicalMesh()
//----------------------------------------
{
	// TODO Auto-generated destructor stub
}

//----------------------------------------------
void AnatomicalMesh::defineDisplayShaderInput(GLint shader_id)
//----------------------------------------------
{
	// Get the material
	Material& material(getMaterial());

	// Define colours in shader
	GLuint handle(glGetUniformLocation(shader_id, "material_ambient"));
	glUniform4fv(handle, 1, &(material.getAmbientColour()[0]));
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(shader_id, "material_diffuse");
	glUniform4fv(handle, 1, &(material.getDiffuseColour()[0]));
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(shader_id, "material_specular");
	glUniform4fv(handle, 1, &(material.getSpecularColour()[0]));
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(shader_id, "material_shininess");
	glUniform1f(handle, material.getShininess());
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(shader_id, "g_use_texture");
	glUniform1iv(handle, 1, &m_use_texture);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}

} // namespace gVirtualXRay
