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
*				http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
*				BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
//#include <fstream>
#include <sstream>

#ifndef OPENGL_UTILITIES_H
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


//----------------------------------------------------------------------
inline void XRayRenderer::addInnerSurface(const TriangleMesh* apSurface)
//----------------------------------------------------------------------
{
	m_p_inner_surface_set.push_back(const_cast<TriangleMesh*>(apSurface));
}


//----------------------------------------------------------------------
inline void XRayRenderer::addOuterSurface(const TriangleMesh* apSurface)
//----------------------------------------------------------------------
{
	m_p_outer_surface = const_cast<TriangleMesh*>(apSurface);
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


//------------------------------------
inline void XRayRenderer::deleteFBOs()
//------------------------------------
{
	// Delete the FBOs
	for (std::vector<FramebufferObject*>::iterator ite(m_p_fbo_set.begin());
			ite != m_p_fbo_set.end();
			++ite)
	{
		if (*ite)
		{
			delete *ite;
			*ite = 0;
		}
	}
	
	m_p_fbo_set.clear();
}


//----------------------------------------
inline void XRayRenderer::initialisePBOs()
//----------------------------------------
{
	for (std::vector<GLuint>::iterator ite(m_p_texture_name_set.begin());
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


//---------------------------------------------------------------------------------------------------
inline RATIONAL_NUMBER XRayRenderer::convertHounsfieldValue2Mu(const RATIONAL_NUMBER& HU,
															   const RATIONAL_NUMBER& mu_water) const
//---------------------------------------------------------------------------------------------------
{
	return RATIONAL_NUMBER(mu_water * (1.0 + HU / 1000.0));
}


inline int XRayRenderer::getNumberOfInnerSurfaces() const
{
	return (m_p_inner_surface_set.size());
}


inline int XRayRenderer::getNumberOfOuterSurfaces() const
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


//--------------------------------------------------------------
inline void XRayRenderer::displayInnerSurface(int anIndex) const
//--------------------------------------------------------------
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


} // namespace gVirtualXRay
