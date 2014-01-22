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
	///	Set the data.
	/**
	*	@param aNumberOfVertices:	number of vertices
	*	@param apVertices:			address of the first vertex
	*	@param aNumberOfNormals:	number of normal vectors
	*	@param apNormals:			address of the first normal
	*	@return the ID of the new VBO
	*/
	//--------------------------------------------------------------------------
	void setVertexData(GLenum aPrimitiveType,
			const GLuint aNumberOfVertices, GLuint aNumberOfCompontentsPerVertex, const GLfloat* vertices,
			const GLuint aNumberOfNormals, GLuint aNumberOfCompontentsPerNormal,  const GLfloat* apNormals,
			const GLuint aNumberOfTextureCoordinates, GLuint aNumberOfCompontentsPerCoord, const GLfloat* apTextureCoordinates);


	//--------------------------------------------------------------------------
	///	Set the index.
	/**
	*	@param aNumberOfVertices:	number of vertices
	*	@param apVertices:			address of the first vertex
	*	@param aNumberOfNormals:	number of normal vectors
	*	@param apNormals:			address of the first normal
	*	@return the ID of the new VBO
	*/
	//--------------------------------------------------------------------------
	void setIndexData(const GLuint aNumberOfIndexes, const GLuint* apIndexSet);


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

