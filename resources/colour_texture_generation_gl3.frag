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
//	constant variables
//******************************************************************************
//size of the arrays to initialize noises
const int TAB_SIZE = 64;

//Frequence for fractal sum and turbulence
const float MINFREQ=0.03;
const float MAXFREQ=1;

//step for bump evaluation 
const float STEP=0.00002;

//******************************************************************************
//	Input
//******************************************************************************
smooth in vec3 g_normal;
smooth in vec3 g_object_position;
smooth in vec2 g_texture_coordinates;

//******************************************************************************
//	Output
//******************************************************************************
out vec4 frag_colour;

//******************************************************************************
//	Uniform variables
//******************************************************************************
uniform vec4 g_base_colour;
uniform vec4 g_second_colour;
uniform int g_has_tendon;
uniform int g_use_second_colour;
uniform sampler2D g_bump_map;
uniform int g_perm[TAB_SIZE];
uniform vec3 g_gradient_tab[TAB_SIZE];

//------------------------------------
int index(int ix, int iy, int iz)
//------------------------------------
{
	return g_perm[(ix+g_perm[(iy+g_perm[iz%TAB_SIZE])%TAB_SIZE])%TAB_SIZE];	
}

//------------------------------------
float spline(float x, float knots[4])
//------------------------------------
{
	vec4 v_knots = vec4(knots[0], knots[1], knots[2], knots[3]);
					
	mat4 matrix = mat4(-0.5, 1., -0.5, 0.,
					1.5, -2.5, 0., 1.,
					-1.5, 2., 0.5, 0.,
					0.5, -0.5, 0., 0.);
					
	vec4 c = matrix*v_knots;
	
	x = clamp(x, 0, 1);
	
	return ((c.x*x + c.y)*x +c.z)*x +c.w;
}


//------------------------------------
float vlattice(int ix, int iy, int iz)
//------------------------------------
{
	return g_gradient_tab[index(ix,iy,iz)].z;
}

//Value noise
//------------------------------------
float vnoise(vec3 v)
//------------------------------------
{
	float x = v.x;
	float y = v.y;
	float z = v.z;

	int ix, iy, iz;
	int i, j, k;
	float fx, fy, fz;
	float xknots[4], yknots[4], zknots[4];
	
	ix = int(floor(x));
	fx = x - ix;
	
	iy = int(floor(y));
	fy = y - iy;

	iz = int(floor(z));
	fz = z - iz;

	for(k = -1; k <= 2; k++)
	{
		for(j = -1; j <= 2; j++)
		{
			for(i = -1; i <= 2; i++)
			{
				xknots[i+1] = vlattice(ix+i, iy+j, iz+k);
			}
			yknots[j+1] = spline(fx, xknots);
		}
		zknots[k+1] = spline(fy, yknots);
	}
	return spline(fz, zknots)*0.5+0.5;
}

//------------------------------------
float glattice(int ix, int iy, int iz, 
		float fx, float fy, float fz)
//------------------------------------
{
	int base_index = index(ix,iy,iz);
	return g_gradient_tab[base_index].x*fx + g_gradient_tab[base_index].y*fy + g_gradient_tab[base_index].z*fz;
}

//------------------------------------
float lerp(float t, float x0, float x1)
//------------------------------------
{
	return (x0 + t*(x1-x0));
}

//------------------------------------
float smoothStep(float x)
//------------------------------------
{
		return (x*x*(3 - 2*x));
}

//gradient noise using Perlin's method
//------------------------------------
float gnoise(vec3 v)
//------------------------------------
{
	float x = v.x;
	float y = v.y;
	float z = v.z;
	
	int ix, iy, iz;
	float fx0, fx1, fy0, fy1, fz0, fz1;
	float wx, wy, wz;
	float vx0, vx1, vy0, vy1, vz0, vz1;
 
	ix = int(floor(x));
	fx0 = x - ix;
	fx1 = fx0 -1;
	wx = smoothStep(fx0);
	
	iy = int(floor(y));
	fy0 = y - iy;
	fy1 = fy0 -1;
	wy = smoothStep(fy0);
	
	iz = int(floor(z));
	fz0 = z - iz;
	fz1 = fz0 -1;
	wz = smoothStep(fz0);
	
	vx0 = glattice(ix, iy, iz, fx0, fy0, fz0);
	vx1 = glattice(ix+1, iy, iz, fx1, fy0, fz0);
	vy0 = lerp(wx, vx0, vx1);	
	vx0 = glattice(ix, iy+1, iz, fx0, fy1, fz0);
	vx1 = glattice(ix+1, iy+1, iz, fx1, fy1, fz0);
	vy1 = lerp(wx, vx0, vx1);
	vz0 = lerp(wy, vy0, vy1);
	vx0 = glattice(ix, iy, iz+1, fx0, fy0, fz1);
	vx1 = glattice(ix+1, iy, iz+1, fx1, fy0, fz1);
	vy0 = lerp(wx, vx0, vx1);
	vx0 = glattice(ix, iy+1, iz+1, fx0, fy1, fz1);
	vx1 = glattice(ix+1, iy+1, iz+1, fx1, fy1, fz1);
	vy1 = lerp(wx, vx0, vx1);
	vz1 = lerp(wy, vy0, vy1);
	
	return lerp(wz, vz0, vz1);
}

//combine value noise and gradient noise
//------------------------
float noise(vec3 v)
//------------------------
{
	return (gnoise(v) + vnoise(v))*0.5;
}

//fractal sum uses gradient noise
//------------------------
float fractalSum(vec3 Q)
//------------------------
{
	float value = 0;
	for(float f=MINFREQ; f<MAXFREQ; f*=2)
	{
		value += gnoise(Q*f)/f;	
	}
	return value*2*MINFREQ+0.5;
}

//turbulence uses value noise + gradient noise
//------------------------
float turbulence(vec3 Q)
//------------------------
{
	float value = 0;
	for (float f=MINFREQ; f<MAXFREQ;f*2)
	{
		value += abs(noise(Q*f))/f;
		return value*2*MINFREQ+0.5;
	}
}

//------------------------
vec4 computeTendonColour()
//------------------------
{
	//modify the normal vector using the bump map
	vec3 deltaNormal = texture( g_bump_map, vec2(g_texture_coordinates.x+STEP, g_texture_coordinates.y)).r*vec3(1,0,0)-
		texture( g_bump_map, vec2(g_texture_coordinates.x-STEP, g_texture_coordinates.y)).r*vec3(-1,0,0);
	
	deltaNormal += texture( g_bump_map, vec2(g_texture_coordinates.x, g_texture_coordinates.y+STEP)).r*vec3(0,1,0)-
		texture( g_bump_map, vec2(g_texture_coordinates.x, g_texture_coordinates.y-STEP)).r*vec3(0,-1,0);
    
    
    vec3 normal_vector   = normalize(g_normal+2*deltaNormal);
	
	float dot_normal_vertical = abs(dot(normal_vector , vec3(0,0,1)));
	
	if(dot_normal_vertical<0.2)
	{
		//if on the sides, it is muscle
		return g_base_colour;
	}
	else if(dot_normal_vertical>0.8)
		{
			//if on top or at the bottom, it is tendon
			return g_second_colour;
		}
	else 
	{
		//interpolate in between
		return (g_base_colour*(0.8-dot_normal_vertical)+g_second_colour*(dot_normal_vertical-0.2))/0.6;
	}

	/*return (g_base_colour*(1-dot_normal_vertical)+
		g_second_colour*(dot_normal_vertical));*/
	
}

//-------------
void main(void)
//-------------
{
	if(g_has_tendon==1)
	{
		frag_colour=computeTendonColour();
	}
	else
	{
		vec4 second_colour;
		float frac_sum_value = fractalSum(g_object_position);
		float turbulence_value;
	
		if(g_use_second_colour==1)
		{
			turbulence_value=turbulence(g_object_position);
			second_colour=g_second_colour*(1-frac_sum_value);
		}
		else
		{
			turbulence_value=1;
			second_colour=vec4(0,0,0,0);
		}
		frag_colour=(g_base_colour*frac_sum_value+second_colour)*turbulence_value;
	}
}
