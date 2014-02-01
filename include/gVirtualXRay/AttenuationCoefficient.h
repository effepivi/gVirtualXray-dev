/**
********************************************************************************
*
*	@file		AttenuationCoefficient.h
*
*	@brief		Class to manage an attenuation coefficient
*
*	@version	1.0
*
*	@date		30/11/2013
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


#ifndef ATTENUATION_COEFFICIENT_H
#define ATTENUATION_COEFFICIENT_H


//******************************************************************************
//	Include
//******************************************************************************
#include <iostream>

#ifndef TYPES_H
#include "gVirtualXRay/Types.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//******************************************************************************
//	Class declaration
//******************************************************************************
class	AttenuationCoefficient;


//******************************************************************************
//	Function declaration
//******************************************************************************
//------------------------------------------------------------------------------
///	operator>>
/**
*	@param is: the input stream
*	@param obj: the record to load from the stream
*	@return the input stream
*/
//------------------------------------------------------------------------------
std::istream& operator>>(std::istream& is, AttenuationCoefficient& obj);


//------------------------------------------------------------------------------
///	operator<<
/**
*	@param os: the output stream
*	@param obj: the record to store in the stream
*	@return the output stream
*/
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const AttenuationCoefficient& obj);


//==============================================================================
/**
*	@class	AttenuationCoefficient
*	@brief	AttenuationCoefficient is a class to handle an attenuation coefficient.
*/
//==============================================================================
class AttenuationCoefficient
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	//--------------------------------------------------------------------------
	///	Default constructor.
	/**
	*	@param anEnergy: the energy (default value: 0)
	*	@param anAttenuationCoefficient: the attenuation coefficient (default value: 0)
	*/
	//--------------------------------------------------------------------------
	AttenuationCoefficient(const RATIONAL_NUMBER& anEnergy = 0,
			const RATIONAL_NUMBER& anAttenuationCoefficient = 0);


	//--------------------------------------------------------------------------
	///	Accessor on the energy.
	/**
	*	@return the energy
	*/
	//--------------------------------------------------------------------------
	RATIONAL_NUMBER getEnergy() const;


	//--------------------------------------------------------------------------
	///	Accessor on the attenuation coefficient.
	/**
	*	@return the attenuation coefficient
	*/
	//--------------------------------------------------------------------------
	RATIONAL_NUMBER getAttenuationCoefficient() const;


//******************************************************************************
protected:
	RATIONAL_NUMBER m_energy;
	RATIONAL_NUMBER m_mu;


	//**************************************************************************
	//	Friend operators
	//**************************************************************************
	friend std::istream& operator>>(std::istream &is, AttenuationCoefficient &obj);
	friend std::ostream& operator<<(std::ostream &os, const AttenuationCoefficient &obj);
};


} // namespace gVirtualXRay


//******************************************************************************
#include "AttenuationCoefficient.inl"


#endif // ATTENUATION_COEFFICIENT_H

