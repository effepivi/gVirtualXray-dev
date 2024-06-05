#ifndef __SGVXR_Internal_h
#define __SGVXR_Internal_h

#include <vector>

#ifndef __Image_h
#include "gVirtualXRay/Image.h"
#endif


void initialiseXRayRenderer();


std::vector<std::vector<std::vector<float> > > getVectorImage3D(
    const gVirtualXRay::Image<float>& anImage);


std::vector<std::vector<float> > getVectorImage2D(
    const gVirtualXRay::Image<float>& anImage);


#endif
