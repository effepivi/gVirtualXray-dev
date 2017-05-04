/*

Copyright (c) 2017, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


#ifndef __AnatomicalDataHandler_h
#define __AnatomicalDataHandler_h


/**
********************************************************************************
*
*   @file       AnatomicalDataHandler.h
*
*   @brief      Class to manage the anatomical data corresponding a patient.
*
*   @version    1.0
*
*   @date       07/04/2017
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
*               http://www.fpvidal.net/, Apr 2017, 2017, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//      Include
//******************************************************************************
#include <string>
#include <fstream>
#include <sstream>
#include <map>

#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <vector>

#ifndef __Vec3_h
#include "gVirtualXRay/Vec3.h"
#endif

#ifndef __Matrix4x4_h
#include "gVirtualXRay/Matrix4x4.h"
#endif

#ifndef __Types_h
#include "gVirtualXRay/Types.h"
#endif

#ifndef __AtomicElement_h
#include "gVirtualXRay/AtomicElement.h"
#endif

#ifndef __XRayBeam_h
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef __XRayDetector_h
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef __XRayRenderer_h
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef __Shader_h
#include "gVirtualXRay/Shader.h"
#endif

#ifndef __TextRenderer_h
#include "gVirtualXRay/TextRenderer.h"
#endif

#ifndef __PolygonMesh_h
#include "gVirtualXRay/PolygonMesh.h"
#endif

#ifndef __Image_h
#include "gVirtualXRay/Image.h"
#endif


//==============================================================================
/**
*   @class  ICPRPhamtom
*   @brief  ICPRPhamtom is a class to manage the ICPR Adult Reference 
*           Computational Phantoms
*           (http://www.icrp.org/publication.asp?id=ICRP%20Publication%20110).
*/
//==============================================================================
class ICPRPhamtom
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    ~ICPRPhamtom();


    //--------------------------------------------------------------------------
    /// Load the data
    /*
    *   @param aName:   the phantom name
    *   @param aPath:   the path to the phantom directory
    */
    //--------------------------------------------------------------------------
    void load(const std::string& aPath, const std::string& aName);


    //--------------------------------------------------------------------------
    /// Save the volume data
    /*
    *   @param aFileName:   the name of the output file
    */
    //--------------------------------------------------------------------------
    void saveVolume(const std::string& aFileName) const;


    void displaySolid();
    void displayWireframe();

    gVirtualXRay::VEC3 getCentre() const;
    gVirtualXRay::VEC3 getCenter() const;


    unsigned int getNumberOfPolygonMeshes() const;


    const std::map<short, gVirtualXRay::PolygonMesh>& getPolygonMeshSet() const;


//******************************************************************************
private:
    void loadTissueMaterial(const std::string& aFileName);

    void loadMaterialDensity(const std::string& aFileName, bool aCreateMeshesFlag);

    void addToPolygon(const gVirtualXRay::Image<unsigned char>& aBinaryImage,
            gVirtualXRay::PolygonMesh& apPolygon);

    gVirtualXRay::Image<short> m_volume_dataset;
    std::map<short, gVirtualXRay::TissueMaterial> m_p_material_table;
    std::map<short, gVirtualXRay::PolygonMesh> m_p_polygon_set;
    std::map<short, gVirtualXRay::Image<unsigned char> > m_p_binary_mask_set;
};

//==============================================================================
/**
*   @class  AnatomicalDataHandler
*   @brief  AnatomicalDataHandler is a class to manage anatomical data.
*/
//==============================================================================
class AnatomicalDataHandler
//------------------------------------------------------------------------------
{
//******************************************************************************
private:
    /// New type defining the type of CT acquisition
    typedef enum
    {
        /// The sample is rotating, the couple X-ray source/detector is static
        ROTATE_SAMPLE,

        /// The sample is static, the couple X-ray source/detector is rotating
        ROTATE_SOURCE_DETECTOR
    } AcquisitionType;


//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    /*
    *   @param apPparent:   the parent widget
    */
    //--------------------------------------------------------------------------
    AnatomicalDataHandler(unsigned int aWidth = 480,
            unsigned int aHeight = 480);


    //--------------------------------------------------------------------------
    /// Destructor
    //--------------------------------------------------------------------------
    virtual ~AnatomicalDataHandler();


    unsigned int getWidth() const;
    unsigned int getHeight() const;
    
    
    //--------------------------------------------------------------------------
    /// Initialise OpenGL
    //--------------------------------------------------------------------------
    virtual void initializeGL();


    //--------------------------------------------------------------------------
    /// initialize the set of anatomical mesh.
    //--------------------------------------------------------------------------
    void initializePatient();
    

    //--------------------------------------------------------------------------
    /// Render the 3D scene using OpenGL
    //--------------------------------------------------------------------------
    virtual void draw();


    //--------------------------------------------------------------------------
    /// Function used to resize the viewport and reset the projection matrix
    /*
    *   @param aWidth: The width of the OpenGL window
    *   @param aHeight: The height of the OpenGL window
    */
    //--------------------------------------------------------------------------
    void resizeGL(int aWidth, int aHeight);
    

    //--------------------------------------------------------------------------
    /// Update the rotation matrix depending on the mouse movement
    /*
    *   @param aRotationMatrix: the rotation matrix (input/output)
    *   @param aXmousePosition: the new mouse position along the X-axis
    *   @param aYmousePosition: the new mouse position along the Y-axis
    */
    //--------------------------------------------------------------------------
    virtual void computeRotation(int aXmousePosition, int aYmousePosition);


    //--------------------------------------------------------------------------
    /// Set flags related to the arc ball
    /*
    *   @param aXmousePosition: the new mouse position along the X-axis
    *   @param aYmousePosition: the new mouse position along the Y-axis
    */
    //--------------------------------------------------------------------------
    virtual void enableArcBall(int aXmousePosition, int aYmousePosition);


    //--------------------------------------------------------------------------
    /// Set flags related to the arc ball
    /*
    *   @param aXmousePosition: the new mouse position along the X-axis
    *   @param aYmousePosition: the new mouse position along the Y-axis
    */
    //--------------------------------------------------------------------------
    virtual void disableArcBall();


    virtual void zoomIn();


    virtual void zoomOut();


//*****************************************************************************
private:
    //--------------------------------------------------------------------------
    /// Initialise OpenGL
    //--------------------------------------------------------------------------
    virtual void initGL();


    //--------------------------------------------------------------------------
    /// Initialise Freetype
    //--------------------------------------------------------------------------
    virtual void initFreeType();


    //--------------------------------------------------------------------------
    /// Set the properties of the X-ray detector
    //--------------------------------------------------------------------------
    virtual void loadDetector();


    //--------------------------------------------------------------------------
    /// Set the properties of the X-ray source
    //--------------------------------------------------------------------------
    virtual void loadSource();


    //--------------------------------------------------------------------------
    /// Set the properties of the X-ray simulation
    //--------------------------------------------------------------------------
    virtual void loadXRaySimulator();


    //--------------------------------------------------------------------------
    /// Load STL files (patient geometrical data)
    //--------------------------------------------------------------------------
    virtual void loadSTLFiles();


    //--------------------------------------------------------------------------
    /// Activate the properties of a patient (geometry and Hounsfield values)
    //--------------------------------------------------------------------------
    virtual void loadPatient();


    //--------------------------------------------------------------------------
    /// Render the 3D scene using OpenGL
    //--------------------------------------------------------------------------
    virtual void render();


    //--------------------------------------------------------------------------
    /// Reset the rotation angle and the acquisition type to their default values
    //--------------------------------------------------------------------------
    virtual void reset();


    //--------------------------------------------------------------------------
    /// Compute an up-to-date X-ray image
    //--------------------------------------------------------------------------
    virtual void updateXRayImage();


    //--------------------------------------------------------------------------
    /// Display a patient using OpenGL
    /*
    *   @param aShaderID:   The GLSL shader used to display apMeshSet
    */
    //--------------------------------------------------------------------------
    virtual void displayPatient(GLint aShaderID);


    //--------------------------------------------------------------------------
    /// Display a help message
    //--------------------------------------------------------------------------
    virtual void displayHelp();


    //--------------------------------------------------------------------------
    /// Decompress STL data from the RAM using the ZLib
    /*
    *   @param aMeshID: the ID of the polygon mesh to load
    *   @param apData:  the uncompressed polygon mesh data
    *   @param aSize:   the size of the uncompressed polygon mesh data
    *                   (in number of bytes)
    */
    //--------------------------------------------------------------------------
    virtual void decompressMesh(unsigned int aMeshID,
            char*& apData,
            unsigned int& aSize);


    //--------------------------------------------------------------------------
    /// Get an arcball vector depending on the mouse movement
    /*
    *   @param x:   the new mouse position along the X-axis
    *   @param y:   the new mouse position along the Y-axis
    *   @return the corresponding arcball vector
    */
    //--------------------------------------------------------------------------
    virtual gVirtualXRay::Vec3<GLfloat> getArcballVector(int x, int y);


    //--------------------------------------------------------------------------
    /// render the patient to the shadow map.
    //--------------------------------------------------------------------------
    void renderToShadowMap();

    /// Window width
    int m_width;

    /// Window height
    int m_height;

    /// The zooming factor
    GLfloat m_zoom;

    /// Flag set to true when rotation using arcball is currently in use,
    /// false otherwise
    bool m_use_arc_ball;

    /// The last known position of the mouse pointer along the X-axis
    GLint m_last_x_position;

    /// The last known position of the mouse pointer along the Y-axis
    GLint m_last_y_position;

    /// Flag set to true when X-ray beam is displayed, false otherwise
    bool m_display_beam;

    /// Projection matrix used to display text
    gVirtualXRay::Matrix4x4<GLfloat> m_text_2D_projection_matrix;

    /// Transformation matrix used to rotate the 3D scene
    gVirtualXRay::Matrix4x4<GLfloat> m_scene_rotation_matrix;

    /// Transformation matrix used to rotate the patient
    gVirtualXRay::Matrix4x4<GLfloat> m_rotation_matrix;

    /// projection and transformation matrix used to create the shadow map
    gVirtualXRay::Matrix4x4<GLfloat> m_shadow_matrix;

    /// Translation vector used to centre the patient
    gVirtualXRay::VEC3 m_translation_vector;

    /// X-ray beam
    gVirtualXRay::XRayBeam m_xray_beam;

    /// X-ray detector
    gVirtualXRay::XRayDetector m_xray_detector;

    /// X-ray renderer
    gVirtualXRay::XRayRenderer m_xray_renderer;

    /// GLSL shader
    gVirtualXRay::Shader m_display_shader;

    /// Flag set to true when the meshes are displayed using wireframe,
    /// false otherwise
    bool m_display_wireframe;

    /// The angular step between two successive rotations
    gVirtualXRay::RATIONAL_NUMBER m_angle_step;

    /// The next angle to use
    float m_next_angle_to_process;

    /// The current angle
    float m_current_angle;

    /// The total simulation time
    float m_simulation_total_time;

    /// The number of rotations that have been performed
    unsigned int m_number_of_rotations;

    /// The current acquisition type
    AnatomicalDataHandler::AcquisitionType m_acquisition_type;

    /// Flag set to true when the help message is displayed, false otherwise
    bool m_display_help;

    /// The text renderer
    gVirtualXRay::TextRenderer m_font_rendered;

    ///the light direction (from the vertex to the camera)
    gVirtualXRay::VEC3 m_light_direction;

    ICPRPhamtom m_female_icrp_phantom;
};


//******************************************************************************
#include "AnatomicalDataHandler.inl"


#endif // __AnatomicalDataHandler_h
