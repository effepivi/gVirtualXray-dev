/**
********************************************************************************
*
*	@file		colour_texture_generation_gl2.frag
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
//	OpenGL 2.1
//******************************************************************************
#version 110


//******************************************************************************
//	constant variables
//******************************************************************************
//size of the arrays to initialize noises
const int TAB_SIZE = 64;

//Frequence for fractal sum and turbulence
const float MINFREQ=0.03;
const float MAXFREQ=1;

//******************************************************************************
//	Input
//******************************************************************************
varying vec3 g_object_position;


//******************************************************************************
//	Uniform variables
//******************************************************************************
uniform vec4 g_base_colour;
uniform int g_perm[TAB_SIZE];
uniform vec3 g_gradient_tab[TAB_SIZE];


//------------------------------------
int index(int ix, int iy, int iz)
//------------------------------------
{
	return g_perm[(ix+g_perm[(iy+g_perm[iz%TAB_SIZE])%TAB_SIZE])%TAB_SIZE];	
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
	return value*MINFREQ+0.5;
}

//-------------
void main(void)
//-------------
{
	gl_FragColor = g_base_colour * fractalSum(g_object_position);
}
