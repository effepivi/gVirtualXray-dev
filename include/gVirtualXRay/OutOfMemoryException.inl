/**
********************************************************************************
*
*	@file		OutOfMemoryException.inl
*
*	@brief		Class to handle exceptions when memory cannot be allocated 
*				dynamically.
*
*	@version	1.2
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
namespace gVirtualXRay {


//--------------------------------------------------------------------------
inline OutOfMemoryException::OutOfMemoryException(const char* aFileName,
												  const char* aFunctionName,
												  int anErrorLine):
//--------------------------------------------------------------------------
		Exception(aFileName, 
				aFunctionName, 
				anErrorLine, 
				"Out of memory.")
//--------------------------------------------------------------------------
{
}


} // namespace gVirtualXRay
