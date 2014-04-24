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

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/FBOException.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
using namespace gVirtualXRay;

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
    case GL_FRAMEBUFFER_UNDEFINED:
        error_message << "FBO error: the target is the default framebuffer, but the default framebuffer does not exist." << std::endl;
		break;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        error_message << "FBO error: any of the framebuffer attachment points are framebuffer incomplete." << std::endl;
		break;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        error_message << "FBO error: the framebuffer does not have at least one image attached to it." << std::endl;
		break;

    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        error_message << "FBO error: the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAW_BUFFERi." << std::endl;
		break;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        error_message << "FBO error: GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER." << std::endl;
		break;

    case GL_FRAMEBUFFER_UNSUPPORTED:
        error_message << "FBO error: the combination of internal formats of the attached images violates an implementation-dependent set of restrictions." << std::endl;
		break;

    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
        error_message << "FBO error:" << std::endl;
        error_message << "the value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers; if the value of GL_TEXTURE_SAMPLES is the not same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES." << std::endl;
        error_message << "or" << std::endl;
        error_message << "the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all attached textures." << std::endl;
		break;

    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
        error_message << "FBO error: any framebuffer attachment is layered, and any populated attachment is not layered, or if all populated color attachments are not from textures of the same target." << std::endl;
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
