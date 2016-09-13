#ifndef __FFT_h
#define __FFT_h


/**
********************************************************************************
*
*   @file       FFT.h
*
*   @brief      Class to compute the FFT of a greyscale image.
*
*   @version    1.0
*
*   @date       13/09/2016
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


#ifndef __Image_h
#include "gVirtualXRay/Image.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  FFT
*   @brief  FFT is a class to compute the FFT of a greyscale image.
*/
//==============================================================================
template<typename T> class FFT
//------------------------------------------------------------------------------
{
    //******************************************************************************
    public:
        //--------------------------------------------------------------------------
        /// Default constructor.
        //--------------------------------------------------------------------------
        FFT();


        //------------------------------------------------------------------------
        /// Copy constructor.
        /**
        * @param aFFT: the FFT to copy
        */
        //------------------------------------------------------------------------
        FFT(const FFT<T>& aFFT);


        //------------------------------------------------------------------------
        /// Constructor to build a FFT.
        /**
        * @param aWidth: the width of the FFT
        * @param aHeight: the height of the FFT
        */
        //------------------------------------------------------------------------
        FFT(unsigned int aWidth,
                     unsigned int aHeight);


        //------------------------------------------------------------------------
        /// Destructor.
        //------------------------------------------------------------------------
        //~FFT();


        //------------------------------------------------------------------------
        /// Assignment operator (also called copy operator).
        /**
        * @param aFFT: the FFT to copy
        * @return the updated version of the current FFT
        */
        //------------------------------------------------------------------------
        FFT<T>& operator=(const FFT<T>& aFFT);

        static FFT<T> computeFFT(const Image<T>& anImage);


        Image<T> getInverseFFT() const;


        FFT<T> filterRamp() const;

        FFT<T> rhoFilter(const std::string& aFilterType, float aScalingFactor);
        FFT<T> rhoFilter(const char* aFilterType, float aScalingFactor);

        Image<T> getMagnitude() const;
        //Image& getMagnitude();
        Image<T> getPhase() const;
       // Image& getPhase();



    private:
        static unsigned int powerOfTwo(unsigned int i);


        static Image<T> swapQuadrants(const Image<T>& anImage);

        Image<T> m_real;
        Image<T> m_img;

        //Image m_magnitude;
        //Image m_phase;

        unsigned int m_origin_image_width;
        unsigned int m_origin_image_height;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "FFT.inl"


#endif // __FFT_h
