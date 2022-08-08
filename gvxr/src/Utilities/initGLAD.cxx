// GLAD
#include <glad/glad.h>

#ifdef HAS_EGL
#include <glad/glad_egl.h>
#endif

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
    }
    else if (anAPI == "EGL")
    {
        if (!gladLoadEGL())
        {
            LOGGER.logError("Cannot initialise EGL using GLAD") << std::endl;
            // throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot initialise EGL using GLAD");
        }
    }
    else
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot initialise an unknown API using GLAD");
    }
}


};
