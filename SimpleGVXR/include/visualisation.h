/*

Copyright (c) 2024-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
UK Research and Innovation, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of UK Research and Innovation nor the names of its contributors
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


#ifndef SGVXR_Visualisation_h
#define SGVXR_Visualisation_h


/**
********************************************************************************
*
*   @file       visualisation.h
*
*   @brief      Expose here all the functions of SimpleGVXR related to 
*               the 3D visualisation.
*
*   @version    1.0
*
*   @date       07/05/2024
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
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, May 2024-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************


//------------------------------------------------------------------------------
/// Display/Hide the beam in the 3D visualisation.
/**
*   @param  aState: if true the beam will be displayed, otherwise it won't be (default value: true)
*/
//------------------------------------------------------------------------------
void displayBeam(bool aState = true);


//------------------------------------------------------------------------------
/// Display/Hide the detector in the 3D visualisation.
/**
*   @param  aState: if true the detector will be displayed, otherwise it won't be (default value: true)
*/
//------------------------------------------------------------------------------
void displayDetector(bool aState = true);


//------------------------------------------------------------------------------
/// Display/Hide the normal vectors of the sample in the 3D visualisation.
/**
*   @param  aState: if true the normal vectors will be displayed, otherwise they won't be (default value: true)
*/
//------------------------------------------------------------------------------
void displayNormalVectors(bool aState = true);


//------------------------------------------------------------------------------
/// Use/Don't use wireframe rendering in the 3D visualisation.
/**
*   @param  aState: if true the sample will be displayed in wireframe, otherwise it'll be solid (default value: true)
*/
//------------------------------------------------------------------------------
void useWireframe(bool aState = true);


//------------------------------------------------------------------------------
/// Use/Don't lighting in the 3D visualisation.
/**
*   @param  aState: if true lighting will be on, otherwise it'll be off (default value: true)
*/
//------------------------------------------------------------------------------
void useLighing(bool aState = true);


//------------------------------------------------------------------------------
/// Use/Don't a negative LUT to show the X-ray projection in the 3D visualisation.
/**
*   @param  aState: if true the X-ray projection will be shown in negative, otherwise in positive (default value: true)
*/
//------------------------------------------------------------------------------
void useNegative(bool aState = true);


//------------------------------------------------------------------------------
/// Set the size of the axis widget in the 3D visualisation.
/**
*   @param  aLength:    Size of the axis widget
*   @param aUnitOfLength:   the unit of length corresponding to aLength.
*                          Acceptable values are:
*                          "um", "micrometre",  "micrometer",
*                          "mm", "millimetre",  "millimeter",
*                          "cm", "centimetre",  "centimeter",
*                          "dm",  "decimetre",  "decimeter",
*                          "m",   "metre",      "meter",
*                          "dam", "decametre",  "decameter",
*                          "hm",  "hectometre", "hectometer",
*                          "km",  "kilometre",  "kilometer"
*/
//------------------------------------------------------------------------------
void setAxisLength(float aLength, const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the size of the marker widgets in the 3D visualisation.
/**
*   @param  aLength:    Size of the marker widgets
*   @param aUnitOfLength:   the unit of length corresponding to aLength.
*                          Acceptable values are:
*                          "um", "micrometre",  "micrometer",
*                          "mm", "millimetre",  "millimeter",
*                          "cm", "centimetre",  "centimeter",
*                          "dm",  "decimetre",  "decimeter",
*                          "m",   "metre",      "meter",
*                          "dam", "decametre",  "decameter",
*                          "hm",  "hectometre", "hectometer",
*                          "km",  "kilometre",  "kilometer"
*/
//------------------------------------------------------------------------------
void setMarkerLength(float aLength, const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Replace the projection matrix by a orthographic projection matrix.
/// It behaves as the old glOrtho function.
/**
*   @param  left:   Specify the coordinates for the left vertical clipping plane
*   @param  right:  Specify the coordinates for the right vertical
*                   clipping plane
*   @param  bottom: Specify the coordinates for the bottom horizontal
*                   clipping plane
*   @param  top:    Specify the coordinates for the top horizontal
*                   clipping plane
*   @param  zNear:  Specify the distances to the near clipping plane.
*                   The value must be positive
*   @param  zFar:   Specify the distances to the far clipping plane.
*                   The value must be positive
*   @param aUnitOfLength:   the unit of length.
*                          Acceptable values are:
*                          "um", "micrometre",  "micrometer",
*                          "mm", "millimetre",  "millimeter",
*                          "cm", "centimetre",  "centimeter",
*                          "dm",  "decimetre",  "decimeter",
*                          "m",   "metre",      "meter",
*                          "dam", "decametre",  "decameter",
*                          "hm",  "hectometre", "hectometer",
*                          "km",  "kilometre",  "kilometer"
*/
//------------------------------------------------------------------------------
void orthographic(double left,
        double right,
        double bottom,
        double top,
        double zNear,
        double zFar,
        const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Replace the projection matrix by a perspective projection matrix.
/// It behaves as the old gluPerspective function.
/**
*   @param  fovy:   specifies the field of view angle, in degrees, in
*                   the y-direction.
*   @param  aspect: Specifies the aspect ratio that determines
*                   the field of view in the x direction.
*                   The aspect ratio is the ratio of x (width) to y (height).
*   @param  zNear:  Specifies the distance from the viewer to the near
*                   clipping plane (always positive)
*   @param  zFar:   Specifies the distance from the viewer to the far
*                   clipping plane (always positive).
*   @param aUnitOfLength:   the unit of length.
*                          Acceptable values are:
*                          "um", "micrometre",  "micrometer",
*                          "mm", "millimetre",  "millimeter",
*                          "cm", "centimetre",  "centimeter",
*                          "dm",  "decimetre",  "decimeter",
*                          "m",   "metre",      "meter",
*                          "dam", "decametre",  "decameter",
*                          "hm",  "hectometre", "hectometer",
*                          "km",  "kilometre",  "kilometer"
*/
//------------------------------------------------------------------------------
void perspective(double fovy, double aspect, double  zNear, double  zFar,
        const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Replace the modelling-viewing matrix by a viewing transformation matrix.
/// It behaves as the old gluLookAt function.
/**
*   @param  eyeX:       Specifies the position of the eye point along the X-axis
*   @param  eyeY:       Specifies the position of the eye point along the Y-axis
*   @param  eyeZ:       Specifies the position of the eye point along the Z-axis
*   @param  lookAtX:    Specifies the position of the reference point along
*                       the X-axis
*   @param  lookAtY:    Specifies the position of the reference point along
*                       the Y-axis
*   @param  lookAtZ:    Specifies the position of the reference point along
*                       the Z-axis
*   @param  upX         Specifies the direction of the up vector along
*                       the X-axis
*   @param  upY         Specifies the direction of the up vector along
*                       the Y-axis
*   @param  upZ         Specifies the direction of the up vector along
*                       the Z-axis
*   @param aUnitOfLength:   the unit of length.
*                          Acceptable values are:
*                          "um", "micrometre",  "micrometer",
*                          "mm", "millimetre",  "millimeter",
*                          "cm", "centimetre",  "centimeter",
*                          "dm",  "decimetre",  "decimeter",
*                          "m",   "metre",      "meter",
*                          "dam", "decametre",  "decameter",
*                          "hm",  "hectometre", "hectometer",
*                          "km",  "kilometre",  "kilometer"
*/
//------------------------------------------------------------------------------
void lookAt(float eyeX,
        float eyeY,
        float eyeZ,
        float lookAtX,
        float lookAtY,
        float lookAtZ,
        float upX,
        float upY,
        float upZ,
        const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Replace the modelling-viewing matrix by a viewing transformation matrix.
/// It behaves as the old gluLookAt function.
/**
*   @param  eyeX:       Specifies the position of the eye point along the X-axis
*   @param  eyeY:       Specifies the position of the eye point along the Y-axis
*   @param  eyeZ:       Specifies the position of the eye point along the Z-axis
*   @param  lookAtX:    Specifies the position of the reference point along
*                       the X-axis
*   @param  lookAtY:    Specifies the position of the reference point along
*                       the Y-axis
*   @param  lookAtZ:    Specifies the position of the reference point along
*                       the Z-axis
*   @param  upX         Specifies the direction of the up vector along
*                       the X-axis
*   @param  upY         Specifies the direction of the up vector along
*                       the Y-axis
*   @param  upZ         Specifies the direction of the up vector along
*                       the Z-axis
*   @param aUnitOfLength:   the unit of length.
*                          Acceptable values are:
*                          "um", "micrometre",  "micrometer",
*                          "mm", "millimetre",  "millimeter",
*                          "cm", "centimetre",  "centimeter",
*                          "dm",  "decimetre",  "decimeter",
*                          "m",   "metre",      "meter",
*                          "dam", "decametre",  "decameter",
*                          "hm",  "hectometre", "hectometer",
*                          "km",  "kilometre",  "kilometer"
*/
//------------------------------------------------------------------------------
void lookAt(double eyeX,
        double eyeY,
        double eyeZ,
        double lookAtX,
        double lookAtY,
        double lookAtZ,
        double upX,
        double upY,
        double upZ,
        const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the camera parameters.
/**
*   @param aParameterSet: the parameters in the format as follows:
*                        - Camera position along the X-axis
*                        - Camera position along the Y-axis
*                        - Camera position along the Z-axis
*                        - Reference point along the X-axis
*                        - Reference point along the Y-axis
*                        - Reference point along the Z-axis
*                        - Up vector along the X-axis
*                        - Up vector along the Y-axis
*                        - Up vector along the Z-axis
*   @param aUnitOfLength:   the unit of length.
*                          Acceptable values are:
*                          "um", "micrometre",  "micrometer",
*                          "mm", "millimetre",  "millimeter",
*                          "cm", "centimetre",  "centimeter",
*                          "dm",  "decimetre",  "decimeter",
*                          "m",   "metre",      "meter",
*                          "dam", "decametre",  "decameter",
*                          "hm",  "hectometre", "hectometer",
*                          "km",  "kilometre",  "kilometer"
*/
//------------------------------------------------------------------------------
void setCameraParameters(const std::vector<float>& aParameterSet,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the camera parameters. 
/**
*   @param aParameterSet: the parameters in the format as follows:
*                        - Camera position along the X-axis
*                        - Camera position along the Y-axis
*                        - Camera position along the Z-axis
*                        - Reference point along the X-axis
*                        - Reference point along the Y-axis
*                        - Reference point along the Z-axis
*                        - Up vector along the X-axis
*                        - Up vector along the Y-axis
*                        - Up vector along the Z-axis
*   @param aUnitOfLength:   the unit of length.
*                          Acceptable values are:
*                          "um", "micrometre",  "micrometer",
*                          "mm", "millimetre",  "millimeter",
*                          "cm", "centimetre",  "centimeter",
*                          "dm",  "decimetre",  "decimeter",
*                          "m",   "metre",      "meter",
*                          "dam", "decametre",  "decameter",
*                          "hm",  "hectometre", "hectometer",
*                          "km",  "kilometre",  "kilometer"
*/
//------------------------------------------------------------------------------
void setCameraParameters(const std::vector<double>& aParameterSet,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Accessor on the parameters of the camera.
/**
*   @param aUnitOfLength:   the unit of length.
*                          Acceptable values are:
*                          "um", "micrometre",  "micrometer",
*                          "mm", "millimetre",  "millimeter",
*                          "cm", "centimetre",  "centimeter",
*                          "dm",  "decimetre",  "decimeter",
*                          "m",   "metre",      "meter",
*                          "dam", "decametre",  "decameter",
*                          "hm",  "hectometre", "hectometer",
*                          "km",  "kilometre",  "kilometer"
*
*   @return the parameters of the camera in the format as follows:
*                        - Camera position along the X-axis
*                        - Camera position along the Y-axis
*                        - Camera position along the Z-axis
*                        - Reference point along the X-axis
*                        - Reference point along the Y-axis
*                        - Reference point along the Z-axis
*                        - Up vector along the X-axis
*                        - Up vector along the Y-axis
*                        - Up vector along the Z-axis
*/
//------------------------------------------------------------------------------
const std::vector<float> getCameraParameters(const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the camera position.
/**
*   @param aPosition: the camera position
*   @param aUnitOfLength:   the unit of length.
*                          Acceptable values are:
*                          "um", "micrometre",  "micrometer",
*                          "mm", "millimetre",  "millimeter",
*                          "cm", "centimetre",  "centimeter",
*                          "dm",  "decimetre",  "decimeter",
*                          "m",   "metre",      "meter",
*                          "dam", "decametre",  "decameter",
*                          "hm",  "hectometre", "hectometer",
*                          "km",  "kilometre",  "kilometer"
*/
//------------------------------------------------------------------------------
void setCameraPosition(const std::vector<float>& aPosition,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the camera position.
/**
*   @param aPosition: the camera position
*   @param aUnitOfLength:   the unit of length.
*                          Acceptable values are:
*                          "um", "micrometre",  "micrometer",
*                          "mm", "millimetre",  "millimeter",
*                          "cm", "centimetre",  "centimeter",
*                          "dm",  "decimetre",  "decimeter",
*                          "m",   "metre",      "meter",
*                          "dam", "decametre",  "decameter",
*                          "hm",  "hectometre", "hectometer",
*                          "km",  "kilometre",  "kilometer"
*/
//------------------------------------------------------------------------------
void setCameraPosition(const std::vector<double>& aPosition,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the camera position.
/**
*   @param aXPosition: the camera position along the X-axis
*   @param aYPosition: the camera position along the Y-axis
*   @param aZPosition: the camera position along the Z-axis
*   @param aUnitOfLength:   the unit of length.
*                          Acceptable values are:
*                          "um", "micrometre",  "micrometer",
*                          "mm", "millimetre",  "millimeter",
*                          "cm", "centimetre",  "centimeter",
*                          "dm",  "decimetre",  "decimeter",
*                          "m",   "metre",      "meter",
*                          "dam", "decametre",  "decameter",
*                          "hm",  "hectometre", "hectometer",
*                          "km",  "kilometre",  "kilometer"
*/
//------------------------------------------------------------------------------
void setCameraPosition(float aXPosition, float aYPosition, float aZPosition,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the camera position.
/**
*   @param aXPosition: the camera position along the X-axis
*   @param aYPosition: the camera position along the Y-axis
*   @param aZPosition: the camera position along the Z-axis
*   @param aUnitOfLength:   the unit of length.
*                          Acceptable values are:
*                          "um", "micrometre",  "micrometer",
*                          "mm", "millimetre",  "millimeter",
*                          "cm", "centimetre",  "centimeter",
*                          "dm",  "decimetre",  "decimeter",
*                          "m",   "metre",      "meter",
*                          "dam", "decametre",  "decameter",
*                          "hm",  "hectometre", "hectometer",
*                          "km",  "kilometre",  "kilometer"
*/
//------------------------------------------------------------------------------
void setCameraPosition(double aXPosition, double aYPosition, double aZPosition,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Accessor on the camera position.
/**
*   @param aUnitOfLength:   the unit of length.
*                          Acceptable values are:
*                          "um", "micrometre",  "micrometer",
*                          "mm", "millimetre",  "millimeter",
*                          "cm", "centimetre",  "centimeter",
*                          "dm",  "decimetre",  "decimeter",
*                          "m",   "metre",      "meter",
*                          "dam", "decametre",  "decameter",
*                          "hm",  "hectometre", "hectometer",
*                          "km",  "kilometre",  "kilometer"
*   @return the camera position
*/
//------------------------------------------------------------------------------
const std::vector<float> getCameraPosition(const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the camera reference point.
/**
*   @param aPosition: the camera reference point
*   @param aUnitOfLength:   the unit of length.
*                          Acceptable values are:
*                          "um", "micrometre",  "micrometer",
*                          "mm", "millimetre",  "millimeter",
*                          "cm", "centimetre",  "centimeter",
*                          "dm",  "decimetre",  "decimeter",
*                          "m",   "metre",      "meter",
*                          "dam", "decametre",  "decameter",
*                          "hm",  "hectometre", "hectometer",
*                          "km",  "kilometre",  "kilometer"
*/
//------------------------------------------------------------------------------
void setCameraReferencePoint(const std::vector<float>& aPosition,
        const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the camera reference point.
/**
*   @param aPosition: the camera reference point
*   @param aUnitOfLength:   the unit of length.
*                          Acceptable values are:
*                          "um", "micrometre",  "micrometer",
*                          "mm", "millimetre",  "millimeter",
*                          "cm", "centimetre",  "centimeter",
*                          "dm",  "decimetre",  "decimeter",
*                          "m",   "metre",      "meter",
*                          "dam", "decametre",  "decameter",
*                          "hm",  "hectometre", "hectometer",
*                          "km",  "kilometre",  "kilometer"
*/
//------------------------------------------------------------------------------
void setCameraReferencePoint(const std::vector<double>& aPosition,
        const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the camera reference point.
/**
*   @param aXPosition: the camera reference point along the X-axis
*   @param aYPosition: the camera reference point along the Y-axis
*   @param aZPosition: the camera reference point along the Z-axis
*   @param aUnitOfLength:   the unit of length.
*                          Acceptable values are:
*                          "um", "micrometre",  "micrometer",
*                          "mm", "millimetre",  "millimeter",
*                          "cm", "centimetre",  "centimeter",
*                          "dm",  "decimetre",  "decimeter",
*                          "m",   "metre",      "meter",
*                          "dam", "decametre",  "decameter",
*                          "hm",  "hectometre", "hectometer",
*                          "km",  "kilometre",  "kilometer"
*/
//------------------------------------------------------------------------------
void setCameraReferencePoint(float aXPosition, float aYPosition, float aZPosition,
        const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the camera reference point.
/**
*   @param aXPosition: the camera reference point along the X-axis
*   @param aYPosition: the camera reference point along the Y-axis
*   @param aZPosition: the camera reference point along the Z-axis
*   @param aUnitOfLength:   the unit of length.
*                          Acceptable values are:
*                          "um", "micrometre",  "micrometer",
*                          "mm", "millimetre",  "millimeter",
*                          "cm", "centimetre",  "centimeter",
*                          "dm",  "decimetre",  "decimeter",
*                          "m",   "metre",      "meter",
*                          "dam", "decametre",  "decameter",
*                          "hm",  "hectometre", "hectometer",
*                          "km",  "kilometre",  "kilometer"
*/
//------------------------------------------------------------------------------
void setCameraReferencePoint(double aXPosition, double aYPosition, double aZPosition,
        const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Accessor on the camera reference point.
/**
*   @param aUnitOfLength:   the unit of length.
*                          Acceptable values are:
*                          "um", "micrometre",  "micrometer",
*                          "mm", "millimetre",  "millimeter",
*                          "cm", "centimetre",  "centimeter",
*                          "dm",  "decimetre",  "decimeter",
*                          "m",   "metre",      "meter",
*                          "dam", "decametre",  "decameter",
*                          "hm",  "hectometre", "hectometer",
*                          "km",  "kilometre",  "kilometer"
*   @return the camera reference point
*/
//------------------------------------------------------------------------------
const std::vector<float> getCameraReferencePoint(const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the camera up vector.
/**
*  @param aUpVector: the camera up vector
*/
 //------------------------------------------------------------------------------
void setCameraUp(const std::vector<float>& aUpVector);


//------------------------------------------------------------------------------
/// Set the camera up vector.
/**
 *  @param aUpVector: the camera up vector
 */
 //------------------------------------------------------------------------------
void setCameraUp(const std::vector<double>& aUpVector);


//------------------------------------------------------------------------------
/// Set the camera up vector.
/**
 *  @param aXUpVector: the camera up vector along the X-axis
 *  @param aYUpVector: the camera up vector along the Y-axis
 *  @param aZUpVector: the camera up vector along the Z-axis
 */
 //------------------------------------------------------------------------------
void setCameraUp(float aXUpVector, float aYUpVector, float aZUpVector);


//------------------------------------------------------------------------------
/// Set the camera up vector.
/**
 *  @param aXUpVector: the camera up vector along the X-axis
 *  @param aYUpVector: the camera up vector along the Y-axis
 *  @param aZUpVector: the camera up vector along the Z-axis
 */
 //------------------------------------------------------------------------------
void setCameraUp(double aXUpVector, double aYUpVector, double aZUpVector);


//------------------------------------------------------------------------------
/// Accessor on the camera up vector.
/**
 *  @return the camera up vector
 */
 //------------------------------------------------------------------------------
const std::vector<float> getCameraUp();


//------------------------------------------------------------------------------
/// Add the current matrix to the model/view matrix stack.
/// It behaves as the combination of the old
/// glMatrixMode(GL_MODELVIEW);glPushMatrix() functions.
//------------------------------------------------------------------------------
void pushModelViewMatrix();


//------------------------------------------------------------------------------
/// Restore the matrix from the model/view matrix stack,
/// and remove the last matrix of the stack.
/// glMatrixMode(GL_MODELVIEW);glPopMatrix() functions.
//------------------------------------------------------------------------------
void popModelViewMatrix();


//------------------------------------------------------------------------------
/// Restore the matrix from the model/view matrix stack,
/// and remove the last matrix of the stack.
/// glMatrixMode(GL_MODELVIEW);glRotate() functions.
/**
 *   @param  anAngle:   the angle of rotation in degrees
 *   @param  x: the coordinate of the rotation axis along the X-axis
 *   @param  y: the coordinate of the rotation axis along the Y-axis
 *   @param  z: the coordinate of the rotation axis along the Z-axis
 */
//------------------------------------------------------------------------------
void rotateModelView(double anAngle, double x, double y, double z);


// Definition of global input keycodes, actions, and modifiers.

// Function Keys
/// Function key F1.
extern const int KEY_F1;

/// Function key F2.
extern const int KEY_F2;

/// Function key F3.
extern const int KEY_F3;

/// Function key F4.
extern const int KEY_F4;

// Number Keys
/// Numeric key 1.
extern const int KEY_1;

/// Numeric key 2.
extern const int KEY_2;

/// Numeric key 3.
extern const int KEY_3;

/// Numeric key 4.
extern const int KEY_4;

// Letter and Utility Keys
/// Escape key.
extern const int KEY_ESCAPE;

/// Letter key B.
extern const int KEY_B;

/// Letter key C.
extern const int KEY_C;

/// Letter key D.
extern const int KEY_D;

/// Letter key H.
extern const int KEY_H;

/// Letter key L.
extern const int KEY_L;

/// Letter key N.
extern const int KEY_N;

/// Letter key Q.
extern const int KEY_Q;

/// Letter key R.
extern const int KEY_R;

/// Letter key S.
extern const int KEY_S;

/// Letter key V.
extern const int KEY_V;

/// Letter key W.
extern const int KEY_W;

/// Up arrow key.
extern const int KEY_UP;

/// Down arrow key.
extern const int KEY_DOWN;

/// Left arrow key.
extern const int KEY_LEFT;

/// Right arrow key.
extern const int KEY_RIGHT;

// Key Actions
/// Key state indicating a button is pressed down.
extern const int PRESS;

/// Key state indicating a button is released.
extern const int RELEASE;

// Keyboard Modifiers
/// Shift key modifier mask.
extern const int MODIF_SHIFT;

/// Alt key modifier mask.
extern const int MODIF_ALT;

/// Control key modifier mask (short alias).
extern const int MODIF_CTRL;

/// Control key modifier mask (full designation).
extern const int MODIF_CONTROL;

// Mouse Buttons
/// Left mouse button identifier.
extern const int MOUSE_BUTTON_LEFT;

/// Middle mouse button (scroll wheel click) identifier.
extern const int MOUSE_BUTTON_MIDDLE;

/// Right mouse button identifier.
extern const int MOUSE_BUTTON_RIGHT;


//------------------------------------------------------------------------------
/// Handle a keyboard event.
///
/// Process a keyboard event and update the state of the interactive viewer.
/// This function can be used from both graphical and headless environments,
/**
 *   @param  key: the key code
 *   @param  scancode: the platform-specific scan code of the key
 *   @param  action: the key action (e.g. press, release, or repeat)
 *   @param  mods: a bit field describing the active modifier keys
 *                 (e.g. Ctrl, Shift, and Alt)
 *   @param  aWindowID: the identifier of the window associated with the
 *                      event, or -1 for the default window
 */
//------------------------------------------------------------------------------
void keyCallback(
    int key,
    int scancode,
    int action,
    int mods,
    int aWindowID = -1);


//------------------------------------------------------------------------------
/// Handle a mouse button event.
///
/// Process a mouse button event and update the state of the interactive viewer.
/// This function can be used from both graphical and headless environments.
/**
 *   @param  aButton: the mouse button identifier
 *                    (e.g. left, middle, or right button)
 *   @param  aButtonState: the button action
 *                         (e.g. press or release)
 *   @param  aModifierKey: a bit field describing the active modifier keys
 *                         (e.g. Ctrl, Shift, and Alt)
 *   @param  aWindowID: the identifier of the window associated with the
 *                      event, or -1 for the default window
 */
//------------------------------------------------------------------------------
void mouseButtonCallback(
    int aButton,
    int aButtonState,
    int aModifierKey,
    double x,
    double y,
    int aWindowID = -1);


//------------------------------------------------------------------------------
/// Handle a cursor position event.
///
/// Process a mouse movement event and update the state of the interactive
/// viewer. The cursor coordinates are expressed in window coordinates.
/// This function can be used from both graphical and headless environments.
/**
 *   @param  x: the horizontal cursor position
 *   @param  y: the vertical cursor position
 *   @param  aWindowID: the identifier of the window associated with the
 *                      event, or -1 for the default window
 */
//------------------------------------------------------------------------------
void cursorPositionCallback(
    double x,
    double y,
    int aWindowID = -1);


//------------------------------------------------------------------------------
/// Handle a mouse scroll event.
///
/// Process a mouse wheel event and update the state of the interactive viewer.
/// The scroll offsets represent the wheel displacement along the horizontal
/// and vertical directions. This function can be used from both graphical and
/// headless environments.
/**
 *   @param  xoffset: the horizontal scroll offset
 *   @param  yoffset: the vertical scroll offset
 *   @param  aWindowID: the identifier of the window associated with the
 *                      event, or -1 for the default window
 */
//------------------------------------------------------------------------------
void scrollCallback(
    double xoffset,
    double yoffset,
    int aWindowID = -1);


//------------------------------------------------------------------------------
/// Automatically unbind the OpenGL context after a draw, or not. This is useful
/// in case there a multithreaded application is used or a Jupyter widget.
/**
 *   @param  aState: automatically unbind if true, otherwise do not
 *                   automatically unbind (default value: true)
 */
//------------------------------------------------------------------------------
void useAutoUnbindContext(bool aState = true);


#endif