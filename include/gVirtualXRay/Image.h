#ifndef __Image_h
#define __Image_h


/**
********************************************************************************
*
*   @file       Image.h
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
//  Include
//******************************************************************************
#include <string>

#ifndef __Types_h
#include "gVirtualXRay/Types.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  Image
*   @brief  Image is a class to manage a greyscale image.
*/
//==============================================================================
template<typename T> class Image
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default constructor.
    //--------------------------------------------------------------------------
    Image();


    //------------------------------------------------------------------------
    /// Copy constructor.
    /**
    * @param anImage: the image to copy
    */
    //------------------------------------------------------------------------
    Image(const Image<T>& anImage);
    
    
    //------------------------------------------------------------------------
    /// Constructor from an array.
    /**
    * @param apData: the array to copy
    * @param aWidth: the width of the image
    * @param aHeight: the height of the image
    * @param aNumberOfSlices: the number of slices (default value: 1)
    */
    //------------------------------------------------------------------------
    Image(const T* apData,
                 unsigned int aWidth,
                 unsigned int aHeight,
                 unsigned int aNumberOfSlices = 1);


    //------------------------------------------------------------------------
    /// Constructor to build a black image.
    /**
    * @param aWidth: the width of the image
    * @param aHeight: the height of the image
    * @param aNumberOfSlices: the number of slices (default value: 1)
    * @param aDefaultValue: the pixel value (default value: 0)
    */
    //------------------------------------------------------------------------
    Image(unsigned int aWidth,
                 unsigned int aHeight,
                 unsigned int aNumberOfSlices = 1,
                 T aDefaultValue = 0);


    //------------------------------------------------------------------------
    /// Destructor.
    //------------------------------------------------------------------------
    ~Image();
    
    
    //------------------------------------------------------------------------
    /// Assignment operator (also called copy operator).
    /**
    * @param anImage: the image to copy
    * @return the updated version of the current image
    */
    //------------------------------------------------------------------------
    Image& operator=(const Image<T>& anImage);
    
    
    //------------------------------------------------------------------------
    /// Assignment operator (also called copy operator).
    /**
    * @param apImage: the data to copy
    * @return the updated version of the current image
    */
    //------------------------------------------------------------------------
    Image& operator=(const T* apImage);


    //------------------------------------------------------------------------
    /// Release the memory.
    //------------------------------------------------------------------------
    void destroy();
    
    
    //------------------------------------------------------------------------
    /// Accessor on the raw data.
    /**
    * @return the raw data
    */
    //------------------------------------------------------------------------
    T* getRawData();


    //------------------------------------------------------------------------
    /// Accessor on the raw data.
    /**
    * @return the raw data
    */
    //------------------------------------------------------------------------
    const T* getRawData() const;


    //------------------------------------------------------------------------
    /// Number of pixels along the horizontal axis
    /**
    * @return the width
    */
    //------------------------------------------------------------------------
    unsigned int getWidth() const;
    

    //------------------------------------------------------------------------
    /// Number of pixels along the vertical axis
    /**
    * @return the height
    */
    //------------------------------------------------------------------------
    unsigned int getHeight() const;
    

    void setSpacing(const VEC3& aPixelSize);


    const VEC3& getSpacing() const;


    //------------------------------------------------------------------------
    /// Compute the minimum pixel value in the image
    /**
    * @return the minimum pixel
    */
    //------------------------------------------------------------------------
    T getMinValue() const;
    

    //------------------------------------------------------------------------
    /// Compute the maximum pixel value in the image
    /**
    * @return the maximum pixel
    */
    //------------------------------------------------------------------------
    T getMaxValue() const;
    

    //------------------------------------------------------------------------
    /// Add aShiftValue to every pixel, then multiply every pixel
    /// by aScaleValue
    /**
    * @param aShiftValue: the shift parameter of the filter
    * @param aScaleValue: the scale parameter of the filter
    */
    //------------------------------------------------------------------------
    Image shiftScaleFilter(double aShiftValue, double aScaleValue) const;
    
    
    //------------------------------------------------------------------------
    /// Normalise the image between 0 and 1
    //------------------------------------------------------------------------
    Image normalised() const;
    
    
    //------------------------------------------------------------------------
    /// Normalize the image between 0 and 1
    //------------------------------------------------------------------------
    Image normalized() const;
    
    
    //------------------------------------------------------------------------
    /// Save the image in a file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    void save(const char* aFileName, bool anInvertLUTFlag = false, const char* aComment = "") const;


    //------------------------------------------------------------------------
    /// Save the image in a file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    void save(const std::string& aFileName,
            bool anInvertLUTFlag = false,
            const std::string& aComment = "") const;


    //------------------------------------------------------------------------
    /// Save the image in a PGM file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    void savePGM(const char* aFileName) const;
    
    
    //------------------------------------------------------------------------
    /// Save the image in a PGM file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    void savePGM(const std::string& aFileName) const;
    

    //------------------------------------------------------------------------
    /// Save the image in a Raw file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    void saveRaw(const char* aFileName) const;
    
    
    //------------------------------------------------------------------------
    /// Save the image in a Raw file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    void saveRaw(const std::string& aFileName) const;
    

    //------------------------------------------------------------------------
    /// Save the image in a Raw file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    void saveRAW(const char* aFileName) const;


    //------------------------------------------------------------------------
    /// Save the image in a Raw file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    void saveRAW(const std::string& aFileName) const;


    //------------------------------------------------------------------------
    /// Save the image in an ASCII file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    void saveASCII(const char* aFileName) const;
    
    
    //------------------------------------------------------------------------
    /// Save the image in an ASCII file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    void saveASCII(const std::string& aFileName) const;
   
   
    //------------------------------------------------------------------------
    /// Save the image in a MHD+RAW file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    void saveMHD(const char* aFileName) const;


    //------------------------------------------------------------------------
    /// Save the image in a MHD+RAW file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    void saveMHD(const std::string& aFileName) const;


    //------------------------------------------------------------------------
    /// Save the image in a DICOM file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    void saveDCM(const char* aFileName,
            bool anInvertLUTFlag = false,
            const char* aComment = "") const;


    //------------------------------------------------------------------------
    /// Save the image in a DICOM file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    void saveDCM(const std::string& aFileName,
            bool anInvertLUTFlag = false,
            const std::string& aComment = "") const;


    //------------------------------------------------------------------------
    /// Operator Equal to
    /**
    * @param anImage: the image to compare with
    * @return true if the images are similar,
    *         false if they are different
    */
    //------------------------------------------------------------------------
    bool operator==(const Image& anImage) const;


    //------------------------------------------------------------------------
    /// Operator Not equal to
    /**
    * @param anImage: the image to compare with
    * @return true if the images are different,
    *         false if they are similar
    */
    //------------------------------------------------------------------------
    bool operator!=(const Image& anImage) const;


    Image<T> abs() const;


    Image<T> log() const;

    double getSum() const;
    double getAverage() const;
    double getVariance() const;
    double getStandardDeviation() const;
    double computeSAE(const Image& anImage) const;
    double computeNCC(const Image& anImage) const;
    

//******************************************************************************
private:

    /// Number of pixel along the horizontal axis
    unsigned int m_width;


    /// Number of pixel along the vertical axis
    unsigned int m_height;

    
    /// Number of slices
    unsigned int m_number_of_slices;


    /// The pixel data
    T* m_p_image;


    /// The space between two successive pixels
    VEC3 m_spacing;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "Image.inl"


#endif // __Image_h
