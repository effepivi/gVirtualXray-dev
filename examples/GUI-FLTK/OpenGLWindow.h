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


/**
********************************************************************************
*
*   @file       OpenGLWindow.h
*
*   @brief      Class to handle the OpenGL rendering in FLTK.
*
*   @version    1.0
*
*   @date       26/02/2014
*
*   @author     Steven Hampson
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


#ifndef __OpenGLWindow_h
#define __OpenGLWindow_h


//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <FL/Fl_Gl_Window.H>

#ifndef __PolygonMesh_h
#include "gVirtualXRay/PolygonMesh.h"
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

#ifndef __StereoHelper_h
#include "gVirtualXRay/StereoHelper.h"
#endif

#if (defined(_WIN32) || defined(_WIN64))
#define _USE_MATH_DEFINES
#endif


//******************************************************************************
//  Function declaration
//******************************************************************************


//------------------------------------------------------------------------------
/// Idle callback.
/**
*   @param  apData: a pointer on the OpenGLWindow
*/
//------------------------------------------------------------------------------
void idleCallback(void* apData);


//==============================================================================
/**
*   @class  OpenGLWindow
*   @brief  OpenGLWindow is a class to handle the OpenGL rendering in FLTK.
*/
//==============================================================================
class OpenGLWindow: public Fl_Gl_Window
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Constructor.
    /**
     *  @param X:   position of the window along the horizontal axis
     *  @param Y:   position of the window along the vertical axis
     *  @param aWidth:  width of the window
     *  @param aHeight: height of the window
     *  @param apTitle: title of the window
     */
    //--------------------------------------------------------------------------
    OpenGLWindow(int X, int Y, int aWidth, int aHeight, char* apTitle);


    //--------------------------------------------------------------------------
    /// Destructor.
    //--------------------------------------------------------------------------
    virtual ~OpenGLWindow();


    //--------------------------------------------------------------------------
    /// Draw the 3D scene.
    //--------------------------------------------------------------------------
    virtual void draw();


    //--------------------------------------------------------------------------
    /// Event handler.
    /**
     *  @param anEvent: the kind of event received
     *  @return 0 if the event was not used or understood,
     *          1 if the event was used and can be deleted
     */
    //--------------------------------------------------------------------------
    virtual int handle(int anEvent);


    //--------------------------------------------------------------------------
    /// Set the rotation angle.
    /**
     *  @param anAngle: the new angle of rotation
     */
    //--------------------------------------------------------------------------
    void setObjectRotationAngle(float anAngle);


    //--------------------------------------------------------------------------
    /// Accessor on the angle of rotation.
    /**
     *  @return the angle of rotation
     */
    //--------------------------------------------------------------------------
    //float getObjectRotationAngle() const;


    //--------------------------------------------------------------------------
    /// Accessor on the animation flag.
    /**
     *  @return the animation flag
     */
    //--------------------------------------------------------------------------
    float getAnimationFlag() const;


    //--------------------------------------------------------------------------
    /// Accessor on the increment value of the rotation angle.
    /**
     *  @return the increment value
     */
    //--------------------------------------------------------------------------
    float getRotationIncrement() const;

    void setZoom(int aValue);

    void rotateObject(const double anAngle);

    void rotateDetector(const double aValue);

    void rotateEnvironment(const double aValue);

    void setNegative(int aValue);

    void setWireframe(const double aValue);

    void setSourceParallel(double aValue);

    void setStereo(bool aValue);

    void setSourceLine(double aValue);

    void setSourceSquare(double aValue);

    void setSourceCube(double aValue);

    void setPointSource(double aValue);

    void setBeam(double aValue);

    void loadObject(double aValue);

    void setIntraocularDistance(float aValue);

    void computeRotation(gVirtualXRay::MATRIX4& aRotationMatrix);

    gVirtualXRay::VEC3 getArcballVector(int x, int y);


    void setExecutable(char *argv);


//******************************************************************************
private:
    //--------------------------------------------------------------------------
    /// Initialise OpenGL.
    //--------------------------------------------------------------------------
    void initializeGL();


    //--------------------------------------------------------------------------
    /// Render the 3D scene.
    //--------------------------------------------------------------------------
    void render();


    //--------------------------------------------------------------------------
    /// Resize the window.
    /**
     *  @param X:   the position of the window along the horizontal axis
     *  @param Y:   the position of the window along the vertical axis
     *  @param W:   the width of the window
     *  @param H:   the height of the window
     */
    //--------------------------------------------------------------------------
    virtual void resize(int X,int Y,int W,int H);


    //--------------------------------------------------------------------------
    /// Set the X-ray detector.
    //--------------------------------------------------------------------------
    void loadDetector();


    //--------------------------------------------------------------------------
    /// Set the X-ray source.
    //--------------------------------------------------------------------------
    void loadSource();


    //--------------------------------------------------------------------------
    /// Set the X-ray renderer.
    //--------------------------------------------------------------------------
    void loadXRaySimulator();


    //--------------------------------------------------------------------------
    /// Set the X-ray renderer.
    /**
     *  @param aFileName:   the file name of the 3D triangle mesh of
     *                      the scanned object
     */
    //--------------------------------------------------------------------------
    void loadSTLFile(const std::string& aFileName);


    //--------------------------------------------------------------------------
    /// Set the GLSL shader to render the 3D scene.
    //--------------------------------------------------------------------------
    void loadShader();


    //--------------------------------------------------------------------------
    /// Update the transformation matrices.
    /**
     *  @param aWidth:  the width of the window
     *  @param aHeight: the height of the window
     */
    //--------------------------------------------------------------------------
    void reshape(int aWidth,int aHeight);


    //--------------------------------------------------------------------------
    /// Update the X-ry image using lazy evaluation.
    //--------------------------------------------------------------------------
    void updateXRayImage();


    std::string m_executable_path;


    /// The scene rotation angle
    float m_scene_rotation_angle;


    /// The object rotation angle
    float m_object_rotation_angle;


    /// The increment value of the rotation angle
    float m_rotation_increment;


    /// The animation flag
    bool  m_animating;


    /// Flag set to false when OpenGL is initialised
    bool  m_first_time;


    /// Flag set to true if the 3D object is rendered in wireframe mode,
    /// to false if the 3D object is rendered normally
    bool m_display_wireframe;


    /// Flag set to true if the X-ray beam is displayed, to false if it isn't
    bool m_display_beam;


    /// Flag set to true if the X-ray detector is displayed, to false if it isn't
    bool m_display_detector;


    /// Stereo manager
    gVirtualXRay::StereoHelper m_stereo_helper;


    /// Flag set to true if the X-ray image is up-to-date,
    /// to false if it needs updating (used for lazy evaluation)
    bool m_is_xray_image_up_to_date;


    /// Distance between the camera and the centre of the scene
    float m_zoom;


    bool m_use_arc_ball;


    int m_last_x_position;


    int m_last_y_position;


    int m_current_x_position;


    int m_current_y_position;


    float m_rotation_speed;


    /// 3D triangle mesh of the scanned object
    gVirtualXRay::PolygonMesh     m_polygon_data;


    /// X-ray beam
    gVirtualXRay::XRayBeam     m_xray_beam;


    /// X-ray detector
    gVirtualXRay::XRayDetector m_xray_detector;


    /// X-ray renderer
    gVirtualXRay::XRayRenderer m_xray_renderer;


    /// GLSL shader to render the 3D scene
    gVirtualXRay::Shader       m_display_shader;


    /// Transformation matrix of the 3D scene
    gVirtualXRay::MATRIX4         m_scene_rotation_matrix;


    /// Transformation matrix of the X-ray detector
    gVirtualXRay::MATRIX4         m_detector_rotation_matrix;


    /// Transformation matrix of the 3D object
    gVirtualXRay::MATRIX4         m_sample_rotation_matrix;
};


//******************************************************************************
#include "OpenGLWindow.inl"


#endif
