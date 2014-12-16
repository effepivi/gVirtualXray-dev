/**
********************************************************************************
*
*	@file		outer_surface_l_buffer_gl2.frag
*
*	@brief		Fragment shader that computes the L-buffer. of the external surface.
*				see:
*          		F. P. Vidal, M. Garnier, N. Freud, J. M. Letang, and
*          		N. W. John. Simulation of X-ray attenuation on the GPU. In
*          		Proceedings of Theory and Practice of Computer Graphics 2009,
*          		pages 25-32, Cardiff, UK, June 2009. Eurographics Association.
*          		Winner of Ken Brodlie Prize for Best Paper.
*
*	@version	1.2
*
*	@date		27/12/2013
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
//	OpenGL 2.0
//******************************************************************************
#version 110


//******************************************************************************
//	Varying variables
//******************************************************************************
varying vec3 g_texture_coordinates;


//******************************************************************************
//   Defines
//******************************************************************************
#define KERNEL_SIZE_3x3  8
#define KERNEL_SIZE_5x5 16
#define KERNEL_SIZE_7x7 24
#define EPSILON         0.01


//******************************************************************************
//   Uniform variables
//******************************************************************************
uniform sampler2D g_cumulated_l_buffer_texture;
uniform bool      g_use_artefact_filtering;
uniform float     g_x_offset;
uniform float     g_y_offset;


//******************************************************************************
//   Global variables
//******************************************************************************
vec2 g_offset_3x3[KERNEL_SIZE_3x3];
vec2 g_offset_5x5[KERNEL_SIZE_5x5];
vec2 g_offset_7x7[KERNEL_SIZE_7x7];


//-----------------------------
bool isValid(vec2 lBufferValue)
//-----------------------------
{
    // Return true if lBufferValue is zero, false if it is smaller or greater than zero
    // Care is given to handle floating point numbers' inacuracy
    return (abs(lBufferValue.g) < EPSILON && lBufferValue.r >= 0.0);
}


//-----------------------------------
float filter7x7(float anInitialValue)
//-----------------------------------
{
    int i = 0;
    float count = 0.0;
    float value = 0.0;

    // Process all the texels of the neighbourhood
    for (i = 0; i < KERNEL_SIZE_7x7; ++i)
    {
    	// Get the texel value
        vec2 tex_coord = vec2(g_texture_coordinates.st + g_offset_7x7[i]);

        // Get the L-buffer value
		vec2 l_buffer_value = texture2D(g_cumulated_l_buffer_texture, tex_coord).rg;

    	// The L-buffer value is valid
        if (isValid(l_buffer_value))
        {
        	// Update the cumulated value
        	value += l_buffer_value.r;
        
            // Update the counter
        	count += 1.0;
        }
    }

    // At least one valid value has been found in the neighbourhood
    if (count > EPSILON)
    {
    	// Return the average value
	    value /= count;
    }

    // Return the filtered value
	return (value);
}


//-----------------------------------
float filter5x5(float anInitialValue)
//-----------------------------------
{
	int i = 0;
	float count = 0.0;
	float value = 0.0;
    
    // Process all the texels of the neighbourhood
	for (i = 0; i < KERNEL_SIZE_5x5; ++i)
	{
    	// Get the texel value
		vec2 tex_coord = vec2(g_texture_coordinates.st + g_offset_5x5[i]);

        // Get the L-buffer value
		vec2 l_buffer_value = texture2D(g_cumulated_l_buffer_texture, tex_coord).rg;
    
    	// The L-buffer value is valid
		if (isValid(l_buffer_value))
		{
        	// Update the cumulated value
			value += l_buffer_value.r;
        
            // Update the counter
			count += 1.0;
		}
	}
	
    // No valid valid value has been found in the neighbourhood
    if (count <= EPSILON)
    {
    	// Filter more
    	value = filter7x7(anInitialValue);
    }
    else
    {
        // Return the average value
        value /= count;
    }

    // Return the filtered value
	return (value);
}


//-----------------------------------
float filter3x3(float anInitialValue)
//-----------------------------------
{
	int i = 0;
	float count = 0.0;
	float value = 0.0;
    
    // Process all the texels of the neighbourhood
	for (i = 0; i < KERNEL_SIZE_3x3; ++i)
	{
    	// Get the texel value
		vec2 tex_coord = vec2(g_texture_coordinates.st + g_offset_3x3[i]);

        // Get the L-buffer value
		vec2 l_buffer_value = texture2D(g_cumulated_l_buffer_texture, tex_coord).rg;
    
    	// The L-buffer value is valid
		if (isValid(l_buffer_value))
		{
        	// Update the cumulated value
        	value += l_buffer_value.r;
        
            // Update the counter
        	count += 1.0;
		}
	}
	
    // No valid valid value has been found in the neighbourhood
    if (count <= EPSILON)
    {
    	// Filter more
    	value = filter5x5(anInitialValue);
    }
    else
    {
    	// Return the average value
	    value /= count;
    }

    // Return the filtered value
	return (value);
}


//-------------
void main(void)
//-------------
{
    // Set the texture coordinate set of the 3x3 kernel
	g_offset_3x3[0] = vec2( 0.0, -g_y_offset);
	g_offset_3x3[1] = vec2(-g_x_offset,  0.0);
	g_offset_3x3[2] = vec2( g_x_offset,  0.0);
	g_offset_3x3[3] = vec2( 0.0,  g_y_offset);
	g_offset_3x3[4] = vec2( g_x_offset,  g_y_offset);
	g_offset_3x3[5] = vec2(-g_x_offset,  g_y_offset);
	g_offset_3x3[6] = vec2(-g_x_offset, -g_y_offset);
	g_offset_3x3[7] = vec2( g_x_offset, -g_y_offset);

    // Set the texture coordinate set of the 5x5 kernel
	g_offset_5x5[ 0] = vec2(-2.0 * g_x_offset, -2.0 * g_y_offset);
	g_offset_5x5[ 1] = vec2(-g_x_offset, -2.0 * g_y_offset);
	g_offset_5x5[ 2] = vec2( 0.0, -2.0 * g_y_offset);
	g_offset_5x5[ 3] = vec2( g_x_offset, -2.0 * g_y_offset);
	g_offset_5x5[ 4] = vec2( 2.0 * g_x_offset, -2.0 * g_y_offset);
	g_offset_5x5[ 5] = vec2(-2.0 * g_x_offset,  2.0 * g_y_offset);
	g_offset_5x5[ 6] = vec2(-g_x_offset,  2.0 * g_y_offset);
	g_offset_5x5[ 7] = vec2( 0.0,  2.0 * g_y_offset);
	g_offset_5x5[ 8] = vec2( g_x_offset,  2.0 * g_y_offset);
	g_offset_5x5[ 9] = vec2( 2.0 * g_x_offset,  2.0 * g_y_offset);
	g_offset_5x5[10] = vec2(-2.0 * g_x_offset, -g_y_offset);
	g_offset_5x5[11] = vec2( 2.0 * g_x_offset, -g_y_offset);
	g_offset_5x5[12] = vec2(-2.0 * g_x_offset,  0.0);
	g_offset_5x5[13] = vec2( 2.0 * g_x_offset,  0.0);
	g_offset_5x5[14] = vec2(-2.0 * g_x_offset,  g_y_offset);
	g_offset_5x5[15] = vec2( 2.0 * g_x_offset,  g_y_offset);

    // Set the texture coordinate set of the 7x7 kernel
	g_offset_7x7[ 0] = vec2(-3.0 * g_x_offset, -3.0 * g_y_offset);
	g_offset_7x7[ 1] = vec2(-2.0 * g_x_offset, -3.0 * g_y_offset);
	g_offset_7x7[ 2] = vec2(-g_x_offset, -3.0 * g_y_offset);
	g_offset_7x7[ 3] = vec2( 0.0, -3.0 * g_y_offset);
	g_offset_7x7[ 4] = vec2( g_x_offset, -3.0 * g_y_offset);
	g_offset_7x7[ 5] = vec2( 2.0 * g_x_offset, -3.0 * g_y_offset);
	g_offset_7x7[ 6] = vec2( 3.0 * g_x_offset, -3.0 * g_y_offset);
	g_offset_7x7[ 7] = vec2(-3.0 * g_x_offset,  3.0 * g_y_offset);
	g_offset_7x7[ 8] = vec2(-2.0 * g_x_offset,  3.0 * g_y_offset);
	g_offset_7x7[ 9] = vec2(-g_x_offset,  3.0 * g_y_offset);
	g_offset_7x7[10] = vec2( 0.0,  3.0 * g_y_offset);
	g_offset_7x7[11] = vec2( g_x_offset,  3.0 * g_y_offset);
	g_offset_7x7[12] = vec2( 2.0 * g_x_offset,  3.0 * g_y_offset);
	g_offset_7x7[13] = vec2( 3.0 * g_x_offset,  3.0 * g_y_offset);
	g_offset_7x7[14] = vec2(-3.0 * g_x_offset, -2.0 * g_y_offset);
	g_offset_7x7[15] = vec2( 3.0 * g_x_offset, -2.0 * g_y_offset);
	g_offset_7x7[16] = vec2(-3.0 * g_x_offset, -g_y_offset);
	g_offset_7x7[17] = vec2( 3.0 * g_x_offset, -g_y_offset);
	g_offset_7x7[18] = vec2(-3.0 * g_x_offset,  0.0);
	g_offset_7x7[19] = vec2( 3.0 * g_x_offset,  0.0);
	g_offset_7x7[20] = vec2(-3.0 * g_x_offset,  g_y_offset);
	g_offset_7x7[21] = vec2( 3.0 * g_x_offset,  g_y_offset);
	g_offset_7x7[22] = vec2(-3.0 * g_x_offset,  2.0 * g_y_offset);
	g_offset_7x7[23] = vec2( 3.0 * g_x_offset,  2.0 * g_y_offset);

    // Get the texel value
    vec2 texel_value = texture2D(g_cumulated_l_buffer_texture, g_texture_coordinates.st).rg;
    
    // Get the L-buffer value
    gl_FragColor = vec4(-texel_value.r, 0.0, 0.0, 1.0);
    
    // The artefact filtering is enable
	if (g_use_artefact_filtering)
	{
    	// The texel value is not valid
		if (!isValid(texel_value))
		{
            // Filter the artefact
			gl_FragColor.r = -filter3x3(texel_value.r);
		}
	}
}

