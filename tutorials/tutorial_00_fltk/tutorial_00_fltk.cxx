/*

Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*   @file       tutorial_00_fltk.cxx
*
*   @brief      Creating a Window using FLTK.
*
*   @version    1.0
*
*   @date       02/09/2016
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

#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <FL/Fl.H>           // Header for FLTK
#include <FL/gl.h>           // Header for OpenGL via FLTK
#include <FL/Fl_Gl_Window.H> // Header to create an OpenGL window using FLTK

#include <iostream>          // Print error messages in the console
#include <exception>         // Catch C++ exception
#include <cstdlib>           // Define return status (EXIT_SUCCESS and
                             // EXIT_FAILURE)


//******************************************************************************
//  Name space
//******************************************************************************
using namespace std;


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
void quitCallback();


//******************************************************************************
//  Class definition
//******************************************************************************
class GLWidget: public Fl_Gl_Window
{
public:
    GLWidget(int X, int Y, int W, int H, const char *L);
    ~GLWidget();


protected:
    void initializeGL();
    virtual void draw();
    virtual void resize(int width, int height);
    virtual int handle(int event);

    bool m_is_first_time;
};


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
        // Initialise FLTK and OpenGL
        GLWidget window(0,
                0,
                g_window_width,
                g_window_height,
                "gVirtualXRay -- Tutorial 00 using FLTK");
        window.resizable(&window);

        // Launch the event loop
        window.show();
        return_code = Fl::run();
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


//------------------------------------------------------------
GLWidget::GLWidget(int X, int Y, int W, int H, const char *L):
//------------------------------------------------------------
        Fl_Gl_Window(X, Y, W, H, L),
        m_is_first_time(true)
//------------------------------------------------------------
{
    mode(FL_RGB | FL_ALPHA | FL_DEPTH | FL_DOUBLE | FL_MULTISAMPLE);
}


//-------------------
GLWidget::~GLWidget()
//-------------------
{}


//---------------------------
void GLWidget::initializeGL()
//---------------------------
{
    // Set the background colour
    glClearColor(0.5, 0.5, 0.5, 1.0);
}


//-------------------
void GLWidget::draw()
//-------------------
{
    // Initialise OpenGL
    if (m_is_first_time)
    {
        initializeGL();
        m_is_first_time = false;
    }

    // Clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


//------------------------------------------
void GLWidget::resize(int width, int height)
//------------------------------------------
{
    // Avoid a division by zero
    if (height == 0)
    {
        // Prevent divide by 0
        height = 1;
    }

    int x(0), y(0), w(width), h(height);

    // Store the width and height of the window
    g_window_width = width;
    g_window_height = height;

    // Compute the aspect ratio of the size of the window
    double screen_aspect_ratio(double(g_window_width) / double(g_window_height));

    // Update the viewport
    glViewport(x, y, w, h);
}


//-----------------------------
int GLWidget::handle(int event)
//-----------------------------
{
    switch(event)
    {
    case FL_KEYBOARD:
        switch (Fl::event_key())
        {
        case 27:
        case 'q':
            exit(EXIT_SUCCESS);

        default:
            break;
        }

    default:
        break;
    }

    return (Fl_Gl_Window::handle(event));
}


//-----------------
void quitCallback()
//-----------------
{
    // Do nothing
}

