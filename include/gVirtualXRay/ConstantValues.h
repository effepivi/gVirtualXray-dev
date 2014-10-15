/**
********************************************************************************
*
*	@file		ConstantValues.h
*
*	@brief		Constant values, such as the Z number of different atoms, etc.
*
*	@version	1.2
*
*	@date		13/10/2014
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


#ifndef GVXR_CONSTANT_VALUES_H
#define GVXR_CONSTANT_VALUES_H


//******************************************************************************
//	Include
//******************************************************************************


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//******************************************************************************
//	Constant declarations
//******************************************************************************
const double PI = 3.14159265358979323846;
const double PI_2 = PI / 2.0;

const double N_Avogadro = 6.02552 * 1.0e23;

const unsigned short Z_H  =  1; ///< Hydrogen atomic number
const unsigned short Z_C  =  6; ///< Carbon atomic number
const unsigned short Z_N  =  7; ///< Nitrogen atomic number
const unsigned short Z_O  =  8; ///< Oxygen atomic number
const unsigned short Z_Na = 11; ///< Sodium atomic number
const unsigned short Z_Mg = 12; ///< Magnesium atomic number
const unsigned short Z_P  = 15; ///< Phosphorus atomic number
const unsigned short Z_S  = 16; ///< Sulfur atomic number
const unsigned short Z_Cl = 17; ///< Chlorine atomic number
const unsigned short Z_Ar = 18; ///< Argon atomic number
const unsigned short Z_K  = 19; ///< Potassium atomic number
const unsigned short Z_Ca = 20; ///< Calcium atomic number
const unsigned short Z_Ti = 22; ///< Titanium atomic number
const unsigned short Z_Fe = 26; ///< Iron atomic number
const unsigned short Z_Cu = 29; ///< Copper atomic number
const unsigned short Z_Zn = 30; ///< Zinc atomic number
const unsigned short Z_Ag = 47; ///< Silver atomic number
const unsigned short Z_Sn = 50; ///< Tin atomic number

} // namespace gVirtualXRay


#endif // GVXR_CONSTANT_VALUES_H
