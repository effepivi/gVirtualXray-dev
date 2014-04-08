/**
 ********************************************************************************
 *
 *	@file		display.vert
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


//-------------
void main(void)
//-------------
{
    /*if (g_use_lighting  == 1)
    {
	    vec3 view_direction  = vec3(g_object_position) / g_object_position.w;
	    vec3 normal_vector   = normalize(g_normal);
	    vec3 light_direction = normalize(vec3(gl_LightSource[0].position));
	
	    vec3 global_ambient = vec3(gl_LightModel.ambient     * gl_FrontMaterial.ambient);
	    vec3 ambient        = vec3(gl_FrontMaterial.ambient  * gl_LightSource[0].ambient);
	    vec3 diffuse        = vec3(gl_FrontMaterial.diffuse  * gl_LightSource[0].diffuse  * max(0.0, dot(light_direction, normal_vector)));
	    vec3 specular       = vec3(gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(max(dot(normal_vector, normalize(gl_LightSource[0].halfVector.xyz)), 0.0), gl_FrontMaterial.shininess));
	
    	fragColor = vec4(global_ambient + ambient + diffuse + specular, 1.0);
    }
    else
    {
		fragColor = gl_Color;
    }*/
    
    fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
