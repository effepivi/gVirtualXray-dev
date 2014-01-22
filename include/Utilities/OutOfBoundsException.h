/**
********************************************************************************
*
*	@file		OutOfBoundsException.h
*
*	@brief		Class to handle exceptions when accessing an array cell that is
*				not accessible, i.e. out of bounds memory access.
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


#ifndef OUT_OF_BOUNDS_EXCEPTION_H
#define OUT_OF_BOUNDS_EXCEPTION_H


//******************************************************************************
//	Include
//******************************************************************************
#ifndef EXCEPTION_H
#include "Exception.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace Utilities {


//==============================================================================
/**
*	@class	OutOfMemoryException
*	@brief	OutOfMemoryException is a class to handle exceptions when accessing
*			an array cell that is not accessible, i.e. out of bounds
*			memory access.
*/
//==============================================================================
class OutOfBoundsException: public Exception
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
	*/
	//--------------------------------------------------------------------------
	OutOfBoundsException(const char* aFileName = 0,
			const char* aFunctionName = 0,
			int anErrorLine = 0);
};


} // namespace Utilities


//******************************************************************************
#include "OutOfBoundsException.inl"


#endif // OUT_OF_BOUNDS_EXCEPTION_H
