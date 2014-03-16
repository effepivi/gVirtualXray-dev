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
	m_index_size(0),
	m_normal_data_type(GL_NONE),
	m_texture_coordinate_data_type(GL_NONE),
	m_vertex_data_type(GL_NONE),
	m_index_data_type(GL_NONE),	
	m_primitive_type(GL_NONE),
	m_storage_type(GL_DYNAMIC_DRAW)
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
	destroy();
}


//------------------------
inline void VBO::destroy()
//------------------------
{
	if (m_id_set[VERTEX_SET_ID])     glDeleteBuffers(1, &m_id_set[VERTEX_SET_ID]);
	if (m_id_set[TEXT_COORD_SET_ID]) glDeleteBuffers(1, &m_id_set[TEXT_COORD_SET_ID]);
	if (m_id_set[NORMAL_SET_ID])     glDeleteBuffers(1, &m_id_set[NORMAL_SET_ID]);
	if (m_id_set[INDEX_SET_ID])      glDeleteBuffers(1, &m_id_set[INDEX_SET_ID]);

	m_id_set[VERTEX_SET_ID]     = 0;
	m_id_set[TEXT_COORD_SET_ID] = 0;
	m_id_set[NORMAL_SET_ID]     = 0;
	m_id_set[INDEX_SET_ID]      = 0;

	reset();
}


//----------------------
inline void VBO::reset()
//----------------------
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

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


//------------------------------
inline void VBO::display() const
//------------------------------
{
	if (m_primitive_type != GL_NONE)
	{
		// There is vertices
		if (m_number_of_vertices && m_vertex_data_type != GL_NONE)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_id_set[VERTEX_SET_ID]);
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, m_vertex_data_type, 0, 0);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		}
		
		// There is texture coordinates
		if (m_number_of_texture_coordinates && m_texture_coordinate_data_type != GL_NONE)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_id_set[TEXT_COORD_SET_ID]);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(3, m_texture_coordinate_data_type, 0, 0);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		}
	
		// There is normal data
		if (m_number_of_normals && m_normal_data_type != GL_NONE)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_id_set[NORMAL_SET_ID]);
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(m_normal_data_type, 0, 0);
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
			glDrawElements(m_primitive_type, m_number_of_indices, m_index_data_type, 0); 
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
	else
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid primitive type.");
	}
}


//------------------------------
inline bool VBO::isReady() const
//-----------------------------
{
	return (m_id_set[0] ? true : false);
}


//-----------------------------------------------
inline void VBO::setBufferUsageHint(GLenum aFlag)
//-----------------------------------------------
{
	m_storage_type = aFlag;
}


//-------------------------------------------
inline GLenum VBO::getBufferUsageHint() const
//-------------------------------------------
{
	return (m_storage_type);
}


//-----------------------------------------
inline VBO& VBO::operator=(const VBO& aVBO)
//-----------------------------------------
{
	memcpy(m_id_set, aVBO.m_id_set, sizeof(m_id_set));

	m_number_of_normals             = aVBO.m_number_of_normals;
	m_number_of_texture_coordinates = aVBO.m_number_of_texture_coordinates;
	m_number_of_vertices            = aVBO.m_number_of_vertices;
	m_number_of_indices             = aVBO.m_number_of_indices;

	m_normal_size             = aVBO.m_normal_size;
	m_texture_coordinate_size = aVBO.m_texture_coordinate_size;
	m_vertex_size             = aVBO.m_vertex_size;
	m_index_size              = aVBO.m_index_size;

	m_normal_data_type             = aVBO.m_normal_data_type;
	m_texture_coordinate_data_type = aVBO.m_texture_coordinate_data_type;
	m_vertex_data_type             = aVBO.m_vertex_data_type;
	m_index_data_type              = aVBO.m_index_data_type;

	m_primitive_type = aVBO.m_primitive_type;
	
	m_storage_type = aVBO.m_storage_type;

	return (*this);
}


} // namespace Graphics
