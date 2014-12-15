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
*	@section	License
*				BSD 3-Clause License.
*
*				For details on use and redistribution please refer
*				to http://opensource.org/licenses/BSD-3-Clause
*
*	@section	Copyright
*				(c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*				http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
*				BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <sstream>

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


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
inline RATIONAL_NUMBER* XRayRenderer::getSinogramRawData() const
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
#endif




} // namespace gVirtualXRay
