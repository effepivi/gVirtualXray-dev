//******************************************************************************
//  Include
//******************************************************************************
#include <iostream>
#include <memory>

#ifndef gVXR_Exception_h
#include <gVirtualXRay/Exception.h>
#endif

#ifndef gVXR_Logger_h
#include <gVirtualXRay/Logger.h>
#endif

#ifndef gVXR_XRayBeam_h
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef gVXR_XRayDetector_h
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef gVXR_XRayRenderer_h
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef SGVXR_Utils_h
#include "Utils.h"
#endif

#ifndef SGVXR_Window_h
#include "Window.h"
#endif

#ifndef SGVXR_Internal_h
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

extern std::map<int, std::unique_ptr<SimpleGVXR::Window> > g_p_window_set;


//--------------------------------------------------------------
void initialiseXRayRenderer(unsigned char aNumberOfBitsPerFloat)
//--------------------------------------------------------------
{
//    if (aNumberOfBitsPerFloat != 16 && aNumberOfBitsPerFloat != 32)
//    {
//        gVirtualXRay::LOGGER.logWarning("The number of bits per floating-point numbers should be 16 or 32, not ") << 
//            aNumberOfBitsPerFloat << ". The renderer will fall back to 32 bits." << std::endl;
//    }

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
            if (g_p_window_set.empty())
            {
                createOpenGLContext();
            }

            int internal_texture_format = GL_RGB32F;
            if (aNumberOfBitsPerFloat == 32)
            {
                internal_texture_format = GL_RGB32F;
            }
            else if (aNumberOfBitsPerFloat == 16)
            {
                internal_texture_format = GL_RGB16F;
            }
            else
            {
                internal_texture_format = 0;
            }

            if (aNumberOfBitsPerFloat == 32 || aNumberOfBitsPerFloat == 16)
                gVirtualXRay::LOGGER.logNow("Initialise the renderer with ") <<
                    int(aNumberOfBitsPerFloat) << " bits per floating-point numbers in GPU memory" << std::endl;
            else
                gVirtualXRay::LOGGER.logNow("Initialise the renderer with mixed-precision floating-point numbers in GPU memory") << std::endl;

            // Initialise the renderer
            g_xray_renderer.initialise(gVirtualXRay::XRayRenderer::OPENGL,
                internal_texture_format,
                &g_xray_detector,
                &g_xray_beam);

            enableArtefactFilteringOnGPU();
            g_xray_renderer.useNegativeFilteringFlag(false);
            g_renderer_initialised = true;
        }
    }
}


//-----------------------------------------------------------------
gVirtualXRay::VEC3 convertVec3D(const std::vector<double>& aVector)
//-----------------------------------------------------------------
{
    gVirtualXRay::VEC3 output;

    if (aVector.size() != 3)
        throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__, "The vector size is incompatible with VEC3. The conversion is impossible.");

    output[0] = static_cast<float>(aVector[0]);
    output[1] = static_cast<float>(aVector[1]);
    output[2] = static_cast<float>(aVector[2]);

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

    output[0] = static_cast<float>(aVector[0]);
    output[1] = static_cast<float>(aVector[1]);

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
