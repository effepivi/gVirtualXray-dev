/**
********************************************************************************
*
*	@file		Exception.h
*
*	@brief		Generic class to handle exceptions.
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


#ifndef GVXR_EXCEPTION_H
#define GVXR_EXCEPTION_H


//******************************************************************************
//	Include
//******************************************************************************
#include <string>
#include <exception>
#include <iostream>


//******************************************************************************
//	namespace
//******************************************************************************
namespace Utilities {


//******************************************************************************
//	class declaration
//******************************************************************************
class Exception;


//******************************************************************************
//	function declaration
//******************************************************************************


//------------------------------------------------------------------------------
///	operator<<
/**
*	@param 	anOutputStream:	the output stream
*	@param 	anException:	the exception to add to the output stream
*	@return the output stream
*/
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& anOutputStream,
		const Exception& anException);


//==============================================================================
/**
*	@class	Exception
*	@brief	Exception is a class to handle exceptions.
*/
//==============================================================================
class Exception: public std::exception
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	//--------------------------------------------------------------------------
	///	Default constructor.
	/**
	*	@param 	aFileName:	name of the source file where the exception was thrown (default value: 0)
	*	@param 	aFunctionName:	name of the function where the exception was thrown  (default value: 0)
	*	@param 	anErrorLine:	name at which the exception was thrown (default value: 0)
	*	@param 	anErrorMessage:	error message corresponding to the exception (default value: 0)
	*/
	//--------------------------------------------------------------------------
	Exception(const char* aFileName = 0, const char* aFunctionName = 0, int anErrorLine = 0, const char* anErrorMessage = 0);


	//--------------------------------------------------------------------------
	///	Constructor.
	/**
	*	@param 	aFileName:	name of the source file where the exception was thrown (default value: 0)
	*	@param 	aFunctionName:	name of the function where the exception was thrown  (default value: 0)
	*	@param 	anErrorLine:	name at which the exception was thrown (default value: 0)
	*	@param 	anErrorMessage:	error message corresponding to the exception (default value: 0)
	*/
	//--------------------------------------------------------------------------
	Exception(const char* aFileName, const char* aFunctionName, int anErrorLine, const std::string& anErrorMessage);


	//--------------------------------------------------------------------------
	///	Destructor.
	//--------------------------------------------------------------------------
	virtual ~Exception() throw();


	//--------------------------------------------------------------------------
	///	Accessor on the error message.
	/**
	*	@return the error message
	*/
	//--------------------------------------------------------------------------
	virtual const char* what() const throw();


//******************************************************************************
protected:
	/// The error message
	std::string m_error_message;
};


} // namespace Utilities


//******************************************************************************
#include "Exception.inl"


#endif // EXCEPTION_H
