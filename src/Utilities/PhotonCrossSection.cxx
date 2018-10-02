/*

 Copyright (c) 2018, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
 *   @file       PhotonCrossSection.cxx
 *
 *   @brief      Class to manage photon cross sections of elements, compounds and mixtures.
 *
 *   @version    1.0
 *
 *   @date       02/10/2018
 *
 *
 *   @author     Dr Franck P. Vidal
 *
 *   License
 *   BSD 3-Clause License.
 *
 *   For details on use and redistribution please refer
 *   to http://opensource.org/licenses/BSD-3-Clause
 *
 *   Copyright
 *   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
 *   http://www.fpvidal.net/, Dec 2014, 2014, version 1.0, BSD 3-Clause License
 *
 ********************************************************************************
 */


//******************************************************************************
//  Include
//******************************************************************************
#ifndef __PhotonCrossSection_h
#include "gVirtualXRay/PhotonCrossSection.h"
#endif

#ifndef __Utilities_h
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef __Exception_h
#include "gVirtualXRay/Exception.h"
#endif

#ifndef __MaterialSet_h
#include "gVirtualXRay/MaterialSet.h"
#endif

#ifndef __ElementSet_h
#include "gVirtualXRay/ElementSet.h"
#endif


//******************************************************************************
//  defines
//******************************************************************************


//******************************************************************************
//  namespace
//******************************************************************************
using namespace gVirtualXRay;


//---------------------------------------------------------------
PhotonCrossSection::PhotonCrossSection(const std::string& aName):
//---------------------------------------------------------------
        m_use_HU(false),
        m_use_element(false),
        m_use_mixture(false),
        m_use_mass_attenuation_coefficient(false),
        m_use_linear_attenuation_coefficient(false),
        m_HU(0),
        m_Z_number(0),
        m_mass_attenuation_coefficient(0.0),
        m_linear_attenuation_coefficient(0.0),
        m_density(-1.0)
//---------------------------------------------------------------
{
    m_Z_number = ElementSet::getInstance().getElement(aName).Z();

    if (m_Z_number)
    {
        m_use_element = true;
    }
    else
    {
        std::map<int, double> weight_set = getWeightSet(aName);
        if (weight_set.size())
        {
            m_mixture.setMixture(weight_set);
            m_use_mixture = true;
        }
        else
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Did not recognise the material " + aName);
        }
    }
}


//-----------------------------------------------------------
void PhotonCrossSection::useElement(const std::string& aName)
//-----------------------------------------------------------
{
    reset();
    m_use_element = true;
    m_Z_number = ElementSet::getInstance().getElement(aName).Z();

    if (!m_Z_number)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Did not recognise the material " + aName);
    }
}


void PhotonCrossSection::useMixture(const std::string& aName)
{
    reset();
    m_use_mixture = true;
    std::map<int, double> weight_set = getWeightSet(aName);

    if (weight_set.size())
    {
        m_mixture.setMixture(weight_set);
    }
    else
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Did not recognise the material " + aName);
    }
}


double PhotonCrossSection::getMassAttenuationCoefficient(double anEnergy) const
{
    if (m_use_HU)
    {
        return (MaterialSet::getInstance().getMaterial(m_HU).getMassAttenuationTotal(anEnergy));
    }
    else if (m_use_element)
    {
        return (ElementSet::getInstance().getElement(m_Z_number).getMassAttenuationTotal(anEnergy));
    }
    else if (m_use_mixture)
    {
        return (m_mixture.getMassAttenuationTotal(anEnergy));
    }
    else
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "No properties set");
    }
}


double PhotonCrossSection::getLinearAttenuationCoefficient(double anEnergy) const
{
    if (m_use_linear_attenuation_coefficient)
    {
        return (m_linear_attenuation_coefficient);
    }

    double density = getDensity();

    if (density < 0.0)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "Cannot compute the value without knowing the density");
    }

    return (getMassAttenuationCoefficient(anEnergy) * density);
}


double PhotonCrossSection::getDensity() const
{
    if (m_density < -0.5)
    {
        if (m_use_HU)
        {
            return (MaterialSet::getInstance().getMaterial(m_HU).getDensity(m_HU));
        }
        else if (m_use_element)
        {
            return (ElementSet::getInstance().getElement(m_Z_number).getDensity());
        }
        else if (m_use_mixture)
        {
            return (m_mixture.getDensity());
        }
        else
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "No properties set");
        }
    }
    else
    {
        return (m_density);
    }
}


double PhotonCrossSection::getMolarMass() const
{
    if (m_use_HU)
    {
        return (MaterialSet::getInstance().getMaterial(m_HU).getMolarMass());
    }
    else if (m_use_element)
    {
        return (ElementSet::getInstance().getElement(m_Z_number).getMolarMass());
    }
    else if (m_use_mixture)
    {
        return (m_mixture.getMolarMass());
    }
    else
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "No properties set");
    }
}


std::map<int, double> PhotonCrossSection::getWeightSet(const std::string& aName)
{
	std::map<int, double> weight_set;

	std::string symbol_string;
	std::string weight_string;
	bool added_number = false;

	for (unsigned int i = 0; i < aName.size(); ++i)
	{
		// The current character is a letter
		if (isLetter(aName[i]))
		{
			if (weight_string.size())
			{
				processElement(symbol_string, weight_string, weight_set);
			}
			symbol_string += aName[i];
		}
		// The current character is a number
		else if (isNumber(aName[i]))
		{
			weight_string += aName[i];
		}
		// The current character is a separator
		else if (aName[i] == ' ' || aName[i] == '/' || aName[i] == '\\' || aName[i] == ',' || aName[i] == ';' || aName[i] == ':')
		{
			processElement(symbol_string, weight_string, weight_set);
		}
	}

	if (symbol_string.size() && symbol_string.size())
	{
		processElement(symbol_string, weight_string, weight_set);
	}

	return (weight_set);
}


//------------------------------------------------------------------------
void PhotonCrossSection::processElement(std::string& aSymbol,
									    std::string& aWeight,
										std::map<int, double>& aWeightSet)
//------------------------------------------------------------------------
{
	int Z = ElementSet::getInstance().getElement(aSymbol).Z();
	double weight = atof(aWeight.data());

	aWeightSet[Z] = weight;

	aSymbol.clear();
	aWeight.clear();
}

