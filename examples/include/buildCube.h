/**
********************************************************************************
*
*	@file		buildCube.h
*
*	@brief		Generate the triangle mesh corresponding to a cube.
*
*	@version	1.0
*
*	@date		25/02/2014
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


#ifndef BUILD_CUBE_H
#define BUILD_CUBE_H


//******************************************************************************
//	Include
//******************************************************************************
#include <vector>

#ifndef TYPES_H
#include <gVirtualXRay/Types.h>
#endif


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
void buildCube(Utilities::RATIONAL_NUMBER aLenght,
		Utilities::VEC3 aCentre,
		std::vector<double>& apVertexSet);


#endif // BUILD_CUBE_H
