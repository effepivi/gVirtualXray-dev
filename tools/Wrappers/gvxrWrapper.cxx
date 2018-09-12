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
#include <memory>

#include <Python.h>
// For Python
#include <boost/python.hpp>

// For Numpy
#include <boost/python/numpy.hpp>

// To perform the computations of the X-ray simulation
#ifndef __XRayRenderer_h
#include "gVirtualXRay/XRayRenderer.h"
#endif

// To load a scenegraph
#ifndef __SceneGraphBinder_h
#include "gVirtualXRay/SceneGraphBinder.h"
#endif

// To load a scenegraph using ASSIMP
#ifndef __AssimpSceneGraphBinder_h
#include "gVirtualXRay/AssimpSceneGraphBinder.h"
#endif

// To load a scenegraph using OpenSceneGraph
#ifndef __OpenSceneGraphBinder_h
#include "gVirtualXRay/OpenSceneGraphBinder.h"
#endif

#ifndef __CubeMesh_h
#include "gVirtualXRay/CubeMesh.h"
#endif

#ifndef __CylinderMesh_h
#include "gVirtualXRay/CylinderMesh.h"
#endif

// Shaders for rendering
#include "display_gl2.frag.h"
#include "display_gl2.vert.h"
#include "display_gl3.frag.h"
#include "display_gl3.vert.h"






/*
// For Python
#include <boost/python.hpp>

// For Numpy
#include <boost/python/numpy.hpp>

// For tuples
#include <boost/python/tuple.hpp>

// For operators
#include <boost/python/operators.hpp>

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


// To initialise GLEW, to define matrix stacks, to define state stacks, etc.
#ifndef __OpenGLUtilities_h
#include "gVirtualXRay/OpenGLUtilities.h"
#endif



*/
using namespace std;
using namespace gVirtualXRay;
namespace np = boost::python::numpy;





#ifndef __gvxrWrapper_h
#include "gvxrWrapper.h"
#endif

GLFWwindow* initGLFW(int aWindowID, bool aVisibilityFlag);
void framebufferSizeCallback(GLFWwindow* apWindow, int aWidth, int aHeight);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseButtonCallback(GLFWwindow* apWindow, int aButton, int aButtonState, int aModifierKey);
void cursorPosCallback(GLFWwindow* apWindow, double x, double y);
void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset);

std::map<int, GLFWwindow*> g_p_window_set;
std::vector<int> g_p_window_id_stack;

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;


int g_file_counter = 0;
bool g_renderer_initialised = false;

bool g_beam_shape_initialised = false;
bool g_beam_energy_initialised = false;
bool g_source_position_initialised = false;
bool g_detector_position_initialised   = false;
bool g_detector_up_vector_initialised  = false;
bool g_detector_size_initialised       = false;
bool g_detector_pixel_size_initialised = false;





const double g_initial_fovy(45);                              // Field of view along the y-axis
double g_initial_near(5.0 * cm);                        // Near clipping plane
double g_initial_far(500.0 * cm);                       // Far clipping plane

GLfloat g_zoom(120.0 * cm);

int g_button(-1);
int g_button_state(-1);
bool g_use_arc_ball(false);
GLint g_last_x_position(0);
GLint g_last_y_position(0);
GLint g_current_x_position(0);
GLint g_current_y_position(0);

bool g_display_beam(true);
bool g_display_wireframe(false);
bool g_use_lighing(true);
bool g_display_detector(true);
bool g_display_normal_vectors(false);

Matrix4x4<GLfloat> g_scene_rotation_matrix;
Matrix4x4<GLfloat> g_detector_rotation_matrix;
Matrix4x4<GLfloat> g_sample_rotation_matrix;


Shader g_display_shader;

bool g_run_loop = false;
bool g_viewer_initialised = false;
const GLfloat g_rotation_speed(2.0);

//AssimpSceneGraphBinder g_assimp_scenegraph;
//OpenSceneGraphBinder g_open_scenegraph;

std::shared_ptr<SceneGraphBinder> g_p_scene_graph = 0;




class PyPolygonMesh//: public boost::python::object
{
public:
	PyPolygonMesh();
	PyPolygonMesh(const PolygonMesh& aMesh);


	//void setMaterial(const boost::python::list& aMaterial);

	void setDensity(double aDensity, const char* aUnitOfDensity);
	double getDensity();

	double getMassAttenuationCoefficient(double anEnergy, const char* aUnitOfEnergy) const;
	double getLinearAttenuationCoefficient(double anEnergy, const char* aUnitOfEnergy) const;

	void setMassAttenuationCoefficient(double aMu, const char* aUnit);
	void setLinearAttenuationCoefficient(double aMu, const char* aUnit);

	const PolygonMesh& getMesh() const;
	PolygonMesh& getMesh();

	void translate(double x, double y, double z, const char* aUnit);
	void scale(double x, double y, double z);
	void rotate(double x, double y, double z, double aRotationAngleInDegrees);

	void saveSTLFile(const std::string& aFileName);

	PyPolygonMesh& operator=(const PyPolygonMesh& aMesh);

	PyPolygonMesh operator+(const PyPolygonMesh& aMesh) const;
	PyPolygonMesh operator-(const PyPolygonMesh& aMesh) const;

	PyPolygonMesh& operator+=(const PyPolygonMesh& aMesh);
	PyPolygonMesh& operator-=(const PyPolygonMesh& aMesh);

private:
	PolygonMesh m_polygon_mesh;
};


/*
BOOST_PYTHON_FUNCTION_OVERLOADS(createWindow_overloads,      createWindow,      0, 2)
BOOST_PYTHON_FUNCTION_OVERLOADS(createOpenGLContext_overloads,      createOpenGLContext,      0, 1)
BOOST_PYTHON_FUNCTION_OVERLOADS(setWindowSize_overloads,     setWindowSize,     2, 3)
BOOST_PYTHON_FUNCTION_OVERLOADS(setMonoChromatic_overloads,  setMonoChromatic,  2, 3)
BOOST_PYTHON_FUNCTION_OVERLOADS(saveLastXRayImage_overloads, saveLastXRayImage, 0, 1)
BOOST_PYTHON_FUNCTION_OVERLOADS(displayScene_overloads,      displayScene,      0, 1)
BOOST_PYTHON_FUNCTION_OVERLOADS(renderLoop_overloads,        renderLoop,        0, 2)
BOOST_PYTHON_FUNCTION_OVERLOADS(moveToCentre_overloads,      moveToCentre,      0, 1)
BOOST_PYTHON_FUNCTION_OVERLOADS(makeCube_overloads,          makeCube,          0, 2)
BOOST_PYTHON_FUNCTION_OVERLOADS(makeCylinder_overloads,      makeCylinder,      0, 4)
*/

PyPolygonMesh::PyPolygonMesh()
{}


PyPolygonMesh::PyPolygonMesh(const PolygonMesh& aMesh):
		//boost::python::object(),
		m_polygon_mesh(aMesh)
{}


/*void PyPolygonMesh::setMaterial(const boost::python::list& aMaterial)
{
	TissueMaterial gvxr_material;
	gvxr_material.reset();

    for (int i = 0; i < boost::python::len(aMaterial); ++i)
    {
    	// Get the tuple from the list
    	boost::python::tuple material_compound = boost::python::extract<boost::python::tuple>(aMaterial[i]);

    	// Extract the data from the tuple
    	std::string material = boost::python::extract<std::string>(material_compound[0]);
    	double quantity = boost::python::extract<double>(material_compound[1]);

    	// Add compound to gvxr_material
    	gvxr_material.addElement(material, quantity);
    }

    // Set the material
    m_polygon_mesh.setTissueMaterial(gvxr_material);
}*/


void PyPolygonMesh::setDensity(double aDensity, const char* aUnitOfDensity)
{
    if (strcmp(aUnitOfDensity, "g.cm-3") &&
            strcmp(aUnitOfDensity, "g/cm3") &&
            strcmp(aUnitOfDensity, "g / cm3"))
    {
		throw Exception(__FILE__, __FUNCTION__, __LINE__,
			"ERROR: Unknown unit of density.");
    }

    m_polygon_mesh.setTissueDensity(aDensity * g / cm3);
}


double PyPolygonMesh::getDensity()
{
	return (m_polygon_mesh.getTissueDensity() / (g / cm3));
}

void PyPolygonMesh::setMassAttenuationCoefficient(double aMu, const char* aUnit)
{
		throw Exception(__FILE__, __FUNCTION__, __LINE__,
			"TODO.");
}


double PyPolygonMesh::getMassAttenuationCoefficient(double anEnergy,
		                                            const char* aUnitOfEnergy) const
{
	return (m_polygon_mesh.getTissueMaterial().getMassAttenuationTotal(anEnergy * getUnitOfEnergy(aUnitOfEnergy)) / (cm2/g));
}


double PyPolygonMesh::getLinearAttenuationCoefficient(double anEnergy,
		                                              const char* aUnitOfEnergy) const
{
	return (m_polygon_mesh.getTissueMaterial().getMU(anEnergy * getUnitOfEnergy(aUnitOfEnergy)) * cm);
}



void PyPolygonMesh::setLinearAttenuationCoefficient(double aMu, const char* aUnit)
{
    if (strcmp(aUnit, "cm-1"))
    {
		throw Exception(__FILE__, __FUNCTION__, __LINE__,
			"ERROR: Unknown unit of linear attenuation coefficient.");
    }

	m_polygon_mesh.setAttenuationCoefficient(aMu / cm);
}


const PolygonMesh& PyPolygonMesh::getMesh() const
{
	return (m_polygon_mesh);
}


PolygonMesh& PyPolygonMesh::getMesh()
{
	return (m_polygon_mesh);
}


void PyPolygonMesh::translate(double x, double y, double z, const char* aUnit)
{
	double unit = getUnitOfLength(aUnit);
	m_polygon_mesh.translate(VEC3(x * unit, y * unit, z * unit));
}


void PyPolygonMesh::scale(double x, double y, double z)
{
	m_polygon_mesh.scale(x, y, z);
}


void PyPolygonMesh::rotate(double x, double y, double z, double aRotationAngleInDegrees)
{
	m_polygon_mesh.rotate(VEC3(x, y, z), aRotationAngleInDegrees);
}


//-----------------------------------------------------------
void PyPolygonMesh::saveSTLFile(const std::string& aFileName)
//-----------------------------------------------------------
{
	m_polygon_mesh.setFileName(aFileName);
	m_polygon_mesh.writeSTLFile();
}


PyPolygonMesh& PyPolygonMesh::operator=(const PyPolygonMesh& aMesh)
{
	m_polygon_mesh = aMesh.m_polygon_mesh;


	return (*this);
}


PyPolygonMesh PyPolygonMesh::operator+(const PyPolygonMesh& aMesh) const
{
	PyPolygonMesh new_mesh = *this;
	new_mesh.m_polygon_mesh += aMesh.m_polygon_mesh;

	return (new_mesh);
}


PyPolygonMesh PyPolygonMesh::operator-(const PyPolygonMesh& aMesh) const
{
	PyPolygonMesh new_mesh = *this;
	new_mesh.m_polygon_mesh -= aMesh.m_polygon_mesh;

	return (new_mesh);
}


PyPolygonMesh& PyPolygonMesh::operator+=(const PyPolygonMesh& aMesh)
{
	m_polygon_mesh += aMesh.m_polygon_mesh;

	return (*this);
}


PyPolygonMesh& PyPolygonMesh::operator-=(const PyPolygonMesh& aMesh)
{
	m_polygon_mesh -= aMesh.m_polygon_mesh;

	return (*this);
}


PyPolygonMesh emptyMesh()
{
	return (PyPolygonMesh());
}


//-------------------------------------
void createWindow(int aWindowID,
                  bool aVisibilityFlag)
//-------------------------------------
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
        GLFWwindow* p_window = initGLFW(aWindowID, aVisibilityFlag);

        // The window has been created
        if (p_window)
        {
            // Add the window
            g_p_window_set[aWindowID] = p_window;

            // Register its ID
            g_p_window_id_stack.push_back(aWindowID);
            
            // Initialise GLEW
            initialiseGLEW();

            // Initialise OpenGL
            initGL();

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


//-------------------------------------
void createOpenGLContext(int aWindowID)
//-------------------------------------
{
    createWindow(aWindowID, false);
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
            if (search->second)
			{
            	glfwSetWindowSize(search->second, aWidth, aHeight);
                framebufferSizeCallback(search->second, aWidth, aHeight);
			}
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
    while (g_p_window_set.size())
    {
        // Destroy the window if possible
        destroyWindow(g_p_window_set.begin()->first);
    }

    // Terminate GLFW
    //glfwTerminate();    
}

/*
//----------------------------
np::ndarray computeXRayImage()
//----------------------------
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
        g_xray_renderer.computeImage(g_sample_rotation_matrix);
    }
    // The renderer is not ready
    else
    {
        cerr << "gvxrWarning:\tX-ray renderer not ready." << endl;
    }

	return (getNumpyImage(g_xray_renderer.getEnergyFluence()));
}


//--------------------------------------------
np::ndarray computeLBuffer(const char* aLabel)
//--------------------------------------------
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
        if (g_p_scene_graph.get())
        {
            // Find the mesh
            auto search_mesh = g_p_scene_graph->getPolygonMeshSet().find(aLabel);
            
            // It has been found
            if(search_mesh != g_p_scene_graph->getPolygonMeshSet().end())
            {
                // Compute the L-buffer
                g_xray_renderer.computeLBuffer(search_mesh->second, VEC3(), g_sample_rotation_matrix, false);
            }
            // It has not been found
            else
            {
                cerr << "gvxrWarning:\tPolygonMesh " << aLabel << " not found in g_p_polygon_mesh_set." << endl;
            }
        }
        else
        {
            cerr << "gvxrWarning:\tThe scenegraph is empty." << endl;
        }
    }
    // The renderer is not ready
    else
    {
        cerr << "gvxrWarning:\tX-ray renderer not ready." << endl;
    }
    
    //g_xray_renderer.printFBO(XRAY_DETECTOR_UNCLEANED_LBUFFER_FBO_ID, "uncleaned_lbuffer.mha");
    //g_xray_renderer.printFBO(XRAY_DETECTOR_CLEANED_LBUFFER_FBO_ID, "cleaned_lbuffer.mha");
     //g_xray_renderer.printFBO(XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID, "cumulated_l-buffers.mha");
    // g_xray_renderer.printFBO(XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID, "energy_fluence.mha");
    
    // Compute its L-buffer
    return (getNumpyImage(g_xray_renderer.getLBuffer()));
}


np::ndarray computeSinogram(double x, double y, double z, const char* aUnitOfLength,
        unsigned int aNumberOfAngle,
        double anAngleOffset)
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

    	double unit_of_length = getUnitOfLength(aUnitOfLength);
    	VEC3 rotation_centre(VEC3(x * unit_of_length, y * unit_of_length, z * unit_of_length));

        g_xray_renderer.computeSinogram(g_sample_rotation_matrix,
                rotation_centre,
                aNumberOfAngle,
                anAngleOffset);
    }
    // The renderer is not ready
    else
    {
        cerr << "gvxrWarning:\tX-ray renderer not ready." << endl;
    }

	return (getNumpyImage(g_xray_renderer.getSinogram()));
}
*/

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


//----------------------
void useParallelSource()
//----------------------
{
    useParallelBeam();
}


//----------------------
void resetBeamSpectrum()
//----------------------
{
    g_xray_beam.clear();
    g_beam_energy_initialised = false;
}

/*
//-----------------------------------------------------
void setMonoChromatic(double anEnergy,
		              const char* aUnitOfEnergy,
					  double aNumberOfPhotons)
//-----------------------------------------------------
{
    g_xray_beam.clear();
    g_xray_beam.addChannel(aNumberOfPhotons, anEnergy * getUnitOfEnergy(aUnitOfEnergy));
    g_beam_energy_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}
*/

//-----------------------------------------------------------
void addEnergyBinToSpectrum(double anEnergy,
							const char* aUnitOfEnergy,
							double aNumberOfPhotons)
//-----------------------------------------------------------
{
    g_xray_beam.addChannel(aNumberOfPhotons, anEnergy * getUnitOfEnergy(aUnitOfEnergy));
    g_beam_energy_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}


//-----------------------------------------------
void setSourcePosition(double x, double y, double z, const char* aUnitOfLength)
//-----------------------------------------------
{
	double unit_of_length = getUnitOfLength(aUnitOfLength);

	g_xray_detector.setXrayPointSource(VEC3(x * unit_of_length, y * unit_of_length, z * unit_of_length));
    g_source_position_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
    initViewer();
}


//-------------------------------------------------
void setDetectorPosition(double x, double y, double z, const char* aUnitOfLength)
//-------------------------------------------------
{
	double unit_of_length = getUnitOfLength(aUnitOfLength);

    g_xray_detector.setDetectorPosition(VEC3(x * unit_of_length, y * unit_of_length, z * unit_of_length));
    g_detector_position_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
    initViewer();
}


//-------------------------------------------------
void setDetectorUpVector(double x, double y, double z)
//-------------------------------------------------
{
    g_xray_detector.setUpVector(VEC3(x, y, z).normal());
    g_detector_up_vector_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
    initViewer();
}


//-----------------------------------------------------
void setDetectorNumberOfPixels(int aWidth, int aHeight)
//-----------------------------------------------------
{
    g_xray_detector.setNumberOfPixels(Vec2ui(aWidth, aHeight));
    g_detector_size_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    g_renderer_initialised = false;
    initialiseXRayRenderer();
}


//--------------------------------------------------
void setDetectorPixelSize(double aWidth,
						  double aHeight,
						  const char* aUnitOfLength)
//--------------------------------------------------
{
	double unit_of_length = getUnitOfLength(aUnitOfLength);
    g_xray_detector.setResolutionInUnitsOfLengthPerPixel(VEC2(aWidth * unit_of_length, aHeight * unit_of_length));
    g_detector_pixel_size_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}


//------------------------------------------
void loadMeshFile(const char* aLabel,
                  const char* aFileName,
                  const char* aUnitOfLength)
//------------------------------------------
{
    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();

    SceneGraphBinder* p_temp = g_p_scene_graph.get();
    if (!p_temp)
    {
    	g_p_scene_graph.reset(new SceneGraphBinder());
    }

    // Set the file name
    g_p_scene_graph->getPolygonMeshSet()[aLabel].setFileName(aFileName);
    
    // Load the file
    g_p_scene_graph->getPolygonMeshSet()[aLabel].loadSTLFile(false, true, true, true, getUnitOfLength(aUnitOfLength), GL_STATIC_DRAW);

    // Add the geometry to the X-ray renderer
    g_xray_renderer.addInnerSurface(&g_p_scene_graph->getPolygonMeshSet()[aLabel]);
}


//--------------------------------------------
void loadSceneGraph(const char* aFileName,
        			const char* aUnitOfLength)
//--------------------------------------------
{
	bool scene_graph_loaded = false;

#ifdef HAS_ASSIMP
	if (!scene_graph_loaded)
	{
		g_p_scene_graph.reset(new AssimpSceneGraphBinder(aFileName, getUnitOfLength(aUnitOfLength)));
		scene_graph_loaded = true;
	}
#endif

#ifdef HAS_OPENSCENEGRAPH
	if (!scene_graph_loaded)
	{
		g_p_scene_graph.reset(new OpenSceneGraphBinder(aFileName, getUnitOfLength(aUnitOfLength)));
		scene_graph_loaded = true;
	}
#endif

	if (!scene_graph_loaded)
	{
        cerr << "gvxrWarning:\tGraphScene " << aFileName << " not loaded." << endl;
	}
    // Add the geometry to the X-ray renderer
	else
	{
		// Look for all the meshes in the scenegraph
		for (std::map<string, PolygonMesh>::const_iterator ite = g_p_scene_graph->getPolygonMeshSet().begin();
				ite != g_p_scene_graph->getPolygonMeshSet().end();
				++ite)
		{
			g_xray_renderer.addInnerSurface(&ite->second);
		}
	}
}


//-----------------------------------
void moveToCentre(const char* aLabel)
//-----------------------------------
{
	if (g_p_scene_graph.get())
	{
		// Find the mesh
		if (aLabel)
		{
			auto search_mesh = g_p_scene_graph->getPolygonMeshSet().find(aLabel);

			// It has been found
			if(search_mesh != g_p_scene_graph->getPolygonMeshSet().end())
			{
				// Move the mesh to the centre
				g_p_scene_graph->getPolygonMeshSet()[aLabel].moveToCentre ();
			}
			// It has not been found
			else
			{
				cerr << "gvxrWarning:\tPolygonMesh " << aLabel << " not found in g_p_polygon_mesh_set." << endl;
			}
		}
		// Move the scenegraph
		else
		{
			g_p_scene_graph->moveToCentre();
			g_p_scene_graph->updateMatrices();
		}
	}
	else
	{
		cerr << "gvxrWarning:\tThe scenegraph is empty." << endl;
	}
}


//---------------------------------------------
void addPolygonMeshAsInnerSurface(const PyPolygonMesh& aMesh, const std::string& aLabel)
//---------------------------------------------
{
	// Add the polygon mesh to the X-ray renderer
	g_xray_renderer.addInnerSurface(&aMesh.getMesh());

	// The scenegraph does not exist
	if (!g_p_scene_graph.get())
	{
		// Create a scenegraph
    	g_p_scene_graph.reset(new SceneGraphBinder());
	}

	// Add the polygon mesh to the scenegraph
	g_p_scene_graph->getPolygonMeshSet()[aLabel] = aMesh.getMesh();
}


//---------------------------------------------
void addPolygonMeshAsOuterSurface(const PyPolygonMesh& aMesh, const std::string& aLabel)
//---------------------------------------------
{
	// Add the polygon mesh to the X-ray renderer
	g_xray_renderer.addOuterSurface(&aMesh.getMesh());

	// The scenegraph does not exist
	if (!g_p_scene_graph.get())
	{
		// Create a scenegraph
    	g_p_scene_graph.reset(new SceneGraphBinder());
	}

	// Add the polygon mesh to the scenegraph
	g_p_scene_graph->getPolygonMeshSet()[aLabel] = aMesh.getMesh();
}


//------------------------
void removePolygonMeshes()
//------------------------
{
	// The scenegraph exists
	if (g_p_scene_graph.get())
	{
		// Empty it
		g_p_scene_graph->clear();
	}

	g_xray_renderer.removeInnerSurfaces();
	g_xray_renderer.removeOuterSurface();
}

/*
MATRIX4 Numpy2gvxrMatrix(const np::ndarray& aNumpyMatrix)
{
	if (aNumpyMatrix.shape(0) != 16 ||
			aNumpyMatrix.shape(1) != 1 ||
			aNumpyMatrix.get_dtype() != np::dtype::get_builtin<double>())
	{
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Incompatible type.");

	}

	return MATRIX4((float*)aNumpyMatrix.get_data());
}


np::ndarray gvxrMatrix2Numpy(const MATRIX4& aMatrix4)
{
	boost::python::object own;
	boost::python::tuple stride = boost::python::make_tuple(sizeof(float)*16,sizeof(float));
    boost::python::tuple shape = boost::python::make_tuple(16, 1);
	np::dtype dtype = np::dtype::get_builtin<double>();

	return np::ndarray(np::from_data(aMatrix4.get4x4(), dtype, shape, stride,own));
}


np::ndarray getSceneTransformationMatrix()
{
	return gvxrMatrix2Numpy(g_sample_rotation_matrix);
}


np::ndarray getRootTransformationMatrix()
{
	if (!g_p_scene_graph.get())
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "The scenegraph is empty.");
	}

	return (gvxrMatrix2Numpy(g_p_scene_graph->getRootNodeTransformationMatrix()));
}


np::ndarray getNodeTransformationMatrix(const char* aLabel)
{
	if (!g_p_scene_graph.get())
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "The scenegraph is empty.");
	}

	if (g_p_scene_graph.get())
	{
		// Find the mesh
		auto search_mesh = g_p_scene_graph->getPolygonMeshSet().find(aLabel);

		// It has not been found
		if(search_mesh == g_p_scene_graph->getPolygonMeshSet().end())
		{
			throw Exception(__FILE__, __FUNCTION__, __LINE__, string("The PolygonMesh ") + string(aLabel) + string(" not found in g_p_polygon_mesh_set."));
		}
	}

	return (gvxrMatrix2Numpy(g_p_scene_graph->getNodeTransformationMatrix(aLabel)));
}


void setSceneTransformationMatrix(const np::ndarray& aMatrix)
{
	g_sample_rotation_matrix = Numpy2gvxrMatrix(aMatrix);
}


void setRootTransformationMatrix(const np::ndarray& aMatrix)
{
	if (g_p_scene_graph.get())
	{
		g_p_scene_graph->setRootNodeTransformationMatrix(Numpy2gvxrMatrix(aMatrix));
	}
	else
	{
		cerr << "gvxrWarning:\tThe scenegraph is empty." << endl;
	}
}


void setNodeTransformationMatrix(const char* aLabel, const np::ndarray& aMatrix)
{
	if (g_p_scene_graph.get())
	{
		// Find the mesh
		auto search_mesh = g_p_scene_graph->getPolygonMeshSet().find(aLabel);

		// It has been found
		if(search_mesh != g_p_scene_graph->getPolygonMeshSet().end())
		{
			g_p_scene_graph->setNodeTransformationMatrix(aLabel, Numpy2gvxrMatrix(aMatrix));
		}
		// It has not been found
		else
		{
			cerr << "gvxrWarning:\tPolygonMesh " << aLabel << " not found in g_p_polygon_mesh_set." << endl;
		}
	}
	else
	{
		cerr << "gvxrWarning:\tThe scenegraph is empty." << endl;
	}
}
*/

void scaleNode(const char* aLabel, double x, double y, double z, const char* aUnitOfLength)
{
	if (g_p_scene_graph.get())
	{
		// Find the mesh
		auto search_mesh = g_p_scene_graph->getPolygonMeshSet().find(aLabel);

		// It has been found
		if(search_mesh != g_p_scene_graph->getPolygonMeshSet().end())
		{
			double unit_of_length = getUnitOfLength(aUnitOfLength);
			g_p_scene_graph->scale(aLabel, VEC3(x * unit_of_length, y * unit_of_length, z * unit_of_length));
			g_p_scene_graph->updateMatrices();
		}
		// It has not been found
		else
		{
			cerr << "gvxrWarning:\tPolygonMesh " << aLabel << " not found in g_p_polygon_mesh_set." << endl;
		}
	}
	else
	{
		cerr << "gvxrWarning:\tThe scenegraph is empty." << endl;
	}
}


void rotateNode(const char* aLabel, double angle, double x, double y, double z)
{
	if (g_p_scene_graph.get())
	{
		// Find the mesh
		auto search_mesh = g_p_scene_graph->getPolygonMeshSet().find(aLabel);

		// It has been found
		if(search_mesh != g_p_scene_graph->getPolygonMeshSet().end())
		{
			g_p_scene_graph->rotate(aLabel, angle, VEC3(x, y, z));
			g_p_scene_graph->updateMatrices();
		}
		// It has not been found
		else
		{
			cerr << "gvxrWarning:\tPolygonMesh " << aLabel << " not found in g_p_polygon_mesh_set." << endl;
		}
	}
	else
	{
		cerr << "gvxrWarning:\tThe scenegraph is empty." << endl;
	}
}


void translateNode(const char* aLabel, double x, double y, double z, const char* aUnitOfLength)
{
	if (g_p_scene_graph.get())
	{
		// Find the mesh
		auto search_mesh = g_p_scene_graph->getPolygonMeshSet().find(aLabel);

		// It has been found
		if(search_mesh != g_p_scene_graph->getPolygonMeshSet().end())
		{
			double unit_of_length = getUnitOfLength(aUnitOfLength);
			g_p_scene_graph->translate(aLabel, VEC3(x * unit_of_length, y * unit_of_length, z * unit_of_length));
			g_p_scene_graph->updateMatrices();
		}
		// It has not been found
		else
		{
			cerr << "gvxrWarning:\tPolygonMesh " << aLabel << " not found in g_p_polygon_mesh_set." << endl;
		}
	}
	else
	{
		cerr << "gvxrWarning:\tThe scenegraph is empty." << endl;
	}
}



void resetSceneTransformation()
{
	g_sample_rotation_matrix = MATRIX4();
}


void scaleScene(double x, double y, double z, const char* aUnitOfLength)
{
	// Move the scenegraph
	double unit_of_length = getUnitOfLength(aUnitOfLength);
	g_sample_rotation_matrix *= MATRIX4::buildScaleMatrix(x * unit_of_length, y * unit_of_length, z * unit_of_length);
}


void rotateScene(double angle, double x, double y, double z)
{
	// Move the scenegraph
	g_sample_rotation_matrix *= MATRIX4::buildRotationMatrix(angle, x, y, z);
}


void translateScene(double x, double y, double z, const char* aUnitOfLength)
{
	// Move the scenegraph
	double unit_of_length = getUnitOfLength(aUnitOfLength);
	g_sample_rotation_matrix *= MATRIX4::buildTranslationMatrix(x * unit_of_length, y * unit_of_length, z * unit_of_length);
}


//-------------------------------------------------
void setHU(const char* aLabel, double aHU)
//-------------------------------------------------
{
    // Find the mesh
    auto search_mesh = g_p_scene_graph->getPolygonMeshSet().find(aLabel);
    
    // It has been found
    if(search_mesh != g_p_scene_graph->getPolygonMeshSet().end())
    {
        // Set its properties
    	g_p_scene_graph->getPolygonMeshSet()[aLabel].setHounsfieldValue (aHU);
    }
    // It has not been found
    else
    {
        cerr << "gvxrWarning:\tPolygonMesh " << aLabel << " not found in g_p_polygon_mesh_set." << endl;
    }
}

void setMassAttenuationCoefficient(const char* aLabel,
                               double anAttenuationCoefficient)
{
    throw Exception(__FILE__, __FUNCTION__, __LINE__, "TODO: mass vs liner attenuation coefficient");
}


//----------------------------------------------------------------------
void setLinearAttenuationCoefficient(const char* aLabel,
                               double anAttenuationCoefficient)
//----------------------------------------------------------------------
{
    throw Exception(__FILE__, __FUNCTION__, __LINE__, "TODO: mass vs liner attenuation coefficient");
    // Find the mesh
    auto search_mesh = g_p_scene_graph->getPolygonMeshSet().find(aLabel);
    
    // It has been found
    if(search_mesh != g_p_scene_graph->getPolygonMeshSet().end())
    {
        // Set its properties
        g_p_scene_graph->getPolygonMeshSet()[aLabel].setAttenuationCoefficient (anAttenuationCoefficient);
    }
    // It has not been found
    else
    {
        cerr << "gvxrWarning:\tPolygonMesh " << aLabel << " not found in g_p_polygon_mesh_set." << endl;
    }
}

/*
//------------------------------------
boost::python::tuple getMeshLabelSet()
//------------------------------------
{
	// Create a Python list
	boost::python::list py_label_set;

	// Look for all the meshes in the scenegraph
	for (std::map<string, PolygonMesh>::const_iterator ite = g_p_scene_graph->getPolygonMeshSet().begin();
			ite != g_p_scene_graph->getPolygonMeshSet().end();
			++ite)
	{
		// Add the name of the node to the Python list
		py_label_set.append(ite->first);
	}

	// Bind the list to Python
	return (boost::python::tuple(py_label_set));
}
*/

PyPolygonMesh makeCube(double aLength, const char* aUnitOfLength)
{
	return PyPolygonMesh(CubeMesh<double>(aLength * getUnitOfLength(aUnitOfLength), GL_UNSIGNED_INT));
}


PyPolygonMesh makeCylinder(unsigned int aNumberOfSectors,
        double aHeight,
        double aRadius,
		const char* aUnitOfLength)
{
	return PyPolygonMesh(CylinderMesh<double>(aNumberOfSectors,
	        aHeight  * getUnitOfLength(aUnitOfLength),
	        aRadius  * getUnitOfLength(aUnitOfLength),
			GL_UNSIGNED_INT));
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

            g_xray_renderer.disableArtifactFilteringOnGPU();
            g_xray_renderer.enableArtefactFilteringOnCPU();

    		g_renderer_initialised = true;
        }
	}
	initViewer();
}


//---------
void quit()
//---------
{
    cout << "gvxrStatus:\tQuit and destroy all the windows" << endl;
    destroyAllWindows();
}


void initViewer()
{
	if (!g_viewer_initialised && g_xray_detector.getSourcePositionSet().size())
	{
		float distance_source_object   = g_xray_detector.getSourcePositionSet()[0].length();
		float distance_detector_object = g_xray_detector.getDetectorPosition().length();

		float distance = std::max(distance_detector_object, distance_source_object);
		float half_fovy = (M_PI * g_initial_fovy / 2.0) / 180.0;


		g_zoom = atan(half_fovy) * distance;

		if (g_p_scene_graph.get())
		{
			g_initial_near = (g_p_scene_graph->getBoundingBox().second - g_p_scene_graph->getBoundingBox().first).length() / 5.0;
			g_viewer_initialised = true;
		}
		g_initial_far = g_zoom * 50.0;
	}
}


//----------------------------------------
GLFWwindow* initGLFW(int aWindowID,
                     bool aVisibilityFlag)
//----------------------------------------
{
    string window_title = "gVirtualXRay -- Window " + to_string(aWindowID);
    GLFWwindow* p_window = 0;

	// Set the version of OpenGL that is required
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    if (aVisibilityFlag)
    {
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    }
    else
    {
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    }
    
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

        framebufferSizeCallback(p_window, 500, 500);

	    // Initialize GLFW callbacks
	    glfwSetKeyCallback(p_window, keyCallback);
	    glfwSetMouseButtonCallback(p_window, mouseButtonCallback);
	    glfwSetCursorPosCallback(p_window, cursorPosCallback);
	    glfwSetScrollCallback(p_window, scrollCallback);
	    glfwSetFramebufferSizeCallback(p_window, framebufferSizeCallback);
    }

    return (p_window);
}


//-----------
void initGL()
//-----------
{
    std::cout << "OpenGL renderer:   " <<
        glGetString(GL_RENDERER)   << std::endl;

    std::cout << "OpenGL version:    " <<
        glGetString(GL_VERSION)    << std::endl;

    std::cout << "OpenGL vender:     " <<
        glGetString(GL_VENDOR)     << std::endl;

    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Enable the Z-buffer
    glEnable(GL_DEPTH_TEST);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Set the background colour
    glClearColor(0.5,
        0.5,
        0.5,
        1.0);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glEnable(GL_MULTISAMPLE);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Initialise the shaders
    char* p_vertex_shader(0);
    char* p_fragment_shader(0);

    int z_lib_return_code_vertex(0);
    int z_lib_return_code_fragment(0);

    std::string vertex_shader;
    std::string fragment_shader;

    // Display shader
    if (useOpenGL3_2OrAbove())
        {
        z_lib_return_code_vertex   = inflate(g_display_gl3_vert,
            sizeof(g_display_gl3_vert),
            &p_vertex_shader);

        z_lib_return_code_fragment = inflate(g_display_gl3_frag,
            sizeof(g_display_gl3_frag),
            &p_fragment_shader);

        g_display_shader.setLabels("display_gl3.vert", "display_gl3.frag");
        }
    else
        {
        z_lib_return_code_vertex   = inflate(g_display_gl2_vert,
            sizeof(g_display_gl2_vert),
            &p_vertex_shader);

        z_lib_return_code_fragment = inflate(g_display_gl2_frag,
            sizeof(g_display_gl2_frag),
            &p_fragment_shader);

        g_display_shader.setLabels("display_gl2.vert", "display_gl2.frag");
        }

    vertex_shader   = p_vertex_shader;
    fragment_shader = p_fragment_shader;

    delete [] p_vertex_shader;
    delete [] p_fragment_shader;

    p_vertex_shader = 0;
    p_fragment_shader = 0;

    if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 ||
            !vertex_shader.size() || !fragment_shader.size())
        {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "Cannot decode the shader using ZLib.");
        }
    g_display_shader.loadSource(vertex_shader, fragment_shader);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//----------------------------------------------------
void errorCallback(int error, const char* description)
//----------------------------------------------------
{
	// Display the error in the terminal
    std::cerr << "GLFW error: " << description << std::endl;
}



/*
//----------------------------------------------------
np::ndarray getNumpyImage(const Image<float>& anImage)
//----------------------------------------------------
{
	boost::python::object own;

	boost::python::tuple stride = boost::python::make_tuple(sizeof(RATIONAL_NUMBER)*anImage.getWidth(),sizeof(RATIONAL_NUMBER));
    boost::python::tuple shape = boost::python::make_tuple(anImage.getHeight(), anImage.getWidth());

    np::dtype dtype = np::dtype::get_builtin<RATIONAL_NUMBER>();

	return (np::from_data(anImage.getRawData(), dtype, shape, stride,own));
}*/


//------------------------------
void displayScene(int aWindowID)
//------------------------------
{
    // Display in the last window that has been created
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

    // There is a window to use
    if (aWindowID != -1)
    {
        // Find the window
        auto search_window = g_p_window_set.find(aWindowID);

        // It has been found
        if(search_window != g_p_window_set.end())
        {
            if (search_window->second)
            {
        	    // Make the window's context current
                glfwShowWindow(search_window->second);
        	    glfwMakeContextCurrent(search_window->second);
        		initViewer();

        		// Clear the buffers
        		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        		// Display the 3D scene
        		render();

        		// Make sure all the OpenGL code is done
        		glFinish();

        		// Swap front and back buffers
        		glfwSwapBuffers(search_window->second);
            }

            // Poll for and process events
            glfwPollEvents();
        }
        else
        {
            cerr << "gvxrWarning:\tWindow " << aWindowID << " not found in g_p_window_set." << endl;
        }
	}
}

/*
//-------------------------------------------------
void renderLoop(int aWindowID, PyObject* aCallBack)
//-------------------------------------------------
{
    // Display in the last window that has been created
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

    // There is a window to use
    if (aWindowID != -1)
    {
        // Find the window
        auto search_window = g_p_window_set.find(aWindowID);

        // It has been found
        if(search_window != g_p_window_set.end())
        {
            if (search_window->second)
            {
            	g_run_loop = true;

            	// Launch the event loop
                while (!glfwWindowShouldClose(search_window->second) && g_run_loop)
                {
                	// There is a callback
                	if (aCallBack)
                	{
                		// Ensure that the current thread is ready to call the Python C API
                		PyGILState_STATE state = PyGILState_Ensure();

                		// Invoke the python function
                		boost::python::call<void>(aCallBack);

                		// Release the global interpreter lock so other threads can resume execution
                		PyGILState_Release(state);
                	}

                	// Udate the X-ray image
                	//computeXRayImage();

                	// Render here
                    glfwShowWindow(search_window->second);
                	displayScene(aWindowID);
                }
            }
        }
        else
        {
            cerr << "gvxrWarning:\tWindow " << aWindowID << " not found in g_p_window_set." << endl;
        }
	}
}
*/

//-----------------------------------------------------------------------
void framebufferSizeCallback(GLFWwindow* apWindow, int width, int height)
//-----------------------------------------------------------------------
{
	if (height == 0)
	{
		// Prevent divide by 0
		height = 1;
	}

	int x(0), y(0), w(width), h(height);
    std::cout << x << " " << y << " " << w << " " << h << std::endl;
	double screen_aspect_ratio(double(width) / double(height));
	glViewport(x, y, w, h);

	initViewer();

	loadPerspectiveProjectionMatrix(g_initial_fovy, screen_aspect_ratio, g_initial_near, g_initial_far);

	loadLookAtModelViewMatrix(0.0, 0.0, g_zoom,
			0.0, 0.0, 0.0,
			0.0, 1.0, 0.0);
}


//-------------------------------------------------------------------------------
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
//-------------------------------------------------------------------------------
{
	if (action == GLFW_PRESS)
	{
		switch(key)
		{
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE:
			g_run_loop = false;
			break;

        case GLFW_KEY_B:
            g_display_beam = !g_display_beam;
            break;


        case GLFW_KEY_W:
            g_display_wireframe = !g_display_wireframe;
            break;

        case GLFW_KEY_N:
            g_xray_renderer.useNegativeFilteringFlag(!g_xray_renderer.getNegativeFilteringFlag());
            break;

        case GLFW_KEY_L:
            g_use_lighing = !g_use_lighing;
            break;

        case GLFW_KEY_D:
            g_display_detector = !g_display_detector;
            break;

        case GLFW_KEY_V:
        	g_display_normal_vectors = !g_display_normal_vectors;
            break;
		}
	}
}


//--------------------------------------------
void mouseButtonCallback(GLFWwindow* apWindow,
						 int aButton,
						 int aButtonState,
						 int aModifierKey)
//--------------------------------------------
{
	g_button = aButton;
	g_button_state = aButtonState;

	// Use the arc ball
	if (g_button_state == GLFW_PRESS)
	{
		g_use_arc_ball = true;
	}
	// Stop using the arc ball
	else
	{
		g_use_arc_ball = false;
	}

	double xpos(0);
	double ypos(0);
	glfwGetCursorPos (apWindow, &xpos, &ypos);

	g_last_x_position = xpos;
	g_last_y_position = ypos;

	g_current_x_position = xpos;
	g_current_y_position = ypos;
}


//-----------------------------------------------------------------------
void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset)
//-----------------------------------------------------------------------
{
	// Scrolling along the Y-axis
	if (fabs(yoffset) > EPSILON)
	{
		g_use_arc_ball = false;
		g_zoom += yoffset * cm;


        loadLookAtModelViewMatrix(0.0, 0.0, g_zoom,
                                  0.0, 0.0, 0.0,
                                  0.0, 1.0, 0.0);
	}
}


//------------------------------------------
Vec3<GLfloat> getArcballVector(GLFWwindow* apWindow, int x, int y)
//------------------------------------------
{
	int width, height;
	glfwGetWindowSize(apWindow, &width, &height);

	Vec3<GLfloat> P(2.0 * float(x) / float(width) - 1.0,
			2.0 * float(y) / float(height) - 1.0,
			0);

	P.setY(-P.getY());

	float OP_squared = P.getX() * P.getX() + P.getY() * P.getY();
	if (OP_squared <= 1.0)
	{
		P.setZ(sqrt(1.0 - OP_squared));  // Pythagore
	}
	else
	{
		P.normalise();  // nearest point
	}

	return (P);
}


//--------------------------------
float radian2degree(float anAngle)
//--------------------------------
{
	return (180.0 * anAngle / gVirtualXRay::PI);
}


//--------------------------------------------
void computeRotation(GLFWwindow* apWindow, MATRIX4& aRotationMatrix)
//--------------------------------------------
{
	if (g_use_arc_ball)
	{
		if (g_current_x_position != g_last_x_position || g_current_y_position != g_last_y_position)
		{
			Vec3<GLfloat> va(getArcballVector(apWindow, g_last_x_position,    g_last_y_position));
			Vec3<GLfloat> vb(getArcballVector(apWindow, g_current_x_position, g_current_y_position));

			float angle(g_rotation_speed * radian2degree(acos(std::min(1.0, va.dotProduct(vb)))));

			Vec3<GLfloat> axis_in_camera_coord(va ^ vb);
			//axis_in_camera_coord.normalize();

			Matrix4x4<GLfloat> camera2object(aRotationMatrix.getInverse());
			Vec3<GLfloat> axis_in_object_coord = camera2object * axis_in_camera_coord;
			//axis_in_object_coord.normalize();

			Matrix4x4<GLfloat> rotation_matrix;
			rotation_matrix.rotate(angle, axis_in_object_coord);
			aRotationMatrix = aRotationMatrix * rotation_matrix;

			g_last_x_position = g_current_x_position;
			g_last_y_position = g_current_y_position;
		}
	}
}


//--------------------------------------------------------------
void cursorPosCallback(GLFWwindow* apWindow, double x, double y)
//--------------------------------------------------------------
{
	g_current_x_position = x;
	g_current_y_position = y;

	// Compute the rotation
	computeRotation(apWindow, g_scene_rotation_matrix);
}


//---------
void draw()
//---------
{
	initViewer();

	// Clear the buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Display the 3D scene
	render();

	// Make sure all the OpenGL code is done
	glFinish();
}


//-----------
void render()
//-----------
{
	try
	{
        // Enable back face culling
        pushEnableDisableState(GL_CULL_FACE);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // Enable the shader
        pushShaderProgram();
        g_display_shader.enable();
        GLint shader_id(g_display_shader.getProgramHandle());

        // Check the status of OpenGL and of the current FBO
        checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Store the current transformation matrix
        pushModelViewMatrix();

        // Rotate the sample
        g_current_modelview_matrix *= g_scene_rotation_matrix;

        GLuint handle(0);
        handle = glGetUniformLocation(shader_id, "g_projection_matrix");
        glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_projection_matrix.get());
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "g_modelview_matrix");
        glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_modelview_matrix.get());
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        MATRIX4 normal_matrix(g_current_modelview_matrix);
        handle = glGetUniformLocation(shader_id, "g_normal_matrix");
        glUniformMatrix3fv(handle, 1, GL_FALSE, normal_matrix.get3x3());
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        GLint lighting;
        if (g_use_lighing)
        {
            lighting = 1;
        }
        else
        {
            lighting = 0;
        }
        handle = glGetUniformLocation(shader_id,"g_use_lighting");
        glUniform1iv(handle, 1, &lighting);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Set the colour of the sample
        {
            const GLfloat material_ambient[]  = {0.19225,  0.0, 0.0, 1.0};
            const GLfloat material_diffuse[]  = {0.50754,  0.0, 0.0, 1.0};
            const GLfloat material_specular[] = {0.508273, 0.0, 0.0, 1.0};
            const GLfloat material_shininess = 50.2;

            handle = glGetUniformLocation(shader_id, "material_ambient");
            glUniform4fv(handle, 1, &material_ambient[0]);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(shader_id, "material_diffuse");
            glUniform4fv(handle, 1, &material_diffuse[0]);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(shader_id, "material_specular");
            glUniform4fv(handle, 1, &material_specular[0]);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(shader_id, "material_shininess");
            glUniform1fv(handle, 1, &material_shininess);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        }

        // Store the current transformation matrix
        pushModelViewMatrix();

        g_current_modelview_matrix *= g_sample_rotation_matrix;



        for (std::map<string, PolygonMesh>::iterator ite = g_p_scene_graph->getPolygonMeshSet().begin();
        		ite != g_p_scene_graph->getPolygonMeshSet().end();
        		++ite)
        {
            // Store the current transformation matrix
            pushModelViewMatrix();

            g_current_modelview_matrix *= ite->second.getTransformationMatrix();

			applyModelViewMatrix();

			// Display the sample
			if (g_display_wireframe)
			{
				ite->second.displayWireFrame();
			}
			else
			{
				ite->second.display();
			}


			if (g_display_normal_vectors)
			{
				int line_width;
				glGetIntegerv(GL_LINE_WIDTH, &line_width);
				glLineWidth(3);

				if (ite->second.getVertexNormalNumber())
				{
					ite->second.displayVertexNormal();
				}

				if (ite->second.getFaceNormalNumber())
				{
					ite->second.displayFaceNormal();
				}
				glLineWidth(line_width);

			}

			// Restore the current transformation matrix
			popModelViewMatrix();
        }

        // Restore the current transformation matrix
        popModelViewMatrix();

        // Set the colour of the source
        {
            const GLfloat material_ambient[]  = {0.0, 0.39225,  0.39225,  1.0};
            const GLfloat material_diffuse[]  = {0.0, 0.70754,  0.70754,  1.0};
            const GLfloat material_specular[] = {0.0, 0.708273, 0.708273, 1.0};
            const GLfloat material_shininess = 50.2;

            handle = glGetUniformLocation(shader_id, "material_ambient");
            glUniform4fv(handle, 1, &material_ambient[0]);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(shader_id, "material_diffuse");
            glUniform4fv(handle, 1, &material_diffuse[0]);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(shader_id, "material_specular");
            glUniform4fv(handle, 1, &material_specular[0]);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(shader_id, "material_shininess");
            glUniform1fv(handle, 1, &material_shininess);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        }

        {
			// Define the light colours here
			GLfloat light_global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
			GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
			GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
			GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

			handle = glGetUniformLocation(shader_id, "light_global_ambient");
			glUniform4fv(handle, 1, &light_global_ambient[0]);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			handle = glGetUniformLocation(shader_id, "light_ambient");
			glUniform4fv(handle, 1, &light_ambient[0]);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			handle = glGetUniformLocation(shader_id, "light_diffuse");
			glUniform4fv(handle, 1, &light_diffuse[0]);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			handle = glGetUniformLocation(shader_id, "light_specular");
			glUniform4fv(handle, 1, &light_specular[0]);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			VEC3 light_position(0, 0, g_zoom);
			handle = glGetUniformLocation(shader_id, "light_position");
			glUniform4f(handle,
				light_position.getX(),
				light_position.getY(),
				light_position.getZ(),
				1.0);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		}

        // Display the source
        g_xray_detector.displaySource();

        // Disable back face culling
        glDisable(GL_CULL_FACE);

        // Display the X-Ray image
        if (g_display_detector)
        {
            g_xray_renderer.display(true, true, false);
        }

        // Display the beam
        if (g_display_beam)
        {
            const GLfloat material_ambient[]  = {0.75, 0, 0.5, 0.3};
            handle = glGetUniformLocation(shader_id, "material_ambient");
            glUniform4fv(handle, 1, &material_ambient[0]);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            lighting = 0;
            handle = glGetUniformLocation(shader_id,"g_use_lighting");
            glUniform1iv(handle, 1, &lighting);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            g_xray_detector.displayBeam();
        }

        // Disable the shader
        popShaderProgram();

        // Restore the current transformation matrix
        popModelViewMatrix();

        // Restore the attributes
        popEnableDisableState();

        // Check the status of OpenGL and of the current FBO
        checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
	// Catch exception if any
	catch (const std::exception& error)
	{
		std::cerr << error.what() << std::endl;
	}
}


//-----------------------------------------------
double getUnitOfEnergy(const char* aUnitOfEnergy)
//-----------------------------------------------
{
	double unit_of_energy = 0;

    if (!strcmp(aUnitOfEnergy, "megaelectronvolt") ||
            !strcmp(aUnitOfEnergy, "MeV"))
    {
    	unit_of_energy = MeV;
    }
    else if (!strcmp(aUnitOfEnergy, "kiloelectronvolt") ||
            !strcmp(aUnitOfEnergy, "keV"))
    {
    	unit_of_energy = keV;
    }
    else if (!strcmp(aUnitOfEnergy, "electronvolt") ||
            !strcmp(aUnitOfEnergy, "eV"))
    {
    	unit_of_energy = eV;
    }
    else
    {
		throw Exception(__FILE__, __FUNCTION__, __LINE__,
			"ERROR: Unknown unit of energy.");
    }

	return (unit_of_energy);
}


//-----------------------------------------------
double getUnitOfLength(const char* aUnitOfLength)
//-----------------------------------------------
{
	double unit_of_length = 0;

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


//--------------------------------------
void disableArtefactFilteringOnGPU(void)
//--------------------------------------
{
	g_xray_renderer.disableArtifactFilteringOnGPU();
}


//--------------------------------------
void disableArtifactFilteringOnGPU(void)
//--------------------------------------
{
	disableArtefactFilteringOnGPU();
}


//--------------------------------------
void disableArtefactFilteringOnCPU(void)
//--------------------------------------
{
	g_xray_renderer.disableArtifactFilteringOnCPU();
}


//--------------------------------------
void disableArtifactFilteringOnCPU(void)
//--------------------------------------
{
	disableArtefactFilteringOnCPU();
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





/*
BOOST_PYTHON_MODULE(gvxrPython)
{

    cout << "gvxrStatus:\tInitialise gvxrPython" << endl;

    // Initialise Python
    Py_Initialize();

    // Initialise Numpy
    np::initialize();

	// Initialize and acquire the global interpreter lock
    PyEval_InitThreads();

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


}
*/
