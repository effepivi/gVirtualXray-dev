//******************************************************************************
//  Include
//******************************************************************************
#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef __Logger_h
#include <gVirtualXRay/Logger.h>
#endif

#include "gVirtualXRay/framebufferObject.h"

#ifndef __Context_h
#include "gVirtualXRay/Context.h"
#endif

#ifndef __ResourceManager_h
#include "gVirtualXRay/ResourceManager.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


ResourceManager::ResourceManager():
    m_p_context(0)
{}


void ResourceManager::releaseResources()
{
    // Release shaders
    for (std::vector<Shader*>::iterator ite = m_p_shader_set.begin();
        ite != m_p_shader_set.end();
        ++ite)
    {
        if (*ite)
        {
            (*ite)->release();
        }
    }
    m_p_shader_set.clear();

    // Release FBOs
    for (std::vector<FramebufferObject*>::iterator ite = m_p_FBO_set.begin();
        ite != m_p_FBO_set.end();
        ++ite)
    {
        if (*ite)
        {
            (*ite)->UnattachAll();
        }
    }
    m_p_FBO_set.clear();

    // Release textures


    // Release VBOs
    for (std::vector<OpenGL2VBO*>::iterator ite = m_p_gl2_vbo.begin();
        ite != m_p_gl2_vbo.end();
        ++ite)
    {
        if (*ite)
        {
            (*ite)->destroy();
        }
    }
    m_p_gl2_vbo.clear();

    for (std::vector<OpenGL3VBO*>::iterator ite = m_p_gl3_vbo.begin();
        ite != m_p_gl3_vbo.end();
        ++ite)
    {
        if (*ite)
        {
            (*ite)->destroy();
        }
    }
    m_p_gl3_vbo.clear();
}


void ResourceManager::addShader(Shader* apShader)
{
    m_p_shader_set.push_back(apShader);
}


void ResourceManager::addFBO(FramebufferObject* apFBO)
{
    m_p_FBO_set.push_back(apFBO);
}


void ResourceManager::addVBO(OpenGL2VBO* apVBO)
{
    m_p_gl2_vbo.push_back(apVBO);
}


void ResourceManager::addVBO(OpenGL3VBO* apVBO)
{
    m_p_gl3_vbo.push_back(apVBO);
}


void ResourceManager::destroyAndRemoveShader(Shader* apShader)
{
    for (std::vector<Shader*>::iterator ite = m_p_shader_set.begin();
        ite != m_p_shader_set.end();
        ++ite)
    {
        if (apShader && *ite == apShader)
        {
            (*ite)->release();
            *ite = 0;
        }
    }
}


void ResourceManager::destroyAndRemoveFBO(FramebufferObject* apFBO)
{
    for (std::vector<FramebufferObject*>::iterator ite = m_p_FBO_set.begin();
        ite != m_p_FBO_set.end();
        ++ite)
    {
        if (apFBO && *ite == apFBO)
        {
            (*ite)->UnattachAll();
            *ite = 0;
        }
    }
}


void ResourceManager::destroyAndRemoveVBO(OpenGL2VBO* apVBO)
{
    for (std::vector<OpenGL2VBO*>::iterator ite = m_p_gl2_vbo.begin();
        ite != m_p_gl2_vbo.end();
        ++ite)
    {
        if (apVBO && *ite == apVBO)
        {
            (*ite)->destroy();
            *ite = 0;
        }
    }
}


void ResourceManager::destroyAndRemoveVBO(OpenGL3VBO* apVBO)
{
    for (std::vector<OpenGL3VBO*>::iterator ite = m_p_gl3_vbo.begin();
        ite != m_p_gl3_vbo.end();
        ++ite)
    {
        if (apVBO && *ite == apVBO)
        {
            (*ite)->destroy();
            *ite = 0;
        }
    }
}


void ResourceManager::setGraphicsContext(Context* apContext)
{
    m_p_context = apContext;
}


Context* ResourceManager::getGraphicsContext()
{
	return m_p_context;
}


void ResourceManager::destroyAndRemoveContext()
{
    releaseResources();

    if (m_p_context)
    {
    	m_p_context->release();
    }
    m_p_context = 0;
}
