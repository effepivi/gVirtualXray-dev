#include <iostream>

#ifndef __Logger_h
#include "gVirtualXRay/Logger.h"
#endif

#ifndef __OpenGLUtilities_h
#include "gVirtualXRay/OpenGLUtilities.h"
#endif


void initGL(gVirtualXRay::Context& aGLContext)
{
        // Create an OpenGL context
    gVirtualXRay::LOGGER.logNow("Try creating an OpenGL 3.2 context using GLFW") << std::endl;
    aGLContext.create("OpenGL", 3, 2);

    if (aGLContext.isReady())
    {
        gVirtualXRay::LOGGER.logNow("\tsuccess") << std::endl;
    }
    else
    {
        gVirtualXRay::LOGGER.logWarning("\tFailure") << std::endl;
    }

#ifdef HAS_GLX
    gVirtualXRay::LOGGER.logNow("GLX is supported by gVXR") << std::endl;
    if (!aGLContext.isReady())
    {
        gVirtualXRay::LOGGER.logNow("Try creating an OpenGL 3.2 context using GLX") << std::endl;
        aGLContext.create("GLX", 3, 2);
    }
#endif

    if (aGLContext.isReady())
    {
        gVirtualXRay::LOGGER.logNow("\tsuccess") << std::endl;
    }
    else
    {
        gVirtualXRay::LOGGER.logWarning("\tFailure") << std::endl;
    }

#ifdef HAS_EGL
    gVirtualXRay::LOGGER.logNow("EGL is supported by gVXR") << std::endl;
    if (!aGLContext.isReady())
    {
        gVirtualXRay::LOGGER.logNow("Try creating an OpenGL 3.2 context using EGL") << std::endl;
        aGLContext.create("EGL", 3, 2);
    }
#endif

    if (aGLContext.isReady())
    {
        gVirtualXRay::LOGGER.logNow("\tsuccess") << std::endl;
    }
    else
    {
        gVirtualXRay::LOGGER.logWarning("\tFailure") << std::endl;
    }

    gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Enable the Z-buffer
    glEnable(GL_DEPTH_TEST);
    gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Set the background colour
    glClearColor(0.5, 0.5, 0.5, 1.0);
    gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glEnable(GL_MULTISAMPLE);
    gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Check the current FBO
    gVirtualXRay::checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}