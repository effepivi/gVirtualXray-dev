//******************************************************************************
//  Include
//******************************************************************************
#include <iostream>
#include <iomanip>

#include <fstream>
#include <vector>
#include <cmath>
#include <numeric>

#include "gtest/gtest.h"

#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef __Image_h
#include "gVirtualXRay/Image.h"
#endif

#ifndef __Context_h
#include "gVirtualXRay/Context.h"
#endif

/*
#ifndef __InvalidInternalFormalException_h
#include "gVirtualXRay/InvalidInternalFormalException.h"
#endif

#ifndef __PhotonCrossSection_h
#include "gVirtualXRay/PhotonCrossSection.h"
#endif

#ifndef __InvalidImageSizeException_h
#include "gVirtualXRay/InvalidImageSizeException.h"
#endif

#ifndef __NoDetectorException_h
#include "gVirtualXRay/NoDetectorException.h"
#endif

#ifndef __NoBeamException_h
#include "gVirtualXRay/NoBeamException.h"
#endif
*/

#ifndef __utils_h
#include "utils.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;
using namespace std;


//******************************************************************************
//  Function declaration
//******************************************************************************


//------------------------------------
TEST(TestImageClass, TestConstructors)
//------------------------------------
{
    // Image();
    // Image(const Image<T>& anImage);
    // Image(const char* aFileName);
    // Image(const std::string& aFileName);
    // Image(const T* apData,
    //              unsigned int aWidth,
    //              unsigned int aHeight,
    //              unsigned int aNumberOfSlices = 1,
    //              const VEC3& aVoxelSize = VEC3(1, 1, 1));
    // Image(unsigned int aWidth,
    //              unsigned int aHeight,
    //              unsigned int aNumberOfSlices = 1,
    //              T aDefaultValue = 0);
}


//------------------------------------------
TEST(TestImageClass, TestOpenGLFloatTexture)
//------------------------------------------
{
	double sigma = 10;
	int kernel_size = 2 * ceil(3 * sigma) + 1;

	Image<float> float_2d_kernel = gauss2D<float>(kernel_size, sigma).normalised();
    float_2d_kernel.save("float_2d_kernel.mha");

    Image<unsigned char> segmentation_cpu = float_2d_kernel.threshold(0.25f, 0.75f, (unsigned char)(128), (unsigned char)(255), true);
    segmentation_cpu.save("segmentation_cpu.mha");

	// Create an OpenGL context
	Context GL_context;
	initGL(GL_context, 4, 3);
    Image<unsigned char> segmentation_gpu = float_2d_kernel.threshold(0.25f, 0.75f, (unsigned char)(128), (unsigned char)(255), true);
    segmentation_gpu.save("segmentation_gpu.mha");

	EXPECT_EQ(segmentation_cpu, segmentation_gpu);

//    Image<unsigned char> uchar_3d_image(1024, 512, 3, 200);
//
//    for (int j = 0; j < float_2d_image.getHeight(); ++j)
//    {
//        for (int i = 0; i < float_2d_image.getWidth(); ++i)
//        {
//            float_2d_image(i, j, 0) = i + j;
//            uchar_2d_image(i, j, 0) = i + j;
//        }
//    }
//
//    for (int k = 0; k < float_3d_image.getDepth(); ++k)
//    {
//        for (int j = 0; j < float_3d_image.getHeight(); ++j)
//        {
//			for (int i = 0; i < float_3d_image.getWidth(); ++i)
//			{
//	            float_3d_image(i, j, k) = i + j + k;
//	            uchar_3d_image(i, j, k) = i + j + k;
//			}
//        }
//    }
//
//    // Create an OpenGL context
//    Context GL_context;
//    initGL(GL_context, 4, 3);
//
//    // Load the images on the device
//    float_2d_image.transferHostToDevice();
//    float_3d_image.transferHostToDevice();
//
//    uchar_2d_image.transferHostToDevice();
//    uchar_3d_image.transferHostToDevice();
//
//    ASSERT_EQ(float_2d_image.getTextureId() > 0, true);
//    ASSERT_EQ(float_3d_image.getTextureId() > 0, true);
//
//    ASSERT_EQ(uchar_2d_image.getTextureId() > 0, true);
//    ASSERT_EQ(uchar_3d_image.getTextureId() > 0, true);
//
//    // Check the data is as expected
//    for (int j = 0; j < float_2d_image.getHeight(); ++j)
//    {
//        for (int i = 0; i < float_2d_image.getWidth(); ++i)
//        {
//            EXPECT_FLOAT_EQ(float_2d_image(i, j, 0), i + j);
//            EXPECT_EQ(uchar_2d_image(i, j, 0), (unsigned char)(i + j));
//        }
//    }
//
//    // Check the data is as expected
//    for (int k = 0; k < float_3d_image.getDepth(); ++k)
//    {
//        for (int j = 0; j < float_3d_image.getHeight(); ++j)
//        {
//			for (int i = 0; i < float_3d_image.getWidth(); ++i)
//			{
//				EXPECT_FLOAT_EQ(float_3d_image(i, j, k), i + j + k);
//				EXPECT_EQ(uchar_3d_image(i, j, k), (unsigned char)(i + j + k));
//			}
//        }
//    }
//
//    // Retrieve the images
//    uchar_2d_image = float_2d_image.threshold(100.0f, 200.0f, (unsigned char)(0), (unsigned char)(255), true);
////    uchar_3d_image.threshold(100.0f, 200.0f, (unsigned char)(0), (unsigned char)(255));
//
//
////    float_2d_image.transferDeviceToHost();
////    float_3d_image.transferDeviceToHost();
////
////    uchar_2d_image.transferDeviceToHost();
////    uchar_3d_image.transferDeviceToHost();
////
////
////
////    float_2d_image.destroyOpenGLTexture();
////    float_3d_image.destroyOpenGLTexture();
////
////    uchar_2d_image.destroyOpenGLTexture();
////    uchar_3d_image.destroyOpenGLTexture();
////
////    float_2d_image.save("float_2d_image.mha");
////    float_3d_image.save("float_3d_image.mha");
//
//    uchar_2d_image.save("uchar_2d_image.mha");
////    uchar_3d_image.save("uchar_3d_image.mha");
}


//------------------------------------
TEST(TestImageClass, TestMakeBlank)
//------------------------------------
{
    // void makeBlank(const T& aDefaultColour = 0);
}


//-------------------------------------
TEST(TestImageClass, TestConvolution2D)
//-------------------------------------
{
}


//-------------------------------
TEST(TestImageClass, TestPadding)
//-------------------------------
{
    const Image<unsigned char> input(2, 2, 2, 128);

    Image<unsigned char> output = input.constantPadFilter(255);

    const unsigned char* p_buffer = output.getRawData();

    ASSERT_EQ(1, 1);
}


// //------------------------------------
// TEST(TestImageClass, Test)
// //------------------------------------
// {
// }


// //------------------------------------
// TEST(TestImageClass, Test)
// //------------------------------------
// {
// }


// //------------------------------------
// TEST(TestImageClass, Test)
// //------------------------------------
// {
// }


// //------------------------------------
// TEST(TestImageClass, Test)
// //------------------------------------
// {
// }


// //------------------------------------
// TEST(TestImageClass, Test)
// //------------------------------------
// {
// }


// //------------------------------------
// TEST(TestImageClass, Test)
// //------------------------------------
// {
// }


// //------------------------------------
// TEST(TestImageClass, Test)
// //------------------------------------
// {
// }


// //------------------------------------
// TEST(TestImageClass, Test)
// //------------------------------------
// {
// }


// //------------------------------------
// TEST(TestImageClass, Test)
// //------------------------------------
// {
// }


// //------------------------------------
// TEST(TestImageClass, Test)
// //------------------------------------
// {
// }


