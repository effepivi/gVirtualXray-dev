/**
********************************************************************************
*
*	@file		BoneMesh.cxx
*
*	@brief      Class to handle a polygon mesh for Bone.
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
#include "gVirtualXRay/BoneMesh.h"

#ifdef HAS_GLEW
#include "GL/glew.h"
#endif

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#include "3d_bump_map_generation_gl3.vert.h"
#include "3d_bump_map_generation_gl3.frag.h"

//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {

//----------------------------------------------
BoneMesh::BoneMesh(unsigned int texture_size):
//----------------------------------------------
		InternalOrganMesh(false, texture_size)
//----------------------------------------------
{
	m_has_thin_bumps = 1;
	getMaterial().setShininess(4);
	getMaterial().setSpecularColour(0.2,0.2,0.2,1.);
	getMaterial().setDiffuseColour(0.84,0.74,0.6,1.);
	getMaterial().setAmbientColour(0.84,0.74,0.6,1.);
}

//----------------------------------------
BoneMesh::~BoneMesh()
//----------------------------------------
{
	// TODO Auto-generated destructor stub
}

//----------------------------------------
bool BoneMesh::isSoftTissue() const
//----------------------------------------
{
	return false;
}

//----------------------------------------------
void BoneMesh::updateVBO(int aTypeOfVBO, int aTypeOfPrimitive)
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
				0, 0, GL_NONE, 0);

        if (m_p_index_set && m_number_of_indices &&
                m_index_data_type != GL_NONE)
            {
            m_vbo->setIndexData(m_number_of_indices,
                    m_index_data_type,
                    m_p_index_set);
            }
	}
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}

//----------------------------------------------
void BoneMesh::defineDisplayShaderInput(int aShaderID)
//----------------------------------------------
{
	//initialize if needed
	if(m_need_initialize)
	{
		initialize();
	}

	//Bind the texture for bump mapping in texture unit 3
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_3D, m_bump_map_texture);

	GLuint handle(glGetUniformLocation(aShaderID, "g_3D_bump_map"));
	glUniform1i(handle,3);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	int use_3D_bump_map(1);
	handle = glGetUniformLocation(aShaderID, "g_use_3D_bump_map");
	glUniform1iv(handle, 1, &use_3D_bump_map);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);


	AnatomicalMesh::defineDisplayShaderInput(aShaderID);
}

//----------------------------------------------
void BoneMesh::initialize()
//----------------------------------------------
{
	//initialize if needed
	if(m_need_initialize)
	{
		mergeVertices();

		//load the shaders
	    char* p_vertex_shader(0);
	    char* p_fragment_shader(0);

	    int z_lib_return_code_vertex(0);
	    int z_lib_return_code_fragment(0);

	    std::string vertex_shader;
	    std::string fragment_shader;

	    //bump map generation shader
	    z_lib_return_code_vertex   = inflate(g_3d_bump_map_generation_gl3_vert, sizeof(g_3d_bump_map_generation_gl3_vert),   &p_vertex_shader);
	    z_lib_return_code_fragment = inflate(g_3d_bump_map_generation_gl3_frag, sizeof(g_3d_bump_map_generation_gl3_frag), &p_fragment_shader);


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
	    initializeBuffer(m_bump_map_texture, m_bump_map_frame_buffer);

	    initGradientTab();
		initPermutation();
		generateTextures();

		m_need_initialize = false;

		// Cleanup for now
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

//----------------------------------------------
void BoneMesh::generateTextures()
//----------------------------------------------
{
	PolygonMesh square_filler;
	std::vector<VEC3 > vertex_data;

	VEC3 v1(0, 0, 0);
	VEC3 v2(1, 0, 0);
	VEC3 v3(1, 1, 0);
	VEC3 v4(0, 1, 0);

	vertex_data.push_back(v1);
	vertex_data.push_back(v2);
	vertex_data.push_back(v3);

	vertex_data.push_back(v1);
	vertex_data.push_back(v3);
	vertex_data.push_back(v4);

	square_filler.setInternalData(m_vbo->getPrimitiveType(), (std::vector<float>*)(&vertex_data), false, m_vbo->getBufferUsageHint());

	//render to the bump buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_bump_map_frame_buffer);
	glViewport(0, 0, m_texture_size, m_texture_size);

	// Disable back face culling
	pushEnableDisableState(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);

	// Enable the bump shader
	pushShaderProgram();
	m_bump_map_shader.enable();

	GLint shader_id(m_bump_map_shader.getProgramHandle());
	defineBumpMapShaderInput(shader_id);

	for(unsigned int i(0); i < m_texture_size; i++)
	{
		//link the frame buffer and the texture
		glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_3D, m_bump_map_texture, 0, i);

		//check the creation of the buffer
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create the frame buffer.");
		}

		square_filler.display();

		square_filler.translate(VEC3(0,0,1.f/m_texture_size));
	}

	// Disable the shader
	popShaderProgram();

	// Restore the attributes
    popEnableDisableState();
}

//----------------------------------------------
void BoneMesh::initializeBuffer(unsigned int& aTexture, unsigned int& aFrameBuffer)
//----------------------------------------------
{
	//Intialize the buffer for the texture
	glGenTextures(1, &aTexture);
	glBindTexture(GL_TEXTURE_3D, aTexture);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_R16, m_texture_size, m_texture_size, m_texture_size, 0, GL_RED, GL_FLOAT, 0);

	//Filtering
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//initalize the frame buffer
	glGenFramebuffers(1, &aFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, aFrameBuffer);

	//link the frame buffer and the texture
	glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_3D, aTexture, 0, 0);

	//check the creation of the buffer
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create the frame buffer.");
	}

	// Cleanup for now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_3D, 0);
}

} // namespace gVirtualXRay
