/*

Copyright (c) 2013-2023, Dr Franck P. Vidal, Bangor University, All rights reserved.
Copyright (c) 2023-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
UK Research and Innovation, All rights reserved.


Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of Bangor University, UK Research and Innovation nor the 
names of its contributors may be used to endorse or promote products derived 
from this software without specific prior written permission.

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
*   @date       22/04/2024
*
*   @author     Prof Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal, Bangor University, Sept 2022-Sept 2023, version 1.1, BSD 3-Clause License
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Oct 2023-present, version 1.1, BSD 3-Clause License
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

#ifndef __Shader_h
#include "gVirtualXRay/Shader.h"
#endif

#ifndef __Logger_h
#include <gVirtualXRay/Logger.h>
#endif

#ifndef __Exception_h
#include <gVirtualXRay/Exception.h>
#endif

#ifndef __utils_h
#include "utils.h"
#endif


void testComputeShader();


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
    ASSERT_EQ(error_message == nullptr, true);

    ASSERT_EQ(GL_context.isReady(), false);
    ASSERT_EQ(GL_context.getWindowHandle() == nullptr, true);

    GL_context.release();
}


//--------------------------------
TEST(TestOpenGL, TestConstructors)
//--------------------------------
{
    std::vector<std::pair<unsigned int, unsigned int>> p_context_version_set;
    /*
    p_context_version_set.push_back(std::pair<unsigned int, unsigned int>(3, 1));

#ifndef __APPLE__
    p_context_version_set.push_back(std::pair<unsigned int, unsigned int>(3, 2));
    p_context_version_set.push_back(std::pair<unsigned int, unsigned int>(3, 3));
#endif

    p_context_version_set.push_back(std::pair<unsigned int, unsigned int>(4, 1));
    p_context_version_set.push_back(std::pair<unsigned int, unsigned int>(4, 2));
    */
#ifndef __APPLE__
    p_context_version_set.push_back(std::pair<unsigned int, unsigned int>(4, 3));
    p_context_version_set.push_back(std::pair<unsigned int, unsigned int>(4, 4));
    p_context_version_set.push_back(std::pair<unsigned int, unsigned int>(4, 5));
    p_context_version_set.push_back(std::pair<unsigned int, unsigned int>(4, 6));
#endif

    for (std::vector<std::pair<unsigned int, unsigned int>>::const_iterator ite = p_context_version_set.begin();
            ite != p_context_version_set.end();
            ++ite)
    {
        LOGGER.logNow("********************************************************************************") << endl;
        LOGGER.logNow("Try to create an OpenGL context for version: ") << ite->first << "." << ite->second << endl;
        LOGGER.logNow("********************************************************************************") << endl;
        LOGGER.logNow("") << endl;

        // Create an OpenGL context
        Context GL_context;
        initGL(GL_context, ite->first, ite->second);
//         try
//         {
//             GL_context.create("OpenGL", ite->first, ite->second);
//         }
//         catch (const Exception& err)
//         {
//             string expected_message("Error: cannot initialise GLFW.");
//             ASSERT_EQ(string(err.what()).substr(0, expected_message.size()), expected_message);
//         }
//         catch(...)
//         {
//             FAIL() << "Expected gVirtualXRay::Exception";
//         }

// #ifdef HAS_GLX
//         if (!GL_context.isReady()) GL_context.create("GLX", ite->first, ite->second);
// #endif

// #ifdef HAS_EGL
//         if (!GL_context.isReady()) GL_context.create("EGL", ite->first, ite->second);
// #endif

        LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << endl;

        // The context should be ready
        EXPECT_EQ(GL_context.isReady(), true);

        int major = 0;
        int minor = 0;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);

        // If major is greater than ite->first, then consider that the test was successful
        if (major == ite->first)
        {
            EXPECT_EQ(major, ite->first);
            EXPECT_EQ(minor >= ite->second, true);
        }
        else if (major < ite->first)
        {
            EXPECT_EQ(major, ite->first);
        }

        LOGGER.logNow("") << endl;
        LOGGER.logNow("") << endl;

        if (ite->first == 4 && ite->second >= 3)
        {
            testComputeShader();
        }

        // Release the context, it shouldn't be ready now
        GL_context.release();
        EXPECT_EQ(GL_context.isReady(), false);

    }
}


void testComputeShader()
{
    EXPECT_EQ(useOpenGLCompute(), true);

    Shader dummy_shader;

    // Compile a dummy shader for OpenGL
    // Compute shader
    const GLchar* g_compute_shader = "\
	\n#version 430\n \
	\n \
	layout(local_size_x=1, local_size_y=1) in;\n \
	\n \
	layout(rgba32f, binding=0) uniform image2D output_image;\n \
	\n \
	void main(void)\n \
	{\n \
		ivec2 pixel_coordinates = ivec2(gl_GlobalInvocationID.xy);\n \
		vec4 pixel_value = vec4(pixel_coordinates.x, pixel_coordinates.y, 0.0, 1.0);\n \
		imageStore(output_image, pixel_coordinates, pixel_value);\n \
	}\n \
	";

    // Give a text ID to the compute  shader, it can be useful when debuging the shader
    dummy_shader.setLabel("g_compute_shader");

    // Load the source code of the shader onto the GPU
    dummy_shader.loadSource(g_compute_shader);

    EXPECT_EQ(dummy_shader.isValid(), true);
    


    // Generate the texture name
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Set the texture properties
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    GLint width(0);
    glTexImage2D(GL_PROXY_TEXTURE_2D, 0, GL_RGB32F, 32, 32, 0, GL_RGB, GL_FLOAT, 0);
    glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    if ((unsigned int)(width) == 32)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 32, 32, 0, GL_RGB, GL_FLOAT, 0);
    }
    else
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create a floating point render texture.");
    }

    // Check if any OpenGL error had occurred
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glBindImageTexture(0, texture_id, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGB32F);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    dummy_shader.enable();
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glDispatchCompute(32, 32, 1);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);


}

