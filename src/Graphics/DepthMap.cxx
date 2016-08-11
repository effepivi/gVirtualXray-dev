/**
*******************************************************************************
*
*  @file       DepthMap.cxx
*
*  @brief      Class to render to a buffer in order to create, for exemple, a shadow map.
*
*  @version    1.0
*
*  @date       19/07/2016
*
*  @author     Andréas Meuleman
*******************************************************************************
*/

//******************************************************************************
//	Include
//******************************************************************************
#include <iostream>
#include <fstream>

#include "gVirtualXRay/DepthMap.h"

//******************************************************************************
//	constant variables
//******************************************************************************
//size of the side of the buffer
static const int MAP_SIZE = 8192;

//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {

//------------------------------------------------------------------------
DepthMap::DepthMap() :
//------------------------------------------------------------------------
	m_isInitialized(false),
	m_mapFrameBuffer(0),
	m_mapTexture(0)
//------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------
DepthMap::~DepthMap()
//------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------
void DepthMap::initialize()
//------------------------------------------------------------------------
{
	//Intialize the buffer for the depth map texture and enable the sampling in the fragment shader as a sampler2DShadow
	glGenTextures(1, &m_mapTexture);
	glBindTexture(GL_TEXTURE_2D, m_mapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, MAP_SIZE, MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

	//initalize the frame buffer for the shadow map
	glGenFramebuffers(1, &m_mapFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_mapFrameBuffer);

	//link the frame buffer and the texture
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_mapTexture, 0);

	//Only depth
	glDrawBuffer(GL_NONE);

	//check the creation of the buffer
	if(!(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE))
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create the frame buffer.");
	}

	// Cleanup for now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	m_isInitialized = true;
}

//------------------------------------------------------------------------
void DepthMap::render(std::vector<AnatomicalMesh*>& apMeshSet,
		Matrix4x4<GLfloat> const& projMatrix, Shader& depthShader)
	//------------------------------------------------------------------------
{
	//Initialize if necessary
	if (!m_isInitialized) {
		initialize();
	}

	//render to the map buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_mapFrameBuffer);
	glViewport(0, 0, MAP_SIZE, MAP_SIZE);

	// Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable back face culling
	pushEnableDisableState(GL_CULL_FACE);
	pushEnableDisableState(GL_DEPTH_TEST);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

	// Enable the shader
	pushShaderProgram();
	depthShader.enable();
	GLint shader_id(depthShader.getProgramHandle());

	// Handle for shader variables
	GLuint handle(0);

	//link the atribute to their IDs
	handle = glGetUniformLocation(shader_id, "matrix");
	glUniformMatrix4fv(handle, 1,  GL_FALSE, projMatrix.get());
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	for (int i(0); i < apMeshSet.size(); ++i)
	{
		if(!(apMeshSet[i]->isTransparent()))
		{
			apMeshSet[i]->display();
		}
	}

	// Disable the shader
	popShaderProgram();

    // Restore the attributes
    popEnableDisableState();
    popEnableDisableState();

	// Cleanup for now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//------------------------------------------------------------------------
GLuint DepthMap::getMapTexture() const
//------------------------------------------------------------------------
{
	return m_mapTexture;
}


} // namespace gVirtualXRay
