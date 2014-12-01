/*

Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
http://www.fpvidal.net/
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the Bangor University nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


/**
********************************************************************************
*
*   @file       TissueMaterial.inl
*
*   @brief      Class to manage a material database. It implements the method
*               as follows:
*               Schneider W, Bortfeld T, Schlegel W.:
*               ``Correlation between CT numbers and tissue parameters needed
*               for Monte Carlo simulations of clinical dose distributions'',
*               Phys Med Biol. 2000 Feb;45(2):459-78.
*
*   @version    1.0
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
*               http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifndef GVXR_TYPES_H
#include "Types.h"
#endif

#ifndef GVXR_ELEMENT_SET_H
#include "ElementSet.h"
#endif

#ifndef GVXR_CONSTANT_VALUES_H
#include "gVirtualXRay/ConstantValues.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//------------------------------------------------------
inline TissueMaterial::TissueMaterial(short aMinHUValue,
                                      short aMaxHUValue,
                                      double H,
                                      double C,
                                      double N,
                                      double O,
                                      double Na,
                                      double Mg,
                                      double P,
                                      double S,
                                      double Cl,
                                      double Ar,
                                      double K,
                                      double Ca,
                                      double Ti,
                                      double Cu,
                                      double Zn,
                                      double Ag,
                                      double Sn):
//------------------------------------------------------
        m_min_HU_value(aMinHUValue),
        m_max_HU_value(aMaxHUValue),
        m_molar_mass(0)
//------------------------------------------------------
{
    m_p_weight_set[0]  =  H;
    m_p_weight_set[1]  =  C;
    m_p_weight_set[2]  =  N;
    m_p_weight_set[3]  =  O;
    m_p_weight_set[4]  = Na;
    m_p_weight_set[5]  = Mg;
    m_p_weight_set[6]  =  P;
    m_p_weight_set[7]  =  S;
    m_p_weight_set[8]  = Cl;
    m_p_weight_set[9]  = Ar;
    m_p_weight_set[10] =  K;
    m_p_weight_set[11] = Ca;
    m_p_weight_set[12] = Ti;
    m_p_weight_set[13] = Cu;
    m_p_weight_set[14] = Zn;
    m_p_weight_set[15] = Ag;
    m_p_weight_set[16] = Sn;

	normaliseWeightSet();
	
	m_molar_mass = computeMolarMass();
}

//---------------------------------------------------------------------
inline TissueMaterial::TissueMaterial(const TissueMaterial& aMaterial):
//---------------------------------------------------------------------
        m_min_HU_value(aMaterial.m_min_HU_value),
        m_max_HU_value(aMaterial.m_max_HU_value),
        m_molar_mass(aMaterial.m_molar_mass)
//---------------------------------------------------------------------
{
	memcpy(m_p_weight_set, aMaterial.m_p_weight_set, sizeof(m_p_weight_set));
}


//----------------------------------------------------------------------------
inline void TissueMaterial::registerElementSet(const ElementSet* apElementSet)
//----------------------------------------------------------------------------
{
    m_p_element_set = const_cast<ElementSet*>(apElementSet);
}


//----------------------------------------------
inline void TissueMaterial::normaliseWeightSet()
//----------------------------------------------
{
	double sum(0);

	for (int i(0); i < NUMBER_OF_ELEMENTS_PER_MATERIAL; ++i)
	{
		sum += m_p_weight_set[i];
	}

	if (sum > EPSILON)
	{
		for (int i(0); i < NUMBER_OF_ELEMENTS_PER_MATERIAL; ++i)
		{
			m_p_weight_set[i] /= sum;
		}
	}
	m_molar_mass = computeMolarMass();
}


//------------------------------------------------
inline short TissueMaterial::getMinHUValue() const
//------------------------------------------------
{
	return (m_min_HU_value);
}
	
	
//------------------------------------------------
inline short TissueMaterial::getMaxHUValue() const
//------------------------------------------------
{
	return (m_max_HU_value);
}
	
	
//-------------------------------------------------------
inline const double* TissueMaterial::getWeightSet() const
//-------------------------------------------------------
{
	return (m_p_weight_set);
}


//----------------------------------------------
inline double TissueMaterial::computeMolarMass()
//----------------------------------------------
{
    if (!m_p_element_set)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "No element set has been registered.");
    }

    m_molar_mass =
            m_p_weight_set[ 0] * m_p_element_set->getElement(Z_H).getMolarMass() +
            m_p_weight_set[ 1] * m_p_element_set->getElement(Z_C).getMolarMass() +
            m_p_weight_set[ 2] * m_p_element_set->getElement(Z_N).getMolarMass() +
            m_p_weight_set[ 3] * m_p_element_set->getElement(Z_O).getMolarMass() +
            m_p_weight_set[ 4] * m_p_element_set->getElement(Z_Na).getMolarMass() +
            m_p_weight_set[ 5] * m_p_element_set->getElement(Z_Mg).getMolarMass() +
            m_p_weight_set[ 6] * m_p_element_set->getElement(Z_P).getMolarMass() +
            m_p_weight_set[ 7] * m_p_element_set->getElement(Z_S).getMolarMass() +
            m_p_weight_set[ 8] * m_p_element_set->getElement(Z_Cl).getMolarMass() +
            m_p_weight_set[ 9] * m_p_element_set->getElement(Z_Ar).getMolarMass() +
            m_p_weight_set[10] * m_p_element_set->getElement(Z_K).getMolarMass() +
            m_p_weight_set[11] * m_p_element_set->getElement(Z_Ca).getMolarMass() +
            m_p_weight_set[12] * m_p_element_set->getElement(Z_Ti).getMolarMass() +
            m_p_weight_set[13] * m_p_element_set->getElement(Z_Cu).getMolarMass() +
            m_p_weight_set[14] * m_p_element_set->getElement(Z_Zn).getMolarMass() +
            m_p_weight_set[15] * m_p_element_set->getElement(Z_Ag).getMolarMass() +
            m_p_weight_set[16] * m_p_element_set->getElement(Z_Sn).getMolarMass();

	return (m_molar_mass);
}


//-----------------------------------------------
inline double TissueMaterial::getMolarMass() const
//-----------------------------------------------
{
	return (m_molar_mass);
}


//-------------------------------------------------------------
inline double TissueMaterial::getDensity(short aHounsfieldValue)
//-------------------------------------------------------------
{
	double density(0);
	
	if (aHounsfieldValue <= -1000)
	{
		density = 1.21e-3;
	}
	else if (-1000 <= aHounsfieldValue && aHounsfieldValue < -98)
	{
		// interpolate mass densities of air (1.21e-3 g/cm3) and adipose tissue 3 (0.93 g/cm3)
		density = 1.21e-3 + (aHounsfieldValue - -1000) * (0.93 - 1.21e-3) / (-98 - -1000);
	}
	else if (-98 <= aHounsfieldValue && aHounsfieldValue <= 14)
	{
		// Use of Eq. 21 in W Schneider et al 2000
		density = 1.018 + 0.893e-3 * aHounsfieldValue;
	}
	else if (14 < aHounsfieldValue && aHounsfieldValue < 23)
	{
		density = 1.03;
	}
	else if (23 <= aHounsfieldValue && aHounsfieldValue <= 100)
	{
		// Use of Eq. 23 in W Schneider et al 2000
		density = 1.003 + 1.169e-3 * aHounsfieldValue;
	}
	else if (100 <= aHounsfieldValue)
	{
		// Use of Eq. 19 in W Schneider et al 2000
		density = 1.017 + 0.592e-3 * aHounsfieldValue;
	}
	
	return (density * (g / cm3));
}	


//------------------------------------------------------------------------------------------------
inline double TissueMaterial::getMassAttenuationTotal(short aHounsfieldValue, double anEnergy) const
//------------------------------------------------------------------------------------------------
{
    if (!m_p_element_set)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "No element set has been registered.");
    }

	return (m_p_weight_set[ 0] * m_p_element_set->getElement(Z_H).getMassAttenuationTotal(anEnergy)  +
            m_p_weight_set[ 1] * m_p_element_set->getElement(Z_C).getMassAttenuationTotal(anEnergy)  +
            m_p_weight_set[ 2] * m_p_element_set->getElement(Z_N).getMassAttenuationTotal(anEnergy)  +
            m_p_weight_set[ 3] * m_p_element_set->getElement(Z_O).getMassAttenuationTotal(anEnergy)  +
            m_p_weight_set[ 4] * m_p_element_set->getElement(Z_Na).getMassAttenuationTotal(anEnergy) +
            m_p_weight_set[ 5] * m_p_element_set->getElement(Z_Mg).getMassAttenuationTotal(anEnergy) +
            m_p_weight_set[ 6] * m_p_element_set->getElement(Z_P).getMassAttenuationTotal(anEnergy)  +
            m_p_weight_set[ 7] * m_p_element_set->getElement(Z_S).getMassAttenuationTotal(anEnergy)  +
            m_p_weight_set[ 8] * m_p_element_set->getElement(Z_Cl).getMassAttenuationTotal(anEnergy) +
            m_p_weight_set[ 9] * m_p_element_set->getElement(Z_Ar).getMassAttenuationTotal(anEnergy) +
            m_p_weight_set[10] * m_p_element_set->getElement(Z_K).getMassAttenuationTotal(anEnergy)  +
            m_p_weight_set[11] * m_p_element_set->getElement(Z_Ca).getMassAttenuationTotal(anEnergy) +
            m_p_weight_set[12] * m_p_element_set->getElement(Z_Ti).getMassAttenuationTotal(anEnergy) +
            m_p_weight_set[13] * m_p_element_set->getElement(Z_Cu).getMassAttenuationTotal(anEnergy) +
            m_p_weight_set[14] * m_p_element_set->getElement(Z_Zn).getMassAttenuationTotal(anEnergy) +
            m_p_weight_set[15] * m_p_element_set->getElement(Z_Ag).getMassAttenuationTotal(anEnergy) +
            m_p_weight_set[16] * m_p_element_set->getElement(Z_Sn).getMassAttenuationTotal(anEnergy)
	);
}


//------------------------------------------------------------------------------
inline double TissueMaterial::getMU(short aHounsfieldValue, double anEnergy) const
//------------------------------------------------------------------------------
{
	return (getMassAttenuationTotal(aHounsfieldValue, anEnergy) * (getDensity(aHounsfieldValue)));
}


//-------------------------------------------------------------------------------
inline TissueMaterial& TissueMaterial::operator=(const TissueMaterial& aMaterial)
//-------------------------------------------------------------------------------
{
	m_min_HU_value = aMaterial.m_min_HU_value;
	m_max_HU_value = aMaterial.m_max_HU_value;
	m_molar_mass   = aMaterial.m_molar_mass;
	memcpy(m_p_weight_set, aMaterial.m_p_weight_set, sizeof(m_p_weight_set));

	return (*this);
}


//-----------------------------------------------------------------------
inline std::ostream & operator << (std::ostream& anOutputSream,
                                   const TissueMaterial& aTissueMaterial)
//-----------------------------------------------------------------------
{
	anOutputSream << aTissueMaterial.m_min_HU_value     << "\t";
	anOutputSream << aTissueMaterial.m_max_HU_value     << "\t";
	anOutputSream << aTissueMaterial.m_molar_mass << "\t";
	anOutputSream << aTissueMaterial.m_p_weight_set[ 0] << "\t";
	anOutputSream << aTissueMaterial.m_p_weight_set[ 1] << "\t";
	anOutputSream << aTissueMaterial.m_p_weight_set[ 2] << "\t";
	anOutputSream << aTissueMaterial.m_p_weight_set[ 3] << "\t";
	anOutputSream << aTissueMaterial.m_p_weight_set[ 4] << "\t";
	anOutputSream << aTissueMaterial.m_p_weight_set[ 5] << "\t";
	anOutputSream << aTissueMaterial.m_p_weight_set[ 6] << "\t";
	anOutputSream << aTissueMaterial.m_p_weight_set[ 7] << "\t";
	anOutputSream << aTissueMaterial.m_p_weight_set[ 8] << "\t";
	anOutputSream << aTissueMaterial.m_p_weight_set[ 9] << "\t";
	anOutputSream << aTissueMaterial.m_p_weight_set[10] << "\t";
	anOutputSream << aTissueMaterial.m_p_weight_set[11] << "\t";
	anOutputSream << aTissueMaterial.m_p_weight_set[12] << "\t";
	anOutputSream << aTissueMaterial.m_p_weight_set[13] << "\t";
	anOutputSream << aTissueMaterial.m_p_weight_set[14] << "\t";
	anOutputSream << aTissueMaterial.m_p_weight_set[15] << "\t";
	anOutputSream << aTissueMaterial.m_p_weight_set[16] << std::endl;
	
	return (anOutputSream);
}


//-----------------------------------------------------------------
inline std::istream & operator >> (std::istream& anInputSream,
                                   TissueMaterial& aTissueMaterial)
//-----------------------------------------------------------------
{
	anInputSream >> aTissueMaterial.m_min_HU_value;
	anInputSream >> aTissueMaterial.m_max_HU_value;
	anInputSream >> aTissueMaterial.m_p_weight_set[ 0];
	anInputSream >> aTissueMaterial.m_p_weight_set[ 1];
	anInputSream >> aTissueMaterial.m_p_weight_set[ 2];
	anInputSream >> aTissueMaterial.m_p_weight_set[ 3];
	anInputSream >> aTissueMaterial.m_p_weight_set[ 4];
	anInputSream >> aTissueMaterial.m_p_weight_set[ 5];
	anInputSream >> aTissueMaterial.m_p_weight_set[ 6];
	anInputSream >> aTissueMaterial.m_p_weight_set[ 7];
	anInputSream >> aTissueMaterial.m_p_weight_set[ 8];
	anInputSream >> aTissueMaterial.m_p_weight_set[ 9];
	anInputSream >> aTissueMaterial.m_p_weight_set[10];
	anInputSream >> aTissueMaterial.m_p_weight_set[11];
	anInputSream >> aTissueMaterial.m_p_weight_set[12];
	anInputSream >> aTissueMaterial.m_p_weight_set[13];
	anInputSream >> aTissueMaterial.m_p_weight_set[14];
	anInputSream >> aTissueMaterial.m_p_weight_set[15];
	anInputSream >> aTissueMaterial.m_p_weight_set[16];
	
	aTissueMaterial.normaliseWeightSet();
	
	return (anInputSream);
}


} // namespace gVirtualXRay/
