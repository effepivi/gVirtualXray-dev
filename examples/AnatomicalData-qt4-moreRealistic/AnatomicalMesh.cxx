/**
********************************************************************************
*
*	@file		AnatomicalMesh.h
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
#include "AnatomicalMesh.h"
#endif

//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay{

//----------------------------------------
AnatomicalMesh::AnatomicalMesh():
//----------------------------------------
		PolygonMesh()
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
void AnatomicalMesh::defineShaderInput(GLint aShaderID)
//----------------------------------------------
{
	// Get the material
	Material& material(getMaterial());

	// Define colours in shader
	GLuint handle(glGetUniformLocation(aShaderID, "material_ambient"));
	glUniform4fv(handle, 1, &(material.getAmbientColour()[0]));
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(aShaderID, "material_diffuse");
	glUniform4fv(handle, 1, &(material.getDiffuseColour()[0]));
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(aShaderID, "material_specular");
	glUniform4fv(handle, 1, &(material.getSpecularColour()[0]));
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(aShaderID, "material_shininess");
	glUniform1f(handle, material.getShininess());
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

}

} // namespace gVirtualXRay
