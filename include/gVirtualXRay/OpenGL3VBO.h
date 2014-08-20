/**
********************************************************************************
*
*   @file       OpenGL3VBO.h
*
*   @brief      Class to handle vertex buffer objects (VBOs) in OpenGL 3.x or 4.x.
*
*   @version    1.2
*
*   @date       07/08/2014
*
*   @author     Dr Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


#ifndef OPENGL3_VBO_H
#define OPENGL3_VBO_H


//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_VBO_H
#include "gVirtualXRay/VBO.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*	@class	OpenGL3VBO
*	@brief	OpenGL3VBO is a class to handle a vertex buffer object (VBO) in OpenGL 3.x or 4.x.
*/
//==============================================================================
class OpenGL3VBO: public VBO
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	//--------------------------------------------------------------------------
	///	Default constructor.
	//--------------------------------------------------------------------------
    OpenGL3VBO();
	
	
	//--------------------------------------------------------------------------
	///	Destructor.
	//--------------------------------------------------------------------------
	virtual ~OpenGL3VBO();
	
	
    //--------------------------------------------------------------------------
    ///	Release the data on the GPU.
    //--------------------------------------------------------------------------
    virtual void destroy();


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
	virtual void setVertexData(int aPrimitiveType,
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
    virtual void setIndexData(const unsigned int aNumberOfIndexes,
            int anIndexDataType,
            const void* apIndexSet);


    //--------------------------------------------------------------------------
    ///	Display the triangular mesh using OpenGL.
    //--------------------------------------------------------------------------
    virtual void display() const;


	//--------------------------------------------------------------------------
	///	Copy operator.
	/**
	*	@param aVbo:	VBO to copy
	*	@return the new VBO
	*/
	//--------------------------------------------------------------------------
	virtual VBO& operator=(const OpenGL3VBO& aVbo);


//******************************************************************************
protected:
	/// ID of VAO
	unsigned int m_vao_id;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "OpenGL3VBO.inl"


#endif // OPENGL3_VBO_H

