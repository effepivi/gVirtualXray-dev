/*

Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
http://www.fpvidal.net/
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the Bangor University nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


/**
********************************************************************************
*
*	@file		test_implicit_surface_density_function.cxx
*
*	@brief		Main test program for implicit surface density functions.
*
*	@version	1.0
*
*	@date		08/03/2014
*
*	@todo		Fix bug when using the special key.
*				There is a problem with the internal vectors of the detector.
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
*				http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
*				BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#include <iostream>
#include <cstdlib>

#ifndef GVXR_IMPLICIT_SURFACE_H
#include "gVirtualXRay/ImplicitSurface.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//	Function declaration
//******************************************************************************
void loadImplicitSurface();


//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
    try
    {
		loadImplicitSurface();
		return (EXIT_SUCCESS);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return (EXIT_FAILURE);
    }
}


//------------------------
void loadImplicitSurface()
//------------------------
{
	ImplicitSurface sphere;
	ImplicitSurface blobby_molecule;
	ImplicitSurface meta_ball;
	ImplicitSurface soft_object;

	ImplicitSurface::DensityFunctionParameterType sphere_parameter(         ImplicitSurface::SPHERE);
	ImplicitSurface::DensityFunctionParameterType blobby_molecule_parameter(ImplicitSurface::BLOBBY_MOLECULE, 2, 1);
	ImplicitSurface::DensityFunctionParameterType meta_ball_parameter(      ImplicitSurface::META_BALL,       1, 3);
	ImplicitSurface::DensityFunctionParameterType soft_object_parameter(    ImplicitSurface::SOFT_OBJECT,     1, 3);

	for (float i(0); i <= 4.0; i += 0.05)
	{
		std::cout << i;
		std::cout << "\t";
		std::cout << sphere.evaluate(i, sphere_parameter);
		std::cout << "\t";
		std::cout << blobby_molecule.evaluate(i, blobby_molecule_parameter);
		std::cout << "\t";
		std::cout << meta_ball.evaluate(i, meta_ball_parameter);
		std::cout << "\t";
		std::cout << soft_object.evaluate(i, soft_object_parameter);
		std::cout << std::endl;
	}
}

