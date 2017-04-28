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
*   @file       TissueMaterial.cxx
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
#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef TISSUE_MATERIAL_H
#include "gVirtualXRay/TissueMaterial.h"
#endif


#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#ifndef GVXR_ATOMIC_ELEMENT_H
#include "gVirtualXRay/AtomicElement.h"
#endif

#ifndef GVXR_ELEMENT_SET_H
#include "gVirtualXRay/ElementSet.h"
#endif

#ifndef GVXR_CONSTANT_VALUES_H
#include "gVirtualXRay/ConstantValues.h"
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Static member
//******************************************************************************
ElementSet* TissueMaterial::m_p_element_set = 0;


//---------------------------------------
void TissueMaterial::normaliseWeightSet()
//---------------------------------------
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


//---------------------------------------
double TissueMaterial::computeMolarMass()
//---------------------------------------
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
            m_p_weight_set[17] * m_p_element_set->getElement(Z_Fe).getMolarMass();
            m_p_weight_set[18] * m_p_element_set->getElement(Z_I).getMolarMass();

    return (m_molar_mass);
}


//-------------------------------------------------------
double TissueMaterial::getDensity(short aHounsfieldValue)
//-------------------------------------------------------
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


//-------------------------------------------------------------------
double TissueMaterial::getMassAttenuationTotal(double anEnergy) const
//-------------------------------------------------------------------
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
            m_p_weight_set[16] * m_p_element_set->getElement(Z_Sn).getMassAttenuationTotal(anEnergy) +
            m_p_weight_set[17] * m_p_element_set->getElement(Z_Fe).getMassAttenuationTotal(anEnergy) +
            m_p_weight_set[18] * m_p_element_set->getElement(Z_I).getMassAttenuationTotal(anEnergy)
    );
}


//--------------------------------------------------------------------
bool TissueMaterial::operator==(const TissueMaterial& aMaterial) const
//--------------------------------------------------------------------
{
    if (m_min_HU_value != aMaterial.m_min_HU_value) return false;
    if (m_max_HU_value != aMaterial.m_max_HU_value) return false;

    for (unsigned int i(0); i < NUMBER_OF_ELEMENTS_PER_MATERIAL; ++i)
        if (m_p_weight_set[i] != aMaterial.m_p_weight_set[i]) return false;

    if (m_molar_mass != aMaterial.m_molar_mass) return false;
    if (m_density    != aMaterial.m_density)    return false;

    return true;
}
