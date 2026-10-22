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

#ifndef SGVXR_Window_h
#include "Window.h"
#endif

#ifndef SGVXR_Acquisition_h
#include "acquisition.h"
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

#ifdef HAS_GLFW
constexpr int KEY_F1     = GLFW_KEY_F1;
constexpr int KEY_F2     = GLFW_KEY_F2;
constexpr int KEY_F3     = GLFW_KEY_F3;
constexpr int KEY_F4     = GLFW_KEY_F4;
constexpr int KEY_1      = GLFW_KEY_1;
constexpr int KEY_2      = GLFW_KEY_2;
constexpr int KEY_3      = GLFW_KEY_3;
constexpr int KEY_4      = GLFW_KEY_4;
constexpr int KEY_ESCAPE = GLFW_KEY_ESCAPE;
constexpr int KEY_B      = GLFW_KEY_B;
constexpr int KEY_C      = GLFW_KEY_C;
constexpr int KEY_D      = GLFW_KEY_D;
constexpr int KEY_H      = GLFW_KEY_H;
constexpr int KEY_L      = GLFW_KEY_L;
constexpr int KEY_N      = GLFW_KEY_N;
constexpr int KEY_Q      = GLFW_KEY_Q;
constexpr int KEY_R      = GLFW_KEY_R;
constexpr int KEY_S      = GLFW_KEY_S;
constexpr int KEY_V      = GLFW_KEY_V;
constexpr int KEY_W      = GLFW_KEY_W;
constexpr int KEY_UP     = GLFW_KEY_UP;
constexpr int KEY_DOWN   = GLFW_KEY_DOWN;
constexpr int KEY_LEFT   = GLFW_KEY_LEFT;
constexpr int KEY_RIGHT  = GLFW_KEY_RIGHT;

constexpr int PRESS      = GLFW_PRESS;
constexpr int RELEASE    = GLFW_RELEASE;

constexpr int MOD_SHIFT  = GLFW_MOD_SHIFT;
constexpr int MOD_ALT    = GLFW_MOD_ALT;
constexpr int MOD_CTRL   = GLFW_MOD_CONTROL;
#else
constexpr int KEY_F1     = 0;
constexpr int KEY_F2     = 0;
constexpr int KEY_F3     = 0;
constexpr int KEY_F4     = 0;
constexpr int KEY_1      = 0;
constexpr int KEY_2      = 0;
constexpr int KEY_3      = 0;
constexpr int KEY_4      = 0;
constexpr int KEY_ESCAPE = 0;
constexpr int KEY_B      = 0;
constexpr int KEY_C      = 0;
constexpr int KEY_D      = 0;
constexpr int KEY_H      = 0;
constexpr int KEY_L      = 0;
constexpr int KEY_N      = 0;
constexpr int KEY_Q      = 0;
constexpr int KEY_R      = 0;
constexpr int KEY_S      = 0;
constexpr int KEY_V      = 0;
constexpr int KEY_W      = 0;
constexpr int KEY_UP     = 0;
constexpr int KEY_DOWN   = 0;
constexpr int KEY_LEFT   = 0;
constexpr int KEY_RIGHT  = 0;

constexpr int PRESS      = 0;
constexpr int RELEASE    = 0;

constexpr int MOD_SHIFT  = 0;
constexpr int MOD_ALT    = 0;
constexpr int MOD_CTRL   = 0;
#endif

extern gVirtualXRay::XRayRenderer g_xray_renderer;
extern gVirtualXRay::XRayDetector g_xray_detector; // X-ray detector

extern std::map<int, std::unique_ptr<SimpleGVXR::Window> > g_p_window_set;      // The set of windows
extern std::vector<int> g_p_window_id_stack;           // The stack of window IDs

extern bool g_use_arc_ball;
extern bool g_S_key_pressed;
extern bool g_D_key_pressed;


//******************************************************************************
//  Function declarations
//******************************************************************************
void updateSOD(float aStep);
void updateODD(float aStep);
void updateCameraOnscroll(double xoffset, double yoffset);


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


//-----------------------------
void keyCallback(int key,
				 int scancode,
				 int action,
				 int mods,
				 int aWindowID)
//-----------------------------
{
	// Display in the last window that has been created
	if (aWindowID == -1)
	{
		// Nothing to do
		if (g_p_window_id_stack.empty() || g_p_window_set.empty())
		{
			gVirtualXRay::LOGGER.logWarning("No active window to destroy.") << std::endl;
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
				// Perform the action
#ifdef HAS_GLFW
				SimpleGVXR::glfwKeyCallback(search_window->second->getContext().getWindowHandle(),
					key,
					scancode,
					action,
					mods);
#endif
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


//--------------------------------------------------------------------------------------
void mouseButtonCallback(int aButton, int aButtonState, int aModifierKey, int aWindowID)
//--------------------------------------------------------------------------------------
{
	// Display in the last window that has been created
	if (aWindowID == -1)
	{
		// Nothing to do
		if (g_p_window_id_stack.empty() || g_p_window_set.empty())
		{
			gVirtualXRay::LOGGER.logWarning("No active window to destroy.") << std::endl;
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
				// Perform the action
#ifdef HAS_GLFW
				SimpleGVXR::glfwMouseButtonCallback(search_window->second->getContext().getWindowHandle(),
					aButton, aButtonState, aModifierKey);
#endif
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


//------------------------------------------------------------
void cursorPositionCallback(double x, double y, int aWindowID)
//------------------------------------------------------------
{
	// Display in the last window that has been created
	if (aWindowID == -1)
	{
		// Nothing to do
		if (g_p_window_id_stack.empty() || g_p_window_set.empty())
		{
			gVirtualXRay::LOGGER.logWarning("No active window to destroy.") << std::endl;
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
				// Perform the action
#ifdef HAS_GLFW
				SimpleGVXR::glfwCursorPositionCallback(search_window->second->getContext().getWindowHandle(),
					x, y);
#endif
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


//----------------------------------------------------------------
void scrollCallback(double xoffset, double yoffset, int aWindowID)
//----------------------------------------------------------------
{
	// Display in the last window that has been created
	if (aWindowID == -1)
	{
		// Nothing to do
		if (g_p_window_id_stack.empty() || g_p_window_set.empty())
		{
			gVirtualXRay::LOGGER.logWarning("No active window to destroy.") << std::endl;
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
				// Perform the action
				std::cout << "xoffset: " << xoffset << "\tyoffset: " << yoffset << std::endl;
				if (!g_S_key_pressed && !g_D_key_pressed)
				{
					updateCameraOnscroll(xoffset, yoffset);
				}
				// Update the SOD
				else if (g_S_key_pressed)
				{
					// Get the current SOD
					float sod = getSOD("mm");
					auto step = static_cast<float>(0.05 * sod * yoffset);
					updateSOD(step);
				}
				// Update the ODD
				else if (g_D_key_pressed)
				{
					// Get the current ODD
					float odd = getODD("mm");
					auto step = static_cast<float>(0.05 * odd * yoffset);
					updateODD(step);
				}
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


//------------------------------------
void useAutoUnbindContext(bool aState)
//------------------------------------
{
	for (const auto& [key, window] : g_p_window_set)
	{
		window->useAutoUnbindContext(aState);
	}
}


//-------------------------------------------------------
void updateCameraOnscroll(double xoffset, double yoffset)
//-------------------------------------------------------
{
	// Scrolling along the Y-axis
	if (fabs(yoffset) > EPSILON)
	{
		g_use_arc_ball = false;

		// Retrieve the view vector and its size
		gVirtualXRay::VEC3 view_direction = g_camera_lookat - g_camera_position;
		auto distance = static_cast<float>(view_direction.length());

		// Make it a unit vector
		view_direction.normalise();

		// Update the camera position
		g_camera_position =  g_camera_lookat - view_direction * (distance + static_cast<float>(yoffset * 0.05 *
			distance));
		gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
										 g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
											g_camera_up[0], g_camera_up[1], g_camera_up[2]);

		// Update the frustrum
		g_initial_far = distance * 20.0;
	}
}