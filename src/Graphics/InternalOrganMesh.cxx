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
#define _USE_MATH_DEFINES
#include <cmath>

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

#include "3d_bump_map_generation_gl3.vert.h"
#include "3d_bump_map_generation_gl3.frag.h"

//******************************************************************************
//	constant variables
//******************************************************************************
//size of the arrays to initialize perlin's noise
const int TAB_SIZE = 64;

//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {

//----------------------------------------
InternalOrganMesh::InternalOrganMesh(bool optimize_texture_coordinates,
		unsigned int texture_2D_size, unsigned int texture_3D_size):
//----------------------------------------
		AnatomicalMesh(),
		m_bump_map_texture(0),
		m_bump_map_frame_buffer(0),
		m_has_tendon(0),
		m_is_lung(0),
		m_has_thin_bumps(0),
		m_use_3D_bump_map(0),
		m_texture_2D_size(texture_2D_size),
		m_texture_3D_size(texture_3D_size),
		m_need_initialize(true),
		m_generation_texture_coord_are_set(false),
		m_optimize_texture_coordinates(optimize_texture_coordinates)
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
void InternalOrganMesh::defineDisplayShaderInput(int aShaderID)
//----------------------------------------------
{
	//initialize if needed
	if(m_need_initialize)
	{
		initialize();
	}

	GLuint handle(glGetUniformLocation(aShaderID, "g_use_3D_bump_map"));
	glUniform1iv(handle, 1, &m_use_3D_bump_map);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	if(m_use_3D_bump_map)
	{
		//Bind the texture for bump mapping in texture unit 3
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_3D, m_bump_map_texture);

		handle = glGetUniformLocation(aShaderID, "g_3D_bump_map");
		glUniform1i(handle,3);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		float repeate_frequency(4.f/float(m_texture_3D_size));

		handle = glGetUniformLocation(aShaderID, "g_repeate_frequency");
		glUniform1fv(handle, 1,&repeate_frequency);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
	else
	{
		//Bind the texture for bump mapping in texture unit 1
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_bump_map_texture);

		handle = glGetUniformLocation(aShaderID, "g_bump_map");
		glUniform1i(handle,1);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	AnatomicalMesh::defineDisplayShaderInput(aShaderID);
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
	glViewport(0, 0, m_texture_2D_size, m_texture_2D_size);

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

	handle = glGetUniformLocation(shader_id, "g_has_thin_bumps");
	glUniform1iv(handle, 1, &m_has_thin_bumps);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(shader_id, "g_perm");
	glUniform1iv(handle, TAB_SIZE,&m_permutation[0]);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(shader_id, "g_gradient_tab");
	glUniform3fv(handle, TAB_SIZE,(const GLfloat*) &m_gradient_tab[0]);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	if(m_use_3D_bump_map)
	{
		float bump_frequency(float(m_texture_3D_size)/4.f);

		handle = glGetUniformLocation(shader_id, "g_bump_frequency");
		glUniform1fv(handle, 1,&bump_frequency);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
}


//----------------------------------------------
void InternalOrganMesh::initialize2DBuffer(unsigned int& aTexture, unsigned int& aFrameBuffer)
//----------------------------------------------
{
	//Intialize the buffer for the texture
	glGenTextures(1, &aTexture);
	glBindTexture(GL_TEXTURE_2D, aTexture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB16, m_texture_2D_size, m_texture_2D_size, 0, GL_RGB, GL_FLOAT, 0);

	//Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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

//----------------------------------------------
void InternalOrganMesh::initialize3DBuffer(unsigned int& aTexture, unsigned int& aFrameBuffer)
//----------------------------------------------
{
	//Intialize the buffer for the texture
	glGenTextures(1, &aTexture);
	glBindTexture(GL_TEXTURE_3D, aTexture);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_R16, m_texture_3D_size, m_texture_3D_size, m_texture_3D_size, 0, GL_RED, GL_FLOAT, 0);

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

//----------------------------------------------
void InternalOrganMesh::load2DBumpShader()
//----------------------------------------------
{
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
}

//----------------------------------------------
void InternalOrganMesh::load3DBumpShader()
//----------------------------------------------
{
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
}

//----------------------------------------------
void InternalOrganMesh::initialize2DTextureCoordinates()
//----------------------------------------------
{
	unsigned int n(int(sqrt(float(m_number_of_vertices)/6.))+2);

	if(m_optimize_texture_coordinates)
	{
		std::cout<<"Optimize texture coordinates (" << m_number_of_vertices << " vertices)"<<std::endl;

		m_2D_texture_coordinates.resize(m_number_of_vertices);
		m_margin_direction.resize(m_number_of_vertices);
		std::vector<bool> has_texture_coordinates(m_number_of_vertices/3,false);
		unsigned int k(0);

		for (unsigned int i(0); i < n; i++) {
			for (unsigned int j(0); j < n; j++) {
				//extract four coordinates
				VEC2 v1(float(i)/n, float(j)/n);
				VEC2 v2(float(i + 1)/n, float(j)/n);
				VEC2 v3(float(i + 1)/n, float(j + 1)/n);
				VEC2 v4(float(i)/n, float(j + 1)/n);

				while(k<m_number_of_vertices/3 && has_texture_coordinates[k])
				{
					k++;
				}
				if(k<m_number_of_vertices/3)
				{
					unsigned int first_triangle_indice(3 * k);
					has_texture_coordinates[k] = true;
					k++;

					bool matching_face_not_found(true);
					unsigned int matching_face_indice(first_triangle_indice+3);
					VEC3 first_vertex(getVertex(first_triangle_indice));
					VEC3 second_vertex(getVertex(first_triangle_indice+1));
					VEC3 third_vertex(getVertex(first_triangle_indice+2));

					while(matching_face_not_found && matching_face_indice < m_number_of_vertices)
					{
						VEC3 first_matching_vertex(getVertex(matching_face_indice));
						VEC3 second_matching_vertex(getVertex(matching_face_indice+1));
						VEC3 third_matching_vertex(getVertex(matching_face_indice+2));

						if(!has_texture_coordinates[matching_face_indice/3])
						{
							if((third_matching_vertex==second_vertex)
									&& (first_matching_vertex==first_vertex))
								{
									//the first triangle
									m_2D_texture_coordinates[first_triangle_indice] = v3;
									m_2D_texture_coordinates[first_triangle_indice+1] = v1;
									m_2D_texture_coordinates[first_triangle_indice+2] = v2;

									//the second triangle
									m_2D_texture_coordinates[matching_face_indice] = v3;
									m_2D_texture_coordinates[matching_face_indice+1] = v4;
									m_2D_texture_coordinates[matching_face_indice+2] = v1;

									//Set the margin direction
									m_margin_direction[first_triangle_indice] = VEC2(-1., -1.);
									m_margin_direction[first_triangle_indice+1] = VEC2(1., 1.);
									m_margin_direction[first_triangle_indice+2] = VEC2(-1., 1.);

									m_margin_direction[matching_face_indice] = VEC2(-1., -1.);
									m_margin_direction[matching_face_indice+1] = VEC2(1., -1.);
									m_margin_direction[matching_face_indice+2] = VEC2(1., 1.);

									matching_face_not_found = false;
									has_texture_coordinates[matching_face_indice/3]=true;
								}
								else if((first_matching_vertex==second_vertex)
									&& (second_matching_vertex==first_vertex))
								{
									//the first triangle
									m_2D_texture_coordinates[first_triangle_indice] = v3;
									m_2D_texture_coordinates[first_triangle_indice+1] = v1;
									m_2D_texture_coordinates[first_triangle_indice+2] = v2;

									//the second triangle
									m_2D_texture_coordinates[matching_face_indice] = v1;
									m_2D_texture_coordinates[matching_face_indice+1] = v3;
									m_2D_texture_coordinates[matching_face_indice+2] = v4;

									//Set the margin direction
									m_margin_direction[first_triangle_indice] = VEC2(-1., -1.);
									m_margin_direction[first_triangle_indice+1] = VEC2(1., 1.);
									m_margin_direction[first_triangle_indice+2] = VEC2(-1., 1.);

									m_margin_direction[matching_face_indice] = VEC2(1., 1.);
									m_margin_direction[matching_face_indice+1] = VEC2(-1., -1.);
									m_margin_direction[matching_face_indice+2] = VEC2(1., -1.);

									matching_face_not_found = false;
									has_texture_coordinates[matching_face_indice/3]=true;
								}
								else if((second_matching_vertex == second_vertex)
									&& (third_matching_vertex == first_vertex))
								{
									//the first triangle
									m_2D_texture_coordinates[first_triangle_indice] = v3;
									m_2D_texture_coordinates[first_triangle_indice+1] = v1;
									m_2D_texture_coordinates[first_triangle_indice+2] = v2;

									//the second triangle
									m_2D_texture_coordinates[matching_face_indice] = v4;
									m_2D_texture_coordinates[matching_face_indice+1] = v1;
									m_2D_texture_coordinates[matching_face_indice+2] = v3;

									//Set the margin direction
									m_margin_direction[first_triangle_indice] = VEC2(-1., -1.);
									m_margin_direction[first_triangle_indice+1] = VEC2(1., 1.);
									m_margin_direction[first_triangle_indice+2] = VEC2(-1., 1.);

									m_margin_direction[matching_face_indice] = VEC2(1., -1.);
									m_margin_direction[matching_face_indice+1] = VEC2(1., 1.);
									m_margin_direction[matching_face_indice+2] = VEC2(-1., -1.);

									matching_face_not_found = false;
									has_texture_coordinates[matching_face_indice/3]=true;
								}
								else

								if((third_matching_vertex==third_vertex)
									&& (first_matching_vertex==second_vertex))
								{
									//the first triangle
									m_2D_texture_coordinates[first_triangle_indice] = v2;
									m_2D_texture_coordinates[first_triangle_indice+1] = v3;
									m_2D_texture_coordinates[first_triangle_indice+2] = v1;

									//the second triangle
									m_2D_texture_coordinates[matching_face_indice] = v3;
									m_2D_texture_coordinates[matching_face_indice+1] = v4;
									m_2D_texture_coordinates[matching_face_indice+2] = v1;

									//Set the margin direction
									m_margin_direction[first_triangle_indice] = VEC2(-1., 1.);
									m_margin_direction[first_triangle_indice+1] = VEC2(-1., -1.);
									m_margin_direction[first_triangle_indice+2] = VEC2(1., 1.);

									m_margin_direction[matching_face_indice] = VEC2(-1., -1.);
									m_margin_direction[matching_face_indice+1] = VEC2(1., -1.);
									m_margin_direction[matching_face_indice+2] = VEC2(1., 1.);

									matching_face_not_found = false;
									has_texture_coordinates[matching_face_indice/3]=true;
								}
								else if((first_matching_vertex==third_vertex)
									&& (second_matching_vertex==second_vertex))
								{
									//the first triangle
									m_2D_texture_coordinates[first_triangle_indice] = v2;
									m_2D_texture_coordinates[first_triangle_indice+1] = v3;
									m_2D_texture_coordinates[first_triangle_indice+2] = v1;

									//the second triangle
									m_2D_texture_coordinates[matching_face_indice] = v1;
									m_2D_texture_coordinates[matching_face_indice+1] = v3;
									m_2D_texture_coordinates[matching_face_indice+2] = v4;

									//Set the margin direction
									m_margin_direction[first_triangle_indice] = VEC2(-1., 1.);
									m_margin_direction[first_triangle_indice+1] = VEC2(-1., -1.);
									m_margin_direction[first_triangle_indice+2] = VEC2(1., 1.);

									m_margin_direction[matching_face_indice] = VEC2(1., 1.);
									m_margin_direction[matching_face_indice+1] = VEC2(-1., -1.);
									m_margin_direction[matching_face_indice+2] = VEC2(1., -1.);

									matching_face_not_found = false;
									has_texture_coordinates[matching_face_indice/3]=true;
								}
								else if((second_matching_vertex == third_vertex)
									&& (third_matching_vertex == second_vertex))
								{
									//the first triangle
									m_2D_texture_coordinates[first_triangle_indice] = v2;
									m_2D_texture_coordinates[first_triangle_indice+1] = v3;
									m_2D_texture_coordinates[first_triangle_indice+2] = v1;

									//the second triangle
									m_2D_texture_coordinates[matching_face_indice] = v4;
									m_2D_texture_coordinates[matching_face_indice+1] = v1;
									m_2D_texture_coordinates[matching_face_indice+2] = v3;

									//Set the margin direction
									m_margin_direction[first_triangle_indice] = VEC2(-1., 1.);
									m_margin_direction[first_triangle_indice+1] = VEC2(-1., -1.);
									m_margin_direction[first_triangle_indice+2] = VEC2(1., 1.);

									m_margin_direction[matching_face_indice] = VEC2(1., -1.);
									m_margin_direction[matching_face_indice+1] = VEC2(1., 1.);
									m_margin_direction[matching_face_indice+2] = VEC2(-1., -1.);

									matching_face_not_found = false;
									has_texture_coordinates[matching_face_indice/3]=true;
								}
								else

								if((third_matching_vertex==first_vertex)
									&& (first_matching_vertex==third_vertex))
								{
									//the first triangle
									m_2D_texture_coordinates[first_triangle_indice] = v1;
									m_2D_texture_coordinates[first_triangle_indice+1] = v2;
									m_2D_texture_coordinates[first_triangle_indice+2] = v3;

									//the second triangle
									m_2D_texture_coordinates[matching_face_indice] = v3;
									m_2D_texture_coordinates[matching_face_indice+1] = v4;
									m_2D_texture_coordinates[matching_face_indice+2] = v1;

									//Set the margin direction
									m_margin_direction[first_triangle_indice] = VEC2(1., 1.);
									m_margin_direction[first_triangle_indice+1] = VEC2(-1., 1.);
									m_margin_direction[first_triangle_indice+2] = VEC2(-1., -1.);

									m_margin_direction[matching_face_indice] = VEC2(-1., -1.);
									m_margin_direction[matching_face_indice+1] = VEC2(1., -1.);
									m_margin_direction[matching_face_indice+2] = VEC2(1., 1.);

									matching_face_not_found = false;
									has_texture_coordinates[matching_face_indice/3]=true;
								}
								else if((first_matching_vertex==first_vertex)
									&& (second_matching_vertex==third_vertex))
								{
									//the first triangle
									m_2D_texture_coordinates[first_triangle_indice] = v1;
									m_2D_texture_coordinates[first_triangle_indice+1] = v2;
									m_2D_texture_coordinates[first_triangle_indice+2] = v3;

									//the second triangle
									m_2D_texture_coordinates[matching_face_indice] = v1;
									m_2D_texture_coordinates[matching_face_indice+1] = v3;
									m_2D_texture_coordinates[matching_face_indice+2] = v4;

									//Set the margin direction
									m_margin_direction[first_triangle_indice] = VEC2(1., 1.);
									m_margin_direction[first_triangle_indice+1] = VEC2(-1., 1.);
									m_margin_direction[first_triangle_indice+2] = VEC2(-1., -1.);

									m_margin_direction[matching_face_indice] = VEC2(1., 1.);
									m_margin_direction[matching_face_indice+1] = VEC2(-1., -1.);
									m_margin_direction[matching_face_indice+2] = VEC2(1., -1.);

									matching_face_not_found = false;
									has_texture_coordinates[matching_face_indice/3]=true;
								}
								else if((second_matching_vertex == first_vertex)
									&& (third_matching_vertex == third_vertex))
								{
									//the first triangle
									m_2D_texture_coordinates[first_triangle_indice] = v1;
									m_2D_texture_coordinates[first_triangle_indice+1] = v2;
									m_2D_texture_coordinates[first_triangle_indice+2] = v3;

									//the second triangle
									m_2D_texture_coordinates[matching_face_indice] = v4;
									m_2D_texture_coordinates[matching_face_indice+1] = v1;
									m_2D_texture_coordinates[matching_face_indice+2] = v3;

									//Set the margin direction
									m_margin_direction[first_triangle_indice] = VEC2(1., 1.);
									m_margin_direction[first_triangle_indice+1] = VEC2(-1., 1.);
									m_margin_direction[first_triangle_indice+2] = VEC2(-1., -1.);

									m_margin_direction[matching_face_indice] = VEC2(1., -1.);
									m_margin_direction[matching_face_indice+1] = VEC2(1., 1.);
									m_margin_direction[matching_face_indice+2] = VEC2(-1., -1.);

									matching_face_not_found = false;
									has_texture_coordinates[matching_face_indice/3]=true;
								}
								else
								{
									matching_face_indice+=3;
								}
							}
							else
							{
								matching_face_indice+=3;
							}
					}
					//In case there is no matching triangle
					if(matching_face_indice >= m_number_of_vertices)
					{
						//only one triangle
						m_2D_texture_coordinates[first_triangle_indice] = v1;
						m_2D_texture_coordinates[first_triangle_indice+1] = v2;
						m_2D_texture_coordinates[first_triangle_indice+2] = v3;

						//Set the margin direction
						m_margin_direction[first_triangle_indice] = VEC2(2., 1.);
						m_margin_direction[first_triangle_indice+1] = VEC2(-1., 1.);
						m_margin_direction[first_triangle_indice+2] = VEC2(-1., -2.);
					}
				}
			}
		}
	}
	else
	{
		std::cout<<"Don't optimize texture coordinates (" << m_number_of_vertices << " vertices)"<<std::endl;

		m_2D_texture_coordinates.reserve(m_number_of_vertices);
		m_margin_direction.reserve(m_number_of_vertices);

		for (unsigned int i(0); i < n; i++) {
			for (unsigned int j(0); j < n; j++) {
				//extract four coordinates
				VEC2 v1(float(i)/n, float(j)/n);
				VEC2 v2(float(i + 1)/n, float(j)/n);
				VEC2 v3(float(i + 1)/n, float(j + 1)/n);
				VEC2 v4(float(i)/n, float(j + 1)/n);

				//the first triangle
				m_2D_texture_coordinates.push_back(v1);
				m_2D_texture_coordinates.push_back(v2);
				m_2D_texture_coordinates.push_back(v3);

				//the second triangle
				m_2D_texture_coordinates.push_back(v1);
				m_2D_texture_coordinates.push_back(v3);
				m_2D_texture_coordinates.push_back(v4);

				m_margin_direction.push_back(VEC2(1., 1.));
				m_margin_direction.push_back(VEC2(-1., 1.));
				m_margin_direction.push_back(VEC2(-1., -1.));

				m_margin_direction.push_back(VEC2(1., 1.));
				m_margin_direction.push_back(VEC2(-1., -1.));
				m_margin_direction.push_back(VEC2(1., -1.));
			}
		}

	}

}

//----------------------------------------------
void InternalOrganMesh::initialize3DTextureCoordinates()
//----------------------------------------------
{
	// Initialize the texture coordinates using the vertices position
    m_3D_texture_coordinates.reserve(m_number_of_vertices);

	for(unsigned int i(0); i < m_number_of_vertices; i++)
    {
    	m_3D_texture_coordinates.push_back(getVertex(i));
    }
}

#if 0
//----------------------------------------------
void InternalOrganMesh::addMargin(float margin)
//----------------------------------------------
{
	//Change the sampling coordinates to avoid artifacts at the boundary of the polygons
	unsigned int k(0);
	for (unsigned int i(0); i < n; i++) {
		for (unsigned int j(0); j < n; j++) {
			//the first triangle
			m_2D_texture_coordinates[k]+=VEC2(2*margin,margin);
			k++;
			m_2D_texture_coordinates[k]+=VEC2(-margin,margin);
			k++;
			m_2D_texture_coordinates[k]+=VEC2(-margin,-2*margin);
			k++;

			//the second triangle
			m_2D_texture_coordinates[k]+=VEC2(margin,2*margin);
			k++;
			m_2D_texture_coordinates[k]+=VEC2(-2*margin,-margin);
			k++;
			m_2D_texture_coordinates[k]+=VEC2(margin,-margin);
			k++;
		}
	}
}

/*unsigned int n(int(sqrt(float(m_number_of_vertices))/2.4));
m_2D_texture_coordinates.reserve(m_number_of_vertices);
m_margin_direction.reserve(m_number_of_vertices);

for (unsigned int i(0); i < n; i++) {
	for (unsigned int j(0); j < n; j++) {
		//extract four coordinates
		VEC2 v1(float(i)/n, float(j)/n);
		VEC2 v2(float(i + 1)/n, float(j)/n);
		VEC2 v3(float(i + 1)/n, float(j + 1)/n);
		VEC2 v4(float(i)/n, float(j + 1)/n);

		//the first triangle
		m_2D_texture_coordinates.push_back(v1);
		m_2D_texture_coordinates.push_back(v2);
		m_2D_texture_coordinates.push_back(v3);

		//the second triangle
		m_2D_texture_coordinates.push_back(v1);
		m_2D_texture_coordinates.push_back(v3);
		m_2D_texture_coordinates.push_back(v4);

		m_margin_direction.push_back(VEC2(1., 1.));
		m_margin_direction.push_back(VEC2(-1., 1.));
		m_margin_direction.push_back(VEC2(-1., -1.));

		m_margin_direction.push_back(VEC2(1., 1.));
		m_margin_direction.push_back(VEC2(-1., -1.));
		m_margin_direction.push_back(VEC2(1., -1.));
	}
}*/

#endif

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
void InternalOrganMesh::initGradientTab()
//----------------------------------------------
{
	float z, r, theta;

	for(unsigned int i(0); i<TAB_SIZE; i++)
	{
		z= 1 - 2*(float(rand()) / RAND_MAX);
		r= sqrt(1-z*z);
		theta=2 * M_PI * (float(rand()) / RAND_MAX);

		m_gradient_tab.push_back(VEC3(
				r*cos(theta),
				r*sin(theta),
				z
			));
	}
}

//----------------------------------------------
void InternalOrganMesh::initPermutation()
//----------------------------------------------
{
	for(unsigned int i(0); i<TAB_SIZE; i++)
	{

		//generate random number until we can add it to the array
		bool nothing_added(true);
		while(nothing_added)
		{

			//generate a random number between 0 and 255
			int random_number(rand()%TAB_SIZE);

			//if the random number isn't already in the array
			if(std::find(m_permutation.begin(), m_permutation.end(), random_number) == m_permutation.end())
			{
				//add it to the array
				m_permutation.push_back(random_number);
				nothing_added = false;
			}
		}
	}
}


} // namespace gVirtualXRay
