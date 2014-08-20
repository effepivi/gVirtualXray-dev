/**
********************************************************************************
*
*	@file		Units.h
*
*	@brief		Units, such as meters, etc.
*
*	@version	1.2
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


#ifndef GVXR_UNITS_H
#define GVXR_UNITS_H


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

const double kilometer  = 1000.0    / 0.001; ///< kilometre
const double hectometer =  100.0    / 0.001; ///< hectometre
const double decameter  =   10.0    / 0.001; ///< decametre
const double meter      =    1.0    / 0.001; ///< metre
const double decimeter  =    0.1    / 0.001; ///< decimetre
const double centimeter =    0.01   / 0.001; ///< centimetre
const double millimeter =    0.001  / 0.001; ///< millimetre
const double micrometer =    0.0001 / 0.001; ///< micrometre

const double kilometre  = kilometer;  ///< kilometre
const double hectometre = hectometer; ///< hectometre
const double decametre  = decameter;  ///< decametre
const double metre      = meter;      ///< metre
const double decimetre  = decimeter;  ///< decimetre
const double centimetre = centimeter; ///< centimetre
const double millimetre = millimeter; ///< millimetre
const double micrometre = micrometer; ///< micrometre

const double km  = kilometer;  ///< kilometre
const double hm  = hectometer; ///< hectometre
const double dam = decameter;  ///< decametre
const double m   = meter;      ///< meter
const double dm  = decimeter;  ///< decimeter
const double cm  = centimeter; ///< centimeter
const double mm  = millimeter; ///< millimeter
const double um  = micrometer; ///< micrometre

const double km2  = km * km;   ///< square kilometre
const double hm2  = hm * hm;   ///< square hectometre
const double dam2 = dam * dam; ///< square decametre
const double m2   = m * m;     ///< square meter
const double dm2  = dm * dm;   ///< square decimeter
const double cm2  = cm * cm;   ///< square centimeter
const double mm2  = mm * mm;   ///< square millimeter
const double um2  = um * um;   ///< square micrometre

const double megaelectronvolt  = 1.0; ///< megaelectron volt
const double kiloelectronvolt  = megaelectronvolt * 1.0e-3;    ///< kiloelectron volt
const double electronvolt      = megaelectronvolt * 1.0e-6;       ///< electronvolt

const double MeV              = megaelectronvolt;  ///< megaelectron volt
const double keV              = kiloelectronvolt;  ///< kiloelectron volt
const double eV               = electronvolt;      ///< electronvolt

const double kilogram  = 1000.0;    ///< kilogram
const double hectogram =  100.0;    ///< hectogram
const double decagram  =   10.0;    ///< decagram
const double gram      =    1.0;    ///< gram
const double decigram  =    0.1;    ///< decigram
const double centigram =    0.01;   ///< centigram
const double milligram =    0.001;  ///< milligram
const double microgram =    0.0001; ///< microgram

const double kg  = kilogram;  ///< kilogram
const double hg  = hectogram; ///< hectogram
const double dag = decagram;  ///< decagram
const double g   = gram;      ///< gram
const double dg  = decigram;  ///< decigram
const double cg  = centigram; ///< centigram
const double mg  = milligram; ///< milligram
const double ug  = microgram; ///< microgram

} // namespace gVirtualXRay


#endif // UNITS_H
