/**
 ********************************************************************************
 *
 *	@file		display_gl3.vert
 *
 *	@brief		Vertex shader to display 3D objects with Phong shading.
 *
 *	@version	1.0
 *
 *	@date		31/12/2013
 *
 *	@author		Dr Franck P. Vidal
 *
 *	@section	License
 *				BSD 3-Clause License.
 *
 *				For details on use and redistribution please refer
 *				to http://opensource.org/licenses/BSD-3-Clause
 *
 *	@section	Copyright
 *				(c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
 *				http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
 *				BSD 3-Clause License
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
smooth out vec3 g_texture_coordinates;
smooth out vec3 g_normal;
smooth out vec4 g_object_position;


//******************************************************************************
//	Uniform variables
//******************************************************************************
uniform mat4 g_projection_matrix;
uniform mat4 g_modelview_matrix;
uniform mat3 g_normal_matrix;


//-------------
void main(void)
//-------------
{
	g_object_position = g_modelview_matrix * vec4(in_Vertex, 1.0);
	g_normal = normalize(g_normal_matrix * in_Normal);
	
    g_texture_coordinates = in_TexCoord;

	gl_Position = g_projection_matrix * g_object_position;
}

