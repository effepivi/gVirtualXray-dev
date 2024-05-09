#ifndef __internal_h
#include "internal.h"
#endif

std::vector<std::vector<std::vector<float> > > getVectorImage3D(
    const gVirtualXRay::Image<float>& anImage)
{
	std::vector < std::vector < std::vector<float> > > output;

    for (unsigned int k = 0; k < anImage.getDepth(); ++k)
    {
    	std::vector < std::vector<float> > slice;
        for (unsigned int j = 0; j < anImage.getHeight(); ++j)
		{
			std::vector<float> row;
	        for (unsigned int i = 0; i < anImage.getWidth(); ++i)
			{
	        	row.push_back(anImage.getPixel(i, j, k));
			}

			slice.push_back(row);
		}

		output.push_back(slice);
    }

    return output;
}

