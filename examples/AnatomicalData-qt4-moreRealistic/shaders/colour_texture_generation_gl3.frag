/**
********************************************************************************
*
*	@file		colour_texture_generation_gl3.frag
*
*	@brief      fragment shader to create a texture.
*
*	@version	1.0
*
*	@date		01/08/2016
*
*	@author		Andréas Meuleman
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
