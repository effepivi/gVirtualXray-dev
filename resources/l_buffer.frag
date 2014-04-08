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
*         		pages 25-32, Cardiff, UK, June 2009. Eurographics Association.
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
//	OpenGL 3.2
//******************************************************************************
#version 150
precision highp float; // Video card drivers require this line to function properly


//******************************************************************************
//	Input
//******************************************************************************
smooth in float g_distance_intersection_detector;
smooth in vec3 g_viewing_vector;
smooth in vec4 g_normal_0;
smooth in vec4 g_normal_1;


//******************************************************************************
//	Output
//******************************************************************************
out vec4 fragColor;


//******************************************************************************
//   Defines
//******************************************************************************
#define EPSILON         0.000001


//******************************************************************************
//	Uniform variables
//******************************************************************************
uniform float g_to_cm_scale;
uniform mat4 g_modelling_matrix;


//--------------
void main (void)
//--------------
{
	// Transformed normal
	vec4 normal_0 = g_modelling_matrix * g_normal_0;
	vec4 normal_1 = g_modelling_matrix * g_normal_1;
	vec3 viewing_vector = g_viewing_vector;

	normal_0.xyz /= normal_0.w;
	normal_0.w = 1.0;		
	normal_0.xyz = normalize(normal_0.xyz);

	normal_1.xyz /= normal_1.w;
	normal_1.w = 1.0;		
	normal_1.xyz = normalize(normal_1.xyz);

	// Save the dot product
	float dot_product_sign = sign(dot(normal_0.xyz, viewing_vector));
/*	if (abs(dot_product_sign) < 0.5)
	{
		normal_0.x += 0.1;
		normal_0.y += 0.1;
		normal_0.xyz = normalize(normal_0.xyz);

		viewing_vector.x += 0.1;
		viewing_vector.y += 0.1;
		viewing_vector.xyz = normalize(viewing_vector);

		dot_product_sign = sign(dot(normal_0.xyz, viewing_vector));
	}
	*/
	float value = -dot_product_sign * g_distance_intersection_detector * g_to_cm_scale;
	//if (g_distance_intersection_detector < 0.0) value = 0.0;
	
	// In the red channel: the L-buffer value
	// In the green channel the sign of the dot product (useful to detect errors in the final L-buffer)
	// In the blue channel: 0.0 (unused)
	// In the alpha channel: 1.0 (unused)
	fragColor = vec4(value, dot_product_sign, dot(normal_0.xyz, viewing_vector), 1.0);
}

