/*

Copyright (c) 2016, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*   @file       tutorial_07_qt4.cxx
*
*   @brief      Improve the second tutorial by:
*               - Adding a better shader program to display
*                 the 3D scene with Phong shading.
*               - Adding the X-ray simulation.
*               - Display objects in wireframe or not
*               - Using operator - to combine two polygon meshes
*                 into a single one
*
*   @version    1.0
*
*   @date       24/08/2016
*
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
*               http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifdef HAS_GLEW
#include <GL/glew.h>
#endif

#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <QOpenGLFunctions_3_2_Core>
#include <QApplication>
#include <QtGui>
#include <QGLWidget>

#include <iostream>   // Print error messages in the console
#include <sstream>    // Generate error messages
#include <exception>  // Catch C++ exception
#include <cstdlib>    // Define return status (EXIT_SUCCESS and EXIT_FAILURE)

// Define new types, e.g.~RATIONAL_NUMBER, VEC2, VEC3 and MATRIX4 to name a few
#include "gVirtualXRay/Types.h"

// Define units such as metre, kilometre, electronvolt, gram, kilogram, etc.
#include "gVirtualXRay/Units.h"

// Some utility functions about OpenGL, e.g. matrix stacks,
// how to set the projection matrix, etc.
#include "gVirtualXRay/OpenGLUtilities.h"

#include "gVirtualXRay/PolygonMesh.h"  // Handle 3D triangle meshes
#include "gVirtualXRay/Shader.h"       // Handle GLSL programs

#include "gVirtualXRay/Utilities.h" // Handle decompression using the Z library

#include "gVirtualXRay/XRayBeam.h"     // Manage X-Ray beams
#include "gVirtualXRay/XRayDetector.h" // Handle virtual X-Ray detectors
#include "gVirtualXRay/XRayRenderer.h" // Compute X-Ray images on the GPU

#include "buildCube.h" // Create the triangle mesh of a cube

#include "tutorial_07_gl2.frag.h" // Fragment shader for OpenGL 2.x
#include "tutorial_07_gl2.vert.h" // Vertex shader for OpenGL 2.x

#include "tutorial_07_gl3.frag.h" // Fragment shader for OpenGL 3.x
#include "tutorial_07_gl3.vert.h" // Vertex shader for OpenGL 3.x


#ifndef __GLWidget_h_
#include "GLWidget.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;
using namespace std;


//******************************************************************************
//  Constant variables
//******************************************************************************
const GLfloat g_rotation_speed(2.0);


//******************************************************************************
//  Global variables
//******************************************************************************
// Keep track of the window width
GLsizei g_window_width(640);

// Keep track of the window height
GLsizei g_window_height(480);


//******************************************************************************
//  Function declaration
//******************************************************************************
/*void initQt(GLWidget& aWindow);

void initGLUT(int argc, char** argv);



void displayCallback();

void idleCallback();

void quitCallback();

void framebufferSizeCallback(int aWidth, int aHeight);

void keyCallback(unsigned char aKey, int aXPosition, int aYPosition);

void mouseButtonCallback(int aButton,
        int aState,
        int aXPosition,
        int aYPosition);

void cursorPosCallback(int aXPosition, int aYPosition);

VEC3 getArcballVector(int x, int y);

float radian2degree(float anAngle);

void computeRotation(MATRIX4& aRotationMatrix);
*/



//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
    // Return code
    int return_code(EXIT_SUCCESS);

    // Register the exit callback
    atexit(quitCallback);

    try
        {
        // Initialise Qt and OpenGL
        QApplication application(argc, argv);

        // Create a GL format to OpenGL 3.2 Core
        QGLFormat gl_format;
        gl_format.setVersion(3, 2);
        gl_format.setProfile(QGLFormat::CoreProfile);

        // Set the GL format of the QGLWidget
        GLWidget window(gl_format);
        initQt(window);

        // Launch the event loop
        return_code = application.exec();
        }
    // Catch exception if any
    catch (const exception& error)
        {
        cerr << error.what() << endl;
        return_code = EXIT_FAILURE;
        }

    // Clean up
    quitCallback();

    // Return an exit code
    return (return_code);
}


//---------------------------
void initQt(GLWidget& aWindow)
//---------------------------
{
    // Initialize Qt
    // and create a windowed mode window and its OpenGL context

    aWindow.resize(g_window_width, g_window_height);
    aWindow.show();
    aWindow.setWindowTitle(QApplication::translate("toplevel",
            "gVirtualXRay -- Tutorial 07 using Qt4"));
    
    // Make the window's context current
    aWindow.makeCurrent();
}




//-----------------
void quitCallback()
//-----------------
{
    // Do nothing
}


//----------------------------------------------------
void errorCallback(int error, const char* description)
//----------------------------------------------------
{
    // Throw an error
    throw Exception(__FILE__, __FUNCTION__, __LINE__, description);
}



