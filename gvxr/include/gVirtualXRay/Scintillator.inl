/*

Copyright (c) 2022, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*   @file       Scintillator.inl
*
*   @brief      Class to generate the energy response due to
*               the scintillator of the X-Ray detector.
*
*   @version    1.0
*
*   @date       18/10/2022
*
*   @author     Dr Franck P. Vidal
*
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*   http://www.fpvidal.net/, Oct 2022, 2022, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <cmath>
#include <xraylib.h>

#ifndef __Exception_h
#include "gVirtualXRay/Exception.h"
#endif

#ifndef __Units_h
#include "gVirtualXRay/Units.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//--------------------------------------------------
inline std::vector<double> logspace(const double &a,
                                    const double &b,
                                    const int &k)
//--------------------------------------------------
{
    std::vector<double> logspace;
    for (int i = 0; i < k; i++)
    {
        logspace.push_back(pow(10, i * (b - a) / (k - 1)));
    }
    return logspace;
}


//----------------------------------
inline Scintillator::Scintillator():
//----------------------------------
    m_thickness(0.0),
    m_density(0.0)
//----------------------------------
{}


//----------------------------------
inline Scintillator::~Scintillator()
//----------------------------------
{
    release();
}


//---------------------------------
inline void Scintillator::release()
//---------------------------------
{
    m_material.clear();
    m_thickness = 0.0;
    m_density = 0.0;
    m_p_energy_response.clear();
}


//-----------------------------------------------------------------
inline void Scintillator::setMaterial(const std::string& aMaterial)
//-----------------------------------------------------------------
{
    m_p_energy_response.clear();
    m_material.clear();

    if (aMaterial == "CsI")
    {
        m_material = aMaterial;
        m_density = 4.51 * g / cm3;
    }
    else
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            std::string("This material (") + aMaterial + ") is not supported. Contact Franck Vidal if you think it should be added.");
    }

    // m_photon_cross_section.useCompound(m_material);
    // m_photon_cross_section.setDensity(m_density);
}


//--------------------------------------------------------------------------
inline void Scintillator::setThickness(const RATIONAL_NUMBER& aThickness,
                                       const RATIONAL_NUMBER& aUnitOfLength)
//--------------------------------------------------------------------------
{
    m_p_energy_response.clear();
    m_thickness = aThickness * aUnitOfLength;
}


//---------------------------------------------------------
inline const std::string& Scintillator::getMaterial() const
//---------------------------------------------------------
{
    return m_material;
}

//-------------------------------------------------------------------------------------
inline RATIONAL_NUMBER Scintillator::getThickness(const RATIONAL_NUMBER& aUnitOfLength)
//-------------------------------------------------------------------------------------
{
    return m_thickness / aUnitOfLength;
}


//------------------------------------------------------------------------------------------------------------------------------------
inline std::vector<std::pair<RATIONAL_NUMBER, RATIONAL_NUMBER> > Scintillator::getEnergyResponse(const RATIONAL_NUMBER& aUnitOfEnergy)
//------------------------------------------------------------------------------------------------------------------------------------
{
    // See email received on Fri 28/10/2022
    m_p_energy_response.clear();



    // avec mu et mu_en les coef de ton scintillateur (dépendant de E bien sûr) et T l'épaisseur de ton scintillateur. La valeur déposée est donc entre 0 et E, et tu peux stocker une simple LUT du genre Edep=f(E).
    // Dis-moi si qqch n'est pas claire. On essaiera de se planifier une visio en novembre pour la thèse.



    // Lazy evaluation
    // The energy response is empty
    if (m_p_energy_response.size() == 0)
    {
        // The material and its thickness exist
        // Evaluate the energy response
        if (m_material.size() > 0 && m_thickness > 1 * um)
        {
            std::vector<double> energy_range = logspace(0., 2.48, 1000);

            for (std::vector<double>::const_iterator ite_energy = energy_range.begin();
                ite_energy != energy_range.end();
                ++ite_energy)
            {
                // Energy in keV
                double energy = *ite_energy * keV;

                double response = 0.0;
                if (energy > EPSILON)
                {
                    // mu / rho in cm2/g
                    double mu_rho = CS_Total_CP(m_material.c_str(), energy / keV, 0);

                    // mu_en / rho in cm2/g
                    double mu_en_rho = CS_Energy_CP(m_material.c_str(), energy / keV, 0);

                    // rho in g/cm3
                    double density = m_density / (g / cm3);

                    // mu in cm-1
                    double mu = mu_rho * density;

                    // mu_en in cm-1
                    double mu_en = mu_en_rho * density;

                    // thickness in cm
                    double thickness = m_thickness / cm;

                    // Energy response
                    response = energy * (mu_en / mu) * (1.0 - exp(-mu * thickness));
                }
                m_p_energy_response.push_back(std::pair<RATIONAL_NUMBER, RATIONAL_NUMBER>(energy, response));
            }
        }
    }

    std::vector<std::pair<RATIONAL_NUMBER, RATIONAL_NUMBER> > p_energy_response;

    for (std::vector<std::pair<RATIONAL_NUMBER, RATIONAL_NUMBER> >::iterator ite = m_p_energy_response.begin();
        ite != m_p_energy_response.end();
        ++ite)
    {
        p_energy_response.push_back(std::pair<RATIONAL_NUMBER, RATIONAL_NUMBER>(ite->first / aUnitOfEnergy, ite->second / aUnitOfEnergy));
    }

    return p_energy_response;
}


} // namespace gVirtualXRay
