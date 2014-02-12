/**
********************************************************************************
*
*	@file		OpenGLException.inl
*
*	@brief		Class to handle exceptions related to OpenGL.
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
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#ifndef EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace Utilities {

//----------------------------------------------------------------
inline OpenGLException::OpenGLException(const char* aFileName,
										const char* aFunctionName,
										int anErrorLine,
										int anErrorCode):
//----------------------------------------------------------------
		Exception(aFileName, aFunctionName, anErrorLine)
//----------------------------------------------------------------
{
	std::stringstream error_message;
	error_message << "OpenGL error: " << gluErrorString(anErrorCode) << std::endl; 
	error_message << "\t- in File: " << aFileName << std::endl; 
	error_message << "\t- in Function: " << aFunctionName << std::endl; 
	error_message << "\t- at Line: " << anErrorLine << std::endl;
	
	m_error_message = error_message.str();
}


} // namespace Utilities
