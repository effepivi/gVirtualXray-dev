/**
********************************************************************************
*
*   @file       ElementSet.cxx
*
*   @brief      Class to manage a table of elements in material.
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
#include <fstream>

#ifndef GVXR_ELEMENT_SET_H
#include "gVirtualXRay/ElementSet.h"
#endif


//******************************************************************************
//	Global variables
//******************************************************************************


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

