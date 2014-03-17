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
*				http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
*				BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#include <iostream>
#include <cstdlib>

#ifndef IMPLICIT_SURFACE_H
#include "gVirtualXRay/ImplicitSurface.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace Graphics;


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

