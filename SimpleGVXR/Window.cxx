#include <iostream>

#ifndef __Window_h
#include "Window.h"
#endif

#ifndef __Exception_h
#include "gVirtualXRay/Exception.h"
#endif


//******************************************************************************
//  Namespaces
//******************************************************************************
using namespace std;


//******************************************************************************
//  Function declarations
//******************************************************************************
void errorCallback(int error, const char* description);


//******************************************************************************
//  Global variables
//******************************************************************************


//******************************************************************************
//  Function definitions
//******************************************************************************


//----------------------------------
Window::Window(int aVisibilityFlag):
//----------------------------------
    m_title("gVirtualXRay -- Window"),
    m_visible(aVisibilityFlag),
    m_width(500),
    m_height(500),
    m_p_window(0)
//----------------------------------
{
    // Set an error callback
    glfwSetErrorCallback(errorCallback);

    // Initialize GLFW
    if (!glfwInit())
        {
        throw gVirtualXRay::Exception(
            __FILE__,
            __FUNCTION__,
            __LINE__,
            "ERROR: cannot initialise GLFW.");
        }

    cout << "gvxrStatus:\tCreate window " << endl;

    // Create the window
    createWindow();

    // Window has not been created
    if (!m_p_window)
        {
        // Display a warning
        cerr << "gvxrWarning:\tWindow " <<
            " cannot be created." <<
            endl;
        }
}


void Window::setWindowSize(int aWidth, int aHeight)
{
    // The size has changed
    if (m_width != aWidth || m_height != aHeight)
    {
        m_width  = aWidth;
        m_height = aHeight;

        glfwSetWindowSize(m_p_window, aWidth, aHeight);
    }
}


//----------------------------------------------------
void errorCallback(int error, const char* description)
//----------------------------------------------------
{
    // Display the error in the terminal
    std::cerr << "GLFW error: " << description << std::endl;
}
