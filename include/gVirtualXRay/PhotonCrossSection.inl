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
 *   @file       PhotonCrossSection.inl
 *
 *   @brief      Class to manage photon cross sections of elements, compounds and mixtures.
 *
 *   @version    1.0
 *
 *   @date       27/09/2018
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


//******************************************************************************
//  defines
//******************************************************************************


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {
/*
    bool  m_use_HU;
    bool  m_use_element;
    bool  m_use_mixture;
    short m_HU;
    std::vector<std::pair<int, double> > m_p_weight_set;
    double m_density;
*/
    double PhotonCrossSection::getMassAttenuationCoefficient(double anEnergy) const
    {
        if (m_use_HU)
        {
            return (MaterialSet::getInstance().getMaterial(m_HU).getMassAttenuationCoefficient(anEnergy));
        }
        else if (m_use_element)
        {
            return (ElementSet::getInstance().getElement(m_Z_number).getMassAttenuationCoefficient(anEnergy));
        }
        else if (m_use_mixture)
        {
            return (m_mixture.getMassAttenuationCoefficient(anEnergy));
        }
        else
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "No properties set");
        }
    }
    
    
    double PhotonCrossSection::getLinearAttenuationCoefficient(double anEnergy) const
    {
        if (m_use_HU)
        {
            return (MaterialSet::getInstance().getMaterial(m_HU).getLinearAttenuationCoefficient(anEnergy));
        }
        else if (m_use_element)
        {
            return (ElementSet::getInstance().getElement(m_Z_number).getLinearAttenuationCoefficient(anEnergy));
        }
        else if (m_use_mixture)
        {
            return (m_mixture.getLinearAttenuationCoefficient(anEnergy));
        }
        else
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "No properties set");
        }
    }
    
    
    double PhotonCrossSection::getMu(double anEnergy) const
    {
        return (getLinearAttenuationCoefficient(anEnergy));
    }
    
    
    double PhotonCrossSection::getDensity(double anEnergy) const
    {
        if (m_density < -0.5)
        {
            if (m_use_HU)
            {
                return (MaterialSet::getInstance().getMaterial(m_HU).getDensity());
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
        
    }
} // namespace gVirtualXRay
