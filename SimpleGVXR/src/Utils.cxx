#include <map>
#include <vector>
#include <iostream>

#ifndef __ConstantValues_h
#include "gVirtualXRay/ConstantValues.h"
#endif

#ifndef __Units_h
#include "gVirtualXRay/Units.h"
#endif

#ifndef __XRayRenderer_h
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef __Detector_h
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef __XRayBeam_h
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef __Context_h
#include "gVirtualXRay/Context.h"
#endif

#ifndef __Exception_h
#include "gVirtualXRay/Exception.h"
#endif

#ifndef __ResourceManager_h
#include "gVirtualXRay/ResourceManager.h"
#endif

#ifndef __Shader_h
#include "gVirtualXRay/Shader.h"
#endif

#ifndef __SceneGraphBinder_h
#include "gVirtualXRay/SceneGraphBinder.h"
#endif

#ifndef __Logger_h
#include "gVirtualXRay/Logger.h"
#endif

#ifndef __SGVXR_Window_h
#include "Window.h"
#endif

#ifndef __OpenGLWindow_h
#include "OpenGLWindow.h"
#endif

#ifndef __VulkanWindow_h
#include "VulkanWindow.h"
#endif

#ifndef __Windowless_h
#include "Windowless.h"
#endif

#ifndef __SGVXR_internal_h
#include "internal.h"
#endif

#ifndef __SGVXR_Utils_h
#include "Utils.h"
#endif


//******************************************************************************
//  Namespace
//******************************************************************************
using namespace std;


//******************************************************************************
//  Global variables
//******************************************************************************
extern gVirtualXRay::XRayRenderer g_xray_renderer; // X-ray graphics engine
extern gVirtualXRay::XRayBeam g_xray_beam;
extern gVirtualXRay::XRayDetector g_xray_detector; // X-ray detector
extern std::map<int, SimpleGVXR::Window*> g_p_window_set;
extern std::vector<int> g_p_window_id_stack;           // The stack of window IDs
extern gVirtualXRay::Shader g_display_shader;      // Visualisation shader
extern std::shared_ptr<gVirtualXRay::SceneGraphBinder> g_p_scene_graph;


//******************************************************************************
//  Function declarations
//******************************************************************************
void errorCallback(int error, const char* description);
void quit();


//******************************************************************************
//  Function definitions
//******************************************************************************


//---------
void quit()
//---------
{
    gVirtualXRay::LOGGER.logNow("Quit and destroy all the windows") << endl;
    gVirtualXRay::ResourceManager::getInstance().setGraphicsContext(nullptr);
    destroyAllWindows();
}

//----------------------------------------------------
void errorCallback(int error, const char* description)
//----------------------------------------------------
{
    // Display the error in the terminal
    gVirtualXRay::LOGGER.logWarning("GLFW error: ") << description << std::endl;
}


//--------------------------------
float radian2degree(float anAngle)
//--------------------------------
{
    return (180.0 * anAngle / gVirtualXRay::Pi);
}


//-----------------------------------------------
double getUnitOfEnergy(const std::string& aUnitOfEnergy)
//-----------------------------------------------
{
    double unit_of_energy = 0;

    if (aUnitOfEnergy == "megaelectronvolt" || aUnitOfEnergy == "MeV") {
    unit_of_energy = gVirtualXRay::MeV;
    } else if (aUnitOfEnergy == "kiloelectronvolt" || aUnitOfEnergy == "keV") {
    unit_of_energy = gVirtualXRay::keV;
    } else if (aUnitOfEnergy == "electronvolt" || aUnitOfEnergy == "eV") {
    unit_of_energy = gVirtualXRay::eV;
    } else {
    throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
    "ERROR: Unknown unit of energy.");
    }

    return (unit_of_energy);
}


//-----------------------------------------------
double getUnitOfLength(const std::string& aUnitOfLength)
//-----------------------------------------------
{
    double unit_of_length = 0;

    if ((aUnitOfLength == "kilometer") || (aUnitOfLength == "kilometre")
    || (aUnitOfLength == "km")) {
    unit_of_length = gVirtualXRay::km;
    } else if ((aUnitOfLength == "hectometer")
    || (aUnitOfLength == "hectometre") || (aUnitOfLength == "hm")) {
    unit_of_length = gVirtualXRay::hm;
    } else if ((aUnitOfLength == "decameter") || (aUnitOfLength == "decametre")
    || (aUnitOfLength == "dam")) {
    unit_of_length = gVirtualXRay::dam;
    } else if ((aUnitOfLength == "meter") || (aUnitOfLength == "metre")
    || (aUnitOfLength == "m")) {
    unit_of_length = gVirtualXRay::m;
    } else if ((aUnitOfLength == "decimeter") || (aUnitOfLength == "decimetre")
    || (aUnitOfLength == "dm")) {
    unit_of_length = gVirtualXRay::dm;
    } else if ((aUnitOfLength == "centimeter")
    || (aUnitOfLength == "centimetre") || (aUnitOfLength == "cm")) {
    unit_of_length = gVirtualXRay::cm;
    } else if ((aUnitOfLength == "millimeter")
    || (aUnitOfLength == "millimetre") || (aUnitOfLength == "mm")) {
    unit_of_length = gVirtualXRay::mm;
    } else if ((aUnitOfLength == "micrometer")
    || (aUnitOfLength == "micrometre") || (aUnitOfLength == "um")) {
    unit_of_length = gVirtualXRay::um;
    } else {
    throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
    "ERROR: Unknown unit of length.");
    }

    return (unit_of_length);
}


//---------------------------------------------------------------------------
double getUnitOfElectriPotential(const std::string& aUnitOfElectriPotential)
//---------------------------------------------------------------------------
{
    double unit_of_electric_potential = 0;

    if ((aUnitOfElectriPotential == "V") || (aUnitOfElectriPotential == "volt"))
    {
        unit_of_electric_potential = gVirtualXRay::V;
    }
    else if ((aUnitOfElectriPotential == "kV") || (aUnitOfElectriPotential == "kilovolt"))
    {
        unit_of_electric_potential = gVirtualXRay::kV;
    }
    else if ((aUnitOfElectriPotential == "MV") || (aUnitOfElectriPotential == "megavolt"))
    {
        unit_of_electric_potential = gVirtualXRay::MV;
    }
    else
    {
        throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
            "ERROR: Unknown unit of electric potential.");
    }

    return (unit_of_electric_potential);
}


//-------------------------------------
void enableArtefactFilteringOnGPU(void)
//-------------------------------------
{
    g_xray_renderer.enableArtifactFilteringOnGPU();
}

//-------------------------------------
void enableArtifactFilteringOnGPU(void)
//-------------------------------------
{
    enableArtefactFilteringOnGPU();
}

//-------------------------------------
void enableArtefactFilteringOnCPU(void)
//-------------------------------------
{
    g_xray_renderer.enableArtifactFilteringOnCPU();
}

//-------------------------------------
void enableArtifactFilteringOnCPU(void)
//-------------------------------------
{
    enableArtefactFilteringOnCPU();
}


//---------------------------------
void disableArtefactFiltering(void)
//---------------------------------
{
    g_xray_renderer.disableArtifactFilteringOnGPU();
    g_xray_renderer.disableArtifactFilteringOnCPU();
}

//---------------------------------
void disableArtifactFiltering(void)
//---------------------------------
{
    disableArtefactFiltering();
}


//----------------------------------
unsigned int getNumberOfEGLDevices()
//----------------------------------
{
    return gVirtualXRay::Context::getNumberOfEGLDevices();
}


//---------------------------------------
std::vector<std::string> findEGLClients()
//---------------------------------------
{
    return gVirtualXRay::Context::findEGLClients();
}


//-------------------------------------------------
void createOpenGLContext(int aWindowID,
		                 int aRendererMajorVersion,
		                 int aRendererMinorVersion)
//-------------------------------------------------
{
    createWindow(aWindowID,
        true,
        "OPENGL",
        aRendererMajorVersion,
        aRendererMinorVersion);
}


//---------------------------------------------
void createWindow(int aWindowID,
                  int aVisibilityFlag,
                  const std::string& aRenderer,
                  int aRendererMajorVersion,
                  int aRendererMinorVersion)
//---------------------------------------------
{
    gVirtualXRay::LOGGER.logNow("Create window (ID: ") << aWindowID << ")\n";

    // Create the window
    std::string renderer = aRenderer;
    for (auto& c: renderer) c = std::toupper(c);

    if (!g_p_window_set.size())
    {
        // Make sure all the windows are destroyed
        atexit(quit);
    }

    if (aWindowID == -1)
    {
        aWindowID = g_p_window_set.size();
    }

    // The window does not exist
    if (!g_p_window_set[aWindowID])
    {
    	SimpleGVXR::Window* p_window = 0;

        // This is a stack (LIFO) not a queue (FIFO)
        std::vector<std::string> backend_priority;

        if (renderer == "OPENGL")
        {
            backend_priority.push_back("GLX");
            backend_priority.push_back("EGL");
            backend_priority.push_back(renderer);
        }
        else if (renderer == "EGL")
        {
            backend_priority.push_back("OPENGL");
            backend_priority.push_back("GLX");
            backend_priority.push_back(renderer);
        }
        else if (renderer == "GLX")
        {
            backend_priority.push_back("OPENGL");
            backend_priority.push_back("EGL");
            backend_priority.push_back(renderer);
        }
        else if (renderer == "VULKAN")
        {
            backend_priority.push_back(renderer);
        }
        else
    	{
    		std::string error_message = "ERROR: Unknown renderer (";
    		error_message += aRenderer;
    		error_message += ")";

    		throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
    				error_message);
    	}

        while (backend_priority.size())
        {
            std::string backend = backend_priority.back();
            backend_priority.pop_back();

            if (backend == "OPENGL")
            {
                gVirtualXRay::LOGGER.logNow("Request an interactive OpenGL context") << std::endl;

                // Use try/catch to prevent crash when the backend fails.
                // It allows us to take advantage of the backend_priority stack.
                try
                {
                    // Set an error callback
                    glfwSetErrorCallback(errorCallback);

                    p_window = new SimpleGVXR::OpenGLWindow(aVisibilityFlag, aRendererMajorVersion, aRendererMinorVersion);
                }
                catch (const gVirtualXRay::Exception& err)
                {
                    std::string expected_message("Error: Cannot initialise GLFW.");

                    if (std::string(err.what()).substr(0, expected_message.size()) != expected_message)
                    {
                        throw gVirtualXRay::Exception(err.what());
                    }
                }
            }
            else if (backend == "EGL")
            {
                gVirtualXRay::LOGGER.logNow("Request a EGL context") << std::endl;

#ifdef HAS_EGL
        	    p_window = new SimpleGVXR::Windowless("EGL", aRendererMajorVersion, aRendererMinorVersion);
#else
                gVirtualXRay::LOGGER.logWarning("An EGL context was requested but it is not available on your platform") << std::endl;
#endif
            }
            else if (backend == "GLX")
            {
                gVirtualXRay::LOGGER.logNow("Request a GLX context") << std::endl;

#ifdef HAS_GLX
        	    p_window = new SimpleGVXR::Windowless("GLX", aRendererMajorVersion, aRendererMinorVersion);
#else
                gVirtualXRay::LOGGER.logWarning("A GLX context was requested but it is not available on your platform") << std::endl;
#endif
            }
            else if (backend == "VULKAN")
            {
                gVirtualXRay::LOGGER.logNow("Request a Vulkan context") << std::endl;
                p_window = new SimpleGVXR::VulkanWindow(aVisibilityFlag);
            }

            if (!p_window->getContext().isReady())
            {
                gVirtualXRay::LOGGER.logWarning("Could not create a context") << std::endl;
                delete p_window;
                p_window = 0;
            }
            else
            {
                backend_priority.clear();
            }
        }

        // The window has been created
        if (p_window)
        {
            // Add the window
            g_p_window_set[aWindowID] = p_window;

            // Register its ID
            g_p_window_id_stack.push_back(aWindowID);

            if (p_window->getContext().getWindowHandle())
            {
                glfwMakeContextCurrent(p_window->getContext().getWindowHandle());
            }

            // Initialise the X-ray renderer if needed and if possible
            gVirtualXRay::LOGGER.logNow("Initialise the X-ray renderer if needed and if possible") << std::endl;
            initialiseXRayRenderer();
        }
        // The window has not been created
        else
        {
            // Display a warning
            gVirtualXRay::LOGGER.logWarning("Window ") <<
                aWindowID <<
                " cannot be created." <<
                endl;

            // Delete the empty record that may have been created
            // in g_p_window_set
            std::map<int, SimpleGVXR::Window*>::iterator it;
            it = g_p_window_set.find(aWindowID);
            if (it != g_p_window_set.end())
                {
                g_p_window_set.erase(it);
                }
        }
    }
    // The window already exist
    // Do nothing
    else
    {
        gVirtualXRay::LOGGER.logWarning("Window ") <<
            aWindowID <<
            " already exists." <<
            endl;
    }
}


//-------------------------------------------------
void createNewContext(const std::string& aRenderer,
                      int aWindowID,
                		  int aVisibilityFlag,
                		  int aRendererMajorVersion,
                		  int aRendererMinorVersion)
//-------------------------------------------------
{
    createWindow(aWindowID,
        aVisibilityFlag,
        aRenderer,
        aRendererMajorVersion,
        aRendererMinorVersion);
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
            gVirtualXRay::LOGGER.logWarning("No active window to resize.") << endl;
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
        if (search != g_p_window_set.end())
            {
            if (search->second)
                {
                search->second->setWindowSize(aWidth, aHeight);
                }
            }
        else
            {
            gVirtualXRay::LOGGER.logWarning("Window") <<
                aWindowID <<
                " not found." <<
                endl;
            }
        }
}


//-------------------------------------------
std::vector<int> getWindowSize(int aWindowID)
//-------------------------------------------
{
    std::vector<int> return_value(2, 0);

    // Resize the last window that has been created
    if (aWindowID == -1)
        {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
            {
            gVirtualXRay::LOGGER.logWarning("No active window to resize.") << endl;
            }
        // Resize the last created window
        else
            {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
            }
        }

    // There is a window to process
    if (aWindowID != -1)
        {
        // Find the window
        auto search = g_p_window_set.find(aWindowID);

        // It has been found
        if (search != g_p_window_set.end())
            {
            if (search->second)
                {
                std::pair<int, int> window_size = search->second->getWindowSize();
                return_value[0] = window_size.first;
                return_value[1] = window_size.second;
                }
            }
        else
            {
            gVirtualXRay::LOGGER.logWarning("Window") <<
                aWindowID <<
                " not found." <<
                endl;
            }
        }

    return return_value;
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
            gVirtualXRay::LOGGER.logWarning("No active window to destroy.") << endl;
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
        if (search_window != g_p_window_set.end())
            {
            gVirtualXRay::LOGGER.logNow("Destroy window ") << aWindowID <<
                "(" << search_window->second << ")" << endl;

            if (search_window->second) search_window->second->destroyWindow();

            g_p_window_set.erase(search_window);
            }
        else
            {
            gVirtualXRay::LOGGER.logWarning("Window ") << aWindowID <<
                " not found in g_p_window_set." << endl;
            }

        // Unregister its ID from the stack of IDs
        // Find the ID
        std::vector<int>::iterator search_id = find(
            g_p_window_id_stack.begin(),
            g_p_window_id_stack.end(),
            aWindowID);

        // It has been found
        if (search_id != g_p_window_id_stack.end())
            {
            g_p_window_id_stack.erase(search_id);
            }
        else
            {
            gVirtualXRay::LOGGER.logWarning("Window ") << aWindowID <<
                " not found in g_p_window_id_stack." << endl;
            }
        }
}


//----------------------
void destroyAllWindows()
//----------------------
{
    gVirtualXRay::LOGGER.logNow("Destroy all the windows") << endl;

    // Process all the windows
    while (g_p_window_set.size())
    {
        // Destroy the window if possible
        destroyWindow(g_p_window_set.begin()->first);
    }
}


//----------------
void destroy(void)
//----------------
{
    destroyAllWindows();

    gVirtualXRay::LOGGER.logNow("Release all the memory") << endl;
    gVirtualXRay::ResourceManager::getInstance().releaseResources();

    g_xray_beam.clear();
    g_xray_detector.release();
    g_xray_renderer.release();
    g_display_shader.release();

    g_p_scene_graph = 0;
}


#ifndef _WIN32
void terminate(void)
{
    gVirtualXRay::LOGGER.logNow("The function `terminate` will be removed in version 2.0.10. Please use `destroy` from now on.") << endl;
    destroy();
}
#endif
