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


SceneGraphNode::SceneGraphNode(const std::string& aLabel,
                               const SceneGraphNode* apParent):
        m_label(aLabel),
        m_p_parent(apParent)
{}


SceneGraphNode::SceneGraphNode(const std::string& aLabel,
                               const PolygonMesh& aPolygonMesh,
                               const VEC3& aPosition,
                               const SceneGraphNode* apParent):
        m_label(aLabel),
        m_geometry(aPolygonMesh),
        m_position(aPosition),
        m_p_parent(apParent)
{}


const SceneGraphNode* SceneGraphNode::getParent() const
{
    return (m_p_parent);
}


unsigned int SceneGraphNode::getNumberOfChildren() const
{
    return (m_p_children.size());
}


SceneGraphNode& SceneGraphNode::getChild(unsigned int i)
{
    if (i >= m_p_children.size())
    {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    return (m_p_children[i]);
}


const SceneGraphNode& SceneGraphNode::getChild(unsigned int i) const
{
    if (i >= m_p_children.size())
    {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    return (m_p_children[i]);
}



const std::string& SceneGraphNode::getLabel() const
{
    return (m_label);
}


const PolygonMesh& SceneGraphNode::getPolygonMesh() const
{
    return (m_geometry);
}


MATRIX4 SceneGraphNode::getLocalTransformationMatrix() const
{
    return (MATRIX4::buildTranslationMatrix(m_position) * m_partial_local_transformation);
}


MATRIX4 SceneGraphNode::getWorldTransformationMatrix() const
{
    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented, Franck is working on it");
}


void SceneGraphNode::applyLocalTransformation(const MATRIX4& aMatrix)
{
    m_partial_local_transformation = aMatrix * m_partial_local_transformation;
}


void SceneGraphNode::addChild(const std::string& aLabel,
                              const PolygonMesh& aPolygonMesh,
                              const VEC3& aPosition)
{
    m_p_children.push_back(SceneGraphNode(aLabel, aPolygonMesh, aPosition, this));
}












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
	m_p_polygon_mesh_set.clear();
	m_root_node.m_partial_local_transformation = MATRIX4();
	m_root_node.m_position = VEC3();
	m_root_node.m_p_children.clear();
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


void SceneGraphBinder::updateMatrices()
{
	//throw Exception(__FILE__, __FUNCTION__, __LINE__, "Do not call");
}


void SceneGraphBinder::scale(const char* aNodeLabel,
		const VEC3& aScalingFactorSet)
{
	// Find the node
	std::map<std::string, PolygonMesh>::iterator ite = m_p_polygon_mesh_set.find(aNodeLabel);

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
	}
}


void SceneGraphBinder::rotate(const char* aNodeLabel,
		float aRotationAngleInDegrees,
		const VEC3& aRotationAxis)
{
	// Find the node
	std::map<std::string, PolygonMesh>::iterator ite = m_p_polygon_mesh_set.find(aNodeLabel);

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
	}
}


void SceneGraphBinder::translate(const char* aNodeLabel,
		const VEC3& aTranslationVector)
{
	// Find the node
	std::map<std::string, PolygonMesh>::iterator ite = m_p_polygon_mesh_set.find(aNodeLabel);

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
	}
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


MATRIX4 SceneGraphBinder::getRootNodeTransformationMatrix() const
{
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Do not call");
}


MATRIX4 SceneGraphBinder::getNodeTransformationMatrix(const char* aNodeLabel) const
{
	return (getTransformationMatrix(aNodeLabel));
}


void SceneGraphBinder::setRootNodeTransformationMatrix(const MATRIX4& aTransformationMatrix)
{
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Do not call");
}


void SceneGraphBinder::setNodeTransformationMatrix(const char* aNodeLabel, const MATRIX4& aTransformationMatrix)
{
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Do not call");
}


std::pair<VEC3, VEC3> SceneGraphBinder::getBoundingBox() const
{
	VEC3 min_corner;
	VEC3 max_corner;

	int i = 0;
	for (std::map<std::string, PolygonMesh>::const_iterator ite = getPolygonMeshSet().begin();
			ite != getPolygonMeshSet().end();
			++ite, ++i)
	{
		if (i == 0)
		{
			min_corner = ite->second.getMinCorner();
			max_corner = ite->second.getMaxCorner();
		}
		else
		{
			min_corner[0] = std::min(min_corner[0], ite->second.getMinCorner()[0]);
			min_corner[1] = std::min(min_corner[1], ite->second.getMinCorner()[1]);
			min_corner[2] = std::min(min_corner[2], ite->second.getMinCorner()[2]);

			max_corner[0] = std::max(max_corner[0], ite->second.getMaxCorner()[0]);
			max_corner[1] = std::max(max_corner[1], ite->second.getMaxCorner()[1]);
			max_corner[2] = std::max(max_corner[2], ite->second.getMaxCorner()[2]);
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
