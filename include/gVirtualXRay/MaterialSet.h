/**
********************************************************************************
*
*   @file       MaterialSet.h
*
*   @brief      Class to manage a table of materials.
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


#ifndef GVXR_MATERIAL_SET_H
#define GVXR_MATERIAL_SET_H


//******************************************************************************
//	Include
//******************************************************************************
#include <iostream>
#include <map>
#include <string>

#ifndef GVXR_TISSUE_MATERIAL_H
#include "TissueMaterial.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*	@class	MaterialSet
*	@brief	MaterialSet is a class to manage a table of materials.
*/
//==============================================================================
class MaterialSet
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	//---------------------------------------------------------------------------
	///	Default Constructor (prevent instanciation).
	//---------------------------------------------------------------------------
	MaterialSet();


	//--------------------------------------------------------------------------
	///	Copy constructor (prevent copy-construction).
	//--------------------------------------------------------------------------
	MaterialSet(const MaterialSet&);


	//---------------------------------------------------------------------------
	///	Constructor (prevent instanciation).
	/*
	*	@param aFileName:	the name of the file to load
	*/
	//---------------------------------------------------------------------------
	MaterialSet(const std::string& aFileName);


	//--------------------------------------------------------------------------
	///	Copy operator (prevent assignment)
	//--------------------------------------------------------------------------
	MaterialSet& operator=(const MaterialSet&);


	//---------------------------------------------------------------------------
	///	Destructor.
	//---------------------------------------------------------------------------
	~MaterialSet();


	//---------------------------------------------------------------------------
	///	Clear all the memory.
	//---------------------------------------------------------------------------
	void clearMemory();


	//---------------------------------------------------------------------------
	///	Load a file containing materials.
	/*
	*	@param aFileName:	the name of the file to load
	*/
	//---------------------------------------------------------------------------
	void loadFromFile(const std::string& aFileName);


    //---------------------------------------------------------------------------
    /// Add a material to the list.
    /*
    *   @param aMaterial    the material to add
    */
    //---------------------------------------------------------------------------
	void addMaterial(const TissueMaterial& aMaterial);


	//---------------------------------------------------------------------------
	///	Get the material corresponding to a given HU value.
	/*
	*	@param aHUValue:	the HU value of the material
	*/
	//---------------------------------------------------------------------------
	const TissueMaterial& getMaterial(short aHUValue) const;


	//---------------------------------------------------------------------------
	///	Accessor on the lower energy cutoff for attenuation coefficient.
	/*
	*	@return the lower energy cutoff for attenuation coefficient
	*/
	//---------------------------------------------------------------------------
	float getMuMinEnergy() const;


	//---------------------------------------------------------------------------
	///	Accessor on the higher energy cutoff for attenuation coefficient.
	/*
	*	@return the higher energy cutoff for attenuation coefficient
	*/
	//---------------------------------------------------------------------------
	float getMuMaxEnergy() const;


	//---------------------------------------------------------------------------
	///	Accessor on the minimum Hounsfield unit that the material set can handle.
	/*
	*	@return the minimum Hounsfield unit that the material set can handle
	*/
	//---------------------------------------------------------------------------
	short getMinHU() const;


	//---------------------------------------------------------------------------
	///	Accessor on the maximum Hounsfield unit that the material set can handle.
	/*
	*	@return the maximum Hounsfield unit that the material set can handle
	*/
	//---------------------------------------------------------------------------
	short getMaxHU() const;
	

	//---------------------------------------------------------------------------
	///	Accessor on the number of materials in the set.
	/*
	*	@return the number of materials
	*/
	//---------------------------------------------------------------------------
	unsigned int getMaterialNumber() const;


	//---------------------------------------------------------------------------
	///	operator <<
	/*
	*	@param anOutputSream:	the output stream
	*	@param anElement:	the element to output
	*	@return the output stream
	*/
	//---------------------------------------------------------------------------
	friend std::ostream & operator << (std::ostream& anOutputSream,
			const MaterialSet& anMaterialSet);


//*****************************************************************************
private:
	//---------------------------------------------------------------------------
	///	An indexed table containing elements.
	//---------------------------------------------------------------------------
	std::map<std::pair<short, short>, TissueMaterial> m_material_set;

	
	//---------------------------------------------------------------------------
	///	The lower energy cutoff for attenuation coefficient.
	//---------------------------------------------------------------------------
	float m_mu_min_energy;


	//---------------------------------------------------------------------------
	///	The higher energy cutoff for attenuation coefficient.
	//---------------------------------------------------------------------------
	float m_mu_max_energy;


	//---------------------------------------------------------------------------
	///	The lower cutoff for Compton scattering cross section samples.
	//---------------------------------------------------------------------------
	float m_S_min_x;


	//---------------------------------------------------------------------------
	///	The higher cutoff for Compton scattering cross section samples.
	//---------------------------------------------------------------------------
	float m_S_max_x;


	//---------------------------------------------------------------------------
	///	The minimum Hounsfield unit that the material set can handle.
	//---------------------------------------------------------------------------
	short m_min_HU;


	//---------------------------------------------------------------------------
	///	The maximum Hounsfield unit that the material set can handle.
	//---------------------------------------------------------------------------
	short m_max_HU;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "MaterialSet.inl"


#endif // GVXR_MATERIAL_SET_H

