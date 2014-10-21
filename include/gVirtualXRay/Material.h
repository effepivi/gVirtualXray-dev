/**
********************************************************************************
*
*	@file		Material.h
*
*	@brief		Class to handle a material.
*
*	@version	1.2
*
*	@date		21/10/2014
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

#ifndef GVXR_MATERIAL_H
#define GVXR_MATERIAL_H


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*	@class	Material
*	@brief	Material is a class to handle materials.
*/
//==============================================================================
class Material
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	//--------------------------------------------------------------------------
	///	Default constructor.
	//--------------------------------------------------------------------------
    Material();


	//--------------------------------------------------------------------------
	///	Destructor.
	//--------------------------------------------------------------------------
	~Material();


    //--------------------------------------------------------------------------
    /// Set the ambient colour.
    /**
    *   @param aRedChannel      red channel of the colour
    *   @param aGreenChannel    green channel of the colour
    *   @param aBlueChannel     blue channel of the colour
    *   @param aAlphaChannel    alpha channel of the colour
    */
    //--------------------------------------------------------------------------
    void setAmbientColour(float aRedChannel,
            float aGreenChannel,
            float aBlueChannel,
            float aAlphaChannel);


    //--------------------------------------------------------------------------
    /// Set the diffuse colour.
    /**
    *   @param aRedChannel      red channel of the colour
    *   @param aGreenChannel    green channel of the colour
    *   @param aBlueChannel     blue channel of the colour
    *   @param aAlphaChannel    alpha channel of the colour
    */
    //--------------------------------------------------------------------------
    void setDiffuseColour(float aRedChannel,
            float aGreenChannel,
            float aBlueChannel,
            float aAlphaChannel);


    //--------------------------------------------------------------------------
    /// Set the specular colour.
    /**
    *   @param aRedChannel      red channel of the colour
    *   @param aGreenChannel    green channel of the colour
    *   @param aBlueChannel     blue channel of the colour
    *   @param aAlphaChannel    alpha channel of the colour
    */
    //--------------------------------------------------------------------------
    void setSpecularColour(float aRedChannel,
            float aGreenChannel,
            float aBlueChannel,
            float aAlphaChannel);


    //--------------------------------------------------------------------------
    /// Set the shininess of the material.
    /**
    *   @param aShininessValue  amount of shininess
    */
    //--------------------------------------------------------------------------
    void setShininess(float aShininessValue);


    //--------------------------------------------------------------------------
    /// Accessor on the ambient colour.
    /**
    *   @return the ambient colour
    */
    //--------------------------------------------------------------------------
    const float* getAmbientColour() const;


    //--------------------------------------------------------------------------
    /// Accessor on the diffuse colour.
    /**
    *   @return the diffuse colour
    */
    //--------------------------------------------------------------------------
    const float* getDiffuseColour() const;


    //--------------------------------------------------------------------------
    /// Accessor on the specular colour.
    /**
    *   @return the specular colour
    */
    //--------------------------------------------------------------------------
    const float* getSpecularColour() const;


    //--------------------------------------------------------------------------
    /// Accessor on the shininess of the material.
    /**
    *   @return the amount of shininess
    */
    //--------------------------------------------------------------------------
    float getShininess() const;


//******************************************************************************
protected:
    /// Ambient colour
    float m_p_ambient_colour[4];


    /// Diffuse colour
    float m_p_diffuse_colour[4];


    /// Specular colour
    float m_p_specular_colour[4];


    /// Shininess
    float m_shininess;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "Material.inl"


#endif // GVXR_MATERIAL_H

