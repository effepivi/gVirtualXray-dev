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
#include <sstream>

#ifndef OUT_OF_BOUNDS_EXCEPTION_H
#include "gVirtualXRay/OutOfBoundsException.h"
#endif


//******************************************************************************
//	define
//******************************************************************************


//******************************************************************************
//	namespace
//******************************************************************************
namespace Graphics {


//----------------------------------
inline TriangleMesh::~TriangleMesh()
//----------------------------------
{
	// Make sure the VBO and the dynamically allocated memory are released
	reset();
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


//--------------------------------------------------------------------
inline bool TriangleMesh::useVBO(int aTypeOfVBO, int aTypeOfPrimitive)
//--------------------------------------------------------------------
{
	// There is no data to store in a VBO
	if (m_number_of_vertices && (getVertexNormalNumber() || getFaceNormalNumber()))
	{
		// Create a VBO if possible
		updateVBO(aTypeOfVBO, aTypeOfPrimitive);
	}
	return (m_vbo.isReady());
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
	if (m_number_of_indices)
	{
		return (m_number_of_indices / 3);
	}
	else
	{
		// There are 3 vertices per triangle
		return (m_number_of_vertices / 3);
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
	return (m_number_of_vertices);
}


//------------------------------------------------------
inline unsigned int TriangleMesh::getIndexNumber() const
//------------------------------------------------------
{
	return (m_number_of_indices);
}


//-----------------------------------------------------------------
inline VEC3 TriangleMesh::getFaceNormal(unsigned int anIndex) const
//-----------------------------------------------------------------
{
	if (getFaceNormalNumber() <= anIndex)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}
	
	return (VEC3(m_p_face_normal_set[anIndex * 3 + 0],
			m_p_face_normal_set[anIndex * 3 + 1],
			m_p_face_normal_set[anIndex * 3 + 2]));
}


//-------------------------------------------------------------------
inline VEC3 TriangleMesh::getVertexNormal(unsigned int anIndex) const
//-------------------------------------------------------------------
{
	if (getVertexNormalNumber() <= anIndex)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}
	
	return (VEC3(m_p_vertex_normal_set[anIndex * 3 + 0],
			m_p_vertex_normal_set[anIndex * 3 + 1],
			m_p_vertex_normal_set[anIndex * 3 + 2]));
}


//--------------------------------------------
inline void TriangleMesh::computeBoundingBox()
//--------------------------------------------
{
	// There are vertices
	if (m_number_of_vertices)
	{
		m_min_corner = getVertex(0);
		m_max_corner = getVertex(0);

		for (unsigned int i(1); i < m_number_of_vertices; ++i)
		{
			VEC3 vertex(getVertex(i));

			if (m_min_corner.getX() > vertex.getX()) m_min_corner.setX(vertex.getX());
			if (m_max_corner.getX() < vertex.getX()) m_max_corner.setX(vertex.getX());
			if (m_min_corner.getY() > vertex.getY()) m_min_corner.setY(vertex.getY());
			if (m_max_corner.getY() < vertex.getY()) m_max_corner.setY(vertex.getY());
			if (m_min_corner.getZ() > vertex.getZ()) m_min_corner.setZ(vertex.getZ());
			if (m_max_corner.getZ() < vertex.getZ()) m_max_corner.setZ(vertex.getZ());
		}

		// Compute diagonal size
		m_bounding_box_diagonal = (m_min_corner - m_max_corner).length();
	}
}


//-----------------------------------------------------------------
inline void TriangleMesh::applyScale(const RATIONAL_NUMBER& aScale)
//-----------------------------------------------------------------
{
	// There are vertices
	if (m_number_of_vertices)
	{
		for (unsigned int i(0); i < m_number_of_vertices; ++i)
		{
			VEC3 vertex(getVertex(i) * aScale);
			setVertex(i, vertex);
		}

		// Update the bounding box
		m_min_corner *= aScale;
		m_max_corner *= aScale;
		m_bounding_box_diagonal *= aScale;

		// Update the VBO
		if (m_vbo.isReady())
		{
			updateVBO(m_vbo.getBufferUsageHint(), m_vbo.getPrimitiveType());
		}
	}
}


//-----------------------------------------------------------------
inline void TriangleMesh::translate(const VEC3& aTranslationVector)
//-----------------------------------------------------------------
{
	if (m_number_of_vertices)
	{
		for (unsigned int i(0); i < m_number_of_vertices; i++)
		{
			VEC3 vertex(getVertex(i) + aTranslationVector);
			setVertex(i, vertex);
		}
	
		m_min_corner += aTranslationVector;
		m_max_corner += aTranslationVector;
	
		// Update the VBO
		if (m_vbo.isReady())
		{
			updateVBO(m_vbo.getBufferUsageHint(), m_vbo.getPrimitiveType());
		}
	}
}


//--------------------------------------
inline void TriangleMesh::moveToCentre()
//--------------------------------------
{
	VEC3 translation_vector(m_min_corner + (m_max_corner - m_min_corner) / 2.0);
	translation_vector *= -1.0;

	translate(translation_vector);
}


//--------------------------------------------
inline void TriangleMesh::inverNormalVectors()
//--------------------------------------------
{
	for (std::vector<RATIONAL_NUMBER>::iterator ite(m_p_vertex_normal_set.begin());
			ite != m_p_vertex_normal_set.end();
			++ite)
	{
		*ite = -(*ite);
	}


	for (std::vector<RATIONAL_NUMBER>::iterator ite(m_p_face_normal_set.begin());
			ite != m_p_face_normal_set.end();
			++ite)
	{
		*ite = -(*ite);
	}
}


} // namespace Graphics
