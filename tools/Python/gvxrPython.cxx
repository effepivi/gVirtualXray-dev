#ifdef HAS_GLEW
#include <GL/glew.h>
#endif

#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#define GLFW_INCLUDE_GLCOREARB 1

#include <GLFW/glfw3.h>


#include <cstdlib>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

#include <boost/python.hpp>

// For exceptions
#ifndef __Exception_h
#include "gVirtualXRay/Exception.h"
#endif

// For types such as VEC3, MATRIX4
#ifndef __Types_h
#include "gVirtualXRay/Types.h"
#endif

// For polygon meshes
#ifndef __PolygonMesh_h
#include "gVirtualXRay/PolygonMesh.h"
#endif

// To set the energy of the photons in the incident beam
#ifndef __XRayBeam_h
#include "gVirtualXRay/XRayBeam.h"
#endif

// To set the properties of the X-ray detector
#ifndef __XRayDetector_h
#include "gVirtualXRay/XRayDetector.h"
#endif

// To perform the computations of the X-ray simulation
#ifndef __XRayRenderer_h
#include "gVirtualXRay/XRayRenderer.h"
#endif

// To initialise GLEW, to define matrix stacks, to define state stacks, etc.
#ifndef __OpenGLUtilities_h
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

// Shaders for rendering
//#include "display_gl2.frag.h"
//#include "display_gl2.vert.h"

//#include "display_gl3.frag.h"
//#include "display_gl3.vert.h"


//******************************************************************************
//  Name space
//******************************************************************************
using namespace std;
using namespace gVirtualXRay;


//******************************************************************************
//  Function declaration
//******************************************************************************
void createWindow(int aWindowID = -1);
void setWindowSize(int aWidth, int aHeight, int aWindowID = -1);
void destroyWindow(int aWindowID = -1);
void destroyAllWindows();
void computeXRayImage();
void usePointSource();
void useParallelBeam();
void setMonoChromatic(float anEnergyInMeV);
void setSourcePosition(float x, float y, float z);
void setDetectorPosition(float x, float y, float z);
void setDetectorUpVector(float x, float y, float z);
void setDetectorNumberOfPixels(int aWidth, int aHeight);
void setDetectorPixelSize(float aWidth, float aHeight);
void saveLastXRayImage(const char* aFileName = 0);

void quit();
GLFWwindow* initGLFW(int aWindowID);
void initGL();
void errorCallback(int error, const char* description);


//******************************************************************************
//  Global variables
//******************************************************************************
std::map<int, GLFWwindow*> g_p_window_set;
std::vector<int> g_p_window_id_stack;

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;
Shader g_display_shader;

int g_file_counter = 0;
bool g_renderer_initialised = false;


//******************************************************************************
//  Implementation
//******************************************************************************


BOOST_PYTHON_FUNCTION_OVERLOADS(createWindow_overloads, createWindow, 0, 1)
BOOST_PYTHON_FUNCTION_OVERLOADS(setWindowSize_overloads, setWindowSize, 2, 3)
BOOST_PYTHON_FUNCTION_OVERLOADS(saveLastXRayImage_overloads, saveLastXRayImage, 0, 1)


//------------------------------
void createWindow(int aWindowID)
//------------------------------
{
    if (aWindowID == -1)
    {
        aWindowID = g_p_window_set.size();
    }

    cout << "gvxrStatus:\tCreate window " << aWindowID << endl;

    // The window does not exist
    if (!g_p_window_set[aWindowID])
    {
        // Create the window
        GLFWwindow* p_window = initGLFW(aWindowID);

        // The window has been created
        if (p_window)
        {
            // Add the window
            g_p_window_set[aWindowID] = p_window;

            // Register its ID
            g_p_window_id_stack.push_back(aWindowID);
        }
        // The window has not been created
        else
        {
            cerr << "gvxrWarning:\tWindow " << aWindowID << " cannot be created." << endl;
        }
    }
    // The window already exist
    // Do nothing
    else
    {
        cerr << "gvxrWarning:\tWindow " << aWindowID << " already exists." << endl;
    }
}


//--------------------------------------------------------
void setWindowSize(int aWidth, int aHeight, int aWindowID)
//--------------------------------------------------------
{
    // Resize the last window that has been created
    if (aWindowID == -1)
    {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
        {
            cerr << "gvxrWarning:\tNo active window to resize." << endl;
        }
        // Resize the last created window
        else
        {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
        }
    }

    // There is a window to resize
    if (aWindowID != -1)
    {
        // Find the window
        auto search = g_p_window_set.find(aWindowID);
        
        // It has been found
        if(search != g_p_window_set.end())
        {
            if (search->second) glfwSetWindowSize(search->second, aWidth, aHeight);
        }
        else
        {
            cerr << "gvxrWarning:\tWindow " << aWindowID << " not found." << endl;
        }
    }
}


//-------------------------------
void destroyWindow(int aWindowID)
//-------------------------------
{
    // Destroy the last window that has been created
    if (aWindowID == -1)
    {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
        {
            cerr << "gvxrWarning:\tNo active window to destroy." << endl;
        }
        // Destroy the last created window
        else
        {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
        }
    }

    // There is a window to destroy
    if (aWindowID != -1)
    {
        // Find the window
        auto search_window = g_p_window_set.find(aWindowID);
        
        // It has been found
        if(search_window != g_p_window_set.end())
        {
            std::cout << "Destroy " << search_window->first << " " << search_window->second << endl;
            if (search_window->second) glfwDestroyWindow(search_window->second);
            g_p_window_set.erase(search_window);
        }
        else
        {
            cerr << "gvxrWarning:\tWindow " << aWindowID << " not found in g_p_window_set." << endl;
        }

        // Unregister its ID from the stack of IDs
        // Find the ID
        std::vector<int>::iterator search_id = find(g_p_window_id_stack.begin(),
                g_p_window_id_stack.end(), aWindowID);

        // It has been found
        if(search_id != g_p_window_id_stack.end())
        {
            g_p_window_id_stack.erase(search_id);
        }
        else
        {
            cerr << "gvxrWarning:\tWindow " << aWindowID << " not found in g_p_window_id_stack." << endl;
        }

    }
}


//----------------------
void destroyAllWindows()
//----------------------
{
    cout << "gvxrStatus:\tDestroy all the windows" << endl;

    // Process all the windows
    for (auto& window_ite: g_p_window_set)
    {
        // Destroy the window if possible
        destroyWindow(window_ite.first);
    }

    // Empty the set of windows
    g_p_window_set.clear();

    // Terminate GLFW
    //glfwTerminate();    
}


//---------------------
void computeXRayImage()
//---------------------
{
    // There is no OpenGL context
    if (g_p_window_id_stack.empty() || g_p_window_set.empty())
    {
        // Create a window
    cout << "\tThere is no OpenGL context" << endl;
    cout << "\tCreate a window" << endl;
        createWindow();
    }

    // The renderer is not initialised
    if (!g_renderer_initialised)
    {
    cout << "\tThe renderer is not initialised" << endl;
        // Initialise the renderer
        cout << "\tInitialise the renderer" << endl;
        g_xray_renderer.initialise(XRayRenderer::OPENGL,
            GL_RGB32F,
            &g_xray_detector,
            &g_xray_beam);
    }

    // The renderer is ready
    cout << "\tTest if the renderer is ready" << endl;
    if (g_xray_renderer.isReady())
    {
        cout << "\tThe renderer is ready" << endl;
        // Compute the X-ray image
        cout << "\tCompute the X-ray image" << endl;
        g_xray_renderer.computeImage(MATRIX4());
    }
    // The renderer is not ready
    else
    {
        cerr << "gvxrWarning:\tX-ray renderer not ready." << endl;
    }
}


//-------------------
void usePointSource()
//-------------------
{
    g_xray_detector.setXrayPointSource();
}


//--------------------
void useParallelBeam()
//--------------------
{
    // Use a parallel source
    g_xray_detector.setParallelBeam();
}


//----------------------------------------
void setMonoChromatic(float anEnergyInMeV)
//----------------------------------------
{
    g_xray_beam.initialise(anEnergyInMeV * MeV);
}


//-----------------------------------------------
void setSourcePosition(float x, float y, float z)
//-----------------------------------------------
{
    g_xray_detector.setXrayPointSource(VEC3(x * mm, y * mm, z * mm));
}


//-------------------------------------------------
void setDetectorPosition(float x, float y, float z)
//-------------------------------------------------
{
    g_xray_detector.setDetectorPosition(VEC3(x * mm, y * mm, z * mm));
}


//-------------------------------------------------
void setDetectorUpVector(float x, float y, float z)
//-------------------------------------------------
{
    g_xray_detector.setUpVector(VEC3(x, y, z).normal());
}


//-----------------------------------------------------
void setDetectorNumberOfPixels(int aWidth, int aHeight)
//-----------------------------------------------------
{
    g_xray_detector.setNumberOfPixels(Vec2ui(aWidth, aHeight));
}


//----------------------------------------------------
void setDetectorPixelSize(float aWidth, float aHeight)
//----------------------------------------------------
{
    g_xray_detector.setResolutionInUnitsOfLengthPerPixel(VEC2(aWidth * mm, aHeight * mm));
}


//-------------------------------------------
void saveLastXRayImage(const char* aFileName)
//-------------------------------------------
{
    string file_name;

    if (aFileName)
        file_name = aFileName;
    else
        file_name = "xray_image-" + to_string(g_file_counter++) + ".mha";

    g_xray_renderer.printXRayImage(file_name);
}


//---------
void quit()
//---------
{
    cout << "gvxrStatus:\tQuit and destroy all the windows" << endl;
    destroyAllWindows();
}


//---------------------------------
GLFWwindow* initGLFW(int aWindowID)
//---------------------------------
{
    string window_title = "gVirtualXRay -- Window " + to_string(aWindowID);
    GLFWwindow* p_window = 0;

	// Set the version of OpenGL that is required
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	// Enable anti-aliasing
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create a windowed mode window and its OpenGL context
	p_window = glfwCreateWindow(500,
		500,
		window_title.data(),
		0,
		0);

	// Window has be created
	if (p_window)
		{
	    // Make the window's context current
	    glfwMakeContextCurrent(p_window);

	    // Initialize GLFW callbacks
	    //glfwSetKeyCallback(g_p_main_window_id, keyCallback);
	    //glfwSetMouseButtonCallback(g_p_main_window_id, mouseButtonCallback);
	    //glfwSetCursorPosCallback(g_p_main_window_id, cursorPosCallback);
	    //glfwSetScrollCallback(g_p_main_window_id, scrollCallback);
	    //glfwSetFramebufferSizeCallback(g_p_main_window_id,
	    //	framebufferSizeCallback);
    }

    return (p_window);
}


void initGL()
{
}


//----------------------------------------------------
void errorCallback(int error, const char* description)
//----------------------------------------------------
{
	// Display the error in the terminal
    std::cerr << "GLFW error: " << description << std::endl;
}


BOOST_PYTHON_MODULE(gvxrPython)
{
    using namespace boost::python;

    cout << "gvxrStatus:\tInitialise gvxrPython" << endl;


    // Make sure all the windows are destroyed
    atexit(quit);

	// Set an error callback
	glfwSetErrorCallback(errorCallback);

	// Initialize GLFW
	if (!glfwInit())
		{
		throw Exception(__FILE__, __FUNCTION__, __LINE__,
			"ERROR: cannot initialise GLFW.");
		}

    // Expose C++ to Python
    def("createWindow",  createWindow,  createWindow_overloads());
    def("setWindowSize", setWindowSize, setWindowSize_overloads());

    def("destroyWindow",     destroyWindow);
    def("destroyAllWindows", destroyAllWindows);        

    def("computeXRayImage", computeXRayImage); 
    
    def("usePointSource",  usePointSource);        
    def("useParallelBeam", useParallelBeam);        

    def("setMonoChromatic", setMonoChromatic);        
           
    def("setSourcePosition",         setSourcePosition);        
    def("setDetectorPosition",       setDetectorPosition);        
    def("setDetectorUpVector",       setDetectorUpVector);        
    def("setDetectorNumberOfPixels", setDetectorNumberOfPixels);        
    def("setDetectorPixelSize",      setDetectorPixelSize);        

    def("saveLastXRayImage", saveLastXRayImage, saveLastXRayImage_overloads());
}

