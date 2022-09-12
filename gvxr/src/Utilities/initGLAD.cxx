// GLAD
#ifdef HAS_GLAD
#include <glad/glad.h>

#ifdef HAS_EGL
#include <glad/glad_egl.h>
#endif // HAS_EGL
#endif // HAS_GLAD

// GLFW
#include <GLFW/glfw3.h>

#include <sstream>

#ifndef __Logger_h
#include <gVirtualXRay/Logger.h>
#endif

#ifndef __Exception_h
#include "gVirtualXRay/Exception.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//---------------------------------------------------------------
void initialiseGLAD(const std::string& anAPI)
//---------------------------------------------------------------
{
    LOGGER.logNow("Initialise ") << anAPI << " using GLAD." << std::endl;

#ifdef HAS_GLAD
    if (anAPI == "GL" || anAPI == "OPENGL")
    {
        /*if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot initialise OpenGL using GLAD");
        }*/


        if(!gladLoadGL())
        {
            LOGGER.logError("Cannot initialise OpenGL using GLAD") << std::endl;
            // throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot initialise OpenGL using GLAD");
        }
        else
        {
            LOGGER.logNow("OpenGL Version ") << GLVersion.major << "." << GLVersion.minor << " loaded." << std::endl;
        }

        glGetError(); // Remove an error that occurs in Windows

        if (GLAD_GL_VERSION_3_0) LOGGER.logNow("Glad supports OpenGL 3.0") << std::endl;
        if (GLAD_GL_VERSION_3_1) LOGGER.logNow("Glad supports OpenGL 3.1") << std::endl;
        if (GLAD_GL_VERSION_3_2) LOGGER.logNow("Glad supports OpenGL 3.2") << std::endl;
        if (GLAD_GL_VERSION_3_3) LOGGER.logNow("Glad supports OpenGL 3.3") << std::endl;
        if (GLAD_GL_VERSION_4_0) LOGGER.logNow("Glad supports OpenGL 4.0") << std::endl;
        if (GLAD_GL_VERSION_4_1) LOGGER.logNow("Glad supports OpenGL 4.1") << std::endl;
        if (GLAD_GL_VERSION_4_2) LOGGER.logNow("Glad supports OpenGL 4.2") << std::endl;
        if (GLAD_GL_VERSION_4_3) LOGGER.logNow("Glad supports OpenGL 4.3") << std::endl;
        if (GLAD_GL_VERSION_4_4) LOGGER.logNow("Glad supports OpenGL 4.4") << std::endl;
        if (GLAD_GL_VERSION_4_5) LOGGER.logNow("Glad supports OpenGL 4.5") << std::endl;
        if (GLAD_GL_VERSION_4_6) LOGGER.logNow("Glad supports OpenGL 4.6") << std::endl;
    }
    else if (anAPI == "EGL")
    {
#ifdef HAS_EGL
        if (!gladLoadEGL())
        {
            LOGGER.logError("Cannot initialise EGL using GLAD") << std::endl;
            // throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot initialise EGL using GLAD");
        }
#else // HAS_EGL
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot initialise EGL as it is not supported on your system.");
#endif // HAS_EGL
    }
    else
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot initialise an unknown API using GLAD");
    }
#else //HAS_GLAD
    LOGGER.logWarning("GLAD was not initialised as it has been turned off.") << std::endl;
#endif // HAS_GLAD
}


};
