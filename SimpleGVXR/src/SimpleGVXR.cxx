/*

Copyright (c) 2019-2023, Dr Franck P. Vidal, Bangor University, All rights reserved.
Copyright (c) 2023-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
UK Research and Innovation, All rights reserved.


Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of Bangor University, UK Research and Innovation nor the 
names of its contributors may be used to endorse or promote products derived 
from this software without specific prior written permission.

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
*   @file       SimpleGVXR.cxx
*
*   @brief      SimpleGVXR is an API to the bulk functionalities and classes of
*               gVirtualXRay. SimpleGVXR is used as the basis for
*               the language bindings generated using SWIG.
*
*   @date       18/04/2024
*
*   @author     Prof Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal, Bangor University, Apr 2019-Sept 2023, version 1.1, BSD 3-Clause License
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Oct 2023-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/

//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <memory>
#include <cmath>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype> // Needed for std::toupper
#include <cmath> // Header file for abs
#include <random>
#include <cctype>
#include <zlib.h>

#ifndef USE_EMSCRIPTEN
#define GLFW_INCLUDE_GLCOREARB 1
#endif

#ifdef HAS_VULKAN
#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#endif

#ifdef HAS_GLFW
#include <GLFW/glfw3.h>
#endif

// To perform the computations of the X-ray simulation
#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef GVXR_SCINTILLATOR_H
#include "gVirtualXRay/Scintillator.h"
#endif

// For polygon meshes
#ifndef GVXR_POLYGON_MESH_H
#include "gVirtualXRay/PolygonMesh.h"
#endif

// To load a scenegraph
#ifndef GVXR_SCENEGRAPH_BINDER_H
#include "gVirtualXRay/SceneGraphBinder.h"
#endif

// To load a scenegraph using ASSIMP
#ifndef GVXR_ASSIMP_SCENEGRAPH_BINDER_H
#include "gVirtualXRay/AssimpSceneGraphBinder.h"
#endif

// To load a scenegraph using OpenSceneGraph
#ifndef GVXR_OpenSceneGraphBinder_h
#include "gVirtualXRay/OpenSceneGraphBinder.h"
#endif

#ifndef GVXR_CUBE_MESH_H
#include "gVirtualXRay/CubeMesh.h"
#endif

#ifndef GVXR_CUBOID_MESH_H
#include "gVirtualXRay/CuboidMesh.h"
#endif

#ifndef GVXR_CYLINDER_MESH_H
#include "gVirtualXRay/CylinderMesh.h"
#endif

#ifndef GVXR_OUT_OF_BOUNDS_EXCEPTION_H
#include "gVirtualXRay/OutOfBoundsException.h"
#endif

#ifndef GVXR_NOT_IMPLEMENTED_EXCEPTION_H
#include "gVirtualXRay/NotImplementedException.h"
#endif

#ifndef GVXR_ELEMENT_SET_H
#include "gVirtualXRay/ElementSet.h"
#endif

#ifndef GVXR_PHOTON_CROSS_SECTION_H
#include "gVirtualXRay/PhotonCrossSection.h"
#endif

#ifndef GVXR_RESOURCE_MANAGER_H
#include <gVirtualXRay/ResourceManager.h>
#endif

#ifndef GVXR_LOGGER_H
#include <gVirtualXRay/Logger.h>
#endif

#ifndef GVXR_RESOURCE_MANAGER_H
#include "gVirtualXRay/ResourceManager.h"
#endif

#ifndef SGVXR_SimpleGVXR_h
#include "SimpleGVXR.h"
#endif

#ifndef SGVXR_Internal_h
#include "internal.h"
#endif

#ifndef SGVXR_Window_h
#include "Window.h"
#endif

#ifndef SGVXR_OpenGLWindow_h
#include "OpenGLWindow.h"
#endif

#ifndef SGVXR_VulkanWindow_h
#include "VulkanWindow.h"
#endif

#ifndef SGVXR_Windowless_h
#include "Windowless.h"
#endif


//******************************************************************************
//  Namespace
//******************************************************************************


//******************************************************************************
//  Function declarations
//******************************************************************************
#ifdef HAS_GLFW
GLFWwindow* initGLFW(int aWindowID, int aVisibilityFlag);
#endif


void computeRotation(gVirtualXRay::MATRIX4& aRotationMatrix,
    int aViewportWidth,
    int aViewportHeight);


gVirtualXRay::Image<float> getImage(const std::vector<std::vector<float> >& anImage);

gVirtualXRay::Image<float> getImage(const std::vector<std::vector<std::vector<float> > >& anImage);

gVirtualXRay::MATRIX4 Vector2gvxrMatrix(const std::vector<std::vector<float> >& aMatrix4);

std::vector<std::vector<float> > gvxrMatrix2Vector(
    const gVirtualXRay::MATRIX4& aMatrix4);


#ifdef HAS_CUDA
void resampleUsingCUDA();
extern int g_samples_per_pixels;
extern gVirtualXRay::Image<float> g_cpu_memory;
#else
int g_samples_per_pixels = 1;
#endif


//******************************************************************************
//  Global variables
//******************************************************************************
std::map<int, std::unique_ptr<SimpleGVXR::Window> > g_p_window_set;      // The set of windows
std::vector<int> g_p_window_id_stack;           // The stack of window IDs

//double g_initial_fovy(45);                // Field of view along
//                                                // the y-axis
//double g_initial_near(0.5 * gVirtualXRay::cm);  // Near clipping plane
//double g_initial_far(500.0 * gVirtualXRay::cm); // Far clipping plane
//
//gVirtualXRay::VEC3 g_camera_position(0.0, 0.0, g_initial_far / 2.0);
//gVirtualXRay::VEC3 g_camera_lookat(0.0, 0.0, 0.0);
//gVirtualXRay::VEC3 g_camera_up(0.0, 1.0, 0.0);

// State variables
bool g_run_loop = false;
bool g_display_help(true);

// bool g_detector_position_initialised = false;
// bool g_detector_up_vector_initialised = false;
// bool g_detector_size_initialised = false;
// bool g_detector_pixel_size_initialised = false;
bool g_renderer_initialised = false;

extern gVirtualXRay::XRayDetector g_xray_detector; // X-ray detector
gVirtualXRay::XRayRenderer g_xray_renderer; // X-ray graphics engine
extern gVirtualXRay::XRayBeam g_xray_beam;         // X-ray beam

gVirtualXRay::Shader g_display_shader;      // Visualisation shader

// Scenegraph of scanned objects
std::unique_ptr<gVirtualXRay::SceneGraphBinder> g_p_scene_graph = nullptr;

// Bias/gain of the shift/scale filter for the display of the X-ray image
double g_shift_filter = 0;
double g_scale_filter = 1;

bool g_has_own_gl_context = false;


std::vector<std::vector<std::vector< float> > > g_p_lastest_screenshot;

std::random_device g_random_device;  // Will be used to obtain a seed for the random number engine
std::mt19937 g_random_generator(g_random_device()); // Standard mersenne_twister_engine seeded with rd()

std::map<std::string, unsigned int> g_warning_message_counters;

/// Latest screenshot
extern gVirtualXRay::Image<uint8_t> m_latest_screenshot;


//******************************************************************************
//  Function definitions
//******************************************************************************

//-----------------------------
int getMajorVersionOfCoreGVXR()
//-----------------------------
{
    return (strtol(gVirtualXRay_VERSION_MAJOR, nullptr, 10));
}

//-----------------------------
int getMinorVersionOfCoreGVXR()
//-----------------------------
{
    return (strtol(gVirtualXRay_VERSION_MINOR, nullptr, 10));
}

//-----------------------------
int getPatchVersionOfCoreGVXR()
//-----------------------------
{
    return (strtol(gVirtualXRay_VERSION_PATCH, nullptr, 10));
}

//--------------------------------
std::string getVersionOfCoreGVXR()
//--------------------------------
{
    return {gvxr_VERSION};
}

//-------------------------------
int getMajorVersionOfSimpleGVXR()
//-------------------------------
{
    return (SimpleGVXR_VERSION_MAJOR);
}

//-------------------------------
int getMinorVersionOfSimpleGVXR()
//-------------------------------
{
    return (SimpleGVXR_VERSION_MINOR);
}

//-------------------------------
int getPatchVersionOfSimpleGVXR()
//-------------------------------
{
    return (SimpleGVXR_VERSION_PATCH);
}

//----------------------------------
std::string getVersionOfSimpleGVXR()
//----------------------------------
{
    return {SimpleGVXR_VERSION_STRING};
}


//---------------------------------------
std::string getBackendName(int aWindowID)
//---------------------------------------
{
    // No window found
    if (g_p_window_id_stack.empty()) return "UNKNOWN";

    // Use the default window, i.e. the last created window
    if (aWindowID == -1)
        aWindowID = g_p_window_id_stack.back();

    // Find the window
    auto search = g_p_window_set.find(aWindowID);

    // The window was not found
    if (search == g_p_window_set.end() || !search->second) return "UNKNOWN";

    // The window was found
    // Get the backend
    std::string backend_name = search->second->getBackendName();

    // Capitalise the name
    std::transform(backend_name.begin(), backend_name.end(), backend_name.begin(),
          ::toupper);

    // Return the name
    return backend_name;
}


//-----------------------------
std::string getOpenGlRenderer()
//-----------------------------
{
    std::string returned_value("UNKNOWN");

#ifdef HAS_OPENGL
    const char* value_c_str = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
    if (value_c_str) returned_value = value_c_str;
#endif

    return returned_value;
}


//----------------------------
std::string getOpenGlVersion()
//----------------------------
{
    std::string returned_value("UNKNOWN");

#ifdef HAS_OPENGL
    const char* value_c_str = reinterpret_cast<const char *>(glGetString(GL_VERSION));
    if (value_c_str) returned_value = value_c_str;
#endif

    return returned_value;
}


//---------------------------
std::string getOpenGlVendor()
//---------------------------
{
    std::string returned_value("UNKNOWN");

#ifdef HAS_OPENGL
    const char* value_c_str = reinterpret_cast<const char *>(glGetString(GL_VENDOR));
    if (value_c_str) returned_value = value_c_str;
#endif

    return returned_value;
}


//-------------------------------------
std::string getShadingLanguageVersion()
//-------------------------------------
{
    std::string returned_value("UNKNOWN");

#ifdef HAS_OPENGL
    const char* value_c_str = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
    if (value_c_str) returned_value = value_c_str;
#endif

    return returned_value;
}


//-----------------------------
std::string getNoiseGenerator()
//-----------------------------
{
    std::string returned_value("gVXR's own implementation");

    #ifdef HAS_OPENMP
        returned_value += " with OpenMP (" + std::to_string(omp_get_max_threads()) + " threads)";
    #else
        returned_value += " without OpenMP";
    #endif

    return returned_value;
}


//-------------------------------
std::string getBuildEnvironment()
//-------------------------------
{
    std::stringstream message;
    

    message << "Has NDEBUG:";
#ifdef NDEBUG
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has WIN32:";
#ifdef HAS_WIN32
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has __APPLE__:";
#ifdef __APPLE__
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has GSL:";
#ifdef HAS_GSL
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has OpenMP:";
#ifdef HAS_OPENMP
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has CUDA:";
#ifdef HAS_CUDA
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has Vulkan:";
#ifdef HAS_VULKAN
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has EGL:";
#ifdef HAS_EGL
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has libGBM:";
#ifdef HAS_GBM
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has EGL_DRM_DEVICE_FILE_EXT:";
#ifdef EGL_DRM_DEVICE_FILE_EXT
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has EGL_CUDA_DEVICE_NV:";
#ifdef EGL_CUDA_DEVICE_NV
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has GLU:";
#ifdef HAS_GLU
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has GLEW:";
#ifdef HAS_GLEW
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has GLAD:";
#ifdef HAS_GLAD
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has GLX:";
#ifdef HAS_GLX
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has ASSIMP:";
#ifdef HAS_ASSIMP
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has OpenSceneGraph:";
#ifdef HAS_OPENSCENEGRAPH
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has Built-in Python support:";
#if defined(USE_PYTHON) && defined(HAS_PythonLibs)
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has ITK:";
#ifdef HAS_ITK
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has GCDM:";
#ifdef HAS_GCDM
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has LibTIFF:";
#if defined(HAS_TIFF) && defined(HAS_LIBTIFF)
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has FreeType:";
#ifdef HAS_FREETYPE
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    return message.str();
}


//------------------------------------
void useLogFile(const char* aFileName)
//------------------------------------
{
    // A filename has been provided
    if (aFileName)
    {
        if (gVirtualXRay::LOGGER.getFilename() != aFileName)
        {
            gVirtualXRay::LOGGER.open(aFileName);
        }
    }
    // A filename has not been provided
    else
    {
        gVirtualXRay::LOGGER.close();
    }
}


//-------------------------------------------
void useLogFile(const std::string& aFileName)
//-------------------------------------------
{
    if (aFileName.empty()) useLogFile(nullptr);
    else useLogFile(aFileName.c_str());
}


//---------------------------------------------
void setSeed(unsigned int aRandomGeneratorSeed)
//---------------------------------------------
{
    g_random_generator = std::mt19937(aRandomGeneratorSeed);
}


//--------------------------------------------------------------
double getShaderTimeInSeconds(const std::string& aShaderProgram)
//--------------------------------------------------------------
{
    std::string shader_program_upper = aShaderProgram;

    std::transform(shader_program_upper.begin(),
                   shader_program_upper.end(),
                   shader_program_upper.begin(),
                   ::toupper);

    double runtime = 0.0;

    if (shader_program_upper == "UNCLEANED D_J")
        runtime = g_xray_renderer.getShaderTimeInSecondsAccumulators()
            [gVirtualXRay::XRayRenderer::XRAY_DETECTOR_LBUFFER_SHADER_ID];

    else if (shader_program_upper == "CLEANED D_J")
        runtime = g_xray_renderer.getShaderTimeInSecondsAccumulators()
            [gVirtualXRay::XRayRenderer::XRAY_DETECTOR_CLEAN_LBUFFER_SHADER_ID];

    else if (shader_program_upper == "SUM_J D_J")
        runtime = g_xray_renderer.getShaderTimeInSecondsAccumulators()
            [gVirtualXRay::XRayRenderer::XRAY_DETECTOR_CUMULATED_LBUFFER_SHADER_ID];

    else if (shader_program_upper == "D_0 - SUM_J D_J")
        runtime = g_xray_renderer.getShaderTimeInSecondsAccumulators()
            [gVirtualXRay::XRayRenderer::XRAY_DETECTOR_OUTER_SURFACE_LBUFFER_SHADER_ID];

    else if (shader_program_upper == "SUM_J MU(E_I) D_J")
        runtime = g_xray_renderer.getShaderTimeInSecondsAccumulators()
            [gVirtualXRay::XRayRenderer::XRAY_DETECTOR_SUM_MUx_Dx_SHADER_ID];

    else if (shader_program_upper == "I")
        runtime = g_xray_renderer.getShaderTimeInSecondsAccumulators()
            [gVirtualXRay::XRayRenderer::XRAY_DETECTOR_ENERGY_FLUENCE_SHADER_ID];

    else if (shader_program_upper == "DISPLAY X-RAY")
        runtime = g_xray_renderer.getShaderTimeInSecondsAccumulators()
            [gVirtualXRay::XRayRenderer::XRAY_DETECTOR_DISPLAY_SHADER_ID];

    else
    {
        std::stringstream error_message;
        error_message << "The shader program " << aShaderProgram <<
            " does not exist. See documentation for valid values.";
        throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
    }

    return runtime;
}


//--------------------------------------------------------
size_t getShaderCounter(const std::string& aShaderProgram)
//--------------------------------------------------------
{
    std::string shader_program_upper = aShaderProgram;

    std::transform(shader_program_upper.begin(),
                   shader_program_upper.end(),
                   shader_program_upper.begin(),
                   ::toupper);

    size_t counter = 0;

    if (shader_program_upper == "UNCLEANED D_J")
        counter = g_xray_renderer.getShaderCounterAccumulators()
            [gVirtualXRay::XRayRenderer::XRAY_DETECTOR_LBUFFER_SHADER_ID];

    else if (shader_program_upper == "CLEANED D_J")
        counter = g_xray_renderer.getShaderCounterAccumulators()
            [gVirtualXRay::XRayRenderer::XRAY_DETECTOR_CLEAN_LBUFFER_SHADER_ID];

    else if (shader_program_upper == "SUM_J D_J")
        counter = g_xray_renderer.getShaderCounterAccumulators()
            [gVirtualXRay::XRayRenderer::XRAY_DETECTOR_CUMULATED_LBUFFER_SHADER_ID];

    else if (shader_program_upper == "D_0 - SUM_J D_J")
        counter = g_xray_renderer.getShaderCounterAccumulators()
            [gVirtualXRay::XRayRenderer::XRAY_DETECTOR_OUTER_SURFACE_LBUFFER_SHADER_ID];

    else if (shader_program_upper == "SUM_J MU(E_I) D_J")
        counter = g_xray_renderer.getShaderCounterAccumulators()
            [gVirtualXRay::XRayRenderer::XRAY_DETECTOR_SUM_MUx_Dx_SHADER_ID];

    else if (shader_program_upper == "I")
        counter = g_xray_renderer.getShaderCounterAccumulators()
            [gVirtualXRay::XRayRenderer::XRAY_DETECTOR_ENERGY_FLUENCE_SHADER_ID];

    else if (shader_program_upper == "DISPLAY X-RAY")
        counter = g_xray_renderer.getShaderCounterAccumulators()
            [gVirtualXRay::XRayRenderer::XRAY_DETECTOR_DISPLAY_SHADER_ID];

    else
    {
        std::stringstream error_message;
        error_message << "The shader program " << aShaderProgram <<
            " does not exist. See documentation for valid values.";
        throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
    }

    return counter;
}


//--------------------------------------
void autoCreateOpenGLContext(bool aFlag)
//--------------------------------------
{
    g_has_own_gl_context = !aFlag;
}








//-----------------------------------------
void displayNode(const std::string& aLabel)
//-----------------------------------------
{
    std::vector<float> colour(4, 1.0);

    // The scenegraph does not exist
    if (!g_p_scene_graph)
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        p_node->display();
        }
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }
}


void resetSceneTransformation()
{
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode("root");
    p_node->setLocalTransformation(gVirtualXRay::MATRIX4());
    requestXRayImageUpdate();
}


void scaleScene(double x,
                double y,
                double z)
{
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode("root");
    p_node->setLocalTransformation(p_node->getLocalTransformationMatrix() * gVirtualXRay::MATRIX4::buildScaleMatrix(
        static_cast<float>(x),
        static_cast<float>(y),
        static_cast<float>(z))
    );
    requestXRayImageUpdate();
}


void scaleScene(double x,
                double y,
                double z,
                const std::string& aUnitOfLength)
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);
    scaleScene(x * unit_of_length,
               y * unit_of_length,
               z * unit_of_length);
    requestXRayImageUpdate();
}


void rotateScene(double angle, double x, double y, double z)
{
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode("root");
    p_node->setLocalTransformation(p_node->getLocalTransformationMatrix() * gVirtualXRay::MATRIX4::buildRotationMatrix(
        static_cast<float>(angle),
        static_cast<float>(x),
        static_cast<float>(y),
        static_cast<float>(z))
    );
    requestXRayImageUpdate();
}


void rotateScene(double yaw, double pitch, double roll)
{
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode("root");
    p_node->setLocalTransformation(p_node->getLocalTransformationMatrix() * gVirtualXRay::MATRIX4::buildYawPitchRollRotationMatrix(
        static_cast<float>(yaw),
        static_cast<float>(pitch),
        static_cast<float>(roll))
    );
    requestXRayImageUpdate();
}


void translateScene(double x,
                    double y,
                    double z,
                    const std::string& aUnitOfLength)
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode("root");
    p_node->setLocalTransformation(p_node->getLocalTransformationMatrix() * gVirtualXRay::MATRIX4::buildTranslationMatrix(
        static_cast<float>(x * unit_of_length),
        static_cast<float>(y * unit_of_length),
        static_cast<float>(z * unit_of_length))
    );
    requestXRayImageUpdate();
}


std::vector<std::vector<float> > getSceneTransformationMatrix()
{
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode("root");
    const gVirtualXRay::MATRIX4& transformation_matrix = p_node->getLocalTransformationMatrix();

    return gvxrMatrix2Vector(transformation_matrix);
}


std::vector<std::vector<float> > getRootTransformationMatrix()
{
    if (!g_p_scene_graph)
    {
        throw gVirtualXRay::Exception(__FILE__,
                                      __FUNCTION__,
                                      __LINE__,
                                      "The scenegraph is empty.");
    }

    return (gvxrMatrix2Vector(
        g_p_scene_graph->getRootNodeTransformationMatrix()));
}


std::vector<std::vector<float> > getNodeLocalTransformationMatrix(
        const std::string& aLabel)
{
    gVirtualXRay::MATRIX4 transformation;

    if (g_p_scene_graph)
        {
        // Find the mesh
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

        // It has been found
        if (p_node)
            {
            transformation = p_node->getLocalTransformationMatrix();
            }
        // It has not been found
        else
            {
            gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
                aLabel <<
                " not found in g_p_polygon_mesh_set." <<
                std::endl;
            }
        }
    else
        {
        gVirtualXRay::LOGGER.logWarning("The scenegraph is empty.") << std::endl;
        }

    return (gvxrMatrix2Vector(transformation));
}


std::vector<std::vector<float> > getNodeWorldTransformationMatrix(
        const std::string& aLabel)
{
    gVirtualXRay::MATRIX4 transformation;

    if (g_p_scene_graph)
        {
        // Find the mesh
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

        // It has been found
        if (p_node)
            {
            transformation = p_node->getWorldTransformationMatrix();
            }
        // It has not been found
        else
            {
            gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
                aLabel <<
                " not found in g_p_polygon_mesh_set." <<
                std::endl;
            }
        }
    else
        {
        gVirtualXRay::LOGGER.logWarning("The scenegraph is empty.") << std::endl;
        }

    return (gvxrMatrix2Vector(transformation));
}


void setSceneTransformationMatrix(
        const std::vector<std::vector<float> >& aMatrix)
{
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode("root");
    p_node->setLocalTransformation(Vector2gvxrMatrix(aMatrix));
    requestXRayImageUpdate();
}

void setRootTransformationMatrix(
        const std::vector<std::vector<float> >& aMatrix)
{
    if (g_p_scene_graph)
        {
        g_p_scene_graph->setRootNodeTransformationMatrix(Vector2gvxrMatrix(aMatrix));
        requestXRayImageUpdate();
        }
    else
        {
        gVirtualXRay::LOGGER.logWarning("The scenegraph is empty.") << std::endl;
        }
}

void setNodeTransformationMatrix(const std::string& aLabel,
        const std::vector<std::vector<float> >& aMatrix)
{
    if (g_p_scene_graph)
        {
        // Find the mesh
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

        // It has been found
        if (p_node)
            {
            p_node->setLocalTransformation(Vector2gvxrMatrix(aMatrix));
            requestXRayImageUpdate();
            }
        // It has not been found
        else
            {
            gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
                aLabel <<
                " not found in g_p_polygon_mesh_set." <<
                std::endl;
            }
        }
    else
        {
        gVirtualXRay::LOGGER.logWarning("The scenegraph is empty.") << std::endl;
        }
}

//---------------------------------------------
void setHU(const std::string& aLabel, short HU)
//---------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set its properties
        p_node->getPolygonMesh().setHounsfieldValue(HU);
        g_xray_renderer.needUpdateSurfacePerMaterial();
        requestXRayImageUpdate();
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }
}

//---------------------------------------------------------
void setHounsfieldUnit(const std::string& aLabel, short HU)
//---------------------------------------------------------
{
    setHU(aLabel, HU);
}

//----------------------------------------------------------
void setHounsfieldValue(const std::string& aLabel, short HU)
//----------------------------------------------------------
{
    setHU(aLabel, HU);
}

//----------------------------------------------------------
void setElement(const std::string& aLabel, unsigned short Z)
//----------------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set its properties
        p_node->getPolygonMesh().setElement(Z);
        g_xray_renderer.needUpdateSurfacePerMaterial();
        requestXRayImageUpdate();
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }
}

//------------------------------------------------------------------
void setElement(const std::string& aLabel, const std::string& aName)
//------------------------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set its properties
        p_node->getPolygonMesh().setElement(aName);
        g_xray_renderer.needUpdateSurfacePerMaterial();
        requestXRayImageUpdate();
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }
}

//------------------------------------------------------------------
void setMixture(const std::string& aLabel, const std::string& aName)
//------------------------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set its properties
        p_node->getPolygonMesh().setMixture(aName);
        g_xray_renderer.needUpdateSurfacePerMaterial();
        requestXRayImageUpdate();
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }
}


//----------------------------------------------------
void setMixture(const std::string& aLabel,
                const std::vector<int>& aZNumberSet,
                const std::vector<double>& aWeightSet)
//----------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set its properties
        std::map<int, double> mixture;

        if (aZNumberSet.size() != aWeightSet.size())
            {
            throw gVirtualXRay::Exception(
                __FILE__,
                __FUNCTION__,
                __LINE__,
                "ERROR: The Z number set and weight set don't have the same size.");
            }

        for (unsigned int i = 0; i != aZNumberSet.size(); ++i)
            {
            mixture[aZNumberSet[i]] = aWeightSet[i];
            }

        p_node->getPolygonMesh().setMixture(mixture);
        g_xray_renderer.needUpdateSurfacePerMaterial();
        requestXRayImageUpdate();
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }
}


//---------------------------------------------------------
void setMixture(const std::string& aLabel,
                const std::vector<std::string>& aSymbolSet,
                const std::vector<double>& aWeightSet)
//---------------------------------------------------------
{
  // Find the mesh
  gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

  // It has been found
  if (p_node)
      {
      // Set its properties
      std::map<std::string, double> mixture;

      if (aSymbolSet.size() != aWeightSet.size())
          {
          throw gVirtualXRay::Exception(
              __FILE__,
              __FUNCTION__,
              __LINE__,
              "ERROR: The symbol set and weight set don't have the same size.");
          }

      for (unsigned int i = 0; i != aSymbolSet.size(); ++i)
          {
          mixture[aSymbolSet[i]] = aWeightSet[i];
          }

      p_node->getPolygonMesh().setMixture(mixture);
      g_xray_renderer.needUpdateSurfacePerMaterial();
      requestXRayImageUpdate();
      }
  // It has not been found
  else
      {
      gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
          aLabel <<
          " not found in g_p_polygon_mesh_set." <<
          std::endl;
      }
}


//-------------------------------------------------------------------
void setCompound(const std::string& aLabel, const std::string& aName)
//-------------------------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set its properties
        p_node->getPolygonMesh().setCompound(aName);
        g_xray_renderer.needUpdateSurfacePerMaterial();
        requestXRayImageUpdate();
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }
}

//-----------------------------------------------------------
void setMassAttenuationCoefficient(const std::string& aLabel,
                                   double aCoefficient,
                                   const std::string& aUnit)
//-----------------------------------------------------------
{
    if (aUnit != "cm2/g" && aUnit != "cm2.g-1")
        {
        throw gVirtualXRay::Exception(
            __FILE__,
            __FUNCTION__,
            __LINE__,
            "ERROR: Unknown unit of mass attenuation coefficient.");
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set its properties
        p_node->getPolygonMesh().setMassAttenuationCoefficient(
            aCoefficient * gVirtualXRay::cm2 / gVirtualXRay::g);
        g_xray_renderer.needUpdateSurfacePerMaterial();
        requestXRayImageUpdate();
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }
}

//-------------------------------------------------------------
void setLinearAttenuationCoefficient(const std::string& aLabel,
                                     double aCoefficient,
                                     const std::string& aUnit)
//-------------------------------------------------------------
{
    if (aUnit != "cm-1")
        {
        throw gVirtualXRay::Exception(
            __FILE__,
            __FUNCTION__,
            __LINE__,
            "ERROR: Unknown unit of linear attenuation coefficient.");
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
          if (aUnit.substr(aUnit.length() - 2) != "-1")
          {
              throw gVirtualXRay::Exception(
                  __FILE__,
                  __FUNCTION__,
                  __LINE__,
                  "ERROR: Unknown unit of linear attenuation coefficient.");
          }

          // Set its properties
          double inv_unit_of_length = 1.0 / getUnitOfLength(aUnit.substr(0, aUnit.length() - 2));
          p_node->getPolygonMesh().setLinearAttenuationCoefficient(
            aCoefficient * inv_unit_of_length);
          g_xray_renderer.needUpdateSurfacePerMaterial();
          requestXRayImageUpdate();
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }
}

//-----------------------------------
void setMu(const std::string& aLabel,
           double aCoefficient,
           const std::string& aUnit)
//-----------------------------------
{
    setLinearAttenuationCoefficient(aLabel, aCoefficient, aUnit);
    requestXRayImageUpdate();
}

//----------------------------------------
void setDensity(const std::string& aLabel,
                double aDensity,
                const std::string& aUnit)
//----------------------------------------
{
    if (aUnit != "g/cm3" && aUnit != "g.cm-3" &&
        aUnit != "mg/cm3" && aUnit != "mg.cm-3" &&
        aUnit != "kg/m3" && aUnit != "kg.m-3")
    {
        throw gVirtualXRay::Exception(
            __FILE__,
            __FUNCTION__,
            __LINE__,
            "ERROR: Unknown unit of density.");
    }

    double density_unit = 0.0;
    double volume_unit = 0.0;

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
    {
        if (aUnit == "g/cm3" || aUnit == "g.cm-3")
        {
            density_unit = gVirtualXRay::g;
            volume_unit = gVirtualXRay::cm3;
        }
        else if (aUnit == "mg/cm3" || aUnit == "mg.cm-3")
        {
            density_unit = gVirtualXRay::mg;
            volume_unit = gVirtualXRay::cm3;
        }
        else if (aUnit == "kg/m3" || aUnit == "kg.m-3")
        {
            density_unit = gVirtualXRay::kg;
            volume_unit = gVirtualXRay::m3;
        }

        // Set its properties
        p_node->getPolygonMesh().setDensity(
            aDensity * density_unit / volume_unit);

        g_xray_renderer.needUpdateSurfacePerMaterial();
        requestXRayImageUpdate();
    }
    // It has not been found
    else
    {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
    }
}

//------------------------------------------
double getDensity(const std::string& aLabel)
//------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        return p_node->getPolygonMesh().getDensity()
        / (gVirtualXRay::g / gVirtualXRay::cm3);
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }

    return 0;
}


//--------------------------------------------------------------------
double getMassAttenuationCoefficient(const std::string& aLabel,
                                     double anEnergy,
                                     const std::string& aUnitOfEnergy)
//--------------------------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph)
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        return (p_node->getPolygonMesh().getPhotonCrossSection().
                getMassAttenuationCoefficient(anEnergy *
                        getUnitOfEnergy(aUnitOfEnergy)) /
                (gVirtualXRay::cm2 / gVirtualXRay::g));
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }

    return 0;
}


//----------------------------------------------------------------------
double getLinearAttenuationCoefficient(const std::string& aLabel,
                                       double anEnergy,
                                       const std::string& aUnitOfEnergy)
//----------------------------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph)
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        return (p_node->getPolygonMesh().getPhotonCrossSection().
                getLinearAttenuationCoefficient(anEnergy *
                    getUnitOfEnergy(aUnitOfEnergy)) /
                (1.0 / gVirtualXRay::cm));
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }

    return 0;
}

//----------------------------------------------
std::string getMaterialLabel(const std::string& aLabel)
//----------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        return p_node->getPolygonMesh().getMaterialLabel();
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }

    return std::string();
}


//------------------------------------------------
std::string getCompound(const std::string& aLabel)
//------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        return p_node->getPolygonMesh().getCompound();
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }

    return std::string();
}


//--------------------------------------------------------------
std::vector<int> getMixtureElementSet(const std::string& aLabel)
//--------------------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
    {
        return p_node->getPolygonMesh().getMixtureElementSet();
    }
    // It has not been found
    else
    {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
    }

    return std::vector<int>();
}


//----------------------------------------------------------------
std::vector<double> getMixtureWeightSet(const std::string& aLabel)
//----------------------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
    {
        return p_node->getPolygonMesh().getMixtureWeightSet();
    }
    // It has not been found
    else
    {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
    }

    return std::vector<double>();
}


//--------------------------------------------------
float getEffectiveZNumber(const std::string& aLabel)
//--------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
    {
        return p_node->getPhotonCrossSection().getEffectiveZNumber();
    }
    // It has not been found
    else
    {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
    }

    return 0.0;
}


//-------------------------------------------------------
float getEffectiveAtomicNumber(const std::string& aLabel)
//-------------------------------------------------------
{
    return getEffectiveZNumber(aLabel);
}


//----------------------------------------------------
float getEffectiveMolarMass(const std::string& aLabel)
//----------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
    {
        return p_node->getPhotonCrossSection().getEffectiveMolarMass();
    }
    // It has not been found
    else
    {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
    }

    return 0.0;
}


//-------------------------------------------------------
void displayScene(bool aSceneRotationFlag, int aWindowID)
//-------------------------------------------------------
{
    // Display in the last window that has been created
    if (aWindowID == -1 && !g_has_own_gl_context)
        {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
            {
            gVirtualXRay::LOGGER.logWarning("No active window to use.") << std::endl;
            }
        // Destroy the last created window
        else
            {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
            }
        }

    // There is a window to use
    if (aWindowID != -1 || g_has_own_gl_context)
        {
        // Find the window
        auto search_window = g_p_window_set.find(aWindowID);

        // It has been found
        if (search_window != g_p_window_set.end() || g_has_own_gl_context)
            {
            if (search_window->second || g_has_own_gl_context)
                {
                search_window->second->displayScene(aSceneRotationFlag);
                }

            // Poll for and process events
#ifdef HAS_GLFW
            if (!g_has_own_gl_context)
            {
                glfwPollEvents();
            }
#endif
            }
        else
            {
            gVirtualXRay::LOGGER.logWarning("Window") <<
                aWindowID <<
                " not found in g_p_window_set." <<
                std::endl;
            }
        }
}

//----------------------------
void renderLoop(int aWindowID)
//----------------------------
{
    // Display in the last window that has been created
    if (aWindowID == -1)
        {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
            {
            gVirtualXRay::LOGGER.logWarning("No active window to use.") << std::endl;
            }
        // Use the last created window
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
        if (search_window != g_p_window_set.end())
            {
            if (search_window->second)
                {
                search_window->second->renderLoop();
                }
            }
        else
            {
            gVirtualXRay::LOGGER.logWarning("Window ") <<
                aWindowID <<
                " not found in g_p_window_set." <<
                std::endl;
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
            gVirtualXRay::LOGGER.logWarning("No active window to show.") << std::endl;
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
                search->second->showWindow();
                }
            }
        else
            {
            gVirtualXRay::LOGGER.logWarning("Window ") << aWindowID <<
                " not found." << std::endl;
            }
        }
}

//----------------------------
void hideWindow(int aWindowID)
//----------------------------
{
    // Hide the last window that has been created
    if (aWindowID == -1)
        {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
            {
            gVirtualXRay::LOGGER.logWarning("No active window to hide.") << std::endl;
            }
        // Hide the last created window
        else
            {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
            }
        }

    // There is a window to hide
    if (aWindowID != -1)
        {
        // Find the window
        auto search = g_p_window_set.find(aWindowID);

        // It has been found
        if (search != g_p_window_set.end())
            {
            if (search->second)
                {
                search->second->hideWindow();
                }
            }
        else
            {
            gVirtualXRay::LOGGER.logWarning("Window ") << aWindowID <<
                " not found." << std::endl;
            }
        }
}


//-------------------------------------------------------------------
void setWindowPosition(int aPositionX, int aPositionY, int aWindowID)
//-------------------------------------------------------------------
{
    // Move the last window that has been created
    if (aWindowID == -1)
        {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
            {
            gVirtualXRay::LOGGER.logWarning("No active window to hide.") << std::endl;
            }
        // Move the last created window
        else
            {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
            }
        }

    // There is a window to move
    if (aWindowID != -1)
        {
        // Find the window
        auto search = g_p_window_set.find(aWindowID);

        // It has been found
        if (search != g_p_window_set.end())
            {
            if (search->second)
                {
                search->second->setPosition(aPositionX, aPositionY);
                }
            }
        else
            {
            gVirtualXRay::LOGGER.logWarning("Window ") << aWindowID <<
                " not found." << std::endl;
            }
        }
}


//----------------------------------------------------------------------
void setWindowBackGroundColour(float R, float G, float B, int aWindowID)
//----------------------------------------------------------------------
{
    // Change the colour of the last window that has been created
    if (aWindowID == -1)
        {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
            {
            gVirtualXRay::LOGGER.logWarning("No active window to hide.") << std::endl;
            }
        // Change the colour of the last window that has been created
        else
            {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
            }
        }

    // There is a window to change
    if (aWindowID != -1)
        {
        // Find the window
        auto search = g_p_window_set.find(aWindowID);

        // It has been found
        if (search != g_p_window_set.end())
            {
            if (search->second)
                {
                search->second->setWindowBackGroundColour(R, G, B);
                }
            }
        else
            {
            gVirtualXRay::LOGGER.logWarning("Window ") << aWindowID <<
                " not found." << std::endl;
            }
        }
}

//---------------------------------------------------------------------
void setWindowBackGroundColor(float R, float G, float B, int aWindowID)
//---------------------------------------------------------------------
{
    setWindowBackGroundColour(R, G, B, aWindowID);
}


//---------------------------------------------------------------------------------
const std::vector<std::vector<std::vector<float> > >& takeScreenshot(int aWindowID)
//---------------------------------------------------------------------------------
{
    // Retrieve the window
    if (aWindowID == -1)
    {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
        {
            gVirtualXRay::LOGGER.logWarning("No active window to hide.") << std::endl;
        }
        // Change the colour of the last window that has been created
        else
        {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
        }
    }

    // There is a window to capture
    if (aWindowID != -1)
    {
        // Find the window
        auto search = g_p_window_set.find(aWindowID);

        // It has been found
        if (search != g_p_window_set.end())
        {
            if (search->second)
            {
                // Take the screenshot
                search->second->takeScreenshot();

                // Convert it in a 3D array (e.g. for Python)
                g_p_lastest_screenshot.clear();
                size_t screenshot_width = search->second->getLastScreenshotWidth();
                size_t screenshot_height = search->second->getLastScreenshotHeight();
                const vector<uint8_t>& screenshot = search->second->getLastScreenshot();

                for (size_t j = 0; j < screenshot_height; ++j)
                {
                    g_p_lastest_screenshot.emplace_back( );

                    for (size_t i = 0; i < screenshot_width; ++i)
                    {
                        g_p_lastest_screenshot.back().emplace_back();

                        g_p_lastest_screenshot.back().back().push_back(
                            static_cast<float>(screenshot[(screenshot_height - 1 - j) *
                                screenshot_width * 3 + i * 3 + 0]) / 255.0F);

                        g_p_lastest_screenshot.back().back().push_back(
                            static_cast<float>(screenshot[(screenshot_height - 1 - j) *
                                screenshot_width * 3 + i * 3 + 1]) / 255.0F);

                        g_p_lastest_screenshot.back().back().push_back(
                            static_cast<float>(screenshot[(screenshot_height - 1 - j) *
                                screenshot_width * 3 + i * 3 + 2]) / 255.0F);
                    }
                }
            }
        }
        else
        {
            gVirtualXRay::LOGGER.logWarning("Window ") << aWindowID <<
                " not found." << std::endl;
        }
    }

    // Return the image
    return g_p_lastest_screenshot;
}


//--------------------------------
void copyScreenshot(int aWindowID)
//--------------------------------
{
    // Retrieve the window
    if (aWindowID == -1)
    {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
        {
            gVirtualXRay::LOGGER.logWarning("No active window to hide.") << std::endl;
        }
        // Change the colour of the last window that has been created
        else
        {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
        }
    }

    // There is a window to capture
    if (aWindowID != -1)
    {
        // Find the window
        auto search = g_p_window_set.find(aWindowID);

        // It has been found
        if (search != g_p_window_set.end())
        {
            if (search->second)
            {
                search->second->copyScreenshot();
            }
        }
        else
        {
            gVirtualXRay::LOGGER.logWarning("Window ") << aWindowID <<
                " not found." << std::endl;
        }
    }
}


//-------------------------------------------------------------------------
const std::vector<std::vector<std::vector<float> > >& getLatestScreenshot()
//-------------------------------------------------------------------------
{
    return g_p_lastest_screenshot;
}


//---------------------------------------------------
void saveLastScreenshot(const std::string& aFileName)
//---------------------------------------------------
{
    static int g_screenshot_file_counter = 0;

    // There is no screenshot to save
    if (g_p_lastest_screenshot.empty()) {
        gVirtualXRay::LOGGER.logWarning("No screenshot to save, call takeScreenshot first.");
    }
    // There is a screenshot to save
    else {
        std::string file_name;

        if (!aFileName.empty())
        {
            file_name = aFileName;
        }
        else
        {
            file_name = "screenshot-" + std::to_string(g_screenshot_file_counter) + ".png";
            ++g_screenshot_file_counter;
        }

        if (!gVirtualXRay::isPNG(file_name)) {
            throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
                "Use PNG file to save screenshot");
        }

        // 1. Prepare PNG scanlines
        size_t width = g_p_lastest_screenshot[0].size();
        size_t height = g_p_lastest_screenshot.size();
        std::vector<uint8_t> screenshot;

        // PNG stores each row preceded by a filter byte.
        screenshot.reserve(height * (1 + width * 3));

        for (size_t y = 0; y < height; ++y)
        {
            screenshot.push_back(0); // Filter: None

            for (size_t x = 0; x < width; ++x)
            {
                screenshot.push_back(static_cast<uint8_t>(255.0 * g_p_lastest_screenshot[y][x][0]));
                screenshot.push_back(static_cast<uint8_t>(255.0 * g_p_lastest_screenshot[y][x][1]));
                screenshot.push_back(static_cast<uint8_t>(255.0 * g_p_lastest_screenshot[y][x][2]));
            }
        }

        // 2. Compress with zlib
        uLongf compressed_size = compressBound(screenshot.size());

        std::vector<uint8_t> compressed(compressed_size);

        int ret = compress2(
            compressed.data(),
            &compressed_size,
            screenshot.data(),
            screenshot.size(),
            Z_BEST_COMPRESSION);

        if (ret != Z_OK)
            throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
                "Compression failed");

        compressed.resize(compressed_size);

        // 3. Write IHDR
        std::vector<uint8_t> ihdr(13);

        ihdr[0] = (width >> 24) & 0xff;
        ihdr[1] = (width >> 16) & 0xff;
        ihdr[2] = (width >> 8) & 0xff;
        ihdr[3] = width & 0xff;

        ihdr[4] = (height >> 24) & 0xff;
        ihdr[5] = (height >> 16) & 0xff;
        ihdr[6] = (height >> 8) & 0xff;
        ihdr[7] = height & 0xff;

        ihdr[8]  = 8; // 8 bits/channel
        ihdr[9]  = 2; // RGB
        ihdr[10] = 0; // compression
        ihdr[11] = 0; // filter
        ihdr[12] = 0; // interlace

        // 4. Write the PNG data
        std::ofstream png_file(file_name, std::ios::binary);
        if (!png_file.is_open()) {
            throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
                "Cannot write the PNG file");
        }

        static const uint8_t signature[8] =
        {
            137, 80, 78, 71,
            13, 10, 26, 10
        };

        png_file.write(reinterpret_cast<const char*>(signature), 8);

        PNG_write_chunk(png_file, "IHDR", ihdr);
        PNG_write_chunk(png_file, "IDAT", compressed);
        PNG_write_chunk(png_file, "IEND", {});
    }
}


//----------------------------------------------------
std::vector<uint8_t> getScreenshotAsPNG(int aWindowID)
//----------------------------------------------------
{
    std::vector<uint8_t> png_data;

    // Retrieve the window
    if (aWindowID == -1)
    {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
        {
            gVirtualXRay::LOGGER.logWarning("No active window to hide.") << std::endl;
        }
        // Change the colour of the last window that has been created
        else
        {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
        }
    }

    // There is a window to capture
    if (aWindowID != -1)
    {
        // Find the window
        auto search = g_p_window_set.find(aWindowID);

        // It has been found
        if (search != g_p_window_set.end())
        {
            if (search->second)
            {
                // Refresh the display
                displayScene();

                // Take the screenshot
                search->second->takeScreenshot();

                // 1. Prepare PNG scanlines
                size_t screenshot_width = search->second->getLastScreenshotWidth();
                size_t screenshot_height = search->second->getLastScreenshotHeight();
                const std::vector<uint8_t>& input_screenshot = search->second->getLastScreenshot();

                // PNG stores each row preceded by a filter byte.
                std::vector<uint8_t> output_screenshot;
                output_screenshot.reserve(screenshot_height * (1 + screenshot_width * 3));

                for (size_t j = 0; j < screenshot_height; ++j)
                {
                    output_screenshot.push_back(0); // Filter: None

                    for (size_t i = 0; i < screenshot_width; ++i)
                    {
                        output_screenshot.push_back(
                            input_screenshot[((screenshot_height - 1 - j) * screenshot_width + i) * 3]);
                        output_screenshot.push_back(
                            input_screenshot[((screenshot_height - 1 - j) * screenshot_width + i) * 3 + 1]);
                        output_screenshot.push_back(
                            input_screenshot[((screenshot_height - 1 - j) * screenshot_width + i) * 3 + 2]);
                    }
                }

                // 2. Compress with zlib
                uLongf compressed_size = compressBound(output_screenshot.size());

                std::vector<uint8_t> compressed(compressed_size);

                int ret = compress2(
                    compressed.data(),
                    &compressed_size,
                    output_screenshot.data(),
                    output_screenshot.size(),
                    Z_BEST_SPEED);

                if (ret != Z_OK)
                    throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
                        "Compression failed");

                compressed.resize(compressed_size);

                // 3. Write IHDR
                std::vector<uint8_t> ihdr(13);

                ihdr[0] = (screenshot_width >> 24) & 0xff;
                ihdr[1] = (screenshot_width >> 16) & 0xff;
                ihdr[2] = (screenshot_width >> 8) & 0xff;
                ihdr[3] = screenshot_width & 0xff;

                ihdr[4] = (screenshot_height >> 24) & 0xff;
                ihdr[5] = (screenshot_height >> 16) & 0xff;
                ihdr[6] = (screenshot_height >> 8) & 0xff;
                ihdr[7] = screenshot_height & 0xff;

                ihdr[8]  = 8; // 8 bits/channel
                ihdr[9]  = 2; // RGB
                ihdr[10] = 0; // compression
                ihdr[11] = 0; // filter
                ihdr[12] = 0; // interlace

                // 4. Write the PNG data

                // The signature
                static const uint8_t signature[8] =
                {
                    137, 80, 78, 71,
                    13, 10, 26, 10
                };

                // Add elements at the end of the vector
                png_data.insert(png_data.end(), std::begin(signature), std::end(signature));


                PNG_write_chunk(png_data, "IHDR", ihdr);
                PNG_write_chunk(png_data, "IDAT", compressed);
                PNG_write_chunk(png_data, "IEND", {});

            }
        }
        else
        {
            gVirtualXRay::LOGGER.logWarning("Window ") << aWindowID <<
                " not found." << std::endl;
        }
    }

    return png_data;
}


//-----------------------------------------------
std::vector<std::vector< float> > getWhiteImage(bool anIntegrateEnergyFlag)
//-----------------------------------------------
{
    return (getVectorImage2D<float>(g_xray_renderer.getWhiteImage(anIntegrateEnergyFlag)));
}


//------------------------------------------------------------------------------------------
std::vector<std::vector< unsigned short> > getWhiteImageWithGain(bool anIntegrateEnergyFlag)
//------------------------------------------------------------------------------------------
{
    return (getVectorImage2D<unsigned short>(g_xray_renderer.getWhiteImageWithGain(anIntegrateEnergyFlag)));
}


//---------------------------------------------------------------------
std::vector<std::vector<std::vector< float> > > getWhiteSpectralImage()
//---------------------------------------------------------------------
{
    std::vector<std::vector<std::vector< float> > > p_white_image_set;

    for (unsigned int i = 0; i < g_xray_beam.getNumberOfPhotonCountingBands(); ++i)
    {
        g_xray_beam.activatePhotonCountingBand(i);
        p_white_image_set.push_back(getVectorImage2D<float>(g_xray_renderer.getWhiteImage(false)));
    }

    return p_white_image_set;
}


//---------------------------------------------------------------------
std::vector<std::vector<std::vector< unsigned short> > > getWhiteSpectralImageWithGain()
//---------------------------------------------------------------------
{
    throw gVirtualXRay::NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
    // std::vector<std::vector<std::vector< float> > > p_white_image_set;
    //
    // for (unsigned int i = 0; i < g_xray_beam.getNumberOfPhotonCountingBands(); ++i)
    // {
    //     g_xray_beam.activatePhotonCountingBand(i);
    //     p_white_image_set.push_back(getVectorImage2D<float>(g_xray_renderer.getWhiteImage(false)));
    // }
    //
    // return p_white_image_set;
}


//--------------------------------------------
void saveSTLfile(const std::string& aLabel,
                 const std::string& aFileName,
                 bool aBinaryFlag)
//--------------------------------------------
{
	// Create the file name
	std::string file_name;
	if (aFileName.size() == 0)
	{
		file_name = aLabel + ".stl";
	}
	else
	{
		file_name = aFileName;
	}

	// Get the scenegraph
    gVirtualXRay::SceneGraphBinder* p_temp = g_p_scene_graph.get();
    if (!p_temp)
        {
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Get the node
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // The node exists
    if (p_node)
        {
        // Set the file name
    	p_node->getPolygonMesh().setFileName(file_name);

        // save the file
    	p_node->getPolygonMesh().writeSTLFile(aBinaryFlag);
        }
    // The node does not exist
    else
        {
		gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
			aLabel <<
			" not found in g_p_polygon_mesh_set." <<
		    std::endl;
        }
}



//-----------------------------------------------------------
void setPhotonCountingBands(double aMinEnergy,
                            double aMaxEnergy,
                            const std::string& aUnitOfEnergy,
                            unsigned int aNumberOfBands)
//-----------------------------------------------------------
{
    g_xray_beam.setPhotonCountingBands(
        aMinEnergy * getUnitOfEnergy(aUnitOfEnergy),
        aMaxEnergy * getUnitOfEnergy(aUnitOfEnergy),
        aNumberOfBands);
    requestXRayImageUpdate();
}


//-------------------------------------------
unsigned int getNumberOfPhotonCountingBands()
//-------------------------------------------
{
    return g_xray_beam.getNumberOfPhotonCountingBands();
}


//-----------------------------
void mergePhotonCountingBands()
//-----------------------------
{
    g_xray_beam.mergePhotonCountingBands();
    requestXRayImageUpdate();
}



//--------------------------------------------------------------------------
std::vector<double> getPhotonCountingBands(const std::string& aUnitOfEnergy)
//--------------------------------------------------------------------------
{
    std::vector<double> p_temp;
    double energy = getUnitOfEnergy(aUnitOfEnergy);

    for (std::vector<gVirtualXRay::RATIONAL_NUMBER>::const_iterator ite = g_xray_beam.getPhotonCountingBands().begin();
        ite != g_xray_beam.getPhotonCountingBands().end();
        ++ite)
    {
        p_temp.push_back(*ite / energy);
    }

    return (p_temp);
}






//------------------------------------------------------------------------------------------
std::vector<std::pair<unsigned int, float> > getFiltration(const std::string& aUnitOfLength)
//------------------------------------------------------------------------------------------
{
    std::vector<std::pair<unsigned int, float> > p_filtration;

    for (std::vector<std::pair<unsigned int, gVirtualXRay::RATIONAL_NUMBER> >::const_iterator ite(g_xray_beam.getFiltration().begin());
        ite != g_xray_beam.getFiltration().end();
        ++ite)
    {
        p_filtration.push_back(std::pair<unsigned int, float>(ite->first, float(ite->second / getUnitOfLength(aUnitOfLength))));
    }

    return p_filtration;
}



//----------------------------------------------
unsigned int getMaxRadiusLbufferArtefactFilter()
//----------------------------------------------
{
    return g_xray_renderer.getMaxRadiusLbufferArtefactFilter();
}


//------------------------------------
bool isArtefactFilteringOnGPUEnabled()
//------------------------------------
{
    return g_xray_renderer.useArtefactFilteringOnGPU();
}


//------------------------------------
bool isArtefactFilteringOnCPUEnabled()
//------------------------------------
{
    return g_xray_renderer.useArtefactFilteringOnCPU();
}


//----------------------------------------------------------
void setMaxRadiusLbufferArtefactFilter(unsigned int aRadius)
//----------------------------------------------------------
{
    g_xray_renderer.setMaxRadiusLbufferArtefactFilter(aRadius);
    requestXRayImageUpdate();
}




int getImageWidth(const std::vector<std::vector<float> >& anImage) {
    int image_width = 0;

    if (getImageHeght(anImage)) {
    image_width = anImage[0].size();
    }

    return (image_width);
}

int getImageHeght(const std::vector<std::vector<float> >& anImage) {
    return (anImage.size());
}

std::vector<float> getImageRow(const std::vector<std::vector<float> >& anImage,
    int j) {
    if (j >= getImageHeght(anImage)) {
    throw gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__,
    __LINE__);
    }

    return (anImage[j]);
}


void pushModelViewMatrix() {
    // Store the current transformation matrix
    gVirtualXRay::pushModelViewMatrix();
}

void popModelViewMatrix() {
    // Store the current transformation matrix
    gVirtualXRay::popModelViewMatrix();
}

//----------------------------------------------------------------
void rotateModelView(double anAngle, double x, double y, double z)
//----------------------------------------------------------------
{
    gVirtualXRay::Matrix4x4 < GLfloat > rotation_matrix;
    rotation_matrix.rotate(anAngle, gVirtualXRay::Vec3 < GLfloat > (x, y, z));

    gVirtualXRay::g_current_modelview_matrix *= rotation_matrix;
}


gVirtualXRay::Image<float> getImage(const std::vector<std::vector<float> >& anImage)
{
    gVirtualXRay::Image<float> output(anImage[0].size(), anImage.size());

    for (unsigned int j = 0; j < anImage.size(); ++j)
    {
        for (unsigned int i = 0; i < anImage[0].size(); ++i)
        {
            output.setPixel(i, j, 0, anImage[j][i]);
        }
    }

    return output;
}


gVirtualXRay::Image<float> getImage(const std::vector<std::vector<std::vector<float> > >& anImage)
{
    gVirtualXRay::Image<float> output(anImage[0][0].size(), anImage[0].size(), anImage.size());

    for (unsigned int k = 0; k < anImage.size(); ++k)
    {
        for (unsigned int j = 0; j < anImage[0].size(); ++j)
        {
			for (unsigned int i = 0; i < anImage[0][0].size(); ++i)
			{
				output.setPixel(i, j, k, anImage[k][j][i]);
			}
		}
    }

    return output;
}


//---------------------------------------------------------------------------------------
gVirtualXRay::MATRIX4 Vector2gvxrMatrix(const std::vector<std::vector<float> >& aMatrix4)
//---------------------------------------------------------------------------------------
{
    if (aMatrix4.size() == 4)
    {
        if (aMatrix4[0].size() == 4 && aMatrix4[1].size() == 4 && aMatrix4[2].size() == 4 && aMatrix4[3].size() == 4)
        {
            return gVirtualXRay::MATRIX4(aMatrix4[0][0], aMatrix4[0][1],
                aMatrix4[0][2], aMatrix4[0][3], aMatrix4[1][0],
                aMatrix4[1][1], aMatrix4[1][2], aMatrix4[1][3],
                aMatrix4[2][0], aMatrix4[2][1], aMatrix4[2][2],
                aMatrix4[2][3], aMatrix4[3][0], aMatrix4[3][1],
                aMatrix4[3][2], aMatrix4[3][3]);
        }
    }

    throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
    "Wrong matrix size");
}

std::vector<std::vector<float> > gvxrMatrix2Vector(
    const gVirtualXRay::MATRIX4& aMatrix4) {
    std::vector < std::vector<float> > output;
    output.push_back(std::vector<float>(4));
    output.push_back(std::vector<float>(4));
    output.push_back(std::vector<float>(4));
    output.push_back(std::vector<float>(4));

    output[0][0] = aMatrix4.get4x4()[0];
    output[0][1] = aMatrix4.get4x4()[1];
    output[0][2] = aMatrix4.get4x4()[2];
    output[0][3] = aMatrix4.get4x4()[3];
    output[1][0] = aMatrix4.get4x4()[4];
    output[1][1] = aMatrix4.get4x4()[5];
    output[1][2] = aMatrix4.get4x4()[6];
    output[1][3] = aMatrix4.get4x4()[7];
    output[2][0] = aMatrix4.get4x4()[8];
    output[2][1] = aMatrix4.get4x4()[9];
    output[2][2] = aMatrix4.get4x4()[10];
    output[2][3] = aMatrix4.get4x4()[11];
    output[3][0] = aMatrix4.get4x4()[12];
    output[3][1] = aMatrix4.get4x4()[13];
    output[3][2] = aMatrix4.get4x4()[14];
    output[3][3] = aMatrix4.get4x4()[15];

    return output;
}

//------------------------------------------
std::string getElementName(unsigned short Z)
//------------------------------------------
{
    return (gVirtualXRay::ElementSet::getInstance().getElement(Z).getName());
}

//--------------------------------------------
std::string getElementSymbol(unsigned short Z)
//--------------------------------------------
{
    return (gVirtualXRay::ElementSet::getInstance().getElement(Z).getSymbol());
}


//----------------------------------------------------
std::string getElementName(const std::string& aSymbol)
//----------------------------------------------------
{
  return (gVirtualXRay::ElementSet::getInstance().getElement(aSymbol).getName());
}


//-----------------------------------------------------------------
unsigned short getElementAtomicNumber(const std::string& anElement)
//-----------------------------------------------------------------
{
    return (gVirtualXRay::ElementSet::getInstance().getElement(anElement).
        getZ());
}

//-----------------------------------------------------
float getElementMolarMass(const std::string& anElement)
//-----------------------------------------------------
{
    return (gVirtualXRay::ElementSet::getInstance().getElement(anElement).
        getMolarMass());
}


double getDensityFromElement(const std::string& anElement) {
    unsigned short Z = getElementAtomicNumber(anElement);
    return (getDensityFromElement(Z));
}

double getDensityFromElement(unsigned short Z) {
    return (gVirtualXRay::ElementSet::getInstance().getElement(Z).getDensity()
    / (gVirtualXRay::g / gVirtualXRay::cm3));
}


//-------------------------------
double getDensityFromHU(double HU)
//-------------------------------
{
    return (gVirtualXRay::XRayRenderer::getDensityFromHU(HU)
    / (gVirtualXRay::g / gVirtualXRay::cm3));
}


//------------------------------------------------------------
double getMassAttenuationFromElement(const std::string& anElement,
		double anEnergy,
				const std::string& aUnitOfEnergy)
//------------------------------------------------------------
{
    unsigned short Z = getElementAtomicNumber(anElement);
    return getMassAttenuationFromElement(Z, anEnergy, aUnitOfEnergy);
}


//------------------------------------------------
double getMassAttenuationFromElement(unsigned short Z,
		double anEnergy,
				const std::string& aUnitOfEnergy)
//------------------------------------------------
{
    return (gVirtualXRay::ElementSet::getInstance().getElement(Z).getMassAttenuationTotal(anEnergy * getUnitOfEnergy(aUnitOfEnergy))
    / (gVirtualXRay::cm2 / gVirtualXRay::g));
}


//-------------------------------------------
double getMassAttenuationFromHU(double HU,
		double anEnergy,
				const std::string& aUnitOfEnergy)
//-------------------------------------------
{
    // Initialise the photon cross-section
    gVirtualXRay::PhotonCrossSection photon_cross_section;
    photon_cross_section.useHU(HU);

	return (photon_cross_section.getMassAttenuationCoefficient(
			anEnergy * getUnitOfEnergy(aUnitOfEnergy)) /
				(gVirtualXRay::cm2 / gVirtualXRay::g));
}


//-----------------------------------------------------------
double getMassAttenuationFromMixture(const std::string& aMixture,
		double anEnergy,
				const std::string& aUnitOfEnergy)
//-----------------------------------------------------------
{
    // Initialise the photon cross-section
    gVirtualXRay::PhotonCrossSection photon_cross_section;
    photon_cross_section.useMixture(aMixture);

	return (photon_cross_section.getMassAttenuationCoefficient(
			anEnergy * getUnitOfEnergy(aUnitOfEnergy)) /
				(gVirtualXRay::cm2 / gVirtualXRay::g));
}


//-------------------------------------------------------------
double getMassAttenuationFromCompound(const std::string& aCompound,
		double anEnergy,
				const std::string& aUnitOfEnergy)
//-------------------------------------------------------------
{
    // Initialise the photon cross-section
    gVirtualXRay::PhotonCrossSection photon_cross_section;
    photon_cross_section.useCompound(aCompound);

	return (photon_cross_section.getMassAttenuationCoefficient(
			anEnergy * getUnitOfEnergy(aUnitOfEnergy)) /
				(gVirtualXRay::cm2 / gVirtualXRay::g));
}


double getMuFromHU(double HU,
		double anEnergy,
		const std::string& aUnitOfEnergy)
{
	return (gVirtualXRay::XRayRenderer::getMuFromHU(HU, anEnergy * getUnitOfEnergy(aUnitOfEnergy)) / (1.0 / gVirtualXRay::cm));
}



std::vector<std::vector<std::vector< float> > > convertHU2mu(const std::vector<std::vector<std::vector< float> > >& anImage,
    double anEnergy,
    const std::string& aUnitOfEnergy)
{
    // Number of slices
    int image_depth = anImage.size();

    // Create an empty image
    std::vector<std::vector<std::vector< float> > > output_image;

    if (image_depth > 0)
    {
        int image_height = anImage[0].size();

        if (image_height > 0)
        {
            int image_width = anImage[0][0].size();

            if (image_width > 0)
            {
                // Allocate the data
                output_image.resize(image_depth);

                for (int z = 0; z < image_depth; ++z)
                {
                    // Allocate the rows
                    output_image[z].resize(image_height);

                    for (int y = 0; y < image_height; ++y)
                    {
                        // Allocate the columns
                        output_image[z][y].resize(image_width);
                    }
                }

#ifdef NDEBUG
#ifdef WIN32
#pragma omp parallel for
#else
#pragma omp parallel for collapse(3)
#endif
#endif
                for (int z = 0; z < image_depth; ++z)
                {
                    for (int y = 0; y < image_height; ++y)
                    {
                        for (int x = 0; x < image_width; ++x)
                        {
                            output_image[z][y][x] = getMuFromHU(anImage[z][y][x],
                                                                anEnergy,
                                                                aUnitOfEnergy);
                        }
                    }
                }
            }
        }
    }

    return output_image;
}



std::vector<float> rayIntersect(const std::string& aLabel,
    double aRayOriginX, double aRayOriginY, double aRayOriginZ,
    double aRayDirectionX, double aRayDirectionY, double aRayDirectionZ)
{
    std::vector<float> p_intersection_set;
    gVirtualXRay::SceneGraphNode* p_source_node = 0;

    // The scenegraph exists
    if (g_p_scene_graph)
    {
        // Find the node. It must exist
        gVirtualXRay::SceneGraphNode* p_source_node =
            g_p_scene_graph->getNode(aLabel);

        // The node exists
        if (p_source_node)
        {
            // Ray tracing
            std::map<float, gVirtualXRay::VEC3> p_intersection_full_set = p_source_node->rayIntersect(
            		gVirtualXRay::VEC3(aRayOriginX, aRayOriginY, aRayOriginZ),
			        gVirtualXRay::VEC3(aRayDirectionX, aRayDirectionY, aRayDirectionZ),
			        gVirtualXRay::MATRIX4());

			// Iterate through the map
			for (std::map<float, gVirtualXRay::VEC3>::const_iterator ite = p_intersection_full_set.begin();
			         ite != p_intersection_full_set.end();
			         ++ite)
			{
    			p_intersection_set.push_back(ite->first);
			}
        }
    }

    // The source does not exist
    if (!p_source_node)
    {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
    }

    return p_intersection_set;
}















void setShiftFilter(double aValue) {
    g_shift_filter = aValue;
}

void setScaleFilter(double aValue) {
    g_scale_filter = aValue;
}

double getShiftFilter() {
    return (g_shift_filter);
}

double getScaleFilter() {
    return (g_scale_filter);
}


std::vector<std::vector<float> > loadImage2D(const std::string& aFileName)
{
    return getVectorImage2D<float>(gVirtualXRay::Image<float>(aFileName));
}


std::vector<std::vector<std::vector<float> > > loadImage3D(const std::string& aFileName)
{
    return getVectorImage3D<float>(gVirtualXRay::Image<float>(aFileName));
}


double computeZNCC(const std::vector<std::vector<std::vector<float> > >& aReferenceImage, const std::vector<std::vector<std::vector<float> > >& aTestImage)
{
    gVirtualXRay::Image<float> ref = getImage(aReferenceImage);
    gVirtualXRay::Image<float> test = getImage(aTestImage);

    return ref.computeNCC(test);
}


double computeRMSE(const std::vector<std::vector<std::vector<float> > >& aReferenceImage, const std::vector<std::vector<std::vector<float> > >& aTestImage)
{
    gVirtualXRay::Image<float> ref = getImage(aReferenceImage);
    gVirtualXRay::Image<float> test = getImage(aTestImage);

    return ref.computeRMSE(test);
}


double getMean(const std::vector<float>& anImage)
{
    double avg = 0.0;

#pragma omp parallel for reduction( + : avg )
#ifdef _WIN32
    for (int i = 0; i < anImage.size(); ++i)
#else
    for (size_t i = 0; i < anImage.size(); ++i)
#endif
    {
        avg += anImage[i];
    }

    return avg / anImage.size();
}

double getStddev(const std::vector<float>& anImage)
{
    double avg = getMean(anImage);
    double stddev = 0.0;

#pragma omp parallel for reduction( + : stddev )
#ifdef _WIN32
    for (int i = 0; i < anImage.size(); ++i)
#else
    for (size_t i = 0; i < anImage.size(); ++i)
#endif
    {
        stddev += std::pow(anImage[i] - avg, 2);
    }

    return sqrt(stddev / anImage.size());
}


double getStddev(const std::vector<float>& anImage, double avg)
{
    double stddev = 0.0;

#pragma omp parallel for reduction( + : stddev )
#ifdef _WIN32
    for (int i = 0; i < anImage.size(); ++i)
#else
    for (size_t i = 0; i < anImage.size(); ++i)
#endif
    {
        stddev += std::pow(anImage[i] - avg, 2);
    }

    return sqrt(stddev / anImage.size());
}

double computeZNCC(const std::vector<float>& aReferenceImage, const std::vector<float>& aTestImage)
{
    double avg_ref = getMean(aReferenceImage);
    double avg_test = getMean(aTestImage);

    double stddev_ref = getStddev(aReferenceImage, avg_ref);
    double stddev_test = getStddev(aTestImage, avg_test);

    double zncc = 0.0;
#pragma omp parallel for reduction( + : zncc )
#ifdef _WIN32
    for (int i = 0; i < aReferenceImage.size(); ++i)
#else
    for (size_t i = 0; i < aReferenceImage.size(); ++i)
#endif
    {
        zncc += (aReferenceImage[i] - avg_ref) * (aTestImage[i] - avg_test);
    }

    return (zncc / (stddev_ref * stddev_test)) / aReferenceImage.size();
}


double computeRMSE(const std::vector<float>& aReferenceImage, const std::vector<float>& aTestImage, bool normalise)
{
    double metrics = 0.0;

    double avg_ref = getMean(aReferenceImage);

#pragma omp parallel for reduction( + : metrics )
#ifdef _WIN32
    for (int i = 0; i < aReferenceImage.size(); ++i)
#else
    for (size_t i = 0; i < aReferenceImage.size(); ++i)
#endif
    {
        metrics += std::pow(aReferenceImage[i] - aTestImage[i], 2);
    }

    metrics = sqrt(metrics);

    if (normalise) metrics /= avg_ref;

    return metrics;
}


std::vector<float> g_p_ref_image;
std::vector<float> g_p_test_image;

double computeRMSE(bool normalise)
{
    return computeRMSE(g_p_ref_image, g_p_test_image, normalise);
}


double computeZNCC()
{
    return computeZNCC(g_p_ref_image, g_p_test_image);
}


void loadReference(const std::vector<float>& aReferenceImage)
{
    g_p_ref_image = aReferenceImage;
}


void loadTest(const std::vector<float>& aTestImage)
{
    g_p_test_image = aTestImage;
}


void lineariseTest(float aThreshold, float aScalingFactor)
{
#pragma omp parallel for
#ifdef _WIN32
    for (int i = 0; i < g_p_test_image.size(); ++i)
#else
    for (size_t i = 0; i < g_p_test_image.size(); ++i)
#endif
    {
        if (g_p_test_image[i] < aThreshold)
        {
            g_p_test_image[i] = aThreshold;
        }

        g_p_test_image[i] *= aScalingFactor;
        g_p_test_image[i] = -log(g_p_test_image[i]);
    }
}


float getMinValue(const std::vector<std::vector<std::vector<float> > >& aImage)
{
    return getImage(aImage).getMinValue();
}


float getMaxValue(const std::vector<std::vector<std::vector<float> > >& aImage)
{
    return getImage(aImage).getMaxValue();
}

/*
void FBO2Thrust(unsigned int aFBO, void* aThrustVector, int anOffset)
{
#ifndef HAS_CUDA
    throw gVirtualXRay::Exception(__FILE__,
                                  __FUNCTION__,
                                  __LINE__,
                                  "CUDA is not supported.");
#else
    cudaGLSetGLDevice(0);
    struct cudaGraphicsResource *fbo_cuda;
    cudaGraphicsGLRegisterBuffer(&fbo_cuda, g_xray_renderer.getFboId(aFBO), cudaGraphicsMapFlagsReadOnly);


    thrust::device_ptr<float4>* p_device_data = static_cast<thrust::device_ptr<float4>*>(aThrustVector);
#endif
}*/
