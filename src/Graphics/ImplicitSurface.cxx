/**
********************************************************************************
*
*	@file		ImplicitSurface.cxx
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

//******************************************************************************
//	Include
//******************************************************************************
#include <sstream>

#ifdef HAS_ITK
#include <itkImageRegionIterator.h>
#include <itkImageRegionConstIterator.h>
#include <itkImageFileWriter.h>
#include <itkMeshFileWriter.h>
#endif

#ifndef IMPLICIT_SURFACE_H
#include "gVirtualXRay/ImplicitSurface.h"
#endif

#ifndef EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace Graphics;


//******************************************************************************
//	Constant variables
//******************************************************************************


//******************************************************************************
//	Global variables
//******************************************************************************


//******************************************************************************
//	Function declaration
//******************************************************************************


//-----------------------------------------------------------------
ImplicitSurface::ImplicitSurface():
//-----------------------------------------------------------------
#ifdef HAS_ITK
			m_p_volume(InputVolumeType::New()),
			m_p_threshold_filter(BinaryThresholdType::New()),
			m_p_iso_surface_extracter(IsoSurfaceExtracterType::New()),
#endif
			m_density_function(BLOBBY_MOLECULE),
			m_a(1),
			m_b(1)
//-----------------------------------------------------------------
{
#ifdef HAS_ITK
	// Initialise the threshold filter
	m_p_threshold_filter->SetInput(m_p_volume);
	m_p_threshold_filter->SetOutsideValue(0);
	m_p_threshold_filter->SetInsideValue(1);

	// Initialise the isosurface extracter
	const OutputVoxelDataType object_value(static_cast<OutputVoxelDataType>(1));
	m_p_iso_surface_extracter->SetObjectValue(object_value);
	m_p_iso_surface_extracter->SetInput(m_p_threshold_filter->GetOutput());
#endif
}


//---------------------------------
ImplicitSurface::~ImplicitSurface()
//---------------------------------
{
}


//-----------------------------------------------------------
void ImplicitSurface::voxelise(const Vec3ui& aNumberOfVoxels,
							   const VEC3& aCenter,
							   const VEC3& aVoxelSize)
//-----------------------------------------------------------
{
#ifdef HAS_ITK
	VEC3 half_voxel_size(aVoxelSize.getX() / 2.0,
			aVoxelSize.getY() / 2.0,
			aVoxelSize.getZ() / 2.0);

	VEC3 half_volume_size(half_voxel_size.getX() * aNumberOfVoxels.getX(),
			half_voxel_size.getY() * aNumberOfVoxels.getY(),
			half_voxel_size.getZ() * aNumberOfVoxels.getZ());

	VEC3 offset(aCenter - half_volume_size + half_voxel_size);

	// Get the current size of the volume
	unsigned int previous_width(0);
	unsigned int previous_height(0);
	unsigned int previous_depth(0);

	if (m_p_volume.IsNotNull())
	{
		previous_width  = m_p_volume->GetBufferedRegion().GetSize()[0];
		previous_height = m_p_volume->GetBufferedRegion().GetSize()[1];
		previous_depth  = m_p_volume->GetBufferedRegion().GetSize()[2];
	}

	// The size has changed
	if (previous_width != aNumberOfVoxels.getX() ||
			previous_height != aNumberOfVoxels.getY() ||
			previous_depth != aNumberOfVoxels.getZ())
	{
		// Change the size of volume
		InputVolumeType::IndexType start_index;
		start_index[0] = 0;
		start_index[1] = 0;
		start_index[2] = 0;

		InputVolumeType::SizeType volume_size;
		volume_size[0] = aNumberOfVoxels.getX();
		volume_size[1] = aNumberOfVoxels.getY();
		volume_size[2] = aNumberOfVoxels.getZ();

		itk::ImageRegion<3> region(start_index, volume_size);
		m_p_volume->SetRegions(region);
		m_p_volume->Allocate();
	}

	double spacing[3];
	spacing[0] = aVoxelSize.getX();
	spacing[1] = aVoxelSize.getY();
	spacing[2] = aVoxelSize.getZ();
	m_p_volume->SetSpacing(spacing);

	double origin[3];
	origin[0] = offset.getX();
	origin[1] = offset.getY();
	origin[2] = offset.getZ();
	m_p_volume->SetOrigin(origin);

	VEC3 voxel_center;

	InputVolumeType::IndexType voxel_index;

	RATIONAL_NUMBER r(0);

	VEC3 point_1;
	VEC3 point_2;

	VEC3 v1;
	VEC3 v2;

	itk::ImageRegionIterator<InputVolumeType> voxel_ite(m_p_volume, m_p_volume->GetLargestPossibleRegion());
	voxel_ite.GoToBegin();
	for (unsigned int z(0); z < aNumberOfVoxels.getZ(); ++z)
	{
		voxel_center.setZ(offset.getZ() + z * aVoxelSize.getZ());

		for (unsigned int y(0); y < aNumberOfVoxels.getY(); ++y)
		{
			voxel_center.setY(offset.getY() + y * aVoxelSize.getY());

			for (unsigned int x(0); x < aNumberOfVoxels.getX(); ++x, ++voxel_ite)
			{
				RATIONAL_NUMBER voxel_value(0);

				voxel_center.setX(offset.getX() + x * aVoxelSize.getX());

				// Process all the points
				for (std::vector<VEC3>::const_iterator point_ite(m_control_point_set.begin());
						point_ite != m_control_point_set.end();
						++point_ite)
				{
					r = (voxel_center - *point_ite).length();
					voxel_value += evaluate(r);
				}

				// Process all the lines
				for (std::vector<std::pair<VEC3, VEC3> >::const_iterator line_ite(m_control_line_set.begin());
						line_ite != m_control_line_set.end();
						++line_ite)
				{
					point_1 = line_ite->first;
					point_2 = line_ite->second;

					const RATIONAL_NUMBER length_squared(pow((point_1 - point_2).length(), 2.0));
					if (fabs(length_squared) < EPSILON)
					{
						r = (voxel_center - point_2).length();
					}
					else
					{
						const RATIONAL_NUMBER normalised_dot_product(((voxel_center - point_1).dotProduct(point_2 - point_1)) / length_squared);
						if (normalised_dot_product < 0.0)
						{
							r = (voxel_center - point_1).length();
						}
						else if (normalised_dot_product > 1.0)
						{
							r = (voxel_center - point_2).length();
						}
						else
						{
							const VEC3 projection(point_1 + normalised_dot_product * (point_2 - point_1));
							r = (voxel_center - projection).length();
						}
					}

					voxel_value += evaluate(r);
				}

				itk::Point<double, 3> voxel_position;
				voxel_position[0] = voxel_center.getX();
				voxel_position[1] = voxel_center.getY();
				voxel_position[2] = voxel_center.getZ();
				m_p_volume->TransformPhysicalPointToIndex(voxel_position, voxel_index);
				m_p_volume->SetPixel(voxel_index, voxel_value);
			}
		}
	}
#else // HAS_ITK
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented, you have to enable ITK using CMake.");
#endif // HAS_ITK
}


//---------------------------------------------------------------
void ImplicitSurface::writeVoxelData(const char* aFileName) const
//---------------------------------------------------------------
{
#ifdef HAS_ITK
	typedef itk::ImageFileWriter<InputVolumeType> FileWriterType;

	FileWriterType::Pointer p_file_writer(FileWriterType::New());
	p_file_writer->SetInput(m_p_volume);
	p_file_writer->SetFileName(aFileName);
	p_file_writer->Update();
#else // HAS_ITK
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented, you have to enable ITK using CMake.");
#endif // HAS_ITK
}


//-----------------------------------------------------------------
void ImplicitSurface::extractIsoSurface(RATIONAL_NUMBER aThreshold)
//-----------------------------------------------------------------
{
#ifdef HAS_ITK
	// Clear the data
	m_p_vertex_data.clear();
	m_p_index_data.clear();

	// Filter the data
	m_p_threshold_filter->SetLowerThreshold(aThreshold);
	m_p_threshold_filter->Update();

	// Extract the isosurface
	m_p_iso_surface_extracter->Update();

	// Get the polygon mesh
	MeshType::Pointer p_mesh(m_p_iso_surface_extracter->GetOutput());

	// There are triangles
	if (p_mesh->GetNumberOfCells() && p_mesh->GetNumberOfPoints())
	{
		// Decimate the mesh by 75%
/*		typedef itk::NumberOfFacesCriterion<MeshType> CriterionType;
		CriterionType::Pointer p_criterion(CriterionType::New());
		p_criterion->SetTopologicalChange(false);
		p_criterion->SetNumberOfElements(meshSource->GetNumberOfCells() * 0.25);

		typedef itk::QuadEdgeMeshSquaredEdgeLengthDecimation<MeshType, MeshType, CriterionType> DecimationType;
		DecimationType::Pointer p_decimate = DecimationType::New();
		p_decimate->SetInput(meshSource->GetOutput());
		p_decimate->SetCriterion(p_criterion);
	//	p_decimate->SetRelocate(true);
		p_decimate->Update();

		// Get the polygon mesh
		p_mesh = p_decimate->GetOutput();
*/

		// The mesh is not empty
		if (p_mesh->GetNumberOfCells() && p_mesh->GetNumberOfPoints())
		{
			m_p_vertex_data.resize(p_mesh->GetNumberOfPoints() * 3, 0);
			m_p_index_data.resize(p_mesh->GetNumberOfCells() * 3, 0);

			MeshType::PointsContainer* p_itk_point_set = p_mesh->GetPoints();
			MeshType::CellsContainer* p_itk_cell_set = p_mesh->GetCells();

			// Copy points
			for(MeshType::PointsContainer::Iterator ite(p_itk_point_set->Begin());
				ite != p_itk_point_set->End();
				++ite)
			{
				// Get the point index from the point container iterator
				int idx = ite.Index();

				// Get the point
				MeshType::PointType p = ite.Value(); // access the point

				// Copy the point
				m_p_vertex_data[idx * 3 + 0] = p[0];
				m_p_vertex_data[idx * 3 + 1] = p[1];
				m_p_vertex_data[idx * 3 + 2] = p[2];
			}

			typedef MeshType::CellType	CellType;
			typedef itk::TriangleCell< CellType >	TriangleType;
			typedef CellType::PointIdIterator	PointIdIterator;

			for(MeshType::CellsContainer::Iterator ite(p_itk_cell_set->Begin());
				ite != p_itk_cell_set->End();
				++ite)
			{
				// Get the cell index from the point container iterator
				int idx = ite.Index();

				// Get the cell
				CellType* p_cell = ite.Value();

				// Copy point indices
				int i(0);
				for (PointIdIterator ite_point_idx(p_cell->PointIdsBegin());
					ite_point_idx != p_cell->PointIdsEnd();
					++ite_point_idx, ++i)
				{
					unsigned int point_id = *ite_point_idx;
					m_p_index_data[idx * 3 + i] = point_id;
				}
			}
		}
	}

	/*typedef itk::MeshFileWriter< MeshType > WriterType;
	WriterType::Pointer writer = WriterType::New();
	writer->SetFileTypeAsBINARY ();
	writer->SetFileName("mesh.obj");
	writer->SetInput(p_mesh);
	try
	{
		writer->Update();
	}
	catch( itk::ExceptionObject & error )
	{
		std::cerr << "Error: " << error << std::endl;
		exit(EXIT_FAILURE);
	}*/
#else // HAS_ITK
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented, you have to enable ITK using CMake.");
#endif // HAS_ITK
}

