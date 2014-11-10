/**
 ********************************************************************************
 *
 *	@file		display_image_gl3.frag
 *
 *	@brief		Fragment shader to display an image.
 *
 *	@version	1.0
 *
 *	@date		09/11/2014
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


//******************************************************************************
//	Output
//******************************************************************************
out vec4 fragColor;


//******************************************************************************
//	Uniform variables
//******************************************************************************
uniform float shift;
uniform float scale;
uniform sampler2D imageTexture;


//-------------
void main(void)
//-------------
{
    float color;
    color = (texture(imageTexture, g_texture_coordinates.st).r + shift) * scale;
    fragColor = vec4(color, color, color, 1.0);
}

