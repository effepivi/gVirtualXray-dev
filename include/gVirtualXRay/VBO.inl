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


//----------------
inline VBO::~VBO()
//----------------
{
	destroy();
}


//------------------------------
inline bool VBO::isReady() const
//-----------------------------
{
	return (m_id_set[0] ? true : false);
}


//--------------------------------------------
inline void VBO::setBufferUsageHint(int aFlag)
//--------------------------------------------
{
	m_storage_type = aFlag;
}


//----------------------------------------
inline int VBO::getBufferUsageHint() const
//----------------------------------------
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
