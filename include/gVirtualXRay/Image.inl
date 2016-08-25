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
#include <sstream> // Header file for stringstream
#include <fstream> // Header file for filestream
#include <algorithm> // Header file for min/max/fill
#include <cmath> // Header file for abs
#include <numeric> // Header file for accumulate
#include <vector>
#include <iostream>

#ifndef __ConstantValues_h
#include "gVirtualXRay/ConstantValues.h"
#endif

#ifndef __Utilities_h
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef __Exception_h
#include "gVirtualXRay/Exception.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//------------------
template<typename T> Image<T>::Image():
//------------------
        m_width(0),
        m_height(0),
        m_number_of_slices(0),
        m_spacing(1, 1, 1),
        m_p_image(0)
//------------------
{}


//----------------------------------------------
template<typename T> Image<T>::Image(const Image& anImage):
//----------------------------------------------
        m_width(anImage.m_width),
        m_height(anImage.m_height),
        m_number_of_slices(anImage.m_number_of_slices),
        m_spacing(anImage.m_spacing),
        m_p_image(new T[m_width * m_height * m_number_of_slices])
//----------------------------------------------
{
    // Out of memory
    if (m_width && m_height && m_number_of_slices && !m_p_image)
    {
        throw "Out of memory";
    }
    
    // Copy the data
    std::copy(anImage.m_p_image, anImage.m_p_image + m_width * m_height * m_number_of_slices, m_p_image);
}


//----------------------------------------------
template<typename T> Image<T>::Image(const T* apData,
             unsigned int aWidth,
             unsigned int aHeight,
             unsigned int aNumberOfSlices):
//----------------------------------------------
        m_width(aWidth),
        m_height(aHeight),
        m_number_of_slices(aNumberOfSlices),
        m_spacing(1, 1, 1),
        m_p_image(new T[m_width * m_height * m_number_of_slices])
//----------------------------------------------
{
    // Out of memeory
    if (m_width && m_height && !m_p_image)
    {
        throw "Out of memory";
    }

    // Copy the data
    std::copy(apData, apData + m_width * m_height * m_number_of_slices, m_p_image);
}


//----------------------------------------------
template<typename T> Image<T>::Image(unsigned int aWidth,
                                     unsigned int aHeight,
                                     unsigned int aNumberOfSlices,
                                     T aDefaultValue):
//----------------------------------------------
        m_width(aWidth),
        m_height(aHeight),
        m_number_of_slices(aNumberOfSlices),
        m_spacing(1, 1, 1),
        m_p_image(new T[m_width * m_height * m_number_of_slices])
//----------------------------------------------
{
    // Out of memeory
    if (m_width && m_height && m_number_of_slices && !m_p_image)
    {
        throw "Out of memory";
    }

    // Initialise the data
    std::fill_n(m_p_image, m_width * m_height * m_number_of_slices, aDefaultValue);
}


//-------------
template<typename T> Image<T>::~Image()
//-------------
{
    // Release memory
    destroy();
}


//-------------------
template<typename T> void Image<T>::destroy()
//-------------------
{
    // Memory has been dynamically allocated
    if (m_p_image)
    {
        // Release the memory
        delete [] m_p_image;
    
        // Make sure the pointer is reset to NULL
        m_p_image = 0;
    }
    
    // There is no pixel in the image
    m_width  = 0;
    m_height = 0;
    m_number_of_slices = 0;
}


//--------------------------------------------
template<typename T> T* Image<T>::getRawData()
//--------------------------------------------
{
    return (m_p_image);
}


//--------------------------------------------------------
template<typename T> const T* Image<T>::getRawData() const
//--------------------------------------------------------
{
    return (m_p_image);
}


//-------------------------------------------
template<typename T> Image<T>& Image<T>::operator=(const Image<T>& anImage)
//-------------------------------------------
{
    // The images different
    if (this != &anImage)
    {
        // Release memory
        destroy();

        // Copy the image properites
        m_width   = anImage.m_width;
        m_height  = anImage.m_height;
        m_number_of_slices = anImage.m_number_of_slices;
        m_spacing = anImage.m_spacing;
        m_p_image = new T[m_width * m_height * m_number_of_slices];

        // Out of memeory
        if (m_width && m_height && m_number_of_slices && !m_p_image)
        {
            throw "Out of memory";
        }
        
        // Copy the data
        std::copy(anImage.m_p_image, anImage.m_p_image + m_width * m_height * m_number_of_slices, m_p_image);
    }

    // Return the instance
    return (*this);
}


template<typename T> Image<T>& Image<T>::operator=(const T* apImage)
{
    // Copy the data
    std::copy(apImage, apImage + m_width * m_height * m_number_of_slices, m_p_image);

    // Return the instance
    return (*this);
}


//----------------------------------
template<typename T> unsigned int Image<T>::getWidth() const
//----------------------------------
{
    return (m_width);
}


//-----------------------------------
template<typename T> unsigned int Image<T>::getHeight() const
//-----------------------------------
{
    return (m_height);
}


//--------------------------------------------------------------------
template<typename T> void Image<T>::setSpacing(const VEC3& aPixelSize)
//--------------------------------------------------------------------
{
    m_spacing = aPixelSize;
}


//-----------------------------------------------------------
template<typename T> const VEC3& Image<T>::getSpacing() const
//-----------------------------------------------------------
{
    return (m_spacing);
}


//------------------------------
template<typename T> T Image<T>::getMinValue() const
//------------------------------
{
    // The image is empty
    if (!m_p_image)
    {
        throw "Empty image";
    }
    
    return (*std::min_element(&m_p_image[0], &m_p_image[m_width * m_height * m_number_of_slices]));
}


//------------------------------
template<typename T> T Image<T>::getMaxValue() const
//------------------------------
{
    // The image is empty
    if (!m_p_image)
    {
        throw "Empty image";
    }
    
    return (*std::max_element(&m_p_image[0], &m_p_image[m_width * m_height * m_number_of_slices]));
}


//-----------------------------------------------------------------------
template<typename T> Image<T> Image<T>::shiftScaleFilter(double aShiftValue, double aScaleValue) const
//-----------------------------------------------------------------------
{
    // Copy the instance into a temporary variable
    Image<T> temp(*this);
    
    // Process every pixel of the image
    for (unsigned int i = 0; i < m_width * m_height * m_number_of_slices; ++i)
    {
        // Apply the shilft/scale filter
        temp[i] = (temp[i] + aShiftValue) * aScaleValue;
    }
    
    // Return the result
    return (temp);
}


//-----------------------------
template<typename T> Image<T> Image<T>::normalised() const
//-----------------------------
{
    return (shiftScaleFilter(-getMinValue(), 1.0 / (getMaxValue() - getMinValue())));
}


//-----------------------------
template<typename T> Image<T> Image<T>::normalized() const
//-----------------------------
{
    return (normalised());
}


//----------------------
template<typename T> Image<T> Image<T>::abs() const
//----------------------
{
    Image<T> temp(*this);

    for (unsigned int i(0); i < m_width * m_height * m_number_of_slices; ++i)
    {
        temp[i] = std::abs(m_p_image[i]);
    }

    return (temp);
}


//----------------------
template<typename T> Image<T> Image<T>::log() const
//----------------------
{
    Image<T> temp(*this);

    float offset(0.0);

    if (std::abs(getMinValue()) < EPSILON)
    {
        offset = 0.000000157;
    }

    for (unsigned int i(0); i < m_width * m_height * m_number_of_slices; ++i)
    {
        float pixel(m_p_image[i]);

        if (std::abs(pixel) < EPSILON)
        {
            pixel = offset;
        }

        temp[i] = std::log(pixel);
    }

    return (temp);
}


//-------------------------------------------------------------------
template<typename T> void Image<T>::save(const char* aFileName) const
//-------------------------------------------------------------------
{
    if (isMAT(aFileName) || isDAT(aFileName) || isTXT(aFileName))
    {
        saveASCII(aFileName);
    }
    else if (isPGM(aFileName))
    {
        savePGM(aFileName);
    }
    else if (isRAW(aFileName))
    {
        saveRAW(aFileName);
    }
    else if (isMHD(aFileName))
    {
        saveMHD(aFileName);
    }
    else if (isMHA(aFileName))
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "Saving MHA files is not implemented, "
                "please contact Dr FP Vidal.");
    }
    else if (isDCM(aFileName))
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "Saving DICOM files is not implemented, "
                "please contact Dr FP Vidal.");
    }
    else if (isTIFF(aFileName))
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "Saving TIFF files is not implemented, "
                "please contact Dr FP Vidal.");
    }
    else if (isJPEG(aFileName))
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "Saving JPEG files is not implemented, "
                "please contact Dr FP Vidal.");
    }
    else
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "Unknown file format, to get it added to the library, "
                "please contact Dr FP Vidal.");
    }

}


//--------------------------------------------------------------------------
template<typename T> void Image<T>::save(const std::string& aFileName) const
//--------------------------------------------------------------------------
{
    save(aFileName.data());
}


//----------------------------------------------
template<typename T> void Image<T>::savePGM(const char* aFileName) const
//----------------------------------------------
{
    // Open the file
    std::ofstream output_file(aFileName);
    
    // The file does not exist
    if (!output_file.is_open())
    {
        // Build the error message
        std::stringstream error_message;
        error_message << "Cannot create the file \"" << aFileName << "\"";
    
        // Throw an error
        throw (error_message.str());
    }
    // The file is open
    else
    {
        // Set the image type
        output_file << "P2" << std::endl;
    
        // Print a comment
        output_file << "# ICP3038 -- Assignment 1 -- 2015/2016" << std::endl;
    
        // The image size
        output_file << m_width << " " << m_height * m_number_of_slices << std::endl;

        // The get the max value
        output_file << std::min(255, std::max(0, int(getMaxValue()))) << std::endl;
    
        // Process every line
        for (unsigned int k = 0; k < m_number_of_slices; ++k)
        {
            for (unsigned int j = 0; j < m_height; ++j)
            {
                // Process every column
                for (unsigned int i = 0; i < m_width; ++i)
                {
                    // Process the pixel
                    int pixel_value(m_p_image[k * m_width * m_height + j * m_width + i]);
                    pixel_value = std::max(0, pixel_value);
                    pixel_value = std::min(255, pixel_value);

                    output_file << pixel_value;

                    // It is not the last pixel of the line
                    if (i < (m_width - 1))
                    {
                        output_file << " ";
                    }
                }
            
                // It is not the last line of the image
                if (j < (m_height - 1))
                {
                    output_file << std::endl;
                }
            }
        }
    }
}


//-----------------------------------------------------
template<typename T> void Image<T>::savePGM(const std::string& aFileName) const
//-----------------------------------------------------
{
    savePGM(aFileName.data());
}


//----------------------------------------------
template<typename T> void Image<T>::saveRaw(const char* aFileName) const
//----------------------------------------------
{
    // Open the file in binary
    std::ofstream output_file (aFileName, std::ifstream::binary);

    // The file is not open
    if (!output_file.is_open())
    {
        std::string error_message("The file (");
        error_message += aFileName;
        error_message += ") cannot be created";

        throw error_message;
    }

    // Write content to file
    output_file.write(reinterpret_cast<char*>(m_p_image), m_width * m_height * m_number_of_slices * sizeof(T));
}


//----------------------------------------------
template<typename T> void Image<T>::saveRAW(const char* aFileName) const
//----------------------------------------------
{
    saveRaw(aFileName);
}


//-----------------------------------------------------
template<typename T> void Image<T>::saveRaw(const std::string& aFileName) const
//-----------------------------------------------------
{
    saveRaw(aFileName.data());
}


//-----------------------------------------------------
template<typename T> void Image<T>::saveRAW(const std::string& aFileName) const
//-----------------------------------------------------
{
    saveRaw(aFileName);
}


//------------------------------------------------
template<typename T> void Image<T>::saveASCII(const char* aFileName) const
//------------------------------------------------
{
    // Open the file
    std::ofstream output_file (aFileName);

    // The file is not open
    if (!output_file.is_open())
    {
        std::string error_message("The file (");
        error_message += aFileName;
        error_message += ") cannot be created";

        throw error_message;
    }

    // Write content to file
    T* p_data(m_p_image);
    for (unsigned int k = 0; k < m_number_of_slices; ++k)
    {
        for (unsigned int j(0); j < m_height; ++j)
        {
            for (unsigned int i(0); i < m_width; ++i)
            {
                output_file << *p_data++;

                // This is not the last pixel of the line
                if (i < m_width - 1)
                {
                    output_file << " ";
                }
            }

            // This is not the last line
            if (j < m_height - 1)
            {
                output_file << std::endl;
            }
        }
    }
}


//-------------------------------------------------------
template<typename T> void Image<T>::saveASCII(const std::string& aFileName) const
//-------------------------------------------------------
{
    saveASCII(aFileName.data());
}


template<typename T> void Image<T>::saveMHD(const char* aFileName) const
{
    saveMHD(std::string(aFileName));
}


//------------------------------------------------
template<typename T> bool Image<T>::operator==(const Image& anImage) const
//------------------------------------------------
{
    if (m_width != anImage.m_width)
    {
        return (false);
    }

    if (m_height != anImage.m_height)
    {
        return (false);
    }

    if (m_number_of_slices != anImage.m_number_of_slices)
    {
        return (false);
    }

    float const * p_data1(m_p_image);
    float const * p_data2(anImage.m_p_image);
    for (unsigned int i(0); i < m_width * m_height * m_number_of_slices; ++i)
    {
        if (std::abs(*p_data1++ - *p_data2++) > EPSILON)
        {
            return (false);
        }
    }

    return (true);
}


//------------------------------------------------
template<typename T> bool Image<T>::operator!=(const Image& anImage) const
//------------------------------------------------
{
    return (!(operator==(anImage)));
}


//--------------------------
template<typename T> double Image<T>::getSum() const
//--------------------------
{
    // The image is empty
    if (!m_p_image)
    {
        throw "Empty image";
    }

    return (std::accumulate(&m_p_image[0], &m_p_image[m_width * m_height * m_number_of_slices], 0.0));
}


//------------------------------
template<typename T> double Image<T>::getAverage() const
//------------------------------
{
    return (getSum() / (m_width * m_height));
}


//-------------------------------
template<typename T> double Image<T>::getVariance() const
//-------------------------------
{
    double variance(0.0);
    double average(getAverage());
    
    float* p_temp(m_p_image);
    for (unsigned int i(0); i < m_width * m_height * m_number_of_slices; ++i)
    {
        variance += std::pow(*p_temp++ - average, 2);
    }

    return (variance / (m_width * m_height));
}


//----------------------------------------
template<typename T> double Image<T>::getStandardDeviation() const
//----------------------------------------
{
    return (std::sqrt(getVariance()));
}


//--------------------------------------------
template<typename T> double Image<T>::computeSAE(const Image& anImage) const
//--------------------------------------------
{
    if (m_width != anImage.m_width || m_height != anImage.m_height || m_number_of_slices != anImage.m_number_of_slices)
    {
        throw "The images have different sizes";
    }
    
    float* p_temp1(m_p_image);
    float* p_temp2(anImage.m_p_image);
    
    double sae(0.0);
    for (unsigned int i(0); i < m_width * m_height * m_number_of_slices; ++i)
    {
        sae += std::abs(*p_temp1++ - *p_temp2++);
    }
    
    return (sae);
}


//--------------------------------------------
template<typename T> double Image<T>::computeNCC(const Image& anImage) const
//--------------------------------------------
{
    if (m_width != anImage.m_width || m_height != anImage.m_height || m_number_of_slices != anImage.m_number_of_slices)
    {
        throw "The images have different sizes";
    }
    
    float* p_temp1(m_p_image);
    float* p_temp2(anImage.m_p_image);
    
    double average1(getAverage());
    double average2(anImage.getAverage());
    
    double standard_deviation_product(getStandardDeviation() * anImage.getStandardDeviation());
    double ncc(0.0);
    for (unsigned int i(0); i < m_width * m_height * m_number_of_slices; ++i)
    {
        ncc += (*p_temp1++ - average1) * (*p_temp2++ - average2);
    }
    
    return (ncc / (m_width * m_height * standard_deviation_product));
}


} // namespace gVirtualXRay
