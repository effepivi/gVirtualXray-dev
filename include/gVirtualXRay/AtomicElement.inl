/**
********************************************************************************
*
*   @file       AtomicElement.inl
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


//******************************************************************************
//	Include
//******************************************************************************
#include <math.h>

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif


//******************************************************************************
//	define
//******************************************************************************


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//-------------------------------------------------------------
inline AtomicElement::AtomicElement(const std::string& aName,
                                    const std::string& aSymbol,
                                    unsigned short aZNumber,
                                    double aMolarMass,
                                    double aDensity,
                                    const std::string& aState):
//-------------------------------------------------------------
        m_name(aName),
        m_symbol(aSymbol),
        m_atomic_number(aZNumber),
        m_molar_mass(aMolarMass),
        m_density(aDensity),
        m_state(aState)
//-------------------------------------------------------------
{
    // Load the data from the XCOM database if possible
    loadDataFromXCOM();
}


//------------------------------------------------------------------
inline AtomicElement::AtomicElement(const AtomicElement& anElement):
//------------------------------------------------------------------
        m_name(anElement.m_name),
        m_symbol(anElement.m_symbol),
        m_atomic_number(anElement.m_atomic_number),
        m_molar_mass(anElement.m_molar_mass),
        m_density(anElement.m_density),
        m_state(anElement.m_state)
//------------------------------------------------------------------
{
}


//------------------------------------
inline AtomicElement::~AtomicElement()
//------------------------------------
{
}


//----------------------------------------------------------
inline void AtomicElement::setName(const std::string& aName)
//----------------------------------------------------------
{
	m_name = aName;
}


//--------------------------------------------------------------
inline void AtomicElement::setSymbol(const std::string& aSymbol)
//--------------------------------------------------------------
{
	m_symbol = aSymbol;
}


//-----------------------------------------------------------------
inline void AtomicElement::setAtomicNumber(unsigned short aZNumber)
//-----------------------------------------------------------------
{
    m_atomic_number = aZNumber;

    // Load the data from the XCOM database if possible
    loadDataFromXCOM();
}


//--------------------------------------------------------
inline void AtomicElement::setMolarMass(double aMolarMass)
//--------------------------------------------------------
{
	m_molar_mass = aMolarMass;
}


//----------------------------------------------------
inline void AtomicElement::setDensity(double aDensity)
//----------------------------------------------------
{
	m_density = aDensity;
}


//------------------------------------------------------------
inline void AtomicElement::setState(const std::string& aState)
//------------------------------------------------------------
{
	m_state = aState;
}


//-------------------------------------------------
inline double AtomicElement::getMU(double anEnergy)
//-------------------------------------------------
{
    return (getMassAttenuationTotal(anEnergy) * m_density);
}


//-------------------------------------------------------------------
inline double AtomicElement::getMassAttenuationTotal(double anEnergy)
//-------------------------------------------------------------------
{
    // Load the data from the XCOM database if possible
    if (!m_p_energy_set.size() || !m_p_mass_attenuation_coefficient_set.size())
    {
        loadDataFromXCOM();
    }

    if (anEnergy <= m_p_energy_set.front())
    {
        return (m_p_mass_attenuation_coefficient_set.front());
    }
    else if (anEnergy >= m_p_energy_set.back())
    {
        return (m_p_mass_attenuation_coefficient_set.back());
    }
    else
    {
        double max_energy(m_p_energy_set[0]);
        double min_energy(max_energy);
        double max_mass_attenuation_coefficient(m_p_mass_attenuation_coefficient_set[0]);
        double min_mass_attenuation_coefficient(max_mass_attenuation_coefficient);

        int i(1);
        while (max_energy < anEnergy)
        {
            min_energy = max_energy;
            min_mass_attenuation_coefficient = max_mass_attenuation_coefficient;

            ++i;

            max_energy = m_p_energy_set[i];
            max_mass_attenuation_coefficient = m_p_mass_attenuation_coefficient_set[i];
        }

        return (exp(log(min_mass_attenuation_coefficient) + (log(max_mass_attenuation_coefficient) - log(min_mass_attenuation_coefficient)) * ((((anEnergy - min_energy)) / ((max_energy - min_energy))))));
    }
}


//------------------------------------------------------
inline const std::string& AtomicElement::getName() const
//------------------------------------------------------
{
    return (m_name);
}


//--------------------------------------------------------
inline const std::string& AtomicElement::getSymbol() const
//--------------------------------------------------------
{
    return (m_symbol);
}


//----------------------------------------------------------
inline unsigned short AtomicElement::getAtomicNumber() const
//----------------------------------------------------------
{
    return (m_atomic_number);
}


//-----------------------------------------------
inline double AtomicElement::getMolarMass() const
//-----------------------------------------------
{
    return (m_molar_mass);
}


//---------------------------------------------
inline double AtomicElement::getDensity() const
//---------------------------------------------
{
    return (m_density);
}


//-------------------------------------------------------
inline const std::string& AtomicElement::getState() const
//-------------------------------------------------------
{
	return (m_state);
}


//---------------------------------------------------------------
inline unsigned int AtomicElement::getEnergyChannelNumber() const
//---------------------------------------------------------------
{
	return (m_p_energy_set.size());
}


//----------------------------------------------------------------------------
inline AtomicElement& AtomicElement::operator=(const AtomicElement& anElement)
//----------------------------------------------------------------------------
{
	m_name = anElement.m_name;
	m_symbol = anElement.m_symbol;
	m_atomic_number = anElement.m_atomic_number;
	m_molar_mass = anElement.m_molar_mass;
	m_density = anElement.m_density;
	m_state = anElement.m_state;

	return (*this);
}


//----------------------------------------------------------------
inline std::ostream & operator << (std::ostream& anOutputSream,
                                   const AtomicElement& anElement)
//----------------------------------------------------------------
{
	anOutputSream << "Element: " <<  anElement.m_name << "(" << anElement.m_symbol << ")" << std::endl;
	anOutputSream << "\tatomic number (Z): " << anElement.m_atomic_number << std::endl;
	anOutputSream << "\tmolar mass (in g/mol): " << anElement.m_molar_mass / (g / mole) << std::endl;
	anOutputSream << "\tdensity (in g/cm3): " << anElement.m_density / (g/cm3) << std::endl;
	anOutputSream << "\tstate: " << anElement.m_state << std::endl;
	
	return (anOutputSream);
}


//------------------------------------------------------------
inline std::istream & operator >> (std::istream& anInputSream,
                                   AtomicElement& anElement)
//------------------------------------------------------------
{
	anInputSream >> anElement.m_name;
	anInputSream >> anElement.m_symbol;
	anInputSream >> anElement.m_atomic_number;
	anInputSream >> anElement.m_molar_mass;
	anInputSream >> anElement.m_density;
	anInputSream >> anElement.m_state;
	
	anElement.m_molar_mass *= g / mole;
	anElement.m_density    *= g / cm3;

    // Load the data from the XCOM database if possible
	anElement.loadDataFromXCOM();

	return (anInputSream);
}


} // namespace gVirtualXRay
