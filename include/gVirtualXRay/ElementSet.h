/**
********************************************************************************
*
*   @file       ElementSet.h
*
*   @brief      Class to manage a table of elements in material.
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


#ifndef GVXR_ELEMENT_SET_H
#define GVXR_ELEMENT_SET_H


//******************************************************************************
//	Include
//******************************************************************************
#include <iostream>
#include <map>
#include <string>

#ifndef GVXR_ATOMIC_ELEMENT_H
#include "AtomicElement.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*	@class	ElementSet
*	@brief	ElementSet is a class to manage a table of elements in material.
*/
//==============================================================================
class ElementSet
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //---------------------------------------------------------------------------
    /// Default Constructor (prevent instanciation).
    //---------------------------------------------------------------------------
    ElementSet();


    //--------------------------------------------------------------------------
    /// Copy constructor (prevent copy-construction).
    //--------------------------------------------------------------------------
    ElementSet(const ElementSet&);


    //---------------------------------------------------------------------------
    /// Constructor (prevent instanciation).
    /*
    *   @param aFileName:   the name of the file to load
    */
    //---------------------------------------------------------------------------
    ElementSet(const std::string& aFileName);


    //--------------------------------------------------------------------------
    /// Copy operator (prevent assignment).
    //--------------------------------------------------------------------------
    ElementSet& operator=(const ElementSet&);


	//---------------------------------------------------------------------------
	///	Destructor.
	//---------------------------------------------------------------------------
	~ElementSet();


	//---------------------------------------------------------------------------
	///	Clear all the memory.
	//---------------------------------------------------------------------------
	void clearMemory();


    //---------------------------------------------------------------------------
    /// Load a file containing elements.
    /*
    *   @param aFileName:   the name of the file to load
    */
    //---------------------------------------------------------------------------
    void loadFromFile(const std::string& aFileName);


    //---------------------------------------------------------------------------
    /// Add an element to the list.
    /*
    *   @param anElement    the element to add
    */
    //---------------------------------------------------------------------------
    void addElement(const AtomicElement& anElement);


	//---------------------------------------------------------------------------
	///	Get a given element.
	/*
	*	@param anAtomicNumber:	the atomic number (Z) of the element
	*	@return the element
	*/
	//---------------------------------------------------------------------------
	AtomicElement& getElement(unsigned short anAtomicNumber);


	//---------------------------------------------------------------------------
	///	Get a given element.
	/*
	*	@param anAtomicNumber:	the atomic number (Z) of the element
	*	@return the element
	*/
	//---------------------------------------------------------------------------
	const AtomicElement& getElement(unsigned short anAtomicNumber) const;


	//---------------------------------------------------------------------------
	///	Accessor on the molar mass of the element.
	/*
	*	@param anAtomicNumber:	the atomic number (Z) of the element
	*	@return the molar mass of the element
	*/
	//---------------------------------------------------------------------------
	float getMolarMass(unsigned short anAtomicNumber) const;


	//---------------------------------------------------------------------------
	///	Accessor on the density of the element.
	/*
	*	@param anAtomicNumber:	the atomic number (Z) of the element
	*	@return the density of the element
	*/
	//---------------------------------------------------------------------------
	float getDensity(unsigned short anAtomicNumber) const;


	//---------------------------------------------------------------------------
	///	Accessor on the number of elements in the set.
	/*
	*	@return	the number of elements in the set
	*/
	//---------------------------------------------------------------------------
	unsigned int getElementSetSize() const;


	//---------------------------------------------------------------------------
	///	operator <<
	/*
	*	@param anOutputSream:	the output stream
	*	@param anElement:	the element to output
	*	@return the output stream
	*/
	//---------------------------------------------------------------------------
	friend std::ostream & operator << (std::ostream& anOutputSream,
			const ElementSet& anElementSet);


//*****************************************************************************
private:
	//---------------------------------------------------------------------------
	///	Clear the dynamically allocated memory
	//---------------------------------------------------------------------------
	void clearDynamicallyMemory();


	//---------------------------------------------------------------------------
	///	An indexed table containing elements.
	//---------------------------------------------------------------------------
	std::map<unsigned short, AtomicElement> m_element_set;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "ElementSet.inl"


#endif // GVXR_ELEMENT_SET_H
