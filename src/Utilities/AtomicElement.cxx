/**
********************************************************************************
*
*   @file       AtomicElement.cxx
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
#ifndef GVXR_ATOMIC_ELEMENT_H
#include "gVirtualXRay/AtomicElement.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
using namespace gVirtualXRay;

/*
//---------------------------------------------------------------------------------------
void AtomicElement::SetMUCoefficientList(const E_MU* apList,
																		 unsigned int aEnergyChannelNumber)
//---------------------------------------------------------------------------------------
{
	if (m_p_MU_coefficient_list) delete [] m_p_MU_coefficient_list;
	m_p_MU_coefficient_list = 0;

	if (apList && aEnergyChannelNumber)
	{
		m_p_MU_coefficient_list = new E_MU[aEnergyChannelNumber];

		if (!m_p_MU_coefficient_list)
		{
			throw Exception("ERROR: out of memory, cannot allocate the list of total attenuation coefficients.", __FILE__, __FUNCTION__, __LINE__);
		}
		memcpy(m_p_MU_coefficient_list, apList, aEnergyChannelNumber * sizeof(E_MU));
		
		m_smallest_energy_step = -1;
		for (unsigned int i(1); i < aEnergyChannelNumber; ++i)
		{
			float e1(m_p_MU_coefficient_list[i].E);
			float e2(m_p_MU_coefficient_list[i - 1].E);
			float delta(fabs(e1 - e2));
			if (delta > EPSILON)
			{
				if (m_smallest_energy_step < 0 || m_smallest_energy_step > delta)
				{
					m_smallest_energy_step = delta;
				}
			}
		}
		m_smallest_energy_step *= MeV;
	}

	m_energy_channel_number = aEnergyChannelNumber;
}
*/
