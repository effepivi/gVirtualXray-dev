/**
********************************************************************************
*
*	@file		TriangleMesh.inl
*
*	@brief		Class to handle 3D triangle meshes.
*
*	@version	1.0
*
*	@date		07/11/2013
*
*	@author		Dr Franck P. Vidal
*
*	@section	License
*				BSD 3-Clause License.
*
*				For details on use and redistribution please refer 
*				to http://opensource.org/licenses/BSD-3-Clause
*
*	@section	Copyright
*				(c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net), 
*				http://www.fpvidal.net/, Dec 2013, 2013, version 1.0, 
*				BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#include <iostream>

#ifndef OUT_OF_BOUNDS_EXCEPTION_H
#include "OutOfBoundsException.h"
#endif


//******************************************************************************
//	define
//******************************************************************************


//******************************************************************************
//	namespace
//******************************************************************************
namespace Graphics {


//----------------------------------
inline TriangleMesh::TriangleMesh():
//----------------------------------
	m_use_hounsfield_value(true),
	m_hounsfield_value(0),
	m_attenuation_coefficient(0),
	m_bounding_box_diagonal(0)
//----------------------------------
{
}


//----------------------------------
inline TriangleMesh::~TriangleMesh()
//----------------------------------
{
	// Make sure the VBO and the dynamically allocated memory are released
	reset();
}


//-------------------------------
inline void TriangleMesh::reset()
//-------------------------------
{
	// Reset class members to their default values 
#ifdef USE_VBO
	m_vbo.reset();
#endif
	
	m_use_hounsfield_value = true;
	m_hounsfield_value = 0;
	m_attenuation_coefficient = 0;

	m_bounding_box_diagonal = 0;

	m_min_corner = VEC3();
	m_max_corner = VEC3();
	
	m_p_face_normal_set.clear();
	m_p_vertex_normal_set.clear();
	m_p_vertex_set.clear();
	m_p_index_set.clear();
}


//----------------------------------------------------
inline void TriangleMesh::display(bool aVBOFlag) const
//----------------------------------------------------
{
	// Chose the normal array
	const std::vector<GLfloat>* p_normal(0);
	
	// By default, chose the per vertex one 
	if (m_p_vertex_normal_set.size())
	{
		p_normal = &m_p_vertex_normal_set;
	}
	else
	{
		p_normal = &m_p_face_normal_set;
	}
	
	
	// Display the polygon mesh using a VBO
	if (m_vbo.isReady() && aVBOFlag)
	{
		m_vbo.display();
	}
	// Display the polygon mesh using immediate mode rendering
	else
	{
		// There is an index
		if (m_p_index_set.size())
		{
			glBegin(GL_TRIANGLES);
			for (int i(0); i < getTriangleNumber(); ++i)
			{
				int id_0(m_p_index_set[i * 3 + 0]);
				int id_1(m_p_index_set[i * 3 + 1]);
				int id_2(m_p_index_set[i * 3 + 2]);

				glNormal3fv(&(*p_normal)[id_0 * 3]);
				glVertex3fv(&m_p_vertex_set[id_0 * 3]);

				glNormal3fv(&(*p_normal)[id_1 * 3]);
				glVertex3fv(&m_p_vertex_set[id_1 * 3]);

				glNormal3fv(&(*p_normal)[id_2 * 3]);
				glVertex3fv(&m_p_vertex_set[id_2 * 3]);
			}
			glEnd();
		}
		// There is not any index
		else
		{
			glBegin(GL_TRIANGLES);
			for (int i(0); i < getTriangleNumber() * 3; ++i)
			{
				glNormal3fv(&(*p_normal)[i * 3]);
				glVertex3fv(&m_p_vertex_set[i * 3]);		
			}
			glEnd();
		}
	}
	
	// Check the OpenGL error status
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//----------------------------------------------------------------------
inline void TriangleMesh::displayVertexNormal(GLfloat aNormalSize) const
//----------------------------------------------------------------------
{
	// There are as many normals as there are vertices
	if (getVertexNormalNumber() == getVertexNumber())
	{
		glBegin(GL_LINES);
		for (int i(0); i < getVertexNumber(); ++i)
		{
			VEC3 vertex(getVertex(i));
			VEC3 normal(getVertexNormal(i));
			normal.normalize();

			normal = vertex + normal * aNormalSize;
			
			glVertex3f(vertex.getX(), vertex.getY(), vertex.getZ());		
			glVertex3f(normal.getX(), normal.getY(), normal.getZ());		
		}
		glEnd();
	}
	// There are not as many normals as there are vertices
	else
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "There are not as many normals as there are vertices.");
	}
}


//----------------------------------------------------------
inline void TriangleMesh::setFilename(const char* aFilename)
//----------------------------------------------------------
{
	// Store the name of the file that contains the polygon mesh
	m_filename = aFilename;
}


//-----------------------------------------------------------------
inline void TriangleMesh::setFilename(const std::string& aFilename)
//-----------------------------------------------------------------
{
	// Store the name of the file that contains the polygon mesh
	m_filename = aFilename;
}


//---------------------------------------------------------
inline const std::string& TriangleMesh::getFilename() const
//---------------------------------------------------------
{
	// Return the name of the file that contains the polygon mesh
	return (m_filename);
}


//--------------------------------
inline bool TriangleMesh::useVBO()
//--------------------------------
{
#ifdef USE_VBO
	// There is no data to store in a VBO
	if (m_p_vertex_set.size() && m_p_normal_set.size())
	{
		// Create a VBO if possible
		m_vbo.set(getVertexNumber(), &m_p_vertex_set[0], getNormalNumber(), &m_p_normal_set[0]);
	}
	return (m_vbo.isReady());
#endif // USE_VBO
	
	return (false);
}


//---------------------------------------------------------
inline bool TriangleMesh::getUseHounsfieldValueFlag() const
//---------------------------------------------------------
{
	// Return true if the Hounsfield value is used, false if the attenuation coefficient is used
	return (m_use_hounsfield_value);
}


//-----------------------------------------------------------------------------------
inline void TriangleMesh::setHounsfieldValue(const RATIONAL_NUMBER& aHounsfieldValue)
//-----------------------------------------------------------------------------------
{
	// Set the Hounsfield value
	m_hounsfield_value = aHounsfieldValue;

	// The Hounsfield value will be used instead of the attenuation coefficient
	m_use_hounsfield_value = true;
}


//--------------------------------------------------------------------------------------------------
inline void TriangleMesh::setAttenuationCoefficient(const RATIONAL_NUMBER& anAttenuationCoefficient)
//--------------------------------------------------------------------------------------------------
{
	// Set the attenuation coefficient
	m_attenuation_coefficient = anAttenuationCoefficient;

	// The attenuation coefficient will be used instead of the Hounsfield value
	m_use_hounsfield_value = false;
}


//-------------------------------------------------------------
inline RATIONAL_NUMBER TriangleMesh::getHounsfieldValue() const
//-------------------------------------------------------------
{
	// Return the Hounsfield value
	return (m_hounsfield_value);
}


//--------------------------------------------------------------------
inline RATIONAL_NUMBER TriangleMesh::getAttenuationCoefficient() const
//--------------------------------------------------------------------
{
	// Return the attenuation coefficient
	return (m_attenuation_coefficient);
}


//---------------------------------------------------
inline const VEC3& TriangleMesh::getMinCorner() const
//---------------------------------------------------
{
	// Return the bounding box lower corner
    return (m_min_corner);
}


//---------------------------------------------------
inline const VEC3& TriangleMesh::getMaxCorner() const
//---------------------------------------------------
{
	// Return the bounding box upper corner
	return (m_max_corner);
}


//---------------------------------------------------------
inline unsigned int TriangleMesh::getTriangleNumber() const
//---------------------------------------------------------
{
	// Return the number of triangles
	if (m_p_index_set.size())
	{
		return (m_p_index_set.size() / 3);
	}
	else
	{
		// There are 3 vertices per triangle
		// There are 3 GLfloat values per vertex
		return ((m_p_vertex_set.size() / 3) / 3);
	}
}


//-----------------------------------------------------------
inline unsigned int TriangleMesh::getFaceNormalNumber() const
//-----------------------------------------------------------
{
	// Return the number of normal vectors
	// There are 3 GLfloat values per normal
	return (m_p_face_normal_set.size() / 3);
}


//-------------------------------------------------------------
inline unsigned int TriangleMesh::getVertexNormalNumber() const
//-------------------------------------------------------------
{
	// Return the number of normal vectors
	// There are 3 GLfloat values per normal
	return (m_p_vertex_normal_set.size() / 3);
}


//-------------------------------------------------------
inline unsigned int TriangleMesh::getVertexNumber() const
//-------------------------------------------------------
{
	// Return the number of vertices
	// There are 3 GLfloat values per vertex
	return (m_p_vertex_set.size() / 3);
}


//----------------------------------------------------
inline VEC3 TriangleMesh::getVertex(int anIndex) const
//----------------------------------------------------
{
	if (getVertexNumber() <= anIndex)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}
	
	return (VEC3(m_p_vertex_set[anIndex * 3 + 0],
			m_p_vertex_set[anIndex * 3 + 1],
			m_p_vertex_set[anIndex * 3 + 2]));
}


//--------------------------------------------------------
inline VEC3 TriangleMesh::getFaceNormal(int anIndex) const
//--------------------------------------------------------
{
	if (getFaceNormalNumber() <= anIndex)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}
	
	return (VEC3(m_p_face_normal_set[anIndex * 3 + 0],
			m_p_face_normal_set[anIndex * 3 + 1],
			m_p_face_normal_set[anIndex * 3 + 2]));
}


//----------------------------------------------------------
inline VEC3 TriangleMesh::getVertexNormal(int anIndex) const
//----------------------------------------------------------
{
	if (getVertexNormalNumber() <= anIndex)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}
	
	return (VEC3(m_p_vertex_normal_set[anIndex * 3 + 0],
			m_p_vertex_normal_set[anIndex * 3 + 1],
			m_p_vertex_normal_set[anIndex * 3 + 2]));
}


} // namespace Graphics
