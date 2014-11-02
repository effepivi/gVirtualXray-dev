/**
********************************************************************************
*
*	@file		OpenGLWindow.inl
*
*	@brief		Class to handle the OpenGL rendering in FLTK.
*
*	@version	1.0
*
*	@date		26/02/2014
*
*	@author		Steven Hampson
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
//	Include
//******************************************************************************
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <FL/fl_ask.H>


//----------------------------------
inline OpenGLWindow::~OpenGLWindow()
//----------------------------------
{}


//-------------------------------------------------
inline float OpenGLWindow::getAnimationFlag() const
//-------------------------------------------------
{
	return (m_animating);
}


//-----------------------------------------------------
inline float OpenGLWindow::getRotationIncrement() const
//-----------------------------------------------------
{
	return (m_rotation_increment);
}


