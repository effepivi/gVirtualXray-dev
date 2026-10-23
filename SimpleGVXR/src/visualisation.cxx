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
#ifdef HAS_CLIPBOARD
#include "clip.h"
#endif

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

#ifndef SGVXR_Sample_h
#include "sample.h"
#endif

#ifndef SGVXR_Internal_h
#include "internal.h"
#endif

#ifndef SGVXR_Source_h
#include "source.h"
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
double g_initial_fovy(45.0);                // Field of view along
// the y-axis
double g_initial_near(0.5 * gVirtualXRay::cm);  // Near clipping plane
double g_initial_far(500.0 * gVirtualXRay::cm); // Far clipping plane

gVirtualXRay::VEC3 g_camera_position(0.0F, 0.0F, static_cast<float>(g_initial_far / 2.0));
gVirtualXRay::VEC3 g_camera_lookat(0.0F, 0.0F, 0.0F);
gVirtualXRay::VEC3 g_camera_up(0.0F, 1.0F, 0.0F);

gVirtualXRay::TextRenderer g_text_renderer; // Text visualisation

bool g_display_beam(true);
bool g_display_detector(true);
bool g_display_normal_vectors(false);
bool g_display_wireframe(false);
bool g_use_lighing(true);

gVirtualXRay::CoordinateSystemMesh g_world_coordinate_system_mesh(2.0F * gVirtualXRay::cm);


// The key, action, modifier and mouse button codes are part of the public API:
// they are what the caller passes to keyCallback and mouseButtonCallback.
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

constexpr int MOD_SHIFT   = GLFW_MOD_SHIFT;
constexpr int MOD_ALT     = GLFW_MOD_ALT;
constexpr int MOD_CTRL    = GLFW_MOD_CONTROL;
constexpr int MOD_CONTROL = GLFW_MOD_CONTROL;

constexpr int MOUSE_BUTTON_LEFT   = GLFW_MOUSE_BUTTON_LEFT;
constexpr int MOUSE_BUTTON_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE;
constexpr int MOUSE_BUTTON_RIGHT  = GLFW_MOUSE_BUTTON_RIGHT;
#else
constexpr int KEY_F1     = 290;
constexpr int KEY_F2     = 291;
constexpr int KEY_F3     = 292;
constexpr int KEY_F4     = 293;
constexpr int KEY_1      = 49;
constexpr int KEY_2      = 50;
constexpr int KEY_3      = 51;
constexpr int KEY_4      = 52;
constexpr int KEY_ESCAPE = 256;
constexpr int KEY_B      = 66;
constexpr int KEY_C      = 67;
constexpr int KEY_D      = 68;
constexpr int KEY_H      = 72;
constexpr int KEY_L      = 76;
constexpr int KEY_N      = 78;
constexpr int KEY_Q      = 81;
constexpr int KEY_R      = 82;
constexpr int KEY_S      = 83;
constexpr int KEY_V      = 86;
constexpr int KEY_W      = 87;
constexpr int KEY_UP     = 265;
constexpr int KEY_DOWN   = 264;
constexpr int KEY_LEFT   = 263;
constexpr int KEY_RIGHT  = 262;

constexpr int PRESS      = 1;
constexpr int RELEASE    = 0;

constexpr int MOD_SHIFT   = 0x0001;
constexpr int MOD_CTRL    = 0x0002;
constexpr int MOD_CONTROL = 0x0002;
constexpr int MOD_ALT     = 0x0004;

constexpr int MOUSE_BUTTON_LEFT   = 0;
constexpr int MOUSE_BUTTON_RIGHT  = 1;
constexpr int MOUSE_BUTTON_MIDDLE = 2;
#endif

const GLfloat ROTATION_SPEED(2.0);

extern gVirtualXRay::XRayRenderer g_xray_renderer;
extern gVirtualXRay::XRayDetector g_xray_detector; // X-ray detector

extern std::map<int, std::unique_ptr<SimpleGVXR::Window> > g_p_window_set;      // The set of windows
extern std::vector<int> g_p_window_id_stack;           // The stack of window IDs

extern bool g_use_arc_ball;
extern bool g_S_key_pressed;
extern bool g_D_key_pressed;

extern GLint g_last_x_position;
extern GLint g_last_y_position;
extern GLint g_current_x_position;
extern GLint g_current_y_position;
extern int g_button;
extern int g_button_state;

extern bool g_run_loop;
extern bool g_display_help;
extern bool g_1_key_pressed;
extern bool g_2_key_pressed;
extern bool g_3_key_pressed;
extern bool g_4_key_pressed;
extern bool g_left_arrow_key_pressed;
extern bool g_right_arrow_key_pressed;
extern bool g_up_arrow_key_pressed;
extern bool g_down_arrow_key_pressed;
// extern bool g_display_beam;
// extern bool g_display_wireframe;
// extern bool g_use_lighing;
// extern bool g_display_detector;
// extern bool g_display_normal_vectors;


//******************************************************************************
//  Function declarations
//******************************************************************************
void updateSOD(float aStep);
void updateODD(float aStep);
gVirtualXRay::Vec3<GLfloat> getArcballVector(int x,
											 int y,
											 float aCentreX,
											 float aCentreY,
											 float aRadius);
bool updateRotation(const gVirtualXRay::MATRIX4& aRotationMatrix);

void updateCameraOnscroll(double xoffset, double yoffset);
void udapteCursorPositionStatus(double x, double y, int aViewportWidth, int aViewportHeight);
void computeRotation(gVirtualXRay::MATRIX4& aRotationMatrix,
					 int aViewportWidth,
					 int aViewportHeight);
bool projectToWindow(const gVirtualXRay::VEC3& aPointInWorldCoordinates,
					 const gVirtualXRay::VEC3& aSideVector,
					 const gVirtualXRay::VEC3& anUpVector,
					 const gVirtualXRay::VEC3& aForwardVector,
					 int aViewportWidth,
					 int aViewportHeight,
					 float& aWindowX,
					 float& aWindowY);
void startArcBallRotation(double x, double y);
void stopArcBallRotation();
void updateSampleTranslation(float aStepX, float aStepY, float aStepZ);


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
			    // Ctrl+Shift+C
			    if (action == PRESS && key == KEY_C && (mods & MOD_CONTROL) && (mods & MOD_SHIFT))
			    {
			#ifdef HAS_CLIPBOARD
			#ifdef CLIP_ENABLE_IMAGE
			        std::cout << "(action == PRESS && key == KEY_C && (mods & MOD_CONTROL) && (mods & "
			                     "MOD_SHIFT))" << std::endl;
			        // Copy to the clipboard

			        // Retrieve the latest screenshot and format it in RGBA
			        auto xray_image = g_xray_renderer.getEnergyFluence();
			        const size_t width = xray_image.getWidth();
			        const size_t height = xray_image.getHeight();

			        std::cout << "Copy to clipboard a " << width << "x" << height <<
			            " radiograph" << std::endl;

			        // Allocate the RGBA data buffer
			        std::vector<unsigned char> p_dst(width * height * 4);
			        const float* p_src = xray_image.getRawData();
			        auto total_energy = static_cast<float>(getTotalEnergyWithDetectorResponse());

			        for (size_t i = 0; i < width * height; ++i)
			        {
			            auto pixel_value_flt = 255.0F * static_cast<float>(p_src[i]) / total_energy;
			            auto pixel_value_uint8 = static_cast<uint8_t>(round(pixel_value_flt));

			            p_dst[4 * i + 0] = pixel_value_uint8;
			            p_dst[4 * i + 1] = pixel_value_uint8;
			            p_dst[4 * i + 2] = pixel_value_uint8;
			            p_dst[4 * i + 3] = 255;
			        }

			        // Create the descriptor
			        clip::image_spec spec;
			        spec.width = width;
			        spec.height = height;
			        spec.bits_per_pixel = 8 * 4;
			        spec.bytes_per_row = width * 4;

			        spec.red_mask    = 0x000000ff;
			        spec.green_mask  = 0x0000ff00;
			        spec.blue_mask   = 0x00ff0000;
			        spec.alpha_mask  = 0xff000000;
			        spec.red_shift   = 0;
			        spec.green_shift = 8;
			        spec.blue_shift  = 16;
			        spec.alpha_shift = 24;

			        // Create the clip::image
			        clip::image img(&p_dst[0], spec);

			        // Copy the image to the clipboard
			        if (!clip::set_image(img))
			        {
			            gVirtualXRay::LOGGER.logWarning("Error copying the X-ray image to clipboard") << std::endl;
			        }

			        if (!clip::has(clip::image_format()))
			        {
			            gVirtualXRay::LOGGER.logWarning("The X-ray image was not copied to clipboard") << std::endl;
			        }
			#else
			        gVirtualXRay::LOGGER.logWarning("Copying the X-ray image to the system clipboard is not support, "
			                                        "image support was not enabled when compiling clip") << std::endl;
			#endif
			#else
			        gVirtualXRay::LOGGER.logWarning("Copying the X-ray image to the system clipboard is not support") << std::endl;
			#endif
			    }
			    // Ctrl+C
			    else if (action == PRESS && key == KEY_C && (mods & MOD_CONTROL))
			    {
			        std::cout << "(action == PRESS && key == KEY_C && (mods & MOD_CONTROL))" << std::endl;
			        search_window->second->copyScreenshot();
			    }
			    // Alt+F4
			    else if (action == PRESS && key == KEY_F4 && (mods & MOD_ALT))
			    {
			            g_run_loop = false;
			    }
			    else if (action == PRESS)
			    {
			        switch (key)
			        {
			        case KEY_Q:
			        case KEY_ESCAPE:
			            g_run_loop = false;
			            break;

			        case KEY_B:
			            g_display_beam = !g_display_beam;
			            break;

			        case KEY_W:
			            g_display_wireframe = !g_display_wireframe;
			            break;

			        case KEY_N:
			            g_xray_renderer.useNegativeFilteringFlag(
			                !g_xray_renderer.getNegativeFilteringFlag());
			            break;

			        case KEY_L:
			            g_use_lighing = !g_use_lighing;
			            break;

			        //case KEY_D:
			        //    g_display_detector = !g_display_detector;
			        //    break;

			        case KEY_V:
			            g_display_normal_vectors = !g_display_normal_vectors;
			            break;

			        case KEY_H:
			            g_display_help = !g_display_help;
			            break;

			        case KEY_S:
			            g_S_key_pressed = true;
			            break;

			        case KEY_D:
			            g_D_key_pressed = true;
			            break;

			        case KEY_1:
			            g_1_key_pressed = true;
			            break;

			        case KEY_2:
			            g_2_key_pressed = true;
			            break;

			        case KEY_3:
			            g_3_key_pressed = true;
			            break;

			        case KEY_4:
			            g_4_key_pressed = true;
			            break;

			        case KEY_LEFT:
			            g_left_arrow_key_pressed = true;
			            break;

			        case KEY_RIGHT:
			            g_right_arrow_key_pressed = true;
			            break;

			        case KEY_UP:
			            g_up_arrow_key_pressed = true;
			            break;

			        case KEY_DOWN:
			            g_down_arrow_key_pressed = true;
			            break;

			        case KEY_R:
			            {
			                search_window->second->restoreInitialViewAndGeometry();
			            }
			            break;

			        default:
			            break;
			        }
			    }
			    else if (action == RELEASE)
			    {
			        switch (key)
			        {
			            case KEY_S:
			                g_S_key_pressed = false;
			                break;

			            case KEY_D:
			                g_D_key_pressed = false;
			                break;

			            case KEY_1:
			                g_1_key_pressed = false;
			                break;

			            case KEY_2:
			                g_2_key_pressed = false;
			                break;

			            case KEY_3:
			                g_3_key_pressed = false;
			                break;

			            case KEY_4:
			                g_4_key_pressed = false;
			                break;

			            case KEY_LEFT:
			                g_left_arrow_key_pressed = false;
			                break;

			            case KEY_RIGHT:
			                g_right_arrow_key_pressed = false;
			                break;

			            case KEY_UP:
			                g_up_arrow_key_pressed = false;
			                break;

			            case KEY_DOWN:
			                g_down_arrow_key_pressed = false;
			                break;

			            default:
			                break;
			        }
			    }

			    // Change the SOD
			    if (g_S_key_pressed)
			    {
			        // Get the current SOD
			        float sod = getSOD("mm");
			        float step = 0.05F * sod;

			        // Increase the SOD
			        if (g_S_key_pressed && (g_up_arrow_key_pressed || g_right_arrow_key_pressed))
			        {
			            updateSOD(step);
			        }
			        // Decrease the SOD
			        else if (g_S_key_pressed && (g_down_arrow_key_pressed || g_left_arrow_key_pressed))
			        {
			            updateSOD(-step);
			        }
			    }

			    // Change the ODD
			    if (g_D_key_pressed || g_1_key_pressed)
			    {
			        // Get the current ODD
			        float odd = getODD("mm");
			        float step = 0.05F * odd;

			        // Increase the ODD
			        if ((g_D_key_pressed && (g_up_arrow_key_pressed || g_left_arrow_key_pressed)) ||
			            (g_1_key_pressed && g_left_arrow_key_pressed))
			        {
			            updateODD(step);
			        }
			        // Decrease the ODD
			        else if ((g_D_key_pressed && (g_down_arrow_key_pressed || g_right_arrow_key_pressed)) ||
			            (g_1_key_pressed && g_right_arrow_key_pressed))
			        {
			            updateODD(-step);
			        }
			    }

			    // Move sample
			    if (g_2_key_pressed)
			    {
			        // Get the current SOD
			        float sod = getSOD("mm");
			        float step = 0.05F * sod;
			        float step_x = 0.0F;
			        float step_y = 0.0F;
			        bool need_update = false;

			        if (g_up_arrow_key_pressed)
			        {
			            step_y = -step;
			            need_update = true;
			        }
			        else if (g_down_arrow_key_pressed)
			        {
			            step_y = step;
			            need_update = true;
			        }

			        if (g_left_arrow_key_pressed)
			        {
			            step_x = step;
			            need_update = true;
			        }
			        else if (g_right_arrow_key_pressed)
			        {
			            step_x = -step;
			            need_update = true;
			        }

			        if (need_update)
			        {
			            updateSampleTranslation(step_x, step_y, 0.0F);
			        }
			    }

			    // Move sample
			    if (g_3_key_pressed)
			    {
			        // Get the current SOD
			        float sod = getSOD("mm");
			        float step = 0.01F * sod;
			        float step_z = 0.0F;
			        bool need_update = false;

			        if (g_up_arrow_key_pressed)
			        {
			            step_z = -step;
			            need_update = true;
			        }
			        else if (g_down_arrow_key_pressed)
			        {
			            step_z = step;
			            need_update = true;
			        }

			        if (need_update)
			        {
			            updateSampleTranslation(0.0F, 0.0F, step_z);
			        }
			    }

			    // Rotate/tilt sample
			    if (g_4_key_pressed)
			    {
			        // Get the current SOD
			        float step = 5.0F;
			        float angle = 0.0F;
			        bool need_update = false;
			        gVirtualXRay::VEC3 rotation_axis(1.0, 0.0, 0.0);

			        if (g_left_arrow_key_pressed)
			        {
			            angle = step;
			            rotation_axis = g_xray_detector.getUpVector();
			            need_update = true;
			        }
			        else if (g_right_arrow_key_pressed)
			        {
			            angle = -step;
			            rotation_axis = g_xray_detector.getUpVector();
			            need_update = true;
			        }
			        else if (g_up_arrow_key_pressed)
			        {
			            angle = step;
			            rotation_axis = (g_camera_lookat - g_camera_position).normal();
			            need_update = true;
			        }
			        else if (g_down_arrow_key_pressed)
			        {
			            angle = -step;
			            rotation_axis = (g_camera_lookat - g_camera_position).normal();
			            need_update = true;
			        }

			        if (need_update)
			        {
			            gVirtualXRay::MATRIX4 rotation_matrix = gVirtualXRay::MATRIX4::buildRotationMatrix(angle,
			                rotation_axis);

			            if (updateRotation(rotation_matrix))
			            {
			                requestXRayImageUpdate();
			            }
			        }
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


//----------------------------------------------------------------------------------------------------------------
void mouseButtonCallback(int aButton, int aButtonState, int aModifierKey, double xpos, double ypos, int aWindowID)
//----------------------------------------------------------------------------------------------------------------
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

				g_button = aButton;
				g_button_state = aButtonState;

				// Start panning the view
				if (g_button == MOUSE_BUTTON_MIDDLE && g_button_state == PRESS)
				{
					g_last_x_position = static_cast<int>(xpos);
					g_last_y_position = static_cast<int>(ypos);

					g_current_x_position = static_cast<int>(xpos);
					g_current_y_position = static_cast<int>(ypos);
				}
				// Not start panning the view, maybe use the arc ball
				else
				{
					// Use the arc ball
					if (g_button_state == PRESS && !g_S_key_pressed && !g_D_key_pressed)
					{
						startArcBallRotation(xpos, ypos);
					}
					// Stop using the arc ball
					else
					{
						stopArcBallRotation();
					}
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
				auto [viewport_width, viewport_height] = search_window->second->getWindowSize();

				// Compute the rotation
				udapteCursorPositionStatus(x, y, viewport_width, viewport_height);
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


//--------------------------------------------------------------------------------------
void udapteCursorPositionStatus(double x, double y, int aViewportWidth, int aViewportHeight)
//--------------------------------------------------------------------------------------
{
    g_current_x_position = static_cast<int>(x);
    g_current_y_position = static_cast<int>(y);

    // Rotate the view
    if (!g_S_key_pressed && !g_D_key_pressed)
    {
        // Rotate the scene
        if (g_button == MOUSE_BUTTON_LEFT && g_button_state == PRESS)
        {
            // Project the previous and current mouse positions onto the
            // virtual arcball sphere. The resulting vectors are unit vectors
            // expressed in camera/screen space. The camera turns about the
            // look-at point, which projects onto the centre of the viewport,
            // so that is where the sphere is centred
            auto centre_x = 0.5F * static_cast<float>(aViewportWidth);
            auto centre_y = 0.5F * static_cast<float>(aViewportHeight);
            auto radius = 0.5F * static_cast<float>(
                std::min(aViewportWidth, aViewportHeight));

            gVirtualXRay::VEC3 v1 = getArcballVector(g_last_x_position, g_last_y_position, centre_x, centre_y, radius);
            gVirtualXRay::VEC3 v2 = getArcballVector(g_current_x_position, g_current_y_position, centre_x, centre_y, radius);

            // Compute the arcball rotation in camera space.
            // The rotation axis is perpendicular to the plane formed by the
            // two sphere vectors. The cross-product order is important:
            // v2 ^ v1 gives the rotation that maps v1 onto v2.
            gVirtualXRay::VEC3 camera_axis = v2 ^ v1;

            // The angle between the two sphere vectors corresponds to the
            // amount of mouse drag on the virtual sphere.
            float angle = static_cast<float>(acos(std::ranges::clamp(v1.dotProduct(v2), -1.0, 1.0)));

            // Build the current camera basis vectors.
            // These vectors are used to transform the arcball rotation axis
            // from camera coordinates into world coordinates.

            // viewing direction
            gVirtualXRay::VEC3 forward = (g_camera_lookat - g_camera_position).normal();

            // horizontal direction in the camera frame
            gVirtualXRay::VEC3 right = forward ^ g_camera_up;

            // vertical direction in the camera frame
            gVirtualXRay::VEC3 up    = right ^ forward;

            // Convert the rotation axis from camera space into world space.
            // camera_axis.x = rotation around the camera right axis
            // camera_axis.y = rotation around the camera up axis
            // camera_axis.z = rotation around the camera forward axis
            gVirtualXRay::VEC3 world_axis =
                right * camera_axis[0] +
                up * camera_axis[1] -
                forward * camera_axis[2];

            // Avoid a division by zero
            if (world_axis.length() > EPSILON)
            {
                // Normalise the vector
                world_axis.normalize();

                // Construct the world-space rotation corresponding to the
                // current arcball drag.
                gVirtualXRay::MATRIX4 rotation_matrix = gVirtualXRay::MATRIX4::buildRotationMatrix(
            gVirtualXRay::radianToDegree(ROTATION_SPEED * angle),
                    world_axis);

                // The camera position is represented as an offset from the
                // look-at point. Rotating this offset moves the camera around
                // the target while keeping the target fixed.
                gVirtualXRay::VEC3 offset = g_camera_position - g_camera_lookat;
                offset      = rotation_matrix * offset;

                // Rotate the camera up vector by the same amount so that the
                // camera orientation remains consistent throughout the orbit.
                g_camera_up = rotation_matrix * g_camera_up;

                // Reconstruct the camera position from the rotated offset.
                g_camera_position = g_camera_lookat + offset;

                // Update the view matrix using the new camera frame.
                gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
                                                        g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
                                                        g_camera_up[0], g_camera_up[1], g_camera_up[2]);
            }
        }
        // Pan the view
        else if (g_button == MOUSE_BUTTON_MIDDLE && g_button_state == PRESS)
        {
            // The viewport must have a surface for the drag to be meaningful
            if (aViewportHeight > 0 && aViewportWidth > 0)
            {
                // Build an orthonormal camera basis. The up vector is carried
                // across frames and rotated by the arcball, so it may have
                // drifted away from the perpendicular to the viewing direction
                gVirtualXRay::VEC3 forward = (g_camera_lookat - g_camera_position);
                auto distance = static_cast<float>(forward.length());
                forward.normalise();

                gVirtualXRay::VEC3 right = forward ^ g_camera_up;
                right.normalise();

                gVirtualXRay::VEC3 up = right ^ forward;
                up.normalise();

                // Panning slides the camera along the plane that passes through
                // the look-at point and faces the camera. Converting the drag
                // from pixels to world units on that plane is what keeps the
                // grabbed point under the cursor: the plane spans
                // 2 * distance * tan(fovy / 2) world units over the height of
                // the viewport. The same ratio applies horizontally because the
                // pixels are square, the wider field of view along the X-axis
                // being exactly compensated by the larger number of columns
                float world_units_per_pixel = static_cast<float>(
                    2.0 * distance * tan(gVirtualXRay::degreeToRadian(g_initial_fovy) / 2.0) /
                    static_cast<double>(aViewportHeight));

                auto offset_x = static_cast<float>(g_current_x_position - g_last_x_position) * world_units_per_pixel;
                auto offset_y = static_cast<float>(g_current_y_position - g_last_y_position) * world_units_per_pixel;

                // Dragging to the right moves the camera to the left, which is
                // what makes the scene follow the cursor. The Y-axis of the
                // window points downwards, so dragging downwards moves the
                // camera upwards
                g_camera_position -= right * offset_x;
                g_camera_lookat -= right * offset_x;

                g_camera_position += up * offset_y;
                g_camera_lookat += up * offset_y;

                gVirtualXRay::loadLookAtModelViewMatrix(g_camera_position[0], g_camera_position[1], g_camera_position[2],
                                                        g_camera_lookat[0], g_camera_lookat[1], g_camera_lookat[2],
                                                        g_camera_up[0], g_camera_up[1], g_camera_up[2]);
            }
        }
        // Rotate the sample
        else if (g_button == MOUSE_BUTTON_RIGHT && g_button_state == PRESS)
        {
            gVirtualXRay::MATRIX4 rotation_matrix;
            computeRotation(rotation_matrix, aViewportWidth, aViewportHeight);

            updateRotation(rotation_matrix);
        }
    }
    // Update the SOD or ODD
    else
    {
        if (g_last_x_position != 0 || g_last_y_position != 0 || g_current_x_position != 0 || g_current_y_position != 0)
        {
            auto offset_x = static_cast<float>(g_current_x_position - g_last_x_position);
            auto offset_y = static_cast<float>(g_current_y_position - g_last_y_position);
            float offset = 0.0F;

            if (abs(offset_x) > abs(offset_y))
                offset = offset_x;
            else
                offset = offset_y;

            if (g_S_key_pressed)
            {
                // Get the current SOD
                float sod = getSOD("mm");
                float step = 0.005F * sod * offset;
                updateSOD(step);
            }
            else if (g_D_key_pressed)
            {
                // Get the current ODD
                float odd = getODD("mm");
                float step = 0.005F * odd * offset;
                updateODD(step);
            }
        }
    }
    g_last_x_position = g_current_x_position;
    g_last_y_position = g_current_y_position;
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
            // Forward vector: centre - eye
            gVirtualXRay::VEC3 forward(g_camera_lookat - g_camera_position);
            forward.normalize();

            // Side vector: forward x up
            gVirtualXRay::VEC3 side(forward ^ g_camera_up);
            side.normalize();

            // New up vector: side x forward
            gVirtualXRay::VEC3 up2(side ^ forward);
            up2.normalize();

            // The virtual sphere is centred on the point the sample turns
            // about, not on the centre of the viewport. Were it centred on
            // the viewport, grabbing a sample that has been translated
            // sideways would land close to the silhouette of the sphere,
            // where a vertical drag mostly rolls the sample about the viewing
            // axis rather than tilting it, whereas a horizontal drag still
            // behaves as expected. If the point cannot be projected, e.g. it
            // is behind the camera, the centre of the viewport is used
            auto centre_x = 0.5F * static_cast<float>(aViewportWidth);
            auto centre_y = 0.5F * static_cast<float>(aViewportHeight);

            // Same expression as the one used to build the pivot of the
            // rotation in udapteCursorPositionStatus: the centre of the sphere
            // and the centre of the rotation must always match
            std::vector<double> bbox_centre_global =
                getNodeAndChildrenBoundingBoxCentre("root", "mm");

            float unit_of_length = static_cast<float>(getUnitOfLength("mm"));

            projectToWindow(gVirtualXRay::VEC3(bbox_centre_global[0],
                bbox_centre_global[1],
                bbox_centre_global[2]) /
                unit_of_length,
                side,
                up2,
                forward,
                aViewportWidth,
                aViewportHeight,
                centre_x,
                centre_y);

            // Radius of the sphere, in pixels
            auto radius = 0.5F * static_cast<float>(
                std::min(aViewportWidth, aViewportHeight));

            gVirtualXRay::Vec3<GLfloat> va(
                getArcballVector(g_last_x_position,
                                 g_last_y_position,
                                 centre_x,
                                 centre_y,
                                 radius));

            gVirtualXRay::Vec3<GLfloat> vb(
                getArcballVector(g_current_x_position,
                                 g_current_y_position,
                                 centre_x,
                                 centre_y,
                                 radius));

            // The arcball rotation is expressed in eye (camera) space:
            // the axis is perpendicular to the plane formed by the two
            // vectors projected onto the virtual sphere.
            gVirtualXRay::Vec3 < GLfloat > axis_in_camera_coord(va ^ vb);

            const double sin_angle = axis_in_camera_coord.length();
            const double cos_angle = va.dotProduct(vb);

            // The drag is too small, the rotation axis is undefined
            if (sin_angle < EPSILON)
            {
                return;
            }

            // atan2 is well conditioned over the whole range, unlike acos,
            // which loses precision for small drags (cos_angle close to 1)
            float angle = ROTATION_SPEED *
                radian2degree(static_cast<float>(atan2(sin_angle, cos_angle)));

            axis_in_camera_coord.normalise();

            // The sample transformation is applied in world space (the
            // modelview matrix is built as V * M, see
            // OpenGLWindow::render), whereas the arcball rotation is
            // expressed in eye space. The eye-space rotation must therefore be
            // conjugated by the view rotation:
            //     R_world = Vrot^-1 * R_eye * Vrot
            // Vrot is orthonormal, so Vrot^-1 is Vrot^T, and conjugating the
            // rotation is equivalent to expressing its axis in world
            // coordinates with Vrot^T. As the rows of Vrot are
            // (side, up2, -forward), its columns are the vectors below.
            gVirtualXRay::Vec3<GLfloat> axis_in_world_coord(
                side * axis_in_camera_coord.getX() +
                up2  * axis_in_camera_coord.getY() -
                forward * axis_in_camera_coord.getZ());

            axis_in_world_coord.normalise();

            gVirtualXRay::Matrix4x4<GLfloat> rotation_matrix;
            rotation_matrix.loadIdentity();

            rotation_matrix.rotate(angle, axis_in_world_coord);

            // Both matrices are now expressed in the world frame, so the new
            // rotation is applied after the accumulated one
            aRotationMatrix =
                rotation_matrix * aRotationMatrix;

            g_last_x_position = g_current_x_position;
            g_last_y_position = g_current_y_position;
        }
    }
}


//------------------------------------------------------------------------
bool projectToWindow(const gVirtualXRay::VEC3& aPointInWorldCoordinates,
					 const gVirtualXRay::VEC3& aSideVector,
					 const gVirtualXRay::VEC3& anUpVector,
					 const gVirtualXRay::VEC3& aForwardVector,
					 int aViewportWidth,
					 int aViewportHeight,
					 float& aWindowX,
					 float& aWindowY)
//------------------------------------------------------------------------
{
	// Position of the point with respect to the camera
	gVirtualXRay::VEC3 offset(aPointInWorldCoordinates - g_camera_position);

	// The camera looks down its own negative z axis, so the depth of a point
	// in front of it is positive
	auto depth = static_cast<float>(offset.dotProduct(aForwardVector));

	// The point is on the plane of the camera, or behind it, and cannot be
	// projected
	if (depth < EPSILON)
	{
		return (false);
	}

	auto x_in_camera_coord = static_cast<float>(offset.dotProduct(aSideVector));
	auto y_in_camera_coord = static_cast<float>(offset.dotProduct(anUpVector));

	// Apply the perspective projection to get normalised device coordinates
	auto half_height = static_cast<float>(tan(
		gVirtualXRay::degreeToRadian(g_initial_fovy) / 2.0));

	auto aspect_ratio = static_cast<float>(aViewportWidth) /
		static_cast<float>(aViewportHeight);

	float x_ndc = x_in_camera_coord / (depth * half_height * aspect_ratio);
	float y_ndc = y_in_camera_coord / (depth * half_height);

	// Convert to window coordinates, which increase downwards
	aWindowX = (x_ndc + 1.0F) * 0.5F * static_cast<float>(aViewportWidth);
	aWindowY = (1.0F - y_ndc) * 0.5F * static_cast<float>(aViewportHeight);

	return (true);
}


void startArcBallRotation(double x, double y) {
	g_last_x_position = static_cast<int>(x);
	g_last_y_position = static_cast<int>(y);

	g_current_x_position = static_cast<int>(x);
	g_current_y_position = static_cast<int>(y);

	g_use_arc_ball = true;
}


void stopArcBallRotation() {
	g_use_arc_ball = false;
	requestXRayImageUpdate();
}


void updateSampleTranslation(float aStepX, float aStepY, float aStepZ)
{
	double unit_of_length = getUnitOfLength("mm");
	gVirtualXRay::VEC3 source_position_in_mm = g_xray_detector.getXraySourceCentre() / unit_of_length;
	gVirtualXRay::VEC3 detector_position_in_mm = g_xray_detector.getDetectorPosition() / unit_of_length;

	gVirtualXRay::VEC3 translation_x_vector = detector_position_in_mm - source_position_in_mm;
	gVirtualXRay::VEC3 translation_y_vector = g_xray_detector.getRightVector();
	gVirtualXRay::VEC3 translation_z_vector = g_xray_detector.getUpVector();

	translation_x_vector.normalise();
	translation_y_vector.normalise();
	translation_z_vector.normalise();

	translation_x_vector *= aStepX;
	translation_y_vector *= aStepY;
	translation_z_vector *= aStepZ;

	gVirtualXRay::VEC3 translation_vector = translation_x_vector + translation_y_vector + translation_z_vector;
	gVirtualXRay::MATRIX4 translation_matrix = gVirtualXRay::MATRIX4::buildTranslationMatrix(translation_vector * unit_of_length);

	if (g_p_scene_graph) {
		gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode("root");

		const gVirtualXRay::MATRIX4 old_transformation_matrix = p_node->getLocalTransformationMatrix();

		p_node->setLocalTransformation(translation_matrix * old_transformation_matrix);

		if (!sampleCrashedIntoDetector() && !sampleCrashedIntoSource())
		{
			requestXRayImageUpdate();
		}
		else
		{
			p_node->setLocalTransformation(old_transformation_matrix);
		}
	}
}