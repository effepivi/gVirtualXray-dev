/**
********************************************************************************
*
*   @file       SceneGraphBinder.cxx
*
*   @brief      Class to handle scenegraphes.
*
*   @version    1.0
*
*   @date       16/07/2018
*
*   @author     Dr Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*   http://www.fpvidal.net/, Jul 2018, 2018, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifdef HAS_GLEW
#include <GL/glew.h>
#endif

#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef __SceneGraphBinder_h
#include "gVirtualXRay/SceneGraphBinder.h"
#endif

// For types such as VEC3, MATRIX4
#ifndef __Types_h
#include "gVirtualXRay/Types.h"
#endif

// To initialise GLEW, to define matrix stacks, to define state stacks, etc.
#ifndef __OpenGLUtilities_h
#include "gVirtualXRay/OpenGLUtilities.h"
#endif



//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


SceneGraphBinder::SceneGraphBinder():
		m_unit_of_length(0.0)
{}


SceneGraphBinder::SceneGraphBinder(const SceneGraphBinder& aSceneGraph):
		m_unit_of_length(aSceneGraph.m_unit_of_length)
{
}


SceneGraphBinder::SceneGraphBinder(const std::string& aFileName, RATIONAL_NUMBER aUnitOfLength)
{
	loadSceneGraph(aFileName.data(), aUnitOfLength);
}


SceneGraphBinder::SceneGraphBinder(const char* aFileName, RATIONAL_NUMBER aUnitOfLength)
{
	loadSceneGraph(aFileName, aUnitOfLength);
}


SceneGraphBinder::~SceneGraphBinder()
{}


SceneGraphBinder& SceneGraphBinder::operator=(const SceneGraphBinder& aSceneGraph)
{
	m_unit_of_length = aSceneGraph.m_unit_of_length;
	return (*this);
}


void SceneGraphBinder::loadSceneGraph(const char* aFileName, RATIONAL_NUMBER aUnitOfLength)
{
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Do not call");
}


void SceneGraphBinder::loadSceneGraph(const std::string& aFileName, RATIONAL_NUMBER aUnitOfLength)
{
	loadSceneGraph(aFileName.data(), aUnitOfLength);
}


const std::map<std::string, PolygonMesh>& SceneGraphBinder::getPolygonMeshSet() const
{
	return (m_p_polygon_mesh_set);
}


std::map<std::string, PolygonMesh>& SceneGraphBinder::getPolygonMeshSet()
{
	return (m_p_polygon_mesh_set);
}

/*
const PolygonMesh& SceneGraphBinder::getPolygonMesh(const std::string& aLabel) const
{
	return (m_p_polygon_mesh_set[aLabel]);
}


const PolygonMesh& SceneGraphBinder::getPolygonMesh(const char* aLabel) const
{
	return (m_p_polygon_mesh_set[std::string(aLabel)]);
}


PolygonMesh& SceneGraphBinder::getPolygonMesh(const std::string& aLabel)
{
	return (m_p_polygon_mesh_set[aLabel]);
}


PolygonMesh& SceneGraphBinder::getPolygonMesh(const char* aLabel)
{
	return (m_p_polygon_mesh_set[std::string(aLabel)]);
}*/


void SceneGraphBinder::scale(const std::string& aNodeLabel,
		const VEC3& aScalingFactorSet)
{
	scale(aNodeLabel.data(), aScalingFactorSet);
}


//--------------------------------------------------------------------
void SceneGraphBinder::rotate(const std::string& aNodeLabel,
		                      float aRotationAngleInDegrees,
							  const VEC3& aRotationAxis)
//--------------------------------------------------------------------
{
	rotate(aNodeLabel.data(), aRotationAngleInDegrees, aRotationAxis);
}


void SceneGraphBinder::translate(const std::string& aNodeLabel,
									 const VEC3& aTranslationVector)
{
	translate(aNodeLabel.data(), aTranslationVector);
}


MATRIX4 SceneGraphBinder::getTransformationMatrix(const std::string& aNodeLabel) const
{
	return (getTransformationMatrix(aNodeLabel.data()));
}


void SceneGraphBinder::display(GLint aShaderID)
{
	for (std::map<std::string, PolygonMesh>::iterator ite = getPolygonMeshSet().begin();
			ite != getPolygonMeshSet().end();
			++ite)
	{
		// Store the current transformation matrix
		pushModelViewMatrix();

		g_current_modelview_matrix *= ite->second.getTransformationMatrix();

		applyModelViewMatrix();

		// Get the material
		Material& material(ite->second.getMaterial());

		// Define colours inshader
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

		ite->second.display();

		// Restore the current transformation matrix
		popModelViewMatrix();
	}
}


void SceneGraphBinder::SceneGraphBinder::updateMatrices()
{
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Do not call");
}


void SceneGraphBinder::SceneGraphBinder::scale(const char* aNodeLabel,
		const VEC3& aScalingFactorSet)
{
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Do not call");
}


void SceneGraphBinder::SceneGraphBinder::rotate(const char* aNodeLabel,
		float aRotationAngleInDegrees,
		const VEC3& aRotationAxis)
{
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Do not call");
}


void SceneGraphBinder::SceneGraphBinder::translate(const char* aNodeLabel,
		const VEC3& aRotationAxis)
{
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Do not call");
}


MATRIX4 SceneGraphBinder::getTransformationMatrix(const char* aNodeLabel) const
{
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Do not call");
}


void SceneGraphBinder::moveToCentre()
{
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Do not call");
}


void SceneGraphBinder::moveToCenter()
{
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Do not call");
}
