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
#include <iostream>

#ifndef OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef VBO_H
#include "gVirtualXRay/VBO.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace Graphics;


//-----------------------------------------------------------------------------------------------------------------------------------------
void VBO::setVertexData(GLenum aPrimitiveType,
						const GLuint aNumberOfVertices, GLuint aNumberOfCompontentsPerVertex, const GLfloat* vertices,
						const GLuint aNumberOfNormals, GLuint aNumberOfCompontentsPerNormal,  const GLfloat* apNormals,
						const GLuint aNumberOfTextureCoordinates, GLuint aNumberOfCompontentsPerCoord, const GLfloat* apTextureCoordinates)
//-----------------------------------------------------------------------------------------------------------------------------------------
{
	// Reset the internal data
	reset();

	// Initialise class members
	m_primitive_type = aPrimitiveType;
	m_number_of_normals  = aNumberOfNormals;
	m_number_of_vertices = aNumberOfVertices;
	m_number_of_texture_coordinates = aNumberOfTextureCoordinates;

	m_normal_size             = m_number_of_normals             * aNumberOfCompontentsPerNormal * sizeof(GLfloat);
	m_texture_coordinate_size = m_number_of_texture_coordinates * aNumberOfCompontentsPerCoord  * sizeof(GLfloat);
	m_vertex_size             = m_number_of_vertices            * aNumberOfCompontentsPerVertex * sizeof(GLfloat);

	// Load the data in the VBO
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
	}

	if (m_id_set[VERTEX_SET_ID] != 0 && m_number_of_vertices == 0)
	{
		glDeleteBuffers(1, &m_id_set[VERTEX_SET_ID]);
		m_id_set[VERTEX_SET_ID] = 0;
	}

	if (m_id_set[TEXT_COORD_SET_ID] != 0 && m_texture_coordinate_size == 0)
	{
		glDeleteBuffers(1, &m_id_set[TEXT_COORD_SET_ID]);
		m_id_set[TEXT_COORD_SET_ID] = 0;
	}

	if (m_id_set[NORMAL_SET_ID] != 0 && m_normal_size == 0)
	{
		glDeleteBuffers(1, &m_id_set[NORMAL_SET_ID]);
		m_id_set[NORMAL_SET_ID] = 0;
	}

	if (m_id_set[INDEX_SET_ID] != 0 && m_number_of_indices == 0)
	{
		glDeleteBuffers(1, &m_id_set[INDEX_SET_ID]);
		m_id_set[INDEX_SET_ID] = 0;
	}

	// Unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//-----------------------------------------------------------------------------
void VBO::setIndexData(const GLuint aNumberOfIndexes, const GLuint* apIndexSet)
//-----------------------------------------------------------------------------
{
	m_number_of_indices = aNumberOfIndexes;

	// Create the VBO
	if (!m_id_set[INDEX_SET_ID] && m_number_of_indices && apIndexSet)
	{
		glGenBuffers(1, &m_id_set[INDEX_SET_ID]);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	if (m_number_of_indices && apIndexSet)
	{
		// Bind the new VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id_set[INDEX_SET_ID]);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Load the data in the VBO
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_number_of_indices * sizeof(GLuint), apIndexSet, m_storage_type);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Unbind the VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
	else
	{
		if (m_id_set[INDEX_SET_ID])
		{
			glDeleteBuffers(1, &m_id_set[INDEX_SET_ID]);
			m_id_set[INDEX_SET_ID] = 0;
		}
	}
}


//----------------------------------------------------------------------------
void VBO::setIndexData(const GLuint aNumberOfIndexes, const GLint* apIndexSet)
//----------------------------------------------------------------------------
{
	m_number_of_indices = aNumberOfIndexes;

	// Create the VBO
	glGenBuffers(1, &m_id_set[INDEX_SET_ID]);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Bind the new VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id_set[INDEX_SET_ID]);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Load the data in the VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_number_of_indices * sizeof(GLint), apIndexSet, m_storage_type);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Unbind the VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}
