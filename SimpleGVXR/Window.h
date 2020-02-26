#ifndef __Window_h
#define __Window_h


#include <string>

#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>


//******************************************************************************
//  Function declarations
//******************************************************************************


class Window
{
public:


    //------------------------------------------------------------------------------
    /// Create a window
    /**
     *  @param aVisibilityFlag: flag controling if the window should be visible (1)
     *                          or hidden (0). (default value: 0)
     */
    //------------------------------------------------------------------------------
    Window(int aVisibilityFlag = 0);


    //------------------------------------------------------------------------------
    /// Create an OpenGL context and display it in a window
    /**
     *  @param aWidth:      the number of pixels along the X-axis
     *  @param aHeight:     the number of pixels along the Y-axis
     */
    //------------------------------------------------------------------------------
    virtual void setWindowSize(int aWidth, int aHeight) = 0;


protected:
    // Initialise GLEW
    virtual void createWindow() = 0;

    // Initialise GLFW
    void initGLFW();

    // Initialise OpenGL
    virtual void initShader() = 0;


    std::string m_title;
    bool m_visible;
    int m_width;
    int m_height;
    GLFWwindow* m_p_window;
};


#endif
