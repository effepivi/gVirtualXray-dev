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
			const GLuint aNumberOfVertices, GLuint aNumberOfCompontentsPerVertex, const GLfloat* apVertices,
			const GLuint aNumberOfNormals, GLuint aNumberOfCompontentsPerNormal,  const GLfloat* apNormals,
			const GLuint aNumberOfTextureCoordinates, GLuint aNumberOfCompontentsPerCoord, const GLfloat* apTextureCoordinates);


	//--------------------------------------------------------------------------
	///	Set the index.
	/**
	*	@param aNumberOfIndexes:	number of indices
	*	@param apIndexSet:			the index set
	*/
	//--------------------------------------------------------------------------
	void setIndexData(const GLuint aNumberOfIndexes, const GLuint* apIndexSet);


	//--------------------------------------------------------------------------
	///	Set the index.
	/**
	*	@param aNumberOfIndexes:	number of indices
	*	@param apIndexSet:			the index set
	*/
	//--------------------------------------------------------------------------
	void setIndexData(const GLuint aNumberOfIndexes, const GLint* apIndexSet);


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

	GLenum m_primitive_type;
};


} // namespace Graphics


//******************************************************************************
#include "VBO.inl"


#endif // VBO_H

