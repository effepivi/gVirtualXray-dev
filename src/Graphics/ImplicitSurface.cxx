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

#include <itkImageRegionIterator.h>
#include <itkImageRegionConstIterator.h>
#include <itkImageFileWriter.h>
#include <itkMeshFileWriter.h>

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


//----------------------------------------------
ImplicitSurface::ImplicitSurface():
//----------------------------------------------
			m_density_function(BLOBBY_MOLECULE),
			m_a(1),
			m_b(1),
			m_p_volume(InputVolumeType::New()),
			m_p_iso_surface_extracter(IsoSurfaceExtracterType::New())
//----------------------------------------------
{
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
	VEC3 half_voxel_size(aVoxelSize.getX() / 2.0,
			aVoxelSize.getY() / 2.0,
			aVoxelSize.getZ() / 2.0);

	VEC3 half_volume_size(half_voxel_size.getX() * aNumberOfVoxels.getX(),
			half_voxel_size.getY() * aNumberOfVoxels.getY(),
			half_voxel_size.getZ() * aNumberOfVoxels.getZ());

	VEC3 offset(aCenter - half_volume_size + half_voxel_size);

	InputVolumeType::IndexType start_index;
	start_index[0] = 0;
	start_index[1] = 0;
	start_index[2] = 0;

	InputVolumeType::SizeType volume_size;
	volume_size[0] = aNumberOfVoxels.getX();
	volume_size[1] = aNumberOfVoxels.getY();
	volume_size[2] = aNumberOfVoxels.getZ();

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

	itk::ImageRegion<3> region(start_index, volume_size);
	m_p_volume->SetRegions(region);
	m_p_volume->Allocate();

	// Get the number of voxels
	/*int number_of_voxels(aNumberOfVoxels.getX() * aNumberOfVoxels.getY() * aNumberOfVoxels.getZ());

	// Resize the vector if needed
	if (m_p_voxel_data.capacity() < number_of_voxels)
	{
		m_p_voxel_data.resize(number_of_voxels);
	}
*/
	// Clear the voxel data
	m_p_volume->FillBuffer(0);



	VEC3 voxel_center;

	InputVolumeType::IndexType voxel_index;

	itk::ImageRegionIterator<InputVolumeType> voxel_ite(m_p_volume, m_p_volume->GetLargestPossibleRegion());
	voxel_ite.GoToBegin();
	for (int z(0); z < aNumberOfVoxels.getZ(); ++z)
	{
		voxel_center.setZ(offset.getZ() + z * aVoxelSize.getZ());

		for (int y(0); y < aNumberOfVoxels.getY(); ++y)
		{
			voxel_center.setY(offset.getY() + y * aVoxelSize.getY());

			for (int x(0); x < aNumberOfVoxels.getX(); ++x, ++voxel_ite)
			{
				RATIONAL_NUMBER voxel_value(0);

				voxel_center.setX(offset.getX() + x * aVoxelSize.getX());

				// Process all the points
				for (std::vector<VEC3>::const_iterator point_ite(m_control_point_set.begin());
						point_ite != m_control_point_set.end();
						++point_ite)
				{
					voxel_value += evaluate((voxel_center - *point_ite).length());
				}

				// Process all the lines
				for (std::vector<std::pair<VEC3, VEC3> >::const_iterator line_ite(m_control_line_set.begin());
						line_ite != m_control_line_set.end();
						++line_ite)
				{

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
}


//---------------------------------------------------------------
void ImplicitSurface::writeVoxelData(const char* aFileName) const
//---------------------------------------------------------------
{
	typedef itk::ImageFileWriter<InputVolumeType> FileWriterType;

	FileWriterType::Pointer p_file_writer(FileWriterType::New());
	p_file_writer->SetInput(m_p_volume);
	p_file_writer->SetFileName(aFileName);
	p_file_writer->Update();
}


//-----------------------------------------------------------------
void ImplicitSurface::extractIsoSurface(RATIONAL_NUMBER aThreshold)
//-----------------------------------------------------------------
{
	// Clear the data
	m_p_vertex_data.clear();
	m_p_index_data.clear();

	// Filter the data
	typedef itk::BinaryThresholdImageFilter<InputVolumeType, OutputVolumeType> BinaryThresholdType;
	BinaryThresholdType::Pointer p_threshold_filter(BinaryThresholdType::New());
	p_threshold_filter->SetInput(m_p_volume);
	p_threshold_filter->SetOutsideValue(0);
	p_threshold_filter->SetInsideValue(1);
	p_threshold_filter->SetLowerThreshold(aThreshold);
	p_threshold_filter->Update();

	// Extract the isosurface
	m_p_iso_surface_extracter->SetInput(p_threshold_filter->GetOutput());
	const OutputVoxelDataType object_value(static_cast<OutputVoxelDataType>(1));
	m_p_iso_surface_extracter->SetObjectValue(object_value);
	m_p_iso_surface_extracter->SetInput(p_threshold_filter->GetOutput());
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
/*
	typedef MeshType::PointsContainer::Iterator PointIterator;
	typedef MeshType::CellType CellType;
	typedef MeshType::PointType PointType;
	typedef MeshType::CellType::PointIdIterator PointIdIterator;


	m_p_vertex_data.clear();
	m_p_index_data.clear();

	m_p_iso_surface_extracter->SetObjectValue(aThreshold);
	m_p_iso_surface_extracter->Update();

	MeshType::Pointer p_mesh(m_p_iso_surface_extracter->GetOutput());

	std::cout << "Points:\t" << p_mesh->GetNumberOfPoints() << std::endl;
	std::cout << "Cells:\t" << p_mesh->GetNumberOfCells() << std::endl;

	CellType::CellAutoPointer triangle;
	for (unsigned int i(0); i < p_mesh->GetNumberOfCells(); ++i)
	{
		p_mesh->GetCell(i, triangle);

		for (PointIdIterator point_ite(triangle->PointIdsBegin());
				point_ite != triangle->PointIdsEnd();
				++point_ite)
		{
			PointType point(p_mesh->GetPoint(*point_ite));
			m_p_vertex_data.push_back(point[0]);
			m_p_vertex_data.push_back(point[1]);
			m_p_vertex_data.push_back(point[2]);
		}
	}
*/
/*
	for (PointIterator point_iterator(p_mesh->GetPoints()->Begin());
			point_iterator != p_mesh->GetPoints()->End();
			++point_iterator)
	{
		MeshType::PointType point(point_iterator.Value());

		int index(point_iterator->Index());
		m_p_vertex_data[index * 3 + 0] = point[0];
		m_p_vertex_data[index * 3 + 1] = point[1];
		m_p_vertex_data[index * 3 + 2] = point[2];
	}
*/
	typedef itk::MeshFileWriter< MeshType > WriterType;
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
	}
}

