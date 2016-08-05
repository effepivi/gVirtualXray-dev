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
#include <cstdlib>

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
		m_bumpMapTexture(0),
		m_bumpMapFrameBuffer(0),
		m_colourTexture(0),
		m_colourFrameBuffer(0)
//----------------------------------------------
{
	getMaterial().setShininess(100);
	m_textCoord.reserve(m_number_of_vertices);
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

		m_textCoord.reserve(m_number_of_vertices);

		//initialize the coordinates for sampling
		unsigned int n(int(sqrt(float(m_number_of_vertices))/2.4)+1);
		unsigned int k(0);

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				//extract four coordinates
				VEC2 v1(float(i)/n, float(j)/n);
				VEC2 v2(float(i + 1)/n, float(j)/n);
				VEC2 v3(float(i + 1)/n, float(j + 1)/n);
				VEC2 v4(float(i)/n, float(j + 1)/n);

				/*//the first triangle
				m_textCoord[getIndex(k)] = v1;
				k++;
				m_textCoord[getIndex(k)] = v2;
				k++;
				m_textCoord[getIndex(k)] = v3;
				k++;

				//the second triangle
				m_textCoord[getIndex(k)] = v1;
				k++;
				m_textCoord[getIndex(k)] = v3;
				k++;
				m_textCoord[getIndex(k)] = v4;
				k++;*/

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
				m_number_of_vertices, 2, GL_FLOAT, &m_textCoord[0]);
	}
	if (m_p_index_set && m_number_of_indices && m_index_data_type != GL_NONE)
		{
			m_vbo->setIndexData(m_number_of_indices, m_index_data_type, m_p_index_set);
		}
}

//----------------------------------------------
void SoftTissueMesh::defineShaderInput(GLint aShaderID)
//----------------------------------------------
{
   /* //render to the colour buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable back face culling
	pushEnableDisableState(GL_CULL_FACE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);


	// Enable the bump shader
	pushShaderProgram();
	m_textureShader.enable();

	defineColourShaderInput();

	display();

	// Disable the shader
	popShaderProgram();

	// Restore the attributes
    popEnableDisableState();*/






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
void SoftTissueMesh::splitFaces()
//----------------------------------------------
{
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented.");
}

//----------------------------------------------
void SoftTissueMesh::generateTextures()
//----------------------------------------------
{
	initialize();

	//render to the bump buffer
	/*glBindFramebuffer(GL_FRAMEBUFFER, m_bumpMapFrameBuffer);
	glViewport(0, 0, TEXTURE_SIZE, TEXTURE_SIZE);

	// Enable back face culling
	pushEnableDisableState(GL_CULL_FACE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Enable the bump shader
	pushShaderProgram();
	m_bumpMapShader.enable();

	defineBumpMapShaderInput();


 	 display();


	// Disable the shader
	popShaderProgram();

	// Restore the attributes
    popEnableDisableState();*/









    //render to the colour buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_colourFrameBuffer);
	glViewport(0, 0, TEXTURE_SIZE, TEXTURE_SIZE);

	// Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable back face culling
	pushEnableDisableState(GL_CULL_FACE);
	pushEnableDisableState(GL_DEPTH_TEST);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);


	// Enable the bump shader
	pushShaderProgram();
	m_textureShader.enable();

	defineColourShaderInput();

	display();



	// Disable the shader
	popShaderProgram();

	// Restore the attributes
    popEnableDisableState();
    popEnableDisableState();

	// Cleanup for now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
	GLint textureShaderID(m_textureShader.getProgramHandle());
	GLuint handle(glGetUniformLocation(textureShaderID, "base_colour"));
	glUniform4fv(handle, 1, &getMaterial().getDiffuseColour()[0]);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	//handle = glGetUniformLocation(textureShaderID, "gradientTab");

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
void SoftTissueMesh::initializeBuffer(GLuint& aTexture, GLuint& aFrameBuffer)
//----------------------------------------------
{
	//Intialize the buffer for the texture
	glGenTextures(1, &aTexture);
	glBindTexture(GL_TEXTURE_2D, aTexture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGB, GL_FLOAT, 0);

	//Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

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

//----------------------------------------------
std::vector<VEC3> SoftTissueMesh::gradientTabInit()
//----------------------------------------------
{
	std::vector<VEC3> table;
	float z, r, theta;

	for(unsigned int i(0); i<m_number_of_vertices; i++)
	{
		z= 1 - 2*(float(rand()) / RAND_MAX);
		r= sqrt(1-z*z);
		theta=2 * M_PI * (float(rand()) / RAND_MAX);

		table.push_back(VEC3(
				r*cos(theta),
				r*sin(theta),
				z
			));
	}

	return table;
}

} // namespace gVirtualXRay

