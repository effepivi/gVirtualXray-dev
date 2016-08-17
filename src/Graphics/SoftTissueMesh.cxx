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
#ifdef HAS_GLEW
#include "GL/glew.h"
#endif

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#include "gVirtualXRay/SoftTissueMesh.h"

#include "colour_texture_generation_gl3.vert.h"
#include "colour_texture_generation_gl3.frag.h"

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

//----------------------------------------------
SoftTissueMesh::SoftTissueMesh()
:
//----------------------------------------------
		InternalOrganMesh(),
		m_colour_texture(0),
		m_colour_texture_frame_buffer(0)
//----------------------------------------------
{
	getMaterial().setShininess(100);
	m_use_texture = 1;
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
void SoftTissueMesh::defineDisplayShaderInput(int aShaderID)
//----------------------------------------------
{
	InternalOrganMesh::defineDisplayShaderInput(aShaderID);

	//Bind the colour texture in texture unit 2
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_colour_texture);

	GLuint handle(glGetUniformLocation(aShaderID, "g_colour_texture"));
	glUniform1i(handle,2);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}

//----------------------------------------------
void SoftTissueMesh::initialize()
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

		//texture generation shader
	    z_lib_return_code_vertex   = inflate(g_colour_texture_generation_gl3_vert, sizeof(g_colour_texture_generation_gl3_vert),   &p_vertex_shader);
	    z_lib_return_code_fragment = inflate(g_colour_texture_generation_gl3_frag, sizeof(g_colour_texture_generation_gl3_frag), &p_fragment_shader);

	    vertex_shader   = p_vertex_shader;
	    fragment_shader = p_fragment_shader;
	    delete [] p_vertex_shader;     p_vertex_shader = 0;
	    delete [] p_fragment_shader; p_fragment_shader = 0;

	    if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
	    {
	    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
	    }
	    m_colour_texture_shader.setLabels("colourTexture.vert", "colourTexture.frag");
	    m_colour_texture_shader.loadSource(vertex_shader, fragment_shader);
	    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	    //initialize the buffers for the bump map and the colour texture
	    initializeBuffer(m_colour_texture, m_colour_texture_frame_buffer);
	}

    InternalOrganMesh::initialize();
}

//----------------------------------------------
void SoftTissueMesh::generateTextures()
//----------------------------------------------
{
	//generate the bump map
    InternalOrganMesh::generateTextures();

    //render to the colour buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_colour_texture_frame_buffer);
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
	m_colour_texture_shader.enable();

	GLint shader_id(m_colour_texture_shader.getProgramHandle());
	defineColourTextureShaderInput(shader_id);

	display();

	// Disable the shader
	popShaderProgram();

	// Restore the attributes
    popEnableDisableState();
    popEnableDisableState();
}

//----------------------------------------------
void SoftTissueMesh::defineBumpMapShaderInput(int shader_id)
//----------------------------------------------
{
	InternalOrganMesh::defineBumpMapShaderInput(shader_id);
}

//----------------------------------------------
void SoftTissueMesh::defineColourTextureShaderInput(int shader_id)
//----------------------------------------------
{
	GLuint handle(glGetUniformLocation(shader_id, "g_has_tendon"));
	glUniform1iv(handle, 1, &m_has_tendon);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(shader_id, "g_base_colour");
	glUniform4fv(handle, 1, &getMaterial().getDiffuseColour()[0]);
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
} // namespace gVirtualXRay

