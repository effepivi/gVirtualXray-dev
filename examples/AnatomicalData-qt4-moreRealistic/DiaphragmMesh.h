#ifndef DIAPHRAGM_MESH_H
#define DIAPHRAGM_MESH_H

/**
********************************************************************************
*
*	@file		DiaphragmMesh.h
*
*	@brief      Class to handle a polygon mesh for a diaphragm.
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

#ifndef SOFT_TISSUE_MESH_H
#include "SoftTissueMesh.h"
#endif

//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  DiaphragmMesh
*   @brief  DiaphragmMesh is a class to handle a polygon mesh for a diaphragm.
*/
//==============================================================================
class DiaphragmMesh: public SoftTissueMesh
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    //--------------------------------------------------------------------------
	DiaphragmMesh();

	//--------------------------------------------------------------------------
	/// Destructor
	//--------------------------------------------------------------------------
	virtual ~DiaphragmMesh();

protected:
	//--------------------------------------------------------------------------
	/// Link the attributes to their id in the shader bump map generation
	//-------------------------------------------------------------------------
	virtual void defineBumpMapShaderInput();

	//--------------------------------------------------------------------------
	/// Link the attributes to their id in the shader for colour texture generation
	//-------------------------------------------------------------------------
	virtual void defineColourShaderInput();

	Matrix4x4<GLfloat> m_model_matrix;
};

} // namespace gVirtualXRay

#endif // DIAPHRAGM_MESH_H
