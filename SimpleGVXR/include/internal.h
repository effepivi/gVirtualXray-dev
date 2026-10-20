#ifndef SGVXR_Internal_h
#define SGVXR_Internal_h

//******************************************************************************
//  Include
//******************************************************************************
#include <vector>

#ifndef GVXR_IMAGE_H
#include "gVirtualXRay/Image.h"
#endif

#ifndef GVXR_VEC2_H
#include "gVirtualXRay/Vec2.h"
#endif

#ifndef GVXR_VEC3_H
#include "gVirtualXRay/Vec3.h"
#endif


//******************************************************************************
//  Namespace
//******************************************************************************


//******************************************************************************
//  Global variables
//******************************************************************************


//******************************************************************************
//  Function declarations
//******************************************************************************


void initialiseXRayRenderer(uint8_t aMixedPrecisionFlag);

void internalComputeXRayImage(bool anIntegrateEnergyFlag);

template<typename T> std::vector<std::vector<std::vector<T> > > getVectorImage3D(
    const gVirtualXRay::Image<T>& anImage);

template<typename T> std::vector<std::vector<T> > getVectorImage2D(
    const gVirtualXRay::Image<T>& anImage);

template<typename T> gVirtualXRay::Image<T> getImageVector3D(
    const std::vector<std::vector<std::vector<T> > > apVolume);

template<typename T> gVirtualXRay::Image<T> getImageVector2D(
    const std::vector<std::vector<T> > apImage);



gVirtualXRay::VEC3 convertVec3D(const std::vector<double>& aVector);
gVirtualXRay::VEC3 convertVec3D(const std::vector<float>& aVector);

gVirtualXRay::VEC2 convertVec2D(const std::vector<double>& aVector);
gVirtualXRay::VEC2 convertVec2D(const std::vector<float>& aVector);


template<typename T> void _makeIsoSurface(const std::string& aMeshLabel,
                    const std::vector<std::vector<std::vector<T>>>& apVolume,
                    T anIsoValue,
                    float aCentrePositionX,
                    float aCentrePositionY,
                    float aCentrePositionZ,
                    float aVoxelWidth,
                    float aVoxelHeight,
                    float aVoxelDepth,
                    const std::string& aUnitOfLength,
                    const std::string& aParent,
                    unsigned char aVerboseLevel);


template<typename T> void _makeIsoSurface(const std::string& aMeshLabel,
                    const std::vector<std::vector<std::vector<T>>>& apVolume,
                    T aLowerIsoValue,
                    T anUpperIsoValue,
                    float aCentrePositionX,
                    float aCentrePositionY,
                    float aCentrePositionZ,
                    float aVoxelWidth,
                    float aVoxelHeight,
                    float aVoxelDepth,
                    const std::string& aUnitOfLength,
                    const std::string& aParent,
                    unsigned char aVerboseLevel);


template<typename T> void _makeIsoSurface(const std::string& aMeshLabel,
                    const T* apVolume,
                    int aNumberOfVoxelsZ,
                    int aNumberOfVoxelsY,
                    int aNumberOfVoxelsX,
                    T anIsoValue,
                    float aCentrePositionX,
                    float aCentrePositionY,
                    float aCentrePositionZ,
                    float aVoxelWidth,
                    float aVoxelHeight,
                    float aVoxelDepth,
                    const std::string& aUnitOfLength,
                    const std::string& aParent,
                    unsigned char aVerboseLevel);


template<typename T> void _makeIsoSurface(const std::string& aMeshLabel,
                    const T* apVolume,
                    int aNumberOfVoxelsZ,
                    int aNumberOfVoxelsY,
                    int aNumberOfVoxelsX,
                    T aLowerIsoValue,
                    T anUpperIsoValue,
                    float aCentrePositionX,
                    float aCentrePositionY,
                    float aCentrePositionZ,
                    float aVoxelWidth,
                    float aVoxelHeight,
                    float aVoxelDepth,
                    const std::string& aUnitOfLength,
                    const std::string& aParent,
                    unsigned char aVerboseLevel);


// Utility function to trim whitespace
std::string trimWhiteSpace(const std::string& str);


//------------------------------------------------------------------------------
/// Write a 32-bit unsigned integer in big-endian format.
/**
 *  PNG stores integer values using network byte order (most significant
 *  byte first). This helper writes a 32-bit unsigned integer to an
 *  output stream following the PNG specification.
 *
 *  @param os: output stream.
 *  @param v: value to write.
 */
//------------------------------------------------------------------------------
void PNG_write_uint32(std::ostream& os, uint32_t v);


//--------------------------------------------------------------------------
/// Write a PNG chunk.
/**
 *  A PNG chunk is composed of:
 *  - a 4-byte length field;
 *  - a 4-byte chunk type;
 *  - the chunk payload;
 *  - a 4-byte CRC checksum.
 *
 *  The CRC is computed from the chunk type and payload according to
 *  the PNG specification.
 *
 *  @param os: output stream.
 *  @param type: four-character PNG chunk type (e.g. "IHDR", "IDAT",
 *               or "IEND").
 *  @param data: chunk payload.
 */
//--------------------------------------------------------------------------
void  PNG_write_chunk(std::ostream& os, const char type[4], const std::vector<uint8_t>& data);


#include "internal.inl"


#endif
