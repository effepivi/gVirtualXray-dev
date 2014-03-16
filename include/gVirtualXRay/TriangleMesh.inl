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


//---------------------------------------
inline TriangleMesh::TriangleMesh():
//---------------------------------------
	m_use_hounsfield_value(true),
	m_hounsfield_value(0),
	m_attenuation_coefficient(0),
	m_bounding_box_diagonal(0),
	m_p_vertex_set(0),
	m_p_index_set(0),
	m_number_of_indices(0),
	m_number_of_vertices(0),
	m_index_data_type(GL_NONE),
	m_vertex_data_type(GL_NONE),
	m_external_data_flag(false)
//---------------------------------------
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
	m_vbo.reset();
	
	m_use_hounsfield_value = true;
	m_hounsfield_value = 0;
	m_attenuation_coefficient = 0;

	m_bounding_box_diagonal = 0;

	m_min_corner = VEC3();
	m_max_corner = VEC3();
	
	m_p_face_normal_set.clear();
	m_p_vertex_normal_set.clear();
	
	// The data is manage externally
	if (m_external_data_flag)
	{
		m_p_index_set  = 0;
		m_p_vertex_set = 0;
	}
	// The data is manage internally
	else
	{
		if (m_p_index_set)
		{
			delete [] m_p_index_set;
			m_p_index_set = 0;
		}

		if (m_p_vertex_set)
		{
			delete [] m_p_vertex_set;
			m_p_vertex_set = 0;
		}
	}

	m_number_of_indices  = 0;
	m_number_of_vertices = 0;

	m_index_data_type = GL_NONE;
	m_vertex_data_type = GL_NONE;
	
	m_external_data_flag = false;
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
		if (m_number_of_indices)
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
	if (m_number_of_indices)
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
	if (getVertexNormalNumber() == m_number_of_vertices)
	{
		glBegin(GL_LINES);
		for (unsigned int i(0); i < m_number_of_vertices; ++i)
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
		error_message << "There are not as many normals (" << getVertexNormalNumber() << ") as there are vertices (" << m_number_of_vertices << ").";
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


//-------------------------------------------------
inline bool TriangleMesh::useVBO(GLenum aTypeOfVBO)
//-------------------------------------------------
{
	// There is no data to store in a VBO
	if (m_number_of_vertices && (getVertexNormalNumber() || getFaceNormalNumber()))
	{
		// Create a VBO if possible
		updateVBO(aTypeOfVBO);
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


//-------------------------------------------------------------
inline VEC3 TriangleMesh::getVertex(unsigned int anIndex) const
//-------------------------------------------------------------
{
	if (m_number_of_vertices <= anIndex && m_p_vertex_set)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}
	
	double x, y, z;

	switch (m_vertex_data_type)
	{
	case GL_FLOAT:
		{
			GLfloat* p_temp(reinterpret_cast<GLfloat*>(m_p_vertex_set));
			x = p_temp[anIndex * 3 + 0];
			y = p_temp[anIndex * 3 + 1];
			z = p_temp[anIndex * 3 + 2];
		}
		break;
			
	case GL_DOUBLE:
		{
			GLdouble* p_temp(reinterpret_cast<GLdouble*>(m_p_vertex_set));
			x = p_temp[anIndex * 3 + 0];
			y = p_temp[anIndex * 3 + 1];
			z = p_temp[anIndex * 3 + 2];
		}
		break;
		
	default:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid data type.");
	}

	return (VEC3(x, y, z));
}


//-------------------------------------------------------
inline void TriangleMesh::setVertex(unsigned int anIndex,
									const VEC3& aVertex)
//-------------------------------------------------------
{
	if (m_number_of_vertices <= anIndex && m_p_vertex_set)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	switch (m_vertex_data_type)
	{
	case GL_FLOAT:
		{
			GLfloat* p_temp(reinterpret_cast<GLfloat*>(m_p_vertex_set));
			p_temp[anIndex * 3 + 0] = aVertex.getX();
			p_temp[anIndex * 3 + 1] = aVertex.getY();
			p_temp[anIndex * 3 + 2] = aVertex.getZ();
		}
		break;
			
	case GL_DOUBLE:
		{
			GLdouble* p_temp(reinterpret_cast<GLdouble*>(m_p_vertex_set));
			p_temp[anIndex * 3 + 0] = aVertex.getX();
			p_temp[anIndex * 3 + 1] = aVertex.getY();
			p_temp[anIndex * 3 + 2] = aVertex.getZ();
		}
		break;
		
	default:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid data type.");
	}
}


//------------------------------------------------------
inline unsigned int TriangleMesh::getIndexNumber() const
//------------------------------------------------------
{
	return (m_number_of_indices);
}


//-----------------------------------------------------------
inline int TriangleMesh::getIndex(unsigned int anIndex) const
//-----------------------------------------------------------
{
	if (m_number_of_indices <= anIndex && m_p_index_set)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	switch (m_index_data_type)
	{
	case GL_UNSIGNED_INT:
		{
			GLuint* p_temp(reinterpret_cast<GLuint*>(m_p_index_set));
			return (int(p_temp[anIndex]));
		}
		break;
		
	case GL_UNSIGNED_SHORT:
		{
			GLushort* p_temp(reinterpret_cast<GLushort*>(m_p_index_set));
			return (int(p_temp[anIndex]));
		}
		break;
		
	case GL_UNSIGNED_BYTE:
		{
			GLubyte* p_temp(reinterpret_cast<GLubyte*>(m_p_index_set));
			return (int(p_temp[anIndex]));
		}
		break;
		
	default:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid data type.");
	}
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


//---------------------------------------------------------------------------------
inline void TriangleMesh::setExternalData(const std::vector<GLfloat>* aVertexArray,
										  bool aCreateVBOFlag,
										  GLenum aTypeOfVBO)
//---------------------------------------------------------------------------------
{
	setExternalData(&((*aVertexArray)[0]),
			aVertexArray->size() / 3,
			GL_FLOAT,
			0,
			0,
			GL_NONE,
			aCreateVBOFlag,
			aTypeOfVBO);
}


//----------------------------------------------------------------------------------
inline void TriangleMesh::setExternalData(const std::vector<GLdouble>* aVertexArray,
										  bool aCreateVBOFlag,
										  GLenum aTypeOfVBO)
//----------------------------------------------------------------------------------
{
	setExternalData(&((*aVertexArray)[0]),
			aVertexArray->size() / 3,
			GL_DOUBLE,
			0,
			0,
			GL_NONE,
			aCreateVBOFlag,
			aTypeOfVBO);
}


//---------------------------------------------------------------------------------
inline void TriangleMesh::setExternalData(const std::vector<GLfloat>* aVertexArray,
										  const std::vector<GLuint>* anIndexArray,
										  bool aCreateVBOFlag,
										  GLenum aTypeOfVBO)
//---------------------------------------------------------------------------------
{
	setExternalData(&((*aVertexArray)[0]),
			aVertexArray->size() / 3,
			GL_FLOAT,
			&((*anIndexArray)[0]),
			anIndexArray->size(),
			GL_UNSIGNED_INT,
			aCreateVBOFlag,
			aTypeOfVBO);
}


//----------------------------------------------------------------------------------
inline void TriangleMesh::setExternalData(const std::vector<GLdouble>* aVertexArray,
										  const std::vector<GLuint>* anIndexArray,
										  bool aCreateVBOFlag,
										  GLenum aTypeOfVBO)
//----------------------------------------------------------------------------------
{
	setExternalData(&((*aVertexArray)[0]),
			aVertexArray->size() / 3,
			GL_DOUBLE,
			&((*anIndexArray)[0]),
			anIndexArray->size(),
			GL_UNSIGNED_INT,
			aCreateVBOFlag,
			aTypeOfVBO);
}


//-------------------------------------------------------------------
inline void TriangleMesh::setExternalData(const GLvoid* aVertexArray,
										  GLuint aNumberOfVertices,
										  GLenum aVertexDataType,
										  const GLvoid* anIndexArray,
										  GLuint aNumberOfIndices,
										  GLenum anIndexDataType,
										  bool aCreateVBOFlag,
										  GLenum aTypeOfVBO)
//-------------------------------------------------------------------
{
	reset();

	m_external_data_flag = true;
	
	m_p_vertex_set       = const_cast<GLvoid*>(aVertexArray);
	m_number_of_vertices        = aNumberOfVertices;
	m_vertex_data_type = aVertexDataType;

	// There are index data
	if (anIndexArray && anIndexDataType != GL_NONE && aNumberOfIndices != 0)
	{
		m_p_index_set       = const_cast<GLvoid*>(anIndexArray);
		m_number_of_indices = aNumberOfIndices;
		m_index_data_type   = anIndexDataType;
	}
	else
	{
		m_p_index_set       = 0;
		m_number_of_indices = 0;
		m_index_data_type   = GL_NONE;

	}
	
	// Compute the bounding box
	computeBoundingBox();

	// Compute the normal vectors
	computeNormalVectors();
	
	// Update the VBO
	if (aCreateVBOFlag)
	{
		updateVBO(aTypeOfVBO);
	}
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


//----------------------------------------------------------
inline void TriangleMesh::applyScale(RATIONAL_NUMBER aScale)
//----------------------------------------------------------
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
			updateVBO(m_vbo.getBufferUsageHint());
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
			updateVBO(m_vbo.getBufferUsageHint());
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
	for (std::vector<GLfloat>::iterator ite(m_p_vertex_normal_set.begin());
			ite != m_p_vertex_normal_set.end();
			++ite)
	{
		*ite = -(*ite);
	}
}


} // namespace Graphics
