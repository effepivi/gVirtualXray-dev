/*

Copyright (c) 2018-2023, Dr Franck P. Vidal, Bangor University, All rights reserved.
Copyright (c) 2023-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
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


#ifndef __SimpleGVXR_h
#define __SimpleGVXR_h


/**
********************************************************************************
*
*   @file       SimpleGVXR.h
*
*   @brief      SimpleGVXR is an API to the bulk functionalities and classes of
*               gVirtualXRay. SimpleGVXR is used as the basis for
*               the language bindings generated using SWIG.
*
*   @version    1.0
*
*   @date       02/05/2024
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
*   (c) by Dr Franck P. Vidal, Bangor University, Apr 2019-Sept 2023, version 1.1, BSD 3-Clause License
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Oct 2023-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <vector>
#include <utility>
#include <string>

#include "acquisition.h"
#include "detector.h"
#include "ImplicitSurface.h"
#include "sample.h"
#include "source.h"
#include "Utils.h"
#include "visualisation.h"

//------------------------------------------------------------------------------
/// Accessor on the major version of the core gVirtualXRay library (gvxr).
/// This number is changed when incompatible API changes have been made.
/**
 *  @return the major version number
 */
//------------------------------------------------------------------------------
int getMajorVersionOfCoreGVXR();


//------------------------------------------------------------------------------
/// Accessor on the minor version of the core gVirtualXRay library (gvxr).
/// This number is changed when new functionalities have been added in
/// a backwards-compatible manner.
/**
 *  @return the minor version number
 */
//------------------------------------------------------------------------------
int getMinorVersionOfCoreGVXR();


//------------------------------------------------------------------------------
/// Accessor on the minor version of the core gVirtualXRay library (gvxr).
/// This number is changed when bug fixes have been added in
/// a backwards-compatible manner.
/**
 *  @return the patch version number
 */
//------------------------------------------------------------------------------
int getPatchVersionOfCoreGVXR();


//------------------------------------------------------------------------------
/// Accessor on the full string version of the core gVirtualXRay library (gvxr).
/**
 *  @return the full version number
 */
//------------------------------------------------------------------------------
std::string getVersionOfCoreGVXR();


//------------------------------------------------------------------------------
/// Accessor on the major version of SimpleGVXR. This number is changed
/// when incompatible API changes have been made.
/**
 *  @return the major version number
 */
//------------------------------------------------------------------------------
int getMajorVersionOfSimpleGVXR();


//------------------------------------------------------------------------------
/// Accessor on the minor version of SimpleGVXR. This number is changed when
/// new functionalities have been added in a backwards-compatible manner.
/**
 *  @return the minor version number
 */
//------------------------------------------------------------------------------
int getMinorVersionOfSimpleGVXR();


//------------------------------------------------------------------------------
/// Accessor on the minor version of SimpleGVXR. This number is changed when
/// bug fixes have been added in a backwards-compatible manner.
/**
 *  @return the patch version number
 */
//------------------------------------------------------------------------------
int getPatchVersionOfSimpleGVXR();


//------------------------------------------------------------------------------
/// Accessor on the full string version of SimpleGVXR.
/**
 *  @return the full version number
 */
//------------------------------------------------------------------------------
std::string getVersionOfSimpleGVXR();


//------------------------------------------------------------------------------
/// Accessor on the OpenGL renderer.
/**
 *  @return a string displaying the OpenGL renderer
 */
//------------------------------------------------------------------------------
std::string getOpenGlRenderer();


//------------------------------------------------------------------------------
/// Accessor on the OpenGL version.
/**
 *  @return a string displaying the OpenGL version
 */
//------------------------------------------------------------------------------
std::string getOpenGlVersion();


//------------------------------------------------------------------------------
/// Accessor on the OpenGL vendor.
/**
 *  @return a string displaying the OpenGL vendor
 */
//------------------------------------------------------------------------------
std::string getOpenGlVendor();


//------------------------------------------------------------------------------
/// Turn on/off the use of a log file.
/**
 *  @param aFileName: the name of the file in which the output will be saved. 
 *                    If it is equal to 0, NULL, or nullptr, the standard output,
 *                    will be used (default value: "gvxr.log")
 */
//------------------------------------------------------------------------------
void useLogFile(const char* aFileName = "gvxr.log");


//------------------------------------------------------------------------------
/// Set the seed of the random number generator.
/**
 *  @param aRandomGeneratorSeed:    the seed
 */
//------------------------------------------------------------------------------
void setSeed(unsigned int aRandomGeneratorSeed);






//------------------------------------------------------------------------------
/// Create an OpenGL context automatically.
/**
 *  @param aFlag:   true to create the OpenGL context automatically,
 *                  false otherwise (default value: true)
 */
//------------------------------------------------------------------------------
void autoCreateOpenGLContext(bool aFlag = true);







//------------------------------------------------------------------------------
/// Return a 4x4 identity matrix.
/**
 *  @return a 4x4 identity matrix
 */
//------------------------------------------------------------------------------
std::vector<double> getIdentityMatrix4();





//------------------------------------------------------------------------------
/// Display the scenegraph node using OpenGL.
/**
 *  @param aLabel:          the label of the polygon mesh
 */
//------------------------------------------------------------------------------
void displayNode(const std::string& aLabel);




//------------------------------------------------------------------------------
/// Move the scenegraph to the centre.
//------------------------------------------------------------------------------
void moveToCenter();


//------------------------------------------------------------------------------
/// Move the scenegraph to the centre.
//------------------------------------------------------------------------------
void moveToCentre();


//------------------------------------------------------------------------------
/// Move a polygon mesh to the centre.
/**
 *  @param aLabel:  the label of the polygon mesh in the scenegraph
 */
//------------------------------------------------------------------------------
void moveToCenter(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Move a polygon mesh to the centre.
/**
 *  @param aLabel:  the label of the polygon mesh in the scenegraph
 */
//------------------------------------------------------------------------------
void moveToCentre(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Scale a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh to transform
 *  @param x:   the scaling factor along the X-axis
 *  @param y:   the scaling factor along the Y-axis
 *  @param z:   the scaling factor along the Z-axis
 */
//------------------------------------------------------------------------------
void scaleNode(const std::string& aLabel,
    double x,
    double y,
    double z);


//------------------------------------------------------------------------------
/// Rotate a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh to transform
 *  @param anAngle:   the rotation angle in degrees
 *  @param x:   the component of the rotation vector along the X-axis
 *  @param y:   the component of the rotation vector along the Y-axis
 *  @param z:   the component of the rotation vector along the Z-axis
 */
//------------------------------------------------------------------------------
void rotateNode(const std::string& aLabel,
    double anAngle,
    double x,
    double y,
    double z);


//------------------------------------------------------------------------------
/// Rotate a polygon mesh using the yaw, pitch, roll method
/**
 *  @param aLabel:  the label of the polygon mesh to transform
 *  @param yaw: a counterclockwise rotation of yaw degrees about the z-axis
 *  @param pitch: a counterclockwise rotation of yaw degrees about the y-axis
 *  @param roll: a counterclockwise rotation of yaw degrees about the x-axis
 */
//------------------------------------------------------------------------------
void rotateNode(const std::string& aLabel,
    double yaw, double pitch, double roll);


//------------------------------------------------------------------------------
/// Translate a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh to transform
 *  @param x:   the component of the translation vector along the X-axis
 *  @param y:   the component of the translation vector along the Y-axis
 *  @param z:   the component of the translation vector along the Z-axis
 *  @param aUnitOfLength:   the unit of length corresponding to the x, y and z
 *                          parameters. Acceptable values are:
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
void translateNode(const std::string& aLabel,
    double x,
    double y,
    double z,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Use a shear matrix  to transform the node.
/**
 *  @param aLabel:  the label of the polygon mesh to transform
 *  @param yx:   the yx component of the shear matrix
 *  @param zx:   the zx component of the shear matrix
 *  @param xy:   the xy component of the shear matrix
 *  @param zy:   the zy component of the shear matrix
 *  @param xz:   the xz component of the shear matrix
 *  @param yz:   the yz component of the shear matrix
 */
//------------------------------------------------------------------------------
void shearNode(const std::string& aLabel,
               double yx,
               double zx,
               double xy,
               double zy,
               double xz,
               double yz);


//------------------------------------------------------------------------------
/// Replace the 3-D transformation matrix of the overall 3-D scene by
/// an identity matrix.
//------------------------------------------------------------------------------
void resetSceneTransformation();


//------------------------------------------------------------------------------
/// Scale the 3-D scene
/// (all the polygon meshes will be affected but not modified).
/**
 *  @param x:   the scaling factor along the X-axis
 *  @param y:   the scaling factor along the Y-axis
 *  @param z:   the scaling factor along the Z-axis
 */
//------------------------------------------------------------------------------
void scaleScene(double x, double y, double z);


//------------------------------------------------------------------------------
/// Scale the 3-D scene
/// (all the polygon meshes will be affected but not modified).
/**
 *  @param x:   the scaling factor along the X-axis
 *  @param y:   the scaling factor along the Y-axis
 *  @param z:   the scaling factor along the Z-axis
 *  @param aUnitOfLength:   the unit of length corresponding to the x, y and z
 *                          parameters. Acceptable values are:
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
void scaleScene(double x, double y, double z, const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Rotate the 3-D scene
/// (all the polygon meshes will be affected but not modified).
/**
 *  @param anAngle:   the rotation angle in degrees
 *  @param x:   the component of the rotation vector along the X-axis
 *  @param y:   the component of the rotation vector along the Y-axis
 *  @param z:   the component of the rotation vector along the Z-axis
 */
//------------------------------------------------------------------------------
void rotateScene(double anAngle, double x, double y, double z);


//------------------------------------------------------------------------------
/// Rotate the 3-D scene using the yaw, pitch, roll method
/// (all the polygon meshes will be affected but not modified).
/**
 *  @param yaw: a counterclockwise rotation of yaw degrees about the z-axis
 *  @param pitch: a counterclockwise rotation of yaw degrees about the y-axis
 *  @param roll: a counterclockwise rotation of yaw degrees about the x-axis
 */
//------------------------------------------------------------------------------
void rotateScene(double yaw, double pitch, double roll);


//------------------------------------------------------------------------------
/// Translate the 3-D scene
/// (all the polygon meshes will be affected but not modified).
/**
 *  @param x:   the component of the translation vector along the X-axis
 *  @param y:   the component of the translation vector along the Y-axis
 *  @param z:   the component of the translation vector along the Z-axis
 *  @param aUnitOfLength:   the unit of length corresponding to the x, y and z
 *                          parameters. Acceptable values are:
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
void translateScene(double x,
                    double y,
                    double z,
                    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Accessor on the 3-D scene transformation matrix.
/**
 *  @return the 3-D scene transformation matrix as a 4x4 array
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > getSceneTransformationMatrix();


//------------------------------------------------------------------------------
/// Accessor on the transformation matrix of the scengraph's root node.
/**
 *  @return the scengraph's root node transformation matrix as a 4x4 array
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > getRootTransformationMatrix();


//------------------------------------------------------------------------------
/// Accessor on the transformation matrix of a given node.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @return the scengraph's root node transformation matrix as a 4x4 array
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > getNodeLocalTransformationMatrix(
    const std::string& aLabel);


//------------------------------------------------------------------------------
/// Accessor on the transformation matrix of a given node.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @return the scengraph's root node transformation matrix as a 4x4 array
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > getNodeWorldTransformationMatrix(
    const std::string& aLabel);


//------------------------------------------------------------------------------
/// Set the 3-D scene transformation matrix.
/**
 *  @param aMatrix: the transformation matrix as a 4x4 array
 */
//------------------------------------------------------------------------------
void setSceneTransformationMatrix(
    const std::vector<std::vector<float> >& aMatrix);


//------------------------------------------------------------------------------
/// Set the transformation matrix of the scengraph's root node.
/**
 *  @param aMatrix: the transformation matrix as a 4x4 array
 */
//------------------------------------------------------------------------------
void setRootTransformationMatrix(
    const std::vector<std::vector<float> >& aMatrix);


//------------------------------------------------------------------------------
/// Set the transformation matrix of a given node.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param aMatrix: the transformation matrix as a 4x4 array
 */
//------------------------------------------------------------------------------
void setNodeTransformationMatrix(
    const std::string& aLabel,
    const std::vector<std::vector<float> >& aMatrix);


//------------------------------------------------------------------------------
/// Set the Hounsfield value corresponding to the material properties of
/// a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param HU:      the Hounsfield value
 */
//------------------------------------------------------------------------------
void setHU(const std::string& aLabel, short HU);


//------------------------------------------------------------------------------
/// Set the Hounsfield value corresponding to the material properties of
/// a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param HU:      the Hounsfield value
 */
//------------------------------------------------------------------------------
void setHounsfieldUnit(const std::string& aLabel, short HU);


//------------------------------------------------------------------------------
/// Set the Hounsfield value corresponding to the material properties of
/// a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param HU:      the Hounsfield value
 */
//------------------------------------------------------------------------------
void setHounsfieldValue(const std::string& aLabel, short HU);


//------------------------------------------------------------------------------
/// Set the chemical element (or element) corresponding
/// to the material properties of a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param Z:       the atomic number (or Z number) corresponding to the element
 */
//------------------------------------------------------------------------------
void setElement(const std::string& aLabel, unsigned short Z);


//------------------------------------------------------------------------------
/// Set the chemical element (or element) corresponding
/// to the material properties of a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param aName:   the symbol or name corresponding to the element
 */
//------------------------------------------------------------------------------
void setElement(const std::string& aLabel, const std::string& aName);


//------------------------------------------------------------------------------
/// Set the mixture corresponding to the material properties of a polygon mesh.
/// Don't forget to set the density of the material.
/**
 *  @param aLabel:      the label of the polygon mesh
 *  @param aMixture:    the details about the mixture. It is given as a sequence
 *                      of element symbol & relative weight, e.g.
 *                      Ti90Al6V4.
 */
//------------------------------------------------------------------------------
void setMixture(const std::string& aLabel, const std::string& aMixture);


//------------------------------------------------------------------------------
/// Set the mixture corresponding to the material properties of a polygon mesh.
/// Don't forget to set the density of the material.
/**
 *  @param aLabel:      the label of the polygon mesh
 *  @param aZNumberSet:      the array of Z numbers
 *  @param aWeightSet:      the corresponding weights
 */
//------------------------------------------------------------------------------
void setMixture(const std::string& aLabel,
                const std::vector<int>& aZNumberSet,
                const std::vector<double>& aWeightSet);


//------------------------------------------------------------------------------
/// Set the mixture corresponding to the material properties of a polygon mesh.
/// Don't forget to set the density of the material.
/**
 *  @param aLabel:      the label of the polygon mesh
 *  @param aSymbolSet:      the array of element symbols
 *  @param aWeightSet:      the corresponding weights
 */
//------------------------------------------------------------------------------
void setMixture(const std::string& aLabel,
                const std::vector<std::string>& aSymbolSet,
                const std::vector<double>& aWeightSet);


//------------------------------------------------------------------------------
/// Set the compound corresponding to the material properties of a polygon mesh.
/// Don't forget to set the density of the material.
/**
 *  @param aLabel:      the label of the polygon mesh
 *  @param aCompound:   the details about the compound. It is given as
 *                      a sequence of element symbol & number of atoms, e.g.
 *                      H2O for water and SiC for silicon carbide.
 */
//------------------------------------------------------------------------------
void setCompound(const std::string& aLabel, const std::string& aCompound);


//------------------------------------------------------------------------------
/// Set the mass attenuation coefficient corresponding to
/// the material properties of a polygon mesh.
/// Don't forget to set the density of the material.
/**
 *  @param aLabel:          the label of the polygon mesh
 *  @param aCoefficient:    the mass attenuation coefficient
 *  @param aUnit:           the unit corresponding to aCoefficient.
 *                          Acceptable values are:
 *                          "cm2/g" and "cm2.g-1"
 */
//------------------------------------------------------------------------------
void setMassAttenuationCoefficient(const std::string& aLabel,
                                   double aCoefficient,
                                   const std::string& aUnit);


//------------------------------------------------------------------------------
/// Set the linear attenuation coefficient corresponding
/// to the material properties of a polygon mesh.
/**
 *  @param aLabel:          the label of the polygon mesh
 *  @param aCoefficient:    the liner attenuation coefficient
 *  @param aUnit:           the unit corresponding to aCoefficient.
 *                          Acceptable values are:
 *                          "cm-1"
 */
//------------------------------------------------------------------------------
void setLinearAttenuationCoefficient(const std::string& aLabel,
                                     double aCoefficient,
                                     const std::string& aUnit);


//------------------------------------------------------------------------------
/// Set the linear attenuation coefficient corresponding
/// to the material properties of a polygon mesh.
/**
 *  @param aLabel:          the label of the polygon mesh
 *  @param aCoefficient:    the liner attenuation coefficient
 *  @param aUnit:           the unit corresponding to aCoefficient.
 *                          Acceptable values are:
 *                          "cm-1"
 */
//------------------------------------------------------------------------------
void setMu(const std::string& aLabel,
           double aCoefficient,
           const std::string& aUnit);


//------------------------------------------------------------------------------
/// Set the density corresponding to the material properties of a polygon mesh.
/**
 *  @param aLabel:      the label of the polygon mesh
 *  @param aDensity:    the density
 *  @param aUnit:       the unit corresponding to aDensity.
 *                      Acceptable values are:
 *                      "g/cm3", "g.cm-3", "mg/cm3", "mg.cm-3",
 *                      "kg/m3", and "kg.m-3".
 */
//------------------------------------------------------------------------------
void setDensity(const std::string& aLabel,
                double aDensity,
                const std::string& aUnit);




//------------------------------------------------------------------------------
/// Accessor on the density corresponding
/// to the material properties of a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @return the density of the polygon mesh's material in g/cm3
 */
//------------------------------------------------------------------------------
double getDensity(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Accessor on the mass attenuation coefficient at a given energy
/// corresponding to the material properties of a polygon mesh.
/**
 *  @param aLabel:          the label of the polygon mesh
 *  @param anEnergy:        the photon energy of interest
 *  @param aUnitOfEnergy:   the unit of energy corresponding to anEnergy.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @return the mass attenuation coefficient of the polygon mesh's material
 *          in cm2/g
 */
//------------------------------------------------------------------------------
double getMassAttenuationCoefficient(const std::string& aLabel,
                                     double anEnergy,
                                     const std::string& aUnitOfEnergy);


//------------------------------------------------------------------------------
/// Accessor on the linear attenuation coefficient at a given energy
/// corresponding to the material properties of a polygon mesh.
/**
 *  @param aLabel:          the label of the polygon mesh
 *  @param anEnergy:        the photon energy of interest
 *  @param aUnitOfEnergy:   the unit of energy corresponding to anEnergy.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @return the linear attenuation coefficient of the polygon mesh's material
 *          in cm-1
 */
//------------------------------------------------------------------------------
double getLinearAttenuationCoefficient(const std::string& aLabel,
                                       double anEnergy,
                                       const std::string& aUnitOfEnergy);


//------------------------------------------------------------------------------
/// Accessor on the material label of a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @return the label of the material
 */
//------------------------------------------------------------------------------
std::string getMaterialLabel(const std::string& aLabel);


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


//------------------------------------------------------------------------------
/// 3-D visualisation of the 3-D scene (source, detector, and scanned objects).
/// Note that there is no interactive loop running.
/**
 *  @param aSceneRotationFlag:  true if the 3-D scene has to be rotated with
 *                              the arc-ball method, false otherwise
 *                              (default value: true)
 *  @param aWindowID:   the numerical ID of the corresponding context
 *                      (default value: -1, means that the active context
 *                      will be used)
 */
//------------------------------------------------------------------------------
void displayScene(bool aSceneRotationFlag = true, int aWindowID = -1);


//------------------------------------------------------------------------------
/// 3-D visualisation of the 3-D scene (source, detector, and scanned objects).
/// Note that there is interactive loop running.
/// Keys are:
/// - Q/Escape: to quit the event loop (does not close the window)
/// - B: display/hide the X-ray beam
/// - W: display the polygon meshes in solid or wireframe
/// - N: display the X-ray image in negative or positive
/// - L: switch lighting on/off
/// - D: display/hide the X-ray detector
/// - V: display/hide normal vectors
/**
 *  @param aWindowID:   the numerical ID of the corresponding context
 *                      (default value: -1, means that the active context
 * will be used)
 */
//------------------------------------------------------------------------------
void renderLoop(int aWindowID = -1);


//------------------------------------------------------------------------------
/// Call this function when the user starts an arc-ball rotation
/// (e.g. left-mouse button click in the 3-D visualisation window).
/**
 *  @param x:   the cursor x-axis position in the viewport coordinate system
 *  @param y:   the cursor y-axis position in the viewport coordinate system
 */
//------------------------------------------------------------------------------
void startArcBallRotation(double x, double y);


//------------------------------------------------------------------------------
/// Call this function when the user stops an arc-ball rotation
/// (e.g. release the left-mouse button click in the 3-D visualisation window).
//------------------------------------------------------------------------------
void stopArcBallRotation();


//------------------------------------------------------------------------------
/// Check if arc-ball rotation is currently in use.
/**
 *  @return true if arc-ball rotation is currently in use, false otherwise
 */
//------------------------------------------------------------------------------
bool usingArcBallRotation();


//------------------------------------------------------------------------------
/// Call this function when the user moves the mouse
/**
 *  @param x:   the cursor x-axis position in the viewport coordinate system
 *  @param y:   the cursor y-axis position in the viewport coordinate system
 *  @param aViewportWidth:  the size of the viewport along the x-axis
 *  @param aViewportHeight: the size of the viewport along the y-axis
 */
//------------------------------------------------------------------------------
void cursorPositionCallback(double x,
                            double y,
                            int aViewportWidth,
                            int aViewportHeight);


//------------------------------------------------------------------------------
/// Call this function when the user scrolls
/**
 *  @param xoffset: the scroll offset along the x-axis (not used)
 *  @param yoffset: the scroll offset along the y-axis (used for zooming in
 *                  and out)
 */
//------------------------------------------------------------------------------
void scrollCallback(double xoffset, double yoffset);


void setZoom(float aZoomValue);


float getZoom();


void setSceneRotationMatrix(const std::vector<double>& aRotationMatrix);


std::vector<double> getSceneRotationMatrix();


//------------------------------------------------------------------------------
/// Make an OpenGL context visible and display a window.
/**
 *  @param aWindowID:   the numerical ID of the corresponding context
 *                      (default value: -1, means that the active context
 *                      will be used)
 */
//------------------------------------------------------------------------------
void showWindow(int aWindowID = -1);


//------------------------------------------------------------------------------
/// Hide a window.
/**
 *  @param aWindowID:   the numerical ID of the corresponding context
 *                      (default value: -1, means that the active context
 *                      will be used)
 */
//------------------------------------------------------------------------------
void hideWindow(int aWindowID = -1);


//------------------------------------------------------------------------------
/// Set window background colour.
/**
 *  @param R: the red colour [0, 1]
 *  @param G: the green colour [0, 1]
 *  @param B: the blue colour [0, 1]
 *  @param aWindowID:   the numerical ID of the corresponding context
 *                      (default value: -1, means that the active context
 *                      will be used)
 */
//------------------------------------------------------------------------------
void setWindowBackGroundColour(float R, float G, float B, int aWindowID = -1);


//------------------------------------------------------------------------------
/// Set window background colour.
/**
 *  @param R: the red colour [0, 1]
 *  @param G: the green colour [0, 1]
 *  @param B: the blue colour [0, 1]
 *  @param aWindowID:   the numerical ID of the corresponding context
 *                      (default value: -1, means that the active context
 *                      will be used)
 */
//------------------------------------------------------------------------------
void setWindowBackGroundColor(float R, float G, float B, int aWindowID = -1);


//------------------------------------------------------------------------------
/// Take screenshot.
/**
 *  @param aWindowID:   the numerical ID of the corresponding context
 *                      (default value: -1, means that the active context
 *                      will be used)
 *  @return:            the 2D image in RGB
 */
//------------------------------------------------------------------------------
const std::vector<std::vector<std::vector< float> > >& takeScreenshot(int aWindowID = -1);


//------------------------------------------------------------------------------
/// Get the latest screenshot.
/**
 *  @return:            the 2D image in RGB
 */
//------------------------------------------------------------------------------
const std::vector<std::vector<std::vector< float> > >& getLatestScreenshot();


//------------------------------------------------------------------------------
/// Close and destroy a given window or OpenGL context.
//------------------------------------------------------------------------------
void destroyWindow(int aWindowID = -1);


//------------------------------------------------------------------------------
/// Close and destroy all the windows and OpenGL contexts that have been created.
/// Same as void terminate(void);
//------------------------------------------------------------------------------
void destroyAllWindows(void);


//------------------------------------------------------------------------------
/// Close and destroy all the windows and OpenGL contexts that have been created.
/// Same as void destroyAllWindows(void);
//------------------------------------------------------------------------------
void terminate(void);




/*
*  @param anIntegrateEnergyFlag: if true the energy fluence is returned,
*                                otherwise the number of photons is returned
*                                (default value: true)
*/
std::vector<std::vector< float> > getWhiteImage(bool anIntegrateEnergyFlag = true);


std::vector<std::vector<std::vector< float> > > getWhiteSpectralImage();


//------------------------------------------------------------------------------
/// Save the polygon mesh of a given node.
/**
 *  @param aLabel:          the label of the polygon mesh
 *  @param aFileName:   the name of the output file
 *  (default: means that the filename will be
 *  automatically generated and the file saved in
 *  the current path)
 */
//------------------------------------------------------------------------------
void saveSTLfile(const std::string& aLabel,
		const std::string& aFileName = std::string());


//------------------------------------------------------------------------------
/// Save the last X-ray image that has been computed.
/**
 *  @param aFileName:   the name of the output file
 *  (default: means that the filename will be
 *  automatically generated and the file saved in
 *  the current path)
 *  @param useCompression:  use data compression is possible (default: true)
 */
//------------------------------------------------------------------------------
void saveLastXRayImage(const std::string& aFileName = std::string(),
    bool useCompression = true);


//------------------------------------------------------------------------------
/// Save the last L-buffer that has been computed.
/**
 *  @param aFileName:   the name of the output file
 *              (default: means that the filename will be
 *              automatically generated and the file saved in
 *              the current path)
 *  @param useCompression:  use data compression is possible (default: true)
 */
//------------------------------------------------------------------------------
void saveLastLBuffer(const std::string& aFileName = std::string(),
    bool useCompression = true);


//------------------------------------------------------------------------------
/// Save the last cumulated L-buffer that has been computed.
/**
 *  @param aFileName:   the name of the output file
 *              (default: means that the filename will be
 *              automatically generated and the file saved in
 *              the current path)
 *  @param useCompression:  use data compression is possible (default: true)
 */
//------------------------------------------------------------------------------
void saveLastCumulatedLBuffer(const std::string& aFileName = std::string(),
    bool useCompression = true);


    //------------------------------------------------------------------------------
    /// Save the last sinogram that has been computed.
    /**
     *  @param aFileName:   the name of the output file
     *                      (default: 0 means that the filename will be
     *                      automatically generated and the file saved in
     *                      the current path)
     *  @param useCompression:  use data compression is possible (default: true)
     */
    //------------------------------------------------------------------------------
    void saveLastSinogram(const std::string& aFileName = std::string(),
        bool useCompression = true);


//------------------------------------------------------------------------------
/// Save the last projection set that has been computed.
/**
 *  @param aFileName:   the name of the output file
 *                      (default: 0 means that the filename will be
 *                      automatically generated and the file saved in
 *                      the current path)
 *  @param useCompression:  use data compression is possible (default: true)
 */
//------------------------------------------------------------------------------
void saveLastProjectionSet(const std::string& aFileName = std::string(),
    bool useCompression = true);


//------------------------------------------------------------------------------
/// Enable artefact filtering on GPU (it is fast but can be inaccurate).
//------------------------------------------------------------------------------
void enableArtefactFilteringOnGPU(void);


//------------------------------------------------------------------------------
/// Enable artefact filtering on GPU (it is fast but can be inaccurate).
//------------------------------------------------------------------------------
void enableArtifactFilteringOnGPU(void);


//------------------------------------------------------------------------------
/// Enable artefact filtering on CPU (can be extremely slow as it makes use
/// of ray-tracing).
//------------------------------------------------------------------------------
void enableArtefactFilteringOnCPU(void);


//------------------------------------------------------------------------------
/// Enable artefact filtering on CPU (can be extremely slow as it makes use
/// of ray-tracing).
//------------------------------------------------------------------------------
void enableArtifactFilteringOnCPU(void);


//------------------------------------------------------------------------------
/// Disable artefact filtering altogether.
//------------------------------------------------------------------------------
void disableArtefactFiltering(void);


//------------------------------------------------------------------------------
/// Disable artefact filtering altogether.
//------------------------------------------------------------------------------
void disableArtifactFiltering(void);


//------------------------------------------------------------------------------
/// Accessor on the max radius of the L-buffer artefact correction filter
/**
 *  @return the radius in number of pixels
 */
//------------------------------------------------------------------------------
unsigned int getMaxRadiusLbufferArtefactFilter();


//------------------------------------------------------------------------------
/// Set the max radius of the L-buffer artefact correction filter
/**
 *  @param aRadius: the radius in number of pixels
 */
//------------------------------------------------------------------------------
void setMaxRadiusLbufferArtefactFilter(unsigned int aRadius);


//------------------------------------------------------------------------------
/// Check whether the GPU implementation of the L-buffer artefact correction filter
/// is enabled
/**
 *  @return true if the filter is active, otherwise return false
 */
//------------------------------------------------------------------------------
bool isArtefactFilteringOnGPUEnabled();


//------------------------------------------------------------------------------
/// Check whether the CPU implementation of the L-buffer artefact correction filter
/// is enabled
/**
 *  @return true if the filter is active, otherwise return false
 */
//------------------------------------------------------------------------------
bool isArtefactFilteringOnCPUEnabled();







//------------------------------------------------------------------------------
/// Accessor on the width of a given image.
/**
 *  @param anImage: the image
 *  @return the width of the image (in number of pixels)
 */
//------------------------------------------------------------------------------
int getImageWidth(const std::vector<std::vector<float> >& anImage);


//------------------------------------------------------------------------------
/// Accessor on the height of a given image.
/**
 *  @param anImage: the image
 *  @return the height of the image (in number of pixels)
 */
//------------------------------------------------------------------------------
int getImageHeght(const std::vector<std::vector<float> >& anImage);


//------------------------------------------------------------------------------
/// Accessor on the j-th row of pixels of a given image.
/**
 *  @param anImage: the image
 *  @param j:       the row number
 *  @return the j-th row of pixels
 */
//------------------------------------------------------------------------------
std::vector<float> getImageRow(const std::vector<std::vector<float> >& anImage,
                               int j);


//------------------------------------------------------------------------------
/// Accessor on the chemical element's name depending on its Z number.
/**
 *  @param Z:   the atomic number of the element
 *  @return the name of the corresponding element
 */
//------------------------------------------------------------------------------
std::string getElementName(unsigned short Z);


//------------------------------------------------------------------------------
/// Accessor on the chemical element's name depending on its symbol.
/**
 *  @param aSymbol:   the symbol of the element
 *  @return the name of the corresponding element
 */
//------------------------------------------------------------------------------
std::string getElementName(const std::string& aSymbol);


//------------------------------------------------------------------------------
/// Accessor on the chemical element' symbol depending on its Z number.
/**
 *  @param Z:   the atomic number of the element
 *  @return the name of the corresponding element
 */
//------------------------------------------------------------------------------
std::string getElementSymbol(unsigned short Z);


//------------------------------------------------------------------------------
/// Accessor on the chemical element's Z number.
/**
 *  @param anElement:   the name or symbol of the element
 *  @return the Z number of the corresponding element
 */
//------------------------------------------------------------------------------
unsigned short getElementAtomicNumber(const std::string& anElement);


//------------------------------------------------------------------------------
/// Accessor on the chemical element's density.
/**
 *  @param anElement:   the name or symbol of the element
 *  @return the density of the corresponding element (in g / cm3)
 */
//------------------------------------------------------------------------------
double getDensityFromElement(const std::string& anElement);


//------------------------------------------------------------------------------
/// Accessor on the chemical element's density.
/**
 *  @param Z:   the Z number of the element
 *  @return the density of the corresponding element (in g / cm3)
 */
//------------------------------------------------------------------------------
double getDensityFromElement(unsigned short Z);


//------------------------------------------------------------------------------
/// Accessor on the HU's density.
/**
 *  @param HU:   the HU
 *  @return the density of the corresponding HU (in g / cm3)
 */
//------------------------------------------------------------------------------
double getDensityFromHU(double HU);


//------------------------------------------------------------------------------
/// Accessor on the chemical element's mass attenuation.
/**
 *  @param anElement:   the name or symbol of the element
 *  @param anEnergy:        the photon energy of interest
 *  @param aUnitOfEnergy:   the unit of energy corresponding to anEnergy.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @return the mass attenuation of the corresponding element (in cm2/g)
 */
//------------------------------------------------------------------------------
double getMassAttenuationFromElement(const std::string& anElement,
		double anEnergy,
		const std::string& aUnitOfEnergy);


//------------------------------------------------------------------------------
/// Accessor on the chemical element's mass attenuation.
/**
 *  @param Z:   the Z number of the element
 *  @param anEnergy:        the photon energy of interest
 *  @param aUnitOfEnergy:   the unit of energy corresponding to anEnergy.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @return the mass attenuation of the corresponding element (in cm2/g)
 */
//------------------------------------------------------------------------------
double getMassAttenuationFromElement(unsigned short Z,
		double anEnergy,
		const std::string& aUnitOfEnergy);


/// Accessor on the HU's mass attenuation.
/**
 *  @param HU:   the HU
 *  @param anEnergy:        the photon energy of interest
 *  @param aUnitOfEnergy:   the unit of energy corresponding to anEnergy.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @return the density of the corresponding HU (in cm2/g)
 */
//------------------------------------------------------------------------------
double getMassAttenuationFromHU(double HU,
		double anEnergy,
		const std::string& aUnitOfEnergy);


//------------------------------------------------------------------------------
/// Accessor on the mixture's mass attenuation.
/**
 *  @param aMixture:    the details about the mixture. It is given as a sequence
 *                      of element symbol & relative weight, e.g.
 *                      Ti90Al6V4.
 *  @param anEnergy:        the photon energy of interest
 *  @param aUnitOfEnergy:   the unit of energy corresponding to anEnergy.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @return the mass attenuation of the corresponding mixture (in cm2/g)
 */
//------------------------------------------------------------------------------
double getMassAttenuationFromMixture(const std::string& aMixture,
		double anEnergy,
		const std::string& aUnitOfEnergy);


//------------------------------------------------------------------------------
/// Accessor on the compound's mass attenuation.
/**
 *  @param aCompound:   the details about the compound. It is given as
 *                      a sequence of element symbol & number of atoms, e.g.
 *                      H2O for water and SiC for silicon carbide.
 *  @param anEnergy:        the photon energy of interest
 *  @param aUnitOfEnergy:   the unit of energy corresponding to anEnergy.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @return the mass attenuation of the corresponding compound (in cm2/g)
 */
//------------------------------------------------------------------------------
double getMassAttenuationFromCompound(const std::string& aCompound,
		double anEnergy,
		const std::string& aUnitOfEnergy);


/// Accessor on the HU's linear attenuation coefficient.
/**
 *  @param HU:   the HU
 *  @param anEnergy:        the photon energy of interest
 *  @param aUnitOfEnergy:   the unit of energy corresponding to anEnergy.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @return the linear attenuation coefficient (in cm-1)
 */
double getMuFromHU(double HU,
		double anEnergy,
		const std::string& aUnitOfEnergy);

std::vector<std::vector<std::vector< float> > > convertHU2mu(const std::vector<std::vector<std::vector< float> > >& anImage,
    double anEnergy,
    const std::string& aUnitOfEnergy);

/**
void setShiftFilter(double aValue);
void setScaleFilter(double aValue);
double getShiftFilter();
double getScaleFilter();
*/

std::vector<float> rayIntersect(const std::string& aLabel,
    double aRayOriginX, double aRayOriginY, double aRayOriginZ,
    double aRayDirectionX, double aRayDirectionY, double aRayDirectionZ);




//------------------------------------------------------------------------------
/// Compute the arithmetic mean of a 1D array.
/**
 *  @param aFileName:   the 1D array
 *  @return the arithmetic mean
 */
//------------------------------------------------------------------------------
double getMean(const std::vector<float>& anImage);


//------------------------------------------------------------------------------
/// Compute the standard deviation of a 1D array.
/**
 *  @param aFileName:   the 1D array
 *  @return the standard deviation
 */
//------------------------------------------------------------------------------
double getStddev(const std::vector<float>& anImage);



//------------------------------------------------------------------------------
/// Compute the zero-normalized cross-correlation (ZNCC) (also known as 
/// zero-mean unit-variance cross-correlation) between two 1D arrays `aReferenceSet` 
/// \f$(\theta)\f$ and `aTestImage` \f$\left(\hat{\theta}\right)\f$.
/// The ZNCC has a range of [-1, 1]. 
/// When the ZNCC is equal to -1, then \f$\theta\f$ and \f$\hat{\theta}\f$ are anticorrelated; 
/// when the ZNCC is equal to +1, then \f$\theta\f$ and \f$\hat{\theta}\f$ are fully correlated; and 
/// when the ZNCC is equal to 0, then \f$\theta\f$ and \f$\hat{\theta}\f$ are not correlated at all.
/**
 *  @param aReferenceSet:   the 1st 1D array
 *  @param aTestSet:   the 2nd 1D array
 *  @return the RMSE between aReferenceSet and aTestSet
 */
//------------------------------------------------------------------------------
double computeZNCC(const std::vector<float>& aReferenceSet,
    const std::vector<float>& aTestSet);


//------------------------------------------------------------------------------
/// Compute the root mean squared error (RMSE) (also known as 
/// root mean squared distance, RMSD) between two 1D arrays `aReferenceSet` 
/// \f$(\theta)\f$ and `aTestImage` \f$\left(\hat{\theta}\right)\f$. When RMSE
/// is close to zero, then \f$\theta\f$ and \f$\hat{\theta}\f$ 
/// are similar.
/**
 \f{eqnarray*}{
    MSE(\theta, \hat{\theta}) &=& \frac{\sum_{i = 0}^{I-1}\left(\theta(i) - \hat{\theta}(i)\right)^2}{I}\\
    RMSE(\theta, \hat\theta)  &=& \sqrt{MSE\left(\theta, \hat\theta\right)}
 \f}
*/
/**
 *  @param aReferenceSet:   the 1st 1D array
 *  @param aTestSet:   the 2nd 1D array
 *  @return the RMSE between aReferenceSet and aTestSet
 */
//------------------------------------------------------------------------------
double computeRMSE(const std::vector<float>& aReferenceSet,
    const std::vector<float>& aTestSet,
    bool normalise = false);





void loadReference(const std::vector<float>& aReferenceImage);


void loadTest(const std::vector<float>& aTestImage);


void lineariseTest(float aThreshold, float aScalingFactor);


double computeZNCC();


double computeRMSE(bool normalise = false);







//------------------------------------------------------------------------------
/// Load a 2D image from a file.
/**
 *  @param aFileName:   the file name of the 2D image
 *  @return the corresponding 2D image
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > loadImage2D(const std::string& aFileName);


//------------------------------------------------------------------------------
/// Load a 3D volume from a file.
/**
 *  @param aFileName:   the file name of the 3D volume
 *  @return the corresponding 3D volume
 */
//------------------------------------------------------------------------------
std::vector<std::vector<std::vector<float> > > loadImage3D(const std::string& aFileName);


//------------------------------------------------------------------------------
/// Find the smallest voxel value in a 3D volume.
/**
 *  @param a3DVolume:   the 3D volume
 *  @return the smallest voxel value
 */
//------------------------------------------------------------------------------
float getMinValue(const std::vector<std::vector<std::vector<float> > >& aImage);


//------------------------------------------------------------------------------
/// Find the largest voxel value in a 3D volume.
/**
 *  @param a3DVolume:   the 3D volume
 *  @return the largest voxel value
 */
//------------------------------------------------------------------------------
float getMaxValue(const std::vector<std::vector<std::vector<float> > >& a3DVolume);


//------------------------------------------------------------------------------
/// Compute the zero-normalized cross-correlation (ZNCC) (also known as 
/// zero-mean unit-variance cross-correlation) between two 3D volumes.
/// The ZNCC has a range of [-1, 1]. When the ZNCC is equal to -1, then
/// the two volume are anticorrelated; when the ZNCC is equal to +1, then
/// the two volume are fully correlated; and when the ZNCC is equal to 0, then
/// the two volume are not correlated at all.
/**
 *  @param aReferenceSet:   the 1st 3D volume
 *  @param aTestSet:   the 2nd 3D volume
 *  @return the ZNCC between aReferenceSet and aTestSet
 */
//------------------------------------------------------------------------------
double computeZNCC(const std::vector<std::vector<std::vector<float> > >& aReferenceImage,
    const std::vector<std::vector<std::vector<float> > >& aTestSet);


//------------------------------------------------------------------------------
/// Compute the root mean squared error (RMSE) (also known as 
/// root mean squared distance or root mean square deviation, RMSD) between two 
/// 3D volumes `aReferenceSet` \f$(\theta)\f$ and `aTestImage` 
/// \f$\left(\hat{\theta}\right)\f$. When RMSE is close to zero, then 
/// \f$\theta\f$ and \f$\hat{\theta}\f$ are similar.
/**
 \f{eqnarray*}{
    MSE(\theta, \hat{\theta}) &=& \frac{\sum_{i = 0}^{I-1}\sum_{j = 0}^{J-1}\sum_{k = 0}^{K-1}\left(\theta(i,j,k) - \hat{\theta}(i,j,k)\right)^2}{I \times J \times K}\\
    RMSE(\theta, \hat\theta)  &=& \sqrt{MSE\left(\theta, \hat\theta\right)}
 \f}
*/
/**
 *  @param aReferenceSet:   the 1st 3D volume
 *  @param aTestImage:   the 2nd 3D volume
 *  @return the RMSE between aReferenceSet and aTestSet
 */
//------------------------------------------------------------------------------
double computeRMSE(const std::vector<std::vector<std::vector<float> > >& aReferenceSet,
    const std::vector<std::vector<std::vector<float> > >& aTestSet);


#endif
