/*

Copyright (c) 2022, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*   @file       TestOpenGLContext
*
*   @brief      Unit testing: Check if the computer supports various verions of OpenGL.
*
*   @version    1.0
*
*   @date       25/09/2022
*   @author     Dr Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Sep 2022, 2022, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <iostream>

#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include "gtest/gtest.h"

#ifndef __Logger_h
#include <gVirtualXRay/Logger.h>
#endif

#ifndef __Context_h
#include "gVirtualXRay/Context.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;
using namespace std;


//--------------------------------
TEST(TestOpenGL2_1, TestOpenGL2_1)
//--------------------------------
{
    Context GL_context;

    // Create an OpenGL context
#ifdef HAS_EGL
    GL_context.create("EGL", 2, 1);
#else
    GL_context.create("OpenGL", 2, 1);
#endif

    LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

    GL_context.release();
}


#ifndef __APPLE__
//--------------------------------
TEST(TestOpenGL3_0, TestOpenGL3_0)
//--------------------------------
{
    Context GL_context;

    // Create an OpenGL context
#ifdef HAS_EGL
    GL_context.create("EGL", 3, 0);
#else
    GL_context.create("OpenGL", 3, 0);
#endif

    LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

    GL_context.release();
}


//--------------------------------
TEST(TestOpenGL3_1, TestOpenGL3_1)
//--------------------------------
{
    Context GL_context;

    // Create an OpenGL context
#ifdef HAS_EGL
    GL_context.create("EGL", 3, 1);
#else
    GL_context.create("OpenGL", 3, 1);
#endif

    LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

    GL_context.release();
}
#endif

//--------------------------------
TEST(TestOpenGL3_2, TestOpenGL3_2)
//--------------------------------
{
    Context GL_context;

    // Create an OpenGL context
#ifdef HAS_EGL
    GL_context.create("EGL", 3, 2);
#else
    GL_context.create("OpenGL", 3, 2);
#endif

    LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

    GL_context.release();
}


//--------------------------------
TEST(TestOpenGL3_3, TestOpenGL3_3)
//--------------------------------
{
    Context GL_context;

    // Create an OpenGL context
#ifdef HAS_EGL
    GL_context.create("EGL", 3, 3);
#else
    GL_context.create("OpenGL", 3, 3);
#endif

    LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

    GL_context.release();
}


//--------------------------------
TEST(TestOpenGL4_0, TestOpenGL4_0)
//--------------------------------
{
    Context GL_context;

    // Create an OpenGL context
#ifdef HAS_EGL
    GL_context.create("EGL", 4, 0);
#else
    GL_context.create("OpenGL", 4, 0);
#endif

    LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

    GL_context.release();
}


//--------------------------------
TEST(TestOpenGL4_1, TestOpenGL4_1)
//--------------------------------
{
    Context GL_context;

    // Create an OpenGL context
#ifdef HAS_EGL
    GL_context.create("EGL", 4, 1);
#else
    GL_context.create("OpenGL", 4, 1);
#endif

    LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

    GL_context.release();
}


#ifndef __APPLE__
//--------------------------------
TEST(TestOpenGL4_2, TestOpenGL4_2)
//--------------------------------
{
    Context GL_context;

    // Create an OpenGL context
#ifdef HAS_EGL
    GL_context.create("EGL", 4, 2);
#else
    GL_context.create("OpenGL", 4, 2);
#endif

    LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

    GL_context.release();
}


//--------------------------------
TEST(TestOpenGL4_3, TestOpenGL4_3)
//--------------------------------
{
    Context GL_context;

    // Create an OpenGL context
#ifdef HAS_EGL
    GL_context.create("EGL", 4, 3);
#else
    GL_context.create("OpenGL", 4, 3);
#endif

    LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

    GL_context.release();
}


//--------------------------------
TEST(TestOpenGL4_4, TestOpenGL4_4)
//--------------------------------
{
    Context GL_context;

    // Create an OpenGL context
#ifdef HAS_EGL
    GL_context.create("EGL", 4, 4);
#else
    GL_context.create("OpenGL", 4, 4);
#endif

    LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

    GL_context.release();
}


//--------------------------------
TEST(TestOpenGL4_5, TestOpenGL4_5)
//--------------------------------
{
    Context GL_context;

    // Create an OpenGL context
#ifdef HAS_EGL
    GL_context.create("EGL", 4, 5);
#else
    GL_context.create("OpenGL", 4, 5);
#endif

    LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

    GL_context.release();
}


//--------------------------------
TEST(TestOpenGL4_6, TestOpenGL4_6)
//--------------------------------
{
    Context GL_context;

    // Create an OpenGL context
#ifdef HAS_EGL
    GL_context.create("EGL", 4, 6);
#else
    GL_context.create("OpenGL", 4, 6);
#endif

    LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

    GL_context.release();
}
#endif
