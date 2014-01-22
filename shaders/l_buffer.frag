/**
********************************************************************************
*
*	@file		l_buffer.frag
*
*	@brief		Fragment shader that computes a L-buffer.
*				see: N. Freud, P. Duvauchelle, J. M. Letang, D. Babot,
*				Fast and robust ray casting algorithms for virtual X-ray
*				imaging, Nuclear Instruments and Methods in Physics Research
*				Section B: Beam Interactions with Materials and Atoms 248 (1)
*				(2006) 175-180.
*
*          		F. P. Vidal, M. Garnier, N. Freud, J. M. Letang, and
*          		N. W. John. Simulation of X-ray attenuation on the GPU. In
*          		Proceedings of Theory and Practice of Computer Graphics 2009,
*         		pages 25Ð32, Cardiff, UK, June 2009. Eurographics Association.
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
uniform float g_to_cm_scale;
uniform vec3  g_third_column;


//--------------
void main (void)
//--------------
{
	// Distance source intersection
	float dist = distance(g_untransformed_vertex, g_third_column);

	// Sign of the dot product
	float dot_product_sign = sign(dot(g_t_norm, g_viewing_vector));

	// In the red channel: the L-buffer value
	// In the green channel the sign of the dot product (useful to detect errors in the final L-buffer)
	// In the blue channel: 0.0 (unused)
	// In the alpha channel: 1.0 (unused)
	gl_FragColor = vec4(-dot_product_sign * dist * g_to_cm_scale, 100.0 * dot_product_sign, 0.0, 1.0);
}
