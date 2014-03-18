/**
********************************************************************************
*
*	@file		OpenGLException.cxx
*
*	@brief		Class to handle exceptions related to frame buffer objects (FBOs).
*
*	@version	1.0
*
*	@date		07/11/2013
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
//	Include
//******************************************************************************
#include <sstream>

#ifndef EXCEPTION_H
#include "gVirtualXRay/FBOException.h"
#endif

#include <GL/glew.h>


//******************************************************************************
//	namespace
//******************************************************************************
namespace Utilities {

//------------------------------------------------------
FBOException::FBOException(const char* aFileName,
						   const char* aFunctionName,
						   int anErrorLine,
						   int anErrorCode):
//------------------------------------------------------
		Exception(aFileName, aFunctionName, anErrorLine)
//------------------------------------------------------
{
	std::stringstream error_message;
	
	switch (anErrorCode)
	{
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
		error_message << "FBO error: Not all framebuffer attachment points are framebuffer attachment complete." << std::endl;
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
		error_message << "FBO error: No images are attached to the framebuffer." << std::endl;
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		error_message << "FBO error: Not all attached images have the same width and height." << std::endl;
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
		error_message << "FBO error: incomplete draw buffer." << std::endl;
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
		error_message << "FBO error: incomplete read buffer." << std::endl;
		break;

	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		error_message << "FBO error: The combination of internal formats of the attached images violates an implementation-dependent set of restrictions." << std::endl;
		break;
	
	default:
		error_message << "FBO error: unknown." << std::endl;
		break;
	}
	
	error_message << "\t- in File: " << aFileName << std::endl; 
	error_message << "\t- in Function: " << aFunctionName << std::endl; 
	error_message << "\t- at Line: " << anErrorLine << std::endl;
	
	m_error_message = error_message.str();
}


} // namespace Utilities
