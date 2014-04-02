/**
 ********************************************************************************
 *
 *	@file		display.frag
 *
 *	@brief		Fragment shader to display 3D objects with Phong shading.
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
//	OpenGL 2.1
//******************************************************************************
#version 120


//******************************************************************************
//	Uniform variables
//******************************************************************************
uniform mat4 g_projection_matrix;
uniform mat4 g_modelview_matrix;
uniform mat3 g_normal_matrix;


//******************************************************************************
//	Varying variables
//******************************************************************************
varying vec3 g_normal;
varying vec4 g_object_position;


//-------------
void main(void)
//-------------
{
	g_object_position = g_modelview_matrix * gl_Vertex;
	g_normal = normalize(g_normal_matrix * gl_Normal);
	
    gl_TexCoord[0] = gl_MultiTexCoord0;

    gl_FrontColor = gl_Color;
	gl_Position = g_projection_matrix * g_modelview_matrix * gl_Vertex;
}
