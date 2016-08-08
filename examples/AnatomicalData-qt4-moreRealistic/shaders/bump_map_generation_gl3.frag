/**
********************************************************************************
*
*	@file		bump_map_generation_gl3.frag
*
*	@brief      fragment shader to create a bump map.
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
smooth in vec3 g_normal;
smooth in vec3 g_object_position;

//******************************************************************************
//	Output
//******************************************************************************
out float fragColor;


//******************************************************************************
//	Uniform variables
//******************************************************************************
uniform int g_has_tendon;

//******************************************************************************
//	constant variables
//******************************************************************************
//Frequence for fractal sum
const float MINFREQ=0.1;
const float MAXFREQ=1;

//coefitient of the perturbation
const float PERTURBATION_COEF = 0.001;


float mod289(float x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 mod289(vec4 x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 perm(vec4 x){return mod289(((x * 34.0) + 1.0) * x);}

float noise(vec3 p){
    vec3 a = floor(p);
    vec3 d = p - a;
    d = d * d * (3.0 - 2.0 * d);

    vec4 b = a.xxyy + vec4(0.0, 1.0, 0.0, 1.0);
    vec4 k1 = perm(b.xyxy);
    vec4 k2 = perm(k1.xyxy + b.zzww);

    vec4 c = k2 + a.zzzz;
    vec4 k3 = perm(c);
    vec4 k4 = perm(c + 1.0);

    vec4 o1 = fract(k3 * (1.0 / 41.0));
    vec4 o2 = fract(k4 * (1.0 / 41.0));

    vec4 o3 = o2 * d.z + o1 * (1.0 - d.z);
    vec2 o4 = o3.yw * d.x + o3.xz * (1.0 - d.x);

    return o4.y * d.y + o4.x * (1.0 - d.y);
}

float fracNoise(vec3 v)
{
	return 2*noise(v)-1;
}

float fractalSum(vec3 Q)
{
	float value = 0;
	for(float f=MINFREQ; f<MAXFREQ; f*=2)
	{
		value += fracNoise(Q*f)/f;	
	}
	return value*0.5+0.5;
}

//-------------
void main(void)
//-------------
{
	float noise_value = noise(g_object_position);
	fragColor = noise_value*0.1;
	
	if(g_has_tendon == 1)
	{
		//create perturbation using noise and fractal sum
		float perturbation = noise_value + fractalSum(g_object_position);
	
		//lower the perturbation
		perturbation *= PERTURBATION_COEF;
	
		//angle of the point to the center
		float theta = atan(g_object_position.y,g_object_position.x+length(g_object_position.xy));
		
		//Creates lines disturbed by fractal sum and noise
		fragColor+=0.2*(1-(pow(abs(sin(500*(theta+perturbation))),14)+0.6*pow(abs(sin(700*(theta+perturbation))),10))+0.4*pow(abs(sin(900*(theta+perturbation))),8))*(1-abs(dot(g_normal,vec3(0,0,1))));
	}
}
