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


//-----------------------------------------
void TriangleMesh::displayWireframe() const
//-----------------------------------------
{
	// Chose the normal array
	const std::vector<GLfloat>* p_normal(0);

	// By default, chose the per vertex one
	if (m_p_vertex_normal_set.size())
	{
		p_normal = &m_p_vertex_normal_set;
	}
	else
	{
		p_normal = &m_p_face_normal_set;
	}

	// There is an index
	if (m_p_index_set.size())
	{
		for (int i(0); i < getTriangleNumber(); ++i)
		{
			const GLfloat* p_vector_0(&m_p_vertex_set[m_p_index_set[i * 3 + 0] * 3]);
			const GLfloat* p_vector_1(&m_p_vertex_set[m_p_index_set[i * 3 + 1] * 3]);
			const GLfloat* p_vector_2(&m_p_vertex_set[m_p_index_set[i * 3 + 2] * 3]);

			const GLfloat* p_normal_0(&(*p_normal)[m_p_index_set[i * 3 + 0] * 3]);
			const GLfloat* p_normal_1(&(*p_normal)[m_p_index_set[i * 3 + 1] * 3]);
			const GLfloat* p_normal_2(&(*p_normal)[m_p_index_set[i * 3 + 2] * 3]);

			glBegin(GL_LINE_LOOP);

			glNormal3fv(p_normal_0);
			glVertex3fv(p_vector_0);

			glNormal3fv(p_normal_1);
			glVertex3fv(p_vector_1);

			glNormal3fv(p_normal_2);
			glVertex3fv(p_vector_2);

			glEnd();
		}
	}
	// There is not any index
	else
	{
		for (int i(0); i < getTriangleNumber(); ++i)
		{
			const GLfloat* p_vector_0(&m_p_vertex_set[i * 9 + 0]);
			const GLfloat* p_vector_1(&m_p_vertex_set[i * 9 + 3]);
			const GLfloat* p_vector_2(&m_p_vertex_set[i * 9 + 6]);

			const GLfloat* p_normal_0(&(*p_normal)[i * 9 + 0]);
			const GLfloat* p_normal_1(&(*p_normal)[i * 9 + 3]);
			const GLfloat* p_normal_2(&(*p_normal)[i * 9 + 6]);

			glBegin(GL_LINES);

			glNormal3fv(p_normal_0);
			glVertex3fv(p_vector_0);

			glNormal3fv(p_normal_1);
			glVertex3fv(p_vector_1);

			glNormal3fv(p_normal_1);
			glVertex3fv(p_vector_1);

			glNormal3fv(p_normal_2);
			glVertex3fv(p_vector_2);

			glNormal3fv(p_normal_0);
			glVertex3fv(p_vector_0);

			glEnd();
		}
	}
}


//-------------------------------------------------------------
void TriangleMesh::displayFaceNormal(GLfloat aNormalSize) const
//-------------------------------------------------------------
{

	// There is an index
	if (m_p_index_set.size())
	{
		glBegin(GL_LINES);
		for (int i(0); i < m_p_index_set.size(); i += 3)
		{
			UINT id_0(m_p_index_set[i + 0]);
			UINT id_1(m_p_index_set[i + 1]);
			UINT id_2(m_p_index_set[i + 2]);

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
			for (int i(0); i < getVertexNumber(); i += 3)
			{
				UINT id_0(i + 0);
				UINT id_1(i + 1);
				UINT id_2(i + 2);

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
	m_p_face_normal_set.resize(triangle_number * 3 * 3);
	m_p_vertex_normal_set.resize(triangle_number * 3 * 3);
	
	// Read the data
	for (int i(0); i < getTriangleNumber(); i++)
	{
		// Read normals
		fread(&m_p_face_normal_set[9 * i], sizeof(float), 3, p_stream);

		// Copy one normal per vertex 
		if (!anAutoComputeNormalFlag)
		{
			for (int k=1; k<3; k++)
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
				m_p_face_normal_set[9 * i + 3 * k]     = normal.getX();
				m_p_face_normal_set[9 * i + 3 * k + 1] = normal.getY();
				m_p_face_normal_set[9 * i + 3 * k + 2] = normal.getZ();

				m_p_vertex_normal_set[9 * i + 3 * k]     = normal.getX();
				m_p_vertex_normal_set[9 * i + 3 * k + 1] = normal.getY();
				m_p_vertex_normal_set[9 * i + 3 * k + 2] = normal.getZ();
			}
		}

		// Skip data
		fseek(p_stream, 2, SEEK_CUR);
	}

	// Normalise all the normal vectors
	float* p_normals(&m_p_face_normal_set[0]);
	for (int i(0); i < getFaceNormalNumber(); i++, p_normals += 3)
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
		updateVBO();
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
	updateVBO();
}


//------------------------------------
void TriangleMesh::autoComputeNormal()
//------------------------------------
{
	m_p_face_normal_set.clear();
	m_p_vertex_normal_set.clear();
	m_p_face_normal_set.resize(m_p_vertex_set.size());
	m_p_vertex_normal_set.resize(m_p_vertex_set.size());

	// There is an an index, there will be no face normal
	if (m_p_index_set.size())
	{
		for (int i(0); i < m_p_index_set.size() / 3; i++)
		{
			VEC3 vec_A(m_p_vertex_set[9 * m_p_index_set[i * 3 + 0] + 0], m_p_vertex_set[9 * m_p_index_set[i * 3 + 0] + 1], m_p_vertex_set[9 * m_p_index_set[i * 3 + 0] + 2]);
			VEC3 vec_B(m_p_vertex_set[9 * m_p_index_set[i * 3 + 1] + 0], m_p_vertex_set[9 * m_p_index_set[i * 3 + 1] + 1], m_p_vertex_set[9 * m_p_index_set[i * 3 + 1] + 2]);
			VEC3 vec_C(m_p_vertex_set[9 * m_p_index_set[i * 3 + 2] + 0], m_p_vertex_set[9 * m_p_index_set[i * 3 + 2] + 1], m_p_vertex_set[9 * m_p_index_set[i * 3 + 2] + 2]);

			VEC3 normal( (vec_C - vec_A)^(vec_C - vec_B) );
			normal.normalize();

			for (int k(0); k < 3; k++)
			{
				m_p_face_normal_set[9 * i + 3 * k    ] = GLfloat(normal.getX());
				m_p_face_normal_set[9 * i + 3 * k + 1] = GLfloat(normal.getY());
				m_p_face_normal_set[9 * i + 3 * k + 2] = GLfloat(normal.getZ());
			}
		}
	}
	else
	{
		for (int i(0); i < getTriangleNumber(); i++)
		{
			VEC3 vec_A(m_p_vertex_set[9 * i     + 0], m_p_vertex_set[9 * i     + 1], m_p_vertex_set[9 * i     + 2]);
			VEC3 vec_B(m_p_vertex_set[9 * i + 3 + 0], m_p_vertex_set[9 * i + 3 + 1], m_p_vertex_set[9 * i + 3 + 2]);
			VEC3 vec_C(m_p_vertex_set[9 * i + 6 + 0], m_p_vertex_set[9 * i + 6 + 1], m_p_vertex_set[9 * i + 6 + 2]);

			VEC3 normal( (vec_C - vec_B)^(vec_C - vec_A) );
			normal.normalize();

			for (int k(0); k < 3; k++)
			{
				m_p_face_normal_set[9 * i + 3 * k    ] = GLfloat(normal.getX());
				m_p_face_normal_set[9 * i + 3 * k + 1] = GLfloat(normal.getY());
				m_p_face_normal_set[9 * i + 3 * k + 2] = GLfloat(normal.getZ());

				m_p_vertex_normal_set[9 * i + 3 * k    ] = GLfloat(normal.getX());
				m_p_vertex_normal_set[9 * i + 3 * k + 1] = GLfloat(normal.getY());
				m_p_vertex_normal_set[9 * i + 3 * k + 2] = GLfloat(normal.getZ());
			}
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
	updateVBO();
}


struct VectorComparer
{
	// Return true if aLeftNode is less than aRightNode
    bool operator()(const VEC3 & aLeftNode, const VEC3 & aRightNode) const
    {
    	double epsilon(0.001);

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


	inline const TempRecord& operator =(UINT anID)
	{
		m_id = anID;

		return (*this);
	}

	VEC3 m_normal;
	UINT m_id;
};


//--------------------------------
void TriangleMesh::mergeVertices()
//--------------------------------
{
	// It only works if no index has been set
	if (!m_p_index_set.size())
	{
		// Allocate the memory of the index
		m_p_index_set.resize(getTriangleNumber() * 3);

		std::map<VEC3, TempRecord, VectorComparer> hash_table;

		// Add the vertices to the hash table
		UINT number_of_vertices(getVertexNumber());
		for (int i(0); i < number_of_vertices; ++i)
		{
			VEC3 current_vertex(getVertex(i));
			VEC3 normal(getFaceNormal(i));

			hash_table[current_vertex].m_normal += normal;
		}

		m_p_vertex_normal_set.clear();
		m_p_vertex_normal_set.resize(hash_table.size() * 3);

		int id(0);
		for (std::map<VEC3, TempRecord>::iterator ite(hash_table.begin());
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
		for (std::map<VEC3, TempRecord>::iterator ite(hash_table.begin());
				ite != hash_table.end();
				++ite, ++id)
		{
			ite->second.m_id = id;

			new_vertex_set[id * 3 + 0] = ite->first.getX();
			new_vertex_set[id * 3 + 1] = ite->first.getY();
			new_vertex_set[id * 3 + 2] = ite->first.getZ();
		}

		// Fill the index
		UINT number_of_triangles(getTriangleNumber());
		for (int i(0); i < number_of_triangles; ++i)
		{
			VEC3 vertex_0(getVertex(i * 3 + 0));
			VEC3 vertex_1(getVertex(i * 3 + 1));
			VEC3 vertex_2(getVertex(i * 3 + 2));

			UINT id_0(hash_table.find(vertex_0)->second.m_id);
			UINT id_1(hash_table.find(vertex_1)->second.m_id);
			UINT id_2(hash_table.find(vertex_2)->second.m_id);

			if (id_0 >= hash_table.size() || id_1 >= hash_table.size() || id_2 >= hash_table.size())
			{
				throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid vertex index");
			}

			m_p_index_set[i * 3 + 0] = id_0;
			m_p_index_set[i * 3 + 1] = id_1;
			m_p_index_set[i * 3 + 2] = id_2;
		}

		// Normalise the normals in the normal set
		UINT number_of_normals(getVertexNormalNumber());
		for (int i(0); i < number_of_normals; ++i)
		{
			VEC3 normal(getVertexNormal(i));
			normal.normalise();
			m_p_vertex_normal_set[i * 3 + 0] += normal.getX();
			m_p_vertex_normal_set[i * 3 + 1] += normal.getY();
			m_p_vertex_normal_set[i * 3 + 2] += normal.getZ();
		}

		// Replace the list of vertices
		m_p_vertex_set = new_vertex_set;

		// Reset the normal per face set
		m_p_face_normal_set.clear();

		// Update the VBO
		updateVBO();
	}
}


//-----------------------------------
inline void TriangleMesh::updateVBO()
//-----------------------------------
{
	if (getVertexNormalNumber())
	{
		m_vbo.setVertexData(GL_TRIANGLES, getVertexNumber(), 3, &m_p_vertex_set[0],
				getVertexNormalNumber(), 3, &m_p_vertex_normal_set[0],
				0, 0, 0);
	}
	else if (getFaceNormalNumber())
	{
		m_vbo.setVertexData(GL_TRIANGLES, getVertexNumber(), 3, &m_p_vertex_set[0],
				getFaceNormalNumber(), 3, &m_p_face_normal_set[0],
				0, 0, 0);
	}
	else
	{
		m_vbo.setVertexData(GL_TRIANGLES, getVertexNumber(), 3, &m_p_vertex_set[0],
				0, 0, 0,
				0, 0, 0);
	}

	if (m_p_index_set.size())
	{
		m_vbo.setIndexData(m_p_index_set.size(), &m_p_index_set[0]);
	}
}
