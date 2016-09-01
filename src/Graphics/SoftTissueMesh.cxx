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

//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {

//----------------------------------------------
SoftTissueMesh::SoftTissueMesh(bool optimize_texture_coordinates,
		unsigned int texture_2D_size, unsigned int texture_3D_size)
:
//----------------------------------------------
		InternalOrganMesh(optimize_texture_coordinates, texture_2D_size, texture_3D_size),
		m_colour_texture(0),
		m_colour_texture_frame_buffer(0),
		m_use_3D_colour_texture(0)
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

		if(m_use_3D_colour_texture || m_use_3D_bump_map)
		{
			m_vbo->setVertexData(aTypeOfPrimitive, m_number_of_vertices, 3, m_vertex_data_type, m_p_vertex_set,
					getVertexNormalNumber(), 3, GL_FLOAT, &m_p_vertex_normal_set[0],
					m_number_of_vertices, 2, GL_FLOAT, &m_2D_texture_coordinates[0],
					m_number_of_vertices, 3, GL_FLOAT, &m_3D_texture_coordinates[0]);
		}
		else
		{
			m_vbo->setVertexData(aTypeOfPrimitive, m_number_of_vertices, 3, m_vertex_data_type, m_p_vertex_set,
					getVertexNormalNumber(), 3, GL_FLOAT, &m_p_vertex_normal_set[0],
					m_number_of_vertices, 2, GL_FLOAT, &m_2D_texture_coordinates[0]);
		}
	}
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}

//----------------------------------------------
void SoftTissueMesh::defineDisplayShaderInput(int aShaderID)
//----------------------------------------------
{
	InternalOrganMesh::defineDisplayShaderInput(aShaderID);

	GLuint handle(glGetUniformLocation(aShaderID, "g_use_3D_colour_texture"));
	glUniform1iv(handle, 1, &m_use_3D_colour_texture);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	if(m_use_3D_colour_texture)
	{
		//Bind the colour texture in texture unit 4
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_3D, m_colour_texture);

		handle = glGetUniformLocation(aShaderID, "g_3D_colour_texture");
		glUniform1i(handle,4);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		float repeate_frequency(2.f/float(m_texture_3D_size));

		handle = glGetUniformLocation(aShaderID, "g_repeat_frequency");
		glUniform1fv(handle, 1,&repeate_frequency);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
	else
	{
		//Bind the colour texture in texture unit 2
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_colour_texture);
		glBindTexture(GL_TEXTURE_2D, m_colour_texture);


		handle = glGetUniformLocation(aShaderID, "g_colour_texture");
		glUniform1i(handle,2);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
}

//----------------------------------------------
void SoftTissueMesh::initialize()
//----------------------------------------------
{
	//initialize if needed
	if(m_need_initialize)
	{
		//load the shaders
		if(m_use_3D_bump_map)
		{
			load3DBumpShader();
		}
		else
		{
			load2DBumpShader();
		}

		//load the texture generation shader
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

		//initialize the buffers for the bump map
	    if(m_use_3D_bump_map)
	    {
		    initialize3DBuffer(m_bump_map_texture, m_bump_map_frame_buffer, false);
	    }
	    else
	    {
	    	initialize2DBuffer(m_bump_map_texture, m_bump_map_frame_buffer);
	    }

		//initialize the buffers for the colour texture
	    if(m_use_3D_colour_texture)
	    {
	    	initialize3DBuffer(m_colour_texture, m_colour_texture_frame_buffer, true);
	    }
	    else
	    {
	    	initialize2DBuffer(m_colour_texture, m_colour_texture_frame_buffer);
	    }

	    //delete indices, there are not compatible with this implementation of solid texturing.
		if(m_number_of_indices)
		{
			deleteIndices();
			std::cout<<"The indices has been deleted" << std::endl;

			shareNormalVectors();

			//initialize the coordinates for sampling
			initialize2DTextureCoordinates();

			if(m_use_3D_colour_texture || m_use_3D_bump_map)
			{
				initialize3DTextureCoordinates();
			}
		}
		else
		{
			//initialize the coordinates for sampling
			initialize2DTextureCoordinates();

			if(m_use_3D_colour_texture || m_use_3D_bump_map)
			{
				initialize3DTextureCoordinates();
			}

			shareNormalVectors();

			updateVBO(m_vbo->getBufferUsageHint(), m_vbo->getPrimitiveType());
		}

		initGradientTab();
		initPermutation();
		generateTextures();

		//Change the sampling coordinates to avoid artifacts at the boundary of the polygons
		for(unsigned int i(0); i<m_number_of_vertices; i++)
		{
			m_2D_texture_coordinates[i]+=m_margin_direction[i]*float(0.6/m_texture_2D_size);
		}

		//Update the VBO
		updateVBO(m_vbo->getBufferUsageHint(), m_vbo->getPrimitiveType());

		//Generate the texture again
		generateTextures();

		//Change the sampling coordinates to avoid artifacts at the boundary of the polygons
		for(unsigned int i(0); i<m_number_of_vertices; i++)
		{
			m_2D_texture_coordinates[i]+=m_margin_direction[i]*float(0.2/m_texture_2D_size);
		}

		//Update the VBO
		updateVBO(m_vbo->getBufferUsageHint(), m_vbo->getPrimitiveType());

		m_need_initialize = false;

		// Cleanup for now
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

    //delete indices, there are not compatible with this implementation of solid texturing.
	if(m_number_of_indices)
	{
		deleteIndices();

		shareNormalVectors();
	}
}

//----------------------------------------------
void SoftTissueMesh::generateTextures()
//----------------------------------------------
{
	if(m_use_3D_bump_map)
	{
		generate3DTexture(m_bump_map_shader, m_bump_map_frame_buffer, m_bump_map_texture, TO_CREATE_BUMP_MAP);
	}
	else
	{
		generate2DTexture(m_bump_map_shader, m_bump_map_frame_buffer, TO_CREATE_BUMP_MAP);
	}

	if(m_use_3D_colour_texture)
	{
		generate3DTexture(m_colour_texture_shader, m_colour_texture_frame_buffer, m_colour_texture, TO_CREATE_COLOUR_TEXTURE);
	}
	else
	{
		generate2DTexture(m_colour_texture_shader, m_colour_texture_frame_buffer, TO_CREATE_COLOUR_TEXTURE);
	}
}

//----------------------------------------------
void SoftTissueMesh::defineBumpMapShaderInput(int shader_id)
//----------------------------------------------
{
	if(m_use_3D_bump_map)
	{
		float bump_frequency(float(m_texture_3D_size)/2.f);

		GLuint handle = glGetUniformLocation(shader_id, "g_bump_frequency");
		glUniform1fv(handle, 1,&bump_frequency);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

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

	handle = glGetUniformLocation(shader_id, "g_use_3D_colour_texture");
	glUniform1iv(handle, 1, &m_use_3D_colour_texture);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(shader_id, "g_is_lung");
	glUniform1iv(handle, 1, &m_is_lung);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(shader_id, "g_perm");
	glUniform1iv(handle, TAB_SIZE,&m_permutation[0]);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(shader_id, "g_gradient_tab");
	glUniform3fv(handle, TAB_SIZE,(const GLfloat*) &m_gradient_tab[0]);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	float texture_frequency(1.);

	if(m_use_3D_colour_texture)
	{
		texture_frequency = float(m_texture_3D_size)/2.f;
	}

	handle = glGetUniformLocation(shader_id, "g_texture_frequency");
	glUniform1fv(handle, 1,&texture_frequency);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}
} // namespace gVirtualXRay

