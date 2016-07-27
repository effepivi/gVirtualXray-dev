#ifndef INTERNALORGANMESH_H
#define INTERNALORGANMESH_H

/**
********************************************************************************
*
*	@file		InternalOrganMesh.h
*
*	@brief      Class to handle a polygon mesh for an internal organ.
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
#include <AnatomicalMesh.h>

//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {

//==============================================================================
/**
*   @class  InternalOrganMesh
*   @brief  InternalOrganMesh is a class to handle a polygon mesh for an internal organ.
*/
//==============================================================================
class InternalOrganMesh: public AnatomicalMesh {
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    //--------------------------------------------------------------------------
	InternalOrganMesh();

	//--------------------------------------------------------------------------
	/// Destructor
	//--------------------------------------------------------------------------
	virtual ~InternalOrganMesh();

	//--------------------------------------------------------------------------
	/// return true if the mesh has to be displayed with transparency
	//--------------------------------------------------------------------------
	virtual bool isTransparent() const;

	//--------------------------------------------------------------------------
	/// return true if the mesh represent a soft tissue
	//--------------------------------------------------------------------------
	virtual bool isSoftTissue() const = 0;

	//--------------------------------------------------------------------------
	/// Link the attributes to their id in the shader
    /*
    *   @param aShaderID: The GLSL shader used to display the anatomical mesh
    */
	//-------------------------------------------------------------------------
	virtual void defineShaderInput(GLint aShaderID);
};

} // namespace gVirtualXRay

#endif // INTERNALORGANMESH_H
