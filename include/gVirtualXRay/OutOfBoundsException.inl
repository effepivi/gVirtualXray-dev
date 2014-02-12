/**
********************************************************************************
*
*	@file		OutOfBoundsException.inl
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


//******************************************************************************
//	namespace
//******************************************************************************
namespace Utilities {


//--------------------------------------------------------------------------
inline OutOfBoundsException::OutOfBoundsException(const char* aFileName,
												  const char* aFunctionName,
												  int anErrorLine):
//--------------------------------------------------------------------------
		Exception(aFileName, 
				aFunctionName, 
				anErrorLine, 
				"Out of bounds Memory Access.")
//--------------------------------------------------------------------------
{
}


} // namespace Utilities
