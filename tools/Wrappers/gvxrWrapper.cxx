#include <iostream>
#include <map>
#include <vector>

#ifdef HAS_GLEW
#include <GL/glew.h>
#endif

#define GLFW_INCLUDE_GLCOREARB 1

#include <GLFW/glfw3.h>

#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

// To perform the computations of the X-ray simulation
#ifndef __XRayRenderer_h
#include "gVirtualXRay/XRayRenderer.h"
#endif

// To initialise GLEW, to define matrix stacks, to define state stacks, etc.
#ifndef __OpenGLUtilities_h
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

// To load a scenegraph
#ifndef __SceneGraphBinder_h
#include "gVirtualXRay/SceneGraphBinder.h"
#endif

#ifndef __gvxrWrapper_h
#include "gvxrWrapper.h"
#endif


// Shaders for rendering
#include "display_gl2.frag.h"
#include "display_gl2.vert.h"
#include "display_gl3.frag.h"
#include "display_gl3.vert.h"


using namespace std;
using namespace gVirtualXRay;


GLFWwindow* initGLFW(int aWindowID, int aVisibilityFlag);
void createOpenGLContext(int aWindowID);
void framebufferSizeCallback(GLFWwindow* apWindow, int aWidth, int aHeight);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseButtonCallback(GLFWwindow* apWindow, int aButton, int aButtonState, int aModifierKey);
void cursorPosCallback(GLFWwindow* apWindow, double x, double y);
void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset);
void quit();
void errorCallback(int error, const char* description);
void initGL();
void initialiseXRayRenderer();
void initViewer();
void computeRotation(GLFWwindow* apWindow, MATRIX4& aRotationMatrix);
Vec3<GLfloat> getArcballVector(GLFWwindow* apWindow, int x, int y);
float radian2degree(float anAngle);
double getUnitOfEnergy(const char* aUnitOfEnergy);
double getUnitOfLength(const char* aUnitOfLength);


const GLfloat ROTATION_SPEED(2.0);


std::map<int, GLFWwindow*> g_p_window_set;
std::vector<int> g_p_window_id_stack;

const double g_initial_fovy(45);                              // Field of view along the y-axis
double g_initial_near(5.0 * cm);                        // Near clipping plane
double g_initial_far(500.0 * cm);                       // Far clipping plane
GLfloat g_zoom(120.0 * cm);

bool g_run_loop = false;
bool g_display_beam(true);
bool g_display_wireframe(false);
bool g_use_lighing(true);
bool g_display_detector(true);
bool g_display_normal_vectors(false);

bool g_beam_shape_initialised = false;
bool g_beam_energy_initialised = false;
bool g_source_position_initialised = false;
bool g_detector_position_initialised   = false;
bool g_detector_up_vector_initialised  = false;
bool g_detector_size_initialised       = false;
bool g_detector_pixel_size_initialised = false;
bool g_renderer_initialised = false;
bool g_viewer_initialised = false;

int g_button(-1);
int g_button_state(-1);
bool g_use_arc_ball(false);
GLint g_last_x_position(0);
GLint g_last_y_position(0);
GLint g_current_x_position(0);
GLint g_current_y_position(0);

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;

Shader g_display_shader;

std::shared_ptr<SceneGraphBinder> g_p_scene_graph = 0;

Matrix4x4<GLfloat> g_scene_rotation_matrix;


//-------------------------------------
void createOpenGLContext(int aWindowID)
//-------------------------------------
{
    createWindow(aWindowID, false);
}


//------------------------------------
void createWindow(int aWindowID,
                  int aVisibilityFlag)
//------------------------------------
{
    if (!g_p_window_set.size())
    {
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
            // Display a warning
            cerr << "gvxrWarning:\tWindow " << aWindowID << " cannot be created." << endl;
            
            // Delete the empty record that may have been created in g_p_window_set
            std::map<int, GLFWwindow*>::iterator it;
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


//----------------------------
void showWindow(int aWindowID)
//----------------------------
{
    // Resize the last window that has been created
    if (aWindowID == -1)
    {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
        {
            cerr << "gvxrWarning:\tNo active window to show." << endl;
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
            	glfwShowWindow(search->second);
			}
        }
        else
        {
            cerr << "gvxrWarning:\tWindow " << aWindowID << " not found." << endl;
        }
    }
}


//----------------------------
void hideWindow(int aWindowID)
//----------------------------
{
    // Resize the last window that has been created
    if (aWindowID == -1)
    {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
        {
            cerr << "gvxrWarning:\tNo active window to hide." << endl;
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
            	glfwHideWindow(search->second);
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


//----------------------------------------------
void setMonoChromatic(double anEnergy,
		              const char* aUnitOfEnergy,
					  double aNumberOfPhotons)
//----------------------------------------------
{
    g_xray_beam.clear();
    addEnergyBinToSpectrum(anEnergy, aUnitOfEnergy, aNumberOfPhotons);
}


//----------------------------------------------------
void addEnergyBinToSpectrum(double anEnergy,
							const char* aUnitOfEnergy,
							double aNumberOfPhotons)
//----------------------------------------------------
{
    g_xray_beam.addChannel(aNumberOfPhotons, anEnergy * getUnitOfEnergy(aUnitOfEnergy));
    g_beam_energy_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}


//-------------------------------------------
void saveLastXRayImage(const char* aFileName)
//-------------------------------------------
{
    static int file_counter = 0;
    string file_name;

    if (aFileName)
        file_name = aFileName;
    else
        file_name = "xray_image-" + to_string(file_counter++) + ".mha";

    g_xray_renderer.printXRayImage(file_name);
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














//---------------------------------------
GLFWwindow* initGLFW(int aWindowID,
                     int aVisibilityFlag)
//---------------------------------------
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


//--------------------------------------------------------------
void cursorPosCallback(GLFWwindow* apWindow, double x, double y)
//--------------------------------------------------------------
{
	g_current_x_position = x;
	g_current_y_position = y;

	// Compute the rotation
	computeRotation(apWindow, g_scene_rotation_matrix);
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


//---------
void quit()
//---------
{
    cout << "gvxrStatus:\tQuit and destroy all the windows" << endl;
    destroyAllWindows();
}


//----------------------------------------------------
void errorCallback(int error, const char* description)
//----------------------------------------------------
{
	// Display the error in the terminal
    std::cerr << "GLFW error: " << description << std::endl;
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


//---------------
void initViewer()
//---------------
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


//--------------------------------------------
void computeRotation(GLFWwindow* apWindow,
                     MATRIX4& aRotationMatrix)
//--------------------------------------------
{
	if (g_use_arc_ball)
	{
		if (g_current_x_position != g_last_x_position || g_current_y_position != g_last_y_position)
		{
			Vec3<GLfloat> va(getArcballVector(apWindow, g_last_x_position,    g_last_y_position));
			Vec3<GLfloat> vb(getArcballVector(apWindow, g_current_x_position, g_current_y_position));

			float angle(ROTATION_SPEED * radian2degree(acos(std::min(1.0, va.dotProduct(vb)))));

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


//--------------------------------------------------
Vec3<GLfloat> getArcballVector(GLFWwindow* apWindow,
                               int x, int y)
//--------------------------------------------------
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

