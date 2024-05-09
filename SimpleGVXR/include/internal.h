#ifndef __internal_h
#define __internal_h

#include <vector>

#ifndef __Image_h
#include "gVirtualXRay/Image.h"
#endif


std::vector<std::vector<std::vector<float> > > getVectorImage3D(
    const gVirtualXRay::Image<float>& anImage);

#endif
