//******************************************************************************
//  Include
//******************************************************************************
#include <iostream>

#ifndef __Exception_h
#include <gVirtualXRay/Exception.h>
#endif

#ifndef __Logger_h
#include <gVirtualXRay/Logger.h>
#endif

#ifndef __XRayBeam_h
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef __XRayDetector_h
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef __XRayRenderer_h
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef __SGVXR_Utils_h
#include "Utils.h"
#endif

#ifndef __SGVXR_internal_h
#include "internal.h"
#endif


//******************************************************************************
//  Namespace
//******************************************************************************


//******************************************************************************
//  Global variables
//******************************************************************************
extern bool g_beam_shape_initialised;
extern bool g_beam_energy_initialised;
extern bool g_source_position_initialised;
extern bool g_detector_position_initialised;
extern bool g_detector_up_vector_initialised;
extern bool g_detector_size_initialised;
extern bool g_detector_pixel_size_initialised;
extern bool g_renderer_initialised;

extern gVirtualXRay::XRayBeam g_xray_beam;         // X-ray beam
extern gVirtualXRay::XRayDetector g_xray_detector; // X-ray detector
extern gVirtualXRay::XRayRenderer g_xray_renderer; // X-ray graphics engine

//---------------------------
void initialiseXRayRenderer()
//---------------------------
{
    if (g_beam_shape_initialised &&
        g_beam_energy_initialised &&
        g_source_position_initialised &&
        g_detector_position_initialised &&
        g_detector_up_vector_initialised &&
        g_detector_size_initialised &&
        g_detector_pixel_size_initialised)
    {
        // The renderer is not initialised
        if (!g_renderer_initialised)
        {
            gVirtualXRay::LOGGER.logNow("Initialise the renderer") << std::endl;
            // Initialise the renderer
            g_xray_renderer.initialise(gVirtualXRay::XRayRenderer::OPENGL,
                GL_RGB32F,
                &g_xray_detector,
                &g_xray_beam);

            enableArtefactFilteringOnGPU();
            g_xray_renderer.useNegativeFilteringFlag(false);
            g_renderer_initialised = true;
        }
    }
}


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


std::vector<std::vector<float> > getVectorImage2D(
    const gVirtualXRay::Image<float>& anImage) {
    std::vector < std::vector<float> > output;

    for (unsigned int j = 0; j < anImage.getHeight(); ++j) {
    std::vector<float> row;
    row.insert(row.end(), anImage.getRawData() + j * anImage.getWidth(),
    anImage.getRawData() + (j + 1) * anImage.getWidth());

    output.push_back(row);
    }

    return output;
}


//-----------------------------------------------------------------
gVirtualXRay::VEC3 convertVec3D(const std::vector<double>& aVector)
//-----------------------------------------------------------------
{
    gVirtualXRay::VEC3 output;

    if (aVector.size() != 3)
        throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__, "The vector size is incompatible with VEC3. The conversion is impossible.");

    output[0] = aVector[0];
    output[1] = aVector[1];
    output[2] = aVector[2];

    return output;
}


//----------------------------------------------------------------
gVirtualXRay::VEC3 convertVec3D(const std::vector<float>& aVector)
//----------------------------------------------------------------
{
    gVirtualXRay::VEC3 output;

    if (aVector.size() != 3)
        throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__, "The vector size is incompatible with VEC3. The conversion is impossible.");

    output[0] = aVector[0];
    output[1] = aVector[1];
    output[2] = aVector[2];

    return output;
}


//-----------------------------------------------------------------
gVirtualXRay::VEC2 convertVec2D(const std::vector<double>& aVector)
//-----------------------------------------------------------------
{
    gVirtualXRay::VEC2 output;

    if (aVector.size() != 2)
        throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__, "The vector size is incompatible with VEC2. The conversion is impossible.");

    output[0] = aVector[0];
    output[1] = aVector[1];

    return output;
}


//----------------------------------------------------------------
gVirtualXRay::VEC2 convertVec2D(const std::vector<float>& aVector)
//----------------------------------------------------------------
{
    gVirtualXRay::VEC2 output;

    if (aVector.size() != 2)
        throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__, "The vector size is incompatible with VEC2. The conversion is impossible.");

    output[0] = aVector[0];
    output[1] = aVector[1];

    return output;
}
