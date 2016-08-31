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
	m_use_3D_bump_map = 1;
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
				m_number_of_vertices, 3, GL_FLOAT, &m_3D_texture_coordinates[0]);

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
	InternalOrganMesh::defineDisplayShaderInput(aShaderID);
}

//----------------------------------------------
void BoneMesh::initialize()
//----------------------------------------------
{
	//initialize if needed
	if(m_need_initialize)
	{
		//load the shaders
		load3DBumpShader();

	    //initialize the buffers for the bump map
	    initialize3DBuffer(m_bump_map_texture, m_bump_map_frame_buffer);

		if(m_number_of_indices)
		{
			initialize3DTextureCoordinates();
		}
		else
		{
			mergeVertices();

			initialize3DTextureCoordinates();

			updateVBO(m_vbo->getBufferUsageHint(), m_vbo->getPrimitiveType());
		}

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

	vertex_data.reserve(6);

	vertex_data.push_back(v1);
	vertex_data.push_back(v2);
	vertex_data.push_back(v3);

	vertex_data.push_back(v1);
	vertex_data.push_back(v3);
	vertex_data.push_back(v4);

	square_filler.setInternalData(GL_TRIANGLES, (std::vector<float>*)(&vertex_data), false, GL_STATIC_DRAW);

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

} // namespace gVirtualXRay
