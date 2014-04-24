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
in vec3 in_TexCoord;
in vec3 in_Normal;


//******************************************************************************
//	Output
//******************************************************************************
smooth out vec3 g_in_vertex;
smooth out vec3 g_normal_0;
smooth out vec3 g_normal_1;
smooth out vec4 g_position_in_screen_coordinate;


//******************************************************************************
//	Constants
//******************************************************************************
const bool PERSPECTIVE_PROJECTION = false;
const bool PARALLEL_PROJECTION    = true;
const float EPSILON = 0.01;


//******************************************************************************
//	Uniform variables
//******************************************************************************
uniform vec4 g_detector_plane;
uniform vec3 g_detector_center;
uniform vec3 g_detector_up_vector;
uniform vec3 g_detector_right_vector;
uniform float g_detector_width_in_units_of_length;
uniform float g_detector_height_in_units_of_length;

uniform mat4 g_modelling_matrix;
uniform mat4 g_viewving_matrix;
uniform mat4 g_projection_matrix;
uniform mat4 g_modelview_matrix;
uniform mat4 g_detector_rotation_matrix;
uniform mat4 g_inverse_detector_rotation_matrix;
uniform mat4 g_source_translation_matrix;

uniform bool g_projection_type;
uniform vec3 g_ray_direction;
uniform vec3 g_source_position;


//-------------
void main(void)
//-------------
{
    // Save incoming vertex
    g_in_vertex = in_Vertex;
    
    // Save incoming normal vectors
	g_normal_0 = in_Normal;
	g_normal_1 = in_TexCoord.xyz;

	// Transform vertex
	vec4 transformed_vertex = g_modelling_matrix * vec4(in_Vertex, 1.0);
	transformed_vertex.xyz /= transformed_vertex.w;
	transformed_vertex.w = 1.0;

    // Get the viewing direction
    vec3 viewing_vector;

	// Use parallel beam
	if (g_projection_type == PARALLEL_PROJECTION)
	{
		viewing_vector = -g_ray_direction;
	}
	// Use perspective beam
	else
	{
		viewing_vector = -normalize(transformed_vertex.xyz - g_source_position);
	}

    // Compute the distance between the ray and the detector
    // (using ray tracing between a ray and a plane)
	float distance_intersection_detector = -(dot(transformed_vertex.xyz, g_detector_plane.xyz) + g_detector_plane.w) / dot(-viewing_vector, g_detector_plane.xyz);

	// Find the intersection between the ray and the detector
	vec3 intersection_in_world_coordinates = transformed_vertex.xyz + distance_intersection_detector * viewing_vector;
    
    // Compute the temporary vector
    vec3 temp_vector = intersection_in_world_coordinates - g_detector_center;

    // Project the intersection on the g_detector_right_vector
    float u = dot(g_detector_right_vector, temp_vector);

    // Project the intersection on the g_detector_up_vector
    float v = -dot(g_detector_up_vector, temp_vector);

    // Rescale the u, v coordinates
	u /= (g_detector_width_in_units_of_length);
	v /= (g_detector_height_in_units_of_length);
    
    u += u;
    v += v;
    
    // Save the position
	gl_Position.x = u;
	gl_Position.y = v;
	gl_Position.z = 1;
	gl_Position.w = 1;
   // g_position_in_screen_coordinate = gl_Position;

    // Apply the modelling and viewing transformations
//	gl_Position = g_projection_matrix * g_source_translation_matrix * g_detector_rotation_matrix * g_viewving_matrix * transformed_vertex;
    //gl_Position.xyzw /= gl_Position.w;
  //  g_position_in_screen_coordinate = gl_Position / gl_Position.w;
}

