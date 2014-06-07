/**
********************************************************************************
*
*	@file		OpenGLException.h
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


#ifndef GVXR_FBO_EXCEPTION_H
#define GVXR_FBO_EXCEPTION_H


//******************************************************************************
//	Include
//******************************************************************************
#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*	@class	FBOException
*	@brief	FBOException is a class to handle exceptions related to FBOs.
*/
//==============================================================================
class FBOException: public Exception
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	//--------------------------------------------------------------------------
	///	Default constructor.
	/**
	*	@param 	aFileName:	name of the source file where the exception was thrown
	*	@param 	aFunctionName:	name of the function where the exception was thrown
	*	@param 	anErrorLine:	name at which the exception was thrown
	*	@param 	anErrorCode:	FBO error code
	*/
	//--------------------------------------------------------------------------
	FBOException(const char* aFileName, const char* aFunctionName, int anErrorLine, int anErrorCode);
};


} // namespace gVirtualXRay


#endif // FBO_EXCEPTION_H
