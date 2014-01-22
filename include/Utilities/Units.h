/**
********************************************************************************
*
*	@file		Units.h
*
*	@brief		Units, such as meters, etc.
*
*	@version	1.0
*
*	@date		17/11/2013
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


#ifndef UNITS_H
#define UNITS_H


//******************************************************************************
//	Include
//******************************************************************************
#ifndef TYPES_H
#include "Types.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace Utilities {


//******************************************************************************
//	Constant declarations
//******************************************************************************
const RATIONAL_NUMBER kilometer  = 1000.0    / 0.001; ///< kilometre
const RATIONAL_NUMBER hectometer =  100.0    / 0.001; ///< hectometre
const RATIONAL_NUMBER decameter  =   10.0    / 0.001; ///< decametre
const RATIONAL_NUMBER meter      =    1.0    / 0.001; ///< metre
const RATIONAL_NUMBER decimeter  =    0.1    / 0.001; ///< decimetre
const RATIONAL_NUMBER centimeter =    0.01   / 0.001; ///< centimetre
const RATIONAL_NUMBER millimeter =    0.001  / 0.001; ///< millimetre
const RATIONAL_NUMBER micrometer =    0.0001 / 0.001; ///< micrometre

const RATIONAL_NUMBER kilometre  = kilometer;  ///< kilometre
const RATIONAL_NUMBER hectometre = hectometer; ///< hectometre
const RATIONAL_NUMBER decametre  = decameter;  ///< decametre
const RATIONAL_NUMBER metre      = meter;      ///< metre
const RATIONAL_NUMBER decimetre  = decimeter;  ///< decimetre
const RATIONAL_NUMBER centimetre = centimeter; ///< centimetre
const RATIONAL_NUMBER millimetre = millimeter; ///< millimetre
const RATIONAL_NUMBER micrometre = micrometer; ///< micrometre

const RATIONAL_NUMBER km  = kilometer;  ///< kilometre
const RATIONAL_NUMBER hm  = hectometer; ///< hectometre
const RATIONAL_NUMBER dam = decameter;  ///< decametre
const RATIONAL_NUMBER m   = meter;      ///< meter
const RATIONAL_NUMBER dm  = decimeter;  ///< decimeter
const RATIONAL_NUMBER cm  = centimeter; ///< centimeter
const RATIONAL_NUMBER mm  = millimeter; ///< millimeter
const RATIONAL_NUMBER um  = micrometer; ///< micrometre

const RATIONAL_NUMBER km2  = km * km;   ///< square kilometre
const RATIONAL_NUMBER hm2  = hm * hm;   ///< square hectometre
const RATIONAL_NUMBER dam2 = dam * dam; ///< square decametre
const RATIONAL_NUMBER m2   = m * m;     ///< square meter
const RATIONAL_NUMBER dm2  = dm * dm;   ///< square decimeter
const RATIONAL_NUMBER cm2  = cm * cm;   ///< square centimeter
const RATIONAL_NUMBER mm2  = mm * mm;   ///< square millimeter
const RATIONAL_NUMBER um2  = um * um;   ///< square micrometre

const RATIONAL_NUMBER megaelectronvolt  = 1.0; ///< megaelectron volt
const RATIONAL_NUMBER kiloelectronvolt  = megaelectronvolt * 1.0e-3;    ///< kiloelectron volt
const RATIONAL_NUMBER electronvolt      = megaelectronvolt * 1.0e-6;       ///< electronvolt

const RATIONAL_NUMBER MeV              = megaelectronvolt;  ///< megaelectron volt
const RATIONAL_NUMBER keV              = kiloelectronvolt;  ///< kiloelectron volt
const RATIONAL_NUMBER eV               = electronvolt;      ///< electronvolt

const RATIONAL_NUMBER kilogram  = 1000.0;    ///< kilogram
const RATIONAL_NUMBER hectogram =  100.0;    ///< hectogram
const RATIONAL_NUMBER decagram  =   10.0;    ///< decagram
const RATIONAL_NUMBER gram      =    1.0;    ///< gram
const RATIONAL_NUMBER decigram  =    0.1;    ///< decigram
const RATIONAL_NUMBER centigram =    0.01;   ///< centigram
const RATIONAL_NUMBER milligram =    0.001;  ///< milligram
const RATIONAL_NUMBER microgram =    0.0001; ///< microgram

const RATIONAL_NUMBER kg  = kilogram;  ///< kilogram
const RATIONAL_NUMBER hg  = hectogram; ///< hectogram
const RATIONAL_NUMBER dag = decagram;  ///< decagram
const RATIONAL_NUMBER g   = gram;      ///< gram
const RATIONAL_NUMBER dg  = decigram;  ///< decigram
const RATIONAL_NUMBER cg  = centigram; ///< centigram
const RATIONAL_NUMBER mg  = milligram; ///< milligram
const RATIONAL_NUMBER ug  = microgram; ///< microgram

} // namespace Utilities


#endif // UNITS_H
