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


#ifndef GVXR_POLYGON_MESH_H
#define GVXR_POLYGON_MESH_H


/**
********************************************************************************
*
*	@file		PolygonMesh.h
*
*	@brief		Class to handle polygon (triangles) meshes.
*
*	@version	1.0
*
*	@date		07/11/2013
*
*	@author		Dr Franck P. Vidal
*
*	@todo		Make it possible to merge vertices if an array of indices exists
*				Convert triangle mesh into arbitrary polygon mesh
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
//	Include
//******************************************************************************
#include <string>
#include <vector>
#include <boost/scoped_ptr.hpp>

#ifndef GVXR_VBO_H
#include "gVirtualXRay/VBO.h"
#endif

#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#ifndef GVXR_MATERIAL_H
#include "gVirtualXRay/Material.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*	@class	PolygonMesh
*	@brief	PolygonMesh is a class to handle polygon (triangles) meshes.
*/
//==============================================================================
class PolygonMesh
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	//--------------------------------------------------------------------------
	///	Default constructor.
	//--------------------------------------------------------------------------
	PolygonMesh();


    //--------------------------------------------------------------------------
    /// Copy constructor.
    /**
    *   @param aPolygonMesh:    the polygon mesh to copy
    */
    //--------------------------------------------------------------------------
    PolygonMesh(const PolygonMesh& aPolygonMesh);


    //--------------------------------------------------------------------------
	///	Destructor.
	//--------------------------------------------------------------------------
	~PolygonMesh();


	//--------------------------------------------------------------------------
	///	Reset the data.
	//--------------------------------------------------------------------------
	void reset();


	//--------------------------------------------------------------------------
	///	Display the triangular mesh using OpenGL.
	//--------------------------------------------------------------------------
	void display();


	//--------------------------------------------------------------------------
	///	Display the triangular mesh in wireframe using OpenGL.
	//--------------------------------------------------------------------------
	void displayWireFrame();


	//--------------------------------------------------------------------------
	///	Display the face normal vectors.
	/**
	*	@param aNormalSize:	the size of the vectors (default value is 0.5 cm)
	*/
	//--------------------------------------------------------------------------
	void displayFaceNormal(RATIONAL_NUMBER aNormalSize = 0.5 * cm) const;


	//--------------------------------------------------------------------------
	///	Display the vertex normal vectors.
	/**
	*	@param aNormalSize:	the size of the vectors (default value is 0.5 cm)
	*/
	//--------------------------------------------------------------------------
	void displayVertexNormal(RATIONAL_NUMBER aNormalSize = 0.5 * cm) const;


	//--------------------------------------------------------------------------
	///	Set the name of the file that contains the polygon mesh.
	/**
	*	@param aFilename:	the file name
	*/
	//--------------------------------------------------------------------------
	void setFilename(const char* aFilename);


	//--------------------------------------------------------------------------
	///	Set the name of the file that contains the polygon mesh.
	/**
	*	@param aFilename:	the file name
	*/
	//--------------------------------------------------------------------------
	void setFilename(const std::string& aFilename);


	//--------------------------------------------------------------------------
	///	Set the name of the file that contains the polygon mesh.
	/**
	*	@param aFilename:	the file name
	*/
	//--------------------------------------------------------------------------
	void setFileName(const char* aFilename);


	//--------------------------------------------------------------------------
	///	Set the name of the file that contains the polygon mesh.
	/**
	*	@param aFilename:	the file name
	*/
	//--------------------------------------------------------------------------
	void setFileName(const std::string& aFilename);


	//--------------------------------------------------------------------------
	///	Accessor on the name of the file that contains the polygon mesh.
	/**
	*	@return the file name
	*/
	//--------------------------------------------------------------------------
	const std::string& getFilename() const;


	//--------------------------------------------------------------------------
	///	Accessor on the name of the file that contains the polygon mesh.
	/**
	*	@return the file name
	*/
	//--------------------------------------------------------------------------
	const std::string& getFileName() const;


	//--------------------------------------------------------------------------
	///	Set the name of the file that contains the polygon mesh.
	/**
	*	@param aMoveToCentreFlag:	a flag to move the polygon to the center or not
	*	@param anAutoComputeNormalFlag:	a flag to compute normals or nor
	*	@param aPrintDebugInfoFlag:	a flag to display additional debug information in the console or not
	*	@param aCreateVBOFlag:	a flag to use a vertex buffer object or not
	*	@param aScale:	the unit in which data is stored in the file (e.g. mm, cm, m)
	*	@param aBufferUsageHing:	the buffer usage hint; valid values are: GL_STATIC_DRAW, GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	*/
	//--------------------------------------------------------------------------
	void loadSTLFile(bool aMoveToCentreFlag,
		bool anAutoComputeNormalFlag,
		bool aPrintDebugInfoFlag,
		bool aCreateVBOFlag,
		RATIONAL_NUMBER aScale,
		int aBufferUsageHing);


	//--------------------------------------------------------------------------
	///	Set the name of the file that contains the polygon mesh.
	/**
	*	@param apInputData:	the input data to load
	*	@param aMoveToCentreFlag:	a flag to move the polygon to the center or not
	*	@param anAutoComputeNormalFlag:	a flag to compute normals or nor
	*	@param aPrintDebugInfoFlag:	a flag to display additional debug information in the console or not
	*	@param aCreateVBOFlag:	a flag to use a vertex buffer object or not
	*	@param aScale:	the unit in which data is stored in the file (e.g. mm, cm, m)
	*	@param aBufferUsageHing:	the buffer usage hint; valid values are: GL_STATIC_DRAW, GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	*/
	//--------------------------------------------------------------------------
	void loadSTLDataFromStream(const char* apInputData,
			bool aMoveToCentreFlag,
			bool anAutoComputeNormalFlag,
			bool aPrintDebugInfoFlag,
			bool aCreateVBOFlag,
			RATIONAL_NUMBER aScale,
			int aBufferUsageHing);


	//--------------------------------------------------------------------------
	///	Write the STL file.
	//--------------------------------------------------------------------------
	void writeSTLFile();


	//--------------------------------------------------------------------------
	///	Move the polygon to the center.
	//--------------------------------------------------------------------------
	void moveToCentre();


	//--------------------------------------------------------------------------
	///	Compute the normal vectors using the cross product method.
	//--------------------------------------------------------------------------
	void computeNormalVectors();


	//--------------------------------------------------------------------------
	///	Use a vertex buffer object if possible.
	/**
	*	@param aBufferUsageHing:	the buffer usage hint; valid values are: GL_STATIC_DRAW, GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	*	@param aTypeOfPrimitive: type of primitive; valid values are GL_TRIANGLES
	*	@return true if the VBO can be used, false otherwise
	*/
	//--------------------------------------------------------------------------
	bool useVBO(int aBufferUsageHing, int aTypeOfPrimitive);


	//--------------------------------------------------------------------------
	///	Set the Hounsfield value of the object material.
	/**
	*	@param aHounsfieldValue:	the Hounsfield value of the object material
	*/
	//--------------------------------------------------------------------------
	void setHounsfieldValue(const RATIONAL_NUMBER& aHounsfieldValue);


	//--------------------------------------------------------------------------
	///	Set the attenuation coefficient of the object material.
	/**
	*	@param anAttenuationCoefficient:	the attenuation coefficient of the object material
	*/
	//--------------------------------------------------------------------------
	void setAttenuationCoefficient(const RATIONAL_NUMBER& anAttenuationCoefficient);
	
	
	//--------------------------------------------------------------------------
	///	Accessor on the flag about the use of the Hounsfield value or
	///	attenuation coefficient.
	/**
	*	@return	true the Hounsfield value is used,
	*			false if the attenuation coefficient is used
	*/
	//--------------------------------------------------------------------------
	bool getUseHounsfieldValueFlag() const;


	//--------------------------------------------------------------------------
	///	Accessor on the Hounsfield value.
	/**
	*	@return the Hounsfield value
	*/
	//--------------------------------------------------------------------------
	RATIONAL_NUMBER getHounsfieldValue() const;


	//--------------------------------------------------------------------------
	///	Accessor on the attenuation coefficient.
	/**
	*	@return the attenuation coefficient
	*/
	//--------------------------------------------------------------------------
	RATIONAL_NUMBER getAttenuationCoefficient() const;


	//--------------------------------------------------------------------------
	///	Accessor on the bounding box lower corner.
	/**
	*	@return the bounding box lower corner
	*/
	//--------------------------------------------------------------------------
	const VEC3& getMinCorner() const;


	//--------------------------------------------------------------------------
	///	Accessor on the bounding box upper corner.
	/**
	*	@return the bounding box upper corner
	*/
	//--------------------------------------------------------------------------
	const VEC3& getMaxCorner() const;


	//--------------------------------------------------------------------------
	///	Use a vertex buffer object if possible.
	/**
	*	@param aTranslationVector:	the translation vector
	*/
	//--------------------------------------------------------------------------
	void translate(const VEC3& aTranslationVector);
    
    
	//--------------------------------------------------------------------------
	///	Accessor on the number of faces.
	/**
	*	@return the number of faces
	*/
	//--------------------------------------------------------------------------
	unsigned int getFaceNumber() const;


	//--------------------------------------------------------------------------
	///	Accessor on the number of triangles.
	/**
	*	@return the number of triangles
	*/
	//--------------------------------------------------------------------------
	unsigned int getTriangleNumber() const;


	//--------------------------------------------------------------------------
	///	Accessor on the number of face normal vectors.
	/**
	*	@return the number of face normal vectors
	*/
	//--------------------------------------------------------------------------
	unsigned int getFaceNormalNumber() const;


	//--------------------------------------------------------------------------
	///	Accessor on the number of vertex normal vectors.
	/**
	*	@return the number of vertex normal vectors
	*/
	//--------------------------------------------------------------------------
	unsigned int getVertexNormalNumber() const;


	//--------------------------------------------------------------------------
	///	Accessor on the number of vertices.
	/**
	*	@return the number of vertices
	*/
	//--------------------------------------------------------------------------
	unsigned int getVertexNumber() const;


	//--------------------------------------------------------------------------
	///	Accessor on the number of indices.
	/**
	*	@return the number of indices
	*/
	//--------------------------------------------------------------------------
	unsigned int getIndexNumber() const;


	//--------------------------------------------------------------------------
	///	Merge the vertices of the mesh if the data is managed by the
	///	class instance, and if no index has been build.
	//--------------------------------------------------------------------------
	void mergeVertices(bool aPrintDebugInfoFlag = false);


	//--------------------------------------------------------------------------
	///	Split each face into four faces
	//--------------------------------------------------------------------------
	void splitFaces(bool aPrintDebugInfoFlag = false);


	//--------------------------------------------------------------------------
	///	Accessor on a given vertex.
	/**
	*	@param anIndex: the index of the vertex to access
	*	@return the vertex
	*/
	//--------------------------------------------------------------------------
	VEC3 getVertex(unsigned int anIndex) const;


	//--------------------------------------------------------------------------
	///	Change the value of a given vertex.
	/**
	*	@param anIndex: the index of the vertex to access
	*	@param aVertex: the new value of the vertex
	*/
	//--------------------------------------------------------------------------
	void setVertex(unsigned int anIndex, const VEC3& aVertex);


	//--------------------------------------------------------------------------
	///	Accessor on a given index.
	/**
	*	@param anIndex: the index of the index to access
	*	@return the index
	*/
	//--------------------------------------------------------------------------
	int getIndex(unsigned int anIndex) const;


	//--------------------------------------------------------------------------
	///	Accessor on a given face normal.
	/**
	*	@param anIndex: the index of the normal to access
	*	@return the normal
	*/
	//--------------------------------------------------------------------------
	VEC3 getFaceNormal(unsigned int anIndex) const;


	//--------------------------------------------------------------------------
	///	Accessor on a given vertex normal.
	/**
	*	@param anIndex: the index of the normal to access
	*	@return the normal
	*/
	//--------------------------------------------------------------------------
	VEC3 getVertexNormal(unsigned int anIndex) const;


	//--------------------------------------------------------------------------
	///	Set the polygon mesh using external data. The class instance will not
	///	manage the memory.
	/**
	*	@param aTypeOfPrimitive: 	type of primitive; valid values are GL_TRIANGLES
	*	@param aVertexArray:		array containing the vertex data
	*	@param aNumberOfVertices:	number of vertices
	*	@param aVertexDataType:		data type of the vertex data; valid values are GL_FLOAT, GL_DOUBLE and GL_NONE
	*	@param anIndexArray:		array containing the index data
	*	@param aNumberOfIndices:	number of indices
	*	@param anIndexDataType:		data type of the index data; valid values are GL_UNSIGNED_INT, GL_UNSIGNED_SHORT, GL_UNSIGNED_BYTE and GL_NONE
	*	@param aCreateVBOFlag: 		flag set to true to create a VBO, set to false to not set a VBO
	*	@param aTypeOfVBO: 			VBO usage hint to set; valid values are GL_STATIC_DRAW, GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	*/
	//--------------------------------------------------------------------------
	void setExternalData(int aTypeOfPrimitive,
			const void* aVertexArray,
			unsigned int aNumberOfVertices,
			int aVertexDataType,
			const void* anIndexArray,
			unsigned int aNumberOfIndices,
			int anIndexDataType,
			bool aCreateVBOFlag,
			int aTypeOfVBO);


	//--------------------------------------------------------------------------
	///	Set the polygon mesh using external data. The class instance will not
	///	manage the memory.
	/**
	*	@param aTypeOfPrimitive: 	type of primitive; valid values are GL_TRIANGLES
	*	@param aVertexArray:		array containing the vertex data
	*	@param aCreateVBOFlag: 		flag set to true to create a VBO, set to false to not set a VBO
	*	@param aTypeOfVBO: 			VBO usage hint to set; valid values are GL_STATIC_DRAW, GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	*/
	//--------------------------------------------------------------------------
	void setExternalData(int aTypeOfPrimitive,
			const std::vector<float>* aVertexArray,
			bool aCreateVBOFlag,
			int aTypeOfVBO);


	//--------------------------------------------------------------------------
	///	Set the polygon mesh using external data. The class instance will not
	///	manage the memory.
	/**
	*	@param aTypeOfPrimitive: 	type of primitive; valid values are GL_TRIANGLES
	*	@param aVertexArray:		array containing the vertex data
	*	@param aCreateVBOFlag: 		flag set to true to create a VBO, set to false to not set a VBO
	*	@param aTypeOfVBO: 			VBO usage hint to set; valid values are GL_STATIC_DRAW, GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	*/
	//--------------------------------------------------------------------------
	void setExternalData(int aTypeOfPrimitive,
			const std::vector<double>* aVertexArray,
			bool aCreateVBOFlag,
			int aTypeOfVBO);


	//--------------------------------------------------------------------------
	///	Set the polygon mesh using external data. The class instance will not
	///	manage the memory.
	/**
	*	@param aTypeOfPrimitive: 	type of primitive; valid values are GL_TRIANGLES
	*	@param aVertexArray:		array containing the vertex data
	*	@param anIndexArray:		array containing the index data
	*	@param aCreateVBOFlag: 		flag set to true to create a VBO, set to false to not set a VBO
	*	@param aTypeOfVBO: 			VBO usage hint to set; valid values are GL_STATIC_DRAW, GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	*/
	//--------------------------------------------------------------------------
	void setExternalData(int aTypeOfPrimitive,
			const std::vector<float>* aVertexArray,
			const std::vector<unsigned int>* anIndexArray,
			bool aCreateVBOFlag,
			int aTypeOfVBO);


	//--------------------------------------------------------------------------
	///	Set the polygon mesh using external data. The class instance will not
	///	manage the memory.
	/**
	*	@param aTypeOfPrimitive: 	type of primitive; valid values are GL_TRIANGLES
	*	@param aVertexArray:		array containing the vertex data
	*	@param anIndexArray:		array containing the index data
	*	@param aCreateVBOFlag: 		flag set to true to create a VBO, set to false to not set a VBO
	*	@param aTypeOfVBO: 			VBO usage hint to set; valid values are GL_STATIC_DRAW, GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	*/
	//--------------------------------------------------------------------------
	void setExternalData(int aTypeOfPrimitive,
			const std::vector<double>* aVertexArray,
			const std::vector<unsigned int>* anIndexArray,
			bool aCreateVBOFlag,
			int aTypeOfVBO);


	//--------------------------------------------------------------------------
	///	Set the polygon mesh using external data. The class instance will not
	///	manage the memory.
	/**
	*	@param aTypeOfPrimitive: 	type of primitive; valid values are GL_TRIANGLES
	*	@param aVertexArray:		array containing the vertex data
	*	@param anIndexArray:		array containing the index data
	*	@param aCreateVBOFlag: 		flag set to true to create a VBO, set to false to not set a VBO
	*	@param aTypeOfVBO: 			VBO usage hint to set; valid values are GL_STATIC_DRAW, GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	*/
	//--------------------------------------------------------------------------
	void setExternalData(int aTypeOfPrimitive,
			const std::vector<float>* aVertexArray,
			const std::vector<unsigned short>* anIndexArray,
			bool aCreateVBOFlag,
			int aTypeOfVBO);


	//--------------------------------------------------------------------------
	///	Set the polygon mesh using external data. The class instance will not
	///	manage the memory.
	/**
	*	@param aTypeOfPrimitive:	type of primitive; valid values are GL_TRIANGLES
	*	@param aVertexArray:		array containing the vertex data
	*	@param anIndexArray:		array containing the index data
	*	@param aCreateVBOFlag: 		flag set to true to create a VBO, set to false to not set a VBO
	*	@param aTypeOfVBO: 			VBO usage hint to set; valid values are GL_STATIC_DRAW, GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	*/
	//--------------------------------------------------------------------------
	void setExternalData(int aTypeOfPrimitive,
			const std::vector<double>* aVertexArray,
			const std::vector<unsigned short>* anIndexArray,
			bool aCreateVBOFlag,
			int aTypeOfVBO);


	//--------------------------------------------------------------------------
	///	Set the polygon mesh using external data. The class instance will not
	///	manage the memory.
	/**
	*	@param aTypeOfPrimitive: 	type of primitive; valid values are GL_TRIANGLES
	*	@param aVertexArray:		array containing the vertex data
	*	@param anIndexArray:		array containing the index data
	*	@param aCreateVBOFlag: 		flag set to true to create a VBO, set to false to not set a VBO
	*	@param aTypeOfVBO: 			VBO usage hint to set; valid values are GL_STATIC_DRAW, GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	*/
	//--------------------------------------------------------------------------
	void setExternalData(int aTypeOfPrimitive,
			const std::vector<float>* aVertexArray,
			const std::vector<unsigned char>* anIndexArray,
			bool aCreateVBOFlag,
			int aTypeOfVBO);


	//--------------------------------------------------------------------------
	///	Set the polygon mesh using external data. The class instance will not
	///	manage the memory.
	/**
	*	@param aTypeOfPrimitive: 	type of primitive; valid values are GL_TRIANGLES
	*	@param aVertexArray:		array containing the vertex data
	*	@param anIndexArray:		array containing the index data
	*	@param aCreateVBOFlag: 		flag set to true to create a VBO, set to false to not set a VBO
	*	@param aTypeOfVBO: 			VBO usage hint to set; valid values are GL_STATIC_DRAW, GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	*/
	//--------------------------------------------------------------------------
	void setExternalData(int aTypeOfPrimitive,
			const std::vector<double>* aVertexArray,
			const std::vector<unsigned char>* anIndexArray,
			bool aCreateVBOFlag,
			int aTypeOfVBO);


	//--------------------------------------------------------------------------
	///	Set the polygon mesh using external data. The class instance will
	///	manage the memory.
	/**
	*	@param aTypeOfPrimitive: 	type of primitive; valid values are GL_TRIANGLES
	*	@param aVertexArray:		array containing the vertex data
	*	@param aNumberOfVertices:	number of vertices
	*	@param aVertexDataType:		data type of the vertex data; valid values are GL_FLOAT, GL_DOUBLE and GL_NONE
	*	@param anIndexArray:		array containing the index data
	*	@param aNumberOfIndices:	number of indices
	*	@param anIndexDataType:		data type of the index data; valid values are GL_UNSIGNED_INT, GL_UNSIGNED_SHORT, GL_UNSIGNED_BYTE and GL_NONE
	*	@param aCreateVBOFlag: 		flag set to true to create a VBO, set to false to not set a VBO
	*	@param aTypeOfVBO: 			VBO usage hint to set; valid values are GL_STATIC_DRAW, GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	*/
	//--------------------------------------------------------------------------
	void setInternalData(int aTypeOfPrimitive,
			const void* aVertexArray,
			unsigned int aNumberOfVertices,
			int aVertexDataType,
			const void* anIndexArray,
			unsigned int aNumberOfIndices,
			int anIndexDataType,
			bool aCreateVBOFlag,
			int aTypeOfVBO);


	//--------------------------------------------------------------------------
	///	Set the polygon mesh using external data. The class instance will
	///	manage the memory.
	/**
	*	@param aTypeOfPrimitive: 	type of primitive; valid values are GL_TRIANGLES
	*	@param aVertexArray:		array containing the vertex data
	*	@param aCreateVBOFlag: 		flag set to true to create a VBO, set to false to not set a VBO
	*	@param aTypeOfVBO: 			VBO usage hint to set; valid values are GL_STATIC_DRAW, GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	*/
	//--------------------------------------------------------------------------
	void setInternalData(int aTypeOfPrimitive,
			const std::vector<float>* aVertexArray,
			bool aCreateVBOFlag,
			int aTypeOfVBO);


	//--------------------------------------------------------------------------
	///	Set the polygon mesh using external data. The class instance will
	///	manage the memory.
	/**
	*	@param aTypeOfPrimitive: 	type of primitive; valid values are GL_TRIANGLES
	*	@param aVertexArray:		array containing the vertex data
	*	@param aCreateVBOFlag: 		flag set to true to create a VBO, set to false to not set a VBO
	*	@param aTypeOfVBO: 			VBO usage hint to set; valid values are GL_STATIC_DRAW, GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	*/
	//--------------------------------------------------------------------------
	void setInternalData(int aTypeOfPrimitive,
			const std::vector<double>* aVertexArray,
			bool aCreateVBOFlag,
			int aTypeOfVBO);


	//--------------------------------------------------------------------------
	///	Set the polygon mesh using external data. The class instance will
	///	manage the memory.
	/**
	*	@param aTypeOfPrimitive: 	type of primitive; valid values are GL_TRIANGLES
	*	@param aVertexArray:		array containing the vertex data
	*	@param anIndexArray:		array containing the index data
	*	@param aCreateVBOFlag: 		flag set to true to create a VBO, set to false to not set a VBO
	*	@param aTypeOfVBO: 			VBO usage hint to set; valid values are GL_STATIC_DRAW, GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	*/
	//--------------------------------------------------------------------------
	void setInternalData(int aTypeOfPrimitive,
			const std::vector<float>* aVertexArray,
			const std::vector<unsigned int>* anIndexArray,
			bool aCreateVBOFlag,
			int aTypeOfVBO);


	//--------------------------------------------------------------------------
	///	Set the polygon mesh using external data. The class instance will
	///	manage the memory.
	/**
	*	@param aTypeOfPrimitive: 	type of primitive; valid values are GL_TRIANGLES
	*	@param aVertexArray:		array containing the vertex data
	*	@param anIndexArray:		array containing the index data
	*	@param aCreateVBOFlag: 		flag set to true to create a VBO, set to false to not set a VBO
	*	@param aTypeOfVBO: 			VBO usage hint to set; valid values are GL_STATIC_DRAW, GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	*/
	//--------------------------------------------------------------------------
	void setInternalData(int aTypeOfPrimitive,
			const std::vector<double>* aVertexArray,
			const std::vector<unsigned int>* anIndexArray,
			bool aCreateVBOFlag,
			int aTypeOfVBO);


	//--------------------------------------------------------------------------
	///	Set the polygon mesh using external data. The class instance will
	///	manage the memory.
	/**
	*	@param aTypeOfPrimitive: 	type of primitive; valid values are GL_TRIANGLES
	*	@param aVertexArray:		array containing the vertex data
	*	@param anIndexArray:		array containing the index data
	*	@param aCreateVBOFlag: 		flag set to true to create a VBO, set to false to not set a VBO
	*	@param aTypeOfVBO: 			VBO usage hint to set; valid values are GL_STATIC_DRAW, GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	*/
	//--------------------------------------------------------------------------
	void setInternalData(int aTypeOfPrimitive,
			const std::vector<float>* aVertexArray,
			const std::vector<unsigned short>* anIndexArray,
			bool aCreateVBOFlag,
			int aTypeOfVBO);


	//--------------------------------------------------------------------------
	///	Set the polygon mesh using external data. The class instance will
	///	manage the memory.
	/**
	*	@param aTypeOfPrimitive:	type of primitive; valid values are GL_TRIANGLES
	*	@param aVertexArray:		array containing the vertex data
	*	@param anIndexArray:		array containing the index data
	*	@param aCreateVBOFlag: 		flag set to true to create a VBO, set to false to not set a VBO
	*	@param aTypeOfVBO: 			VBO usage hint to set; valid values are GL_STATIC_DRAW, GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	*/
	//--------------------------------------------------------------------------
	void setInternalData(int aTypeOfPrimitive,
			const std::vector<double>* aVertexArray,
			const std::vector<unsigned short>* anIndexArray,
			bool aCreateVBOFlag,
			int aTypeOfVBO);


	//--------------------------------------------------------------------------
	///	Set the polygon mesh using external data. The class instance will
	///	manage the memory.
	/**
	*	@param aTypeOfPrimitive: 	type of primitive; valid values are GL_TRIANGLES
	*	@param aVertexArray:		array containing the vertex data
	*	@param anIndexArray:		array containing the index data
	*	@param aCreateVBOFlag: 		flag set to true to create a VBO, set to false to not set a VBO
	*	@param aTypeOfVBO: 			VBO usage hint to set; valid values are GL_STATIC_DRAW, GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	*/
	//--------------------------------------------------------------------------
	void setInternalData(int aTypeOfPrimitive,
			const std::vector<float>* aVertexArray,
			const std::vector<unsigned char>* anIndexArray,
			bool aCreateVBOFlag,
			int aTypeOfVBO);


	//--------------------------------------------------------------------------
	///	Set the polygon mesh using external data. The class instance will
	///	manage the memory.
	/**
	*	@param aTypeOfPrimitive: 	type of primitive; valid values are GL_TRIANGLES
	*	@param aVertexArray:		array containing the vertex data
	*	@param anIndexArray:		array containing the index data
	*	@param aCreateVBOFlag: 		flag set to true to create a VBO, set to false to not set a VBO
	*	@param aTypeOfVBO: 			VBO usage hint to set; valid values are GL_STATIC_DRAW, GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	*/
	//--------------------------------------------------------------------------
	void setInternalData(int aTypeOfPrimitive,
			const std::vector<double>* aVertexArray,
			const std::vector<unsigned char>* anIndexArray,
			bool aCreateVBOFlag,
			int aTypeOfVBO);


	//--------------------------------------------------------------------------
	///	Apply a scaling factor to the vertex data. Note that the bounding box
	///	will be automatically updated.
	/**
	*	@param aScale:	scaling factor
	*/
	//--------------------------------------------------------------------------
	void applyScale(const RATIONAL_NUMBER& aScale);


	//--------------------------------------------------------------------------
	///	Inverse the normal vectors.
	//--------------------------------------------------------------------------
	void inverNormalVectors();


	//--------------------------------------------------------------------------
	///	Update the VBO.
	/**
	*	@param aTypeOfVBO: 			VBO usage hint to set; valid values are GL_STATIC_DRAW, GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	*	@param aTypeOfPrimitive:	type of primitive; valid values are GL_TRIANGLES
	*/
	//--------------------------------------------------------------------------
	virtual void updateVBO(int aTypeOfVBO, int aTypeOfPrimitive);


    //--------------------------------------------------------------------------
    /// Accessor on the material of the polygon mesh.
    /**
    *   @return the material
    */
    //--------------------------------------------------------------------------
    const Material& getMaterial() const;


    //--------------------------------------------------------------------------
    /// Accessor on the material of the polygon mesh.
    /**
    *   @return the material
    */
    //--------------------------------------------------------------------------
    Material& getMaterial();


    //--------------------------------------------------------------------------
    /// Copy operator.
    /**
    *   @param aPolygonMesh:    the polygon mesh to copy
    *   @return the new PolygonMesh
    */
    //--------------------------------------------------------------------------
    virtual PolygonMesh& operator=(const PolygonMesh& aPolygonMesh);


//******************************************************************************
protected:
	//--------------------------------------------------------------------------
	///	Update the bounding box.
	//--------------------------------------------------------------------------
	void computeBoundingBox();


	//--------------------------------------------------------------------------
	///	Normalise the normal vectors.
	//--------------------------------------------------------------------------
	void normaliseNormals();


	//--------------------------------------------------------------------------
	///	Release the memory held by the vertex data if needed.
	//--------------------------------------------------------------------------
	void destroyVertexData();


	//--------------------------------------------------------------------------
	///	Release the memory held by the index data if needed.
	//--------------------------------------------------------------------------
	void destroyIndexData();


	//--------------------------------------------------------------------------
	///	Remove the index.
	//--------------------------------------------------------------------------
	void removeIndex(bool aPrintDebugInfoFlag = false);


    //--------------------------------------------------------------------------
    /// Copy a vertex set.
    //--------------------------------------------------------------------------
    void copyVertexSet(const void* apVertexSet);


    //--------------------------------------------------------------------------
    /// Copy an index set.
    //--------------------------------------------------------------------------
    void copyIndexSet(const void* apIndexSet);


	/// Name of the file that contains the polygon mesh
	std::string m_filename;


	/// Vertex buffer object
	boost::scoped_ptr<VBO> m_vbo;


	/// Flag about the use of the Hounsfield value or attenuation coefficient
	bool m_use_hounsfield_value;


	/// Hounsfield value
	RATIONAL_NUMBER m_hounsfield_value;


	/// Attenuation coefficient
	RATIONAL_NUMBER m_attenuation_coefficient;


	/// Length of the bounding box diagonal
	RATIONAL_NUMBER m_bounding_box_diagonal;


	/// Lower corner of the bounding box
	VEC3 m_min_corner;


	/// Upper corner of the bounding box
	VEC3 m_max_corner;


	/// Array containing the normal vectors (one per triangle)
	std::vector<RATIONAL_NUMBER> m_p_face_normal_set;


	/// Array containing the normal vectors (one per vertex)
	std::vector<RATIONAL_NUMBER> m_p_vertex_normal_set;


	/// Array containing the vertices
	void* m_p_vertex_set;


	/// Array containing the index
	void* m_p_index_set;


	/// Number of indices
	unsigned int m_number_of_indices;


	/// Number of vertices
	unsigned int m_number_of_vertices;


	/// Data type of the index data; valid values are GL_UNSIGNED_INT, GL_UNSIGNED_SHORT, GL_UNSIGNED_BYTE and GL_NONE
	int m_index_data_type;


	/// Data type of the vertex data; valid values are GL_FLOAT, GL_DOUBLE and GL_NONE
	int m_vertex_data_type;


	/// Polygon type; valid values are GL_TRIANGLES
	int m_polygon_type;


	/// A flag set to true if the memory is managed outside of this class instance;
	/// set to false if the memory is managed by this class instance
	bool m_external_data_flag;


	/// Material of the object
	Material m_material;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "PolygonMesh.inl"


#endif // POLYGON_MESH_H
