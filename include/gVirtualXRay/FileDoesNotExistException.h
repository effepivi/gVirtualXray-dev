/**
********************************************************************************
*
*	@file		FileDoesNotExistException.inl
*
*	@brief		Class to handle exceptions when trying to open a read-only file
*				that is not accessible.
*
*	@version	1.0
*
*	@date		30/11/2013
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


#ifndef FILE_DOES_NOT_EXIST_EXCEPTION_H
#define FILE_DOES_NOT_EXIST_EXCEPTION_H


//******************************************************************************
//	Include
//******************************************************************************
#include <string>

#ifndef EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace Utilities {


//==============================================================================
/**
*	@class	FileDoesNotExistException
*	@brief	FileDoesNotExistException is a class to handle exceptions
*			when trying to open a read-only file that is not accessible.
*/
//==============================================================================
class FileDoesNotExistException: public Exception
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	//--------------------------------------------------------------------------
	///	Constructor.
	/**
	*	@param 	aFileName:	name of the source file where the exception was thrown
	*	@param 	aFunctionName:	name of the function where the exception was thrown
	*	@param 	anErrorLine:	name at which the exception was thrown
	*	@param 	aNameOfFileToOpen:	the name of the file to open
	*/
	//--------------------------------------------------------------------------
	FileDoesNotExistException(const char* aFileName,
			const char* aFunctionName,
			int anErrorLine,
			const char* aNameOfFileToOpen);


	//--------------------------------------------------------------------------
	///	Constructor.
	/**
	*	@param 	aFileName:	name of the source file where the exception was thrown
	*	@param 	aFunctionName:	name of the function where the exception was thrown
	*	@param 	anErrorLine:	name at which the exception was thrown
	*	@param 	aNameOfFileToOpen:	the name of the file to open
	*/
	//--------------------------------------------------------------------------
	FileDoesNotExistException(const char* aFileName,
			const char* aFunctionName,
			int anErrorLine,
			const std::string& aNameOfFileToOpen);
};


} // namespace Utilities


//******************************************************************************
#include "FileDoesNotExistException.inl"


#endif // FILE_DOES_NOT_EXIST_EXCEPTION_H
