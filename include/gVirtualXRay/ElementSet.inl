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
*   @file       ElementSet.inl
*
*   @brief      Class to manage a table of elements in material.
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
#include <sstream>

#ifndef EXCEPTION_H
#include "Exception.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//-----------------------------
inline ElementSet::ElementSet()
//-----------------------------
{
}


//---------------------------------------------------------
inline ElementSet::ElementSet(const std::string& aFileName)
//---------------------------------------------------------
{
	loadFromFile(aFileName);
}


//------------------------------
inline ElementSet::~ElementSet()
//------------------------------
{
	clearMemory();
}


//-----------------------------------
inline void ElementSet::clearMemory()
//-----------------------------------
{
	m_element_set.clear();
}


//----------------------------------------------------------------
inline void ElementSet::addElement(const AtomicElement& anElement)
//----------------------------------------------------------------
{
    m_element_set.insert(std::pair<unsigned short, AtomicElement>(anElement.getAtomicNumber(), anElement));
}


//-------------------------------------------------------
inline unsigned int ElementSet::getElementSetSize() const
//-------------------------------------------------------
{
    return (m_element_set.size());
}

//-------------------------------------------------------------------------
inline AtomicElement& ElementSet::getElement(unsigned short anAtomicNumber)
//-------------------------------------------------------------------------
{
    std::map<unsigned short, AtomicElement>::iterator element_ite(m_element_set.find(anAtomicNumber));

    // Element found
    if (element_ite != m_element_set.end())
    {
        return (element_ite->second);
    }
    // Element not found in the list
    else
    {
        std::stringstream error_message;
        error_message << "ERROR: Element (atomic number: " << anAtomicNumber << ") not found.";
        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
    }
}


//------------------------------------------------------------------------
inline float ElementSet::getMolarMass(unsigned short anAtomicNumber) const
//------------------------------------------------------------------------
{
    return (getElement(anAtomicNumber).getMolarMass());
}


//----------------------------------------------------------------------
inline float ElementSet::getDensity(unsigned short anAtomicNumber) const
//----------------------------------------------------------------------
{
    return (getElement(anAtomicNumber).getDensity());
}


//-------------------------------------------------------------------------------------
inline const AtomicElement& ElementSet::getElement(unsigned short anAtomicNumber) const
//-------------------------------------------------------------------------------------
{
    std::map<unsigned short, AtomicElement>::const_iterator element_ite(m_element_set.find(anAtomicNumber));

    // Element found
    if (element_ite != m_element_set.end())
    {
        return (element_ite->second);
    }
    // Element not found in the list
    else
    {
        std::stringstream error_message;
        error_message << "ERROR: Element (atomic number: " << anAtomicNumber << ") not found.";
        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
    }
}


//----------------------------------------------------------------
inline std::ostream & operator << (std::ostream& anOutputSream,
                                   const ElementSet& anElementSet)
//----------------------------------------------------------------
{
	for (std::map<unsigned short, AtomicElement>::const_iterator ite(anElementSet.m_element_set.begin());
		ite != anElementSet.m_element_set.end();
		++ite)
	{
		anOutputSream << ite->second << std::endl;
	}
	
	return (anOutputSream);
}


} // namespace gCDRR

