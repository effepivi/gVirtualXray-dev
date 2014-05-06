/**
********************************************************************************
*
*	@file		VBO.cxx
*
*	@brief		Class to handle vertex buffer objects (VBOs).
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
#define GL_GLEXT_PROTOTYPES 1
#include <GL/glcorearb.h>

#include <iostream>

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef GVXR_VBO_H
#include "gVirtualXRay/VBO.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//----------------------------------------------
VBO::VBO():
//----------------------------------------------
		m_vao_id(0),
		m_number_of_normals(0),
		m_number_of_texture_coordinates(0),
		m_number_of_vertices(0),
		m_number_of_indices(0),
		m_normal_size(0),
		m_texture_coordinate_size(0),
		m_vertex_size(0),
		m_index_size(0),
		m_normal_data_type(GL_NONE),
		m_texture_coordinate_data_type(GL_NONE),
		m_vertex_data_type(GL_NONE),
		m_index_data_type(GL_NONE),
		m_primitive_type(GL_NONE),
		m_storage_type(GL_DYNAMIC_DRAW)
//----------------------------------------------
{
	m_id_set[VERTEX_SET_ID]     = 0;
	m_id_set[TEXT_COORD_SET_ID] = 0;
	m_id_set[NORMAL_SET_ID]     = 0;
	m_id_set[INDEX_SET_ID]      = 0;
}


//-----------------
void VBO::destroy()
//-----------------
{
	if (m_id_set[VERTEX_SET_ID])     glDeleteBuffers(1, &m_id_set[VERTEX_SET_ID]);
	if (m_id_set[TEXT_COORD_SET_ID]) glDeleteBuffers(1, &m_id_set[TEXT_COORD_SET_ID]);
	if (m_id_set[NORMAL_SET_ID])     glDeleteBuffers(1, &m_id_set[NORMAL_SET_ID]);
	if (m_id_set[INDEX_SET_ID])      glDeleteBuffers(1, &m_id_set[INDEX_SET_ID]);
	if (m_vao_id)                    glDeleteVertexArrays(1, &m_vao_id);

	m_id_set[VERTEX_SET_ID]     = 0;
	m_id_set[TEXT_COORD_SET_ID] = 0;
	m_id_set[NORMAL_SET_ID]     = 0;
	m_id_set[INDEX_SET_ID]      = 0;
	m_vao_id = 0;

	reset();
}


//---------------
void VBO::reset()
//---------------
{
	m_number_of_normals             = 0;
	m_number_of_texture_coordinates = 0;
	m_number_of_vertices            = 0;
	m_number_of_indices             = 0;

	m_normal_size             = 0;
	m_texture_coordinate_size = 0;
	m_vertex_size             = 0;
	m_index_size              = 0;

	m_normal_data_type             = GL_NONE;
	m_texture_coordinate_data_type = GL_NONE;
	m_vertex_data_type             = GL_NONE;
	m_index_data_type              = GL_NONE;

	m_primitive_type = GL_NONE;

	m_storage_type = GL_DYNAMIC_DRAW;

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


//-----------------------
void VBO::display() const
//-----------------------
{
	if (m_primitive_type != GL_NONE)
	{
		// Bind the VAO
		glBindVertexArray(m_vao_id);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Draw the polygon mesh
		if (!m_number_of_indices)
		{
			glDrawArrays(m_primitive_type, 0, m_number_of_vertices);
		}
		else
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id_set[INDEX_SET_ID]);
			glDrawElements(m_primitive_type, m_number_of_indices, m_index_data_type, 0);
		}
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Unbind the VAO
		glBindVertexArray(0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
	else
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid primitive type.");
	}
}


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void VBO::setVertexData(int aPrimitiveType,
						const unsigned int aNumberOfVertices,           unsigned int aNumberOfCompontentsPerVertex, int aVertexDataType, const void* vertices,
						const unsigned int aNumberOfNormals,            unsigned int aNumberOfCompontentsPerNormal, int aNormalDataType, const void* apNormals,
						const unsigned int aNumberOfTextureCoordinates, unsigned int aNumberOfCompontentsPerCoord,  int aCoordDataType,  const void* apTextureCoordinates)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
{
	// Reset the internal data
	reset();

	// Initialise class members
	m_primitive_type = aPrimitiveType;
	m_number_of_normals  = aNumberOfNormals;
	m_number_of_vertices = aNumberOfVertices;
	m_number_of_texture_coordinates = aNumberOfTextureCoordinates;

	m_normal_size             = m_number_of_normals             * aNumberOfCompontentsPerNormal;
	m_texture_coordinate_size = m_number_of_texture_coordinates * aNumberOfCompontentsPerCoord;
	m_vertex_size             = m_number_of_vertices            * aNumberOfCompontentsPerVertex;

	m_normal_data_type = aNormalDataType;
	m_texture_coordinate_data_type = aCoordDataType;
	m_vertex_data_type = aVertexDataType;

	switch (m_normal_data_type)
	{
	case GL_FLOAT:
		m_normal_size *= sizeof(GLfloat);
		break;

	case GL_DOUBLE:
		m_normal_size *= sizeof(GLdouble);
		break;

	case GL_NONE:
		m_normal_size = 0;
		break;

	default:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid data type.");
	}

	switch (m_texture_coordinate_data_type)
	{
	case GL_FLOAT:
		m_texture_coordinate_size *= sizeof(GLfloat);
		break;

	case GL_DOUBLE:
		m_texture_coordinate_size *= sizeof(GLdouble);
		break;

	case GL_NONE:
		m_texture_coordinate_size = 0;
		break;

	default:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid data type.");
	}

	switch (m_vertex_data_type)
	{
	case GL_FLOAT:
		m_vertex_size *= sizeof(GLfloat);
		break;

	case GL_DOUBLE:
		m_vertex_size *= sizeof(GLdouble);
		break;

	default:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid data type.");
	}

	// Load the data in the VBO
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// There is no VAO
	if (!m_vao_id)
	{
		// Create a VAO
		glGenVertexArrays(1, &m_vao_id);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	// Bind the VAO
	glBindVertexArray(m_vao_id);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// There is normal data
	if (apNormals)
	{
		// Create the VBO
		if (!m_id_set[NORMAL_SET_ID])
		{
			glGenBuffers(1, &m_id_set[NORMAL_SET_ID]);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		}

		// Bind the new VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_id_set[NORMAL_SET_ID]);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		glBufferData(GL_ARRAY_BUFFER, m_normal_size, apNormals, m_storage_type);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		glEnableVertexAttribArray(2);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		glVertexAttribPointer(2, 3, m_normal_data_type, GL_FALSE, 0, 0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
	else
	{
		glDisableVertexAttribArray(2);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	// There are texture coordinates
	if (apTextureCoordinates)
	{
		// Create the VBO
		if (!m_id_set[TEXT_COORD_SET_ID])
		{
			glGenBuffers(1, &m_id_set[TEXT_COORD_SET_ID]);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		}

		// Bind the new VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_id_set[TEXT_COORD_SET_ID]);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		glBufferData(GL_ARRAY_BUFFER, m_texture_coordinate_size, apTextureCoordinates, m_storage_type);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		glEnableVertexAttribArray(1);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		glVertexAttribPointer(1, 3, m_texture_coordinate_data_type, GL_FALSE, 0, 0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
	else
	{
		glDisableVertexAttribArray(1);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	// There are vertex coordinates
	if (vertices)
	{
		// Create the VBO
		if (!m_id_set[VERTEX_SET_ID])
		{
			glGenBuffers(1, &m_id_set[VERTEX_SET_ID]);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		}

		// Bind the new VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_id_set[VERTEX_SET_ID]);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		glBufferData(GL_ARRAY_BUFFER, m_vertex_size, vertices, m_storage_type);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		glEnableVertexAttribArray(0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		glVertexAttribPointer(0, 3, m_vertex_data_type, GL_FALSE, 0, 0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
	else
	{
		glDisableVertexAttribArray(0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	if (m_id_set[VERTEX_SET_ID] != 0 && m_number_of_vertices == 0)
	{
		glDeleteBuffers(1, &m_id_set[VERTEX_SET_ID]);
		m_id_set[VERTEX_SET_ID] = 0;

		glDisableVertexAttribArray(0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	if (m_id_set[TEXT_COORD_SET_ID] != 0 && m_texture_coordinate_size == 0)
	{
		glDeleteBuffers(1, &m_id_set[TEXT_COORD_SET_ID]);
		m_id_set[TEXT_COORD_SET_ID] = 0;

		glDisableVertexAttribArray(1);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	if (m_id_set[NORMAL_SET_ID] != 0 && m_normal_size == 0)
	{
		glDeleteBuffers(1, &m_id_set[NORMAL_SET_ID]);
		m_id_set[NORMAL_SET_ID] = 0;

		glDisableVertexAttribArray(2);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	if (m_id_set[INDEX_SET_ID] != 0 && m_number_of_indices == 0)
	{
		glDeleteBuffers(1, &m_id_set[INDEX_SET_ID]);
		m_id_set[INDEX_SET_ID] = 0;
	}

	// Unbind the VAO
	glBindVertexArray(0);

	// Unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//---------------------------------------------------------
void VBO::setIndexData(const unsigned int aNumberOfIndexes,
					   int anIndexDataType,
					   const void* apIndexSet)
//---------------------------------------------------------
{
	m_number_of_indices = aNumberOfIndexes;

	// Bind the VAO
	glBindVertexArray(m_vao_id);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Create the VBO
	if (!m_id_set[INDEX_SET_ID] && m_number_of_indices && apIndexSet)
	{
		glGenBuffers(1, &m_id_set[INDEX_SET_ID]);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	m_index_size = aNumberOfIndexes;
	m_index_data_type = anIndexDataType;
	switch (m_index_data_type)
	{
	case GL_UNSIGNED_INT:
		m_index_size *= sizeof(GLuint);
		break;

	case GL_UNSIGNED_SHORT:
		m_index_size *= sizeof(GLushort);
		break;

	case GL_UNSIGNED_BYTE:
		m_index_size *= sizeof(GLubyte);
		break;

	default:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid data type.");
	}

	// Load data
	if (m_number_of_indices && apIndexSet && m_index_size)
	{
		// Bind the new VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id_set[INDEX_SET_ID]);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Load the data in the VBO
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_index_size, apIndexSet, m_storage_type);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Unbind the VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
	// There is no data to load
	else
	{
		// There is no need for a buffer
		if (m_id_set[INDEX_SET_ID])
		{
			glDeleteBuffers(1, &m_id_set[INDEX_SET_ID]);
			m_id_set[INDEX_SET_ID] = 0;
		}
	}

	// Unbind the VAO
	glBindVertexArray(0);
}
