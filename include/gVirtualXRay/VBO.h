/**
********************************************************************************
*
*	@file		VBO.h
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


#ifndef GVXR_VBO_H
#define GVXR_VBO_H


//******************************************************************************
//	namespace
//******************************************************************************
namespace Graphics {


//==============================================================================
/**
*	@class	VBO
*	@brief	VBO is a class to handle a vertex buffer object (VBO).
*/
//==============================================================================
class VBO
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	//--------------------------------------------------------------------------
	///	Default constructor.
	//--------------------------------------------------------------------------
	VBO();
	
	
	//--------------------------------------------------------------------------
	///	Destructor.
	//--------------------------------------------------------------------------
	~VBO();
	
	
	//--------------------------------------------------------------------------
	///	Release the data on the GPU.
	//--------------------------------------------------------------------------
	void destroy();


	//--------------------------------------------------------------------------
	///	Reset the data.
	//--------------------------------------------------------------------------
	void reset();
	
	
	//--------------------------------------------------------------------------
	///	Set the vertex data.
	/**
	*	@param aPrimitiveType:					the primitive type; valid values are:  GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY and GL_PATCHES
	*	@param aNumberOfVertices:				number of vertices
	*	@param aNumberOfCompontentsPerVertex:	the number of component per vertex
	*	@param aVertexDataType:					the data type of vertices; valid values are GL_FLOAT and GL_DOUBLE
	*	@param apVertices:						address of the first component of the first vertex
	*	@param aNumberOfNormals:				number of normal vectors
	*	@param aNumberOfCompontentsPerNormal:	the number of component per normal vector
	*	@param aNormalDataType:					the data type of normal vectors; valid values are GL_FLOAT and GL_DOUBLE
	*	@param apNormals:						address of the first component of the firth normal vector
	*	@param aNumberOfTextureCoordinates:		number of texture coordinates
	*	@param aNumberOfCompontentsPerCoord:	the number of component per texture coordinate
	*	@param aCoordDataType:					the data type of texture coordinates; valid values are GL_FLOAT and GL_DOUBLE
	*	@param apTextureCoordinates:			address of the first component of the first texture coordinate
	*/
	//--------------------------------------------------------------------------
	void setVertexData(int aPrimitiveType,
			const unsigned int aNumberOfVertices,           unsigned int aNumberOfCompontentsPerVertex, int aVertexDataType, const void* apVertices,
			const unsigned int aNumberOfNormals,            unsigned int aNumberOfCompontentsPerNormal, int aNormalDataType, const void* apNormals,
			const unsigned int aNumberOfTextureCoordinates, unsigned int aNumberOfCompontentsPerCoord,  int aCoordDataType,  const void* apTextureCoordinates);


	//--------------------------------------------------------------------------
	///	Set the index.
	/**
	*	@param aNumberOfIndexes:	number of indices
	*	@param anIndexDataType:		the data type of indices; valid values are GL_UNSGINED_BYTE, GL_UNSGINED_SHORT and GL_UNSGINED_INT
	*	@param apIndexSet:			the index set
	*/
	//--------------------------------------------------------------------------
	void setIndexData(const unsigned int aNumberOfIndexes, int anIndexDataType, const void* apIndexSet);


	//--------------------------------------------------------------------------
	///	Display the triangular mesh using OpenGL.
	//--------------------------------------------------------------------------
	void display() const;


	//--------------------------------------------------------------------------
	///	Check if the vertex buffer object is ready.
	/**
	*	@return true if the VBO is ready to use, false if it is not
	*/
	//--------------------------------------------------------------------------
	bool isReady() const;
	
	
	//--------------------------------------------------------------------------
	///	Set the buffer usage hint. Accepted values are:
	///	    GL_STATIC_DRAW is for vertex buffers that are rendered many times,
	///				and whose contents are specified once and never change.
	///		GL_DYNAMIC_DRAW is for vertex buffers that are rendered many times,
	///				and whose contents change during the rendering loop.
	///		GL_STREAM_DRAW is for vertex buffers that are rendered a small number of times and then discarded.
	/**
	*	@param aFlag: the buffer usage hint to set
	*/
	//--------------------------------------------------------------------------
	void setBufferUsageHint(int aFlag);


	//--------------------------------------------------------------------------
	///	Accessor on the buffer usage hint.
	/**
	*	@return the buffer usage hint
	*/
	//--------------------------------------------------------------------------
	int getBufferUsageHint() const;


	//--------------------------------------------------------------------------
	///	Accessor on the primitive type.
	/**
	*	@return the primitive type
	*/
	//--------------------------------------------------------------------------
	int getPrimitiveType() const;


	//--------------------------------------------------------------------------
	///	Copy operator.
	/**
	*	@param aVbo:	VBO to copy
	*	@return the new VBO
	*/
	//--------------------------------------------------------------------------
	VBO& operator=(const VBO& aVbo);

	
//******************************************************************************
protected:
	/// ID of VAO
	unsigned int m_vao_id;


	/// ID of VBOs
	unsigned int m_id_set[4];


	/// Number of normal vectors
	unsigned int m_number_of_normals;


	/// Number of vertices
	unsigned int m_number_of_texture_coordinates;


	/// Number of vertices
	unsigned int m_number_of_vertices;


	/// Number of indices
	unsigned int m_number_of_indices;


	/// Size in bytes of the array of normal vectors
	unsigned int m_normal_size;


	/// Size in bytes of the array of texture coordinates
	unsigned int m_texture_coordinate_size;


	/// Size in bytes of the array of vertices
	unsigned int m_vertex_size;


	/// Size in bytes of the array of indices
	unsigned int m_index_size;


	/// Data type of normal vectors; valid values are GL_FLOAT and GL_DOUBLE
	int m_normal_data_type;


	/// Data type of texture coordinates; valid values are GL_FLOAT and GL_DOUBLE
	int m_texture_coordinate_data_type;


	/// Data type of vertices; valid values are GL_FLOAT and GL_DOUBLE
	int m_vertex_data_type;


	/// Data type of indices; valid values are GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT and GL_UNSIGNED_INT
	int m_index_data_type;


	/// Type of primitive to draw; valid values are  GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY and GL_PATCHES
	int m_primitive_type;


	/// The buffer usage hint; valid values are GL_DYNAMIC_DRAW, GL_STATIC_DRAW and GL_STREAM_DRAW
	int m_storage_type;
};


} // namespace Graphics


//******************************************************************************
#include "VBO.inl"


#endif // VBO_H

