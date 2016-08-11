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
*   @file       tutorial_00_glfw.cxx
*
*   @brief      Creating a Window using GLFW.
*
*   @version    1.0
*
*   @date       26/08/2014
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
#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <QApplication>
#include <QtGui>
#include <QGLWidget> 

#include <iostream>   // Print error messages in the console
#include <exception>  // Catch C++ exception
#include <cstdlib>    // Define return status (EXIT_SUCCESS and EXIT_FAILURE)


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
//  Class declaration
//******************************************************************************
class GLWidget;


//******************************************************************************
//  Function declaration
//******************************************************************************
void initQt(GLWidget& aWindow);
void quitCallback();


//******************************************************************************
//  Class definition
//******************************************************************************
class GLWidget: public QGLWidget
{
public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent * event);
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
        // Initialise Qt and OpenGL
        QApplication application(argc, argv);
        GLWidget window;
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
    aWindow.setWindowTitle(QApplication::translate("toplevel", "gVirtualXRay -- Tutorial 00"));
    
    // Make the window's context current
    aWindow.makeCurrent();
}


//----------------------------------
GLWidget::GLWidget(QWidget *parent):
//----------------------------------
    QGLWidget(parent)
//----------------------------------
{
}


//-------------------
GLWidget::~GLWidget()
//-------------------
{}


//-------------------------------------
QSize GLWidget::minimumSizeHint() const
//-------------------------------------
{
    return QSize(50, 50);
}


//------------------------------
QSize GLWidget::sizeHint() const
//------------------------------
{
    return QSize(g_window_width, g_window_height);
}
   

//---------------------------
void GLWidget::initializeGL()
//---------------------------
{
    // Set the background colour
    qglClearColor(QColor(255.0 * 0.5, 255.0 * 0.5, 255.0 * 0.5, 255.0 * 1.0));
}


//----------------------
void GLWidget::paintGL()
//----------------------
{
    // Clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


//--------------------------------------------
void GLWidget::resizeGL(int width, int height)
//--------------------------------------------
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


//-------------------------------------------
void GLWidget::keyPressEvent(QKeyEvent * event)
//-------------------------------------------
{
    switch(event->key())
    {
    // Close the program
    case Qt::Key_Q:
    case Qt::Key_Escape:
            close();
            break;

    default:
    std::cout << event->key() << std::endl;
        break;
    }
}


//-----------------
void quitCallback()
//-----------------
{
    // Do nothing
}

