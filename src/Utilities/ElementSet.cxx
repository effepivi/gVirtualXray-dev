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
*   @file       ElementSet.cxx
*
*   @brief      Class to manage a table of elements in material.
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
#include <fstream>

#ifndef GVXR_ELEMENT_SET_H
#include "gVirtualXRay/ElementSet.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
using namespace gVirtualXRay;


//---------------------------------------------------------
void ElementSet::loadFromFile(const std::string& aFileName)
//---------------------------------------------------------
{
	// Clear the memory
	clearMemory();

	// Open the file
	std::ifstream input_file(aFileName.c_str());

	// The file cannot be opened
	if (!input_file.is_open())
	{
		// Throw an error
		throw Exception(__FILE__, __FUNCTION__, __LINE__, std::string("ERROR: cannot read the element table file using ") + aFileName);
	}

	// Skip the first line
	char first_line_[1024];
	if (input_file.getline(first_line_, 1024).fail())
	{
		// Close the input file
		input_file.close();

		// Throw an error
		throw Exception(__FILE__, __FUNCTION__, __LINE__, std::string("ERROR: fail reading the first line of ") + aFileName);
	}

	// Read all the elements in the file,
	// and add them to the table
	while (!input_file.eof())
	{
		// Read the current element
		AtomicElement element;
		input_file >> element;

		// Add the element to the table
		// (no element has a null atomic number)
		if (element.getAtomicNumber() > 0)
		{
			m_element_set.insert(std::pair<unsigned short, AtomicElement>(element.getAtomicNumber(), element));
		}
	}

	// Close the input file
	input_file.close();
}

