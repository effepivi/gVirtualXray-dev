/**
********************************************************************************
*
*   @file       TissueMaterial.h
*
*   @brief      Class to manage a material database. It implements the method
*               as follows:
*               Schneider W, Bortfeld T, Schlegel W.:
*               ``Correlation between CT numbers and tissue parameters needed
*               for Monte Carlo simulations of clinical dose distributions'',
*               Phys Med Biol. 2000 Feb;45(2):459-78.
*
*   @version    1.2
*
*   @date       13/10/2014
*
*   @author     Dr Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


#ifndef GVXR_TISSUE_MATERIAL_H
#define GVXR_TISSUE_MATERIAL_H


//******************************************************************************
//	Include
//******************************************************************************
#include <iostream>
#include <map>
#include <string>


//******************************************************************************
//	define
//******************************************************************************


//------------------------------------------------------------------------------
///	The number of elements in a material
//------------------------------------------------------------------------------
#define NUMBER_OF_ELEMENTS_PER_MATERIAL 17


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//******************************************************************************
//  Class declaration
//******************************************************************************
class ElementSet;


//==============================================================================
/**
*	@class	TissueMaterial
*	@brief	TissueMaterial is a class to manage a material.
*/
//==============================================================================
class TissueMaterial
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	//---------------------------------------------------------------------------
	///	Default Constructor
	/*
	*	@param aMinHUValue:	the minimum Hounsfield unit of the material
	*	@param aMaxHUValue:	the maximum Hounsfield unit of the material
	*	@param H:	the amount of H in the material
	*	@param C:	the amount of C in the material
	*	@param N:	the amount of N in the material
	*	@param O:	the amount of O in the material
	*	@param Na:	the amount of Na in the material
	*	@param Mg:	the amount of Mg in the material
	*	@param P:	the amount of P in the material
	*	@param S:	the amount of S in the material
	*	@param Cl:	the amount of Cl in the material
	*	@param Ar:	the amount of Ar in the material
	*	@param K:	the amount of K in the material
	*	@param Ca:	the amount of Ca in the material
	*	@param Ti:	the amount of Ti in the material
	*	@param Cu:	the amount of Cu in the material
	*	@param Zn:	the amount of Zn in the material
	*	@param Ag:	the amount of Ag in the material
	*	@param Sn:	the amount of Sn in the material
	*/
	//---------------------------------------------------------------------------
	TissueMaterial(short aMinHUValue = 0,
			short aMaxHUValue = 0,
			double H = 0,
			double C = 0,
			double N = 0,
			double O = 0,
			double Na = 0,
			double Mg = 0,
			double P = 0,
			double S = 0,
			double Cl = 0,
			double Ar = 0,
			double K = 0,
			double Ca = 0,
			double Ti = 0,
			double Cu = 0,
			double Zn = 0,
			double Ag = 0,
			double Sn = 0);


	//---------------------------------------------------------------------------
	///	Copy constructor
	/*
	*	@param aMaterial:	the material to copy
	*/
	//---------------------------------------------------------------------------
	TissueMaterial(const TissueMaterial& aMaterial);


    static void registerElementSet(const ElementSet* apElementSet);


	//---------------------------------------------------------------------------
	///	Normalise the weight set.
	//---------------------------------------------------------------------------
	void normaliseWeightSet();


	//---------------------------------------------------------------------------
	///	Accessor on the minimum HU.
	/*
	*	@return the minimum HU
	*/
	//---------------------------------------------------------------------------
	short getMinHUValue() const;


	//---------------------------------------------------------------------------
	///	Accessor on the maximum HU.
	/*
	*	@return the maximum HU
	*/
	//---------------------------------------------------------------------------
	short getMaxHUValue() const;


	//---------------------------------------------------------------------------
	///	Accessor on the atomic element weights.
	/*
	*	@return the atomic element weights
	*/
	//---------------------------------------------------------------------------
	const double* getWeightSet() const;


	//---------------------------------------------------------------------------
	///	Update the value of the molar mass of the material.
	/*
	*	@return the molar mass of the material
	*/
	//---------------------------------------------------------------------------
	double computeMolarMass();


	//---------------------------------------------------------------------------
	///	Accessor on the molar mass of the material.
	/*
	*	@return the molar mass of the element
	*/
	//---------------------------------------------------------------------------
	double getMolarMass() const;


	//---------------------------------------------------------------------------
	///	Get the density for a given material (in HU).
	/*
	*	@param aHounsfieldValue:	the material (HU) value
	*	@return the material (HU) density
	*/
	//---------------------------------------------------------------------------
	static double getDensity(short aHounsfieldValue);


	//---------------------------------------------------------------------------
	///	Accessor on the number of element atom per voxel.
	/*
	*	@param aHounsfieldValue:	the material (HU) value
	*	@return the number of element atom per voxel
	*/
	//---------------------------------------------------------------------------
	double getAtomNumberPexVoxel(short aHounsfieldValue) const;


	//---------------------------------------------------------------------------
	///	Get the mass attenuation coefficient at a given energy (values are interpolated).
	/*
	*	@param aHounsfieldValue:	the material (HU) value
	*	@param anEnergy:	the energy	
	*	@return	the corresponding linear attenuation coefficient
	*/
	//---------------------------------------------------------------------------
	double getMassAttenuationTotal(short aHounsfieldValue, double anEnergy) const;


	//---------------------------------------------------------------------------
	///	Get the linear attenuation coefficient at a given energy (in mm-1) (values are interpolated).
	/*
	*	@param aHounsfieldValue:	the material (HU) value
	*	@param anEnergy:	the energy	
	*	@return	the corresponding linear attenuation coefficient
	*/
	//---------------------------------------------------------------------------
	double getMU(short aHounsfieldValue, double anEnergy) const;


	//---------------------------------------------------------------------------
	///	Get the Compton scattering cross section sample (values are interpolated).
	/*
	*	@param x:	the sample
	*	@return	the Compton scattering cross section value
	*/
	//---------------------------------------------------------------------------
	double getS(double x) const;


	//---------------------------------------------------------------------------
	///	Copy operator.
	/*
	*	@param aMaterial:	the material to copy
	*	@return the newly created material
	*/
	//---------------------------------------------------------------------------
	TissueMaterial& operator=(const TissueMaterial& aMaterial);


	//---------------------------------------------------------------------------
	///	operator <<
	/*
	*	@param anOutputSream:	the output stream
	*	@param aTissueMaterial:	the material to output
	*	@return the output stream
	*/
	//---------------------------------------------------------------------------
	friend std::ostream & operator << (std::ostream& anOutputSream,
			const TissueMaterial& aTissueMaterial);

	//---------------------------------------------------------------------------
	///	operator <<
	/*
	*	@param anInputSream:	the input stream
	*	@param aTissueMaterial:	the material to read
	*	@return the input stream
	*/
	//---------------------------------------------------------------------------
	friend std::istream & operator >> (std::istream& anInputSream,
			TissueMaterial& aTissueMaterial);


//*****************************************************************************
private:
	static ElementSet* m_p_element_set;                     ///< The element set
	short m_min_HU_value;                                   ///< The minimum HU value of the material
	short m_max_HU_value;                                   ///< The maximum HU value of the material
	double m_p_weight_set[NUMBER_OF_ELEMENTS_PER_MATERIAL]; ///< The weight of each element in the material
	double m_molar_mass;                                    ///< The molar mass of the material
};


} // namespace gVirtualXRay


//******************************************************************************
#include "TissueMaterial.inl"


#endif // GVXR_TISSUE_MATERIAL_H

