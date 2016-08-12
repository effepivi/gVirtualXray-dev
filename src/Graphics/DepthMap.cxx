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

#ifdef HAS_GLEW
#include "GL/glew.h"
#endif

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#include "gVirtualXRay/DepthMap.h"

#include "depth_map_gl3.frag.h"
#include "depth_map_gl3.vert.h"

//******************************************************************************
//	constant variables
//******************************************************************************
//size of the side of the buffer
const int MAP_SIZE = 8192;

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

	//load the shader
    char* p_vertex_shader(0);
    char* p_fragment_shader(0);

    int z_lib_return_code_vertex(0);
    int z_lib_return_code_fragment(0);

    std::string vertex_shader;
    std::string fragment_shader;

    // Use OpenGL 3.x
    if (useOpenGL3_2OrAbove())
    {
        z_lib_return_code_vertex   = inflate(g_depth_map_gl3_vert, sizeof(g_depth_map_gl3_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_depth_map_gl3_frag, sizeof(g_depth_map_gl3_frag), &p_fragment_shader);
    }
    // Fall back to OpenGL 2.x
    else
    {
        throw "Not implemented";
        //z_lib_return_code_vertex   = inflate(g_depth_map_gl2_vert, sizeof(g_depth_map_gl2_vert),   &p_vertex_shader);
        //z_lib_return_code_fragment = inflate(g_depth_map_gl2_frag, sizeof(g_depth_map_gl2_frag), &p_fragment_shader);
    }


    vertex_shader   = p_vertex_shader;
    fragment_shader = p_fragment_shader;
    delete [] p_vertex_shader;     p_vertex_shader = 0;
    delete [] p_fragment_shader; p_fragment_shader = 0;

    if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
    {
    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
    }
    m_depth_shader.setLabels("depth.vert", "depth.frag");
    m_depth_shader.loadSource(vertex_shader, fragment_shader);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	m_isInitialized = true;
}

//------------------------------------------------------------------------
void DepthMap::render(std::vector<AnatomicalMesh*>& apMeshSet,
		Matrix4x4<GLfloat> const& projMatrix)
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
	m_depth_shader.enable();
	GLint shader_id(m_depth_shader.getProgramHandle());

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
