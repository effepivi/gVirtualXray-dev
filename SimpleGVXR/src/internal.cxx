//******************************************************************************
//  Include
//******************************************************************************
#include <iostream>
#include <memory>

#ifndef GVXR_EXCEPTION_H
#include <gVirtualXRay/Exception.h>
#endif

#ifndef GVXR_LOGGER_H
#include <gVirtualXRay/Logger.h>
#endif

#ifndef GVXR_XRAY_BEAM_H
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef GVXR_XRAY_DETECTOR_H
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef GVXR_XRAY_RENDERER_H
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

extern bool g_has_own_gl_context;
extern std::vector<int> g_p_window_id_stack;
extern gVirtualXRay::Matrix4x4<GLfloat> g_sample_transformation_matrix;

extern gVirtualXRay::XRayBeam g_xray_beam;         // X-ray beam
extern gVirtualXRay::XRayDetector g_xray_detector; // X-ray detector
extern gVirtualXRay::XRayRenderer g_xray_renderer; // X-ray graphics engine

extern std::map<int, std::unique_ptr<SimpleGVXR::Window> > g_p_window_set;


//------------------------------------------------------
void initialiseXRayRenderer(uint8_t aMixedPrecisionFlag)
//------------------------------------------------------
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
            if (g_xray_renderer.getComputingMode() == gVirtualXRay::XRayRenderer::OPENGL)
            {
                if (g_p_window_set.empty())
                {
                    createOpenGLContext();
                }

                gVirtualXRay::LOGGER.logNow("Mixed-precision flag: ") << int(aMixedPrecisionFlag) << std::endl;
                gVirtualXRay::LOGGER.logNow("Unclean L-buffer precision: ") << (gVirtualXRay::getPrecisionUncleanedLBuffer(aMixedPrecisionFlag) ? "32" : "16") << " bits" << std::endl;
                gVirtualXRay::LOGGER.logNow("Clean L-buffer precision: ") << (gVirtualXRay::getPrecisionCleanedLBuffer(aMixedPrecisionFlag) ? "32" : "16") << " bits" << std::endl;
                gVirtualXRay::LOGGER.logNow("Cumulative L-buffer precision: ") << (gVirtualXRay::getPrecisionCumulatedLBuffer(aMixedPrecisionFlag) ? "32" : "16") << " bits" << std::endl;
                gVirtualXRay::LOGGER.logNow("Energy fluence precision: ") << (gVirtualXRay::getPrecisionEnergyFluence(aMixedPrecisionFlag) ? "32" : "16") << " bits" << std::endl;
                gVirtualXRay::LOGGER.logNow("Sum MuX dX precision: ") << (gVirtualXRay::getPrecisionSumMuXdX(aMixedPrecisionFlag) ? "32" : "16") << " bits" << std::endl;

                if (gVirtualXRay::isFullPrecision(aMixedPrecisionFlag))
                    gVirtualXRay::LOGGER.logNow("Initialise the renderer with 32") <<
                        " bits per floating-point numbers in GPU memory" << std::endl;
                else if (gVirtualXRay::isHalfPrecision(aMixedPrecisionFlag))
                    gVirtualXRay::LOGGER.logNow("Initialise the renderer with 16") <<
                    " bits per floating-point numbers in GPU memory" << std::endl;
                else
                    gVirtualXRay::LOGGER.logNow("Initialise the renderer with mixed-precision floating-point numbers in GPU memory") << std::endl;

                // Initialise the renderer
                g_xray_renderer.initialise(gVirtualXRay::XRayRenderer::OPENGL,
                    aMixedPrecisionFlag,
                    &g_xray_detector,
                    &g_xray_beam);

                enableArtefactFilteringOnGPU();
            }
            else if (g_xray_renderer.getComputingMode() == gVirtualXRay::XRayRenderer::OPENMP)
            {
                // Initialise the renderer
                g_xray_renderer.initialise(gVirtualXRay::XRayRenderer::OPENMP,
                    aMixedPrecisionFlag,
                    &g_xray_detector,
                    &g_xray_beam);
            }
            else {
                throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__, "The renderer is currently unsupported.");
            }

            g_xray_renderer.useNegativeFilteringFlag(false);
            g_renderer_initialised = true;
        }
    }
}


void internalComputeXRayImage(bool anIntegrateEnergyFlag)
//-------------------------------------------------------
{
    // There is no OpenGL context
    if (!g_has_own_gl_context && (g_p_window_id_stack.empty() || g_p_window_set.empty()))
    {
        // Create a window
        createWindow();
    }

    // The renderer is ready
    if (g_xray_renderer.isReady())
    {
        // Compute the X-ray image
        g_xray_renderer.computeImage(gVirtualXRay::MATRIX4(), anIntegrateEnergyFlag);
    }
    // The renderer is not ready
    else
    {
        gVirtualXRay::LOGGER.logWarning("X-ray renderer not ready.") << std::endl;

        if (!g_beam_energy_initialised)
            gVirtualXRay::LOGGER.logWarning("The energy spectrum has not been set.") << std::endl;

        if (!g_beam_shape_initialised)
            gVirtualXRay::LOGGER.logWarning("The beam shape has not been set.") << std::endl;

        if (!g_source_position_initialised)
            gVirtualXRay::LOGGER.logWarning("The source's position has not been set.") << std::endl;

        if (!g_detector_up_vector_initialised)
            gVirtualXRay::LOGGER.logWarning("The detector's orientation has not been set.") << std::endl;

        if (!g_detector_size_initialised)
            gVirtualXRay::LOGGER.logWarning("The detector's pixel resolution has not been set.") << std::endl;

        if (!g_detector_pixel_size_initialised)
            gVirtualXRay::LOGGER.logWarning("The detector's pixel spacing has not been set.") << std::endl;
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


//-------------------------------------------------
void PNG_write_uint32(std::ostream& os, uint32_t v)
//-------------------------------------------------
{
    os.put((v >> 24) & 0xff);
    os.put((v >> 16) & 0xff);
    os.put((v >> 8) & 0xff);
    os.put(v & 0xff);
}


//----------------------------------------------------
void PNG_write_chunk(std::ostream& os,
                     const char type[4],
                     const std::vector<uint8_t>& data)
//----------------------------------------------------
{
    PNG_write_uint32(os, static_cast<uint32_t>(data.size()));

    os.write(type, 4);

    if (!data.empty())
        os.write(reinterpret_cast<const char*>(data.data()),
                 data.size());

    uint32_t crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc,
                reinterpret_cast<const Bytef*>(type),
                4);

    if (!data.empty())
        crc = crc32(crc,
                    reinterpret_cast<const Bytef*>(data.data()),
                    data.size());

    PNG_write_uint32(os, crc);
}


//---------------------------------------------------------------
void PNG_write_uint32(std::vector<uint8_t>& buffer, uint32_t v)
//---------------------------------------------------------------
{
    buffer.push_back((v >> 24) & 0xff);
    buffer.push_back((v >> 16) & 0xff);
    buffer.push_back((v >>  8) & 0xff);
    buffer.push_back(v & 0xff);
}


//-------------------------------------------------------------
void PNG_write_chunk(std::vector<uint8_t>& buffer,
                     const char type[4],
                     const std::vector<uint8_t>& data)
//-------------------------------------------------------------
{
    PNG_write_uint32(buffer,
                     static_cast<uint32_t>(data.size()));

    buffer.insert(buffer.end(), type, type + 4);

    buffer.insert(buffer.end(),
                  data.begin(),
                  data.end());

    uint32_t crc = crc32(0L, Z_NULL, 0);

    crc = crc32(crc,
                reinterpret_cast<const Bytef*>(type),
                4);

    if (!data.empty())
    {
        crc = crc32(crc,
                    reinterpret_cast<const Bytef*>(data.data()),
                    data.size());
    }

    PNG_write_uint32(buffer, crc);
}
