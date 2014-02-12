/**
********************************************************************************
*
*	@file		VBO.inl
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
#include <stdio.h>
#include <string>
#include <string.h>

#ifndef OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif


//******************************************************************************
//	define
//******************************************************************************
#define VBO_BUFFER_OFFSET(a) ((char*)NULL + (a))
#define VERTEX_SET_ID     0
#define TEXT_COORD_SET_ID 1
#define NORMAL_SET_ID     2
#define INDEX_SET_ID      3


//******************************************************************************
//	namespace
//******************************************************************************
namespace Graphics {
using namespace Utilities;


//-------------------------------------
inline VBO::VBO():
//-------------------------------------
	m_number_of_normals(0),
	m_number_of_texture_coordinates(0),
	m_number_of_vertices(0),
	m_number_of_indices(0),
	m_normal_size(0),
	m_texture_coordinate_size(0),
	m_vertex_size(0),
	m_primitive_type(GL_TRIANGLES)
//-------------------------------------
{
	m_id_set[VERTEX_SET_ID]     = 0;
	m_id_set[TEXT_COORD_SET_ID] = 0;
	m_id_set[NORMAL_SET_ID]     = 0;
	m_id_set[INDEX_SET_ID]      = 0;
}


//----------------
inline VBO::~VBO()
//----------------
{
	reset();
}


//----------------------
inline void VBO::reset()
//----------------------
{
	if (m_id_set[VERTEX_SET_ID])     glDeleteBuffers(1, &m_id_set[VERTEX_SET_ID]);
	if (m_id_set[TEXT_COORD_SET_ID]) glDeleteBuffers(1, &m_id_set[TEXT_COORD_SET_ID]);
	if (m_id_set[NORMAL_SET_ID])     glDeleteBuffers(1, &m_id_set[NORMAL_SET_ID]);
	if (m_id_set[INDEX_SET_ID])      glDeleteBuffers(1, &m_id_set[INDEX_SET_ID]);

	m_id_set[VERTEX_SET_ID]     = 0;
	m_id_set[TEXT_COORD_SET_ID] = 0;
	m_id_set[NORMAL_SET_ID]     = 0;
	m_id_set[INDEX_SET_ID]      = 0;

	m_number_of_normals             = 0;
	m_number_of_texture_coordinates = 0;
	m_number_of_vertices            = 0;
	m_number_of_indices             = 0;

	m_normal_size             = 0;
	m_texture_coordinate_size = 0;
	m_vertex_size             = 0;
		
	m_primitive_type = GL_TRIANGLES;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


//------------------------------
inline void VBO::display() const
//------------------------------
{
	// There is vertices
	if (m_number_of_vertices)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id_set[VERTEX_SET_ID]);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
	
	// There is texture coordinates
	if (m_number_of_texture_coordinates)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id_set[TEXT_COORD_SET_ID]);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(3, GL_FLOAT, 0, 0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	// There is normal data
	if (m_number_of_normals)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id_set[NORMAL_SET_ID]);
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, 0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}		
	
	// Draw the polygon mesh
	if (!m_number_of_indices)
	{
		glDrawArrays(m_primitive_type, 0, m_number_of_vertices);
	}
	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id_set[INDEX_SET_ID]);
		glDrawElements(m_primitive_type, m_number_of_indices, GL_UNSIGNED_INT, 0); 
	}
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	
	// There are vertices
	if (m_number_of_vertices)
	{
		glDisableClientState(GL_VERTEX_ARRAY);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	// There are texture coordinates
	if (m_number_of_texture_coordinates)
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	// There are normal data
	if(m_number_of_normals)
	{
		glDisableClientState(GL_NORMAL_ARRAY);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


//------------------------------
inline bool VBO::isReady() const
//-----------------------------
{
	return (m_id_set[0] ? true : false);
}


//-----------------------------------------
inline VBO& VBO::operator=(const VBO& aFbo)
//-----------------------------------------
{
	memcpy(m_id_set, aFbo.m_id_set, sizeof(m_id_set));

	m_number_of_normals  = aFbo.m_number_of_normals;
	m_number_of_texture_coordinates = aFbo.m_number_of_texture_coordinates;
	m_number_of_vertices = aFbo.m_number_of_vertices;
	m_number_of_indices = aFbo.m_number_of_indices;

	m_normal_size = aFbo.m_normal_size;
	m_texture_coordinate_size = aFbo.m_texture_coordinate_size;
	m_vertex_size = aFbo.m_vertex_size;

	m_primitive_type = aFbo.m_primitive_type;
	
	return *this;
}


} // namespace Graphics
