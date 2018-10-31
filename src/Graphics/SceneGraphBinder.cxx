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

#include <algorithm> //For std::min and std::max Windows only
#include <cfloat> //For FLT_MAX

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
		m_unit_of_length(0.0),
		m_root_node("root")
{}





SceneGraphBinder::SceneGraphBinder(const SceneGraphBinder& aSceneGraph):
		m_unit_of_length(aSceneGraph.m_unit_of_length),
        m_root_node("root")
{
}


SceneGraphBinder::SceneGraphBinder(const std::string& aFileName, RATIONAL_NUMBER aUnitOfLength):
        m_root_node("root")
{
	loadSceneGraph(aFileName.data(), aUnitOfLength);
}


SceneGraphBinder::SceneGraphBinder(const char* aFileName, RATIONAL_NUMBER aUnitOfLength):
                m_root_node("root")
{
	loadSceneGraph(aFileName, aUnitOfLength);
}


SceneGraphBinder::~SceneGraphBinder()
{}


SceneGraphBinder& SceneGraphBinder::operator=(const SceneGraphBinder& aSceneGraph)
{
	m_unit_of_length = aSceneGraph.m_unit_of_length;
    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented");

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


void SceneGraphBinder::clear()
{
	m_root_node = SceneGraphNode("root");
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

	SceneGraphNode* p_node = getNode(aNodeLabel);
	if (p_node)
	{
	    p_node->applyLocalTransformation(MATRIX4::buildScaleMatrix(aScalingFactorSet));
	}
}


//--------------------------------------------------------------------
void SceneGraphBinder::rotate(const std::string& aNodeLabel,
		                      float aRotationAngleInDegrees,
							  const VEC3& aRotationAxis)
//--------------------------------------------------------------------
{
	rotate(aNodeLabel.data(), aRotationAngleInDegrees, aRotationAxis);

    SceneGraphNode* p_node = getNode(aNodeLabel);
    if (p_node)
    {
        p_node->applyLocalTransformation(MATRIX4::buildRotationMatrix(aRotationAngleInDegrees, aRotationAxis));
    }
}


void SceneGraphBinder::translate(const std::string& aNodeLabel,
									 const VEC3& aTranslationVector)
{
	translate(aNodeLabel.data(), aTranslationVector);

    SceneGraphNode* p_node = getNode(aNodeLabel);
    if (p_node)
    {
        p_node->applyLocalTransformation(MATRIX4::buildTranslationMatrix(aTranslationVector));
    }
}


MATRIX4 SceneGraphBinder::getLocalTransformationMatrix(const std::string& aNodeLabel) const
{
	return (getLocalTransformationMatrix(aNodeLabel.data()));
}


MATRIX4 SceneGraphBinder::getWorldTransformationMatrix(const std::string& aNodeLabel) const
{
	return (getWorldTransformationMatrix(aNodeLabel.data()));
}


void SceneGraphBinder::display(GLint aShaderID)
{
    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented, Franck is working on it");
	/*for (std::map<std::string, PolygonMesh>::iterator ite = getPolygonMeshSet().begin();
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
	}*/
}


void SceneGraphBinder::updateMatrices()
{
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented");
	//throw Exception(__FILE__, __FUNCTION__, __LINE__, "Do not call");
}


void SceneGraphBinder::scale(const char* aNodeLabel,
		const VEC3& aScalingFactorSet)
{
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented");
	// Find the node
	/*std::map<std::string, PolygonMesh>::iterator ite = m_p_polygon_mesh_set.find(aNodeLabel);

	// The node has been found
	if (ite != m_p_polygon_mesh_set.end())
	{
		ite->second.scale(aScalingFactorSet[0], aScalingFactorSet[1], aScalingFactorSet[2]);
		updateMatrices();
	}
	// The node has not been found
	else
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unknown node");
	}*/
}


void SceneGraphBinder::rotate(const char* aNodeLabel,
		float aRotationAngleInDegrees,
		const VEC3& aRotationAxis)
{
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented");
	// Find the node
	/*std::map<std::string, PolygonMesh>::iterator ite = m_p_polygon_mesh_set.find(aNodeLabel);

	// The node has been found
	if (ite != m_p_polygon_mesh_set.end())
	{
		ite->second.rotate(aRotationAxis, aRotationAngleInDegrees);
		updateMatrices();
	}
	// The node has not been found
	else
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unknown node");
	}*/
}


void SceneGraphBinder::translate(const char* aNodeLabel,
		const VEC3& aTranslationVector)
{
    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented, Franck is working on it");

	// Find the node
	/*std::map<std::string, PolygonMesh>::iterator ite = m_p_polygon_mesh_set.find(aNodeLabel);

	// The node has been found
	if (ite != m_p_polygon_mesh_set.end())
	{
		ite->second.translate(aTranslationVector);
		updateMatrices();
	}
	// The node has not been found
	else
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unknown node");
	}*/
}


MATRIX4 SceneGraphBinder::getLocalTransformationMatrix(const char* aNodeLabel) const
{
    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented, Franck is working on it");
}


MATRIX4 SceneGraphBinder::getWorldTransformationMatrix(const char* aNodeLabel) const
{
    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented, Franck is working on it");
}


void SceneGraphBinder::moveToCentre()
{
    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented, Franck is working on it");
}


void SceneGraphBinder::moveToCenter()
{
    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented, Franck is working on it");
}


MATRIX4 SceneGraphBinder::getRootNodeTransformationMatrix() const
{
    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented, Franck is working on it");
}


MATRIX4 SceneGraphBinder::getNodeLocalTransformationMatrix(const char* aNodeLabel) const
{
	return (getLocalTransformationMatrix(aNodeLabel));
}


MATRIX4 SceneGraphBinder::getNodeWorldTransformationMatrix(const char* aNodeLabel) const
{
	return (getWorldTransformationMatrix(aNodeLabel));
}


void SceneGraphBinder::setRootNodeTransformationMatrix(const MATRIX4& aTransformationMatrix)
{
    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented, Franck is working on it");
}


void SceneGraphBinder::setNodeLocalTransformationMatrix(const char* aNodeLabel, const MATRIX4& aTransformationMatrix)
{
    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented, Franck is working on it");
}


std::pair<VEC3, VEC3> SceneGraphBinder::getBoundingBox()
{
	VEC3 min_corner( FLT_MAX,  FLT_MAX,  FLT_MAX);
	VEC3 max_corner(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	// Create a waiting list
    std::vector<SceneGraphNode*> p_waiting_list;

    // Add the root node to the waiting list
    p_waiting_list.push_back(&m_root_node);

    // The waiting list is not empty
    while (p_waiting_list.size())
    {
    	// Get the last element of the list
    	SceneGraphNode* p_node = p_waiting_list.back();

    	// Remove the last element of the list
    	p_waiting_list.pop_back();

    	// It is not NULL
    	if (p_node)
    	{
    	    // Add its children to the waiting list
			for (int i = 0; i < p_node->getNumberOfChildren(); ++i)
			{
				p_waiting_list.push_back(&p_node->getChild(i));
			}

			// Process the node
			min_corner[0] = std::min(min_corner[0], p_node->getWorldMinCorner()[0]);
			min_corner[1] = std::min(min_corner[1], p_node->getWorldMinCorner()[1]);
			min_corner[2] = std::min(min_corner[2], p_node->getWorldMinCorner()[2]);

			max_corner[0] = std::max(max_corner[0], p_node->getWorldMaxCorner()[0]);
			max_corner[1] = std::max(max_corner[1], p_node->getWorldMaxCorner()[1]);
			max_corner[2] = std::max(max_corner[2], p_node->getWorldMaxCorner()[2]);
    	}
    }

	return std::pair<VEC3, VEC3>(min_corner, max_corner);
}


SceneGraphNode* SceneGraphBinder::getNode(const std::string& aLabel)
{
    SceneGraphNode* p_current_node = &m_root_node;
    std::vector<SceneGraphNode*> p_waiting_list;

    while (p_current_node)
    {
        if (p_current_node->getLabel() == aLabel)
        {
            return (p_current_node);
        }
        else if (p_current_node->getNumberOfChildren())
        {
            for (int i = 0; i < p_current_node->getNumberOfChildren(); ++i)
            {
                p_waiting_list.push_back(&p_current_node->getChild(i));
            }
        }

        p_current_node = 0;

        if (p_waiting_list.size())
        {
            p_current_node = p_waiting_list.back();
            p_waiting_list.pop_back();
        }
    }

    return 0;
}


const SceneGraphNode* SceneGraphBinder::getNode(const std::string& aLabel) const
{
    const SceneGraphNode* p_current_node = &m_root_node;
    std::vector<const SceneGraphNode*> p_waiting_list;

    while (p_current_node)
    {
        if (p_current_node->getLabel() == aLabel)
        {
            return (p_current_node);
        }
        else if (p_current_node->getNumberOfChildren())
        {
            for (int i = 0; i < p_current_node->getNumberOfChildren(); ++i)
            {
                p_waiting_list.push_back(&p_current_node->getChild(i));
            }
        }

        p_current_node = 0;

        if (p_waiting_list.size())
        {
            p_current_node = p_waiting_list.back();
            p_waiting_list.pop_back();
        }
    }

    return 0;
}


void SceneGraphBinder::addPolygonMesh(const std::string& aLabel,
                                      const PolygonMesh& aPolygonMesh,
                                      const VEC3& aPosition,
                                      const std::string& aParent)
{
    SceneGraphNode* p_parent = getNode(aParent);

    if (!p_parent)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "The parent node does not exist.");
    }

    p_parent->addChild(aLabel, aPolygonMesh, aPosition);
}
