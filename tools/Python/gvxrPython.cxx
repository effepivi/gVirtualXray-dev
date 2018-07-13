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

// For Python
#include <boost/python.hpp>

// For Numpy
#include <boost/python/numpy.hpp>

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
namespace np = boost::python::numpy;


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
void setMonoChromatic(RATIONAL_NUMBER anEnergy, const char* aUnitOfEnergy, RATIONAL_NUMBER aNumberOfPhotons = 1);
void setSourcePosition(RATIONAL_NUMBER x, RATIONAL_NUMBER y, RATIONAL_NUMBER z, const char* aUnitOfLength);
void setDetectorPosition(RATIONAL_NUMBER x, RATIONAL_NUMBER y, RATIONAL_NUMBER z, const char* aUnitOfLength);
void setDetectorUpVector(RATIONAL_NUMBER x, RATIONAL_NUMBER y, RATIONAL_NUMBER z);
void setDetectorNumberOfPixels(int aWidth, int aHeight);
void setDetectorPixelSize(RATIONAL_NUMBER aWidth, RATIONAL_NUMBER aHeight, const char* aUnitOfLength);
void saveLastXRayImage(const char* aFileName = 0);
np::ndarray getNumpyImage();

void loadMeshFile(const char* aLabel,
        const char* aFileName,
        const char* aUnitOfLength);
void moveToCentre(const char* aLabel);
void setHU(const char* aLabel, RATIONAL_NUMBER aHU);
void setAttenuationCoefficient(const char* aLabel,
        RATIONAL_NUMBER anAttenuationCoefficient);

void initialiseXRayRenderer();

void quit();
GLFWwindow* initGLFW(int aWindowID);
void initGL();
void errorCallback(int error, const char* description);
RATIONAL_NUMBER getUnitOfLength(const char* aUnitOfLength);


//******************************************************************************
//  Global variables
//******************************************************************************
std::map<int, GLFWwindow*> g_p_window_set;
std::vector<int> g_p_window_id_stack;
std::map<string, PolygonMesh> g_p_polygon_mesh_set;

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;
Shader g_display_shader;

int g_file_counter = 0;
bool g_renderer_initialised = false;

bool g_beam_shape_initialised = false;
bool g_beam_energy_initialised = false;
bool g_source_position_initialised = false;
bool g_detector_position_initialised   = false;
bool g_detector_up_vector_initialised  = false;
bool g_detector_size_initialised       = false;
bool g_detector_pixel_size_initialised = false;





//******************************************************************************
//  Implementation
//******************************************************************************


BOOST_PYTHON_FUNCTION_OVERLOADS(createWindow_overloads, createWindow, 0, 1)
BOOST_PYTHON_FUNCTION_OVERLOADS(setWindowSize_overloads, setWindowSize, 2, 3)
BOOST_PYTHON_FUNCTION_OVERLOADS(setMonoChromatic_overloads, setMonoChromatic, 2, 3)
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
            
            // Initialise GLEW
            initialiseGLEW();


            // Initialise the X-ray renderer if needed and if possible
            initialiseXRayRenderer();
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
            cout << "gvxrStatus:\tDestroy window " << aWindowID << "(" << search_window->second << ")"<< endl;
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
        createWindow();
    }

    // The renderer is ready
    if (g_xray_renderer.isReady())
    {
        // Compute the X-ray image
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
    g_beam_shape_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}


//--------------------
void useParallelBeam()
//--------------------
{
    // Use a parallel source
    g_xray_detector.setParallelBeam();
    g_beam_shape_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}


//-----------------------------------------------------
void setMonoChromatic(RATIONAL_NUMBER anEnergy,
		              const char* aUnitOfEnergy,
					  RATIONAL_NUMBER aNumberOfPhotons)
//-----------------------------------------------------
{
    RATIONAL_NUMBER unit = 0;

    if (!strcmp(aUnitOfEnergy, "megaelectronvolt") ||
            !strcmp(aUnitOfEnergy, "MeV"))
    {
    unit = MeV;
    }
    else if (!strcmp(aUnitOfEnergy, "kiloelectronvolt") ||
            !strcmp(aUnitOfEnergy, "keV"))
    {
    unit = keV;
    }
    else if (!strcmp(aUnitOfEnergy, "electronvolt") ||
            !strcmp(aUnitOfEnergy, "eV"))
    {
    	unit = eV;
    }
    else
    {
		throw Exception(__FILE__, __FUNCTION__, __LINE__,
			"ERROR: Unknown unit of energy.");
    }

    g_xray_beam.clear();
    g_xray_beam.addChannel(aNumberOfPhotons, anEnergy * unit);
    g_beam_energy_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}


//-----------------------------------------------
void setSourcePosition(RATIONAL_NUMBER x, RATIONAL_NUMBER y, RATIONAL_NUMBER z, const char* aUnitOfLength)
//-----------------------------------------------
{
	RATIONAL_NUMBER unit_of_length = getUnitOfLength(aUnitOfLength);

	g_xray_detector.setXrayPointSource(VEC3(x * unit_of_length, y * unit_of_length, z * unit_of_length));
    g_source_position_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}


//-------------------------------------------------
void setDetectorPosition(RATIONAL_NUMBER x, RATIONAL_NUMBER y, RATIONAL_NUMBER z, const char* aUnitOfLength)
//-------------------------------------------------
{
	RATIONAL_NUMBER unit_of_length = getUnitOfLength(aUnitOfLength);

    g_xray_detector.setDetectorPosition(VEC3(x * unit_of_length, y * unit_of_length, z * unit_of_length));
    g_detector_position_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}


//-------------------------------------------------
void setDetectorUpVector(RATIONAL_NUMBER x, RATIONAL_NUMBER y, RATIONAL_NUMBER z)
//-------------------------------------------------
{
    g_xray_detector.setUpVector(VEC3(x, y, z).normal());
    g_detector_up_vector_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}


//-----------------------------------------------------
void setDetectorNumberOfPixels(int aWidth, int aHeight)
//-----------------------------------------------------
{
    g_xray_detector.setNumberOfPixels(Vec2ui(aWidth, aHeight));
    g_detector_size_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}


//--------------------------------------------------
void setDetectorPixelSize(RATIONAL_NUMBER aWidth,
						  RATIONAL_NUMBER aHeight,
						  const char* aUnitOfLength)
//--------------------------------------------------
{
	RATIONAL_NUMBER unit_of_length = getUnitOfLength(aUnitOfLength);
    g_xray_detector.setResolutionInUnitsOfLengthPerPixel(VEC2(aWidth * unit_of_length, aHeight * unit_of_length));
    g_detector_pixel_size_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
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


//------------------------------------------
void loadMeshFile(const char* aLabel,
                  const char* aFileName,
                  const char* aUnitOfLength)
//------------------------------------------
{
    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();

    // Set the file name    
    g_p_polygon_mesh_set[aLabel].setFileName(aFileName);
    
    // Load the file
    g_p_polygon_mesh_set[aLabel].loadSTLFile(false, true, false, false, getUnitOfLength(aUnitOfLength), GL_STATIC_DRAW);

    g_p_polygon_mesh_set[aLabel].setFileName("titi.stl");
    g_p_polygon_mesh_set[aLabel].writeSTLFile();

    // Add the geometry to the X-ray renderer
    g_xray_renderer.addInnerSurface(&g_p_polygon_mesh_set[aLabel]);
}


//-----------------------------------
void moveToCentre(const char* aLabel)
//-----------------------------------
{
    // Find the mesh
    auto search_mesh = g_p_polygon_mesh_set.find(aLabel);
    
    // It has been found
    if(search_mesh != g_p_polygon_mesh_set.end())
    {
        // Move the mesh to the centre
        g_p_polygon_mesh_set[aLabel].moveToCentre ();

        g_p_polygon_mesh_set[aLabel].setFileName("toto.stl");
        g_p_polygon_mesh_set[aLabel].writeSTLFile();

    }
    // It has not been found
    else
    {
        cerr << "gvxrWarning:\tPolygonMesh " << aLabel << " not found in g_p_polygon_mesh_set." << endl;
    }
}


//-------------------------------------------------
void setHU(const char* aLabel, RATIONAL_NUMBER aHU)
//-------------------------------------------------
{
    // Find the mesh
    auto search_mesh = g_p_polygon_mesh_set.find(aLabel);
    
    // It has been found
    if(search_mesh != g_p_polygon_mesh_set.end())
    {
        // Set its properties
        g_p_polygon_mesh_set[aLabel].setHounsfieldValue (aHU);
    }
    // It has not been found
    else
    {
        cerr << "gvxrWarning:\tPolygonMesh " << aLabel << " not found in g_p_polygon_mesh_set." << endl;
    }
}


//----------------------------------------------------------------------
void setAttenuationCoefficient(const char* aLabel,
                               RATIONAL_NUMBER anAttenuationCoefficient)
//----------------------------------------------------------------------
{
    // Find the mesh
    auto search_mesh = g_p_polygon_mesh_set.find(aLabel);
    
    // It has been found
    if(search_mesh != g_p_polygon_mesh_set.end())
    {
        // Set its properties
        g_p_polygon_mesh_set[aLabel].setAttenuationCoefficient (anAttenuationCoefficient);
    }
    // It has not been found
    else
    {
        cerr << "gvxrWarning:\tPolygonMesh " << aLabel << " not found in g_p_polygon_mesh_set." << endl;
    }
}


//---------------------------
void initialiseXRayRenderer()
//---------------------------
{
	if (g_beam_shape_initialised &&
			g_beam_energy_initialised &&
			g_source_position_initialised &&
			g_detector_position_initialised &&
			g_detector_up_vector_initialised &&
			g_detector_size_initialised &&
			g_detector_pixel_size_initialised)
	{
        // The renderer is not initialised
        if (!g_renderer_initialised)
        {
        	cout << "INIT RENDER" << endl;
            // Initialise the renderer
            g_xray_renderer.initialise(XRayRenderer::OPENGL,
                GL_RGB32F,
                &g_xray_detector,
                &g_xray_beam);

    		g_renderer_initialised = true;
        }
	}
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


//--------------------------------------------------------
RATIONAL_NUMBER getUnitOfLength(const char* aUnitOfLength)
//--------------------------------------------------------
{
	RATIONAL_NUMBER unit_of_length = 0;

	if (!strcmp(aUnitOfLength, "kilometer") ||
			!strcmp(aUnitOfLength, "kilometre") ||
			!strcmp(aUnitOfLength, "km"))
	{
		unit_of_length = km;
	}
	else if (!strcmp(aUnitOfLength, "hectometer") ||
			!strcmp(aUnitOfLength, "hectometre") ||
			!strcmp(aUnitOfLength, "hm"))
	{
		unit_of_length = hm;
	}
	else if (!strcmp(aUnitOfLength, "decameter") ||
			!strcmp(aUnitOfLength, "decametre") ||
			!strcmp(aUnitOfLength, "dam"))
	{
		unit_of_length = dam;
	}
	else if (!strcmp(aUnitOfLength, "meter") ||
			!strcmp(aUnitOfLength, "metre") ||
			!strcmp(aUnitOfLength, "m"))
	{
		unit_of_length = m;
	}
	else if (!strcmp(aUnitOfLength, "decimeter") ||
			!strcmp(aUnitOfLength, "decimetre") ||
			!strcmp(aUnitOfLength, "dm"))
	{
		unit_of_length = dm;
	}
	else if (!strcmp(aUnitOfLength, "centimeter") ||
			!strcmp(aUnitOfLength, "centimeter") ||
			!strcmp(aUnitOfLength, "cm"))
	{
		unit_of_length = cm;
	}
	else if (!strcmp(aUnitOfLength, "millimeter") ||
			!strcmp(aUnitOfLength, "millimeter") ||
			!strcmp(aUnitOfLength, "mm"))
	{
		unit_of_length = mm;
	}
	else if (!strcmp(aUnitOfLength, "micrometer") ||
			!strcmp(aUnitOfLength, "micrometer") ||
			!strcmp(aUnitOfLength, "um"))
	{
		unit_of_length = um;
	}
	else
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__,
			"ERROR: Unknown unit of length.");
	}

	return (unit_of_length);
}


np::ndarray getNumpyImage()
{
	boost::python::object own;
	boost::python::tuple stride = boost::python::make_tuple(sizeof(RATIONAL_NUMBER)*g_xray_detector.getNumberOfPixels().getX(),sizeof(RATIONAL_NUMBER));


    boost::python::tuple shape = boost::python::make_tuple(g_xray_detector.getNumberOfPixels().getY(), g_xray_detector.getNumberOfPixels().getX());
	np::dtype dtype = np::dtype::get_builtin<RATIONAL_NUMBER>();
	//np::ndarray a = np::zeros(shape, dtype);


	np::ndarray a = np::from_data(g_xray_renderer.getEnergyFluenceRawData(), dtype, shape, stride,own);



	//std::cout << a(0,0) << endl;
	return a;
}


BOOST_PYTHON_MODULE(gvxrPython)
{

    cout << "gvxrStatus:\tInitialise gvxrPython" << endl;

    // Initialise Python
    Py_Initialize();

    // Initialise Numpy
    np::initialize();

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
    boost::python::def("createWindow",  createWindow,  createWindow_overloads());
    boost::python::def("setWindowSize", setWindowSize, setWindowSize_overloads());

    boost::python::def("destroyWindow",     destroyWindow);
    boost::python::def("destroyAllWindows", destroyAllWindows);

    boost::python::def("computeXRayImage", computeXRayImage);
    
    boost::python::def("usePointSource",  usePointSource);
    boost::python::def("useParallelBeam", useParallelBeam);

    boost::python::def("setMonoChromatic", setMonoChromatic, setMonoChromatic_overloads());
           
    boost::python::def("setSourcePosition",         setSourcePosition);
    boost::python::def("setDetectorPosition",       setDetectorPosition);
    boost::python::def("setDetectorUpVector",       setDetectorUpVector);
    boost::python::def("setDetectorNumberOfPixels", setDetectorNumberOfPixels);
    boost::python::def("setDetectorPixelSize",      setDetectorPixelSize);

    boost::python::def("loadMeshFile",              loadMeshFile);
    boost::python::def("moveToCenter",              moveToCentre);
    boost::python::def("moveToCentre",              moveToCentre);
    boost::python::def("setHU",                     setHU);
    boost::python::def("setAttenuationCoefficient", setAttenuationCoefficient);
        
    boost::python::def("getNumpyImage", getNumpyImage);
    boost::python::def("saveLastXRayImage", saveLastXRayImage, saveLastXRayImage_overloads());
}

