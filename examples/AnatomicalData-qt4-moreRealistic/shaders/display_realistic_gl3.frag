/*

Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
http://www.fpvidal.net/
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the Bangor University nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


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
smooth in vec2 g_texture_coordinates;
smooth in vec3 g_normal;
smooth in vec3 g_view_direction;
smooth in vec3 g_light_direction;
smooth in vec4 g_shadowCoord;

//******************************************************************************
//	Output
//******************************************************************************
out vec4 fragcolour;


//******************************************************************************
//	Uniform variables
//******************************************************************************
uniform int g_use_lighting;
uniform int g_useTexture;
uniform vec3 light_direction;
uniform vec4 light_global_ambient;
uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;
uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform float material_shininess;
uniform sampler2DShadow shadowMap;
uniform sampler2D bumpMap;
uniform sampler2D colourTexture;

//******************************************************************************
//	constant variables
//******************************************************************************
//BIAS to reduce shadow acne
const float MAX_BIAS=0.005;
const float BIAS_RATE=0.005;

//-------------
void main(void)
//-------------
{
    if (g_use_lighting  == 1)
    {
   	 	vec3 normal_vector = g_normal;
    	
    	if(0 == 1)
	    {
	    	vec3 deltaNormal = texture( bumpMap, vec2(g_texture_coordinates.x*1.01, g_texture_coordinates.y)).rgb-texture( bumpMap, vec2(g_texture_coordinates.x*0.09, g_texture_coordinates.y)).rgb;
	    	deltaNormal = texture( bumpMap, vec2(g_texture_coordinates.x, g_texture_coordinates.y*1.01)).rgb-texture( bumpMap, vec2(g_texture_coordinates.x, g_texture_coordinates.y*0.09)).rgb;
    		normal_vector   = g_normal+deltaNormal;
    	}
  		normal_vector = normalize(normal_vector);
    
    	//cosinus of the light direction and the normal vector
    	float cosLightNormal = max(0., dot(light_direction, normal_vector));
    	
    	//shadow bias to reduce shadow acne
    	float bias = BIAS_RATE * tan(acos(cosLightNormal));
		bias = clamp(bias, 0,MAX_BIAS)/g_shadowCoord.w;
    
		//change the shadow coordinates. They have to be in [0,1] for texture samlping
		vec3 shadowChangedCoord = g_shadowCoord.xyz/g_shadowCoord.w;
		shadowChangedCoord = shadowChangedCoord*0.5 + 0.5;
		    
    	//visibility equals 0.3 if the fragment if in the shadow and 1 if not.
    	float visibility = texture(shadowMap, vec3(shadowChangedCoord.xy, shadowChangedCoord.z-bias));
		visibility = visibility*0.7+0.3;
	        	
	  	vec3 global_ambient;
	   	vec3 ambient;
	    vec3 diffuse;
	    
	    if(1==0)
	    {
	    	global_ambient =vec3(light_global_ambient*texture( colourTexture,g_texture_coordinates));
	   		ambient        = vec3(light_ambient * texture( colourTexture, g_texture_coordinates ));
	    	diffuse        =  vec3(texture( colourTexture, g_texture_coordinates)*light_diffuse)* cosLightNormal;
	    }
	    else
	    {
	    	global_ambient = vec3(light_global_ambient * material_ambient);
	   		ambient        = vec3(material_ambient  * light_ambient);
	    	diffuse        = vec3(material_diffuse  * light_diffuse)  * cosLightNormal;
	    }

	    vec3 specular       = vec3(material_specular * light_specular * pow(max(dot(g_view_direction, reflect(light_direction, normal_vector)), 0.), material_shininess));
	
    	fragcolour = visibility*vec4(global_ambient+ambient + diffuse + specular, 1.);
    }
    else
    {
        fragcolour = material_ambient;
    }
}
