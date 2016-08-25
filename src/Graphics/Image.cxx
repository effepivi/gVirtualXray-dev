/**
********************************************************************************
*
*   @file       Image.cpp
*
*   @brief      Class to handle a greyscale image.
*
*   @version    1.0
*
*   @todo       Write here anything you did not implement.
*
*   @date       11/11/2016
*
*   @author     Franck Vidal
*
*
********************************************************************************
*/


//******************************************************************************
//  Define
//******************************************************************************
#define LINE_SIZE 4096


//******************************************************************************
//  Include
//******************************************************************************
#include <fstream>

#ifndef __Image_h
#include "gVirtualXRay/Image.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


template<> void Image<float>::saveMHD(const std::string& aFileName) const
{
    std::ofstream header_file(aFileName.data());

    if (!header_file.is_open())
    {
        throw "Cannot open the file";
    }

    // Save the header file
    header_file << "ObjectType = Image" << std::endl;
    header_file << "NDims = 3" << std::endl;
    header_file << "BinaryData = True" << std::endl;
    header_file << "BinaryDataByteOrderMSB = False" << std::endl;
    header_file << "CompressedData = False" << std::endl;
    header_file << "TransformMatrix = 1 0 0 0 1 0 0 0 1" << std::endl;
    header_file << "Offset = 0 0 0" << std::endl;
    header_file << "CenterOfRotation = 0 0 0" << std::endl;
    header_file << "AnatomicalOrientation = RAI" << std::endl;
    header_file << "ElementSpacing = " << m_spacing.getX() << " " << m_spacing.getY() << " " << m_spacing.getZ() << std::endl;
    header_file << "DimSize = " << m_width << " " << m_height << " " << m_number_of_slices << std::endl;
    header_file << "ElementType = MET_FLOAT" << std::endl;

    std::string file_name_with_out_extension(aFileName.substr(0, aFileName.size() - 4));
    header_file << "ElementDataFile = " << file_name_with_out_extension + ".raw" << std::endl;

    // Save the RAW file
    saveRAW(file_name_with_out_extension + ".raw");
}


};

/*

*/
