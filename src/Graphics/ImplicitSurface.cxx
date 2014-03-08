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
			m_b(1)
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
	// Get the number of voxels
	int number_of_voxels(aNumberOfVoxels.getX() * aNumberOfVoxels.getY() * aNumberOfVoxels.getZ());

	// Resize the vector if needed
	if (m_p_voxel_data.capacity() < number_of_voxels)
	{
		m_p_voxel_data.resize(number_of_voxels);
	}

	// Clear the voxel data
	m_p_voxel_data.clear();

	// Store states
	m_volume_size = aNumberOfVoxels;
	m_center = aCenter;
	m_voxel_size = aVoxelSize;

	VEC3 half_voxel_size(m_voxel_size.getX() / 2.0,
			m_voxel_size.getY() / 2.0,
			m_voxel_size.getZ() / 2.0);

	VEC3 half_volume_size(half_voxel_size.getX() * m_volume_size.getX(),
			half_voxel_size.getY() * m_volume_size.getY(),
			half_voxel_size.getZ() * m_volume_size.getZ());

	VEC3 offset(m_center - half_volume_size + half_voxel_size);

	VEC3 voxel_center;

	std::vector<RATIONAL_NUMBER>::iterator voxel_ite(m_p_voxel_data.begin());
	for (int z(0); z < m_volume_size.getZ(); ++z)
	{
		voxel_center.setZ(offset.getZ() + z * m_voxel_size.getZ());

		for (int y(0); y < m_volume_size.getY(); ++y)
		{
			voxel_center.setY(offset.getY() + y * m_voxel_size.getY());

			for (int x(0); x < m_volume_size.getX(); ++x, ++voxel_ite)
			{
				voxel_center.setX(offset.getX() + x * m_voxel_size.getX());

				RATIONAL_NUMBER voxel_value(0);

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

				*voxel_ite = voxel_value;
			}
		}
	}
}


//---------------------------------------------------------------
void ImplicitSurface::writeVoxelData(const char* aFileName) const
//---------------------------------------------------------------
{
	// Open the file
	FILE* p_file_descriptor(fopen(aFileName, "wb"));

	// The file is not open
	if (!p_file_descriptor)
	{
		std::stringstream error_message;
		error_message << "Cannot create the file \"" << aFileName << ".\"";
		throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str().data());
	}

	std::vector<RATIONAL_NUMBER>::const_iterator voxel_ite(m_p_voxel_data.begin());
	for (int z(0); z < m_volume_size.getZ(); ++z)
	{
		for (int y(0); y < m_volume_size.getY(); ++y, voxel_ite += m_volume_size.getX())
		{
			fwrite(&(*voxel_ite), m_volume_size.getX(), sizeof(RATIONAL_NUMBER), p_file_descriptor);
		}
	}

	// Close the file
	fclose(p_file_descriptor);
}
