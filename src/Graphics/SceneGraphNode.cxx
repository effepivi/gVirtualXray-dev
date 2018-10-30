#ifndef __SceneGraphNode_h
#include "gVirtualXRay/SceneGraphNode.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


SceneGraphNode::SceneGraphNode(const std::string& aLabel,
                               const SceneGraphNode* apParent):
        m_label(aLabel),
        m_p_parent(apParent)
{
	updateWorldMatrix();
}


SceneGraphNode::SceneGraphNode(const std::string& aLabel,
                               const PolygonMesh& aPolygonMesh,
                               const VEC3& aPosition,
                               const SceneGraphNode* apParent):
        m_label(aLabel),
        m_geometry(aPolygonMesh),
        m_position(aPosition),
        m_p_parent(apParent)
{
	updateWorldMatrix();
}


SceneGraphNode::~SceneGraphNode()
{
	std::cout << "Delete node: " << m_label << std::endl;
}


void SceneGraphNode::display()
{
	m_geometry.display();
}


void SceneGraphNode::displayWireFrame()
{
	m_geometry.displayWireFrame();
}

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


PolygonMesh& SceneGraphNode::getPolygonMesh()
{
    return (m_geometry);
}


const PolygonMesh& SceneGraphNode::getPolygonMesh() const
{
    return (m_geometry);
}


MATRIX4 SceneGraphNode::getLocalTransformationMatrix() const
{
    return (m_local_transformation * MATRIX4::buildTranslationMatrix(m_position));
}


MATRIX4 SceneGraphNode::getWorldTransformationMatrix() const
{
    return (m_global_transformation);
}


std::map<RATIONAL_NUMBER, VEC3> SceneGraphNode::rayIntersect(
		const VEC3& aRayOrigin,
		const VEC3& aRayDirection,
		const MATRIX4& aTransformationMatrix) const
{
	return m_geometry.rayIntersect(
			aRayOrigin,
			aRayDirection,
			getWorldTransformationMatrix() * aTransformationMatrix);
}


void SceneGraphNode::setLocalTransformation(const MATRIX4& aMatrix)
{
	m_local_transformation = aMatrix;
	updateWorldMatrix();
}


void SceneGraphNode::applyLocalTransformation(const MATRIX4& aMatrix)
{
    m_local_transformation = aMatrix * m_local_transformation;
	updateWorldMatrix();
}


void SceneGraphNode::addChild(const std::string& aLabel)
{
	m_p_children.push_back(SceneGraphNode(aLabel, this));
}


void SceneGraphNode::addChild(const std::string& aLabel,
                              const PolygonMesh& aPolygonMesh,
                              const VEC3& aPosition)
{
    m_p_children.push_back(SceneGraphNode(aLabel, aPolygonMesh, aPosition, this));
}


VEC3 SceneGraphNode::getLocalMinCorner() const
{
	return (getLocalTransformationMatrix() * m_geometry.getLocalMinCorner());
}


VEC3 SceneGraphNode::getLocalMaxCorner() const
{
	return (getLocalTransformationMatrix() * m_geometry.getLocalMaxCorner());
}


VEC3 SceneGraphNode::getWorldMinCorner() const
{
	return (m_global_transformation * getLocalMinCorner());
}


VEC3 SceneGraphNode::getWorldMaxCorner() const
{
	return (m_global_transformation * getLocalMaxCorner());
}


const PhotonCrossSection& SceneGraphNode::getPhotonCrossSection() const
{
	return (m_geometry.getPhotonCrossSection());
}


SceneGraphNode& SceneGraphNode::getChild(const std::string& aLabel)
{
    for (std::vector<SceneGraphNode>::iterator ite = m_p_children.begin();
    		ite != m_p_children.end();
    		++ite)
    {
    	if (ite->m_label == aLabel)
    	{
    		return *ite;
    	}
    }

    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Node not found");
}


const SceneGraphNode& SceneGraphNode::getChild(const std::string& aLabel) const
{
    for (std::vector<SceneGraphNode>::const_iterator ite = m_p_children.begin();
    		ite != m_p_children.end();
    		++ite)
    {
    	if (ite->m_label == aLabel)
    	{
    		return *ite;
    	}
    }

    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Node not found");
}


void SceneGraphNode::updateWorldMatrix()
{
	m_global_transformation = getLocalTransformationMatrix();

	const SceneGraphNode* p_parent = m_p_parent;

	while (p_parent)
	{
		m_global_transformation = p_parent->getLocalTransformationMatrix() * m_global_transformation;
		p_parent = p_parent->m_p_parent;
	}
}

/*
SceneGraphNode& SceneGraphNode::operator=(const SceneGraphNode& aNode)
{
	// The current node has children
	// They need to be deleted
	if (m_p_children.size())
	{
		// Add all the children of the node to the temp list
		std::vector<SceneGraphNode*> p_temp_pool_of_nodes;
		for (std::vector<SceneGraphNode>::iterator ite = m_p_children.begin();
				ite != m_p_children.end();
				++ite)
		{
			p_temp_pool_of_nodes.push_back(&(*ite));
		}

		// Process until the list is empty
		while (p_temp_pool_of_nodes.size())
		{

		}
	}

	return (*this);
}
*/

