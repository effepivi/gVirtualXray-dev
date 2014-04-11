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


#ifndef GVXR_IMPLICIT_SURFACE_H
#define GVXR_IMPLICIT_SURFACE_H


//******************************************************************************
//	Include
//******************************************************************************
#include <vector>

#ifdef HAS_ITK
#include <itkImage.h>
#include <itkBinaryMask3DMeshSource.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkMesh.h>
#include <itkQuadEdgeMesh.h>
#endif

#ifndef GVXR_TYPES_H
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
		SPHERE,				///< Sphere
		BLOBBY_MOLECULE,	///< Blobby molecule, first created by Jim Blinn and modelled after electron density fields
		META_BALL,			///< Meta balls
		SOFT_OBJECT			///< Soft Objects, first created by the Wyvill brothers
	} DensityFunctionType;


	//--------------------------------------------------------------------------
	///	Type of density function parameter.
	//--------------------------------------------------------------------------
	class DensityFunctionParameterType
	{
	public:
		//--------------------------------------------------------------------------
		///	Default constructor.
		/**
		*	@param aDensityFunctionType:	the type of density function (default value: BLOBBY_MOLECULE)
		*	@param a:	the first parameter (default value: 1)
		*	@param b:	the second parameter (default value: 1)
		*	@return the density
		*/
		//--------------------------------------------------------------------------
		DensityFunctionParameterType(const DensityFunctionType aDensityFunctionType = BLOBBY_MOLECULE,
				RATIONAL_NUMBER a = 1,
				RATIONAL_NUMBER b = 1):
			m_density_function(aDensityFunctionType),
			m_a(a),
			m_b(b)
		{}


        /// Type of density function
		DensityFunctionType m_density_function;
    
    
        /// First parameter of the density function
		RATIONAL_NUMBER m_a;
    
    
        /// Secnd parameter of the density function
        /// (not used if m_density_function is SPHERE)
		RATIONAL_NUMBER m_b;
	};


	//--------------------------------------------------------------------------
	///	Default constructor.
	//--------------------------------------------------------------------------
	ImplicitSurface();


	//--------------------------------------------------------------------------
	///	Destructor.
	//--------------------------------------------------------------------------
	~ImplicitSurface();


	//--------------------------------------------------------------------------
	///	Evaluate the density function for the distance r.
	/**
	*	@param r:	the distance
	*	@param aParameter:	the parameter of the density function
	*	@return the density
	*/
	//--------------------------------------------------------------------------
	RATIONAL_NUMBER evaluate(RATIONAL_NUMBER r,
			const DensityFunctionParameterType& aParameter) const;


	//--------------------------------------------------------------------------
	//	Use the density function that creates spheres.
	//--------------------------------------------------------------------------
	//void setSphere();


	//--------------------------------------------------------------------------
	//	Use the density function that creates Blobby Molecules.
	//	It was first created by Jim Blinn.
	//	It is modelled after electron density fields.
	/*
	*	@param a:	the height
	*	@param b:	the standard deviation of the curve
	*/
	//--------------------------------------------------------------------------
	//void setBlobbyMolecule(RATIONAL_NUMBER a, RATIONAL_NUMBER b);


	//--------------------------------------------------------------------------
	//	Use the density function that creates Meta Balls.
	/*
	*	@param a:	the scaling factor
	*	@param b:	the maximum distance a control primitive contributes to the field
	*/
	//--------------------------------------------------------------------------
	//void setMetaBall(RATIONAL_NUMBER a, RATIONAL_NUMBER b);


	//--------------------------------------------------------------------------
	//	Use the density function that creates Soft Objects.
	//	It was first created by the Wyvill brothers.
	/*
	*	@param a:	the scaling factor
	*	@param b:	the maximum distance a control primitive contributes to the field
	*/
	//--------------------------------------------------------------------------
	//void setSoftObject(RATIONAL_NUMBER a, RATIONAL_NUMBER b);


	//--------------------------------------------------------------------------
	///	Remove all the control points.
	//--------------------------------------------------------------------------
	void resetControlPoints();


	//--------------------------------------------------------------------------
	///	Remove all the control lines.
	//--------------------------------------------------------------------------
	void resetControlLines();


	//--------------------------------------------------------------------------
	///	Add a control point.
	/**
	*	@param aParameter:	the parameter of the density function
	*	@param aControlPoint:	the control point to add
	*/
	//--------------------------------------------------------------------------
	void addControlPoint(const DensityFunctionParameterType& aParameter,
			const VEC3& aControlPoint);


	//--------------------------------------------------------------------------
	///	Add a control line.
	/**
	*	@param aParameter:	the parameter of the density function
	*	@param aControlPoint1:	the first point that defines the line segment
	*	@param aControlPoint2:	the second point that defines the line segment
	*/
	//--------------------------------------------------------------------------
	void addControlLine(const DensityFunctionParameterType& aParameter,
			const VEC3& aControlPoint1,
			const VEC3& aControlPoint2);


	//--------------------------------------------------------------------------
	///	Accessor on a given control point.
	/**
	*	@param anIndex:	the index of the control point
	*	@return the corresponding control point
	*/
	//--------------------------------------------------------------------------
	const VEC3& getControlPoint(unsigned int anIndex) const;


	//--------------------------------------------------------------------------
	///	Accessor on a given control point.
	/**
	*	@param anIndex:	the index of the control point
	*	@return the corresponding control point
	*/
	//--------------------------------------------------------------------------
	VEC3& getControlPoint(unsigned int anIndex);


	//--------------------------------------------------------------------------
	///	Accessor on a given control line.
	/**
	*	@param anIndex:	the index of the control point
	*	@return the corresponding control line segment
	*/
	//--------------------------------------------------------------------------
	const std::pair<VEC3, VEC3>& getControlLine(unsigned int anIndex) const;


	//--------------------------------------------------------------------------
	///	Accessor on a given control line.
	/**
	*	@param anIndex:	the index of the control point
	*	@return the corresponding control line segment
	*/
	//--------------------------------------------------------------------------
	std::pair<VEC3, VEC3>& getControlLine(unsigned int anIndex);


	//--------------------------------------------------------------------------
	///	Accessor on a given control line.
	/**
	*	@param aNumberOfVoxels:	the number of voxels
	*	@param aCenter:	the 3D position of the centre of the dataset
	*	@param aVoxelSize:	the size of voxels
	*/
	//--------------------------------------------------------------------------
	void voxelise(const Vec3ui& aNumberOfVoxels,
			const VEC3& aCenter,
			const VEC3& aVoxelSize);


	//--------------------------------------------------------------------------
	///	Accessor on a given control line.
	/**
	*	@param aNumberOfVoxels:	the number of voxels
	*	@param aCenter:	the 3D position of the centre of the dataset
	*	@param aVoxelSize:	the size of voxels
	*/
	//--------------------------------------------------------------------------
	void voxelize(const Vec3ui& aNumberOfVoxels,
			const VEC3& aCenter,
			const VEC3& aVoxelSize);


	//--------------------------------------------------------------------------
	///	Write the current voxel data into a file.
	/**
	*	@param aFileName:	the name of the file where to save the voxel data
	*/
	//--------------------------------------------------------------------------
	void writeVoxelData(const char* aFileName) const;


	//--------------------------------------------------------------------------
	///	Write the current voxel data into a file.
	/**
	*	@param aFileName:	the name of the file where to save the voxel data
	*/
	//--------------------------------------------------------------------------
	void writeVoxelData(const std::string& aFileName) const;


	//--------------------------------------------------------------------------
	///	Write the polygon data.
	/**
	*	@param aFileName:	the name of the file where to save the polygon data
	*/
	//--------------------------------------------------------------------------
	void writePolygonMesh(const char* aFileName) const;


	//--------------------------------------------------------------------------
	///	Write the polygon data.
	/**
	*	@param aFileName:	the name of the file where to save the polygon data
	*/
	//--------------------------------------------------------------------------
	void writePolygonMesh(const std::string& aFileName) const;


	//--------------------------------------------------------------------------
	///	Extract the surface from the voxel data corresponding to a given iso-value.
	/**
	*	@param aThreshold:	the threshold value
	*/
	//--------------------------------------------------------------------------
	void extractIsoSurface(const RATIONAL_NUMBER& aThreshold = 0);


	//--------------------------------------------------------------------------
	///	Accessor on the vertex data.
	/**
	*	@return the vertex data
	*/
	//--------------------------------------------------------------------------
	const std::vector<RATIONAL_NUMBER>& getVertexData() const;


	//--------------------------------------------------------------------------
	///	Accessor on the index data.
	/**
	*	@return the index data
	*/
	//--------------------------------------------------------------------------
	const std::vector<unsigned int>& getIndexData() const;


//******************************************************************************
protected:
	RATIONAL_NUMBER evaluateSphere(RATIONAL_NUMBER r) const;
	RATIONAL_NUMBER evaluateBlobbyMolecule(RATIONAL_NUMBER r, RATIONAL_NUMBER a, RATIONAL_NUMBER b) const;
	RATIONAL_NUMBER evaluateMetaBall(RATIONAL_NUMBER r, RATIONAL_NUMBER a, RATIONAL_NUMBER b) const;
	RATIONAL_NUMBER evaluateSoftObject(RATIONAL_NUMBER r, RATIONAL_NUMBER a, RATIONAL_NUMBER b) const;

#ifdef HAS_ITK
	typedef RATIONAL_NUMBER InputVoxelDataType;
	typedef unsigned char OutputVoxelDataType;
	typedef itk::Image<InputVoxelDataType, 3> InputVolumeType;
	typedef itk::Image<OutputVoxelDataType, 3> OutputVolumeType;
	typedef itk::QuadEdgeMesh<float, 3> MeshType;

	typedef itk::BinaryThresholdImageFilter<InputVolumeType, OutputVolumeType> BinaryThresholdType;
	typedef itk::BinaryMask3DMeshSource<OutputVolumeType, MeshType> IsoSurfaceExtracterType;

	InputVolumeType::Pointer m_p_volume;
	BinaryThresholdType::Pointer m_p_threshold_filter;
	IsoSurfaceExtracterType::Pointer m_p_iso_surface_extracter;
#endif // HAS_ITK

	std::vector<std::pair<DensityFunctionParameterType, VEC3> > m_control_point_set;
	std::vector<std::pair<DensityFunctionParameterType, std::pair<VEC3, VEC3> > > m_control_line_set;

	std::vector<RATIONAL_NUMBER> m_p_vertex_data;
	std::vector<unsigned int> m_p_index_data;
};


} // namespace Graphics


//******************************************************************************
#include "ImplicitSurface.inl"


#endif // IMPLICIT_SURFACE_H
