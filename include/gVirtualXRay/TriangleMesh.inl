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
inline TriangleMesh::TriangleMesh():
//----------------------------------
	m_use_hounsfield_value(true),
	m_hounsfield_value(0),
	m_attenuation_coefficient(0),
	m_bounding_box_diagonal(0),
	m_p_external_index_set(0),
	m_p_external_vertex_set(0)
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
	m_p_internal_vertex_set.clear();
	m_p_internal_index_set.clear();

	m_p_external_index_set = 0;
	m_p_external_vertex_set = 0;
}


//----------------------------------------------------
inline void TriangleMesh::display(bool aVBOFlag) const
//----------------------------------------------------
{
	// Display the polygon mesh using a VBO
	if (m_vbo.isReady() && aVBOFlag)
	{
		m_vbo.display();
	}
	// Do not use the VBO
	else
	{
		// There is an index
		if (getIndexNumber())
		{
			displayUsingImmediateModeWithIndex();
		}
		// There is not any index
		else
		{
			displayUsingImmediateModeWithoutIndex();
		}
	}

	// Check the OpenGL error status
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//------------------------------------------------
inline void TriangleMesh::displayWireFrame() const
//------------------------------------------------
{
	// There is an index
	if (getIndexNumber())
	{
		displayWireFrameWithIndex();
	}
	// There is not any index
	else
	{
		displayWireFrameWithoutIndex();
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
		std::stringstream error_message;
		error_message << "There are not as many normals (" << getVertexNormalNumber() << ") as there are vertices (" << getVertexNumber() << ").";
		throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
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
	int number_of_indices(getIndexNumber());
	int number_of_vertices(getVertexNumber());

	// Return the number of triangles
	if (number_of_indices)
	{
		return (number_of_indices / 3);
	}
	else
	{
		// There are 3 vertices per triangle
		return (number_of_vertices / 3);
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
	// By default try the external data
	if (m_p_external_vertex_set)
	{
		// Return the number of vertices
		// There are 3 GLfloat values per vertex
		return (m_p_external_vertex_set->size() / 3);
	}
	// Use the internal data
	else
	{
		// Return the number of vertices
		// There are 3 GLfloat values per vertex
		return (m_p_internal_vertex_set.size() / 3);
	}
}


//----------------------------------------------------
inline VEC3 TriangleMesh::getVertex(int anIndex) const
//----------------------------------------------------
{
	if (getVertexNumber() <= anIndex)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}
	
	double x, y, z;

	// By default try the external data
	if (m_p_external_vertex_set)
	{
		x = (*m_p_external_vertex_set)[anIndex * 3 + 0];
		y = (*m_p_external_vertex_set)[anIndex * 3 + 1];
		z = (*m_p_external_vertex_set)[anIndex * 3 + 2];
	}
	// Use the internal data
	else
	{
		x = m_p_internal_vertex_set[anIndex * 3 + 0];
		y = m_p_internal_vertex_set[anIndex * 3 + 1];
		z = m_p_internal_vertex_set[anIndex * 3 + 2];
	}
	return (VEC3(x, y, z));
}


//------------------------------------------------------
inline void TriangleMesh::setVertex(int anIndex,
									const VEC3& aVertex)
//------------------------------------------------------
{
	if (getVertexNumber() <= anIndex)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	// By default try the external data
	if (m_p_external_vertex_set)
	{
		(*m_p_external_vertex_set)[anIndex * 3 + 0] = aVertex.getX();
		(*m_p_external_vertex_set)[anIndex * 3 + 1] = aVertex.getY();
		(*m_p_external_vertex_set)[anIndex * 3 + 2] = aVertex.getZ();
	}
	// Use the internal data
	else
	{
		m_p_internal_vertex_set[anIndex * 3 + 0] = aVertex.getX();
		m_p_internal_vertex_set[anIndex * 3 + 1] = aVertex.getY();
		m_p_internal_vertex_set[anIndex * 3 + 2] = aVertex.getZ();
	}
}


//------------------------------------------------------
inline unsigned int TriangleMesh::getIndexNumber() const
//------------------------------------------------------
{
	// By default try the external data
	if (m_p_external_index_set)
	{
		// Return the number of indices
		return (m_p_external_index_set->size());
	}
	// Use the internal data
	else
	{
		// Return the number of indices
		return (m_p_internal_index_set.size());
	}
}


//--------------------------------------------------
inline int TriangleMesh::getIndex(int anIndex) const
//--------------------------------------------------
{
	if (getIndexNumber() <= anIndex)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	// By default try the external data
	if (m_p_external_index_set)
	{
		return ((*m_p_external_index_set)[anIndex]);
	}
	// Use the internal data
	else
	{
		return (m_p_internal_index_set[anIndex]);
	}
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


//---------------------------------------------------------------------------------
inline void TriangleMesh::setExternalData(const std::vector<GLfloat>* aVertexArray,
								   	   	  const std::vector<int>* anIndexArray)
//---------------------------------------------------------------------------------
{
	reset();

	m_p_external_vertex_set = const_cast<std::vector<GLfloat>*>(aVertexArray);
	m_p_external_index_set = const_cast<std::vector<int>*>(anIndexArray);

	// Compute the bounding box
	computeBoundingBox();

	// Compute the normal vectors
	computeNormalVectors();
	
	// Update the VBO
	if (m_vbo.isReady())
	{
		updateVBO();
	}
}


//--------------------------------------------
inline void TriangleMesh::computeBoundingBox()
//--------------------------------------------
{
	// There are vertices
	unsigned int number_of_vertices(getVertexNumber());
	if (number_of_vertices)
	{
		m_min_corner = getVertex(0);
		m_max_corner = getVertex(0);

		for (int i(1); i < number_of_vertices; ++i)
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


//----------------------------------------------------------
inline void TriangleMesh::applyScale(RATIONAL_NUMBER aScale)
//----------------------------------------------------------
{
	// There are vertices
	unsigned int number_of_vertices(getVertexNumber());
	if (number_of_vertices)
	{
		for (int i(0); i < number_of_vertices; ++i)
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
			updateVBO();
		}
	}
}


//-----------------------------------------------------------------
inline void TriangleMesh::translate(const VEC3& aTranslationVector)
//-----------------------------------------------------------------
{
	for (int i(0); i < getVertexNumber(); i++)
	{
		VEC3 vertex(getVertex(i) + aTranslationVector);
		setVertex(i, vertex);
	}

	m_min_corner += aTranslationVector;
	m_max_corner += aTranslationVector;

	// Update the VBO
	if (m_vbo.isReady())
	{
		updateVBO();
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


} // namespace Graphics
