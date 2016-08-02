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

#include <SoftTissueMesh.h>
#include <cassert>

namespace gVirtualXRay {

//----------------------------------------------
SoftTissueMesh::SoftTissueMesh()
:
//----------------------------------------------
		InternalOrganMesh(),
		m_textureIsGenerated(false),
		m_noiseTexture(0),
		m_noiseFrameBuffer(0),
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
		unsigned int m=int(sqrt(float(m_number_of_vertices))/6)+1;
		unsigned int n=int(sqrt(float(m_number_of_vertices))*10)+1;

		for (int i = 0; i < m; i++) {
			for (int j = 0; j < m; j++) {
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
	if(!m_textureIsGenerated)
	{
		generateAll();
	}


	//Bind the texture for bump mapping in texture unit 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_bumpMapTexture);

	GLuint handle(glGetUniformLocation(aShaderID, "bumpMap"));
	glUniform1i(handle,1);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	//Bind the texture unit 2
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_colourTexture);

	handle=glGetUniformLocation(aShaderID, "colourTexture");
	glUniform1i(handle,2);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);


	GLint useTexture(1);
	handle = glGetUniformLocation(aShaderID, "g_useTexture");
	glUniform1iv(handle, 1, &useTexture);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	AnatomicalMesh::defineShaderInput(aShaderID);
}


//----------------------------------------------
void SoftTissueMesh::generateAll()
//----------------------------------------------
{
	initialize();

	m_textureIsGenerated=true;
}

//----------------------------------------------
void SoftTissueMesh::generateNoiseTexture()
//----------------------------------------------
{

}

//----------------------------------------------
void SoftTissueMesh::generateBumpMapTexture()
//----------------------------------------------
{

}


//----------------------------------------------
void SoftTissueMesh::initialize()
//----------------------------------------------
{

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

