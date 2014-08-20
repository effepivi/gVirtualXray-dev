/**
********************************************************************************
*
*   @file       OpenGL2VBO.inl
*
*   @brief      Class to handle vertex buffer objects (VBOs) in OpenGL 2.x.
*
*   @version    1.2
*
*   @date       07/08/2014
*
*   @author     Dr Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#include <stdio.h>
#include <string>
#include <string.h>

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//------------------------------
inline OpenGL2VBO::~OpenGL2VBO()
//------------------------------
{
	destroy();
}


} // namespace gVirtualXRay
