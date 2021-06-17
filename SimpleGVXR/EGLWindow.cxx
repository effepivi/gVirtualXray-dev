/*

Copyright (c) 2020, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*   @file       EGLWindow.cxx
*
*   @brief      EGLWindow is a class to handle an OpenGL context create
*   			using EGL.
*
*   @version    1.0
*
*   @date       08/09/2020
*
*   @author     Dr Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*   http://www.fpvidal.net/, Sept 2020, 2020, version 1.1, BSD 3-Clause License
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

// To initialise GLEW, to define matrix stacks, to define state stacks, etc.
#ifndef __OpenGLUtilities_h
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef __EGLWindow_h
#include "EGLWindow.h"
#endif


//******************************************************************************
//  Namespaces
//******************************************************************************
using namespace std;


extern std::vector<std::vector<std::vector< float> > > lastest_screenshot;


//---------------------------------------------
EGLWindow::EGLWindow(int anOpenGLMajorVersion,
                     int anOpenGLMinorVersion):
//---------------------------------------------
    Window(false)
//---------------------------------------------
{
    // Create the context
	m_context.create("EGL",
			anOpenGLMajorVersion,
	        anOpenGLMinorVersion);

    // Window has not been created
    if (!m_context.isReady())
    {
        // Display a warning
        cerr << "gvxrWarning:\tEGLWindow " <<
            " cannot be created." <<
            endl;
    }
}


//-----------------------------------------------------
void EGLWindow::setWindowSize(int aWidth, int aHeight)
//-----------------------------------------------------
{
	// Do nothing
}

//----------------------------------------------------
void EGLWindow::displayScene(bool aSceneRotationFlag)
//----------------------------------------------------
{
	// Do nothing
}


//------------------------------------
void EGLWindow::takeScreenshot() const
//------------------------------------
{
    GLfloat* p_pixel_set = new GLfloat[3 * m_width * m_height];
    glReadPixels(0, 0, m_width, m_height, GL_RGB, GL_FLOAT, p_pixel_set);

    lastest_screenshot.clear();
    
    for (unsigned int j = 0; j < m_height; ++j)
    {
        lastest_screenshot.push_back(std::vector<std::vector< float> >() );
        
        for (unsigned int i = 0; i < m_width; ++i)
        {
            lastest_screenshot.back().push_back(std::vector< float>());
            
            lastest_screenshot.back().back().push_back(p_pixel_set[j * m_width * 3 + i * 3 + 0]);
            lastest_screenshot.back().back().push_back(p_pixel_set[j * m_width * 3 + i * 3 + 1]);
            lastest_screenshot.back().back().push_back(p_pixel_set[j * m_width * 3 + i * 3 + 2]);
        }
    }

    delete [] p_pixel_set;
}


//---------------------------------------------------
void EGLWindow::createWindow(bool aVisibilityFlag)
//---------------------------------------------------
{
	// Do nothing
}


//---------------------------
void EGLWindow::initShader()
//---------------------------
{
	// Do nothing
}


//----------------------------------------------
void EGLWindow::render(bool aSceneRotationFlag)
//----------------------------------------------
{
	// Do nothing
}
