#ifndef __SGVXR_Internal_h
#define __SGVXR_Internal_h

#include <vector>

#ifndef __Image_h
#include "gVirtualXRay/Image.h"
#endif

#ifndef __Vec2_h
#include "gVirtualXRay/Vec2.h"
#endif

#ifndef __Vec3_h
#include "gVirtualXRay/Vec3.h"
#endif


void initialiseXRayRenderer();


std::vector<std::vector<std::vector<float> > > getVectorImage3D(
    const gVirtualXRay::Image<float>& anImage);


std::vector<std::vector<float> > getVectorImage2D(
    const gVirtualXRay::Image<float>& anImage);


gVirtualXRay::VEC3 convertVec3D(const std::vector<double>& aVector);
gVirtualXRay::VEC3 convertVec3D(const std::vector<float>& aVector);

gVirtualXRay::VEC2 convertVec2D(const std::vector<double>& aVector);
gVirtualXRay::VEC2 convertVec2D(const std::vector<float>& aVector);



#endif
