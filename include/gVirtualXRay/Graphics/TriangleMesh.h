/**
********************************************************************************
*
*	@file		TriangleMesh.h
*
*	@brief		Class to handle 3D triangle meshes.
*
*	@version	1.0
*
*	@date		07/11/2013
*
*	@author		Dr Franck P. Vidal
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


#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H


//******************************************************************************
//	Include
//******************************************************************************
#include <string>
#include <vector>

#include<GL/glew.h>
#ifdef __APPLE__
#include<OpenGL/gl.h>
#else
#include<GL/gl.h>
#endif

#ifndef VBO_H
#include "gVirtualXRay/Graphics/VBO.h"
#endif

#ifndef TYPES_H
#include "gVirtualXRay/Utilities/Types.h"
#endif

#ifndef UNITS_H
#include "gVirtualXRay/Utilities/Units.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace Graphics {
	using namespace Utilities;


//==============================================================================
/**
*	@class	TriangleMesh
*	@brief	TriangleMesh is a class to handle a 3D triangle mesh.
*/
//==============================================================================
class TriangleMesh
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	//--------------------------------------------------------------------------
	///	Default constructor.
	//--------------------------------------------------------------------------
	TriangleMesh();


	//--------------------------------------------------------------------------
	///	Destructor.
	//--------------------------------------------------------------------------
	~TriangleMesh();


	//--------------------------------------------------------------------------
	///	Reset the data.
	//--------------------------------------------------------------------------
	void reset();


	//--------------------------------------------------------------------------
	///	Display the triangular mesh using OpenGL.
	/**
	*	@param aVBOFlag:	true to use a vertex buffer object,
	*						false to use Immediate mode rendering
	*						(default value is TRUE)
	*/
	//--------------------------------------------------------------------------
	void display(bool aVBOFlag = true) const;


	void displayWireFrame() const;


	void displayFaceNormal(GLfloat aNormalSize = 0.5 * cm) const;


	void displayVertexNormal(GLfloat aNormalSize = 0.5 * cm) const;


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
	///	Accessor on the name of the file that contains the polygon mesh.
	/**
	*	@return the file name
	*/
	//--------------------------------------------------------------------------
	const std::string& getFilename() const;


	//--------------------------------------------------------------------------
	///	Set the name of the file that contains the polygon mesh.
	/**
	*	@param aMoveToCentreFlag:	a flag to move the polygon to the center or not
	*	@param anAutoComputeNormalFlag:	a flag to compute normals or nor
	*	@param aPrintDebugInfoFlag:	a flag to display additional debug information in the console or not
	*	@param aCreateVBOFlag:	a flag to use a vertex buffer object or not
	*	@param aScale:	the unit in which data is stored in the file (e.g. mm, cm, m)
	*/
	//--------------------------------------------------------------------------
	void loadSTLFile(bool aMoveToCentreFlag,
		bool anAutoComputeNormalFlag,
		bool aPrintDebugInfoFlag,
		bool aCreateVBOFlag,
		RATIONAL_NUMBER aScale);


	//--------------------------------------------------------------------------
	///	Move the polygon to the center.
	//--------------------------------------------------------------------------
	void moveToCentre();


	//--------------------------------------------------------------------------
	///	Compute the normal vectors using the cross product method.
	//--------------------------------------------------------------------------
	void autoComputeNormal();


	//--------------------------------------------------------------------------
	///	Use a vertex buffer object if possible.
	/**
	*	@return true if the VBO can be used, false otherwise
	*/
	//--------------------------------------------------------------------------
	bool useVBO();


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
	///	Accessor on the number of triangles.
	/**
	*	@return the number of triangles
	*/
	//--------------------------------------------------------------------------
	unsigned int getTriangleNumber() const;


	//--------------------------------------------------------------------------
	///	Accessor on the number of normal vectors.
	/**
	*	@return the number of normal vectors
	*/
	//--------------------------------------------------------------------------
	unsigned int getFaceNormalNumber() const;
	unsigned int getVertexNormalNumber() const;


	//--------------------------------------------------------------------------
	///	Accessor on the number of vertices.
	/**
	*	@return the number of vertices
	*/
	//--------------------------------------------------------------------------
	unsigned int getVertexNumber() const;
	unsigned int getIndexNumber() const;


	void mergeVertices();


	VEC3 getVertex(int anIndex) const;
	void setVertex(int anIndex, const VEC3& aVertex);


	int getIndex(int anIndex) const;


	VEC3 getFaceNormal(int anIndex) const;


	VEC3 getVertexNormal(int anIndex) const;

	void setExternalData(const std::vector<GLfloat>* aVertexArray,
			const std::vector<int>* anIndexArray);


	void applyScale(RATIONAL_NUMBER aScale);


	void displayUsingImmediateModeWithIndex() const;
	void displayUsingImmediateModeWithoutIndex() const;
	void displayWireFrameWithIndex() const;
	void displayWireFrameWithoutIndex() const;

//******************************************************************************
protected:
	void updateVBO();
	void computeBoundingBox();
	void normaliseNormals();


	/// Name of the file that contains the polygon mesh
	std::string m_filename;


	/// Vertex buffer object
	VBO m_vbo;


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
	std::vector<GLfloat> m_p_face_normal_set;


	/// Array containing the normal vectors (one per vertex)
	std::vector<GLfloat> m_p_vertex_normal_set;


	/// Array containing the vertices
	std::vector<GLfloat> m_p_internal_vertex_set;


	/// Array containing the index
	std::vector<GLuint> m_p_internal_index_set;


	std::vector<int>* m_p_external_index_set;
	std::vector<GLfloat>* m_p_external_vertex_set;
};


} // namespace Graphics


//******************************************************************************
#include "TriangleMesh.inl"


#endif // TRIANGLE_MESH_H
