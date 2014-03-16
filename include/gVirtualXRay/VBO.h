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


#ifndef VBO_H
#define VBO_H


//******************************************************************************
//	Include
//******************************************************************************
#include <GL/glew.h>


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
	*	@param apVertices:						address of the first component of the first vertex
	*	@param aNumberOfNormals:				number of normal vectors
	*	@param aNumberOfCompontentsPerNormal:	the number of component per normal vector
	*	@param apNormals:						address of the first component of the firth normal vector
	*	@param aNumberOfTextureCoordinates:		number of texture coordinates
	*	@param aNumberOfCompontentsPerCoord:	the number of component per texture coordinate
	*	@param apTextureCoordinates:			address of the first component of the first texture coordinate
	*/
	//--------------------------------------------------------------------------
	void setVertexData(GLenum aPrimitiveType,
			const GLuint aNumberOfVertices,           GLuint aNumberOfCompontentsPerVertex, GLenum aVertexDataType, const GLvoid* apVertices,
			const GLuint aNumberOfNormals,            GLuint aNumberOfCompontentsPerNormal, GLenum aNormalDataType, const GLvoid* apNormals,
			const GLuint aNumberOfTextureCoordinates, GLuint aNumberOfCompontentsPerCoord,  GLenum aCoordDataType,  const GLvoid* apTextureCoordinates);


	//--------------------------------------------------------------------------
	///	Set the index.
	/**
	*	@param aNumberOfIndexes:	number of indices
	*	@param apIndexSet:			the index set
	*/
	//--------------------------------------------------------------------------
	void setIndexData(const GLuint aNumberOfIndexes, GLenum anIndexDataType, const GLvoid* apIndexSet);


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
	void setBufferUsageHint(GLenum aFlag);


	//--------------------------------------------------------------------------
	///	Accessor on the buffer usage hint.
	/**
	*	@return the buffer usage hint
	*/
	//--------------------------------------------------------------------------
	GLenum getBufferUsageHint() const;


	//--------------------------------------------------------------------------
	///	Copy operator.
	/**
	*	@param aFbo:	VBO to copy
	*	@return the new VBO
	*/
	//--------------------------------------------------------------------------
	VBO& operator=(const VBO& aFbo);

	
//******************************************************************************
protected:
	/// ID of VBOs
	GLuint m_id_set[4];


	/// Number of normal vectors
	GLuint m_number_of_normals;


	/// Number of vertices
	GLuint m_number_of_texture_coordinates;


	/// Number of vertices
	GLuint m_number_of_vertices;

	GLuint m_number_of_indices;

	GLuint m_normal_size;
	GLuint m_texture_coordinate_size;
	GLuint m_vertex_size;
	GLuint m_index_size;

	GLenum m_normal_data_type;
	GLenum m_texture_coordinate_data_type;
	GLenum m_vertex_data_type;
	GLenum m_index_data_type;

	GLenum m_primitive_type;

	GLenum m_storage_type; ///< GL_DYNAMIC_DRAW, GL_STATIC_DRAW, GL_STREAM_DRAW
};


} // namespace Graphics


//******************************************************************************
#include "VBO.inl"


#endif // VBO_H

