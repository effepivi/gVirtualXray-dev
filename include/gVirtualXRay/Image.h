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
*   @todo       Fix problem in marchingCubes (normals?).
*               See bug in demos (ImplicitSurface and SimpleBackProjection)
*
*   @date       30/09/2016
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

#ifdef HAS_GDCM
#include <gdcmFile.h>
#endif

#ifndef __Types_h
#include "gVirtualXRay/Types.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//******************************************************************************
//  Class declaration
//******************************************************************************
template<typename T> class Image;
template<typename T> class FFT;
template<typename T> class Sinogram;
class PolygonMesh;


//******************************************************************************
//  Function declaration
//******************************************************************************
template<typename T> Image<T> operator*(const T& aValue, const Image<T>& anImage);
template<typename T> Image<T> operator/(const T& aValue, const Image<T>& anImage);
template<typename T> Image<T> log(const Image<T>& anImage);
std::string getPixelType(const std::string& aFileName);
std::string getPixelType(const char* aFileName);


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
    /// Compute a region of interest (ROI).
    /**
    * @param i: the position of the first pixel of the ROI along the horizontal axis
    * @param j: the position of the first pixel of the ROI along the vertical axis
    * @param k: the position of the first pixel of the ROI along the depth axis
    * @param aWidth: the width of the ROI (in number of pixels)
    * @param aHeight: the height of the ROI (in number of pixels)
    * @param aDepth: the depth of the ROI (in number of pixels)
    * @return the ROI
    */
    //------------------------------------------------------------------------
    Image getROI(unsigned int i,
            unsigned int j,
            unsigned int k,
            unsigned int aWidth,
            unsigned int aHeight,
            unsigned int aDepth) const;


    //------------------------------------------------------------------------
    /// Set a pixel.
    /**
    * @param i: the position of the pixel along the horizontal axis
    * @param j: the position of the pixel along the vertical axis
    * @param aValue: the new pixel value
    */
    //------------------------------------------------------------------------
    void setPixel(unsigned int i, unsigned int j, unsigned int k, T aValue);


    //------------------------------------------------------------------------
    /// Accessor on a pixel value.
    /**
    * @param i: the position of the pixel along the horizontal axis
    * @param j: the position of the pixel along the vertical axis
    * @return the pixel value
    */
    //------------------------------------------------------------------------
    T& getPixel(unsigned int i, unsigned int j, unsigned int k = 0);
    const T& getPixel(unsigned int i, unsigned int j, unsigned int k = 0) const;


    T& getPixelValue(unsigned int i, unsigned int j, unsigned int k = 0);
    const T& getPixelValue(unsigned int i, unsigned int j, unsigned int k = 0) const;

    VEC3 getPixelPosition(unsigned int i, unsigned int j, unsigned int k = 0) const;

    T& operator[](unsigned anIndex);


    const T& operator[](unsigned anIndex) const;


    T& operator()(unsigned int i, unsigned int j, unsigned int k);


    const T& operator()(unsigned int i, unsigned int j, unsigned int k) const;


    //------------------------------------------------------------------------
    /// Assignment operator (also called copy operator).
    /**
    * @param anImage: the image to copy
    * @return the updated version of the current image
    */
    //------------------------------------------------------------------------
    Image<T>& operator=(const Image<T>& anImage);
    
    
    //------------------------------------------------------------------------
    /// Assignment operator (also called copy operator).
    /**
    * @param apImage: the data to copy
    * @return the updated version of the current image
    */
    //------------------------------------------------------------------------
    Image<T>& operator=(const T* apImage);


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
    

    //------------------------------------------------------------------------
    /// Number of pixels along the Z axis
    /**
    * @return the depth
    */
    //------------------------------------------------------------------------
    unsigned int getDepth() const;


    void setSpacing(const VEC3& aPixelSize);
    void setSpacing(const double& aPixelWidth,
            const double& aPixelHeight = 0.0,
            const double& aPixelDepth = 0.0);


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
    /// Compute the minimum pixel value in a given slice
    /**
    * @param aSliceID: the slice ID
    * @return the minimum pixel
    */
    //------------------------------------------------------------------------
    T getMinValue(unsigned int aSliceID) const;
    

    //------------------------------------------------------------------------
    /// Compute the maximum pixel value in a given slice
    /**
    * @param aSliceID: the slice ID
    * @return the maximum pixel
    */
    //------------------------------------------------------------------------
    T getMaxValue(unsigned int aSliceID) const;
    

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
    
    
    void load(const char* aFileName);
    void load(const std::string& aFileName);

    void loadSeries(const char* aPattern, int aFirstSliceIndex, int aLastSliceIndex, int anIncrementIndex = 1);
    void loadSeries(const std::string& aPattern, int aFirstSliceIndex, int aLastSliceIndex, int anIncrementIndex = 1);

    void loadDicomSeries(const char* aDirectory);
    void loadDicomSeries(const std::string& aDirectory);

    void loadASCII(const char* aFileName);
    void loadASCII(const std::string& aFileName);

    void loadMHD(const char* aFileName);
    void loadMHD(const std::string& aFileName);

    void loadRAW(const char* aFileName,
            unsigned int aWidth,
            unsigned int aHeight,
            unsigned int aNumberOfSlices = 1,
            bool aChangeEndianessFlag = false);

    void loadRAW(const std::string& aFileName,
            unsigned int aWidth,
            unsigned int aHeight,
            unsigned int aNumberOfSlices = 1,
            bool aChangeEndianessFlag = false);
    

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
    /// Save the image in a TIFF file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    void saveTIFF(const char* aFileName,
            bool anInvertLUTFlag) const;


    //------------------------------------------------------------------------
    /// Save the image in a TIFF file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    void saveTIFF(const std::string& aFileNam,
            bool anInvertLUTFlag) const;


    //------------------------------------------------------------------------
    /// Operator Equal to
    /**
    * @param anImage: the image to compare with
    * @return true if the images are similar,
    *         false if they are different
    */
    //------------------------------------------------------------------------
    bool operator==(const Image<T>& anImage) const;


    //------------------------------------------------------------------------
    /// Operator Not equal to
    /**
    * @param anImage: the image to compare with
    * @return true if the images are different,
    *         false if they are similar
    */
    //------------------------------------------------------------------------
    bool operator!=(const Image<T>& anImage) const;


    //------------------------------------------------------------------------
    /// Addition operator. Add anImage
    /**
    * @param anImage: the image to add
    * @return the resulting image
    */
    //------------------------------------------------------------------------
    Image operator+(const Image<T>& anImage) const;


    //------------------------------------------------------------------------
    /// Subtraction operator. Add anImage
    /**
    * @param anImage: the image to subtract
    * @return the resulting image
    */
    //------------------------------------------------------------------------
    Image operator-(const Image<T>& anImage) const;


    //------------------------------------------------------------------------
    /// Multiplication operator. Add anImage
    /**
    * @param anImage: the image to add
    * @return the resulting image
    */
    //------------------------------------------------------------------------
    Image operator*(const Image<T>& anImage) const;


    //------------------------------------------------------------------------
    /// Division operator. Add anImage
    /**
    * @param anImage: the image to subtract
    * @return the resulting image
    */
    //------------------------------------------------------------------------
    Image operator/(const Image<T>& anImage) const;


    //------------------------------------------------------------------------
    /// Addition assignment operator. Add anImage
    /**
    * @param anImage: the image to add
    * @return the updated version of the current image
    */
    //------------------------------------------------------------------------
    Image<T>& operator+=(const Image<T>& anImage);


    //------------------------------------------------------------------------
    /// Subraction assignment operator. Add anImage
    /**
    * @param anImage: the image to subtract
    * @return the updated version of the current image
    */
    //------------------------------------------------------------------------
    Image<T>& operator-=(const Image<T>& anImage);


    //------------------------------------------------------------------------
    /// Multiplication assignment operator. Add anImage
    /**
    * @param anImage: the image to add
    * @return the updated version of the current image
    */
    //------------------------------------------------------------------------
    Image<T>& operator*=(const Image<T>& anImage);


    //------------------------------------------------------------------------
    /// Division assignment operator. Add anImage
    /**
    * @param anImage: the image to subtract
    * @return the updated version of the current image
    */
    //------------------------------------------------------------------------
    Image<T>& operator/=(const Image<T>& anImage);


    //------------------------------------------------------------------------
    /// Addition operator. Add aValue to every pixel of the image
    /**
    * @param aValue: the value to add
    * @return the resulting image
    */
    //------------------------------------------------------------------------
    Image operator+(T aValue) const;


    //------------------------------------------------------------------------
    /// Subtraction operator. Subtract aValue to every pixel of the image
    /**
    * @param aValue: the value to subtract
    * @return the resulting image
    */
    //------------------------------------------------------------------------
    Image operator-(T aValue) const;


    //------------------------------------------------------------------------
    /// Multiplication operator. Multiply every pixel of the image by aValue
    /**
    * @param aValue: the value for the multiplication
    * @return the resulting image
    */
    //------------------------------------------------------------------------
    Image operator*(T aValue) const;


    //------------------------------------------------------------------------
    /// Division operator. Divide every pixel of the image by aValue
    /**
    * @param aValue: the value for the division
    * @return the resulting image
    */
    //------------------------------------------------------------------------
    Image operator/(T aValue) const;


    //------------------------------------------------------------------------
    /// Addition operator. Add aValue to every pixel of the image
    /**
    * @param aValue: the value to add
    * @return the updated version of the current image
    */
    //------------------------------------------------------------------------
    Image<T>& operator+=(T aValue);


    //------------------------------------------------------------------------
    /// Subtraction operator. Subtract aValue to every pixel of the image
    /**
    * @param aValue: the value to subtract
    * @return the updated version of the current image
    */
    //------------------------------------------------------------------------
    Image<T>& operator-=(T aValue);


    //------------------------------------------------------------------------
    /// Multiplication operator. Multiply every pixel of the image by aValue
    /**
    * @param aValue: the value for the multiplication
    * @return the updated version of the current image
    */
    //------------------------------------------------------------------------
    Image<T>& operator*=(T aValue);


    //------------------------------------------------------------------------
    /// Division operator. Divide every pixel of the image by aValue
    /**
    * @param aValue: the value for the division
    * @return the updated version of the current image
    */
    //------------------------------------------------------------------------
    Image<T>& operator/=(T aValue);


    //------------------------------------------------------------------------
    /// Negation operator. Compute the negative of the current image.
    /**
    * @return the negative image
    */
    //------------------------------------------------------------------------
    Image operator!() const;


    Image<T> abs() const;


    Image<T> log() const;
    
    Image<T> flipVertically() const;
    Image<T> flipHorizontally() const;


    double getSum() const;
    double getAverage() const;
    double getVariance() const;
    double getStandardDeviation() const;
    double computeSAE(const Image<T>& anImage) const;
    double computeNCC(const Image<T>& anImage) const;
    

    gVirtualXRay::FFT<T> getFFT() const;

    Image<T> transpose() const;


    Image<T> resize(unsigned int aNewWidth, unsigned int aNewHeightunsigned, unsigned int aNewNumberOfSlices) const;
    Image<T> rotate(float anAngleInDegrees) const;
    Image<T> setCanvasSize(unsigned int aNewWidth, unsigned int aNewHeight, unsigned int aNewNumberOfSlices) const;
    gVirtualXRay::Sinogram<T> radonTransform(double aFirstAngle, double anAngleStep, double aLastAngle) const;


    //------------------------------------------------------------------------
    /// Apply the marching cubes algorithm to extract the polygon mesh
    /// corresponding to aThresholdValue.
    /**
    * @param aThresholdValue:   the iso value
    * @return the polygon mesh
    */
    //------------------------------------------------------------------------
    PolygonMesh marchingCubes(const T& aFirstThresholdValue,
            const T& aSecondThresholdValue = 0,
            bool aTwoThresholdFlag = false) const;
    
    
//******************************************************************************
private:
#ifdef HAS_GDCM
    void addTag(double aValue,
            unsigned int anAddress1,
            unsigned int anAddress2,
            gdcm::File& aFile) const;


    void addTag(const char* aValue,
            unsigned int anAddress1,
            unsigned int anAddress2,
            gdcm::File& aFile) const;


    void addTag(const std::string& aValue,
            unsigned int anAddress1,
            unsigned int anAddress2,
            gdcm::File& aFile) const;
#endif


//******************************************************************************
protected:
    VEC3 VertexInterp(const T& aThresholdValue,
            const VEC3& p0,
            const VEC3& p1,
            const T& v0,
            const T& v1) const;


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

    static unsigned int g_mesh_type;
    static unsigned int g_vbo_type;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "Image.inl"


#endif // __Image_h
