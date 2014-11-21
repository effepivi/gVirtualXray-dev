/**
********************************************************************************
*
*	@file		TextRenderer.h
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


#ifndef GVXR_TEXT_RENDERER_H
#define GVXR_TEXT_RENDERER_H


//******************************************************************************
//  Include
//******************************************************************************
#include <map>
#include <boost/scoped_ptr.hpp>

#if defined(_WIN32) || defined(_WIN64)
#include <GL/glew.h>
#else
#ifdef __APPLE__
#define GL3_PROTOTYPES 1
#include <OpenGL/gl3.h>
#else
#define GL_GLEXT_PROTOTYPES 1
#include <GL/glcorearb.h>
#endif
#endif

#ifdef HAS_FREETYPE
#include <ft2build.h>
#include FT_FREETYPE_H
#endif

#ifdef GVXR_VBO_H
#include "VBO.h"
#endif

#ifndef GVXR_SHADER_H
#include "gVirtualXRay/Shader.h"
#endif

#ifdef GVXR_VEC3_H
#include "Vec3.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*	@class	TextRenderer
*	@brief	TextRenderer is a class to render text using OpenGL and FreeType2.
*/
//==============================================================================
class TextRenderer
//------------------------------------------------------------------------------
{
//******************************************************************************
private:


    //==========================================================================
    /**
    *   @class  Character
    *   @brief  Character is a class to handle characters rendered using OpenGL
    *           and FreeType2.
    */
    //==========================================================================
    class Character
    {
    //**************************************************************************
    public:
        Character(GLuint aTextureID = 0,
                int aWidth = 0,
                int aHeight = 0,
                float aVX = 0,
                float aVY = 0,
                float aXOffset = 0,
                float aYOffset = 0);

        virtual ~Character();

        GLuint m_texture_id;
        int m_width;
        int m_height;
        float m_vx;
        float m_vy;
        float m_x_offset;
        float m_y_offset;
    };


//******************************************************************************
public:


    //--------------------------------------------------------------------------
    /// Default constructor.
    //--------------------------------------------------------------------------
    inline TextRenderer();


    //--------------------------------------------------------------------------
    /// Destructor.
    //--------------------------------------------------------------------------
    virtual ~TextRenderer();


    void initialise(const VEC3& aBackgroundColour = VEC3(0, 0, 0),
            const VEC3& aFontColour = VEC3(1, 1, 1));

    void initialize(const VEC3& aBackgroundColour = VEC3(0, 0, 0),
            const VEC3& aFontColour = VEC3(1, 1, 1));

    void initialise(const std::string aFontFile,
            const VEC3& aBackgroundColour = VEC3(0, 0, 0),
            const VEC3& aFontColour = VEC3(1, 1, 1));


    void initialize(const std::string aFontFile,
            const VEC3& aBackgroundColour = VEC3(0, 0, 0),
            const VEC3& aFontColour = VEC3(1, 1, 1));


    void renderText(const std::string& aText, float x, float y, Matrix4x4<GLfloat>& aProjectionMatrix);


    GLint getShaderID() const;


//******************************************************************************
protected:

 void initialiseInternalStates();


#ifdef HAS_FREETYPE
    /// The FreeType library main object
    FT_Library m_free_type;


    /// The font face
    FT_Face m_face;


    /// The cache of characters
    std::map<char, Character> m_character_set;


    /// The VBO used to display the characters
    boost::scoped_ptr<VBO> m_text_vbo;


    Shader m_display_text_shader;
#endif

    bool m_initialised;


    VEC3 m_background_colour;
    VEC3 m_font_colour;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "TextRenderer.inl"


#endif // GVXR_TEXT_RENDERER_H
