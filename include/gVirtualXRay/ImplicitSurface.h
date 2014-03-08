/**
********************************************************************************
*
*	@file		ImplicitSurface.h
*
*	@brief		Class to build 3D implicit surfaces. To know what implicit
*				surfaces are, please visit Paul Bourke's tutorial available at
*				http://paulbourke.net/geometry/implicitsurf/
*
*	@version	1.0
*
*	@date		08/03/2014
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


#ifndef IMPLICIT_SURFACE_H
#define IMPLICIT_SURFACE_H


//******************************************************************************
//	Include
//******************************************************************************
#include <vector>

#ifndef TYPES_H
#include "gVirtualXRay/Types.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace Graphics {
	using namespace Utilities;


//==============================================================================
/**
*	@class	ImplicitSurface
*	@brief	ImplicitSurface is a class to build 3D implicit surfaces.
*/
//==============================================================================
class ImplicitSurface
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	//--------------------------------------------------------------------------
	///	Type of density function.
	//--------------------------------------------------------------------------
	typedef enum DensityFunctionTypeTag
	{
		SPHERE,
		BLOBBY_MOLECULE,	///< Blobby molecule, first created by Jim Blinn and modelled after electron density fields
		META_BALL,			///< Meta balls
		SOFT_OBJECT			///< Soft Objects, first created by the Wyvill brothers
	} DensityFunctionType;


	//--------------------------------------------------------------------------
	///	Default constructor.
	//--------------------------------------------------------------------------
	ImplicitSurface();


	//--------------------------------------------------------------------------
	///	Destructor.
	//--------------------------------------------------------------------------
	~ImplicitSurface();

	

	RATIONAL_NUMBER evaluate(RATIONAL_NUMBER r) const;


	void setSphere();


	//--------------------------------------------------------------------------
	///	Use the density function that creates Blobby Molecules.
	///	It was first created by Jim Blinn.
	///	It is modelled after electron density fields.
	/**
	*	@param a:	the height
	*	@param b:	the standard deviation of the curve
	*/
	//--------------------------------------------------------------------------
	void setBlobbyMolecule(RATIONAL_NUMBER a, RATIONAL_NUMBER b);


	//--------------------------------------------------------------------------
	///	Use the density function that creates Meta Balls.
	/**
	*	@param a:	the scaling factor
	*	@param b:	the maximum distance a control primitive contributes to the field
	*/
	//--------------------------------------------------------------------------
	void setMetaBall(RATIONAL_NUMBER a, RATIONAL_NUMBER b);


	//--------------------------------------------------------------------------
	///	Use the density function that creates Soft Objects.
	///	It was first created by the Wyvill brothers.
	/**
	*	@param a:	the scaling factor
	*	@param b:	the maximum distance a control primitive contributes to the field
	*/
	//--------------------------------------------------------------------------
	void setSoftObject(RATIONAL_NUMBER a, RATIONAL_NUMBER b);


	void resetControlPoints();


	void resetControlLines();


	void addControlPoint(const VEC3& aControlPoint);


	void addControlLine(const VEC3& aControlPoint1, const VEC3& aControlPoint2);


	const VEC3& getControlPoint(unsigned int anIndex) const;
	VEC3& getControlPoint(unsigned int anIndex);


	const std::pair<VEC3, VEC3>& getControlLine(unsigned int anIndex) const;
	std::pair<VEC3, VEC3>& getControlLine(unsigned int anIndex);


//******************************************************************************
protected:
	RATIONAL_NUMBER evaluateSphere(RATIONAL_NUMBER r) const;
	RATIONAL_NUMBER evaluateBlobbyMolecule(RATIONAL_NUMBER r) const;
	RATIONAL_NUMBER evaluateMetaBall(RATIONAL_NUMBER r) const;
	RATIONAL_NUMBER evaluateSoftObject(RATIONAL_NUMBER r) const;

	DensityFunctionType m_density_function;
	RATIONAL_NUMBER m_a;
	RATIONAL_NUMBER m_b;

	std::vector<VEC3> m_control_point_set;
	std::vector<std::pair<VEC3, VEC3> > m_control_line_set;
};


} // namespace Graphics


//******************************************************************************
#include "ImplicitSurface.inl"


#endif // IMPLICIT_SURFACE_H
