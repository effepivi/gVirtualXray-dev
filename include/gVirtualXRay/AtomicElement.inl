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
        m_state(aState),
        m_energy_channel_number(0),
        m_smallest_energy_step(-1)
//-------------------------------------------------------------
{
}


//------------------------------------------------------------------
inline AtomicElement::AtomicElement(const AtomicElement& anElement):
//------------------------------------------------------------------
        m_name(anElement.m_name),
        m_symbol(anElement.m_symbol),
        m_atomic_number(anElement.m_atomic_number),
        m_molar_mass(anElement.m_molar_mass),
        m_density(anElement.m_density),
        m_state(anElement.m_state),
        //m_p_MU_coefficient_list(0),
        m_energy_channel_number(anElement.m_energy_channel_number),
        m_smallest_energy_step(anElement.m_smallest_energy_step)
//------------------------------------------------------------------
{
    /*if (anElement.m_p_MU_coefficient_list && anElement.m_energy_channel_number)
	{
		m_p_MU_coefficient_list = new E_MU[anElement.m_energy_channel_number];

		if (!m_p_MU_coefficient_list)
		{
			throw Exception("ERROR: out of memory, cannot allocate the list of attenuation coefficients.", __FILE__, __FUNCTION__, __LINE__);
		}

		memcpy(m_p_MU_coefficient_list, anElement.m_p_MU_coefficient_list, anElement.m_energy_channel_number * sizeof(E_MU));
	}*/
}


//------------------------------------
inline AtomicElement::~AtomicElement()
//------------------------------------
{
	/*if (m_p_MU_coefficient_list)
	{
		delete [] m_p_MU_coefficient_list;
	}*/
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


//-------------------------------------------------------
inline double AtomicElement::getMU(double anEnergy) const
//-------------------------------------------------------
{
    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented");

    // The list is empty
	/*if (!m_energy_channel_number || !m_p_MU_coefficient_list)
	{
		throw Exception("ERROR: the list of total attenuation coefficients does not exist.", __FILE__, __FUNCTION__, __LINE__);
	}

	// The requested energy is too low
	if (anEnergy <= m_p_MU_coefficient_list[0].E * MeV)
	{
		if (abs(anEnergy - m_p_MU_coefficient_list[0].E * MeV) > EPSILON)
		{
			throw Exception("ERROR: the list of total attenuation coefficients does not include the requested energy.", __FILE__, __FUNCTION__, __LINE__);
		}
		else
		{
			return (m_p_MU_coefficient_list[0].MU_per_mm;
		}
	}

	// The requested energy is too high
	if (anEnergy >= m_p_MU_coefficient_list[m_energy_channel_number - 1].E * MeV)
	{
		if (abs(anEnergy - m_p_MU_coefficient_list[m_energy_channel_number - 1].E * MeV) > EPSILON)
		{
			throw Exception("ERROR: the list of total attenuation coefficients does not include the requested energy.", __FILE__, __FUNCTION__, __LINE__);
		}
		else
		{
			return (m_p_MU_coefficient_list[m_energy_channel_number - 1].MU_per_mm;
		}
	}

	// Perform a dichotomic search
	int high, i, low;

    for (low = -1, high = m_energy_channel_number - 1;  high - low > 1;)
    {
    	i = (high + low) / 2;

    	if (anEnergy <= m_p_MU_coefficient_list[i].E * MeV)
    	{
    		high = i;
    	}
    	else
    	{
    		low  = i;
    	}
    }

	if (low < 0) return (m_p_MU_coefficient_list[0].MU_per_mm;
	if (high >= m_energy_channel_number) return (m_p_MU_coefficient_list[m_energy_channel_number - 1].MU_per_mm;
	
	return (m_p_MU_coefficient_list[low].MU_per_mm +
		(anEnergy - m_p_MU_coefficient_list[low].E * MeV) *
		(m_p_MU_coefficient_list[high].MU_per_mm - m_p_MU_coefficient_list[low].MU_per_mm) /
		(m_p_MU_coefficient_list[high].E * MeV - m_p_MU_coefficient_list[low].E * MeV);*/
}


//-------------------------------------------------------------------------
inline double AtomicElement::getMassAttenuationTotal(double anEnergy) const
//-------------------------------------------------------------------------
{
	return (getMU(anEnergy) / ((m_density / (g/cm3)) * 0.1));
}


//-----------------------------------------------
inline double AtomicElement::getS(double x) const
//-----------------------------------------------
{
    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented");
/*
    // The list is empty
	if (!m_x_S_List.size())
	{
		throw Exception("ERROR: the list of x S factors does not exist.", __FILE__, __FUNCTION__, __LINE__);
	}

	// The requested energy is too low
	if (x < m_x_S_List[0].numtype1)
	{
		if (abs(x - m_x_S_List[0].numtype1) > EPSILON)
		{
			throw Exception("ERROR: the list of total attenuation coefficients does not include the requested energy.", __FILE__, __FUNCTION__, __LINE__);
		}
		else
		{
			return (m_x_S_List[0].numtype2;
		}
	}

	// The requested energy is too high
	if (x > m_x_S_List[m_x_S_List.size() - 1].numtype1)
	{
		if (abs(x - m_x_S_List[m_x_S_List.size() - 1].numtype1) > EPSILON)
		{
			throw Exception("ERROR: the list of total attenuation coefficients does not include the requested energy.", __FILE__, __FUNCTION__, __LINE__);
		}
		else
		{
			return (m_x_S_List[m_x_S_List.size() - 1].numtype2;
		}
	}

	// Perform a dichotomic search
	int high, i, low;

    for (low = -1, high = m_x_S_List.size() - 1;  high - low > 1;)
    {
    	i = (high + low) / 2;

    	if (x <= m_x_S_List[i].numtype1)
    	{
    		high = i;
    	}
    	else
    	{
    		low  = i;
    	}
    }

	if (low < 0) return (m_x_S_List[0].numtype2;
	if (high >= m_x_S_List.size()) return (m_x_S_List[m_x_S_List.size() - 1].numtype2;

	return (m_x_S_List[low].numtype2 +
    		(x - m_x_S_List[low].numtype1) *
    		(m_x_S_List[high].numtype2 - m_x_S_List[low].numtype2) /
    		(m_x_S_List[high].numtype1 - m_x_S_List[low].numtype1);*/
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
	return (m_energy_channel_number);
}

/*
//---------------------------------------------------
inline const E_MU* AtomicElement::GetE_MUList() const
//---------------------------------------------------
{
	return (m_p_MU_coefficient_list;
}
*/

//--------------------------------------------------------
inline double AtomicElement::getSmallestEnergyStep() const
//--------------------------------------------------------
{
	return (m_smallest_energy_step);
}

/*
//---------------------------------------------------------------
inline std::vector<numtype1_numtype2>& AtomicElement::GetXSList()
//---------------------------------------------------------------
{
	return (m_x_S_List;
}*/

/*
//---------------------------------------------------------------------------
inline const std::vector<numtype1_numtype2>& AtomicElement::GetXSList() const
//---------------------------------------------------------------------------
{
	return (m_x_S_List;
}
*/

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
	m_energy_channel_number = anElement.m_energy_channel_number;

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

	return (anInputSream);
}


} // namespace gVirtualXRay
