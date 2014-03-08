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

#ifndef IMPLICIT_SURFACE_H
#include "gVirtualXRay/ImplicitSurface.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace Graphics;


//******************************************************************************
//	Defines
//******************************************************************************


//******************************************************************************
//	Constant variables
//******************************************************************************


//******************************************************************************
//	Global variables
//******************************************************************************


//******************************************************************************
//	Function declaration
//******************************************************************************


//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
	ImplicitSurface sphere;
	ImplicitSurface blobby_molecule;
	ImplicitSurface meta_ball;
	ImplicitSurface soft_object;

	sphere.setSphere();
	blobby_molecule.setBlobbyMolecule(2, 1);
	meta_ball.setMetaBall(1, 3);
	soft_object.setSoftObject(1, 3);

	for (float i(0); i <= 4.0; i += 0.05)
	{
		std::cout << i;
		std::cout << "\t";
		std::cout << sphere.evaluate(i);
		std::cout << "\t";
		std::cout << blobby_molecule.evaluate(i);
		std::cout << "\t";
		std::cout << meta_ball.evaluate(i);
		std::cout << "\t";
		std::cout << soft_object.evaluate(i);
		std::cout << std::endl;
	}

	meta_ball.addControlPoint(VEC3(-2.0, 0, 0));
	meta_ball.addControlPoint(VEC3( 2.0, 0, 0));

	meta_ball.voxelise(Vec3ui(128, 128, 128),
			VEC3(0, 0, 0),
			VEC3(0.1, 0.1, 0.1));
	meta_ball.writeVoxelData("meta_ball.raw");
}
