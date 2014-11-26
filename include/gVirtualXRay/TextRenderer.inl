/**
********************************************************************************
*
*	@file		TextRenderer.inl
*
*	@brief		Class to render text using OpenGL and FreeType2.
*
*	@version	1.0
*
*	@date		16/11/2014
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
#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif


//******************************************************************************
//	define
//******************************************************************************


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


    //----------------------------------------------------------
    inline TextRenderer::Character::Character(GLuint aTextureID,
                                              int aWidth,
                                              int aHeight,
                                              float aVX,
                                              float aVY,
                                              float aXOffset,
                                              float aYOffset):
            //----------------------------------------------------------
            m_texture_id(aTextureID),
            m_width(aWidth),
            m_height(aHeight),
            m_vx(aVX),
            m_vy(aVY),
            m_x_offset(aXOffset),
            m_y_offset(aYOffset)
    //----------------------------------------------------------
    {
    }


    //------------------------------------------
    inline TextRenderer::Character::~Character()
    //------------------------------------------
    {
        if (m_texture_id)
        {
            glDeleteTextures(1, &m_texture_id);
        }
    }


    //----------------------------------
    inline TextRenderer::TextRenderer():
    //----------------------------------
            m_initialised(false)
    //----------------------------------
    {
    }


    //----------------------------------
    inline TextRenderer::~TextRenderer()
    //----------------------------------
    {
#ifdef HAS_FREETYPE
        m_character_set.clear();
#endif
    }


    //----------------------------------------------------------
    inline void TextRenderer::initialize(unsigned int aFontSize)
    //----------------------------------------------------------
    {
        initialise(aFontSize);
    }


    //---------------------------------------------------------------
    inline void TextRenderer::initialize(const std::string aFontFile,
                                         unsigned int aFontSize)
    //---------------------------------------------------------------
    {
        initialise(aFontFile, aFontSize);
    }


    //--------------------------------------------
    inline GLint TextRenderer::getShaderID() const
    //--------------------------------------------
    {
        return (m_display_text_shader.getProgramHandle());
    }


} // namespace gVirtualXRay
