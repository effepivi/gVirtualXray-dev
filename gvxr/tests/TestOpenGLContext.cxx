/*

Copyright (c) 2012-2023, Dr Franck P. Vidal, Bangor University, All rights reserved.
Copyright (c) 2023-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
UK Research and Innovation, All rights reserved.

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
*   @date       10/04/2024
*
*   @author     Prof Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal, Bangor University, Sept 2022-Sept 2023, version 1.1, BSD 3-Clause License
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Oct 2023-present, version 1.1, BSD 3-Clause License
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

#ifndef __Context_h
#include "gVirtualXRay/Context.h"
#endif

#ifndef __Logger_h
#include <gVirtualXRay/Logger.h>
#endif

#ifndef __Exception_h
#include <gVirtualXRay/Exception.h>
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;
using namespace std;


//--------------------------------------
TEST(TestOpenGL, TestDefaultConstructor)
//--------------------------------------
{
    Context GL_context;

    // Unknown OpenGL version
    const GLubyte* error_message = glGetString(GL_VERSION);
    ASSERT_EQ(error_message, nullptr);

    ASSERT_EQ(GL_context.isReady(), false);
    ASSERT_EQ(GL_context.getWindowHandle(), nullptr);

    GL_context.release();
}


//-------------------------------------------
TEST(TestOpenGL2_1, TestConstructorOpenGL2_1)
//-------------------------------------------
{
    // Create an OpenGL context
#ifdef HAS_EGL
    Context GL_context("EGL", 2, 1);
#else
    Context GL_context("OpenGL", 2, 1);
#endif

    LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

    GL_context.release();
}


#ifndef __APPLE__
//-------------------------------------------
TEST(TestOpenGL3_0, TestConstructorOpenGL3_0)
//-------------------------------------------
{
    // Create an OpenGL context
#ifdef HAS_EGL
    Context GL_context("EGL", 3, 0);
#else
    Context GL_context("OpenGL", 3, 0);
#endif

    LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

    GL_context.release();
}


//-------------------------------------------
TEST(TestOpenGL3_1, TestConstructorOpenGL3_1)
//-------------------------------------------
{
    // Create an OpenGL context
#ifdef HAS_EGL
    Context GL_context("EGL", 3, 1);
#else
    Context GL_context("OpenGL", 3, 1);
#endif

    LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

    GL_context.release();
}
#endif

//-------------------------------------------
TEST(TestOpenGL3_2, TestConstructorOpenGL3_2)
//-------------------------------------------
{
    // Create an OpenGL context
#ifdef HAS_EGL
    Context GL_context("EGL", 3, 2);
#else
    Context GL_context("OpenGL", 3, 2);
#endif

    LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

    GL_context.release();
}


//-------------------------------------------
TEST(TestOpenGL3_3, TestConstructorOpenGL3_3)
//-------------------------------------------
{
    // Create an OpenGL context
#ifdef HAS_EGL
    Context GL_context("EGL", 3, 3);
#else
    Context GL_context("OpenGL", 3, 3);
#endif

    LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

    GL_context.release();
}


//-------------------------------------------
TEST(TestOpenGL4_0, TestConstructorOpenGL4_0)
//-------------------------------------------
{
    // Create an OpenGL context
#ifdef HAS_EGL
    Context GL_context("EGL", 4, 0);
#else
    Context GL_context("OpenGL", 4, 0);
#endif

    LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

    GL_context.release();
}


//-------------------------------------------
TEST(TestOpenGL4_1, TestConstructorOpenGL4_1)
//-------------------------------------------
{
    // Create an OpenGL context
#ifdef HAS_EGL
    Context GL_context("EGL", 4, 1);
#else
    Context GL_context("OpenGL", 4, 1);
#endif

    LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

    GL_context.release();
}


#ifndef __APPLE__
//-------------------------------------------
TEST(TestOpenGL4_2, TestConstructorOpenGL4_2)
//-------------------------------------------
{
    // Create an OpenGL context
#ifdef HAS_EGL
    Context GL_context("EGL", 4, 2);
#else
    Context GL_context("OpenGL", 4, 2);
#endif

    LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

    GL_context.release();
}


//-------------------------------------------
TEST(TestOpenGL4_3, TestConstructorOpenGL4_3)
//-------------------------------------------
{
    // Create an OpenGL context
#ifdef HAS_EGL
    Context GL_context("EGL", 4, 3);
#else
    Context GL_context("OpenGL", 4, 3);
#endif

    LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

    GL_context.release();
}


//-------------------------------------------
TEST(TestOpenGL4_4, TestConstructorOpenGL4_4)
//-------------------------------------------
{
    // Create an OpenGL context
#ifdef HAS_EGL
    Context GL_context("EGL", 4, 4);
#else
    Context GL_context("OpenGL", 4, 4);
#endif

    LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

    GL_context.release();
}


//-------------------------------------------
TEST(TestOpenGL4_5, TestConstructorOpenGL4_5)
//-------------------------------------------
{
    // Create an OpenGL context
#ifdef HAS_EGL
    Context GL_context("EGL", 4, 5);
#else
    Context GL_context("OpenGL", 4, 5);
#endif

    LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

    GL_context.release();
}


//-------------------------------------------
TEST(TestOpenGL4_6, TestConstructorOpenGL4_6)
//-------------------------------------------
{
    // Create an OpenGL context
#ifdef HAS_EGL
    Context GL_context("EGL", 4, 6);
#else
    Context GL_context("OpenGL", 4, 6);
#endif

    LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

    GL_context.release();
}
#endif


//--------------------------------------
TEST(TestOpenGL2_1, TestCreateOpenGL2_1)
//--------------------------------------
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
//--------------------------------------
TEST(TestOpenGL3_0, TestCreateOpenGL3_0)
//--------------------------------------
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


//--------------------------------------
TEST(TestOpenGL3_1, TestCreateOpenGL3_1)
//--------------------------------------
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

//--------------------------------------
TEST(TestOpenGL3_2, TestCreateOpenGL3_2)
//--------------------------------------
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


//--------------------------------------
TEST(TestOpenGL3_3, TestCreateOpenGL3_3)
//--------------------------------------
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


//--------------------------------------
TEST(TestOpenGL4_0, TestCreateOpenGL4_0)
//--------------------------------------
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


//--------------------------------------
TEST(TestOpenGL4_1, TestCreateOpenGL4_1)
//--------------------------------------
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
//--------------------------------------
TEST(TestOpenGL4_2, TestCreateOpenGL4_2)
//--------------------------------------
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


//--------------------------------------
TEST(TestOpenGL4_3, TestCreateOpenGL4_3)
//--------------------------------------
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


//--------------------------------------
TEST(TestOpenGL4_4, TestCreateOpenGL4_4)
//--------------------------------------
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


//--------------------------------------
TEST(TestOpenGL4_5, TestCreateOpenGL4_5)
//--------------------------------------
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


//--------------------------------------
TEST(TestOpenGL4_6, TestCreateOpenGL4_6)
//--------------------------------------
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
