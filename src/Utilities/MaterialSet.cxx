/**
********************************************************************************
*
*   @file       MaterialSet.cxx
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
#include <fstream>

#ifndef GVXR_MATERIAL_SET_H
#include "gVirtualXRay/MaterialSet.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
using namespace gVirtualXRay;


//----------------------------------------------------------
void MaterialSet::loadFromFile(const std::string& aFileName)
//----------------------------------------------------------
{
	//	Clear all the memory memory
	clearMemory();

	// Open the file
	std::ifstream input_file(aFileName.c_str());

	// The file cannot be opened
	if (!input_file.is_open())
	{
		// Throw an error
		throw Exception(__FILE__, __FUNCTION__, __LINE__, std::string("ERROR: cannot read the material table file using ") + aFileName);
	}

	// Skip the first line
	char first_line_[1024];
	if (input_file.getline(first_line_, 1024).fail())
	{
		// Close the file
		input_file.close();

		// Throw an error
		throw Exception(__FILE__, __FUNCTION__, __LINE__, std::string("ERROR: fail reading the first line of ") + aFileName);
	}

	// Read all the materials in the file,
	// and add them to the table
	while (!input_file.eof())
	{
		// Read the current material
		TissueMaterial tissue_material;
		input_file >> tissue_material;

		// Add the material to the table
		if (!(tissue_material.getMinHUValue() == 0 && tissue_material.getMaxHUValue() == 0))
		{
			tissue_material.normaliseWeightSet();
			m_material_set.insert(std::pair<std::pair<short, short>, TissueMaterial>(std::pair<short, short>(tissue_material.getMinHUValue(), tissue_material.getMaxHUValue()), tissue_material));
		}
	}
}


//------------------------------------------------------------
void MaterialSet::addMaterial(const TissueMaterial& aMaterial)
//------------------------------------------------------------
{
    m_material_set.insert(std::pair<std::pair<short, short>, TissueMaterial>(std::pair<short, short>(aMaterial.getMinHUValue(), aMaterial.getMaxHUValue()), aMaterial));
}


//------------------------------------------------------------
const TissueMaterial& MaterialSet::getMaterial(short aHUValue)
//------------------------------------------------------------
{
	// The material table is empty
	if (m_material_set.size() == 0)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: the material table is empty.");
	}

	// This iterator is used in case of the HU is higher than the last record in the table
	std::map<std::pair<short, short>, TissueMaterial>::const_iterator last_ite(m_material_set.begin());

	// Find the record that fits aHUValue
	for (std::map<std::pair<short, short>, TissueMaterial>::const_iterator ite(m_material_set.begin());
			ite != m_material_set.end();
			++ite)
	{
		if (aHUValue <= (ite->first).second)
		{
			return ite->second;
		}

		last_ite = ite;
	}

	// No record has been found, use the last record in the table
	return (last_ite->second);
}
