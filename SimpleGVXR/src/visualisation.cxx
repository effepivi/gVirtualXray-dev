/*

Copyright (c) 2026-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk),
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
*   @date       06/08/2026
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
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, August 2026-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_XRAY_UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#ifndef GVXR_XRAY_TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef SGVXR_Visualisation_h
#include "visualisation.h"
#endif


//******************************************************************************
//  Namespace
//******************************************************************************


//******************************************************************************
//  Global variables
//******************************************************************************
double g_initial_fovy(45);                // Field of view along
// the y-axis
double g_initial_near(0.5 * gVirtualXRay::cm);  // Near clipping plane
double g_initial_far(500.0 * gVirtualXRay::cm); // Far clipping plane

gVirtualXRay::VEC3 g_camera_position(0.0, 0.0, g_initial_far / 2.0);
gVirtualXRay::VEC3 g_camera_lookat(0.0, 0.0, 0.0);
gVirtualXRay::VEC3 g_camera_up(0.0, 1.0, 0.0);


//******************************************************************************
//  Function definitions
//******************************************************************************


//------------------------
void lookAt(float eye_x,
			float eye_y,
			float eye_z,
			float lookAtX,
			float lookAtY,
			float lookAtZ,
			float upX,
			float upY,
			float upZ)
//------------------------
{
	g_camera_position = gVirtualXRay::VEC3(eye_x, eye_y, eye_z);
	g_camera_lookat = gVirtualXRay::VEC3(lookAtX, lookAtY, lookAtZ);
	g_camera_up = gVirtualXRay::VEC3(upX, upY, upZ).normal();

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
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
	g_camera_position = gVirtualXRay::VEC3(eye_x, eye_y, eye_z);
	g_camera_lookat = gVirtualXRay::VEC3(lookAtX, lookAtY, lookAtZ);
	g_camera_up = gVirtualXRay::VEC3(upX, upY, upZ).normal();

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}


//--------------------------------------------------
void setCameraParameters(const float* aParameterSet)
//--------------------------------------------------
{
	g_camera_position = gVirtualXRay::VEC3(aParameterSet[0], aParameterSet[1], aParameterSet[2]);
	g_camera_lookat = gVirtualXRay::VEC3(aParameterSet[3], aParameterSet[4], aParameterSet[5]);
	g_camera_up = gVirtualXRay::VEC3(aParameterSet[6], aParameterSet[7], aParameterSet[8]).normal();

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}


//---------------------------------------------------
void setCameraParameters(const double* aParameterSet)
//---------------------------------------------------
{
	g_camera_position = gVirtualXRay::VEC3(aParameterSet[0], aParameterSet[1], aParameterSet[2]);
	g_camera_lookat = gVirtualXRay::VEC3(aParameterSet[3], aParameterSet[4], aParameterSet[5]);
	g_camera_up = gVirtualXRay::VEC3(aParameterSet[6], aParameterSet[7], aParameterSet[8]).normal();

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}


//--------------------------------------------
const std::vector<float> getCameraParameters()
//--------------------------------------------
{
	std::vector<float> p_parameter_set = {
		g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]
	};

	return p_parameter_set;
}


//--------------------------------------------
void setCameraPosition(const float* aPosition)
//--------------------------------------------
{
	g_camera_position = gVirtualXRay::VEC3(aPosition[0], aPosition[1], aPosition[2]);

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}


//---------------------------------------------
void setCameraPosition(const double* aPosition)
//---------------------------------------------
{
	g_camera_position = gVirtualXRay::VEC3(aPosition[0], aPosition[1], aPosition[2]);

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}


//--------------------------------------------------------------------------
void setCameraPosition(float aXPosition, float aYPosition, float aZPosition)
//--------------------------------------------------------------------------
{
	g_camera_position[0] = aXPosition;
	g_camera_position[1] = aYPosition;
	g_camera_position[2] = aZPosition;

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}

//-----------------------------------------------------------------------------
void setCameraPosition(double aXPosition, double aYPosition, double aZPosition)
//-----------------------------------------------------------------------------
{
	g_camera_position[0] = aXPosition;
	g_camera_position[1] = aYPosition;
	g_camera_position[2] = aZPosition;

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}


//------------------------------------------
const std::vector<float> getCameraPosition()
//------------------------------------------
{
	std::vector<float> p_position = {
		g_camera_position[0], g_camera_position[1], g_camera_position[2]
	};

	return p_position;
}


//--------------------------------------------------
void setCameraReferencePoint(const float* aPosition)
//--------------------------------------------------
{
	g_camera_lookat = gVirtualXRay::VEC3(aPosition[0], aPosition[1], aPosition[2]);
}


//---------------------------------------------------
void setCameraReferencePoint(const double* aPosition)
//---------------------------------------------------
{
	g_camera_lookat = gVirtualXRay::VEC3(aPosition[0], aPosition[1], aPosition[2]);
}


//--------------------------------------------------------------------------------
void setCameraReferencePoint(float aXPosition, float aYPosition, float aZPosition)
//--------------------------------------------------------------------------------
{
	g_camera_lookat[0] = aXPosition;
	g_camera_lookat[1] = aYPosition;
	g_camera_lookat[2] = aZPosition;

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}


//-----------------------------------------------------------------------------------
void setCameraReferencePoint(double aXPosition, double aYPosition, double aZPosition)
//-----------------------------------------------------------------------------------
{
	g_camera_lookat[0] = aXPosition;
	g_camera_lookat[1] = aYPosition;
	g_camera_lookat[2] = aZPosition;

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}


//------------------------------------------------
const std::vector<float> getCameraReferencePoint()
//------------------------------------------------
{
	std::vector<float> p_position = {
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2]
	};

	return p_position;
}


//--------------------------------------
void setCameraUp(const float* aUpVector)
//--------------------------------------
{
	g_camera_up = gVirtualXRay::VEC3(aUpVector[0], aUpVector[1], aUpVector[2]);

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}


//---------------------------------------
void setCameraUp(const double* aUpVector)
//---------------------------------------
{
	g_camera_up = gVirtualXRay::VEC3(aUpVector[0], aUpVector[1], aUpVector[2]);

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}


//--------------------------------------------------------------------
void setCameraUp(float aXUpVector, float aYUpVector, float aZUpVector)
//--------------------------------------------------------------------
{
	g_camera_up[0] = aXUpVector;
	g_camera_up[1] = aYUpVector;
	g_camera_up[2] = aZUpVector;

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}


//-----------------------------------------------------------------------
void setCameraUp(double aXUpVector, double aYUpVector, double aZUpVector)
//-----------------------------------------------------------------------
{
	g_camera_up[0] = aXUpVector;
	g_camera_up[1] = aYUpVector;
	g_camera_up[2] = aZUpVector;

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}


//------------------------------------
const std::vector<float> getCameraUp()
//------------------------------------
{
	std::vector<float> p_up = {
		g_camera_up[0], g_camera_up[1], g_camera_up[2]
	};

	return p_up;
}
