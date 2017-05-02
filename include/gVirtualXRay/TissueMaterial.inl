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
//	Include
//******************************************************************************
#include <cstring>


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
                                      double Sn,
                                      double Fe,
                                      double I):
//------------------------------------------------------
        m_min_HU_value(aMinHUValue),
        m_max_HU_value(aMaxHUValue),
        m_molar_mass(0),
        m_density(-1)
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
    m_p_weight_set[17] = Fe;
    m_p_weight_set[18] = I;

	normaliseWeightSet();
	
	m_molar_mass = computeMolarMass();
}

//---------------------------------------------------------------------
inline TissueMaterial::TissueMaterial(const TissueMaterial& aMaterial):
//---------------------------------------------------------------------
        m_min_HU_value(aMaterial.m_min_HU_value),
        m_max_HU_value(aMaterial.m_max_HU_value),
        m_molar_mass(aMaterial.m_molar_mass),
        m_density(aMaterial.m_density)
//---------------------------------------------------------------------
{
	std::memcpy(m_p_weight_set, aMaterial.m_p_weight_set, sizeof(m_p_weight_set));
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


//-----------------------------------------------
inline double TissueMaterial::getMolarMass() const
//-----------------------------------------------
{
	return (m_molar_mass);
}


//-----------------------------------------------------
inline void TissueMaterial::setDensity(double aDensity)
//-----------------------------------------------------
{
    m_density = aDensity;
}


//----------------------------------------------
inline double TissueMaterial::getDensity() const
//----------------------------------------------
{
    return m_density;
}


//---------------------------------------------------------
inline double TissueMaterial::getMU(short aHounsfieldValue,
                                    double anEnergy) const
//---------------------------------------------------------
{
    return (getMassAttenuationTotal(anEnergy) *
            (getDensity(aHounsfieldValue)));
}


//---------------------------------------------------------
inline double TissueMaterial::getMU(double anEnergy) const
//---------------------------------------------------------
{
    return (getMassAttenuationTotal(anEnergy) *
            (getDensity()));
}


//-------------------------------------------------------------------------------
inline TissueMaterial& TissueMaterial::operator=(const TissueMaterial& aMaterial)
//-------------------------------------------------------------------------------
{
	m_min_HU_value = aMaterial.m_min_HU_value;
	m_max_HU_value = aMaterial.m_max_HU_value;
    m_molar_mass   = aMaterial.m_molar_mass;
    m_density      = aMaterial.m_density;
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
    anOutputSream << aTissueMaterial.m_p_weight_set[16] << "\t";
    anOutputSream << aTissueMaterial.m_p_weight_set[17] << "\t";
	anOutputSream << aTissueMaterial.m_p_weight_set[18] << std::endl;
	
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
    anInputSream >> aTissueMaterial.m_p_weight_set[17];
    anInputSream >> aTissueMaterial.m_p_weight_set[18];
	
	aTissueMaterial.normaliseWeightSet();
	
	return (anInputSream);
}


//---------------------------------------------------------------------------
inline bool TissueMaterial::operator!=(const TissueMaterial& aMaterial) const
//---------------------------------------------------------------------------
{
    return (!(operator==(aMaterial)));
}
} // namespace gVirtualXRay/
