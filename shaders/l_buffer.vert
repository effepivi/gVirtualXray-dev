/**
********************************************************************************
*
*	@file		l_buffer.vert
*
*	@brief		Vertex shader that computes a L-buffer.
*				see: N. Freud, P. Duvauchelle, J. M. Letang, D. Babot,
*				Fast and robust ray casting algorithms for virtual X-ray
*				imaging, Nuclear Instruments and Methods in Physics Research
*				Section B: Beam Interactions with Materials and Atoms 248 (1)
*				(2006) 175-180.
*
*          		F. P. Vidal, M. Garnier, N. Freud, J. M. Letang, and
*          		N. W. John. Simulation of X-ray attenuation on the GPU. In
*          		Proceedings of Theory and Practice of Computer Graphics 2009,
*          		pages 25Ð32, Cardiff, UK, June 2009. Eurographics Association.
*          		Winner of Ken Brodlie Prize for Best Paper.
*
*	@version	1.0
*
*	@date		15/12/2013
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
//	Varying variables
//******************************************************************************
varying vec3 g_untransformed_vertex;
varying vec3 g_viewing_vector;
varying vec3 g_t_norm;


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
	// Vertex in eye coordinate
	vec3 ecPosition = vec3(g_modelview_matrix * gl_Vertex);
	
	// Transformed normal
	g_t_norm = normalize(g_normal_matrix * gl_Normal);
	
	// Viewing vector
	g_viewing_vector = normalize(-ecPosition);
	
	// Untransform vertex position
	g_untransformed_vertex = gl_Vertex.xyz;

	// Compute the final position of the vertex
	gl_Position = g_projection_matrix * g_modelview_matrix * gl_Vertex;
}
