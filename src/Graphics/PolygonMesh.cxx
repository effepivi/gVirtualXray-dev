/**
********************************************************************************
*
*	@file		PolygonMesh.cxx
*
*	@brief		Class to handle polygon (triangles) meshes.
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

#ifndef POLYGON_MESH_H
#include "gVirtualXRay/PolygonMesh.h"
#endif

#ifndef UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#ifndef FILE_DOES_NOT_EXIST_EXCEPTION_H
#include "gVirtualXRay/FileDoesNotExistException.h"
#endif

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/gl3.h>
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


//-----------------------------------
PolygonMesh::PolygonMesh():
//-----------------------------------
		m_use_hounsfield_value(true),
		m_hounsfield_value(0),
		m_attenuation_coefficient(0),
		m_bounding_box_diagonal(0),
		m_p_vertex_set(0),
		m_p_index_set(0),
		m_number_of_indices(0),
		m_number_of_vertices(0),
		m_index_data_type(GL_NONE),
		m_vertex_data_type(GL_NONE),
		m_polygon_type(GL_NONE),
		m_external_data_flag(false)
//-----------------------------------
{
}


//-----------------------
void PolygonMesh::reset()
//-----------------------
{
	// Reset class members to their default values
	m_vbo.reset();

	m_use_hounsfield_value = true;
	m_hounsfield_value = 0;
	m_attenuation_coefficient = 0;

	m_bounding_box_diagonal = 0;

	m_min_corner = VEC3();
	m_max_corner = VEC3();

	m_p_face_normal_set.clear();
	m_p_vertex_normal_set.clear();

	destroyIndexData();
	destroyVertexData();

	m_number_of_indices  = 0;
	m_number_of_vertices = 0;

	m_index_data_type = GL_NONE;
	m_vertex_data_type = GL_NONE;

	m_external_data_flag = false;
}


//-------------------------
void PolygonMesh::display()
//-------------------------
{
	// Update the VBO if needed
	if (!m_vbo.isReady())
	{
		updateVBO(GL_DYNAMIC_DRAW, m_polygon_type);
	}

	// Display the polygon mesh using a VBO
	if (m_vbo.isReady())
	{
		m_vbo.display();
	}
	// Do not use the VBO
	else
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create the VBO.");
	}

	// Check the OpenGL error status
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//----------------------------------
void PolygonMesh::displayWireFrame()
//----------------------------------
{
	// Turn on wireframe mode
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);

	display();

	// Turn off wireframe mode
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);

	// Check the OpenGL error status
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//----------------------------------------------------------------------
void PolygonMesh::displayVertexNormal(RATIONAL_NUMBER aNormalSize) const
//----------------------------------------------------------------------
{
	// There are as many normals as there are vertices
	if (getVertexNormalNumber() == m_number_of_vertices)
	{
    		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented");
	}
	// There are not as many normals as there are vertices
	else
	{
		std::stringstream error_message;
		error_message << "There are not as many normals (" << getVertexNormalNumber() << ") as there are vertices (" << m_number_of_vertices << ").";
		throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
	}
}


//------------------------------------------------------------
void PolygonMesh::displayFaceNormal(GLfloat aNormalSize) const
//------------------------------------------------------------
{
	// There is an index
	if (m_p_index_set && m_number_of_indices)
	{
    		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented");
	}
	// There is no index
	else
	{
		// There are as many normals as there are vertices
		if (getFaceNormalNumber() == m_number_of_vertices)
		{
    		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented");
		}
		// There are not as many normals as there are vertices
		else
		{
			throw Exception(__FILE__, __FUNCTION__, __LINE__, "There are not as many normals as there are vertices.");
		}
	}
}


//---------------------------------------------
unsigned int PolygonMesh::getFaceNumber() const
//---------------------------------------------
{
	int number_vertex_per_face(0);

	switch (m_polygon_type)
	{
	case GL_TRIANGLES:
		number_vertex_per_face = 3;
		break;

	default:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid polygon type.");
	}

	// Return the number of faces
	if (m_p_index_set && m_number_of_indices)
	{
		// There are 3 or 4 indices per face
		return (m_number_of_indices / number_vertex_per_face);
	}
	else
	{
		// There are 3 or 4 vertices per face
		return (m_number_of_vertices / number_vertex_per_face);
	}
}


//-------------------------------------------------
unsigned int PolygonMesh::getTriangleNumber() const
//-------------------------------------------------
{
	switch (m_polygon_type)
	{
	case GL_TRIANGLES:
		// Return the number of faces
		if (m_p_index_set && m_number_of_indices)
		{
			// There are 3 indices per triangle
			return (m_number_of_indices / 3);
		}
		else
		{
			// There are 3 vertices per triangle
			return (m_number_of_vertices / 3);
		}

	default:
		return (0);
	}
}


//---------------------------------------------------------
void PolygonMesh::loadSTLFile(bool aMoveToCentreFlag,
							  bool anAutoComputeNormalFlag,
							  bool aPrintDebugInfoFlag,
							  bool aCreateVBOFlag,
							  RATIONAL_NUMBER aScale,
							  int aTypeOfVBO)
//---------------------------------------------------------
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

	// Set the kind of polygon
	m_polygon_type = GL_TRIANGLES;

	// Set the size of arrays to store the vertices and normal vectors
	m_external_data_flag = false;
	m_number_of_vertices = triangle_number * 3;
	GLfloat* p_vertex_set(new GLfloat[m_number_of_vertices * 3]);
	m_vertex_data_type = GL_FLOAT;
	m_p_vertex_set = p_vertex_set;

	m_p_face_normal_set.resize(m_number_of_vertices * 3);
	m_p_vertex_normal_set.resize(m_number_of_vertices * 3);

	
	// Read the data
	for (unsigned int i(0); i < getFaceNumber(); i++)
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
		fread(&p_vertex_set[9 * i],     sizeof(float), 3, p_stream);
		fread(&p_vertex_set[9 * i + 3], sizeof(float), 3, p_stream);
		fread(&p_vertex_set[9 * i + 6], sizeof(float), 3, p_stream);

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
		std::cout << "file_name:\t" << m_filename <<
				"\tnb_faces:\t" << getFaceNumber() <<
				"\tnb_vertices:\t" << m_number_of_vertices <<
				"\tbounding_box (in cm):\t(" << m_min_corner.getX() / cm << ", " << m_min_corner.getY() / cm << ", " << m_min_corner.getZ() / cm << ")\t(" <<
										m_max_corner.getX() / cm << ", " << m_max_corner.getY() / cm << ", " << m_max_corner.getZ() / cm << ")" << std::endl;
	}
	
	// Close the STL file
	fclose(p_stream);

	// Create the VBO
	if (aCreateVBOFlag)
	{
		updateVBO(aTypeOfVBO, m_polygon_type);
	}
}


//--------------------------------------
void PolygonMesh::computeNormalVectors()
//--------------------------------------
{
	m_p_face_normal_set.clear();
	m_p_vertex_normal_set.clear();

	// There is an index
	if (m_number_of_indices)
	{
		if (m_number_of_indices % 3 != 0)
		{
			throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
		}

		// Allocate the memory
		m_p_vertex_normal_set.resize(m_number_of_vertices * 3, 0.0);

		// Process every triangle
		for (unsigned int i(0); i < m_number_of_indices; i += 3)
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
		if (m_number_of_vertices % 3 != 0)
		{
			throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
		}

		// Allocate the memory
		m_p_face_normal_set.resize(m_number_of_vertices * 3, 0.0);
		m_p_vertex_normal_set.resize(m_number_of_vertices * 3, 0.0);

		// Process every triangle
		for (unsigned int i(0); i < m_number_of_vertices; i += 3)
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


//----------------------------------
void PolygonMesh::normaliseNormals()
//----------------------------------
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


//-------------------------------------------------------
void PolygonMesh::mergeVertices(bool aPrintDebugInfoFlag)
//-------------------------------------------------------
{
	// Merge vertices only if external data is not used
	if (!m_external_data_flag)
	{
		// It only works if no index has been set
		if (!m_p_index_set && !m_number_of_indices)
		{
			// The polygon is made of triangles
			if (m_polygon_type == GL_TRIANGLES)
			{
				// Allocate the memory of the index
				int number_of_vertices_per_face(3);
				m_number_of_indices = getFaceNumber() * number_of_vertices_per_face;
				GLuint* p_index_set(new GLuint[m_number_of_indices]);
				m_index_data_type = GL_UNSIGNED_INT;
				m_p_index_set = p_index_set;

				// Add the vertices to the hash table
				std::map<VEC3, TempRecord, VectorComparer> hash_table;
				unsigned int number_of_vertices(m_number_of_vertices);
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

				// Set the index in the hash table
				id = 0;
				for (std::map<VEC3, TempRecord, VectorComparer>::iterator ite(hash_table.begin());
						ite != hash_table.end();
						++ite, ++id)
				{
					ite->second.m_id = id;
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

					p_index_set[i * 3 + 0] = id_0;
					p_index_set[i * 3 + 1] = id_1;
					p_index_set[i * 3 + 2] = id_2;
				}

				// Set the index in the hash table and create the list of vertices
				destroyVertexData();
				m_number_of_vertices = hash_table.size();

				GLfloat* p_float_vertex_set(0);
				GLdouble* p_double_vertex_set(0);
				switch (m_vertex_data_type)
				{
				case GL_FLOAT:
					m_p_vertex_set = new GLfloat[m_number_of_vertices * 3];
					p_float_vertex_set = reinterpret_cast<GLfloat*>(m_p_vertex_set);
					break;

				case GL_DOUBLE:
					m_p_vertex_set = new GLdouble[m_number_of_vertices * 3];
					p_double_vertex_set = reinterpret_cast<GLdouble*>(m_p_vertex_set);
					break;

				default:
					throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid data type.");
				}
				id = 0;
				for (std::map<VEC3, TempRecord, VectorComparer>::iterator ite(hash_table.begin());
						ite != hash_table.end();
						++ite, ++id)
				{
					if (p_float_vertex_set)
					{
						p_float_vertex_set[id * 3 + 0] = ite->first.getX();
						p_float_vertex_set[id * 3 + 1] = ite->first.getY();
						p_float_vertex_set[id * 3 + 2] = ite->first.getZ();
					}
					if (p_double_vertex_set)
					{
						p_double_vertex_set[id * 3 + 0] = ite->first.getX();
						p_double_vertex_set[id * 3 + 1] = ite->first.getY();
						p_double_vertex_set[id * 3 + 2] = ite->first.getZ();
					}
				}

				// Normalise the normals in the normal set
				unsigned int number_of_normals(getVertexNormalNumber());
				for (unsigned int i(0); i < number_of_normals; ++i)
				{
					VEC3 normal(getVertexNormal(i));
					normal.normalise();
					m_p_vertex_normal_set[i * 3 + 0] = normal.getX();
					m_p_vertex_normal_set[i * 3 + 1] = normal.getY();
					m_p_vertex_normal_set[i * 3 + 2] = normal.getZ();
				}

				// Reset the normal per face set
				m_p_face_normal_set.clear();

				// Update the VBO
				if (m_vbo.isReady())
				{
					updateVBO(m_vbo.getBufferUsageHint(), m_vbo.getPrimitiveType());
				}

				// Display extra debug info
				if (aPrintDebugInfoFlag)
				{
					std::cout << "file_name:\t" << m_filename <<
							"\tnb_faces:\t" << getFaceNumber() <<
							"\tnb_vertices:\t" << m_number_of_vertices <<
							"\tbounding_box (in cm):\t(" << m_min_corner.getX() / cm << ", " << m_min_corner.getY() / cm << ", " << m_min_corner.getZ() / cm << ")\t(" <<
													m_max_corner.getX() / cm << ", " << m_max_corner.getY() / cm << ", " << m_max_corner.getZ() / cm << ")" << std::endl;
				}
			}
			// Not supported
			else
			{
				throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented.");
			}
		}
		// Generate an error
		else
		{
			throw Exception(__FILE__, __FUNCTION__, __LINE__, "There is no vertex to merge.");
		}
	}
	// Generate an error
	else
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "The operation cannot be performed.");
	}
}


//----------------------------------------------------
void PolygonMesh::splitFaces(bool aPrintDebugInfoFlag)
//----------------------------------------------------
{
	// Split faces only if external data is not used
	if (!m_external_data_flag)
	{
		// It only works if no index has been set
		bool has_an_index(false);
		if (m_p_index_set && m_number_of_indices)
		{
			removeIndex(aPrintDebugInfoFlag);
			has_an_index = true;
		}

		// The polygon is made of triangles
		if (m_polygon_type == GL_TRIANGLES)
		{
			// Reset the normal per face set
			m_p_vertex_normal_set.clear();

			// Reset the normal per face set
			m_p_face_normal_set.clear();

			// Store the new vertices
			std::vector<double> p_new_vertex_set;

			// Process every triangle
			unsigned int triangle_number(getTriangleNumber());
			for (unsigned int i(0); i < triangle_number; ++i)
			{
				// Get the current vertices
				VEC3 vertex_0(getVertex(i * 3 + 0));
				VEC3 vertex_1(getVertex(i * 3 + 1));
				VEC3 vertex_2(getVertex(i * 3 + 2));

				// Get the new vertices
				VEC3 vertex_3((vertex_0 + vertex_1) / 2.0);
				VEC3 vertex_4((vertex_1 + vertex_2) / 2.0);
				VEC3 vertex_5((vertex_2 + vertex_0) / 2.0);

				// Compute the normal vector
				VEC3 normal((vertex_2 - vertex_0)^(vertex_2 - vertex_1));
				normal.normalize();

				// Add the first triangle
				p_new_vertex_set.push_back(vertex_0.getX());
				p_new_vertex_set.push_back(vertex_0.getY());
				p_new_vertex_set.push_back(vertex_0.getZ());
				p_new_vertex_set.push_back(vertex_3.getX());
				p_new_vertex_set.push_back(vertex_3.getY());
				p_new_vertex_set.push_back(vertex_3.getZ());
				p_new_vertex_set.push_back(vertex_5.getX());
				p_new_vertex_set.push_back(vertex_5.getY());
				p_new_vertex_set.push_back(vertex_5.getZ());

				for (unsigned int j(0); j < 3; ++j)
				{
					m_p_face_normal_set.push_back(normal.getX());
					m_p_face_normal_set.push_back(normal.getY());
					m_p_face_normal_set.push_back(normal.getZ());
				}

				// Add the second triangle
				p_new_vertex_set.push_back(vertex_3.getX());
				p_new_vertex_set.push_back(vertex_3.getY());
				p_new_vertex_set.push_back(vertex_3.getZ());
				p_new_vertex_set.push_back(vertex_1.getX());
				p_new_vertex_set.push_back(vertex_1.getY());
				p_new_vertex_set.push_back(vertex_1.getZ());
				p_new_vertex_set.push_back(vertex_4.getX());
				p_new_vertex_set.push_back(vertex_4.getY());
				p_new_vertex_set.push_back(vertex_4.getZ());

				for (unsigned int j(0); j < 3; ++j)
				{
					m_p_face_normal_set.push_back(normal.getX());
					m_p_face_normal_set.push_back(normal.getY());
					m_p_face_normal_set.push_back(normal.getZ());
				}

				// Add the third triangle
				p_new_vertex_set.push_back(vertex_3.getX());
				p_new_vertex_set.push_back(vertex_3.getY());
				p_new_vertex_set.push_back(vertex_3.getZ());
				p_new_vertex_set.push_back(vertex_4.getX());
				p_new_vertex_set.push_back(vertex_4.getY());
				p_new_vertex_set.push_back(vertex_4.getZ());
				p_new_vertex_set.push_back(vertex_5.getX());
				p_new_vertex_set.push_back(vertex_5.getY());
				p_new_vertex_set.push_back(vertex_5.getZ());

				for (unsigned int j(0); j < 3; ++j)
				{
					m_p_face_normal_set.push_back(normal.getX());
					m_p_face_normal_set.push_back(normal.getY());
					m_p_face_normal_set.push_back(normal.getZ());
				}

				// Add the fourth triangle
				p_new_vertex_set.push_back(vertex_5.getX());
				p_new_vertex_set.push_back(vertex_5.getY());
				p_new_vertex_set.push_back(vertex_5.getZ());
				p_new_vertex_set.push_back(vertex_4.getX());
				p_new_vertex_set.push_back(vertex_4.getY());
				p_new_vertex_set.push_back(vertex_4.getZ());
				p_new_vertex_set.push_back(vertex_2.getX());
				p_new_vertex_set.push_back(vertex_2.getY());
				p_new_vertex_set.push_back(vertex_2.getZ());

				for (unsigned int j(0); j < 3; ++j)
				{
					m_p_face_normal_set.push_back(normal.getX());
					m_p_face_normal_set.push_back(normal.getY());
					m_p_face_normal_set.push_back(normal.getZ());
				}
			}

			// Set the index in the hash table and create the list of vertices
			destroyVertexData();
			m_number_of_vertices = p_new_vertex_set.size() / 3;

			GLfloat* p_float_vertex_set(0);
			GLdouble* p_double_vertex_set(0);
			switch (m_vertex_data_type)
			{
			case GL_FLOAT:
				m_p_vertex_set = new GLfloat[m_number_of_vertices * 3];
				p_float_vertex_set = reinterpret_cast<GLfloat*>(m_p_vertex_set);
				break;

			case GL_DOUBLE:
				m_p_vertex_set = new GLdouble[m_number_of_vertices * 3];
				p_double_vertex_set = reinterpret_cast<GLdouble*>(m_p_vertex_set);
				break;

			default:
				throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid data type.");
			}

			// Save the result
			for (unsigned int id(0); id < p_new_vertex_set.size(); ++id)
			{
				if (p_float_vertex_set)
				{
					p_float_vertex_set[id] = p_new_vertex_set[id];
				}
				if (p_double_vertex_set)
				{
					p_double_vertex_set[id] = p_new_vertex_set[id];
				}
			}

			// Restore the index
			if (has_an_index)
			{
				mergeVertices(aPrintDebugInfoFlag);
			}

			// Update the VBO
			if (m_vbo.isReady())
			{
				updateVBO(m_vbo.getBufferUsageHint(), m_vbo.getPrimitiveType());
			}

			// Display extra debug info
			if (aPrintDebugInfoFlag)
			{
				std::cout << "file_name:\t" << m_filename <<
						"\tnb_faces:\t" << getFaceNumber() <<
						"\tnb_vertices:\t" << m_number_of_vertices <<
						"\tbounding_box (in cm):\t(" << m_min_corner.getX() / cm << ", " << m_min_corner.getY() / cm << ", " << m_min_corner.getZ() / cm << ")\t(" <<
												m_max_corner.getX() / cm << ", " << m_max_corner.getY() / cm << ", " << m_max_corner.getZ() / cm << ")" << std::endl;
			}
		}
		// Not supported
		else
		{
			throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented.");
		}
	}
	// Generate an error
	else
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "The operation cannot be performed.");
	}
}


//---------------------------------------------------------------
void PolygonMesh::updateVBO(int aTypeOfVBO, int aTypeOfPrimitive)
//---------------------------------------------------------------
{
	if (m_p_vertex_set && m_number_of_vertices)
	{
		// Set the type of VBO
		m_vbo.setBufferUsageHint(aTypeOfVBO);

		if (getVertexNormalNumber())
		{
			m_vbo.setVertexData(aTypeOfPrimitive, m_number_of_vertices, 3, m_vertex_data_type, m_p_vertex_set,
					getVertexNormalNumber(), 3, GL_FLOAT, &m_p_vertex_normal_set[0],
					0, 0, GL_NONE, 0);
		}
		else if (getFaceNormalNumber())
		{
			m_vbo.setVertexData(aTypeOfPrimitive, m_number_of_vertices, 3, m_vertex_data_type, m_p_vertex_set,
					getFaceNormalNumber(), 3, GL_FLOAT, &m_p_face_normal_set[0],
					0, 0, GL_NONE, 0);
		}
		else
		{
			m_vbo.setVertexData(aTypeOfPrimitive, m_number_of_vertices, 3, m_vertex_data_type, m_p_vertex_set,
					0, 0, GL_NONE, 0,
					0, 0, GL_NONE, 0);
		}

		if (m_p_index_set && m_number_of_indices && m_index_data_type != GL_NONE)
		{
			m_vbo.setIndexData(m_number_of_indices, m_index_data_type, m_p_index_set);
		}
	}
}


//-----------------------------------------------------
VEC3 PolygonMesh::getVertex(unsigned int anIndex) const
//-----------------------------------------------------
{
	if (m_number_of_vertices <= anIndex && m_p_vertex_set)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	double x, y, z;

	switch (m_vertex_data_type)
	{
	case GL_FLOAT:
		{
			GLfloat* p_temp(reinterpret_cast<GLfloat*>(m_p_vertex_set));
			x = p_temp[anIndex * 3 + 0];
			y = p_temp[anIndex * 3 + 1];
			z = p_temp[anIndex * 3 + 2];
		}
		break;

	case GL_DOUBLE:
		{
			GLdouble* p_temp(reinterpret_cast<GLdouble*>(m_p_vertex_set));
			x = p_temp[anIndex * 3 + 0];
			y = p_temp[anIndex * 3 + 1];
			z = p_temp[anIndex * 3 + 2];
		}
		break;

	default:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid data type.");
	}

	return (VEC3(x, y, z));
}


//-----------------------------------------------
void PolygonMesh::setVertex(unsigned int anIndex,
							const VEC3& aVertex)
//-----------------------------------------------
{
	if (m_number_of_vertices <= anIndex && m_p_vertex_set)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	switch (m_vertex_data_type)
	{
	case GL_FLOAT:
		{
			GLfloat* p_temp(reinterpret_cast<GLfloat*>(m_p_vertex_set));
			p_temp[anIndex * 3 + 0] = aVertex.getX();
			p_temp[anIndex * 3 + 1] = aVertex.getY();
			p_temp[anIndex * 3 + 2] = aVertex.getZ();
		}
		break;

	case GL_DOUBLE:
		{
			GLdouble* p_temp(reinterpret_cast<GLdouble*>(m_p_vertex_set));
			p_temp[anIndex * 3 + 0] = aVertex.getX();
			p_temp[anIndex * 3 + 1] = aVertex.getY();
			p_temp[anIndex * 3 + 2] = aVertex.getZ();
		}
		break;

	default:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid data type.");
	}
}


//---------------------------------------------------
int PolygonMesh::getIndex(unsigned int anIndex) const
//---------------------------------------------------
{
	if (m_number_of_indices <= anIndex && m_p_index_set)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	switch (m_index_data_type)
	{
	case GL_UNSIGNED_INT:
		{
			GLuint* p_temp(reinterpret_cast<GLuint*>(m_p_index_set));
			return (int(p_temp[anIndex]));
		}
		break;

	case GL_UNSIGNED_SHORT:
		{
			GLushort* p_temp(reinterpret_cast<GLushort*>(m_p_index_set));
			return (int(p_temp[anIndex]));
		}
		break;

	case GL_UNSIGNED_BYTE:
		{
			GLubyte* p_temp(reinterpret_cast<GLubyte*>(m_p_index_set));
			return (int(p_temp[anIndex]));
		}
		break;

	default:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid data type.");
	}
}


//-----------------------------------------------------------------------
void PolygonMesh::setExternalData(int aPolygonType,
								  const std::vector<float>* aVertexArray,
								  bool aCreateVBOFlag,
								  int aTypeOfVBO)
//-----------------------------------------------------------------------
{
	setExternalData(aPolygonType,
			&((*aVertexArray)[0]),
			aVertexArray->size() / 3,
			GL_FLOAT,
			0,
			0,
			GL_NONE,
			aCreateVBOFlag,
			aTypeOfVBO);
}


//------------------------------------------------------------------------
void PolygonMesh::setExternalData(int aPolygonType,
								  const std::vector<double>* aVertexArray,
								  bool aCreateVBOFlag,
								  int aTypeOfVBO)
//------------------------------------------------------------------------
{
	setExternalData(aPolygonType,
			&((*aVertexArray)[0]),
			aVertexArray->size() / 3,
			GL_DOUBLE,
			0,
			0,
			GL_NONE,
			aCreateVBOFlag,
			aTypeOfVBO);
}


//------------------------------------------------------------------------------
void PolygonMesh::setExternalData(int aPolygonType,
								  const std::vector<float>* aVertexArray,
								  const std::vector<unsigned int>* anIndexArray,
								  bool aCreateVBOFlag,
								  int aTypeOfVBO)
//------------------------------------------------------------------------------
{
	setExternalData(aPolygonType,
			&((*aVertexArray)[0]),
			aVertexArray->size() / 3,
			GL_FLOAT,
			&((*anIndexArray)[0]),
			anIndexArray->size(),
			GL_UNSIGNED_INT,
			aCreateVBOFlag,
			aTypeOfVBO);
}


//------------------------------------------------------------------------------
void PolygonMesh::setExternalData(int aPolygonType,
								  const std::vector<double>* aVertexArray,
								  const std::vector<unsigned int>* anIndexArray,
								  bool aCreateVBOFlag,
								  int aTypeOfVBO)
//------------------------------------------------------------------------------
{
	setExternalData(aPolygonType,
			&((*aVertexArray)[0]),
			aVertexArray->size() / 3,
			GL_DOUBLE,
			&((*anIndexArray)[0]),
			anIndexArray->size(),
			GL_UNSIGNED_INT,
			aCreateVBOFlag,
			aTypeOfVBO);
}


//--------------------------------------------------------------------------------
void PolygonMesh::setExternalData(int aPolygonType,
								  const std::vector<float>* aVertexArray,
								  const std::vector<unsigned short>* anIndexArray,
								  bool aCreateVBOFlag,
								  int aTypeOfVBO)
//--------------------------------------------------------------------------------
{
	setExternalData(aPolygonType,
			&((*aVertexArray)[0]),
			aVertexArray->size() / 3,
			GL_FLOAT,
			&((*anIndexArray)[0]),
			anIndexArray->size(),
			GL_UNSIGNED_SHORT,
			aCreateVBOFlag,
			aTypeOfVBO);
}


//--------------------------------------------------------------------------------
void PolygonMesh::setExternalData(int aPolygonType,
								  const std::vector<double>* aVertexArray,
								  const std::vector<unsigned short>* anIndexArray,
								  bool aCreateVBOFlag,
								  int aTypeOfVBO)
//--------------------------------------------------------------------------------
{
	setExternalData(aPolygonType,
			&((*aVertexArray)[0]),
			aVertexArray->size() / 3,
			GL_DOUBLE,
			&((*anIndexArray)[0]),
			anIndexArray->size(),
			GL_UNSIGNED_SHORT,
			aCreateVBOFlag,
			aTypeOfVBO);
}


//-------------------------------------------------------------------------------
void PolygonMesh::setExternalData(int aPolygonType,
								  const std::vector<float>* aVertexArray,
								  const std::vector<unsigned char>* anIndexArray,
								  bool aCreateVBOFlag,
								  int aTypeOfVBO)
//-------------------------------------------------------------------------------
{
	setExternalData(aPolygonType,
			&((*aVertexArray)[0]),
			aVertexArray->size() / 3,
			GL_FLOAT,
			&((*anIndexArray)[0]),
			anIndexArray->size(),
			GL_UNSIGNED_BYTE,
			aCreateVBOFlag,
			aTypeOfVBO);
}


//-------------------------------------------------------------------------------
void PolygonMesh::setExternalData(int aPolygonType,
								  const std::vector<double>* aVertexArray,
								  const std::vector<unsigned char>* anIndexArray,
								  bool aCreateVBOFlag,
								  int aTypeOfVBO)
//-------------------------------------------------------------------------------
{
	setExternalData(aPolygonType,
			&((*aVertexArray)[0]),
			aVertexArray->size() / 3,
			GL_DOUBLE,
			&((*anIndexArray)[0]),
			anIndexArray->size(),
			GL_UNSIGNED_BYTE,
			aCreateVBOFlag,
			aTypeOfVBO);
}


//---------------------------------------------------------------
void PolygonMesh::setExternalData(int aPolygonType,
								  const void* aVertexArray,
								  unsigned int aNumberOfVertices,
								  int aVertexDataType,
								  const void* anIndexArray,
								  unsigned int aNumberOfIndices,
								  int anIndexDataType,
								  bool aCreateVBOFlag,
								  int aTypeOfVBO)
//---------------------------------------------------------------
{
	reset();

	m_external_data_flag = true;
	m_polygon_type       = aPolygonType;
	m_p_vertex_set       = const_cast<void*>(aVertexArray);
	m_number_of_vertices = aNumberOfVertices;
	m_vertex_data_type   = aVertexDataType;

	// There are index data
	if (anIndexArray && anIndexDataType != GL_NONE && aNumberOfIndices != 0)
	{
		m_p_index_set       = const_cast<void*>(anIndexArray);
		m_number_of_indices = aNumberOfIndices;
		m_index_data_type   = anIndexDataType;
	}
	else
	{
		m_p_index_set       = 0;
		m_number_of_indices = 0;
		m_index_data_type   = GL_NONE;

	}

	// Compute the bounding box
	computeBoundingBox();

	// Compute the normal vectors
	computeNormalVectors();

	// Update the VBO
	if (aCreateVBOFlag)
	{
		updateVBO(aTypeOfVBO, m_polygon_type);
	}
}


//-----------------------------------
void PolygonMesh::destroyVertexData()
//-----------------------------------
{
	// The data is manage externally
	if (m_external_data_flag)
	{
		m_p_vertex_set = 0;
	}
	// The data is manage internally
	else
	{
		if (m_p_vertex_set)
		{
			// Cannot release the memory of an array whose type is void*
			switch (m_vertex_data_type)
			{
			case GL_FLOAT:
				delete [] reinterpret_cast<GLfloat*>(m_p_vertex_set);
				break;

			case GL_DOUBLE:
				delete [] reinterpret_cast<GLdouble*>(m_p_vertex_set);
				break;

			default:
				throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid data type.");
			}
			m_p_vertex_set = 0;
		}
	}
	m_number_of_vertices = 0;
}


//----------------------------------
void PolygonMesh::destroyIndexData()
//----------------------------------
{
	// The data is manage externally
	if (m_external_data_flag)
	{
		m_p_index_set  = 0;
	}
	// The data is manage internally
	else
	{
		if (m_p_index_set)
		{
			// Cannot release the memory of an array whose type is void*
			switch (m_index_data_type)
			{
			case GL_UNSIGNED_INT:
				delete [] reinterpret_cast<GLuint*>(m_p_index_set);
				break;

			case GL_UNSIGNED_SHORT:
				delete [] reinterpret_cast<GLushort*>(m_p_index_set);
				break;

			case GL_UNSIGNED_BYTE:
				delete [] reinterpret_cast<GLubyte*>(m_p_index_set);
				break;

			default:
				throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid data type.");
			}
			m_p_index_set = 0;
		}
	}
	m_number_of_indices = 0;
}


//-----------------------------------------------------
void PolygonMesh::removeIndex(bool aPrintDebugInfoFlag)
//-----------------------------------------------------
{
	// Split faces only if external data is not used
	if (!m_external_data_flag)
	{
		// It only works if an index has been set
		if (m_p_index_set && m_number_of_indices)
		{
			// The polygon is made of triangles
			if (m_polygon_type == GL_TRIANGLES)
			{
				// Reset the normal per face set
				m_p_vertex_normal_set.clear();

				// Reset the normal per face set
				m_p_face_normal_set.clear();

				// Store the new vertices
				std::vector<double> p_new_vertex_set;

				// Process every triangle
				unsigned int triangle_number(getTriangleNumber());
				for (unsigned int i(0); i < triangle_number; ++i)
				{
					// Get the current vertices
					int id_0(getIndex(i * 3 + 0));
					int id_1(getIndex(i * 3 + 1));
					int id_2(getIndex(i * 3 + 2));

					VEC3 vertex_0(getVertex(id_0));
					VEC3 vertex_1(getVertex(id_1));
					VEC3 vertex_2(getVertex(id_2));

					// Compute the normal vector
					VEC3 normal((vertex_2 - vertex_0)^(vertex_2 - vertex_1));
					normal.normalize();

					// Add the first triangle
					p_new_vertex_set.push_back(vertex_0.getX());
					p_new_vertex_set.push_back(vertex_0.getY());
					p_new_vertex_set.push_back(vertex_0.getZ());
					p_new_vertex_set.push_back(vertex_1.getX());
					p_new_vertex_set.push_back(vertex_1.getY());
					p_new_vertex_set.push_back(vertex_1.getZ());
					p_new_vertex_set.push_back(vertex_2.getX());
					p_new_vertex_set.push_back(vertex_2.getY());
					p_new_vertex_set.push_back(vertex_2.getZ());

					for (unsigned int j(0); j < 3; ++j)
					{
						m_p_face_normal_set.push_back(normal.getX());
						m_p_face_normal_set.push_back(normal.getY());
						m_p_face_normal_set.push_back(normal.getZ());
					}
				}

				// Set the index in the hash table and create the list of vertices
				destroyIndexData();
				destroyVertexData();
				m_number_of_vertices = p_new_vertex_set.size() / 3;

				GLfloat* p_float_vertex_set(0);
				GLdouble* p_double_vertex_set(0);
				switch (m_vertex_data_type)
				{
				case GL_FLOAT:
					m_p_vertex_set = new GLfloat[m_number_of_vertices * 3];
					p_float_vertex_set = reinterpret_cast<GLfloat*>(m_p_vertex_set);
					break;

				case GL_DOUBLE:
					m_p_vertex_set = new GLdouble[m_number_of_vertices * 3];
					p_double_vertex_set = reinterpret_cast<GLdouble*>(m_p_vertex_set);
					break;

				default:
					throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid data type.");
				}

				// Save the result
				for (unsigned int id(0); id < p_new_vertex_set.size(); ++id)
				{
					if (p_float_vertex_set)
					{
						p_float_vertex_set[id] = p_new_vertex_set[id];
					}
					if (p_double_vertex_set)
					{
						p_double_vertex_set[id] = p_new_vertex_set[id];
					}
				}

				// Update the VBO
				if (m_vbo.isReady())
				{
					updateVBO(m_vbo.getBufferUsageHint(), m_vbo.getPrimitiveType());
				}

				// Display extra debug info
				if (aPrintDebugInfoFlag)
				{
					std::cout << "file_name:\t" << m_filename <<
							"\tnb_faces:\t" << getFaceNumber() <<
							"\tnb_vertices:\t" << m_number_of_vertices <<
							"\tbounding_box (in cm):\t(" << m_min_corner.getX() / cm << ", " << m_min_corner.getY() / cm << ", " << m_min_corner.getZ() / cm << ")\t(" <<
													m_max_corner.getX() / cm << ", " << m_max_corner.getY() / cm << ", " << m_max_corner.getZ() / cm << ")" << std::endl;
				}
			}
			// Not supported
			else
			{
				throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented.");
			}
		}
	}
	// Generate an error
	else
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "The operation cannot be performed.");
	}
}
