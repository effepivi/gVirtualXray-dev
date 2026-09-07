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
//	namespace
//******************************************************************************
namespace gVirtualXRay {


    //----------------------------------------------------------------
    inline TextRenderer::Character::Character(unsigned int aTextureID,
                                              int aWidth,
                                              int aHeight,
                                              float aVX,
                                              float aVY,
                                              float aXOffset,
                                              float aYOffset):
    //----------------------------------------------------------------
            m_texture_id(aTextureID),
            m_width(aWidth),
            m_height(aHeight),
            m_vx(aVX),
            m_vy(aVY),
            m_x_offset(aXOffset),
            m_y_offset(aYOffset)
    //----------------------------------------------------------------
    {
    }


    //------------------------------------------
    inline TextRenderer::Character::~Character()
    //------------------------------------------
    {
        release();
    }


    //--------------------------------------------
    inline void TextRenderer::Character::release()
    //--------------------------------------------
    {
        if (m_texture_id)
        {
            glDeleteTextures(1, &m_texture_id);
        }
    }


    //----------------------------------
    inline TextRenderer::TextRenderer():
    //----------------------------------
            m_font_size(16.0),
            m_oversampling(1.0),
            m_p_font_file_data(nullptr),
            m_initialised(false)
    //----------------------------------
    {
    }


    //----------------------------------
    inline TextRenderer::~TextRenderer()
    //----------------------------------
    {
        release();
    }


    //---------------------------------
    inline void TextRenderer::release()
    //---------------------------------
    {
#ifdef HAS_FREETYPE
        m_character_set.clear();
        m_text_vbo.reset();
        m_display_text_shader.release();
        m_initialised = false;

        FT_Done_Face(m_face);
        FT_Done_FreeType(m_library);

        if (m_p_font_file_data) {
            delete [] m_p_font_file_data;
            m_p_font_file_data = nullptr;
        }
#endif
    }


    //--------------------------------------------------------------
    inline void TextRenderer::initialize(unsigned int aFontSize,
                                         float anOversamplingFactor)
    //--------------------------------------------------------------
    {
        initialise(aFontSize, anOversamplingFactor);
    }


    //---------------------------------------------------------------
    inline void TextRenderer::initialize(const std::string aFontFile,
                                         unsigned int aFontSize,
                                         float anOversamplingFactor)
    //---------------------------------------------------------------
    {
        initialise(aFontFile, aFontSize, anOversamplingFactor);
    }


    //---------------------------------------------
    inline bool TextRenderer::isInitialised() const
    //---------------------------------------------
    {
        return (m_initialised);
    }


    //--------------------------------------------
    inline float TextRenderer::getFontSize() const
    //--------------------------------------------
    {
        return m_font_size;
    }


} // namespace gVirtualXRay
