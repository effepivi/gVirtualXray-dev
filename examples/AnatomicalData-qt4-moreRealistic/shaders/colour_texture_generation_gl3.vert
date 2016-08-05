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
in vec2 in_TexCoord;
in vec3 in_Normal;

//******************************************************************************
//	Output
//******************************************************************************

smooth out vec3 g_normal;
smooth out vec3 g_object_position;
smooth out vec2 g_texture_coordinates;










//-------------
void main(void)
//-------------
{
	g_object_position = in_Vertex;
	g_normal = in_Normal;
	g_texture_coordinates = in_TexCoord;

	gl_Position = vec4(in_TexCoord*2-1,0,1);
}

