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
*          		pages 25-32, Cardiff, UK, June 2009. Eurographics Association.
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


//******************************************************************************
//	Input
//******************************************************************************
in vec3 in_Vertex;
in vec4 in_Color;
in vec3 in_TexCoord;
in vec3 in_Normal;


//******************************************************************************
//	Output
//******************************************************************************
smooth out float g_distance_intersection_detector;
smooth out vec3 g_viewing_vector;
smooth out vec4 g_normal_0;
smooth out vec4 g_normal_1;


//******************************************************************************
//	Uniform variables
//******************************************************************************
uniform int g_projection_type; // 1 for parallel beam
uniform vec3 g_ray_direction;
uniform vec3 g_source_position;
uniform mat4 g_modelling_matrix;
uniform vec4 g_detector_plane;
uniform vec3 g_detector_center;
uniform vec3 g_detector_up_vector;
uniform vec3 g_detector_right_vector;
uniform float g_detector_width_in_units_of_length;
uniform float g_detector_height_in_units_of_length;


//-------------
void main(void)
//-------------
{
	float u;
	float v;
	
	// Transformed vertex
	vec4 transformed_vertex = g_modelling_matrix * vec4(in_Vertex, 1.0);
	transformed_vertex.xyz /= transformed_vertex.w;
	transformed_vertex.w = 1.0;
	
	// Use parallel beam
	if (g_projection_type == 1)
	{
		g_viewing_vector = -g_ray_direction;
	}
	else
	{
		g_viewing_vector = -normalize(transformed_vertex.xyz - g_source_position);
	}

	// Find the intersection between the ray and the detector
	g_distance_intersection_detector = -(dot(transformed_vertex.xyz, g_detector_plane.xyz) + g_detector_plane.w) / dot(-g_viewing_vector, g_detector_plane.xyz);
	vec3 intersection_in_world_coordinates = transformed_vertex.xyz + g_distance_intersection_detector * g_viewing_vector;
	
	// Distance (u, v) from the detector centre to the intersection in world coordinates
	v = (g_detector_right_vector.x * (intersection_in_world_coordinates.y - g_detector_center.y) - g_detector_right_vector.y * (intersection_in_world_coordinates.x - g_detector_center.x)) / (g_detector_up_vector.y * g_detector_right_vector.x - g_detector_up_vector.x * g_detector_right_vector.y);

	if (abs(g_detector_right_vector.x) > 0.0000001)
		u = (intersection_in_world_coordinates.x - g_detector_center.x - v * g_detector_up_vector.x) / g_detector_right_vector.x;
	else if (abs(g_detector_right_vector.y) > 0.0000001)
		u = (intersection_in_world_coordinates.y - g_detector_center.y - v * g_detector_up_vector.y) / g_detector_right_vector.y;
	else if (abs(g_detector_right_vector.z) > 0.0000001)
		u = (intersection_in_world_coordinates.z - g_detector_center.z - v * g_detector_up_vector.z) / g_detector_right_vector.z;

	u /= g_detector_width_in_units_of_length / 2.0;
	v /= g_detector_height_in_units_of_length / 2.0;
	
	gl_Position.x = u;
	gl_Position.y = v;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;
	
	// Normals
	g_normal_0 = vec4(-in_Normal, 1.0);
	g_normal_1 = vec4(-in_TexCoord.xyz, 1.0);
}

