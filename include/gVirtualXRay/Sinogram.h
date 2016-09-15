#ifndef __Sinogram_h
#define __Sinogram_h


/**
********************************************************************************
*
*   @file       Sinogram.h
*
*   @brief      Class to reconstruct images from a sinogram.
*
*   @version    1.0
*
*   @date       14/09/2016
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
*   @class  Sinogram
*   @brief  Sinogram is a class to reconstruct images from a sinogram.
*/
//==============================================================================
template<typename T> class Sinogram: public Image<T>
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    Sinogram();
    Sinogram(const Image<T> anImage);
    Sinogram(unsigned int aWidth,
            unsigned int aHeight,
            unsigned int aNumberOfSlices = 1);
    
    Image<T> ART(double aFirstAngle, double anAngleStep, double aLastAngle, unsigned int aNumberOfIterations = 0, const char* aDirectory = 0) const;
    Image<T> MART(double aFirstAngle, double anAngleStep, double aLastAngle, unsigned int aNumberOfIterations = 0, const char* aDirectory = 0) const;
    Image<T> SIRT(double aFirstAngle, double anAngleStep, double aLastAngle, unsigned int aNumberOfIterations = 0, const char* aDirectory = 0) const;
    Image<T> simpleBackProjection(double aFirstAngle, double anAngleStep, double aLastAngle, const char* aDirectorye = 0) const;
    Image<T> filteredBackProjection(double aFirstAngle, double anAngleStep, double aLastAngle, const char* aFilterType = 0, float aScalingFactor = 1.0, const char* aDirectory = 0) const;
    
    Image<T> ART(double aFirstAngle, double anAngleStep, double aLastAngle, unsigned int aNumberOfIterations, const std::string& aDirectory) const;
    Image<T> MART(double aFirstAngle, double anAngleStep, double aLastAngle, unsigned int aNumberOfIterations, const std::string& aDirectory) const;
    Image<T> SIRT(double aFirstAngle, double anAngleStep, double aLastAngle, unsigned int aNumberOfIterations, const std::string& aDirectory) const;
    Image<T> simpleBackProjection(double aFirstAngle, double anAngleStep, double aLastAngle, const std::string& aDirectory) const;
    Image<T> filteredBackProjection(double aFirstAngle, double anAngleStep, double aLastAngle, const std::string& aFilterType, float aScalingFactor, const std::string& aDirectory) const;


private:
};


} // namespace gVirtualXRay


//******************************************************************************
#include "Sinogram.inl"


#endif // __Sinogram_h
