/**
 ********************************************************************************
 *
 *	@file		display_gl3.frag
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
//	OpenGL 3.2
//******************************************************************************
#version 150
precision highp float; // Video card drivers require this line to function properly


//******************************************************************************
//	Input
//******************************************************************************
smooth in vec3 g_texture_coordinates;
smooth in vec3 g_normal;
smooth in vec4 g_object_position;


//******************************************************************************
//	Output
//******************************************************************************
out vec4 fragColor;


//******************************************************************************
//	Uniform variables
//******************************************************************************
uniform int g_use_lighting;
uniform vec4 light_global_ambient;
uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;
uniform vec4 light_position;
uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform float material_shininess;


//-------------
void main(void)
//-------------
{
    if (g_use_lighting  == 1)
    {
	    vec3 view_direction  = vec3(g_object_position) / g_object_position.w;
	    vec3 normal_vector   = normalize(g_normal);
        vec3 light_direction = normalize(vec3(light_position.xyz));
        
        vec3 half_vector     = normalize(view_direction + light_direction);
	
	    vec3 global_ambient = vec3(light_global_ambient * material_ambient);
	    vec3 ambient        = vec3(material_ambient  * light_ambient);
	    vec3 diffuse        = vec3(material_diffuse  * light_diffuse  * max(0.0, dot(light_direction, normal_vector)));
	    vec3 specular       = vec3(material_specular * light_specular * pow(max(dot(normal_vector, half_vector), 0.0), material_shininess));
	
    	fragColor = vec4(global_ambient + ambient + diffuse + specular, 1.0);
    }
    else
    {
        fragColor = material_ambient;
    }
}
