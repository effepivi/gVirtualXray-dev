/**
********************************************************************************
*
*   @file       AtomicElement.h
*
*   @brief      Class to manage elements in material.
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


#ifndef GVXR_ATOMIC_ELEMENT_H
#define GVXR_ATOMIC_ELEMENT_H


//******************************************************************************
//	Include
//******************************************************************************
#include <iostream>
#include <map>
#include <string>


//******************************************************************************
//	defines
//******************************************************************************


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*	@class	AtomicElement
*	@brief	AtomicElement is a class to manage elements in material.
*/
//==============================================================================
class AtomicElement
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	//---------------------------------------------------------------------------
	///	Default Constructor
	/*
	*	@param aName:	the name of the element
	*	@param aSymbol:	the symbol of the element
	*	@param aZNumber:	the atomic number (Z) of the element
	*	@param aMolarMass:	the molar mass of the element (in g/mol))
	*	@param aDensity:	the density of the element (in g/cm3)
	*	@param aState:	the state of the element
	*	@param aVoxelSize:	the size of a voxel
	*/
	//---------------------------------------------------------------------------
	AtomicElement(const std::string& aName = "",
			const std::string& aSymbol = "",
			unsigned short aZNumber = 0,
			double aMolarMass = 0,
			double aDensity = 0,
			const std::string& aState = "");


	//---------------------------------------------------------------------------
	///	Copy Constructor
	/*
	*	@param anElement:	the element to copy
	*/
	//---------------------------------------------------------------------------
	AtomicElement(const AtomicElement& anElement);


	//---------------------------------------------------------------------------
	///	Destructor
	//---------------------------------------------------------------------------
	~AtomicElement();


	//---------------------------------------------------------------------------
	///	Set the name of the element.
	/*
	*	@param aName:	the name of the element
	*/
	//---------------------------------------------------------------------------
	void setName(const std::string& aName);


	//---------------------------------------------------------------------------
	///	Set the symbol of the element.
	/*
	*	@param aSymbol:	the symbol of the element
	*/
	//---------------------------------------------------------------------------
	void setSymbol(const std::string& aSymbol);


	//---------------------------------------------------------------------------
	///	Set the atomic number (Z) of the element.
	/*
	*	@param aZNumber:	the atomic number (Z) of the element
	*/
	//---------------------------------------------------------------------------
	void setAtomicNumber(unsigned short aZNumber);


	//---------------------------------------------------------------------------
	///	Set the molar mass of the element.
	/*
	*	@param aMolarMass:	the molar mass of the element
	*/
	//---------------------------------------------------------------------------
	void setMolarMass(double aMolarMass);


	//---------------------------------------------------------------------------
	///	Set the density of the element.
	/*
	*	@param aDensity:	the density of the element
	*/
	//---------------------------------------------------------------------------
	void setDensity(double aDensity);


	//---------------------------------------------------------------------------
	///	Set the state of the element.
	/*
	*	@param aState:	the state of the element
	*/
	//---------------------------------------------------------------------------
	void setState(const std::string& aState);


	//---------------------------------------------------------------------------
	///	Get the mass attenuation coefficient at a given energy (values are interpolated).
	/*
	*	@param anEnergy:	the energy	
	*	@return	the corresponding linear attenuation coefficient
	*/
	//---------------------------------------------------------------------------
	double getMassAttenuationTotal(double anEnergy) const;


	//---------------------------------------------------------------------------
	///	Get the linear attenuation coefficient at a given energy (in mm-1) (values are interpolated).
	/*
	*	@param anEnergy:	the energy	
	*	@return	the corresponding linear attenuation coefficient
	*/
	//---------------------------------------------------------------------------
	double getMU(double anEnergy) const;


	//---------------------------------------------------------------------------
	///	Get a Compton scattering cross section sample (values are interpolated).
	/*
	*	@param x:	the sample
	*	@return	the Compton scattering cross section value
	*/
	//---------------------------------------------------------------------------
	double getS(double x) const;


	//---------------------------------------------------------------------------
	///	Accessor on the name of the element.
	/*
	*	@return the name of the element
	*/
	//---------------------------------------------------------------------------
	const std::string& getName() const;


	//---------------------------------------------------------------------------
	///	Accessor on the symbol of the element.
	/*
	*	@return the symbol of the element
	*/
	//---------------------------------------------------------------------------
	const std::string& getSymbol() const;


	//---------------------------------------------------------------------------
	///	Accessor on the atomic number (Z) of the element.
	/*
	*	@return the atomic number (Z) of the element
	*/
	//---------------------------------------------------------------------------
	unsigned short getAtomicNumber() const;


	//---------------------------------------------------------------------------
	///	Accessor on the molar mass of the element.
	/*
	*	@return the molar mass of the element
	*/
	//---------------------------------------------------------------------------
	double getMolarMass() const;


	//---------------------------------------------------------------------------
	///	Accessor on the density of the element.
	/*
	*	@return the density of the element
	*/
	//---------------------------------------------------------------------------
	double getDensity() const;


	//---------------------------------------------------------------------------
	///	Accessor on the state of the element.
	/*
	*	@return the state of the element
	*/
	//---------------------------------------------------------------------------
	const std::string& getState() const;


	//---------------------------------------------------------------------------
	///	Accessor on the number of energy channels in the attenuation coefficient list.
	/*
	*	@return the number of energy channels
	*/
	//---------------------------------------------------------------------------
	unsigned int getEnergyChannelNumber() const;


	//---------------------------------------------------------------------------
	///	Accessor on the smallest energy step in the linear attenuation coefficient list.
	/*
	*	@return the smallest energy step
	*/
	//---------------------------------------------------------------------------
	double getSmallestEnergyStep() const;
	

	//---------------------------------------------------------------------------
	///	Copy operator
	/*
	*	@param anElement:	the element to copy
	*/
	//---------------------------------------------------------------------------
	AtomicElement& operator=(const AtomicElement& anElement);


	//---------------------------------------------------------------------------
	///	operator <<
	/*
	*	@param anOutputSream:	the output stream
	*	@param anElement:	the element to output
	*	@return the output stream
	*/
	//---------------------------------------------------------------------------
	friend std::ostream & operator << (std::ostream& anOutputSream,
			const AtomicElement& anElement);


	//---------------------------------------------------------------------------
	///	operator >>
	/*
	*	@param anInputSream:	the input stream
	*	@param anElement:	the element to read
	*	@return the input stream
	*/
	//---------------------------------------------------------------------------
	friend std::istream & operator >> (std::istream& anInputSream,
			AtomicElement& anElement);


//*****************************************************************************
private:
    std::string    m_name;                  ///< The name of the element
    std::string    m_symbol;                ///< The symbol of the element
    unsigned short m_atomic_number;         ///< The atomic number of the element
    double         m_molar_mass;            ///< The molar mass of the element
    double         m_density;               ///< The density of the element
    std::string    m_state;                 ///< The state of the element
    unsigned int   m_energy_channel_number; ///< The number of energy channels in the list of linear attenuation coefficients
    double         m_smallest_energy_step;  ///< The smallest energy step in the list of linear attenuation coefficients
};


} // namespace gVirtualXRay


//******************************************************************************
#include "AtomicElement.inl"


#endif // GVXR_ATOMIC_ELEMENT_H

