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

#include <GL/glew.h>

#ifdef HAS_ITK
#include <itkImage.h>
#include <itkBinaryMask3DMeshSource.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkMesh.h>
#include <itkQuadEdgeMesh.h>
#endif

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
		SPHERE,				///< Electron density
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

	void voxelise(const Vec3ui& aNumberOfVoxels,
			const VEC3& aCenter,
			const VEC3& aVoxelSize);

	void voxelize(const Vec3ui& aNumberOfVoxels,
			const VEC3& aCenter,
			const VEC3& aVoxelSize);


	void writeVoxelData(const char* aFileName) const;
	void writeVoxelData(const std::string& aFileName) const;

	void extractIsoSurface(RATIONAL_NUMBER aThreshold = 0);

	const std::vector<GLfloat>& getVertexData() const;
	const std::vector<GLuint>& getIndexData() const;


//******************************************************************************
protected:
	RATIONAL_NUMBER evaluateSphere(RATIONAL_NUMBER r) const;
	RATIONAL_NUMBER evaluateBlobbyMolecule(RATIONAL_NUMBER r) const;
	RATIONAL_NUMBER evaluateMetaBall(RATIONAL_NUMBER r) const;
	RATIONAL_NUMBER evaluateSoftObject(RATIONAL_NUMBER r) const;

#ifdef HAS_ITK
	typedef RATIONAL_NUMBER InputVoxelDataType;
	typedef unsigned char OutputVoxelDataType;
	typedef itk::Image<InputVoxelDataType, 3> InputVolumeType;
	typedef itk::Image<OutputVoxelDataType, 3> OutputVolumeType;
	typedef itk::QuadEdgeMesh<GLfloat, 3> MeshType;

	typedef itk::BinaryThresholdImageFilter<InputVolumeType, OutputVolumeType> BinaryThresholdType;
	typedef itk::BinaryMask3DMeshSource<OutputVolumeType, MeshType> IsoSurfaceExtracterType;

	InputVolumeType::Pointer m_p_volume;
	BinaryThresholdType::Pointer m_p_threshold_filter;
	IsoSurfaceExtracterType::Pointer m_p_iso_surface_extracter;
#endif // HAS_ITK

	DensityFunctionType m_density_function;
	RATIONAL_NUMBER m_a;
	RATIONAL_NUMBER m_b;

	std::vector<VEC3> m_control_point_set;
	std::vector<std::pair<VEC3, VEC3> > m_control_line_set;

	std::vector<GLfloat> m_p_vertex_data;
	std::vector<GLuint> m_p_index_data;
};


} // namespace Graphics


//******************************************************************************
#include "ImplicitSurface.inl"


#endif // IMPLICIT_SURFACE_H
