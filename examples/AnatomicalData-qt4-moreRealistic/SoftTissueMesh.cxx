/**
********************************************************************************
*
*	@file		SoftTissueMesh.h
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

#include <cassert>

#include "SoftTissueMesh.h"

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#include "colour_texture_generation_gl3.vert.h"
#include "colour_texture_generation_gl3.frag.h"
#include "bump_map_generation_gl3.vert.h"
#include "bump_map_generation_gl3.frag.h"

//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {

//----------------------------------------------
SoftTissueMesh::SoftTissueMesh()
:
//----------------------------------------------
		InternalOrganMesh(),
		m_texturesAreGenerated(false),
		m_bumpMapTexture(0),
		m_bumpMapFrameBuffer(0),
		m_colourTexture(0),
		m_colourFrameBuffer(0)
//----------------------------------------------
{
	getMaterial().setShininess(100);
}

//----------------------------------------------
SoftTissueMesh::~SoftTissueMesh()
//----------------------------------------------
{
	// TODO Auto-generated destructor stub
}

//----------------------------------------------
bool SoftTissueMesh::isSoftTissue() const
//----------------------------------------------
{
	return true;
}

//----------------------------------------------
void SoftTissueMesh::updateVBO(int aTypeOfVBO, int aTypeOfPrimitive)
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

		m_textCoord.resize(0);
		//initialize the coordinate for sampling
		unsigned int n=int(sqrt(float(m_number_of_vertices))/6)+1;

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				//extract four coordinates
				VEC3 v1(0,float(i)/n, float(j)/n);
				VEC3 v2(0,float(i + 1)/n, float(j)/n);
				VEC3 v3(0,float(i + 1)/n, float(j + 1)/n);
				VEC3 v4(0,float(i)/n, float(j + 1)/n);

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

		m_vbo->setVertexData(aTypeOfPrimitive, m_number_of_vertices, 3, m_vertex_data_type, m_p_vertex_set,
				getVertexNormalNumber(), 3, GL_FLOAT, &m_p_vertex_normal_set[0],
				m_number_of_vertices, 3, GL_FLOAT, &m_textCoord[0]);


		if (m_p_index_set && m_number_of_indices && m_index_data_type != GL_NONE)
		{
			m_vbo->setIndexData(m_number_of_indices, m_index_data_type, m_p_index_set);
		}
	}
}

//----------------------------------------------
void SoftTissueMesh::defineShaderInput(GLint aShaderID)
//----------------------------------------------
{
	//Initialize if necessery
	if(!m_texturesAreGenerated)
	{
		generateTextures();
	}


	//Bind the texture for bump mapping in texture unit 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_bumpMapTexture);

	GLuint handle(glGetUniformLocation(aShaderID, "bumpMap"));
	glUniform1i(handle,1);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	//Bind the colour texture in texture unit 2
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_colourTexture);

	handle=glGetUniformLocation(aShaderID, "colourTexture");
	glUniform1i(handle,2);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	//All soft tissue meshes uses texturing
	GLint useTexture(1);
	handle = glGetUniformLocation(aShaderID, "g_useTexture");
	glUniform1iv(handle, 1, &useTexture);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	AnatomicalMesh::defineShaderInput(aShaderID);
}


//----------------------------------------------
void SoftTissueMesh::generateTextures()
//----------------------------------------------
{
	initialize();

	//render to the bump buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_bumpMapFrameBuffer);
	glViewport(0, 0, TEXTURE_SIZE, TEXTURE_SIZE);

	// Enable back face culling
	pushEnableDisableState(GL_CULL_FACE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Enable the bump shader
	pushShaderProgram();
	m_bumpMapShader.enable();

	defineBumpMapShaderInput();

/*
 * display();
 */

	// Disable the shader
	popShaderProgram();

	// Restore the attributes
    popEnableDisableState();









    //render to the colour buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_bumpMapFrameBuffer);
	glViewport(0, 0, TEXTURE_SIZE, TEXTURE_SIZE);

	// Enable back face culling
	pushEnableDisableState(GL_CULL_FACE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Enable the bump shader
	pushShaderProgram();
	m_textureShader.enable();

	defineColourShaderInput();

	/*
	 * display();
	 */


	// Disable the shader
	popShaderProgram();

	// Restore the attributes
    popEnableDisableState();






	// Cleanup for now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	m_texturesAreGenerated=true;
}

//----------------------------------------------
void SoftTissueMesh::defineBumpMapShaderInput()
//----------------------------------------------
{

}

//----------------------------------------------
void SoftTissueMesh::defineColourShaderInput()
//----------------------------------------------
{

}


//----------------------------------------------
void SoftTissueMesh::initialize()
//----------------------------------------------
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
    m_bumpMapShader.setLabels("bump.vert", "bump.frag");
    m_bumpMapShader.loadSource(vertex_shader, fragment_shader);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    z_lib_return_code_vertex   = inflate(g_colour_texture_generation_gl3_vert, sizeof(g_colour_texture_generation_gl3_vert),   &p_vertex_shader);
    z_lib_return_code_fragment = inflate(g_colour_texture_generation_gl3_frag, sizeof(g_colour_texture_generation_gl3_frag), &p_fragment_shader);

	//texture generation shader
    vertex_shader   = p_vertex_shader;
    fragment_shader = p_fragment_shader;
    delete [] p_vertex_shader;     p_vertex_shader = 0;
    delete [] p_fragment_shader; p_fragment_shader = 0;

    if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
    {
    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
    }
    m_textureShader.setLabels("colourTexture.vert", "colourTexture.frag");
    m_textureShader.loadSource(vertex_shader, fragment_shader);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    //initialize the buffers for the bump map and the colour texture
    initializeBuffer(m_bumpMapTexture, m_bumpMapFrameBuffer);
    initializeBuffer(m_colourTexture, m_colourFrameBuffer);
}

//----------------------------------------------
void SoftTissueMesh::initializeBuffer(GLuint aTexture, GLuint aFrameBuffer)
//----------------------------------------------
{
	//Intialize the buffer for the depth map texture and enable the sampling in the fragment shader as a sampler2DShadow
	glGenTextures(1, &aTexture);
	glBindTexture(GL_TEXTURE_2D, aTexture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGB, GL_FLOAT, 0);

	//Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//initalize the frame buffer for the shadow map
	glGenFramebuffers(1, &aFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, aFrameBuffer);

	//link the frame buffer and the texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, aTexture, 0);

	//Only depth
	glDrawBuffer(GL_NONE);

	//check the creation of the buffer
	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	// Cleanup for now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace gVirtualXRay

