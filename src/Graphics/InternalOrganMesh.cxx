/**
********************************************************************************
*
*	@file		InternalOrganMesh.cxx
*
*	@brief      Class to handle a polygon mesh for an internal organ.
*
*	@version	1.0
*
*	@date		18/07/2016
*
*	@author		Andréas Meuleman
*
********************************************************************************
*/

//******************************************************************************
//	Include
//******************************************************************************
#include <cstdlib>
#include <map>
#include <algorithm>

#ifdef HAS_GLEW
#include "GL/glew.h"
#endif

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#include "gVirtualXRay/InternalOrganMesh.h"

#include "bump_map_generation_gl3.vert.h"
#include "bump_map_generation_gl3.frag.h"

//******************************************************************************
//	constant variables
//******************************************************************************
//size of the arrays to initialize perlin's noise
const int TAB_SIZE = 64;

//size of the side of the buffer
const int TEXTURE_SIZE = 2048;

//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {

//----------------------------------------
InternalOrganMesh::InternalOrganMesh():
//----------------------------------------
		AnatomicalMesh(),
		m_bump_map_texture(0),
		m_bump_map_frame_buffer(0),
		m_has_tendon(0),
		m_is_lung(0),
		m_need_initialize(true)
//----------------------------------------
{
	//internal organs use bump mapping
	m_use_bump_map = 1;
}

//----------------------------------------
InternalOrganMesh::~InternalOrganMesh()
//----------------------------------------
{
	// TODO Auto-generated destructor stub
}

//----------------------------------------
bool InternalOrganMesh::isTransparent() const
//----------------------------------------
{
	return false;
}

//----------------------------------------------
void InternalOrganMesh::updateVBO(int aTypeOfVBO, int aTypeOfPrimitive)
//----------------------------------------------
{
	if (m_p_vertex_set && m_number_of_vertices)
	{
		// Create the VBO if needed
		if (!m_vbo.get())
		{
			m_vbo.reset(createVBO());
		}

		// Set the type of VBO
		m_vbo->setBufferUsageHint(aTypeOfVBO);

		m_vbo->setVertexData(aTypeOfPrimitive, m_number_of_vertices, 3, m_vertex_data_type, m_p_vertex_set,
				getVertexNormalNumber(), 3, GL_FLOAT, &m_p_vertex_normal_set[0],
				m_number_of_vertices, 2, GL_FLOAT, &m_textCoord[0]);
	}
}

//----------------------------------------------
void InternalOrganMesh::defineDisplayShaderInput(int aShaderID)
//----------------------------------------------
{
	//initialize if needed
	if(m_need_initialize)
	{
		initialize();
	}

  	//Bind the texture for bump mapping in texture unit 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_bump_map_texture);

	GLuint handle(glGetUniformLocation(aShaderID, "g_bump_map"));
	glUniform1i(handle,1);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	AnatomicalMesh::defineDisplayShaderInput(aShaderID);
}

//----------------------------------------------
void InternalOrganMesh::initialize()
//----------------------------------------------
{
	//initialize if needed
	if(m_need_initialize)
	{
		//load the shaders
	    char* p_vertex_shader(0);
	    char* p_fragment_shader(0);

	    int z_lib_return_code_vertex(0);
	    int z_lib_return_code_fragment(0);

	    std::string vertex_shader;
	    std::string fragment_shader;

	    //bump map generation shader
	    z_lib_return_code_vertex   = inflate(g_bump_map_generation_gl3_vert, sizeof(g_bump_map_generation_gl3_vert),   &p_vertex_shader);
	    z_lib_return_code_fragment = inflate(g_bump_map_generation_gl3_frag, sizeof(g_bump_map_generation_gl3_frag), &p_fragment_shader);


	    vertex_shader   = p_vertex_shader;
	    fragment_shader = p_fragment_shader;
	    delete [] p_vertex_shader;     p_vertex_shader = 0;
	    delete [] p_fragment_shader; p_fragment_shader = 0;

	    if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
	    {
	    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
	    }
	    m_bump_map_shader.setLabels("bump.vert", "bump.frag");
	    m_bump_map_shader.loadSource(vertex_shader, fragment_shader);
	    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	    //initialize the buffers for the bump map
	    initializeBuffer(m_bump_map_texture, m_bump_map_frame_buffer, false);

	    bool vbo_need_update(true);

	    //delete indices, there are not compatible with this implementation of solid texturing.
		if(m_number_of_indices)
		{
			deleteIndices();
			vbo_need_update = false;
		}
		shareNormalVectors();

		//initialize the coordinates for sampling
		unsigned int n(int(sqrt(float(m_number_of_vertices))/2.4)+1);

		m_textCoord.reserve(n);

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				//extract four coordinates
				VEC2 v1(float(i)/n, float(j)/n);
				VEC2 v2(float(i + 1)/n, float(j)/n);
				VEC2 v3(float(i + 1)/n, float(j + 1)/n);
				VEC2 v4(float(i)/n, float(j + 1)/n);

				//the first triangle
				m_textCoord.push_back(v1);
				m_textCoord.push_back(v2);
				m_textCoord.push_back(v3);

				//the second triangle
				m_textCoord.push_back(v1);
				m_textCoord.push_back(v3);
				m_textCoord.push_back(v4);
			}
		}

		if(vbo_need_update)
		{
			updateVBO(m_vbo->getBufferUsageHint(), m_vbo->getPrimitiveType());
		}

		generateTextures();

		//Change the sampling coordinates to avoid artifacts at the boundary of the polygons
		unsigned int k(0);
		const float STEP(float(0.6)/TEXTURE_SIZE);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				//the first triangle
				m_textCoord[k]+=VEC2(2*STEP,STEP);
				k++;
				m_textCoord[k]+=VEC2(-STEP,STEP);
				k++;
				m_textCoord[k]+=VEC2(-STEP,-2*STEP);
				k++;

				//the second triangle
				m_textCoord[k]+=VEC2(STEP,2*STEP);
				k++;
				m_textCoord[k]+=VEC2(-2*STEP,-STEP);
				k++;
				m_textCoord[k]+=VEC2(STEP,-STEP);
				k++;
			}
		}

		//Update the VBO
		updateVBO(m_vbo->getBufferUsageHint(), m_vbo->getPrimitiveType());

		m_need_initialize = false;
	}

    //delete indices, there are not compatible with this implementation of solid texturing.
	if(m_number_of_indices)
	{
		deleteIndices();

		shareNormalVectors();
	}
}


//----------------------------------------------
void InternalOrganMesh::splitFaces()
//----------------------------------------------
{
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented.");
}

//----------------------------------------------
void InternalOrganMesh::generateTextures()
//----------------------------------------------
{
	//render to the bump buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_bump_map_frame_buffer);
	glViewport(0, 0, TEXTURE_SIZE, TEXTURE_SIZE);

	// Enable back face culling
	pushEnableDisableState(GL_CULL_FACE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Enable the bump shader
	pushShaderProgram();
	m_bump_map_shader.enable();

	GLint shader_id(m_bump_map_shader.getProgramHandle());
	defineBumpMapShaderInput(shader_id);

 	display();

	// Disable the shader
	popShaderProgram();

	// Restore the attributes
    popEnableDisableState();

	// Cleanup for now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//----------------------------------------------
void InternalOrganMesh::defineBumpMapShaderInput(int shader_id)
//----------------------------------------------
{
	GLuint handle(glGetUniformLocation(shader_id, "g_has_tendon"));
	glUniform1iv(handle, 1, &m_has_tendon);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(shader_id, "g_is_lung");
	glUniform1iv(handle, 1, &m_is_lung);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(shader_id, "g_perm");
	glUniform1iv(handle, TAB_SIZE,&initPermutation()[0]);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(shader_id, "g_gradient_tab");
	glUniform3fv(handle, TAB_SIZE,(const GLfloat*) &initGradientTab()[0]);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//----------------------------------------------
void InternalOrganMesh::initializeBuffer(unsigned int& aTexture, unsigned int& aFrameBuffer, bool has_colour)
//----------------------------------------------
{
	//Intialize the buffer for the texture
	glGenTextures(1, &aTexture);
	glBindTexture(GL_TEXTURE_2D, aTexture);
	if(has_colour)
	{
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGB, GL_FLOAT, 0);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RED, GL_FLOAT, 0);
	}

	//Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//initalize the frame buffer
	glGenFramebuffers(1, &aFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, aFrameBuffer);

	//link the frame buffer and the texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, aTexture, 0);

	//check the creation of the buffer
	if(!(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE))
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create the frame buffer.");
	}

	// Cleanup for now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
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


//----------------------------------------------
void InternalOrganMesh::shareNormalVectors()
//----------------------------------------------
{
	// Add the vertices to the hash table
	std::map<VEC3, TempRecord, VectorComparer> hash_table;
	//Compute the vertices normal vectors (sum of the adjacent faces normal vectors)
	for (unsigned int i(0); i < m_number_of_vertices; ++i)
	{
		VEC3 current_vertex(getVertex(i));
		VEC3 normal(getFaceNormal(i));

		hash_table[current_vertex].m_normal += normal;
	}

	//set the vertices normal data
	m_p_vertex_normal_set.clear();
	m_p_vertex_normal_set.resize(m_number_of_vertices * 3);

	for (unsigned int i(0); i < m_number_of_vertices; ++i)
	{
		VEC3 current_vertex(getVertex(i));
		VEC3 current_normal(hash_table[current_vertex].m_normal);

		m_p_vertex_normal_set[i*3] =  current_normal.getX();
		m_p_vertex_normal_set[i*3+1] =  current_normal.getY();
		m_p_vertex_normal_set[i*3+2] =  current_normal.getZ();
	}

	m_p_face_normal_set.clear();

	normaliseNormals();
}

void InternalOrganMesh::deleteIndices()
{
	if(m_number_of_indices)
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

		computeNormalVectors();
	}
}

//----------------------------------------------
std::vector<VEC3> InternalOrganMesh::initGradientTab()
//----------------------------------------------
{
	std::vector<VEC3> tab;
	float z, r, theta;

	for(unsigned int i(0); i<TAB_SIZE; i++)
	{
		z= 1 - 2*(float(rand()) / RAND_MAX);
		r= sqrt(1-z*z);
		theta=2 * M_PI * (float(rand()) / RAND_MAX);

		tab.push_back(VEC3(
				r*cos(theta),
				r*sin(theta),
				z
			));
	}

	return tab;
}

//----------------------------------------------
std::vector<int> InternalOrganMesh::initPermutation()
//----------------------------------------------
{
	std::vector<int> tab;

	for(unsigned int i(0); i<TAB_SIZE; i++)
	{

		//generate random number until we can add it to the array
		bool nothing_added(true);
		while(nothing_added)
		{

			//generate a random number between 0 and 255
			int random_number(rand()%TAB_SIZE);

			//if the random number isn't already in the array
			if(std::find(tab.begin(), tab.end(), random_number) == tab.end())
			{
				//add it to the array
				tab.push_back(random_number);
				nothing_added = false;
			}
		}
	}
	return tab;
}


} // namespace gVirtualXRay
