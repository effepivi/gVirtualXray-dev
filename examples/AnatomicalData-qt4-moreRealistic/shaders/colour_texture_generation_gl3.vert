/**
********************************************************************************
*
*	@file		colour_texture_generation_gl3.vert
*
*	@brief      vertex shader to create a texture.
*
*	@version	1.0
*
*	@date		01/08/2016
*
*	@author		Andréas Meuleman
*
********************************************************************************
*/



//******************************************************************************
//	OpenGL 3.2
//******************************************************************************
#version 150


//******************************************************************************
//	Input
//******************************************************************************
in vec3 in_Vertex;
in vec3 in_TexCoord;
in vec3 in_Normal;

//******************************************************************************
//	Output
//******************************************************************************

smooth out vec2 g_texture_coordinates;
smooth out vec3 g_normal;
smooth out vec3 g_view_direction;
smooth out vec4 g_shadowCoord;

//******************************************************************************
//	Uniform variables
//******************************************************************************
uniform mat4 g_projection_matrix;
uniform mat4 g_modelview_matrix;
uniform mat4 g_shadowMapMatrix;
uniform mat3 g_normal_matrix;

//-------------
void main(void)
//-------------
{
	vec4 g_object_position = g_modelview_matrix * vec4(in_Vertex, 1.0);
	
	g_normal = g_normal_matrix * in_Normal;
	g_view_direction = normalize(g_object_position.xyz);
	
    g_texture_coordinates = in_TexCoord.yz;

	//coordinates of the vertex for the shadow map
	g_shadowCoord = g_shadowMapMatrix * vec4(in_Vertex, 1.0);
	
	gl_Position = g_projection_matrix * g_object_position;
}

