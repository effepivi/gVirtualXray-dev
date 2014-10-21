/**
********************************************************************************
*
*	@file		Material.inl
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

//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//-------------------------
inline Material::Material():
        m_shininess(50.0)
//-------------------------
{
    /// Ambient colour
    m_p_ambient_colour[0] = 1.0;
    m_p_ambient_colour[1] = 1.0;
    m_p_ambient_colour[2] = 1.0;
    m_p_ambient_colour[3] = 1.0;

    /// Diffuse colour
    m_p_diffuse_colour[0] = 1.0;
    m_p_diffuse_colour[1] = 1.0;
    m_p_diffuse_colour[2] = 1.0;
    m_p_diffuse_colour[3] = 1.0;

    /// Specular colour
    m_p_specular_colour[0] = 1.0;
    m_p_specular_colour[1] = 1.0;
    m_p_specular_colour[2] = 1.0;
    m_p_specular_colour[3] = 1.0;
}


//--------------------------
inline Material::~Material()
//--------------------------
{
}


//---------------------------------------------------------
inline void Material::setAmbientColour(float aRedChannel,
                                       float aGreenChannel,
                                       float aBlueChannel,
                                       float aAlphaChannel)
//---------------------------------------------------------
{
    m_p_ambient_colour[0] = aRedChannel;
    m_p_ambient_colour[1] = aGreenChannel;
    m_p_ambient_colour[2] = aBlueChannel;
    m_p_ambient_colour[3] = aAlphaChannel;
}


//---------------------------------------------------------
inline void Material::setDiffuseColour(float aRedChannel,
                                       float aGreenChannel,
                                       float aBlueChannel,
                                       float aAlphaChannel)
//---------------------------------------------------------
{
    m_p_diffuse_colour[0] = aRedChannel;
    m_p_diffuse_colour[1] = aGreenChannel;
    m_p_diffuse_colour[2] = aBlueChannel;
    m_p_diffuse_colour[3] = aAlphaChannel;
}



//----------------------------------------------------------
inline void Material::setSpecularColour(float aRedChannel,
                                        float aGreenChannel,
                                        float aBlueChannel,
                                        float aAlphaChannel)
//----------------------------------------------------------
{
    m_p_specular_colour[0] = aRedChannel;
    m_p_specular_colour[1] = aGreenChannel;
    m_p_specular_colour[2] = aBlueChannel;
    m_p_specular_colour[3] = aAlphaChannel;
}


//-------------------------------------------------------
inline void Material::setShininess(float aShininessValue)
//-------------------------------------------------------
{
    m_shininess = aShininessValue;
}

//----------------------------------------------------
inline const float* Material::getAmbientColour() const
//----------------------------------------------------
{
    return (m_p_ambient_colour);
}


//----------------------------------------------------
inline const float* Material::getDiffuseColour() const
//----------------------------------------------------
{
    return (m_p_diffuse_colour);
}


//-----------------------------------------------------
inline const float* Material::getSpecularColour() const
//-----------------------------------------------------
{
    return (m_p_specular_colour);
}


//-----------------------------------------
inline float Material::getShininess() const
//-----------------------------------------
{
    return (m_shininess);
}


} // namespace gVirtualXRay
