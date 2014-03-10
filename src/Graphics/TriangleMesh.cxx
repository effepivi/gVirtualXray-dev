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
#include <map>

#ifndef TRIANGLE_MESH_H
#include "gVirtualXRay/TriangleMesh.h"
#endif

#ifndef UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#ifndef FILE_DOES_NOT_EXIST_EXCEPTION_H
#include "gVirtualXRay/FileDoesNotExistException.h"
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


//-----------------------------------------------------------
void TriangleMesh::displayUsingImmediateModeWithIndex() const
//-----------------------------------------------------------
{
	unsigned int number_of_indices(getIndexNumber());

	if (number_of_indices == 0)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	if (number_of_indices % 3 != 0)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid number of indices.");
	}

	glBegin(GL_TRIANGLES);
	for (unsigned int i(0); i < number_of_indices; ++i)
	{
		int id(getIndex(i));
		VEC3 vertex(getVertex(id));
		VEC3 normal(getVertexNormal(id));

		glNormal3f(normal.getX(), normal.getY(), normal.getZ());
		glVertex3f(vertex.getX(), vertex.getY(), vertex.getZ());
	}
	glEnd();

	// Check the OpenGL error status
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//--------------------------------------------------------------
void TriangleMesh::displayUsingImmediateModeWithoutIndex() const
//--------------------------------------------------------------
{
	unsigned int number_of_vertices(getVertexNumber());

	if (number_of_vertices == 0)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	if (number_of_vertices % 3 != 0)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid number of vertices.");
	}

	glBegin(GL_TRIANGLES);
	for (unsigned int i(0); i < number_of_vertices; ++i)
	{
		VEC3 vertex(getVertex(i));
		VEC3 normal(getVertexNormal(i));

		glNormal3f(normal.getX(), normal.getY(), normal.getZ());
		glVertex3f(vertex.getX(), vertex.getY(), vertex.getZ());
	}
	glEnd();

	// Check the OpenGL error status
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//--------------------------------------------------
void TriangleMesh::displayWireFrameWithIndex() const
//--------------------------------------------------
{
	unsigned int number_of_indices(getIndexNumber());

	if (number_of_indices == 0)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	if (number_of_indices % 3 != 0)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid number of indices.");
	}

	for (unsigned int i(0); i < number_of_indices; i += 3)
	{
		glBegin(GL_LINE_LOOP);
		for (unsigned int j(0); j < 3; ++j)
		{
			int id(getIndex(i + j));
			VEC3 vertex(getVertex(id));
			VEC3 normal(getVertexNormal(id));

			glNormal3f(normal.getX(), normal.getY(), normal.getZ());
			glVertex3f(vertex.getX(), vertex.getY(), vertex.getZ());
		}
		glEnd();
	}

	// Check the OpenGL error status
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//-----------------------------------------------------
void TriangleMesh::displayWireFrameWithoutIndex() const
//-----------------------------------------------------
{
	unsigned int number_of_vertices(getVertexNumber());

	if (number_of_vertices == 0)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	if (number_of_vertices % 3 != 0)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid number of vertices.");
	}

	for (unsigned int i(0); i < number_of_vertices; i += 3)
	{
		glBegin(GL_LINE_LOOP);
		for (unsigned int j(0); j < 3; ++j)
		{
			VEC3 vertex(getVertex(i + j));
			VEC3 normal(getVertexNormal(i + j));

			glNormal3f(normal.getX(), normal.getY(), normal.getZ());
			glVertex3f(vertex.getX(), vertex.getY(), vertex.getZ());
		}
		glEnd();
	}

	// Check the OpenGL error status
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//-------------------------------------------------------------
void TriangleMesh::displayFaceNormal(GLfloat aNormalSize) const
//-------------------------------------------------------------
{
	// By default try the external data
	if (m_p_external_vertex_set)
	{
	}
	// Use the internal data
	else
	{
		// There is an index
		if (m_p_internal_index_set.size())
		{
			glBegin(GL_LINES);
			for (unsigned int i(0); i < m_p_internal_index_set.size(); i += 3)
			{
				unsigned int id_0(m_p_internal_index_set[i + 0]);
				unsigned int id_1(m_p_internal_index_set[i + 1]);
				unsigned int id_2(m_p_internal_index_set[i + 2]);

				// Normal per face does not exist, but it can be computed
				VEC3 vertex_0(getVertex(id_0));
				VEC3 vertex_1(getVertex(id_1));
				VEC3 vertex_2(getVertex(id_2));

				VEC3 centre((vertex_0 + vertex_1 + vertex_2) / 3.0);

				VEC3 normal( (vertex_2 - vertex_0)^(vertex_2 - vertex_1) );
				normal.normalize();

				normal = centre + normal * 0.5 * cm;

				glVertex3f(centre.getX(), centre.getY(), centre.getZ());
				glVertex3f(normal.getX(), normal.getY(), normal.getZ());
			}
			glEnd();
		}
		// There is no index
		else
		{
			// There are as many normals as there are vertices
			if (getFaceNormalNumber() == getVertexNumber())
			{
				glBegin(GL_LINES);
				for (unsigned int i(0); i < getVertexNumber(); i += 3)
				{
					unsigned int id_0(i + 0);
					unsigned int id_1(i + 1);
					unsigned int id_2(i + 2);

					VEC3 vertex_0(getVertex(id_0));
					VEC3 vertex_1(getVertex(id_1));
					VEC3 vertex_2(getVertex(id_2));

					VEC3 centre((vertex_0 + vertex_1 + vertex_2) / 3.0);

					VEC3 normal_0(getFaceNormal(id_0));
					VEC3 normal_1(getFaceNormal(id_1));
					VEC3 normal_2(getFaceNormal(id_2));

					VEC3 normal((normal_0 + normal_1 + normal_2) / 3.0);
					normal.normalize();

					normal = centre + normal * aNormalSize;

					glVertex3f(centre.getX(), centre.getY(), centre.getZ());
					glVertex3f(normal.getX(), normal.getY(), normal.getZ());
				}
				glEnd();
			}
			// There are not as many normals as there are vertices
			else
			{
				throw Exception(__FILE__, __FUNCTION__, __LINE__, "There are not as many normals as there are vertices.");
			}
		}
	}
}


//----------------------------------------------------------
void TriangleMesh::loadSTLFile(bool aMoveToCentreFlag,
							   bool anAutoComputeNormalFlag,
							   bool aPrintDebugInfoFlag,
							   bool aCreateVBOFlag,
							   RATIONAL_NUMBER aScale,
							   GLenum aTypeOfVBO)
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
	m_p_internal_vertex_set.resize(triangle_number * 3 * 3);
	m_p_face_normal_set.resize(triangle_number * 3 * 3);
	m_p_vertex_normal_set.resize(triangle_number * 3 * 3);
	
	// Read the data
	for (unsigned int i(0); i < getTriangleNumber(); i++)
	{
		// Read normals
		fread(&m_p_face_normal_set[9 * i], sizeof(float), 3, p_stream);

		// Copy one normal per vertex 
		if (!anAutoComputeNormalFlag)
		{
			for (unsigned int k=1; k<3; k++)
			{
				m_p_face_normal_set[9 * i + 3 * k]     = m_p_face_normal_set[9 * i];
				m_p_face_normal_set[9 * i + 3 * k + 1] = m_p_face_normal_set[9 * i + 1];
				m_p_face_normal_set[9 * i + 3 * k + 2] = m_p_face_normal_set[9 * i + 2];

				m_p_vertex_normal_set[9 * i + 3 * k]     = m_p_face_normal_set[9 * i];
				m_p_vertex_normal_set[9 * i + 3 * k + 1] = m_p_face_normal_set[9 * i + 1];
				m_p_vertex_normal_set[9 * i + 3 * k + 2] = m_p_face_normal_set[9 * i + 2];
			}
		}

		// Read vertices
		fread(&m_p_internal_vertex_set[9 * i],     sizeof(float), 3, p_stream);
		fread(&m_p_internal_vertex_set[9 * i + 3], sizeof(float), 3, p_stream);
		fread(&m_p_internal_vertex_set[9 * i + 6], sizeof(float), 3, p_stream);

		// Skip data
		fseek(p_stream, 2, SEEK_CUR);
	}

	// Compute the normal vectors using the cross product
	if (anAutoComputeNormalFlag)
	{
		computeNormalVectors();
		normaliseNormals();
	}

	// Change scale and compute bounding box
	applyScale(aScale);
	computeBoundingBox();

	// Move the object to the center
	if (aMoveToCentreFlag)
	{
		moveToCentre();
	}

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
		updateVBO(aTypeOfVBO);
	}
}


//---------------------------------------
void TriangleMesh::computeNormalVectors()
//---------------------------------------
{
	m_p_face_normal_set.clear();
	m_p_vertex_normal_set.clear();

	// There is an index
	if (getIndexNumber())
	{
		if (getIndexNumber() % 3 != 0)
		{
			throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
		}

		// Allocate the memory
		m_p_vertex_normal_set.resize(getVertexNumber() * 3, 0.0);

		// Process every triangle
		for (unsigned int i(0); i < getIndexNumber(); i += 3)
		{
			// Get the vertex IDs
			int id_0(getIndex(i + 0));
			int id_1(getIndex(i + 1));
			int id_2(getIndex(i + 2));

			// Get the vertices
			VEC3 vec_A(getVertex(id_0));
			VEC3 vec_B(getVertex(id_1));
			VEC3 vec_C(getVertex(id_2));

			// Compute the normal for the triangle
			VEC3 normal((vec_C - vec_A)^(vec_C - vec_B));

			// Normalise the vector
			normal.normalize();

			// Add the new normal
			m_p_vertex_normal_set[id_0 * 3 + 0] += normal.getX();
			m_p_vertex_normal_set[id_0 * 3 + 1] += normal.getY();
			m_p_vertex_normal_set[id_0 * 3 + 2] += normal.getZ();

			m_p_vertex_normal_set[id_1 * 3 + 0] += normal.getX();
			m_p_vertex_normal_set[id_1 * 3 + 1] += normal.getY();
			m_p_vertex_normal_set[id_1 * 3 + 2] += normal.getZ();

			m_p_vertex_normal_set[id_2 * 3 + 0] += normal.getX();
			m_p_vertex_normal_set[id_2 * 3 + 1] += normal.getY();
			m_p_vertex_normal_set[id_2 * 3 + 2] += normal.getZ();
		}

		normaliseNormals();
	}
	// There is not any index
	else
	{
		if (getVertexNumber() % 3 != 0)
		{
			throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
		}

		// Allocate the memory
		m_p_face_normal_set.resize(getVertexNumber() * 3, 0.0);
		m_p_vertex_normal_set.resize(getVertexNumber() * 3, 0.0);

		// Process every triangle
		for (unsigned int i(0); i < getVertexNumber(); i += 3)
		{
			// Get the vertex IDs
			int id_0(i + 0);
			int id_1(i + 1);
			int id_2(i + 2);

			// Get the vertices
			VEC3 vec_A(getVertex(id_0));
			VEC3 vec_B(getVertex(id_1));
			VEC3 vec_C(getVertex(id_2));

			// Compute the normal for the triangle
			VEC3 normal((vec_C - vec_A)^(vec_C - vec_B));

			// Normalise the vector
			normal.normalize();

			// Add the new normal
			m_p_face_normal_set[id_0 * 3 + 0] = normal.getX();
			m_p_face_normal_set[id_0 * 3 + 1] = normal.getY();
			m_p_face_normal_set[id_0 * 3 + 2] = normal.getZ();

			m_p_face_normal_set[id_1 * 3 + 0] = normal.getX();
			m_p_face_normal_set[id_1 * 3 + 1] = normal.getY();
			m_p_face_normal_set[id_1 * 3 + 2] = normal.getZ();

			m_p_face_normal_set[id_2 * 3 + 0] = normal.getX();
			m_p_face_normal_set[id_2 * 3 + 1] = normal.getY();
			m_p_face_normal_set[id_2 * 3 + 2] = normal.getZ();

			m_p_vertex_normal_set[id_0 * 3 + 0] = normal.getX();
			m_p_vertex_normal_set[id_0 * 3 + 1] = normal.getY();
			m_p_vertex_normal_set[id_0 * 3 + 2] = normal.getZ();

			m_p_vertex_normal_set[id_1 * 3 + 0] = normal.getX();
			m_p_vertex_normal_set[id_1 * 3 + 1] = normal.getY();
			m_p_vertex_normal_set[id_1 * 3 + 2] = normal.getZ();

			m_p_vertex_normal_set[id_2 * 3 + 0] = normal.getX();
			m_p_vertex_normal_set[id_2 * 3 + 1] = normal.getY();
			m_p_vertex_normal_set[id_2 * 3 + 2] = normal.getZ();
		}
	}
}


//-----------------------------------
void TriangleMesh::normaliseNormals()
//-----------------------------------
{
	if (!getFaceNormalNumber() && !getVertexNormalNumber())
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	for (unsigned int i(0); i < getFaceNormalNumber(); ++i)
	{
		VEC3 normal(m_p_face_normal_set[i * 3 + 0],
				m_p_face_normal_set[i * 3 + 1],
				m_p_face_normal_set[i * 3 + 2]);

		normal.normalise();

		m_p_face_normal_set[i * 3 + 0] = normal.getX();
		m_p_face_normal_set[i * 3 + 1] = normal.getY();
		m_p_face_normal_set[i * 3 + 2] = normal.getZ();
	}

	for (unsigned int i(0); i < getVertexNormalNumber(); ++i)
	{
		VEC3 normal(m_p_vertex_normal_set[i * 3 + 0],
				m_p_vertex_normal_set[i * 3 + 1],
				m_p_vertex_normal_set[i * 3 + 2]);

		normal.normalise();

		m_p_vertex_normal_set[i * 3 + 0] = normal.getX();
		m_p_vertex_normal_set[i * 3 + 1] = normal.getY();
		m_p_vertex_normal_set[i * 3 + 2] = normal.getZ();
	}
}


struct VectorComparer
{
	// Return true if aLeftNode is less than aRightNode
    bool operator()(const VEC3 & aLeftNode, const VEC3 & aRightNode) const
    {
    	double epsilon(EPSILON);

    	if (fabs(aLeftNode.getX() - aRightNode.getX()) > epsilon)
    	{
    		if (aLeftNode.getX() < aRightNode.getX())
    		{
    			return (true);
    		}
    		else
    		{
    			return (false);
    		}
    	}

    	if (fabs(aLeftNode.getY() - aRightNode.getY()) > epsilon)
    	{
    		if (aLeftNode.getY() < aRightNode.getY())
    		{
    			return (true);
    		}
    		else
    		{
    			return (false);
    		}
    	}

    	if (fabs(aLeftNode.getZ() - aRightNode.getZ()) > epsilon)
    	{
    		if (aLeftNode.getZ() < aRightNode.getZ())
    		{
    			return (true);
    		}
    		else
    		{
    			return (false);
    		}
    	}

    	return (false);
    }
};


class TempRecord
{
public:
	inline const TempRecord& operator +=(const VEC3& aNormal)
	{
		m_normal += aNormal;

		return (*this);
	}


	inline const TempRecord& operator =(unsigned int anID)
	{
		m_id = anID;

		return (*this);
	}

	VEC3 m_normal;
	unsigned int m_id;
};


//--------------------------------
void TriangleMesh::mergeVertices()
//--------------------------------
{
	// Merge vertices only if external data is not used
	if (!m_p_external_vertex_set)
	{
		// It only works if no index has been set
		if (!m_p_internal_index_set.size())
		{
			// Allocate the memory of the index
			m_p_internal_index_set.resize(getTriangleNumber() * 3);

			std::map<VEC3, TempRecord, VectorComparer> hash_table;

			// Add the vertices to the hash table
			unsigned int number_of_vertices(getVertexNumber());
			for (unsigned int i(0); i < number_of_vertices; ++i)
			{
				VEC3 current_vertex(getVertex(i));
				VEC3 normal(getFaceNormal(i));

				hash_table[current_vertex].m_normal += normal;
			}

			m_p_vertex_normal_set.clear();
			m_p_vertex_normal_set.resize(hash_table.size() * 3);

			unsigned int id(0);
			for (std::map<VEC3, TempRecord, VectorComparer>::iterator ite(hash_table.begin());
					ite != hash_table.end();
					++ite, ++id)
			{
				VEC3 normal(ite->second.m_normal.normal());

				m_p_vertex_normal_set[id * 3 + 0] = normal.getX();
				m_p_vertex_normal_set[id * 3 + 1] = normal.getY();
				m_p_vertex_normal_set[id * 3 + 2] = normal.getZ();
			}

			// Set the index in the hash table and create the list of vertices
			std::vector<GLfloat> new_vertex_set(hash_table.size() * 3);
			id = 0;
			for (std::map<VEC3, TempRecord, VectorComparer>::iterator ite(hash_table.begin());
					ite != hash_table.end();
					++ite, ++id)
			{
				ite->second.m_id = id;

				new_vertex_set[id * 3 + 0] = ite->first.getX();
				new_vertex_set[id * 3 + 1] = ite->first.getY();
				new_vertex_set[id * 3 + 2] = ite->first.getZ();
			}

			// Fill the index
			unsigned int number_of_triangles(getTriangleNumber());
			for (unsigned int i(0); i < number_of_triangles; ++i)
			{
				VEC3 vertex_0(getVertex(i * 3 + 0));
				VEC3 vertex_1(getVertex(i * 3 + 1));
				VEC3 vertex_2(getVertex(i * 3 + 2));

				unsigned int id_0(hash_table.find(vertex_0)->second.m_id);
				unsigned int id_1(hash_table.find(vertex_1)->second.m_id);
				unsigned int id_2(hash_table.find(vertex_2)->second.m_id);

				if (id_0 >= hash_table.size() || id_1 >= hash_table.size() || id_2 >= hash_table.size())
				{
					throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid vertex index");
				}

				m_p_internal_index_set[i * 3 + 0] = id_0;
				m_p_internal_index_set[i * 3 + 1] = id_1;
				m_p_internal_index_set[i * 3 + 2] = id_2;
			}

			// Normalise the normals in the normal set
			unsigned int number_of_normals(getVertexNormalNumber());
			for (unsigned int i(0); i < number_of_normals; ++i)
			{
				VEC3 normal(getVertexNormal(i));
				normal.normalise();
				m_p_vertex_normal_set[i * 3 + 0] += normal.getX();
				m_p_vertex_normal_set[i * 3 + 1] += normal.getY();
				m_p_vertex_normal_set[i * 3 + 2] += normal.getZ();
			}

			// Replace the list of vertices
			m_p_internal_vertex_set = new_vertex_set;

			// Reset the normal per face set
			m_p_face_normal_set.clear();

			// Update the VBO
			if (m_vbo.isReady())
			{
				updateVBO(m_vbo.getBufferUsageHint());
			}
		}
	}
	// Generate an error
	else
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "The operation cannot be performed.");
	}
}


//---------------------------------------------
void TriangleMesh::updateVBO(GLenum aTypeOfVBO)
//---------------------------------------------
{
	// Set the type of VBO
	m_vbo.setBufferUsageHint(aTypeOfVBO);

	// By default try the external data
	if (m_p_external_vertex_set)
	{
		if (getVertexNormalNumber())
		{
			m_vbo.setVertexData(GL_TRIANGLES, getVertexNumber(), 3, &(*m_p_external_vertex_set)[0],
					getVertexNormalNumber(), 3, &m_p_vertex_normal_set[0],
					0, 0, 0);
		}
		else if (getFaceNormalNumber())
		{
			m_vbo.setVertexData(GL_TRIANGLES, getVertexNumber(), 3, &(*m_p_external_vertex_set)[0],
					getFaceNormalNumber(), 3, &m_p_face_normal_set[0],
					0, 0, 0);
		}
		else
		{
			m_vbo.setVertexData(GL_TRIANGLES, getVertexNumber(), 3, &(*m_p_external_vertex_set)[0],
					0, 0, 0,
					0, 0, 0);
		}

		if (m_p_external_index_set)
		{
			if (m_p_external_index_set->size())
			{
				m_vbo.setIndexData(m_p_external_index_set->size(), &(*m_p_external_index_set)[0]);
			}
		}
	}
	// Use the internal data
	else
	{
		if (getVertexNormalNumber())
		{
			m_vbo.setVertexData(GL_TRIANGLES, getVertexNumber(), 3, &m_p_internal_vertex_set[0],
					getVertexNormalNumber(), 3, &m_p_vertex_normal_set[0],
					0, 0, 0);
		}
		else if (getFaceNormalNumber())
		{
			m_vbo.setVertexData(GL_TRIANGLES, getVertexNumber(), 3, &m_p_internal_vertex_set[0],
					getFaceNormalNumber(), 3, &m_p_face_normal_set[0],
					0, 0, 0);
		}
		else
		{
			m_vbo.setVertexData(GL_TRIANGLES, getVertexNumber(), 3, &m_p_internal_vertex_set[0],
					0, 0, 0,
					0, 0, 0);
		}

		if (m_p_internal_index_set.size())
		{
			m_vbo.setIndexData(m_p_internal_index_set.size(), &m_p_internal_index_set[0]);
		}
	}
}
