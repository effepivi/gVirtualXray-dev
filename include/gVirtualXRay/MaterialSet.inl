/**
********************************************************************************
*
*   @file       MaterialSet.inl
*
*   @brief      Class to manage a table of materials.
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
#ifndef EXCEPTION_H
#include "Exception.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//--------------------------------
inline MaterialSet::MaterialSet():
//--------------------------------
	m_mu_min_energy(0),
	m_mu_max_energy(0),
	m_S_min_x(0),
	m_S_max_x(0),
	m_min_HU(0),
	m_max_HU(0)
//--------------------------------
{
}


//-----------------------------------------------------------
inline MaterialSet::MaterialSet(const std::string& aFileName)
//-----------------------------------------------------------
{
	loadFromFile(aFileName);
}


//--------------------------------
inline MaterialSet::~MaterialSet()
//--------------------------------
{
	clearMemory();
}


//------------------------------------
inline void MaterialSet::clearMemory()
//------------------------------------
{
	m_material_set.clear();
}


//----------------------------------------------
inline float MaterialSet::getMuMinEnergy() const
//----------------------------------------------
{
	return (m_mu_min_energy);
}


//----------------------------------------------
inline float MaterialSet::getMuMaxEnergy() const
//----------------------------------------------
{
	return (m_mu_max_energy);
}


//----------------------------------------
inline float MaterialSet::getSMinX() const
//----------------------------------------
{
	return (m_S_min_x);
}


//----------------------------------------
inline float MaterialSet::getSMaxX() const
//----------------------------------------
{
	return (m_S_max_x);
}


inline short MaterialSet::getMinHU() const
{
	return (m_min_HU);
}


inline short MaterialSet::getMaxHU() const
{
	return (m_max_HU);
}


//--------------------------------------------------------
inline unsigned int MaterialSet::getMaterialNumber() const
//--------------------------------------------------------
{
	return (m_material_set.size());
}


//-----------------------------------------------------------------
inline std::ostream & operator << (std::ostream& anOutputSream,
                                   const MaterialSet& aMaterialSet)
//-----------------------------------------------------------------
{
	for (std::map<std::pair<short, short>, TissueMaterial>::const_iterator ite(aMaterialSet.m_material_set.begin());
		ite != aMaterialSet.m_material_set.end();
		++ite)
	{
		anOutputSream << ite->second << std::endl;
	}
	
	return (anOutputSream);
}


} // namespace gVirtualXRay
