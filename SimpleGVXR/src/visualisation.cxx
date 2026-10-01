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

#ifndef GVXR_TEXT_RENDERER_H
#include "gVirtualXRay/TextRenderer.h"
#endif

#ifndef GVXR_COORDINATE_SYSTEM_MESH_H
#include "gVirtualXRay/CoordinateSystemMesh.h"
#endif

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

// To perform the computations of the X-ray simulation
#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef SGVXR_Utils_h
#include "Utils.h"
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

gVirtualXRay::TextRenderer g_text_renderer; // Text visualisation

bool g_display_beam(true);
bool g_display_detector(true);
bool g_display_normal_vectors(false);
bool g_display_wireframe(false);
bool g_use_lighing(true);

gVirtualXRay::CoordinateSystemMesh g_world_coordinate_system_mesh(2.0F * gVirtualXRay::cm);

extern gVirtualXRay::XRayRenderer g_xray_renderer;
extern gVirtualXRay::XRayDetector g_xray_detector; // X-ray detector


//******************************************************************************
//  Function definitions
//******************************************************************************


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
	g_world_coordinate_system_mesh.setLength(static_cast<float>(aLength * getUnitOfLength(aUnitOfLength)));
}


//-------------------------------------------------------------------
void setMarkerLength(float aLength, const std::string& aUnitOfLength)
//-------------------------------------------------------------------
{
	g_xray_detector.setWidgetLength(static_cast<float>(aLength * getUnitOfLength(aUnitOfLength)));
}



//-------------------------------------------------
void orthographic(double left,
				  double right,
				  double bottom,
				  double top,
				  double nearZ,
				  double farZ,
				  const std::string& aUnitOfLength)
//-------------------------------------------------
{
	float unit_of_length = getUnitOfLength(aUnitOfLength);

	gVirtualXRay::loadOrthoProjectionMatrix(
		left * unit_of_length, right * unit_of_length,
		bottom * unit_of_length, top * unit_of_length,
		nearZ * unit_of_length, farZ * unit_of_length);
}

//------------------------------------------------
void perspective(double initial_fovy,
				 double screen_aspect_ratio,
				 double initial_near,
				 double initial_far,
				 const std::string& aUnitOfLength)
//------------------------------------------------
{
	float unit_of_length = getUnitOfLength(aUnitOfLength);

	gVirtualXRay::loadPerspectiveProjectionMatrix(initial_fovy,
			screen_aspect_ratio,
			initial_near * unit_of_length,
			initial_far * unit_of_length);
}


//-------------------------------------------
void lookAt(float eye_x,
			float eye_y,
			float eye_z,
			float lookAtX,
			float lookAtY,
			float lookAtZ,
			float upX,
			float upY,
			float upZ,
			const std::string& aUnitOfLength)
//-------------------------------------------
{
	float unit_of_length = getUnitOfLength(aUnitOfLength);

	g_camera_position = gVirtualXRay::VEC3(eye_x, eye_y, eye_z) * unit_of_length;
	g_camera_lookat = gVirtualXRay::VEC3(lookAtX, lookAtY, lookAtZ) * unit_of_length;
	g_camera_up = gVirtualXRay::VEC3(upX, upY, upZ).normal();

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}


//-------------------------------------------
void lookAt(double eye_x,
			double eye_y,
			double eye_z,
			double lookAtX,
			double lookAtY,
			double lookAtZ,
			double upX,
			double upY,
			double upZ,
			const std::string& aUnitOfLength)
//-------------------------------------------
{
	float unit_of_length = getUnitOfLength(aUnitOfLength);

	g_camera_position = gVirtualXRay::VEC3(eye_x, eye_y, eye_z) * unit_of_length;
	g_camera_lookat = gVirtualXRay::VEC3(lookAtX, lookAtY, lookAtZ) * unit_of_length;
	g_camera_up = gVirtualXRay::VEC3(upX, upY, upZ).normal();

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}


//---------------------------------------------------------------
void setCameraParameters(const std::vector<float>& aParameterSet,
	                     const std::string& aUnitOfLength)
//---------------------------------------------------------------
{
	if (aParameterSet.size() != 9) {
		throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
			"Wrong input, 9 floating pont numbers where expected.");
	}

	float unit_of_length = getUnitOfLength(aUnitOfLength);

	g_camera_position = gVirtualXRay::VEC3(aParameterSet[0], aParameterSet[1], aParameterSet[2]) * unit_of_length;
	g_camera_lookat = gVirtualXRay::VEC3(aParameterSet[3], aParameterSet[4], aParameterSet[5]) * unit_of_length;
	g_camera_up = gVirtualXRay::VEC3(aParameterSet[6], aParameterSet[7], aParameterSet[8]).normal();

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}


//----------------------------------------------------------------
void setCameraParameters(const std::vector<double>& aParameterSet,
                         const std::string& aUnitOfLength)
//----------------------------------------------------------------
{
	if (aParameterSet.size() != 9) {
		throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
			"Wrong input, 9 floating pont numbers where expected.");
	}

	float unit_of_length = getUnitOfLength(aUnitOfLength);

	g_camera_position = gVirtualXRay::VEC3(aParameterSet[0], aParameterSet[1], aParameterSet[2]) * unit_of_length;
	g_camera_lookat = gVirtualXRay::VEC3(aParameterSet[3], aParameterSet[4], aParameterSet[5]) * unit_of_length;
	g_camera_up = gVirtualXRay::VEC3(aParameterSet[6], aParameterSet[7], aParameterSet[8]).normal();

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}


//----------------------------------------------------------------------------
const std::vector<float> getCameraParameters(const std::string& aUnitOfLength)
//----------------------------------------------------------------------------
{
	float unit_of_length = getUnitOfLength(aUnitOfLength);

	std::vector<float> p_parameter_set = {
		g_camera_position[0] / unit_of_length,
		g_camera_position[1]/ unit_of_length,
		g_camera_position[2]/ unit_of_length,

		g_camera_lookat[0]/ unit_of_length,
		g_camera_lookat[1]/ unit_of_length,
		g_camera_lookat[2]/ unit_of_length,

		g_camera_up[0]/ unit_of_length,
		g_camera_up[1]/ unit_of_length,
		g_camera_up[2]/ unit_of_length
	};

	return p_parameter_set;
}


//---------------------------------------------------------
void setCameraPosition(const std::vector<float>& aPosition,
                       const std::string& aUnitOfLength)
//---------------------------------------------------------
{
	if (aPosition.size() != 3) {
		throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
			"Wrong input, 3 floating pont numbers where expected.");
	}

	float unit_of_length = getUnitOfLength(aUnitOfLength);

	g_camera_position = gVirtualXRay::VEC3(aPosition[0], aPosition[1], aPosition[2]) * unit_of_length;

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}


//----------------------------------------------------------
void setCameraPosition(const std::vector<double>& aPosition,
	                   const std::string& aUnitOfLength)
//----------------------------------------------------------
{
	if (aPosition.size() != 3) {
		throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
			"Wrong input, 3 floating pont numbers where expected.");
	}

	float unit_of_length = getUnitOfLength(aUnitOfLength);

	g_camera_position = gVirtualXRay::VEC3(aPosition[0], aPosition[1], aPosition[2]) * unit_of_length;

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}


//--------------------------------------------------------------------------
void setCameraPosition(float aXPosition, float aYPosition, float aZPosition,
	                   const std::string& aUnitOfLength)
//--------------------------------------------------------------------------
{
	float unit_of_length = getUnitOfLength(aUnitOfLength);

	g_camera_position[0] = aXPosition * unit_of_length;
	g_camera_position[1] = aYPosition * unit_of_length;
	g_camera_position[2] = aZPosition * unit_of_length;

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}

//-----------------------------------------------------------------------------
void setCameraPosition(double aXPosition, double aYPosition, double aZPosition,
	                   const std::string& aUnitOfLength)
//-----------------------------------------------------------------------------
{
	double unit_of_length = getUnitOfLength(aUnitOfLength);

	g_camera_position[0] = aXPosition * unit_of_length;
	g_camera_position[1] = aYPosition * unit_of_length;
	g_camera_position[2] = aZPosition * unit_of_length;

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}


//--------------------------------------------------------------------------
const std::vector<float> getCameraPosition(const std::string& aUnitOfLength)
//--------------------------------------------------------------------------
{
	float unit_of_length = getUnitOfLength(aUnitOfLength);

	std::vector<float> p_position = {
		g_camera_position[0] / unit_of_length,
		g_camera_position[1] / unit_of_length,
		g_camera_position[2] / unit_of_length
	};

	return p_position;
}


//---------------------------------------------------------------
void setCameraReferencePoint(const std::vector<float>& aPosition,
	                         const std::string& aUnitOfLength)
//---------------------------------------------------------------
{
	if (aPosition.size() != 3) {
		throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
			"Wrong input, 3 floating pont numbers where expected.");
	}

	float unit_of_length = getUnitOfLength(aUnitOfLength);

	g_camera_lookat = gVirtualXRay::VEC3(aPosition[0], aPosition[1], aPosition[2]) * unit_of_length;

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}


//----------------------------------------------------------------
void setCameraReferencePoint(const std::vector<double>& aPosition,
	                         const std::string& aUnitOfLength)
//----------------------------------------------------------------
{
	if (aPosition.size() != 3) {
		throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
			"Wrong input, 3 floating pont numbers where expected.");
	}

	float unit_of_length = getUnitOfLength(aUnitOfLength);

	g_camera_lookat = gVirtualXRay::VEC3(aPosition[0], aPosition[1], aPosition[2]) * unit_of_length;

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}


//--------------------------------------------------------------------------------
void setCameraReferencePoint(float aXPosition, float aYPosition, float aZPosition,
	                         const std::string& aUnitOfLength)
//--------------------------------------------------------------------------------
{
	float unit_of_length = getUnitOfLength(aUnitOfLength);

	g_camera_lookat[0] = aXPosition * unit_of_length;
	g_camera_lookat[1] = aYPosition * unit_of_length;
	g_camera_lookat[2] = aZPosition * unit_of_length;

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}


//-----------------------------------------------------------------------------------
void setCameraReferencePoint(double aXPosition, double aYPosition, double aZPosition,
	                         const std::string& aUnitOfLength)
//-----------------------------------------------------------------------------------
{
	double unit_of_length = getUnitOfLength(aUnitOfLength);

	g_camera_lookat[0] = aXPosition * unit_of_length;
	g_camera_lookat[1] = aYPosition * unit_of_length;
	g_camera_lookat[2] = aZPosition * unit_of_length;

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}


//--------------------------------------------------------------------------------
const std::vector<float> getCameraReferencePoint(const std::string& aUnitOfLength)
//--------------------------------------------------------------------------------
{
	float unit_of_length = getUnitOfLength(aUnitOfLength);

	std::vector<float> p_position = {
		g_camera_lookat[0] / unit_of_length,
		g_camera_lookat[1] / unit_of_length,
		g_camera_lookat[2] / unit_of_length
	};

	return p_position;
}


//---------------------------------------------------
void setCameraUp(const std::vector<float>& aUpVector)
//---------------------------------------------------
{
	if (aUpVector.size() != 3) {
		throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
			"Wrong input, 3 floating pont numbers where expected.");
	}

	g_camera_up = gVirtualXRay::VEC3(aUpVector[0], aUpVector[1], aUpVector[2]);

	gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
		g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
		g_camera_up[0], g_camera_up[1], g_camera_up[2]);
}


//----------------------------------------------------
void setCameraUp(const std::vector<double>& aUpVector)
//----------------------------------------------------
{
	if (aUpVector.size() != 3) {
		throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
			"Wrong input, 3 floating pont numbers where expected.");
	}

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
