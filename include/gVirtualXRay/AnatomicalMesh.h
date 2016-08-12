#ifndef ANATOMICAL_MESH_H
#define ANATOMICAL_MESH_H


/**
********************************************************************************
*
*	@file		AnatomicalMesh.h
*
*	@brief      Class to handle a polygon mesh for tissue.
*
*	@version	1.0
*
*	@date		18/07/2016
*
*	@author		Andréas Meuleman
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************

#ifndef GVXR_POLYGON_MESH_H
#include "gVirtualXRay/PolygonMesh.h"
#endif

#ifndef GVXR_MATRIX4X4_H
#include "gVirtualXRay/Matrix4x4.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay{


//==============================================================================
/**
*   @class  AnatomicalMesh
*   @brief  AnatomicalMesh is a class to handle a polygon mesh for tissue.
*/
//==============================================================================
class AnatomicalMesh: public PolygonMesh
//------------------------------------------------------------------------------
{
//******************************************************************************
public:

    //--------------------------------------------------------------------------
    /// Default Constructor
    //--------------------------------------------------------------------------
	AnatomicalMesh();

	//--------------------------------------------------------------------------
	/// Destructor
	//--------------------------------------------------------------------------
	virtual ~AnatomicalMesh();

	//--------------------------------------------------------------------------
	/// return true if the mesh has to be displayed with transparency
	//--------------------------------------------------------------------------
	virtual bool isTransparent() const = 0;

	//--------------------------------------------------------------------------
	/// return true if the mesh represent a soft tissue
	//--------------------------------------------------------------------------
	virtual bool isSoftTissue() const = 0;

	//--------------------------------------------------------------------------
	/// Link the attributes to their id in the shader
    /*
    *   @param shader_id: The GLSL shader used to display the anatomical mesh
    */
	//-------------------------------------------------------------------------
	virtual void defineDisplayShaderInput(GLint shader_id);

	//--------------------------------------------------------------------------
	/// initialize the needful depending on the organ
	//--------------------------------------------------------------------------
	virtual void initialize() = 0;


//******************************************************************************
protected:
	/// 1 if the mesh use texturing; default value is 0
	int m_use_texture;

};
} //namespace gVirtualXRay
#endif //ANATOMICAL_MESH_H
