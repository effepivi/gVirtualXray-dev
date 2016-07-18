/*

Copyright (c) 2016, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


#ifndef GVXR_GL_WIDGET_H
#define GVXR_GL_WIDGET_H


/**
********************************************************************************
*
*	@file		GLWidget.h
*
*	@brief      Qt4 widget to handle our OpenGL window.
*
*	@version	1.0
*
*	@date		13/07/2016
*
*	@author		Dr Franck P. Vidal
*
*	@section	License
*				BSD 3-Clause License.
*
*				For details on use and redistribution please refer
*				to http://opensource.org/licenses/BSD-3-Clause
*
*	@section	Copyright
*				(c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*				http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
*				BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************

#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <vector>

#include <QGLWidget>

#ifndef GVXR_VEC3_H
#include "gVirtualXRay/Vec3.h"
#endif

#ifndef GVXR_MATRIX4X4_H
#include "gVirtualXRay/Matrix4x4.h"
#endif

#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef GVXR_ATOMIC_ELEMENT_H
#include "gVirtualXRay/AtomicElement.h"
#endif

#ifndef GVXR_XRAY_BEAM_H
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef GVXR_XRAY_DETECTOR_H
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef GVXR_SHADER_H
#include "gVirtualXRay/Shader.h"
#endif

#ifndef GVXR_TEXT_RENDERER_H
#include "gVirtualXRay/TextRenderer.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//******************************************************************************
//  Class declaration
//******************************************************************************
class PolygonMesh;


//==============================================================================
/**
*   @class  GLWidget
*   @brief  GLWidget is a class to manage a Qt4 widget that handles our OpenGL window.
*/
//==============================================================================
class GLWidget: public QGLWidget
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

    /// New type defining a patient
   typedef enum
    {
        /// Patient 4
        P4,

        /// Patient 5
        P5
    } PatientID;


//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    /*
    *   @param apPparent:   the parent widget
    */
    //--------------------------------------------------------------------------
    GLWidget(QWidget* apPparent = 0);


    //--------------------------------------------------------------------------
    /// Destructor
    //--------------------------------------------------------------------------
    virtual ~GLWidget();


//*****************************************************************************
private:
    //--------------------------------------------------------------------------
    /// Set the minimum window size
    //--------------------------------------------------------------------------
    virtual QSize minimumSizeHint() const;


    //--------------------------------------------------------------------------
    /// Set the default window size
    //--------------------------------------------------------------------------
    virtual QSize sizeHint() const;


    //--------------------------------------------------------------------------
    /// Initialise OpenGL
    //--------------------------------------------------------------------------
    virtual void initializeGL();


    //--------------------------------------------------------------------------
    /// Display the 3D scene using OpenGL
    //--------------------------------------------------------------------------
    virtual void paintGL();


    //--------------------------------------------------------------------------
    /// Set the viewport and projection matrix when the window is resized
    /*
    *   @param aWidth:  the width of the widget in number of pixels
    *   @param aHeight: the height of the widget in number of pixels
    */
    //--------------------------------------------------------------------------
    virtual void resizeGL(int aWidth, int aHeight);


    //--------------------------------------------------------------------------
    /// Callback when a keyboard key is pressed
    /*
    *   @param anEvent: The Qt event
    */
    //--------------------------------------------------------------------------
    virtual void keyPressEvent(QKeyEvent* anEvent);


    //--------------------------------------------------------------------------
    /// Callback when a mouse button is pressed
    /*
    *   @param anEvent: The Qt event
    */
    //--------------------------------------------------------------------------
    virtual void mousePressEvent(QMouseEvent* anEvent);


    //--------------------------------------------------------------------------
    /// Callback when a mouse button is released
    /*
    *   @param anEvent: The Qt event
    */
    //--------------------------------------------------------------------------
    virtual void mouseReleaseEvent(QMouseEvent* anEvent);


    //--------------------------------------------------------------------------
    /// Callback when a mouse wheel is turned
    /*
    *   @param anEvent: The Qt event
    */
    //--------------------------------------------------------------------------
    virtual void wheelEvent(QWheelEvent* anEvent);


    //--------------------------------------------------------------------------
    /// Post an event to create the animation
    //--------------------------------------------------------------------------
    void animate();


    //--------------------------------------------------------------------------
    /// Callback when a mouse is moved
    /*
    *   @param anEvent: The Qt event
    */
    //--------------------------------------------------------------------------
    virtual void mouseMoveEvent(QMouseEvent* anEvent);


    //--------------------------------------------------------------------------
    /// Initialise GLEW
    //--------------------------------------------------------------------------
    void initGLEW();


    //--------------------------------------------------------------------------
    /// Initialise OpenGL
    //--------------------------------------------------------------------------
    void initGL();


    //--------------------------------------------------------------------------
    /// Initialise Freetype
    //--------------------------------------------------------------------------
    void initFreeType();


    //--------------------------------------------------------------------------
    /// Set the properties of the X-ray detector
    //--------------------------------------------------------------------------
    void loadDetector();


    //--------------------------------------------------------------------------
    /// Set the properties of the X-ray source
    //--------------------------------------------------------------------------
    void loadSource();


    //--------------------------------------------------------------------------
    /// Set the properties of the X-ray simulation
    //--------------------------------------------------------------------------
    void loadXRaySimulator();


    //--------------------------------------------------------------------------
    /// Load STL files (patient geometrical data)
    //--------------------------------------------------------------------------
    void loadSTLFiles();


    //--------------------------------------------------------------------------
    /// Activate the properties of a patient (geometry and Hounsfield values)
    /*
    *   @param aPatientID:  ID of the patient to use
    */
    //--------------------------------------------------------------------------
    void loadPatient(PatientID aPatientID);


    //--------------------------------------------------------------------------
    /// Activate a patient
    /*
    *   @param aPatientID:  ID of the patient to use
    */
    //--------------------------------------------------------------------------
    void usePatient(const PatientID& aPatientID);


    //--------------------------------------------------------------------------
    /// Reset the rotation angle and the acquisition type to their default values
    //--------------------------------------------------------------------------
    void reset();


    //--------------------------------------------------------------------------
    /// Compute an up-to-date X-ray image
    //--------------------------------------------------------------------------
    void updateXRayImage();


    //--------------------------------------------------------------------------
    /// Render the 3D scene using OpenGL
    //--------------------------------------------------------------------------
    void render();


    //--------------------------------------------------------------------------
    /// Display a patient using OpenGL
    /*
    *   @param apMeshSet:   The set of polygon meshes to display
    *   @param aShaderID:   The GLSL shader used to display apMeshSet
    */
    //--------------------------------------------------------------------------
    void displayPatient(std::vector<PolygonMesh>& apMeshSet,
                        GLint aShaderID);


    //--------------------------------------------------------------------------
    /// Display a help message
    //--------------------------------------------------------------------------
    void displayHelp();


    //--------------------------------------------------------------------------
    /// Decompress STL data from the RAM using the ZLib
    /*
    *   @param aMeshID: the ID of the polygon mesh to load
    *   @param aPatientID:  the ID of the patient corresponding to aMeshID
    *   @param apData:  the uncompressed polygon mesh data
    *   @param aSize:   the size of the uncompressed polygon mesh data (in number of bytes)
    */
    //--------------------------------------------------------------------------
    void decompressMesh(unsigned int aMeshID,
                        PatientID aPatientID,
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
    Vec3<GLfloat> getArcballVector(int x, int y);


    //--------------------------------------------------------------------------
    /// Update the rotation matrix depending on the mouse movement
    /*
    *   @param aRotationMatrix: the rotation matrix (input/output)
    *   @param aXmousePosition: the new mouse position along the X-axis
    *   @param aYmousePosition: the new mouse position along the Y-axis
    */
    //--------------------------------------------------------------------------
    void computeRotation(MATRIX4& aRotationMatrix,
            int aXmousePosition,
            int aYmousePosition);


    /// The zooming factor
    GLfloat m_zoom;

    /// Flag set to true when rotation using arcball is currently in use, false otherwise
    bool m_use_arc_ball;

    /// The last known position of the mouse pointer along the X-axis
    GLint m_last_x_position;

    /// The last known position of the mouse pointer along the Y-axis
    GLint m_last_y_position;

    /// Flag set to true when X-ray beam is displayed, false otherwise
    bool m_display_beam;

    /// Projection matrix used to display text
    Matrix4x4<GLfloat> m_text_2D_projection_matrix;

    /// Transformation matrix used to rotate the 3D scene
    Matrix4x4<GLfloat> m_scene_rotation_matrix;

    /// Transformation matrix used to rotate Patient 4
    Matrix4x4<GLfloat> m_P4_rotation_matrix;

    /// Transformation matrix used to rotate Patient 5
    Matrix4x4<GLfloat> m_P5_rotation_matrix;

    /// Translation vector used to centre Patient 4
    VEC3 m_P4_translation_vector;

    /// Translation vector used to centre Patient 5
    VEC3 m_P5_translation_vector;

    /// X-ray beam
    XRayBeam m_xray_beam;

    /// X-ray detector
    XRayDetector m_xray_detector;

    /// X-ray renderer
    XRayRenderer m_xray_renderer;

    /// GLSL shader
    Shader m_display_shader;

    /// Set of polygon meshes for Patient 4
    std::vector<PolygonMesh> m_p_P4_polygon_mesh_set;

    /// Set of polygon meshes for Patient 5
    std::vector<PolygonMesh> m_p_P5_polygon_mesh_set;

    /// Flag that keeps track of which patient is currently in use
    GLWidget::PatientID m_current_patient;

    /// Flag set to true when the meshes are displayed using wireframe, false otherwise
    bool m_display_wireframe;

    /// The angular step between two successive rotations
    RATIONAL_NUMBER m_angle_step;

    /// The next angle to use
    float m_next_angle_to_process;

    /// The current angle
    float m_current_angle;

    /// The total simulation time
    float m_simulation_total_time;

    /// The number of rotations that have been performed
    unsigned int m_number_of_rotations;

    /// The current acquisition type
    GLWidget::AcquisitionType m_acquisition_type;

#ifdef HAS_FREETYPE
    /// Flag set to true when the help message is displayed, false otherwise
    bool m_display_help;

    /// The text renderer
    TextRenderer m_font_rendered;
#endif

    /// Flag set to true if the patients have to be swapped at the next iteration, false otherwise
    bool m_swap_patient;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "GLWidget.inl"


#endif // GVXR_GL_WIDGET_H
