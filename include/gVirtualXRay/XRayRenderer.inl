/*

Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
http://www.fpvidal.net/
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the Bangor University nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


/**
********************************************************************************
*
*	@file		XRayRenderer.inl
*
*	@brief		Class to compute and renderer X-ray images on GPU.
*
*	@version	1.0
*
*	@date		01/12/2013
*
*	@author		Dr Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net), 
*   http://www.fpvidal.net/, Dec 2014, 2014, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifdef HAS_ITK
#include <itkImageFileWriter.h>
#endif

#include <sstream>

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef __Utilities_h
#include "gVirtualXRay/Utilities.h"
#endif


//******************************************************************************
//  define
//******************************************************************************

// 2D FBOs first, then 3D FBOs
#define XRAY_DETECTOR_UNCLEANED_LBUFFER_FBO_ID           0
#define XRAY_DETECTOR_CLEANED_LBUFFER_FBO_ID             1
#define XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID          2
#define XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID              3
#define XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID                  4

// 2D textures first, then 3D textures
#define XRAY_DETECTOR_UNCLEANED_LBUFFER_TEXTURE_NAME_ID  0
#define XRAY_DETECTOR_CLEANED_LBUFFER_TEXTURE_NAME_ID    1
#define XRAY_DETECTOR_CUMULATED_LBUFFERS_TEXTURE_NAME_ID 2
#define XRAY_DETECTOR_ENERGY_FLUENCE_TEXTURE_NAME_ID     3
#define XRAY_DETECTOR_SUM_MUx_Dx_TEXTURE_NAME_ID         4


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


    //----------------------------------
    inline XRayRenderer::~XRayRenderer()
    //----------------------------------
    {
        reset();
    }


    //------------------------------------------------------------
    inline void XRayRenderer::useNegativeFilteringFlag(bool aFlag)
    //------------------------------------------------------------
    {
        m_negative_display_flag = aFlag;
    }


    //--------------------------------------------------------
    inline bool XRayRenderer::getNegativeFilteringFlag() const
    //--------------------------------------------------------
    {
        return (m_negative_display_flag);
    }


    //---------------------------------------------------------------------
    inline void XRayRenderer::addInnerSurface(const PolygonMesh* apSurface)
    //---------------------------------------------------------------------
    {
        m_p_inner_surface_set.push_back(const_cast<PolygonMesh*>(apSurface));
    }


    //---------------------------------------------------------------------
    inline void XRayRenderer::addOuterSurface(const PolygonMesh* apSurface)
    //---------------------------------------------------------------------
    {
        m_p_outer_surface = const_cast<PolygonMesh*>(apSurface);
    }


    //---------------------------------------------
    inline void XRayRenderer::removeInnerSurfaces()
    //---------------------------------------------
    {
        m_p_inner_surface_set.clear();
    }


    //--------------------------------------------
    inline void XRayRenderer::removeOuterSurface()
    //--------------------------------------------
    {
        m_p_outer_surface = 0;
    }


    //--------------------------------------------------------------
    inline void XRayRenderer::printFBO(unsigned int aFBOID,
                                       const std::string& aFileName)
    //--------------------------------------------------------------
    {
        printFBO(aFBOID, aFileName.data());
    }


    //--------------------------------------------------------------------
    inline void XRayRenderer::print2DTexture(unsigned int aTextureID,
                                             const std::string& aFileName)
    //--------------------------------------------------------------------
    {
        print2DTexture(aTextureID, aFileName.data());
    }


    //------------------------------------------------------------------
    inline void XRayRenderer::printLBuffer(const std::string& aFileName)
    //------------------------------------------------------------------
    {
        printLBuffer(aFileName.data());
    }


    //---------------------------------------------------------------------------
    inline void XRayRenderer::printCumulatedLBuffer(const std::string& aFileName)
    //---------------------------------------------------------------------------
    {
        printCumulatedLBuffer(aFileName.data());
    }


    //-------------------------------------------------------------------
    inline void XRayRenderer::printSumMuxDx(const std::string& aFileName)
    //-------------------------------------------------------------------
    {
        printSumMuxDx(aFileName.data());
    }


    //------------------------------------------------------------------------
    inline void XRayRenderer::printEnergyFluence(const std::string& aFileName)
    //------------------------------------------------------------------------
    {
        printEnergyFluence(aFileName.data());
    }


    //--------------------------------------------------------------------
    inline void XRayRenderer::printXRayImage(const std::string& aFileName)
    //--------------------------------------------------------------------
    {
        printXRayImage(aFileName.data());
    }


    //-----------------------------------------------------------------------
    inline void XRayRenderer::printMuWaterTable(std::ostream& anOutputStream)
    //-----------------------------------------------------------------------
    {
        for (std::vector<AttenuationCoefficient>::const_iterator ite(m_mu_water_set.begin());
                ite != m_mu_water_set.end();
                ++ite)
        {
            anOutputStream << *ite << std::endl;
        }
    }


    //----------------------------------------
    inline void XRayRenderer::initialisePBOs()
    //----------------------------------------
    {
        for (std::vector<unsigned int>::iterator ite(m_p_texture_name_set.begin());
                ite != m_p_texture_name_set.end();
                ++ite)
        {
            // Delete the PBO
            try
            {
                if (*ite != 0)
                {
                    deletePBO(*ite);
                }
            }
            catch (std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }

            // Make sure the value is reset
            *ite = 0;

            // Create a new PBO
            createPBO(*ite);
        }
    }


    //-------------------------------------------------------------------
    inline void XRayRenderer::init2DTextures(int anInternalTextureFormat)
    //-------------------------------------------------------------------
    {
        // Store the texturing flags
        pushTexture2D();

        // Initialise textures
        for (int i(0); i < XRAY_DETECTOR_2D_TEXTURE_NUMBER; ++i)
        {
            create2DTexture(&m_p_texture_name_set[i], anInternalTextureFormat);
        }

        // Restore the texturing flags
        popTexture();
    }


    //-----------------------------------------------------------------------------------------------
    inline RATIONAL_NUMBER XRayRenderer::getMuFromHUAndMuWater(const RATIONAL_NUMBER& HU,
                                                               const RATIONAL_NUMBER& mu_water) const
    //-----------------------------------------------------------------------------------------------
    {
        // Convert a Hounsfield value into a liner attenuation coefficient given mu(water)
        return RATIONAL_NUMBER(mu_water * (1.0 + HU / 1000.0));
    }


    //-------------------------------------------------------------------------------
    inline RATIONAL_NUMBER XRayRenderer::getMuFromHU(const RATIONAL_NUMBER& HU,
                                                     const RATIONAL_NUMBER& anEnergy)
    //-------------------------------------------------------------------------------
    {
        bool mu_values_loaded(false);
    #ifdef gVirtualXRay_XCOM_PATH
        if (std::string(gVirtualXRay_XCOM_PATH) != "XCOM_PATH-NOTFOUND")
        {
            if (!m_element_set.getElementSetSize())
            {
                loadElementSet();
            }

            if (!m_material_set.getMaterialNumber())
            {
                loadMaterialSet();
            }

            mu_values_loaded = true;
        }
    #endif

        if (!mu_values_loaded)
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "The XCOM database is not installed. You can find it on the NIST's website at http://www.nist.gov/pml/data/xcom/index.cfm If you install it, you will need to re-compile the library.");
        }

        return (m_material_set.getMaterial(HU).getMU(HU, anEnergy));
    }


    //--------------------------------------------------------------------------------------------
    inline RATIONAL_NUMBER XRayRenderer::getMassAttenuationFromHU(const RATIONAL_NUMBER& HU,
                                                                  const RATIONAL_NUMBER& anEnergy)
    //--------------------------------------------------------------------------------------------
    {
        bool rho_values_loaded(false);
    #ifdef gVirtualXRay_XCOM_PATH
        if (std::string(gVirtualXRay_XCOM_PATH) != "XCOM_PATH-NOTFOUND")
        {
            if (!m_element_set.getElementSetSize())
            {
                loadElementSet();
            }

            if (!m_material_set.getMaterialNumber())
            {
                loadMaterialSet();
            }

            rho_values_loaded = true;
        }
    #endif

        if (!rho_values_loaded)
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "The XCOM database is not installed. You can find it on the NIST's website at http://www.nist.gov/pml/data/xcom/index.cfm If you install it, you will need to re-compile the library.");
        }

        return (m_material_set.getMaterial(HU).getMassAttenuationTotal(HU, anEnergy));
    }


    //------------------------------------------------------------------------------
    inline RATIONAL_NUMBER XRayRenderer::getDensityFromHU(const RATIONAL_NUMBER& HU)
    //------------------------------------------------------------------------------
    {
        if (!m_element_set.getElementSetSize())
        {
            loadElementSet();
        }

        if (!m_material_set.getMaterialNumber())
        {
            loadMaterialSet();
        }

        return (TissueMaterial::getDensity(HU));
    }


    //----------------------------------------------------------------
    inline unsigned int XRayRenderer::getNumberOfInnerSurfaces() const
    //----------------------------------------------------------------
    {
        return (m_p_inner_surface_set.size());
    }


    //----------------------------------------------------------------
    inline unsigned int XRayRenderer::getNumberOfOuterSurfaces() const
    //----------------------------------------------------------------
    {
        if (m_p_outer_surface)
        {
            return (1);
        }
        else
        {
            return (0);
        }
    }


    //-----------------------------------------------------------------------
    inline void XRayRenderer::displayInnerSurface(unsigned int anIndex) const
    //-----------------------------------------------------------------------
    {
        if (anIndex >= m_p_inner_surface_set.size())
        {
            throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }

        m_p_inner_surface_set[anIndex]->display();
    }


    //---------------------------------------------------
    inline void XRayRenderer::displayOuterSurface() const
    //---------------------------------------------------
    {
        if (!m_p_outer_surface)
        {
            throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }

        m_p_outer_surface->display();
    }


    //-----------------------------------------------------------
    inline RATIONAL_NUMBER XRayRenderer::getMinPixelValue() const
    //-----------------------------------------------------------
    {
        return (m_min_value_in_simulated_xray_image);
    }


    //-----------------------------------------------------------
    inline RATIONAL_NUMBER XRayRenderer::getMaxPixelValue() const
    //-----------------------------------------------------------
    {
        return (m_max_value_in_simulated_xray_image);
    }
    


#ifdef HAS_ITK

    //------------------------------------------------------------------------------
    inline const XRayRenderer::ImageType::Pointer& XRayRenderer::getSinogram() const
    //------------------------------------------------------------------------------
    {
        return (m_p_sinogram_image);
    }


    //------------------------------------------------------------------
    inline XRayRenderer::ImageType::Pointer& XRayRenderer::getSinogram()
    //------------------------------------------------------------------
    {
        return (m_p_sinogram_image);
    }


    //--------------------------------------------------------------
    inline const PixelType* XRayRenderer::getSinogramRawData() const
    //--------------------------------------------------------------
    {
        if (m_p_sinogram_image.IsNull())
        {
            return (0);
        }
        else
        {
            return (m_p_sinogram_image->GetBufferPointer());
        }
    }


    //-----------------------------------------------------------------------------------
    inline const XRayRenderer::ImageType::Pointer& XRayRenderer::getProjectionSet() const
    //-----------------------------------------------------------------------------------
    {
        return (m_p_projection_set);
    }


    //-----------------------------------------------------------------------
    inline XRayRenderer::ImageType::Pointer& XRayRenderer::getProjectionSet()
    //-----------------------------------------------------------------------
    {
        return (m_p_projection_set);
    }


    //-------------------------------------------------------------------
    inline const PixelType* XRayRenderer::getProjectionSetRawData() const
    //-------------------------------------------------------------------
    {
        if (m_p_projection_set.IsNull())
        {
            return (0);
        }
        else
        {
            return (m_p_projection_set->GetBufferPointer());
        }
    }

#else // HAS_ITK

    //---------------------------------------------------------------------
    inline const XRayRenderer::PixelType* XRayRenderer::getSinogram() const
    //---------------------------------------------------------------------
    {
        return (m_p_sinogram_image);
    }


    //---------------------------------------------------------
    inline XRayRenderer::PixelType* XRayRenderer::getSinogram()
    //---------------------------------------------------------
    {
        return (m_p_sinogram_image);
    }


    //----------------------------------------------------------------------------
    inline const XRayRenderer::PixelType* XRayRenderer::getSinogramRawData() const
    //----------------------------------------------------------------------------
    {
        return (m_p_sinogram_image);
    }


    //--------------------------------------------------------------------------
    inline const XRayRenderer::PixelType* XRayRenderer::getProjectionSet() const
    //--------------------------------------------------------------------------
    {
        return (m_p_projection_set);
    }


    //--------------------------------------------------------------
    inline XRayRenderer::PixelType* XRayRenderer::getProjectionSet()
    //--------------------------------------------------------------
    {
        return (m_p_projection_set);
    }


    //---------------------------------------------------------------------------------
    inline const XRayRenderer::PixelType* XRayRenderer::getProjectionSetRawData() const
    //---------------------------------------------------------------------------------
    {
        return (m_p_projection_set);
    }

#endif // HAS_ITK


    //-------------------------------------------------
    inline void XRayRenderer::enableArtefactFiltering()
    //-------------------------------------------------
    {
        m_use_L_buffer_artefact_filtering = true;
    }


    //-------------------------------------------------
    inline void XRayRenderer::enableArtifactFiltering()
    //-------------------------------------------------
    {
        m_use_L_buffer_artefact_filtering = true;
    }


    //--------------------------------------------------
    inline void XRayRenderer::disableArtefactFiltering()
    //--------------------------------------------------
    {
        m_use_L_buffer_artefact_filtering = false;
    }


    //--------------------------------------------------
    inline void XRayRenderer::disableArtifactFiltering()
    //--------------------------------------------------
    {
        m_use_L_buffer_artefact_filtering = false;
    }


    //------------------------------------------------------------
    inline void XRayRenderer::printSinogram(const char* aFileName)
    //------------------------------------------------------------
    {
#ifdef HAS_ITK
        // The filename is not empty
        if (aFileName)
        {
            // Create the file writer
            typedef itk::ImageFileWriter<ImageType> ImageWriter;
            ImageWriter::Pointer p_writer(ImageWriter::New());
            p_writer->SetInput(m_p_sinogram_image);

            // Set the writer
            p_writer->SetFileName(aFileName);

            // Write the data into a file
            p_writer->Update();
        }
#else // HAS_ITK
        if (isMHD(aFileName))
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented, please contact Dr FP Vidal.");
        }
        else if (isMHA(aFileName))
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented, please contact Dr FP Vidal.");
        }
        else
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented, please contact Dr FP Vidal.");
        }
#endif // HAS_ITK
    }


    //-----------------------------------------------------------------
    inline void XRayRenderer::printProjectionSet(const char* aFileName)
    //-----------------------------------------------------------------
    {
#ifdef HAS_ITK
        // The filename is not empty
        if (aFileName)
        {
            // Create the file writer
            typedef itk::ImageFileWriter<ImageType> ImageWriter;
            ImageWriter::Pointer p_writer(ImageWriter::New());
            p_writer->SetInput(m_p_projection_set);

            // Set the writer
            p_writer->SetFileName(aFileName);

            // Write the data into a file
            p_writer->Update();
        }
#else // HAS_ITK
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented, please contact Dr FP Vidal.");
#endif // HAS_ITK
    }


    //--------------------------------------------
    inline void XRayRenderer::resetEnergyFluence()
    //--------------------------------------------
    {
        // The energy fluence is not empty
        if (!m_energy_fluence_empty)
        {
            switch (m_computing_mode)
            {
            case OPENGL:
                reset2DBuffer(XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID);
                break;

            case CUDA:
                throw Exception(__FILE__, __FUNCTION__, __LINE__, "CUDA is not supported.");
                break;

            case OPENCL:
                throw Exception(__FILE__, __FUNCTION__, __LINE__, "OpenCL is not supported.");
                break;

            default:
                throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
                break;
            }
        }

        // The energy fluence is now empty
        m_energy_fluence_empty = true;
    }


    //---------------------------------------
    inline void XRayRenderer::resetSumMuxDx()
    //---------------------------------------
    {
        switch (m_computing_mode)
        {
        case OPENGL:
            reset2DBuffer(XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID);
            break;

        case CUDA:
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "CUDA is not supported.");
            break;

        case OPENCL:
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "OpenCL is not supported.");
            break;

        default:
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
            break;
        }
    }


    //-------------------------------------------------------------------------------------
    inline void XRayRenderer::computeLBuffer(PolygonMesh& anObject,
                                             const VEC3& aXRaySourceDetectorTranslation,
                                             const MATRIX4& aModellingTransformationMatrix)
    //-------------------------------------------------------------------------------------
    {
        // This is an internal mesh
        if (&anObject != m_p_outer_surface || !m_p_inner_surface_set.size())
        {
            computeInnerSurfaceLBuffer(anObject, aXRaySourceDetectorTranslation, aModellingTransformationMatrix);
        }
        // This is the external mesh
        else
        {
            computeOuterSurfaceLBuffer(anObject, aXRaySourceDetectorTranslation, aModellingTransformationMatrix);
        }
    }


    //-----------------------------------------------------------
    inline void XRayRenderer::printLBuffer(const char* aFileName)
    //-----------------------------------------------------------
    {
        if (m_use_L_buffer_artefact_filtering)
        {
            //print2DTexture(XRAY_DETECTOR_CLEANED_LBUFFER_TEXTURE_NAME_ID, aFileName);
            printFBO(XRAY_DETECTOR_CLEANED_LBUFFER_FBO_ID, aFileName);
        }
        else
        {
            //print2DTexture(XRAY_DETECTOR_UNCLEANED_LBUFFER_TEXTURE_NAME_ID, aFileName);
            printFBO(XRAY_DETECTOR_UNCLEANED_LBUFFER_FBO_ID, aFileName);
        }
    }


    //--------------------------------------------------------------------
    inline void XRayRenderer::printCumulatedLBuffer(const char* aFileName)
    //--------------------------------------------------------------------
    {
        //print2DTexture(XRAY_DETECTOR_LBUFFER_TEXTURE_NAME_ID, aFileName);
        printFBO(XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID, aFileName);
    }


    //------------------------------------------------------------
    inline void XRayRenderer::printSumMuxDx(const char* aFileName)
    //------------------------------------------------------------
    {
        //print2DTexture(XRAY_DETECTOR_SUM_MUx_Dx_TEXTURE_NAME_ID, aFileName);
        printFBO(XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID, aFileName);
    }


    //-----------------------------------------------------------------
    inline void XRayRenderer::printEnergyFluence(const char* aFileName)
    //-----------------------------------------------------------------
    {
        //print2DTexture(XRAY_DETECTOR_ENERGY_FLUENCE_TEXTURE_NAME_ID, aFileName);
        printFBO(XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID, aFileName);
    }


    //-------------------------------------------------------------
    inline void XRayRenderer::printXRayImage(const char* aFileName)
    //-------------------------------------------------------------
    {
        //print2DTexture(XRAY_DETECTOR_ENERGY_FLUENCE_TEXTURE_NAME_ID, aFileName);
        printFBO(XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID, aFileName);
    }


    //------------------------------------------------------
    inline RATIONAL_NUMBER* XRayRenderer::getLBuffer() const
    //------------------------------------------------------
    {
        if (m_use_L_buffer_artefact_filtering)
        {
            //return (get2DTexture(XRAY_DETECTOR_CLEANED_LBUFFER_TEXTURE_NAME_ID));
            return (getFBO(XRAY_DETECTOR_CLEANED_LBUFFER_FBO_ID));
        }
        else
        {
            //return (get2DTexture(XRAY_DETECTOR_UNCLEANED_LBUFFER_TEXTURE_NAME_ID));
            return (getFBO(XRAY_DETECTOR_UNCLEANED_LBUFFER_FBO_ID));
        }
    }


    //---------------------------------------------------------------
    inline RATIONAL_NUMBER* XRayRenderer::getCumulatedLBuffer() const
    //---------------------------------------------------------------
    {
        //return (get2DTexture(XRAY_DETECTOR_LBUFFER_TEXTURE_NAME_ID));
        return (getFBO(XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID));
    }


    //-------------------------------------------------------
    inline RATIONAL_NUMBER* XRayRenderer::getSumMuxDx() const
    //-------------------------------------------------------
    {
        //return (get2DTexture(XRAY_DETECTOR_SUM_MUx_Dx_TEXTURE_NAME_ID));
        return (getFBO(XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID));
    }


    //------------------------------------------------------------
    inline RATIONAL_NUMBER* XRayRenderer::getEnergyFluence() const
    //------------------------------------------------------------
    {
        //return (get2DTexture(XRAY_DETECTOR_ENERGY_FLUENCE_TEXTURE_NAME_ID));
        return (getFBO(XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID));
    }


    //--------------------------------------------------------
    inline RATIONAL_NUMBER* XRayRenderer::getXRayImage() const
    //--------------------------------------------------------
    {
        //return (get2DTexture(XRAY_DETECTOR_ENERGY_FLUENCE_TEXTURE_NAME_ID));
        return (getFBO(XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID));
    }


    //----------------------------------------
    inline void XRayRenderer::displayLBuffer()
    //----------------------------------------
    {
        if (m_use_L_buffer_artefact_filtering)
        {
            display2DTexture(XRAY_DETECTOR_CLEANED_LBUFFER_TEXTURE_NAME_ID);
        }
        else
        {
            display2DTexture(XRAY_DETECTOR_UNCLEANED_LBUFFER_TEXTURE_NAME_ID);
        }
    }


    //----------------------------------------------
    inline void XRayRenderer::displayEnergyFluence()
    //----------------------------------------------
    {
        display2DTexture(XRAY_DETECTOR_ENERGY_FLUENCE_TEXTURE_NAME_ID);
    }


    //------------------------------------------
    inline void XRayRenderer::displayXRayImage()
    //------------------------------------------
    {
        display2DTexture(XRAY_DETECTOR_ENERGY_FLUENCE_TEXTURE_NAME_ID);
    }


} // namespace gVirtualXRay
