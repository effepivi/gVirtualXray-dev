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
#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <cmath>

#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype> // Needed for std::toupper
#include <cmath> // Header file for abs
#include <random>


#define GLFW_INCLUDE_GLCOREARB 1

#ifdef HAS_VULKAN
#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#endif

#include <GLFW/glfw3.h>


// To perform the computations of the X-ray simulation
#ifndef __XRayRenderer_h
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef __Scintillator_h
#include "gVirtualXRay/Scintillator.h"
#endif


// For polygon meshes
#ifndef __PolygonMesh_h
#include "gVirtualXRay/PolygonMesh.h"
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

#ifndef __CuboidMesh_h
#include "gVirtualXRay/CuboidMesh.h"
#endif

#ifndef __CylinderMesh_h
#include "gVirtualXRay/CylinderMesh.h"
#endif

#ifndef __OutOfBoundsException_h
#include "gVirtualXRay/OutOfBoundsException.h"
#endif

#ifndef __ElementSet_h
#include "gVirtualXRay/ElementSet.h"
#endif

#ifndef __PhotonCrossSection_h
#include "gVirtualXRay/PhotonCrossSection.h"
#endif

#ifndef __ResourceManager_h
#include <gVirtualXRay/ResourceManager.h>
#endif

#ifndef __Logger_h
#include <gVirtualXRay/Logger.h>
#endif

#ifndef __ResourceManager_h
#include "gVirtualXRay/ResourceManager.h"
#endif

#ifndef __CoordinateSystemMesh_h
#include "gVirtualXRay/CoordinateSystemMesh.h"
#endif


#ifndef __SimpleGVXR_h
#include "SimpleGVXR.h"
#endif

#ifndef __internal_h
#include "internal.h"
#endif

#ifndef __Window_h
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


//******************************************************************************
//  Namespace
//******************************************************************************
using namespace std;


//******************************************************************************
//  Function declarations
//******************************************************************************
GLFWwindow* initGLFW(int aWindowID, int aVisibilityFlag);




void computeRotation(gVirtualXRay::MATRIX4& aRotationMatrix,
    int aViewportWidth,
    int aViewportHeight);

gVirtualXRay::Vec3<GLfloat> getArcballVector(int x,
    int y,
    int aViewportWidth,
    int aViewportHeight);

float radian2degree(float anAngle);

std::vector<std::vector<float> > getVectorImage2D(
    const gVirtualXRay::Image<float>& anImage);


gVirtualXRay::Image<float> getImage(const std::vector<std::vector<float> >& anImage);

gVirtualXRay::Image<float> getImage(const std::vector<std::vector<std::vector<float> > >& anImage);

gVirtualXRay::MATRIX4 Vector2gvxrMatrix(
    const std::vector<std::vector<float> >& aNumpyMatrix);

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
const GLfloat ROTATION_SPEED(2.0);

std::map<int, SimpleGVXR::Window*> g_p_window_set;      // The set of windows
std::vector<int> g_p_window_id_stack;           // The stack of window IDs

double g_initial_fovy(45);                // Field of view along
                                                // the y-axis
double g_initial_near(5.0 * gVirtualXRay::cm);  // Near clipping plane
double g_initial_far(500.0 * gVirtualXRay::cm); // Far clipping plane
GLfloat g_zoom(250.0 * gVirtualXRay::cm);       // How far is the camera from
                                                // the look at point
// State variables
bool g_run_loop = false;
bool g_display_beam(true);
bool g_display_wireframe(false);
bool g_use_lighing(true);
bool g_display_detector(true);
bool g_display_normal_vectors(false);

bool g_beam_shape_initialised = false;
bool g_beam_energy_initialised = false;
bool g_source_position_initialised = false;
bool g_detector_position_initialised = false;
bool g_detector_up_vector_initialised = false;
bool g_detector_size_initialised = false;
bool g_detector_pixel_size_initialised = false;
bool g_renderer_initialised = false;

int g_button(-1);
int g_button_state(-1);
bool g_use_arc_ball(false);
GLint g_last_x_position(0);
GLint g_last_y_position(0);
GLint g_current_x_position(0);
GLint g_current_y_position(0);

gVirtualXRay::XRayBeam g_xray_beam;         // X-ray beam
gVirtualXRay::XRayDetector g_xray_detector; // X-ray detector
gVirtualXRay::XRayRenderer g_xray_renderer; // X-ray graphics engine

gVirtualXRay::Shader g_display_shader;      // Visualisation shader

// Scenegraph of scanned objects
std::shared_ptr<gVirtualXRay::SceneGraphBinder> g_p_scene_graph = 0;

// Rotate the whole 3-D view
gVirtualXRay::Matrix4x4<GLfloat> g_scene_rotation_matrix;

// Translate the whole 3-D view
gVirtualXRay::Matrix4x4 < GLfloat > g_scene_translation_matrix;

// Rotate the scanned objects
gVirtualXRay::Matrix4x4<GLfloat> g_sample_rotation_matrix;

// Bias/gain of the shift/scale filter for the display of the X-ray image
double g_shift_filter = 0;
double g_scale_filter = 1;

bool g_has_own_gl_context = false;


std::vector<std::vector<std::vector< float> > > lastest_screenshot;

gVirtualXRay::CoordinateSystemMesh g_world_coordinate_system_mesh(2 * gVirtualXRay::cm);

std::random_device g_random_device;  // Will be used to obtain a seed for the random number engine
std::mt19937 g_random_generator(g_random_device()); // Standard mersenne_twister_engine seeded with rd()


//******************************************************************************
//  Function definitions
//******************************************************************************

//-----------------------------
int getMajorVersionOfCoreGVXR()
//-----------------------------
{
    return (atoi(gVirtualXRay_VERSION_MAJOR));
}

//-----------------------------
int getMinorVersionOfCoreGVXR()
//-----------------------------
{
    return (atoi(gVirtualXRay_VERSION_MINOR));
}

//-----------------------------
int getPatchVersionOfCoreGVXR()
//-----------------------------
{
    return (atoi(gVirtualXRay_VERSION_PATCH));
}

//--------------------------------
std::string getVersionOfCoreGVXR()
//--------------------------------
{
    return (std::string(gvxr_VERSION));
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
    return (std::string(SimpleGVXR_VERSION_STRING));
}


//-----------------------------
std::string getOpenGlRenderer()
//-----------------------------
{
    return std::string(reinterpret_cast<const char *>(glGetString(GL_RENDERER)));
}


//----------------------------
std::string getOpenGlVersion()
//----------------------------
{
    return std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION)));
}


//---------------------------
std::string getOpenGlVendor()
//---------------------------
{
    return std::string(reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
}


//-----------------------------
std::string getNoiseGenerator()
//-----------------------------
{
#ifdef HAS_GSL
    #ifdef HAS_OPENMP
        return "GSL with OpenMP";
    #else
        return "GSL without OpenMP";
    #endif
#else
    #ifdef HAS_OPENMP
        return "C++ standard library with OpenMP";
    #else
        return "C++ standard library without OpenMP";
    #endif
#endif
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


//---------------------------------------------
void setSeed(unsigned int aRandomGeneratorSeed)
//---------------------------------------------
{
    g_random_generator = std::mt19937(aRandomGeneratorSeed);
}


//---------------------------
void displayBeam(bool aState)
//---------------------------
{
    g_display_beam = aState;
}


//-------------------------------
void displayDetector(bool aState)
//-------------------------------
{
    g_display_detector = aState;
}


//------------------------------------
void displayNormalVectors(bool aState)
//------------------------------------
{
    g_display_normal_vectors = aState;
}


//----------------------------
void useWireframe(bool aState)
//----------------------------
{
    g_display_wireframe = aState;
}


//--------------------------
void useLighing(bool aState)
//--------------------------
{
    g_use_lighing = aState;
}


//---------------------------
void useNegative(bool aState)
//---------------------------
{
    g_xray_renderer.useNegativeFilteringFlag(aState);
}


//------------------------------------------------------------
void setAxisLength(float aLength, const std::string& aUnitOfLength)
//------------------------------------------------------------
{
    g_world_coordinate_system_mesh.setLength(aLength * getUnitOfLength(aUnitOfLength));
}




//--------------------------------------
void autoCreateOpenGLContext(bool aFlag)
//--------------------------------------
{
    g_has_own_gl_context = !aFlag;
}


//------------------------------------------------------
void setSourcePosition(double x,
                       double y,
                       double z,
                       const std::string& aUnitOfLength)
//------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    // Save the current X-ray source shape
    gVirtualXRay::XRayDetector::SourceShape source_shape_backup = g_xray_detector.getSourceShape();

    // Update the position
    g_xray_detector.setXrayPointSource(
            gVirtualXRay::VEC3(x * unit_of_length, y * unit_of_length,
                    z * unit_of_length));

    // The position has been initialised
    g_source_position_initialised = true;

    // Restore the X-ray source shape
    switch (source_shape_backup)
    {
      case gVirtualXRay::XRayDetector::POINT:
        g_xray_detector.setXrayPointSource();
        break;

      case gVirtualXRay::XRayDetector::PARALLEL:
        g_xray_detector.setParallelBeam();
        break;

      case gVirtualXRay::XRayDetector::SQUARE:
        throw gVirtualXRay::Exception(__FILE__,
                                      __FUNCTION__,
                                      __LINE__,
                                      "This method should not be used with a square source, call `setFocalSpotAsSquare` instead.");

      case gVirtualXRay::XRayDetector::RECTANGLE:
        throw gVirtualXRay::Exception(__FILE__,
                                      __FUNCTION__,
                                      __LINE__,
                                      "This method should not be used with a square source, call `setFocalSpotAsRectangle` instead.");

      case gVirtualXRay::XRayDetector::CUBE:
        throw gVirtualXRay::Exception(__FILE__,
                                      __FUNCTION__,
                                      __LINE__,
                                      "This method should not be used with a cube source, call `setFocalSpot` instead");

      case gVirtualXRay::XRayDetector::LINE:
        throw gVirtualXRay::Exception(__FILE__,
                                      __FUNCTION__,
                                      __LINE__,
                                      "This method should not be used with a line source.");

      default: // This case should never occur, but just in case
        g_xray_detector.setXrayPointSource();
    }

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}


//--------------------------
std::string getSourceShape()
//--------------------------
{
    switch (g_xray_detector.getSourceShape())
    {
    case gVirtualXRay::XRayDetector::POINT:
        return std::string("POINT");

    case gVirtualXRay::XRayDetector::PARALLEL:
        return std::string("PARALLEL");

    case gVirtualXRay::XRayDetector::SQUARE:
        return std::string("SQUARE");

    case gVirtualXRay::XRayDetector::CUBE:
        return std::string("CUBE");

    case gVirtualXRay::XRayDetector::LINE:
        return std::string("LINE");

    default: // This case should never occur, but just in case
        return std::string("UNKNOWN");
    }
}




//---------------------------------------------------------------------
std::vector<double> getSourcePosition(const std::string& aUnitOfLength)
//---------------------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    std::vector<double> position(3);

    position[0] = g_xray_detector.getXraySourceCentre()[0] / unit_of_length;
    position[1] = g_xray_detector.getXraySourceCentre()[1] / unit_of_length;
    position[2] = g_xray_detector.getXraySourceCentre()[2] / unit_of_length;

    return (position);
}


//-----------------------------------------
void displayNode(const std::string& aLabel)
//-----------------------------------------
{
    std::vector<float> colour(4, 1.0);

    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
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
            endl;
        }
}


//-----------------
void moveToCentre()
//-----------------
{
    if (g_p_scene_graph.get())
        {
        g_p_scene_graph->moveToCentre();
        }
    else
        {
        gVirtualXRay::LOGGER.logWarning("The scenegraph is empty.") << endl;
        }
}

//-----------------
void moveToCenter()
//-----------------
{
    moveToCentre();
}

//-----------------------------------
void moveToCentre(const std::string& aLabel)
//-----------------------------------
{
    if (g_p_scene_graph.get())
        {
        g_p_scene_graph->moveToCentre(aLabel);
        }
    else
        {
        gVirtualXRay::LOGGER.logWarning("The scenegraph is empty.") << endl;
        }
}

//-----------------------------------
void moveToCenter(const std::string& aLabel)
//-----------------------------------
{
    moveToCentre(aLabel);
}

void scaleNode(const std::string& aLabel,
               double x,
               double y,
               double z)
{
    if (g_p_scene_graph.get())
        {
        // Find the mesh
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

        // It has been found
        if (p_node)
            {
            gVirtualXRay::MATRIX4 scaling_matrix =
            gVirtualXRay::MATRIX4::buildScaleMatrix(x, y, z);

            // Root node
            if (p_node->getLabel() == "root")
                {
                p_node->setLocalTransformation(
                scaling_matrix
                * p_node->getLocalTransformationMatrix());
                }
            else
                {
                p_node->setLocalTransformation(
                p_node->getLocalTransformationMatrix()
                * scaling_matrix);
                }
            }
        // It has not been found
        else
            {
            gVirtualXRay::LOGGER.logWarning("PolygonMesh ") << aLabel
            << " not found in g_p_polygon_mesh_set." << endl;
            }
        }
    else
        {
        gVirtualXRay::LOGGER.logWarning("The scenegraph is empty.") << endl;
        }
}


void rotateNode(const std::string& aLabel,
                double angle,
                double x,
                double y,
                double z)
{
    if (g_p_scene_graph.get())
        {
        // Find the mesh
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

        // It has been found
        if (p_node)
            {
            gVirtualXRay::MATRIX4 rotation_matrix =
            gVirtualXRay::MATRIX4::buildRotationMatrix(angle, x, y, z);

            // Root node
            if (p_node->getLabel() == "root")
                {
                p_node->setLocalTransformation(
                rotation_matrix
                * p_node->getLocalTransformationMatrix());
                }
            else
                {
                p_node->setLocalTransformation(
                    p_node->getLocalTransformationMatrix() * rotation_matrix);
                }
            }
        // It has not been found
        else
            {
            gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
                aLabel <<
                " not found in g_p_polygon_mesh_set." <<
                endl;
            }
        }
    else
        {
        gVirtualXRay::LOGGER.logWarning("The scenegraph is empty.") << endl;
        }
}


void rotateNode(const std::string& aLabel,
                double yaw,
                double pitch,
                double roll)
{
    if (g_p_scene_graph.get())
        {
        // Find the mesh
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

        // It has been found
        if (p_node)
            {
            gVirtualXRay::MATRIX4 rotation_matrix =
            gVirtualXRay::MATRIX4::buildYawPitchRollRotationMatrix(yaw, pitch, roll);

            // Root node
            if (p_node->getLabel() == "root")
                {
                p_node->setLocalTransformation(
                rotation_matrix
                * p_node->getLocalTransformationMatrix());
                }
            else
                {
                p_node->setLocalTransformation(
                    p_node->getLocalTransformationMatrix() * rotation_matrix);
                }
            }
        // It has not been found
        else
            {
            gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
                aLabel <<
                " not found in g_p_polygon_mesh_set." <<
                endl;
            }
        }
    else
        {
        gVirtualXRay::LOGGER.logWarning("The scenegraph is empty.") << endl;
        }
}


void translateNode(const std::string& aLabel,
                   double x,
                   double y,
                   double z,
                   const std::string& aUnitOfLength)
{
    if (g_p_scene_graph.get())
        {
        // Find the mesh
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

        // It has been found
        if (p_node)
            {
            double unit_of_length = getUnitOfLength(aUnitOfLength);
            gVirtualXRay::MATRIX4 translation_matrix =
            gVirtualXRay::MATRIX4::buildTranslationMatrix(
            x * unit_of_length, y * unit_of_length,
            z * unit_of_length);

            // Root node
            if (p_node->getLabel() == "root")
                {
                p_node->setLocalTransformation(
                translation_matrix
                * p_node->getLocalTransformationMatrix());
                }
            else
                {
                p_node->setLocalTransformation(
                p_node->getLocalTransformationMatrix()
                * translation_matrix);
                }
            }
        // It has not been found
        else
            {
            gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
            }
        }
    else
        {
        gVirtualXRay::LOGGER.logWarning("The scenegraph is empty.") << endl;
        }
}


void shearNode(const std::string& aLabel,
               double yx,
               double zx,
               double xy,
               double zy,
               double xz,
               double yz)
{
    if (g_p_scene_graph.get())
        {
        // Find the mesh
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

        // It has been found
        if (p_node)
            {
            gVirtualXRay::MATRIX4 shear_matrix(
                     1, yx, zx, 0,
					xy,  1, zy, 0,
					xz, yz,  1, 0,
					 0,  0,  0, 1);

            // Root node
            if (p_node->getLabel() == "root")
                {
                p_node->setLocalTransformation(
                        shear_matrix *
                        p_node->getLocalTransformationMatrix());
                }
            else
                {
                p_node->setLocalTransformation(
                        p_node->getLocalTransformationMatrix() *
                        shear_matrix);
                }
            }
        // It has not been found
        else
            {
            gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
            }
        }
    else
        {
        gVirtualXRay::LOGGER.logWarning("The scenegraph is empty.") << endl;
        }
}


void resetSceneTransformation()
{
    g_sample_rotation_matrix = gVirtualXRay::MATRIX4();
}


void scaleScene(double x,
                double y,
                double z)
{
    g_sample_rotation_matrix *= gVirtualXRay::MATRIX4::buildScaleMatrix(
        x, y, z);
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
}


void rotateScene(double angle, double x, double y, double z)
{
    // Move the scenegraph
    g_sample_rotation_matrix *= gVirtualXRay::MATRIX4::buildRotationMatrix(
        angle, x, y, z);
}


void rotateScene(double yaw, double pitch, double roll)
{
    // Move the scenegraph
    g_sample_rotation_matrix *= gVirtualXRay::MATRIX4::buildYawPitchRollRotationMatrix(
        yaw, pitch, roll);
}


void translateScene(double x,
                    double y,
                    double z,
                    const std::string& aUnitOfLength)
{
    // Move the scenegraph
    double unit_of_length = getUnitOfLength(aUnitOfLength);
    g_sample_rotation_matrix *= gVirtualXRay::MATRIX4::buildTranslationMatrix(
        x * unit_of_length,
        y * unit_of_length,
        z * unit_of_length);
}


std::vector<std::vector<float> > getSceneTransformationMatrix()
{
    return gvxrMatrix2Vector(g_sample_rotation_matrix);
}


std::vector<std::vector<float> > getRootTransformationMatrix()
{
    if (!g_p_scene_graph.get())
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

    if (g_p_scene_graph.get())
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
                endl;
            }
        }
    else
        {
        gVirtualXRay::LOGGER.logWarning("The scenegraph is empty.") << endl;
        }

    return (gvxrMatrix2Vector(transformation));
}


std::vector<std::vector<float> > getNodeWorldTransformationMatrix(
        const std::string& aLabel)
{
    gVirtualXRay::MATRIX4 transformation;

    if (g_p_scene_graph.get())
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
                endl;
            }
        }
    else
        {
        gVirtualXRay::LOGGER.logWarning("The scenegraph is empty.") << endl;
        }

    return (gvxrMatrix2Vector(transformation));
}


void setSceneTransformationMatrix(
        const std::vector<std::vector<float> >& aMatrix)
{
    g_sample_rotation_matrix = Vector2gvxrMatrix(aMatrix);
}

void setRootTransformationMatrix(
        const std::vector<std::vector<float> >& aMatrix)
{
    if (g_p_scene_graph.get())
        {
        g_p_scene_graph->setRootNodeTransformationMatrix(
        Vector2gvxrMatrix(aMatrix));
        }
    else
        {
        gVirtualXRay::LOGGER.logWarning("The scenegraph is empty.") << endl;
        }
}

void setNodeTransformationMatrix(const std::string& aLabel,
        const std::vector<std::vector<float> >& aMatrix)
{
    if (g_p_scene_graph.get())
        {
        // Find the mesh
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

        // It has been found
        if (p_node)
            {
            p_node->setLocalTransformation(Vector2gvxrMatrix(aMatrix));
            }
        // It has not been found
        else
            {
            gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
                aLabel <<
                " not found in g_p_polygon_mesh_set." <<
                endl;
            }
        }
    else
        {
        gVirtualXRay::LOGGER.logWarning("The scenegraph is empty.") << endl;
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
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
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
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
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
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
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
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
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
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
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
      }
  // It has not been found
  else
      {
      gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
          aLabel <<
          " not found in g_p_polygon_mesh_set." <<
          endl;
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
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
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
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
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
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }
}

//-----------------------------------
void setMu(const std::string& aLabel,
           double aCoefficient,
           const std::string& aUnit)
//-----------------------------------
{
    setLinearAttenuationCoefficient(aLabel, aCoefficient, aUnit);
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

    double density_unit;
    double volume_unit;

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
    }
    // It has not been found
    else
    {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
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
            endl;
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
    if (!g_p_scene_graph.get())
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
            endl;
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
    if (!g_p_scene_graph.get())
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
            endl;
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
            endl;
        }

    return std::string();
}









bool g_fisrt_pass = true;


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
            gVirtualXRay::LOGGER.logWarning("No active window to use.") << endl;
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
            if (!g_has_own_gl_context)
                {
                glfwPollEvents();
                }
            }
        else
            {
            gVirtualXRay::LOGGER.logWarning("Window") <<
                aWindowID <<
                " not found in g_p_window_set." <<
                endl;
            }
        }

    if (g_fisrt_pass)
    {
        g_fisrt_pass = false;
        displayScene(aSceneRotationFlag, aWindowID);
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
            gVirtualXRay::LOGGER.logWarning("No active window to destroy.") << endl;
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
                endl;
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
            gVirtualXRay::LOGGER.logWarning("No active window to show.") << endl;
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
                " not found." << endl;
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
            gVirtualXRay::LOGGER.logWarning("No active window to hide.") << endl;
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
                " not found." << endl;
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
            gVirtualXRay::LOGGER.logWarning("No active window to hide.") << endl;
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
                " not found." << endl;
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
            gVirtualXRay::LOGGER.logWarning("No active window to hide.") << endl;
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
                " not found." << endl;
            }
        }
}

//---------------------------------------------------------------------
void setWindowBackGroundColor(float R, float G, float B, int aWindowID)
//---------------------------------------------------------------------
{
    setWindowBackGroundColour(R, G, B, aWindowID);
}


//----------------------------------------------------------------------------------
const std::vector<std::vector<std::vector< float> > >& takeScreenshot(int aWindowID)
//----------------------------------------------------------------------------------
{
    // Retrieve the window
    if (aWindowID == -1)
        {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
            {
            gVirtualXRay::LOGGER.logWarning("No active window to hide.") << endl;
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
                search->second->takeScreenshot();
                }
            }
        else
            {
            gVirtualXRay::LOGGER.logWarning("Window ") << aWindowID <<
                " not found." << endl;
            }
        }

    // Return the image
    return lastest_screenshot;
}


//--------------------------------------------------------------------------
const std::vector<std::vector<std::vector< float> > >& getLatestScreenshot()
//--------------------------------------------------------------------------
{
    return lastest_screenshot;
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


//----------------
void useConeBeam()
//----------------
{
    usePointSource();
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


//------------------------
bool isUsingParallelBeam()
//------------------------
{
    gVirtualXRay::XRayDetector::SourceShape source_shape = g_xray_detector.getSourceShape();

    return source_shape == gVirtualXRay::XRayDetector::PARALLEL;
}


//--------------------------
bool isUsingParallelSource()
//--------------------------
{
    return isUsingParallelBeam();
}


//-----------------------
bool isUsingPointSource()
//-----------------------
{
    gVirtualXRay::XRayDetector::SourceShape source_shape = g_xray_detector.getSourceShape();

    return source_shape == gVirtualXRay::XRayDetector::POINT;
}


//--------------------
bool isUsingConeBeam()
//--------------------
{
    gVirtualXRay::XRayDetector::SourceShape source_shape = g_xray_detector.getSourceShape();

    return source_shape == gVirtualXRay::XRayDetector::POINT ||
        source_shape == gVirtualXRay::XRayDetector::SQUARE || 
        source_shape == gVirtualXRay::XRayDetector::RECTANGLE || 
        source_shape == gVirtualXRay::XRayDetector::CUBE ||
        source_shape == gVirtualXRay::XRayDetector::LINE;
}


//---------------------
bool isUsingFocalSpot()
//---------------------
{
    gVirtualXRay::XRayDetector::SourceShape source_shape = g_xray_detector.getSourceShape();

    return source_shape == gVirtualXRay::XRayDetector::SQUARE || 
        source_shape == gVirtualXRay::XRayDetector::RECTANGLE || 
        source_shape == gVirtualXRay::XRayDetector::CUBE ||
        source_shape == gVirtualXRay::XRayDetector::LINE;
}


//--------------------------------------------------------------------------------------------
void setFocalSpot(float x, float y, float z, float l, const std::string& aUnitOfLength, int i)
//--------------------------------------------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    if (i % 2 == 0)
    {
        gVirtualXRay::LOGGER.logWarning("i (") << i << ") must be an odd number, " <<
            i+1 << " will be used instead." << endl;
    }

    g_xray_detector.setCubicSource(gVirtualXRay::VEC3(x * unit_of_length, y * unit_of_length, z * unit_of_length),
                      i,
                      l * unit_of_length);

    g_beam_shape_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}


//---------------------------------------------------------
void setFocalSpotAsSquare(float x, float y, float z,
                          float l,
                          const std::string& aUnitOfLength,
                          int i)
//---------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    if (i % 2 == 0)
    {
        gVirtualXRay::LOGGER.logWarning("i (") << i << ") must be an odd number, " <<
            i+1 << " will be used instead." << endl;
    }

    g_xray_detector.setSquareSource(gVirtualXRay::VEC3(x * unit_of_length, y * unit_of_length, z * unit_of_length),
                      i,
                      l * unit_of_length);

    g_beam_shape_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}


//------------------------------------------------------------
void setFocalSpotAsRectangle(float x, float y, float z,
                             float l, float k,
                             const std::string& aUnitOfLength,
                             int i, int j)
//------------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    if (i % 2 == 0)
    {
        gVirtualXRay::LOGGER.logWarning("i (") << i << ") must be an odd number, " <<
            i+1 << " will be used instead." << endl;
    }

    if (j % 2 == 0)
    {
        gVirtualXRay::LOGGER.logWarning("j (") << j << ") must be an odd number, " <<
            j+1 << " will be used instead." << endl;
    }

    g_xray_detector.setRectangleSource(gVirtualXRay::VEC3(x * unit_of_length, y * unit_of_length, z * unit_of_length),
                      i, j,
                      l * unit_of_length,
                      k * unit_of_length);

    g_beam_shape_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}


//------------------------------------------------------------------------------
void rotateFocalSpot(const double& aRotationAngle, double u, double v, double w)
//------------------------------------------------------------------------------
{
    g_xray_detector.rotateFocalSpot(aRotationAngle, gVirtualXRay::VEC3(u, v, w));
}


//---------------------------------------------
void rotateSource(const double& aRotationAngle,
                  double x, double y, double z,
                  double u, double v, double w)
//---------------------------------------------
{
    g_xray_detector.rotateSource(aRotationAngle,
        gVirtualXRay::VEC3(x, y, z),
        gVirtualXRay::VEC3(u, v, w));
}


//--------------------------------------------------
void resetBeamSpectrum(bool aClearTubeParameterFlag)
//--------------------------------------------------
{
    if (aClearTubeParameterFlag) g_xray_beam.clear();
    else g_xray_beam.clearSpectrumOnly();

    g_beam_energy_initialised = false;
}

//-----------------------------------------------------
void setMonoChromatic(double anEnergy,
                      const std::string& aUnitOfEnergy,
                      double aNumberOfPhotons)
//-----------------------------------------------------
{
    g_xray_beam.clear();
    addEnergyBinToSpectrum(anEnergy, aUnitOfEnergy, aNumberOfPhotons);
}

//----------------------------------------------------
void addEnergyBinToSpectrum(double anEnergy, const std::string& aUnitOfEnergy,
    double aNumberOfPhotons)
//----------------------------------------------------
{
    if (g_xray_beam.addChannel(aNumberOfPhotons,
        anEnergy * getUnitOfEnergy(aUnitOfEnergy)))
    {
        g_beam_energy_initialised = true;

        // Initialise the X-ray renderer if needed and if possible
        initialiseXRayRenderer();
    }
}

//--------------------------------------------------------
void loadSpectrumFromTSV(const std::string& aFileName,
    const std::string& aUnitOfEnergy, bool aNormalisationFlag)
//--------------------------------------------------------
{
    g_xray_beam.clear();

    g_xray_beam.initialise(aFileName.c_str(), getUnitOfEnergy(aUnitOfEnergy));

    if (aNormalisationFlag) {
    g_xray_beam.normalise();
    }

    g_beam_energy_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}

//-----------------------------------------------------------------
std::vector<double> getEnergyBins(const std::string& aUnitOfEnergy)
//-----------------------------------------------------------------
{
    std::vector<double> p_temp;

    for (unsigned int i = 0; i < g_xray_beam.getEnergyChannelNumber(); ++i) {
    p_temp.push_back(
    g_xray_beam.getEnergyChannel(i).getPhotonEnergy()
    / getUnitOfEnergy(aUnitOfEnergy));
    }

    return (p_temp);
}


//-----------------------
void enablePoissonNoise()
//-----------------------
{
    g_xray_beam.enablePoissonNoise();
}


//------------------------
void disablePoissonNoise()
//------------------------
{
    g_xray_beam.disablePoissonNoise();
}


//----------------------------------------------------
void setNumberOfPhotons(unsigned int aNumberOfPhotons)
//----------------------------------------------------
{
    g_xray_beam.setNumberOfPhotons(aNumberOfPhotons);
}


//------------------------------------------------------------
void setNumberOfPhotonsPerPixel(unsigned int aNumberOfPhotons)
//------------------------------------------------------------
{
    setNumberOfPhotons(aNumberOfPhotons);
}




//----------------------------------------------------------
void setNumberOfPhotonsPerCM2(double aNumberOfPhotons)
//----------------------------------------------------------
{
    g_xray_beam.setNumberOfPhotons(round(aNumberOfPhotons * getDetectorPixelArea("cm")));
}


//-------------------------------
unsigned int getNumberOfPhotons()
//-------------------------------
{
    return g_xray_beam.getNumberOfPhotons();
}


//---------------------------------------
unsigned int getNumberOfPhotonsPerPixel()
//---------------------------------------
{
    return getNumberOfPhotons();
}


//----------------------------------
unsigned int getNumberOfPhotonsPerCM2()
//-------------------------------------
{

    return g_xray_beam.getNumberOfPhotons() / getDetectorPixelArea("cm");
}


//------------------------------
void usePoissonNoise(bool aFlag)
//------------------------------
{
    g_xray_beam.usePoissonNoise(aFlag);
}


//--------------------
bool usePoissonNoise()
//--------------------
{
    return (g_xray_beam.usePoissonNoise());
}


//-----------------------------------------------
std::vector<std::vector< float> > getWhiteImage(bool anIntegrateEnergyFlag)
//-----------------------------------------------
{
    return (getVectorImage2D(g_xray_renderer.getWhiteImage(anIntegrateEnergyFlag)));
}


//---------------------------------------------------------------------
std::vector<std::vector<std::vector< float> > > getWhiteSpectralImage()
//---------------------------------------------------------------------
{
    std::vector<std::vector<std::vector< float> > > p_white_image_set;

    for (int i = 0; i < g_xray_beam.getNumberOfPhotonCountingBands(); ++i)
    {
        g_xray_beam.activatePhotonCountingBand(i);
        p_white_image_set.push_back(getVectorImage2D(g_xray_renderer.getWhiteImage(false)));
    }

    return p_white_image_set;
}


//--------------------------------------------
void saveSTLfile(const std::string& aLabel,
                 const std::string& aFileName)
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
    	p_node->getPolygonMesh().writeSTLFile();
        }
    // The node does not exist
    else
        {
		gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
			aLabel <<
			" not found in g_p_polygon_mesh_set." <<
			endl;
        }
}

//--------------------------------------------
std::vector<double> getPhotonCountEnergyBins()
//--------------------------------------------
{
    std::vector<double> p_temp;

    for (unsigned int i = 0; i < g_xray_beam.getEnergyChannelNumber(); ++i) {
    p_temp.push_back(g_xray_beam.getEnergyChannel(i).getPhotonNumber());
    }

    return (p_temp);
}


//-----------------------------------------
double getTotalEnergyWithDetectorResponse()
//-----------------------------------------
{
    return g_xray_renderer.getTotalEnergyWithDetectorResponse();
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


//--------------------------------------------------------------------------------
void setVoltage(const float& aVoltage, const std::string& aUnitOfElectriPotential)
//--------------------------------------------------------------------------------
{
    g_xray_beam.setVoltage(aVoltage * getUnitOfElectriPotential(aUnitOfElectriPotential));
}


//----------------------------------------------------------
float getVoltage(const std::string& aUnitOfElectriPotential)
//----------------------------------------------------------
{
    return g_xray_beam.getVoltage() / getUnitOfElectriPotential(aUnitOfElectriPotential);
}


//------------------------------
void setmAs(const float& an_mAs)
//------------------------------
{
    g_xray_beam.setmAs(an_mAs);
}


//------------
float getmAs()
//------------
{
    return g_xray_beam.getmAs();
}


//-------------------------------------
void setTubeAngle(const float& anAngle)
//-------------------------------------
{
    g_xray_beam.setTubeAngle(anAngle);
}


//------------------
float getTubeAngle()
//------------------
{
    return g_xray_beam.getTubeAngle();
}


//--------------------
void clearFiltration()
//--------------------
{
    g_xray_beam.clearFiltration();
}


//-----------------------------------------------------------------------------------
void setFiltration(const std::vector<std::pair<unsigned int, float> >& aSetOfFilters,
                    const std::string& aUnitOfLength)
//-----------------------------------------------------------------------------------
{
    std::vector<std::pair<unsigned int, gVirtualXRay::RATIONAL_NUMBER> > p_filtration;

    for (std::vector<std::pair<unsigned int, float> >::const_iterator ite(g_xray_beam.getFiltration().begin());
        ite != g_xray_beam.getFiltration().end();
        ++ite)
    {
        p_filtration.push_back(std::pair<unsigned int, float>(ite->first, ite->second * getUnitOfLength(aUnitOfLength)));
    }

    g_xray_beam.setFiltration(p_filtration);
}


//----------------------------------------------
void addFilter(const std::string& aMaterial,
               const double& aThickness,
               const std::string& aUnitOfLength)
//----------------------------------------------
{
    g_xray_beam.addFilter(aMaterial, aThickness * getUnitOfLength(aUnitOfLength));
}


//----------------------------------------------
void addFilter(unsigned int aMaterial,
               const double& aThickness,
               const std::string& aUnitOfLength)
//----------------------------------------------
{
    g_xray_beam.addFilter(aMaterial, aThickness * getUnitOfLength(aUnitOfLength));

}


//-----------------------------------------------
std::vector<unsigned int> getFiltrationMaterial()
//-----------------------------------------------
{
    std::vector<unsigned int> p_filtration;

    for (std::vector<std::pair<unsigned int, gVirtualXRay::RATIONAL_NUMBER> >::const_iterator ite(g_xray_beam.getFiltration().begin());
        ite != g_xray_beam.getFiltration().end();
        ++ite)
    {
        p_filtration.push_back(ite->first);
    }

    return p_filtration;
}


//-------------------------------------------------------------------------
std::vector<float> getFiltrationThickness(const std::string& aUnitOfLength)
//-------------------------------------------------------------------------
{
    std::vector<float> p_filtration;

    for (std::vector<std::pair<unsigned int, gVirtualXRay::RATIONAL_NUMBER> >::const_iterator ite(g_xray_beam.getFiltration().begin());
        ite != g_xray_beam.getFiltration().end();
        ++ite)
    {
        p_filtration.push_back(ite->second / getUnitOfLength(aUnitOfLength));
    }

    return p_filtration;
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
        p_filtration.push_back(std::pair<unsigned int, float>(ite->first, ite->second / getUnitOfLength(aUnitOfLength)));
    }

    return p_filtration;
}


bool hasSpekpy()
{
    return g_xray_beam.hasSpekpy();
}


bool hasXpecgen()
{
    return g_xray_beam.hasXpecgen();
}


void useSpekpy()
{
    g_xray_beam.useSpekpy();
}


void useXpecgen()
{
    g_xray_beam.useXpecgen();
}


bool usingSpekpy()
{
    return g_xray_beam.usingSpekpy();
}


bool usingXpecgen()
{
    return g_xray_beam.usingXpecgen();
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



void startArcBallRotation(double x, double y) {
    g_last_x_position = x;
    g_last_y_position = y;

    g_current_x_position = x;
    g_current_y_position = y;

    g_use_arc_ball = true;
}

void stopArcBallRotation() {
    g_use_arc_ball = false;
}

bool usingArcBallRotation() {
    return (g_use_arc_ball);
}

void cursorPositionCallback(double x, double y, int aViewportWidth,
    int aViewportHeight) {
    g_current_x_position = x;
    g_current_y_position = y;

    computeRotation(g_scene_rotation_matrix, aViewportWidth, aViewportHeight);
}


//-------------------------------------------------
void scrollCallback(double xoffset, double yoffset)
//-------------------------------------------------
{
    // Scrolling along the Y-axis
    if (fabs(yoffset) > EPSILON)
    {
		g_use_arc_ball = false;
		g_zoom += yoffset * gVirtualXRay::cm;

		gVirtualXRay::loadLookAtModelViewMatrix(0.0, 0.0, g_zoom,
		                                        0.0, 0.0, 0.0,
		                                        0.0, 1.0, 0.0);
    }
}


//----------------------------
void setZoom(float aZoomValue)
//----------------------------
{
    if (g_fisrt_pass)
    {
        displayScene(false);
    }

    g_zoom = aZoomValue;

    gVirtualXRay::loadLookAtModelViewMatrix(0.0, 0.0, g_zoom,
                                            0.0, 0.0, 0.0,
                                            0.0, 1.0, 0.0);
}


//-------------
float getZoom()
//-------------
{
    return g_zoom;
}


void setSceneRotationMatrix(const std::vector<double>& aRotationMatrix)
{
    g_scene_rotation_matrix = gVirtualXRay::Matrix4x4<GLfloat>(aRotationMatrix);
}

std::vector<double> getSceneRotationMatrix()
{
    return g_scene_rotation_matrix.getAsVector();
}

//--------------------------------------------
void mouseButtonCallback(GLFWwindow* apWindow, int aButton, int aButtonState,
    int aModifierKey)
//--------------------------------------------
{
    g_button = aButton;
    g_button_state = aButtonState;

    // Use the arc ball
    if (g_button_state == GLFW_PRESS) {
    double xpos(0);
    double ypos(0);
    glfwGetCursorPos(apWindow, &xpos, &ypos);

    startArcBallRotation(xpos, ypos);
    }
    // Stop using the arc ball
    else {
    stopArcBallRotation();
    }
}

//--------------------------------------------------------------
void cursorPosCallback(GLFWwindow* apWindow, double x, double y)
//--------------------------------------------------------------
{
    int viewport_width  = 0;
    int viewport_height = 0;

    glfwGetWindowSize(apWindow, &viewport_width, &viewport_height);

    // Compute the rotation
    cursorPositionCallback(x, y, viewport_width, viewport_height);
}

//-----------------------------------------------------------------------
void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset)
//-----------------------------------------------------------------------
{
    scrollCallback(xoffset, yoffset);
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


//------------------------------
void orthographic(double left,
                  double right,
                  double bottom,
                  double top,
                  double nearZ,
                  double farZ)
//------------------------------
{
    gVirtualXRay::loadOrthoProjectionMatrix(
        left, right,
        bottom, top,
        nearZ, farZ);
}

//------------------------------------------
void perspective(double initial_fovy,
                 double screen_aspect_ratio,
                 double initial_near,
                 double initial_far)
//------------------------------------------
{
    gVirtualXRay::loadPerspectiveProjectionMatrix(initial_fovy,
    screen_aspect_ratio, initial_near, initial_far);
}

//-------------------------
void lookAt(double eye_x,
            double eye_y,
            double eye_z,
            double lookAtX,
            double lookAtY,
            double lookAtZ,
            double upX,
            double upY,
            double upZ)
//-------------------------
{
	gVirtualXRay::loadLookAtModelViewMatrix(eye_x, eye_y, eye_z,
			lookAtX, lookAtY, lookAtZ,
			upX, upY, upZ);
}


//----------------------------------------------------------
void computeRotation(gVirtualXRay::MATRIX4& aRotationMatrix,
                     int aViewportWidth,
                     int aViewportHeight)
//----------------------------------------------------------
{
    if (g_use_arc_ball)
    {
        if (g_current_x_position != g_last_x_position ||
            g_current_y_position != g_last_y_position)
        {
            gVirtualXRay::Vec3<GLfloat> va(
                getArcballVector(g_last_x_position,
                                 g_last_y_position,
                                 aViewportWidth,
                                 aViewportHeight));

            gVirtualXRay::Vec3<GLfloat> vb(
                getArcballVector(g_current_x_position,
                                 g_current_y_position,
                                 aViewportWidth,
                                 aViewportHeight));

            float angle(ROTATION_SPEED *
                radian2degree(acos(std::min(1.0, va.dotProduct(vb)))));

            gVirtualXRay::Vec3 < GLfloat > axis_in_camera_coord(va ^ vb);
            //axis_in_camera_coord.normalize();

            gVirtualXRay::Matrix4x4<GLfloat> camera2object(
                aRotationMatrix.getInverse());

            gVirtualXRay::Vec3<GLfloat> axis_in_object_coord =
            camera2object * axis_in_camera_coord;
            //axis_in_object_coord.normalize();

            gVirtualXRay::Matrix4x4 < GLfloat > rotation_matrix;
            rotation_matrix.rotate(angle, axis_in_object_coord);
            aRotationMatrix = aRotationMatrix * rotation_matrix;

            g_last_x_position = g_current_x_position;
            g_last_y_position = g_current_y_position;
        }
    }
}

//---------------------------------------------------------------
gVirtualXRay::Vec3<GLfloat> getArcballVector(int x,
                                             int y,
                                             int aViewportWidth,
                                             int aViewportHeight)
//---------------------------------------------------------------
{
    gVirtualXRay::Vec3 < GLfloat
    > P(2.0 * float(x) / float(aViewportWidth) - 1.0,
    2.0 * float(y) / float(aViewportHeight) - 1.0, 0);

    P.setY(-P.getY());

    float OP_squared = P.getX() * P.getX() + P.getY() * P.getY();
    if (OP_squared <= 1.0) {
    P.setZ(sqrt(1.0 - OP_squared));  // Pythagore
    } else {
    P.normalise();  // nearest point
    }

    return (P);
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



gVirtualXRay::MATRIX4 Vector2gvxrMatrix(
    const std::vector<std::vector<float> >& aSTLMatrix) {
    if (aSTLMatrix.size() == 4) {
    if (aSTLMatrix[0].size() == 4 && aSTLMatrix[1].size() == 4
    && aSTLMatrix[2].size() == 4 && aSTLMatrix[3].size() == 4) {
    return gVirtualXRay::MATRIX4(aSTLMatrix[0][0], aSTLMatrix[0][1],
    aSTLMatrix[0][2], aSTLMatrix[0][3], aSTLMatrix[1][0],
    aSTLMatrix[1][1], aSTLMatrix[1][2], aSTLMatrix[1][3],
    aSTLMatrix[2][0], aSTLMatrix[2][1], aSTLMatrix[2][2],
    aSTLMatrix[2][3], aSTLMatrix[3][0], aSTLMatrix[3][1],
    aSTLMatrix[3][2], aSTLMatrix[3][3]);
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

    // The scenegraph does not exist
    if (g_p_scene_graph.get())
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
            endl;
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
    return getVectorImage2D(gVirtualXRay::Image<float>(aFileName));
}


std::vector<std::vector<std::vector<float> > > loadImage3D(const std::string& aFileName)
{
    return getVectorImage3D(gVirtualXRay::Image<float>(aFileName));
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
    for (int i = 0; i < anImage.size(); ++i)
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
    for (int i = 0; i < anImage.size(); ++i)
    {
        stddev += std::pow(anImage[i] - avg, 2);
    }

    return sqrt(stddev / anImage.size());
}


double getStddev(const std::vector<float>& anImage, double avg)
{
    double stddev = 0.0;

#pragma omp parallel for reduction( + : stddev )
    for (int i = 0; i < anImage.size(); ++i)
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
    for (int i = 0; i < aReferenceImage.size(); ++i)
    {
        zncc += (aReferenceImage[i] - avg_ref) * (aTestImage[i] - avg_test);
    }

    return (zncc / (stddev_ref * stddev_test)) / aReferenceImage.size();
}


double computeRMSE(const std::vector<float>& aReferenceImage, const std::vector<float>& aTestImage, bool normalise)
{
    double sse = 0.0;

    if (normalise)
    {
        double avg_ref = getMean(aReferenceImage);
        double avg_test = getMean(aTestImage);

        double stddev_ref = getStddev(aReferenceImage, avg_ref);
        double stddev_test = getStddev(aTestImage, avg_test);

#pragma omp parallel for reduction( + : sse )
        for (int i = 0; i < aReferenceImage.size(); ++i)
        {
            sse += std::pow(((aReferenceImage[i] - avg_ref) / stddev_ref) -
                ((aTestImage[i] - avg_test) / stddev_test), 2);
        }
    }
    else
    {
#pragma omp parallel for reduction( + : sse )
        for (int i = 0; i < aReferenceImage.size(); ++i)
        {
            sse += std::pow(aReferenceImage[i] - aTestImage[i], 2);
        }
    }

    return sqrt(sse / aReferenceImage.size());
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
    for (int i = 0; i < g_p_test_image.size(); ++i)
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
