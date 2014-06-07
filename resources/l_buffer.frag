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
smooth in vec3 g_in_vertex;
smooth in vec3 g_normal_0;
smooth in vec3 g_normal_1;


//******************************************************************************
//	Output
//******************************************************************************
out vec4 fragColor;


//******************************************************************************
//	Constants
//******************************************************************************
const bool PERSPECTIVE_PROJECTION = false;
const bool PARALLEL_PROJECTION    = true;
const float EPSILON = 0.01;


//******************************************************************************
//	Uniform variables
//******************************************************************************
uniform float g_to_cm_scale;
uniform mat4 g_modelling_matrix;
uniform mat3 g_normal_matrix;
uniform mat4 g_inverse_modelview_matrix;
uniform bool g_projection_type; // 1 for parallel beam
uniform vec3 g_ray_direction;
uniform vec3 g_source_position;


//--------------
void main (void)
//--------------
{
    float distance_intersection_detector = distance(g_in_vertex, g_inverse_modelview_matrix[3].xyz);

	// Transformed normal
	vec3 normal_0 = normalize(g_normal_matrix * g_normal_0);
	vec3 normal_1 = normalize(g_normal_matrix * g_normal_1);

    // Transform the vertex
	vec4 transformed_vertex = g_modelling_matrix * vec4(g_in_vertex, 1.0);
	transformed_vertex.xyz /= transformed_vertex.w;
	transformed_vertex.w = 1.0;

    // Get the viewing direction
    vec3 viewing_vector;
    
	// Use parallel beam
	if (g_projection_type == PARALLEL_PROJECTION)
	{
		viewing_vector = -g_ray_direction;
	}
    // Use perspective projection
	else
	{
		viewing_vector = -normalize(transformed_vertex.xyz - g_source_position);
	}

	// Save the dot product
	float dot_product = dot(normal_0, viewing_vector);
	float dot_product_sign = sign(dot_product);
    
	if (abs(dot_product_sign) < 0.5)
	{
		normal_0.x += 0.1;
		normal_0.y += 0.1;
		normal_0 = normalize(normal_0);

		viewing_vector.x += 0.1;
		viewing_vector.y += 0.1;
		viewing_vector = normalize(viewing_vector);

        dot_product = dot(normal_0, viewing_vector);
		dot_product_sign = sign(dot_product);
	}
	
	float value = dot_product_sign * distance_intersection_detector * g_to_cm_scale;
	//if (g_distance_intersection_detector < 0.0) value = 0.0;
	
	// In the red channel: the L-buffer value
	// In the green channel the sign of the dot product (useful to detect errors in the final L-buffer)
	// In the blue channel: 0.0 (unused)
	// In the alpha channel: 1.0 (unused)
	fragColor = vec4(value, dot_product_sign, dot_product, 1.0);
}

