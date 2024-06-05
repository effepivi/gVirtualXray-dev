#ifndef __SGVXR_Utils_h
#define __SGVXR_Utils_h


#include <string>
#include <vector>


//------------------------------------------------------------------------------
/// Convert an angle given in radians into degrees.
/**
 *  @param anAngle:   the angle in radians
 *  @return the corresponding angle in degrees
 */
//------------------------------------------------------------------------------
float radian2degree(float anAngle);


//------------------------------------------------------------------------------
/// Accessor on the numerical value corresponding to a unit of energy.
/**
 *  @param aUnitOfEnergy:   the unit of energy.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @return the corresponding numerical value
 */
//------------------------------------------------------------------------------
double getUnitOfEnergy(const std::string& aUnitOfEnergy);


//------------------------------------------------------------------------------
/// Accessor on the numerical value corresponding to a unit of length.
/**
 *  @param aUnitOfLength:   the unit of length. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *  @return the corresponding numerical value
 */
//------------------------------------------------------------------------------
double getUnitOfLength(const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Accessor on the numerical value corresponding to a unit of electric potential.
/**
 *  @param aUnitOfElectriPotential:   the unit of electric potential.
 *                                    Acceptable values are:
 *                                    "V", "volt",
 *                                    "kV", "kilovolt",
 *                                    "MV", "megavolt"
 *  @return the corresponding numerical value
 */
//------------------------------------------------------------------------------
double getUnitOfElectriPotential(const std::string& aUnitOfElectriPotential);


//------------------------------------------------------------------------------
/// Accessor on the number of EGL devices
/*
*  @return on the number of EGL devices
*/
//------------------------------------------------------------------------------
unsigned int getNumberOfEGLDevices();


//------------------------------------------------------------------------------
/// Accessor on the list of supported EGL clients
/*
*  @return on the list of supported EGL clients
*/
//------------------------------------------------------------------------------
std::vector<std::string> findEGLClients();


//------------------------------------------------------------------------------
/// Create an OpenGL context (the window won't be shown).
/**
 *  @param aWindowID:   the numerical ID of the context to create
 *                      (default value: -1, means that the ID will be
 *                      automatically generated)
 *  @param aRendererMajorVersion: Select the major version of the renderer.
 *                    (default value: 3)
 *  @param aRendererMajorVersion: Select the minor version of the renderer.
 *                    (default value: 2)
 */
//------------------------------------------------------------------------------
void createOpenGLContext(int aWindowID = -1,
		int aRendererMajorVersion = 3,
		int aRendererMinorVersion = 2);


//------------------------------------------------------------------------------
/// Create an OpenGL context and display it in a window
/**
 *  @param aWindowID:   the numerical ID of the context to create
 *                      (default value: -1, means that the ID will be
 *                      automatically generated)
 *  @param aRenderer: Select the renderer to use, e.g. OpenGL or Vulkan.
 *                    (default value: OPENGL)
 *  @param aRendererMajorVersion: Select the major version of the renderer.
 *                    (default value: 3)
 *  @param aRendererMajorVersion: Select the minor version of the renderer.
 *                    (default value: 2)
 *  @param aVisibilityFlag: flag controling if the window should be visible (1)
 *                          or hidden (0). (default value: 0)
 */
//------------------------------------------------------------------------------
#ifndef __APPLE__
void createWindow(int aWindowID = -1,
		int aVisibilityFlag = 0,
		const std::string& aRenderer = "OPENGL",
		int aRendererMajorVersion = 3,
		int aRendererMinorVersion = 2);
#else // __APPLE__
void createWindow(int aWindowID = -1,
		int aVisibilityFlag = 0,
		const std::string& aRenderer = "OPENGL",
		int aRendererMajorVersion = 2,
		int aRendererMinorVersion = 1);
#endif // __APPLE__


//------------------------------------------------------------------------------
/// Create an OpenGL context
/**
 *  @param aRenderer: Select the renderer to use, e.g. OpenGL or Vulkan.
 *                    (default value: OPENGL)
 *  @param aWindowID:   the numerical ID of the context to create
 *                      (default value: -1, means that the ID will be
 *                      automatically generated)
 *  @param aRendererMajorVersion: Select the major version of the renderer.
 *                    (default value: 3)
 *  @param aRendererMajorVersion: Select the minor version of the renderer.
 *                    (default value: 2)
 *  @param aVisibilityFlag: flag controling if the window should be visible (1)
 *                          or hidden (0). (default value: 0)
 */
//------------------------------------------------------------------------------
void createNewContext(const std::string& aRenderer = "OPENGL",
    int aWindowID = -1,
		int aVisibilityFlag = 0,
		int aRendererMajorVersion = 3,
		int aRendererMinorVersion = 2);


//------------------------------------------------------------------------------
/// Set the size of the given window.
/**
 *  @param aWidth:      the number of pixels along the X-axis
 *  @param aHeight:     the number of pixels along the Y-axis
 *  @param aWindowID:   the numerical ID of the corresponding context
 *                      (default value: -1, means that the active context
 *                      will be used)
 */
//------------------------------------------------------------------------------
void setWindowSize(int aWidth, int aHeight, int aWindowID = -1);


//------------------------------------------------------------------------------
/// Accessor on the size of a given window.
/**
 *  @return the size of the window
 */
//------------------------------------------------------------------------------
std::vector<int> getWindowSize(int aWindowID = -1);


#endif
