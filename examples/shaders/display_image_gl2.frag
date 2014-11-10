/**
 ********************************************************************************
 *
 *	@file		display_image_gl2.frag
 *
 *	@brief		Fragment shader to display an image.
 *
 *	@version	1.0
 *
 *	@date		08/11/2014
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
varying vec2 g_texture_coordinates;


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
    float color = (texture2D(imageTexture, g_texture_coordinates.st).r + shift) * scale;
    gl_FragColor = vec4(color, color, color, 1.0);
}
