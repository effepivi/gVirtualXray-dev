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
#include <limits> // Numeric limits per types
#include <vector>
#include <iostream>

#ifdef HAS_GDCM
#include <typeinfo>

#include <gdcmGlobal.h>

#include "gdcmImageReader.h"
#include "gdcmSequenceOfFragments.h"
#include "gdcmSystem.h"
#include "gdcmImageWriter.h"
#include "gdcmPixmapWriter.h"
#include <gdcmRescaler.h>

#include <gdcmImage.h>
#include <gdcmBitmap.h>
#include <gdcmImageWriter.h>
#include <gdcmPixelFormat.h>
#include <gdcmPhotometricInterpretation.h>
#include <gdcmImageChangeTransferSyntax.h>
#endif

#ifdef HAS_TIFF
#include <tiff.h>
#include <tiffio.h>
#endif

#ifndef __ConstantValues_h
#include "gVirtualXRay/ConstantValues.h"
#endif

#ifndef __Utilities_h
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef __Exception_h
#include "gVirtualXRay/Exception.h"
#endif

#ifndef __OutOfMemoryException_h
#include "gVirtualXRay/OutOfMemoryException.h"
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
        throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
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
        throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
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
        throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
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
            throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
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
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Empty image");
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
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Empty image");
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


//------------------------------------------------------------------
template<typename T> void Image<T>::save(const char* aFileName,
                                         bool anInvertLUTFlag,
                                         const char* aComment) const
//------------------------------------------------------------------
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
        saveDCM(aFileName, anInvertLUTFlag, aComment);
    }
    else if (isTIFF(aFileName))
    {
        saveTIFF(aFileName, anInvertLUTFlag);
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


//-------------------------------------------------------------------------
template<typename T> void Image<T>::save(const std::string& aFileName,
                                         bool anInvertLUTFlag,
                                         const std::string& aComment) const
//-------------------------------------------------------------------------
{
    save(aFileName.data(), anInvertLUTFlag, aComment.data());
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
        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
    }
    // The file is open
    else
    {
        // Get the min/max/range values
        T min_value(getMinValue());
        T max_value(getMaxValue());
        T range_value(max_value - min_value);

        // Set the image type
        output_file << "P2" << std::endl;
    
        // Print a comment
        output_file << "gVirtualXRay" << std::endl;
    
        // The image size
        output_file << m_width << " " << m_height * m_number_of_slices << std::endl;

        // The get the max value
        output_file << "255" << std::endl;

        // Process every line
        for (unsigned int k = 0; k < m_number_of_slices; ++k)
        {
            for (unsigned int j = 0; j < m_height; ++j)
            {
                // Process every column
                for (unsigned int i = 0; i < m_width; ++i)
                {
                    // Process the pixel
                    int pixel_value;
                    if (typeid(T) == typeid(unsigned char))
                    {
                        pixel_value = m_p_image[k * m_width * m_height + j * m_width + i];
                    }
                    else
                    {
                        pixel_value = 255.0 * (m_p_image[k * m_width * m_height + j * m_width + i] - min_value) / range_value;
                    }

#ifndef _WIN32
					pixel_value = std::max(0, pixel_value);
                    pixel_value = std::min(255, pixel_value);
#else
					pixel_value = max(0, pixel_value);
					pixel_value = min(255, pixel_value);
#endif
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

        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
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

        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
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


//---------------------------------------------------------------------
template<typename T> void Image<T>::saveDCM(const char* aFileName,
                                            bool anInvertLUTFlag,
                                            const char* aComment) const
//---------------------------------------------------------------------
{
#ifdef HAS_GDCM
    T min_input_value(getMinValue());
    T max_input_value(getMaxValue());
    T difference_input(max_input_value - min_input_value);

    unsigned short min_output_value(((gdcm::PixelFormat)gdcm::PixelFormat::UINT16).GetMin());
    unsigned short max_output_value(((gdcm::PixelFormat)gdcm::PixelFormat::UINT16).GetMax());
    unsigned short difference_output(max_output_value - min_output_value);

    double shift(min_input_value);
    double scale(double(max_input_value - min_input_value) / max_output_value);

    if (m_number_of_slices == 1)
    {

        // Write the modified DataSet back to disk
        gdcm::ImageWriter writer;
        //writer.CheckFileMetaInformationOff(); // Do not attempt to reconstruct the file meta to preserve the file
                                              // as close to the original as possible.

        // Create the image (GDCM format)
        gdcm::Image& gdcm_image(writer.GetImage());

        // Set the size of the image
        unsigned int dims[3] = {};
        dims[0] = m_width;
        dims[1] = m_height;

        double spacing[3] = {};
        spacing[0] = m_spacing[0];
        spacing[1] = m_spacing[1];

        if (m_number_of_slices == 1)
            {
            gdcm_image.SetNumberOfDimensions(2);
            }
        else
            {
            dims[2] = m_number_of_slices;
            gdcm_image.SetNumberOfDimensions(3);

            spacing[2] = 1;//m_spacing[2];
            }

        gdcm_image.SetSpacing (spacing);

        gdcm_image.SetDimensions( dims );


        unsigned short* p_temp(new unsigned short[m_width * m_height * m_number_of_slices]);

        for (unsigned int i(0); i < m_width * m_height * m_number_of_slices; ++i)
        {
            p_temp[i] = min_output_value + difference_output * (double(m_p_image[i] - min_input_value) / double(difference_input));
        }

        unsigned int input_length(m_width * m_height * m_number_of_slices * sizeof(T));
        unsigned int output_length(m_width * m_height * m_number_of_slices * sizeof(unsigned short));

        gdcm::PixelFormat input_pixel_format(gdcm::PixelFormat::UINT16);
        input_pixel_format.SetSamplesPerPixel(1);

        gdcm_image.SetSlope(scale);
        gdcm_image.SetIntercept(shift);

        //gdcm_image.SetPixelFormat(pixel_format.ComputeInterceptSlopePixelType());
        gdcm_image.SetPixelFormat(input_pixel_format);
        gdcm::PhotometricInterpretation photometric_interpretation;

        if (anInvertLUTFlag)
            {
            photometric_interpretation = gdcm::PhotometricInterpretation::MONOCHROME2;
            }
        else
            {
            photometric_interpretation = gdcm::PhotometricInterpretation::MONOCHROME1;
            }
        gdcm_image.SetPhotometricInterpretation(photometric_interpretation);
        gdcm_image.SetTransferSyntax(gdcm::TransferSyntax::ExplicitVRLittleEndian);



        // Disable compression
        gdcm_image.SetLossyFlag(false);


        // Window / Level
        addTag(difference_input,
                0x0028, 0x1051,
                writer.GetFile());

        addTag(min_input_value + difference_input / 2.0,
                0x0028,0x1050,
                writer.GetFile());

        // Study description
        addTag("X-ray simulation",
                0x0008,0x1030,
                writer.GetFile());

        // Patient
        addTag("Patient",
                0x0010,0x0010,
                writer.GetFile());

        // Modality
        addTag(aComment,
                0x0008,0x0060,
                writer.GetFile());

        // Manufacturer
        addTag("gVirtualXRay",
                0x0008,0x0070,
                writer.GetFile());

        // InstitutionName
        addTag("Bangor University",
                0x0008,0x0080,
                writer.GetFile());

        // InstitutionAddress
        addTag("http://gvirtualxray.sourceforge.net/",
                0x0008,0x0081,
                writer.GetFile());

        // DICOM fields for X-rays:
        // See http://dicomlookup.com/lookup.asp?sw=Ttable&q=C.8-27


        // Set pixel data
        gdcm::DataElement pixel_data(gdcm::Tag(0x7fe0,0x0010));
        pixel_data.SetByteValue((const char*)p_temp, uint32_t(output_length));
        gdcm_image.SetDataElement(pixel_data);

        // Set the file name
        writer.SetFileName( aFileName );

        // Save the data
        if( !writer.Write() )
            {
            delete [] p_temp;

            std::string error_message("Could not write the DICOM file (");
            error_message += aFileName;
            error_message += ")";
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
            }
        delete [] p_temp;
    }

#else

    throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "Saving DICOM files is not implemented, "
            "please contact Dr FP Vidal.");

#endif
}


//----------------------------------------------------------------------------
template<typename T> void Image<T>::saveDCM(const std::string& aFileName,
                                            bool anInvertLUTFlag,
                                            const std::string& aComment) const
//----------------------------------------------------------------------------
{
    saveDCM(aFileName.data(), anInvertLUTFlag, aComment.data());
}


//----------------------------------------------------------------------
template<typename T> void Image<T>::saveTIFF(const char* aFileName,
                                             bool anInvertLUTFlag) const
//----------------------------------------------------------------------
{
#ifdef HAS_TIFF

    unsigned char* p_ubyte_data(0);
    unsigned short* p_ushort_data(0);
    unsigned int number_of_pixels(m_width * m_height * m_number_of_slices);

    double min_value(getMinValue());
    double max_value(getMaxValue());
    double value_range(max_value - min_value);

    // Open the file
    TIFF* p_file = TIFFOpen(aFileName, "w");

    // The file is not open
    if (!p_file)
        {
        std::string error_message("The file (");
        error_message += aFileName;
        error_message += ") cannot be created";

        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
        }

    // Set the file header
    
    // Set the image width
    TIFF_UINT32_T image_width(m_width);

    if (TIFFSetField(p_file, TIFFTAG_IMAGEWIDTH, image_width) != 1)
        {
            std::string error_message("Could not set the image width in the TIFF file (");
            error_message += aFileName;
            error_message += ")";
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
        }
        
    // Set the image height
    TIFF_UINT32_T image_height(m_height);

    if (TIFFSetField(p_file, TIFFTAG_IMAGELENGTH, m_height) != 1)
        {
            std::string error_message("Could not set the image height in the TIFF file (");
            error_message += aFileName;
            error_message += ")";
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
        }

    // Set the number of bits per pixel
    TIFF_UINT16_T number_of_bits_per_pixel(0);

    if (typeid(T) == typeid(unsigned char))
        {
        number_of_bits_per_pixel = 8;
        p_ubyte_data = new unsigned char[number_of_pixels];
        memcpy(p_ubyte_data, m_p_image, sizeof(unsigned char) * number_of_pixels);
        }
    else
        {
        number_of_bits_per_pixel = 16;
        p_ushort_data = new unsigned short[number_of_pixels];
        
        for (unsigned int i(0); i < number_of_pixels; ++i)
            {
            double temp(std::numeric_limits<unsigned short>::min() + std::numeric_limits<unsigned short>::max() * (double(m_p_image[i] - min_value) / value_range));
            if (temp < 0.0)
                {
                p_ushort_data[i] = std::numeric_limits<unsigned short>::min();
                }
            else if (temp > std::numeric_limits<unsigned short>::max())
                {
                p_ushort_data[i] = std::numeric_limits<unsigned short>::max();
                }
            else
                {
                p_ushort_data[i] = temp;
                }
            }
        }

    if (TIFFSetField(p_file, TIFFTAG_BITSPERSAMPLE, number_of_bits_per_pixel) != 1)
        {
            std::string error_message("Could not set the number of bits per pixel in the TIFF file (");
            error_message += aFileName;
            error_message += ")";
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
        }

    // Set the compression
    TIFF_UINT16_T compression(COMPRESSION_LZW);

    if (TIFFSetField(p_file, TIFFTAG_COMPRESSION, compression) != 1)
        {
            std::string error_message("Could not set the compression in the TIFF file (");
            error_message += aFileName;
            error_message += ")";
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
        }

    // Set the photometric interpretation
    TIFF_UINT16_T photometric_interpretation;
        if (anInvertLUTFlag)
            {
            photometric_interpretation = PHOTOMETRIC_MINISWHITE;
            }
        else
            {
            photometric_interpretation = PHOTOMETRIC_MINISBLACK;
            }
  
    if (TIFFSetField(p_file, TIFFTAG_PHOTOMETRIC, photometric_interpretation) != 1)
        {
            std::string error_message("Could not set the photometric interpretation in the TIFF file (");
            error_message += aFileName;
            error_message += ")";
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
        }

    // Set the orientation
    TIFF_UINT16_T orientation(ORIENTATION_TOPLEFT);

    if (TIFFSetField(p_file, TIFFTAG_ORIENTATION, orientation) != 1)
        {
            std::string error_message("Could not set the orientation in the TIFF file (");
            error_message += aFileName;
            error_message += ")";
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
        }

    // Set the number of samples per pixel
    TIFF_UINT16_T number_of_samples_per_pixel(1);

    if (TIFFSetField(p_file, TIFFTAG_SAMPLESPERPIXEL, number_of_samples_per_pixel) != 1)
        {
            std::string error_message("Could not set the number of samples per pixel in the TIFF file (");
            error_message += aFileName;
            error_message += ")";
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
        }

    // Set the min sample value
    if (TIFFSetField(p_file, TIFFTAG_MINSAMPLEVALUE, min_value) != 1)
        {
            std::string error_message("Could not set the min sample value in the TIFF file (");
            error_message += aFileName;
            error_message += ")";
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
        }

    // Set the max sample value
    if (TIFFSetField(p_file, TIFFTAG_MAXSAMPLEVALUE, max_value) != 1)
        {
            std::string error_message("Could not set the max sample value in the TIFF file (");
            error_message += aFileName;
            error_message += ")";
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
        }

    // Set the planar configuration
    TIFF_UINT16_T planar_configuration(PLANARCONFIG_CONTIG);

    if (TIFFSetField(p_file, TIFFTAG_PLANARCONFIG, planar_configuration) != 1)
        {
            std::string error_message("Could not set the planar configuration in the TIFF file (");
            error_message += aFileName;
            error_message += ")";
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
        }

    std::string software;
    software += "gVirtualXRay-";
    software += gVirtualXRay_VERSION_MAJOR;
    software += ".";
    software += gVirtualXRay_VERSION_MINOR;
    software += ".";
    software += gVirtualXRay_VERSION_PATCH;

    if (TIFFSetField(p_file, TIFFTAG_COPYRIGHT, software.data()) != 1)
        {
            std::string error_message("Could not set the software value in the TIFF file (");
            error_message += aFileName;
            error_message += ")";
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
        }

    // Set the format of a sample
    TIFF_UINT16_T format_of_a_sample;

    if (typeid(T) == typeid(unsigned char) || typeid(T) == typeid(unsigned short))
        {
        format_of_a_sample = SAMPLEFORMAT_UINT;
        }
    else if (typeid(T) == typeid(char) || typeid(T) == typeid(short))
        {
        format_of_a_sample = SAMPLEFORMAT_INT;
        }
    else if (typeid(T) == typeid(float))
        {
        format_of_a_sample = SAMPLEFORMAT_IEEEFP;
        }
    else
        {
            std::string error_message("Could not set the format of a sample in the TIFF file (");
            error_message += aFileName;
            error_message += ")";
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
        }

    if (TIFFSetField(p_file, TIFFTAG_SAMPLEFORMAT, format_of_a_sample) != 1)
        {
            std::string error_message("Could not set the compression in the TIFF file (");
            error_message += aFileName;
            error_message += ")";
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
        }

    // Length in memory of one row of pixel in the image
    tsize_t number_of_bytes_per_line(number_of_samples_per_pixel * m_width);

    // We set the strip size of the file to be size of one row of pixels
    TIFFSetField(p_file, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(p_file, m_width * number_of_samples_per_pixel));

    // Write the image data, line by line
    if (m_number_of_slices == 1)
        {
        for (unsigned int j(0); j < m_height; ++j)
            {
            int error_code(0);
            
            if (p_ubyte_data)
                {
                error_code = TIFFWriteScanline(p_file, p_ubyte_data + m_width * j, j, 0);
                }
            else if (p_ushort_data)
                {
                error_code = TIFFWriteScanline(p_file, p_ushort_data + m_width * j, j, 0);
                }

            if (error_code != 1)
                {
                // Close the file
                TIFFClose(p_file);
                
                // Release the memory if necessary
                if (p_ubyte_data)
                    {
                    delete [] p_ubyte_data;
                    p_ubyte_data = 0;
                    }
                
                if (p_ushort_data)
                    {
                    delete [] p_ushort_data;
                    p_ushort_data = 0;
                    }
                
                // Generate an error
                std::stringstream error_message;
                error_message << "Could not write the pixel data (Line " << j << ") in the TIFF file (";
                error_message << aFileName;
                error_message << ")";
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
                }
            }
        }
    else
        {
            std::string error_message("Could not write several slices in the TIFF file (");
            error_message += aFileName;
            error_message += ")";
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
        }

/*

       TIFFTAG_COLORMAP                3      uint16*            1<<BitsPerSample arrays
                   1      uint16             -
 */

    // Release the memory if necessary
    if (p_ubyte_data)
        {
        delete [] p_ubyte_data;
        p_ubyte_data = 0;
        }
    
    if (p_ushort_data)
        {
        delete [] p_ushort_data;
        p_ushort_data = 0;
        }
    
    // Close the file
    TIFFClose(p_file);

#else

    throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "Saving TIFF files is not implemented, "
            "please contact Dr FP Vidal.");

#endif
}


//------------------------------------------------------------------------
template<typename T> void Image<T>::saveTIFF(const std::string& aFileName,
                                             bool anInvertLUTFlag) const
//------------------------------------------------------------------------
{
    saveTIFF(aFileName.data(), anInvertLUTFlag);
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


#ifdef HAS_GDCM
//-----------------------------------------------------------------
template<typename T> void Image<T>::addTag(double aValue,
                                           unsigned int anAddress1,
                                           unsigned int anAddress2,
                                           gdcm::File& aFile) const
//-----------------------------------------------------------------
{
    std::stringstream message;
    message << aValue;

    addTag(message.str(), anAddress1, anAddress2, aFile);
}


//-----------------------------------------------------------------
template<typename T> void Image<T>::addTag(const char* aValue,
                                           unsigned int anAddress1,
                                           unsigned int anAddress2,
                                           gdcm::File& aFile) const
//-----------------------------------------------------------------
{
    std::stringstream message;
    message << aValue;

    addTag(std::string(aValue), anAddress1, anAddress2, aFile);
}


//-------------------------------------------------------------------
template<typename T> void Image<T>::addTag(const std::string& aValue,
                                           unsigned int anAddress1,
                                           unsigned int anAddress2,
                                           gdcm::File& aFile) const
//-------------------------------------------------------------------
{
    gdcm::DataElement data_element(gdcm::Tag(anAddress1, anAddress2));


    data_element.SetByteValue((const char*)aValue.data(), uint32_t(aValue.size()));

    aFile.GetDataSet().Insert(data_element);
}
#endif

} // namespace gVirtualXRay
