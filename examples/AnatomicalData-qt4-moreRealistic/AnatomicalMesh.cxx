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
#include <map>

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

struct VectorComparer
{
	// Return true if aLeftNode is less than aRightNode
    bool operator()(const VEC3 & aLeftNode, const VEC3 & aRightNode) const
    {
    	double epsilon(EPSILON);

    	if (fabs(aLeftNode.getX() - aRightNode.getX()) > epsilon)
    	{
    		if (aLeftNode.getX() < aRightNode.getX())
    		{
    			return (true);
    		}
    		else
    		{
    			return (false);
    		}
    	}

    	if (fabs(aLeftNode.getY() - aRightNode.getY()) > epsilon)
    	{
    		if (aLeftNode.getY() < aRightNode.getY())
    		{
    			return (true);
    		}
    		else
    		{
    			return (false);
    		}
    	}

    	if (fabs(aLeftNode.getZ() - aRightNode.getZ()) > epsilon)
    	{
    		if (aLeftNode.getZ() < aRightNode.getZ())
    		{
    			return (true);
    		}
    		else
    		{
    			return (false);
    		}
    	}

    	return (false);
    }
};


class TempRecord
{
public:
	inline const TempRecord& operator +=(const VEC3& aNormal)
	{
		m_normal += aNormal;

		return (*this);
	}


	inline const TempRecord& operator =(unsigned int anID)
	{
		m_id = anID;

		return (*this);
	}

	VEC3 m_normal;
	unsigned int m_id;
};


//----------------------------------------------
void AnatomicalMesh::shareNormalVectors()
//----------------------------------------------
{
	// Add the vertices to the hash table
	std::map<VEC3, TempRecord, VectorComparer> hash_table;
	//Compute the vertices normal vectors (sum of the adjacent faces normal vectors)
	for (unsigned int i(0); i < m_number_of_vertices; ++i)
	{
		VEC3 current_vertex(getVertex(i));
		VEC3 normal(getFaceNormal(i));

		hash_table[current_vertex].m_normal += normal;
	}

	//set the vertices normal data
	m_p_vertex_normal_set.clear();
	m_p_vertex_normal_set.resize(m_number_of_vertices * 3);

	for (unsigned int i(0); i < m_number_of_vertices; ++i)
	{
		VEC3 current_vertex(getVertex(i));
		VEC3 current_normal(hash_table[current_vertex].m_normal);

		m_p_vertex_normal_set[i*3] =  current_normal.getX();
		m_p_vertex_normal_set[i*3+1] =  current_normal.getY();
		m_p_vertex_normal_set[i*3+2] =  current_normal.getZ();
	}


	// Reset the normal per face set
	m_p_face_normal_set.clear();

	// Update the VBO
	normaliseNormals();

	updateVBO(m_vbo->getBufferUsageHint(), m_vbo->getPrimitiveType());

}

} // namespace gVirtualXRay
