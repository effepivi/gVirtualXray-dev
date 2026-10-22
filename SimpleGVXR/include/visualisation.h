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
 *  @param aParameterSet: the parameters in the format as follows:
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
void setCameraParameters(const std::vector<float>& aParameterSet,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the camera parameters. 
/**
 *  @param aParameterSet: the parameters in the format as follows:
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
void setCameraParameters(const std::vector<double>& aParameterSet,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Accessor on the parameters of the camera.
/**
 *  @return the parameters of the camera in the format as follows:
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
 *  @param aPosition: the camera position
 */
//------------------------------------------------------------------------------
void setCameraPosition(const std::vector<float>& aPosition,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the camera position.
/**
 *  @param aPosition: the camera position
 */
//------------------------------------------------------------------------------
void setCameraPosition(const std::vector<double>& aPosition,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the camera position.
/**
 *  @param aXPosition: the camera position along the X-axis
 *  @param aYPosition: the camera position along the Y-axis
 *  @param aZPosition: the camera position along the Z-axis
 */
//------------------------------------------------------------------------------
void setCameraPosition(float aXPosition, float aYPosition, float aZPosition,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the camera position.
/**
 *  @param aXPosition: the camera position along the X-axis
 *  @param aYPosition: the camera position along the Y-axis
 *  @param aZPosition: the camera position along the Z-axis
 */
//------------------------------------------------------------------------------
void setCameraPosition(double aXPosition, double aYPosition, double aZPosition,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Accessor on the camera position.
/**
 *  @return the camera position
 */
 //------------------------------------------------------------------------------
const std::vector<float> getCameraPosition(const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the camera reference point.
/**
 *  @param aPosition: the camera reference point
 */
//------------------------------------------------------------------------------
void setCameraReferencePoint(const std::vector<float>& aPosition,
        const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the camera reference point.
/**
 *  @param aPosition: the camera reference point
 */
 //------------------------------------------------------------------------------
void setCameraReferencePoint(const std::vector<double>& aPosition,
        const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the camera reference point.
/**
 *  @param aXPosition: the camera reference point along the X-axis
 *  @param aYPosition: the camera reference point along the Y-axis
 *  @param aZPosition: the camera reference point along the Z-axis
 */
 //------------------------------------------------------------------------------
void setCameraReferencePoint(float aXPosition, float aYPosition, float aZPosition,
        const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the camera reference point.
/**
 *  @param aXPosition: the camera reference point along the X-axis
 *  @param aYPosition: the camera reference point along the Y-axis
 *  @param aZPosition: the camera reference point along the Z-axis
 */
 //------------------------------------------------------------------------------
void setCameraReferencePoint(double aXPosition, double aYPosition, double aZPosition,
        const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Accessor on the camera reference point.
/**
 *  @return the camera reference point
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


extern const int KEY_F1;
extern const int KEY_F2;
extern const int KEY_F3;
extern const int KEY_F4;
extern const int KEY_1;
extern const int KEY_2;
extern const int KEY_3;
extern const int KEY_4;
extern const int KEY_ESCAPE;
extern const int KEY_B;
extern const int KEY_C;
extern const int KEY_D;
extern const int KEY_H;
extern const int KEY_L;
extern const int KEY_N;
extern const int KEY_Q;
extern const int KEY_R;
extern const int KEY_S;
extern const int KEY_V;
extern const int KEY_W;
extern const int KEY_UP;
extern const int KEY_DOWN;
extern const int KEY_LEFT;
extern const int KEY_RIGHT;

extern const int PRESS;
extern const int RELEASE;

extern const int MOD_SHIFT;
extern const int MOD_ALT;
extern const int MOD_CTRL;


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
/// Automatically unbind the OpenGL context after a draw, or not. This is useful
/// in case there a multithreaded application is used or a Jupyter widget.
/**
 *   @param  aState: automatically unbind if true, otherwise do not
 *                   automatically unbind (default value: true)
 */
//------------------------------------------------------------------------------
void useAutoUnbindContext(bool aState = true);


#endif