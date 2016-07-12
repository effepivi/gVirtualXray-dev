/*

Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
http://www.fpvidal.net/
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the Bangor University nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


#ifndef OPENGL3_VBO_H
#define OPENGL3_VBO_H


/**
********************************************************************************
*
*   @file       OpenGL3VBO.h
*
*   @brief      Class to handle vertex buffer objects (VBOs) in OpenGL 3.x or 4.x.
*
*   @version    1.0
*
*   @date       07/08/2014
*
*   @author     Dr Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net), 
*   http://www.fpvidal.net/, Dec 2014, 2014, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

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
            const unsigned int aNumberOfVertices,
            unsigned int aNumberOfCompontentsPerVertex,
            int aVertexDataType,
            const void* apVertices,
            const unsigned int aNumberOfNormals,
            unsigned int aNumberOfCompontentsPerNormal,
            int aNormalDataType,
            const void* apNormals,
            const unsigned int aNumberOfTextureCoordinates,
            unsigned int aNumberOfCompontentsPerCoord,
            int aCoordDataType,
            const void* apTextureCoordinates);


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

