/**
********************************************************************************
*
*	@file		Utilities.h
*
*	@brief		Some utility functions that do not fit anywhere else.
*
*	@version	1.0
*
*	@date		12/02/2014
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


#ifndef UTILITIES_H
#define UTILITIES_H


//******************************************************************************
//	namespace
//******************************************************************************
namespace Utilities {


//------------------------------------------------------------------------------
///	Uncompress data using the Zlib.
/**
*	@param 	src:	compressed data
*	@param 	srcLen:	length of the uncompressed data
*	@param 	apDst:	uncompressed data
*/
//------------------------------------------------------------------------------
int inflate(const void *src, int srcLen, char** apDst);


} // namespace Utilities


//******************************************************************************
//#include "Utilities.inl"


#endif // UTILITIES_H
