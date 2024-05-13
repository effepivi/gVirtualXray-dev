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


#ifndef __SGVXR_Visualisation_h
#define __SGVXR_Visualisation_h


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




void displayBeam(bool aState = true);
void displayDetector(bool aState = true);
void displayNormalVectors(bool aState = true);
void useWireframe(bool aState = true);
void useLighing(bool aState = true);
void useNegative(bool aState = true);

void setAxisLength(float aLength, const std::string& aUnitOfLength);


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
        double zFar);


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
void perspective(double fovy, double aspect, double  zNear, double  zFar);


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
            double upZ);


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


#endif