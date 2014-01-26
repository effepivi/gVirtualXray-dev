/**
********************************************************************************
*
*	@file		TriangleMesh.cxx
*
*	@brief		Class to handle 3D triangle meshes.
*
*	@version	1.0
*
*	@date		07/11/2013
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
#ifndef TRIANGLE_MESH_H
#include "TriangleMesh.h"
#endif

#ifndef UNITS_H
#include "Units.h"
#endif

#ifndef FILE_DOES_NOT_EXIST_EXCEPTION_H
#include "FileDoesNotExistException.h"
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


//----------------------------------------------------------
void TriangleMesh::loadSTLFile(bool aMoveToCentreFlag,
							   bool anAutoComputeNormalFlag,
							   bool aPrintDebugInfoFlag,
							   bool aCreateVBOFlag,
							   RATIONAL_NUMBER aScale)
//----------------------------------------------------------
{
	// Reset the data
	reset();
	
	// Open the file in reading more
	FILE* p_stream(fopen(m_filename.c_str(),"rb"));

	// The file is not open
	if (!p_stream)
	{
		throw FileDoesNotExistException(__FILE__, __FUNCTION__, __LINE__, m_filename);
	}

	// Get the number of triangles in the file header
	// (the header is made of 80 bytes followded by the number of triangles (4 bytes))
	fseek(p_stream, 80, SEEK_SET);
	
	int triangle_number;
	fread(&(triangle_number), sizeof(float),1,p_stream);

	// Set the size of arrays to store the vertices and normal vectors
	m_p_vertex_set.resize(triangle_number * 3 * 3);
	m_p_normal_set.resize(triangle_number * 3 * 3);
	
	// Read the data
	for (int i(0); i < getTriangleNumber(); i++)
	{
		// Read normals
		fread(&m_p_normal_set[9 * i], sizeof(float), 3, p_stream);

		// Copy one normal per vertex 
		if (!anAutoComputeNormalFlag)
		{
			for (int k=1; k<3; k++)
			{
				m_p_normal_set[9 * i + 3 * k]     = m_p_normal_set[9 * i];
				m_p_normal_set[9 * i + 3 * k + 1] = m_p_normal_set[9 * i + 1];
				m_p_normal_set[9 * i + 3 * k + 2] = m_p_normal_set[9 * i + 2];
			}
		}

		// Read vertices
		fread(&m_p_vertex_set[9 * i],     sizeof(float), 3, p_stream);
		fread(&m_p_vertex_set[9 * i + 3], sizeof(float), 3, p_stream);
		fread(&m_p_vertex_set[9 * i + 6], sizeof(float), 3, p_stream);

		// Compute the normal vectors using the cross product
		if (anAutoComputeNormalFlag)
		{
			VEC3 vec_A(m_p_vertex_set[9 * i     + 0], m_p_vertex_set[9 *i     + 1], m_p_vertex_set[9 * i     + 2]);
			VEC3 vec_B(m_p_vertex_set[9 * i + 3 + 0], m_p_vertex_set[9 *i + 3 + 1], m_p_vertex_set[9 * i + 3 + 2]);
			VEC3 vec_C(m_p_vertex_set[9 * i + 6 + 0], m_p_vertex_set[9 *i + 6 + 1], m_p_vertex_set[9 * i + 6 + 2]);

			VEC3 normal( (vec_C - vec_A)^(vec_C - vec_B) );
			normal.normalize();

			for (int k=0; k<3; k++)
			{
				m_p_normal_set[9 * i + 3 * k]     = normal.getX();
				m_p_normal_set[9 * i + 3 * k + 1] = normal.getY();
				m_p_normal_set[9 * i + 3 * k + 2] = normal.getZ();
			}
		}

		// Skip data
		fseek(p_stream, 2, SEEK_CUR);
	}

	// Normalise all the normal vectors
	float* p_normals(&m_p_normal_set[0]);
	for (int i(0); i < getNormalNumber(); i++, p_normals += 3)
	{
		VEC3 normal(p_normals[0], p_normals[1], p_normals[2]);
		normal.normalize();

		p_normals[0] = normal.getX();
		p_normals[1] = normal.getY();
		p_normals[2] = normal.getZ();
	}

	// Update the first vertex
	m_p_vertex_set[0] *= aScale;
	m_p_vertex_set[1] *= aScale;
	m_p_vertex_set[2] *= aScale;

	// Change scale and compute bounding box
	m_min_corner = VEC3(m_p_vertex_set[0], m_p_vertex_set[1], m_p_vertex_set[2]);
	m_max_corner = m_min_corner;
	for (int i(1); i < getVertexNumber(); i++)
	{
		m_p_vertex_set[i * 3 + 0] *= aScale;
		m_p_vertex_set[i * 3 + 1] *= aScale;
		m_p_vertex_set[i * 3 + 2] *= aScale;

		if (m_min_corner.getX() > m_p_vertex_set[i * 3 + 0]) m_min_corner.setX(m_p_vertex_set[i * 3 + 0]);
		if (m_max_corner.getX() < m_p_vertex_set[i * 3 + 0]) m_max_corner.setX(m_p_vertex_set[i * 3 + 0]);
		if (m_min_corner.getY() > m_p_vertex_set[i * 3 + 1]) m_min_corner.setY(m_p_vertex_set[i * 3 + 1]);
		if (m_max_corner.getY() < m_p_vertex_set[i * 3 + 1]) m_max_corner.setY(m_p_vertex_set[i * 3 + 1]);
		if (m_min_corner.getZ() > m_p_vertex_set[i * 3 + 2]) m_min_corner.setZ(m_p_vertex_set[i * 3 + 2]);
		if (m_max_corner.getZ() < m_p_vertex_set[i * 3 + 2]) m_max_corner.setZ(m_p_vertex_set[i * 3 + 2]);
	}

	// Move the object to the center
	if (aMoveToCentreFlag)
	{
		VEC3 translation_vector(m_min_corner + (m_max_corner - m_min_corner) / 2.0);
		translation_vector *= -1.0;

		for (int i(0); i < getVertexNumber(); i++)
		{
			m_p_vertex_set[i * 3 + 0] += translation_vector.getX();
			m_p_vertex_set[i * 3 + 1] += translation_vector.getY();
			m_p_vertex_set[i * 3 + 2] += translation_vector.getZ();
		}

		m_min_corner += translation_vector;
		m_max_corner += translation_vector;
	}

	// Compute diagonal size
	m_bounding_box_diagonal = (m_min_corner - m_max_corner).length();

	// Display extra debug info
	if (aPrintDebugInfoFlag)
	{
		std::cout << "file_name:\t" << m_filename << "\tnb_triangles:\t" << getTriangleNumber() <<
				"\tbounding_box (in cm):\t(" << m_min_corner.getX() / cm << ", " << m_min_corner.getY() / cm << ", " << m_min_corner.getZ() / cm << ")\t(" <<
										m_max_corner.getX() / cm << ", " << m_max_corner.getY() / cm << ", " << m_max_corner.getZ() / cm << ")" << std::endl;
	}
	
	// Close the STL file
	fclose(p_stream);

	// Create the VBO
	if (aCreateVBOFlag)
	{
		m_vbo.setVertexData(GL_TRIANGLES, getVertexNumber(), 3, &m_p_vertex_set[0],
				getNormalNumber(), 3, &m_p_normal_set[0],
				0, 0, 0);
	}
}


//-------------------------------
void TriangleMesh::moveToCentre()
//-------------------------------
{
	VEC3 translation_vector(m_min_corner + (m_max_corner - m_min_corner) / 2.0);
	translation_vector *= -1.0;

	for (int i = 0; i < getVertexNumber(); i++)
	{
		m_p_vertex_set[i * 3 + 0] += GLfloat(translation_vector.getX());
		m_p_vertex_set[i * 3 + 1] += GLfloat(translation_vector.getY());
		m_p_vertex_set[i * 3 + 2] += GLfloat(translation_vector.getZ());
	}

	m_min_corner += translation_vector;
	m_max_corner += translation_vector;

	// Update the VBO
	if (m_vbo.isReady())
	{
		m_vbo.setVertexData(GL_TRIANGLES, getVertexNumber(), 3, &m_p_vertex_set[0],
				getNormalNumber(), 3, &m_p_normal_set[0],
				0, 0, 0);
	}
}


//------------------------------------
void TriangleMesh::autoComputeNormal()
//------------------------------------
{
	for (int i(0); i < getTriangleNumber(); i++)
	{
		VEC3 vec_A(m_p_vertex_set[9 * i     + 0], m_p_vertex_set[9 * i     + 1], m_p_vertex_set[9 * i     + 2]);
		VEC3 vec_B(m_p_vertex_set[9 * i + 3 + 0], m_p_vertex_set[9 * i + 3 + 1], m_p_vertex_set[9 * i + 3 + 2]);
		VEC3 vec_C(m_p_vertex_set[9 * i + 6 + 0], m_p_vertex_set[9 * i + 6 + 1], m_p_vertex_set[9 * i + 6 + 2]);

		VEC3 normal( (vec_C - vec_A)^(vec_C - vec_B) );
		normal.normalize();

		for (int k(0); k<3; k++)
		{
			m_p_normal_set[9 * i + 3 * k    ] = GLfloat(normal.getX());
			m_p_normal_set[9 * i + 3 * k + 1] = GLfloat(normal.getY());
			m_p_normal_set[9 * i + 3 * k + 2] = GLfloat(normal.getZ());
		}
	}
}


//----------------------------------------------------------
void TriangleMesh::translate(const VEC3& aTranslationVector)
//----------------------------------------------------------
{
    for (int i = 0; i < getVertexNumber(); i++)
    {
        m_p_vertex_set[i * 3 + 0] += GLfloat(aTranslationVector.getX());
        m_p_vertex_set[i * 3 + 1] += GLfloat(aTranslationVector.getY());
        m_p_vertex_set[i * 3 + 2] += GLfloat(aTranslationVector.getZ());
    }

	m_min_corner += aTranslationVector;
	m_max_corner += aTranslationVector;

	// Update the VBO
	if (m_vbo.isReady())
	{
		m_vbo.setVertexData(GL_TRIANGLES, getVertexNumber(), 3, &m_p_vertex_set[0],
				getNormalNumber(), 3, &m_p_normal_set[0],
				0, 0, 0);
	}
}
