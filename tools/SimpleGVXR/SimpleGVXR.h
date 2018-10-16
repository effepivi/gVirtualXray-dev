/*

Copyright (c) 2018, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*   @date       03/10/2018
*
*   @author     Dr Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*   http://www.fpvidal.net/, Oct 2018, 2018, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <vector>
#include <string>


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
/// Set the position of the X-ray source.
/**
 *  @param x:   the position along the X-axis
 *  @param y:   the position along the Y-axis
 *  @param z:   the position along the Z-axis
 *  @param aUnitOfLength:	the unit of length corresponding to the x, y and z
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
void setSourcePosition(double x, double y, double z, const char* aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the position of the X-ray detector.
/**
 *  @param x:   the position along the X-axis
 *  @param y:   the position along the Y-axis
 *  @param z:   the position along the Z-axis
 *  @param aUnitOfLength:	the unit of length corresponding to the x, y and z
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
void setDetectorPosition(double x, double y, double z, const char* aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the up-vector defining the orientation of the X-ray detector.
/**
 *  @param x:   the component of the vector along the X-axis
 *  @param y:   the component of the vector along the Y-axis
 *  @param z:	the component of the vector along the Z-axis
 */
//------------------------------------------------------------------------------
void setDetectorUpVector(double x, double y, double z);


//------------------------------------------------------------------------------
/// Set the number of pixels of the X-ray detector.
/**
 *  @param aWidth:  the number of pixels along the X-axis
 *  @param aHeight:	the number of pixels along the Y-axis
 */
//------------------------------------------------------------------------------
void setDetectorNumberOfPixels(int aWidth, int aHeight);


//------------------------------------------------------------------------------
/// Set the pixel size.
/**
 *  @param aWidth:  the pixel size along the X-axis
 *  @param aHeight:	the pixel size along the Y-axis
 *  @param aUnitOfLength:	the unit of length corresponding to the aWidth and aHeight
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
void setDetectorPixelSize(double aWidth,
						  double aHeight,
						  const char* aUnitOfLength);


//------------------------------------------------------------------------------
/// Load a polygon mesh from a file, set its label in the scenegraph
/// (i.e. identifier) and add it to the X-ray renderer.
/**
 *  @param aLabel:		the label in the scenegraph
 *  @param aFileName:	the file where the polygon mesh data is stored
 *  @param aUnitOfLength:	the unit of length corresponding to the data stored
 *                          in the file. Acceptable values are:
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
void loadMeshFile(const char* aLabel,
                  const char* aFileName,
                  const char* aUnitOfLength);


//------------------------------------------------------------------------------
/// Load a scenegraph from a file and add its polygon meshes
/// to the X-ray renderer.
/**
 *  @param aFileName:	the file where the polygon mesh data is stored
 *  @param aUnitOfLength:	the unit of length corresponding to the data stored
 *                          in the file. Acceptable values are:
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
void loadSceneGraph(const char* aFileName,
        			const char* aUnitOfLength);


//------------------------------------------------------------------------------
/// Create an empty polygon mesh and set its label in the scenegraph
/// (i.e. identifier). Note that it is not added to the X-ray renderer.
/**
 *  @param aLabel:		the label in the scenegraph
 *  @param aParent: the parent of the node in the scenegraph (default: "root")
 */
//------------------------------------------------------------------------------
void emptyMesh(const std::string& aLabel,
        const std::string& aParent = "root");


//------------------------------------------------------------------------------
/// Create a cube and set its label in the scenegraph (i.e. identifier).
/// Note that it is not added to the X-ray renderer.
/**
 *  @param aLabel:	the label in the scenegraph
 *  @param aLength:	the size of an edge of the cube
 *  @param aUnitOfLength:	the unit of length corresponding to aLength.
 *                          Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *                          (default value: "cm")
 *  @param aParent: the parent of the node in the scenegraph (default: "root")
 */
//------------------------------------------------------------------------------
void makeCube(const std::string& aLabel,
        double aLength = 1.0,
        const char* aUnitOfLength = "cm",
        const std::string& aParent = "root");


//------------------------------------------------------------------------------
/// Create a cylinder and set its label in the scenegraph (i.e. identifier).
/// Note that it is not added to the X-ray renderer.
/**
 *  @param aLabel:	the label in the scenegraph
 *  @param aNumberOfSectors:	the number of sector defining the mesh
 *  @param aHeight:	the height of the cylinder
 *  @param aRadius:	the radius of the cylinder
 *  @param aUnitOfLength:	the unit of length corresponding to aHeight and
 *                          aRadius. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *                          (default value: "cm")
 *  @param aParent: the parent of the node in the scenegraph (default: "root")
 */
//------------------------------------------------------------------------------
void makeCylinder(const std::string& aLabel,
		unsigned int aNumberOfSectors = 10,
        double aHeight = 1.0,
        double aRadius = 0.5,
		const char* aUnitOfLength = "cm",
        const std::string& aParent = "root");


//------------------------------------------------------------------------------
/// Add a polygon mesh (aSource) to another one (aDestination).
/// It corresponds to the operation as follows: aDestination += aSource.
/// Both aSource and aDestination have to already exist in the scenegraph.
/**
 *  @param aDestination:	the polygon mesh that is going to be modified
 *  @param aSource:			the polygon mesh that is going to be added
 *   						to aDestination
 */
//------------------------------------------------------------------------------
void addMesh(const std::string& aDestination, const std::string& aSource);


//------------------------------------------------------------------------------
/// Subtract a polygon mesh (aSource) from another one (aDestination).
/// It corresponds to the operation as follows: aDestination -= aSource.
/// Both aSource and aDestination have to already exist in the scenegraph.
/**
 *  @param aDestination:	the polygon mesh that is going to be modified
 *  @param aSource:			the polygon mesh that is going to be subtracted
 *   						from aDestination
 */
//------------------------------------------------------------------------------
void subtractMesh(const std::string& aDestination, const std::string& aSource);


//------------------------------------------------------------------------------
/// Accessor on the list of labels of polygon meshes in the scenegraph.
/**
 *  @return the list of labels
 */
//------------------------------------------------------------------------------
std::vector<std::string> getMeshLabelSet();


//------------------------------------------------------------------------------
/// Accessor on the number of polygon meshes in the scenegraph.
/**
 *  @return the number of polygon meshes in the scenegraph
 */
//------------------------------------------------------------------------------
unsigned int getMeshLabelSetSize();


//------------------------------------------------------------------------------
/// Accessor on the i-th label in the list of polygon meshes in the scenegraph.
/**
 *  @param i:	the ID of the polygon mesh
 *  @return the label corresponding to the i-th polygon mesh
 */
//------------------------------------------------------------------------------
std::string getMeshLabel(unsigned int i);


//------------------------------------------------------------------------------
/// Add a polygon mesh, given its label, to the X-ray renderer as an inner surface.
/**
 *  @param aLabel:	the label of the polygon mesh in the scenegraph
 */
//------------------------------------------------------------------------------
void addPolygonMeshAsInnerSurface(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Add a polygon mesh, given its label, to the X-ray renderer as an outer surface.
/**
 *  @param aLabel:	the label of the polygon mesh in the scenegraph
 */
//------------------------------------------------------------------------------
void addPolygonMeshAsOuterSurface(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Empty the scenegraph. Note that it also empties the X-ray renderer from all its meshes.
//------------------------------------------------------------------------------
void removePolygonMeshesFromSceneGraph();


//------------------------------------------------------------------------------
/// Empty the X-ray renderer from all its meshes.
/// Note that the meshes are kept in the scenegraph.
//------------------------------------------------------------------------------
void removePolygonMeshesFromXRayRenderer();


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
/*
 *  @param aLabel:	the label of the polygon mesh in the scenegraph
 */
//------------------------------------------------------------------------------
void moveToCenter(const char* aLabel);


//------------------------------------------------------------------------------
/// Move a polygon mesh to the centre.
/*
 *  @param aLabel:	the label of the polygon mesh in the scenegraph
 */
//------------------------------------------------------------------------------
void moveToCentre(const char* aLabel);


//------------------------------------------------------------------------------
/// Scale a polygon mesh.
/*
 *  @param aLabel:	the label of the polygon mesh to transform
 *  @param x:   the scaling factor along the X-axis
 *  @param y:   the scaling factor along the Y-axis
 *  @param z:   the scaling factor along the Z-axis
 *  @param aUnitOfLength:	the unit of length corresponding to the x, y and z
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
void scaleNode(const char* aLabel, double x, double y, double z, const char* aUnitOfLength);


//------------------------------------------------------------------------------
/// Rotate a polygon mesh.
/*
 *  @param aLabel:	the label of the polygon mesh to transform
 *  @param anAngle:   the rotation angle in degrees
 *  @param x:   the component of the rotation vector along the X-axis
 *  @param y:   the component of the rotation vector along the Y-axis
 *  @param z:   the component of the rotation vector along the Z-axis
 */
//------------------------------------------------------------------------------
void rotateNode(const char* aLabel, double anAngle, double x, double y, double z);


//------------------------------------------------------------------------------
/// Translate a polygon mesh.
/*
 *  @param aLabel:	the label of the polygon mesh to transform
 *  @param x:   the component of the translation vector along the X-axis
 *  @param y:   the component of the translation vector along the Y-axis
 *  @param z:   the component of the translation vector along the Z-axis
 *  @param aUnitOfLength:	the unit of length corresponding to the x, y and z
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
void translateNode(const char* aLabel, double x, double y, double z, const char* aUnitOfLength);


//------------------------------------------------------------------------------
/// Replace the 3-D transformation matrix of the overall 3-D scene by an identity matrix.
//------------------------------------------------------------------------------
void resetSceneTransformation();


//------------------------------------------------------------------------------
/// Scale the 3-D scene (all the polygon meshes will be affected but not modified).
/*
 *  @param x:   the scaling factor along the X-axis
 *  @param y:   the scaling factor along the Y-axis
 *  @param z:   the scaling factor along the Z-axis
 *  @param aUnitOfLength:	the unit of length corresponding to the x, y and z
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
void scaleScene(double x, double y, double z, const char* aUnitOfLength);


//------------------------------------------------------------------------------
/// Rotate the 3-D scene (all the polygon meshes will be affected but not modified).
/*
 *  @param anAngle:   the rotation angle in degrees
 *  @param x:   the component of the rotation vector along the X-axis
 *  @param y:   the component of the rotation vector along the Y-axis
 *  @param z:   the component of the rotation vector along the Z-axis
 */
//------------------------------------------------------------------------------
void rotateScene(double anAngle, double x, double y, double z);


//------------------------------------------------------------------------------
/// Translate the 3-D scene (all the polygon meshes will be affected but not modified).
/*
 *  @param x:   the component of the translation vector along the X-axis
 *  @param y:   the component of the translation vector along the Y-axis
 *  @param z:   the component of the translation vector along the Z-axis
 *  @param aUnitOfLength:	the unit of length corresponding to the x, y and z
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
void translateScene(double x, double y, double z, const char* aUnitOfLength);


//------------------------------------------------------------------------------
/// Accessor on the 3-D scene transformation matrix.
/*
 *  @return the 3-D scene transformation matrix as a 4x4 array
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > getSceneTransformationMatrix();


//------------------------------------------------------------------------------
/// Accessor on the transformation matrix of the scengraph's root node.
/*
 *  @return the scengraph's root node transformation matrix as a 4x4 array
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > getRootTransformationMatrix();


//------------------------------------------------------------------------------
/// Accessor on the transformation matrix of a given node.
/*
 *  @param aLabel:	the label of the polygon mesh
 *  @return the scengraph's root node transformation matrix as a 4x4 array
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > getNodeTransformationMatrix(const char* aLabel);


//------------------------------------------------------------------------------
/// Set the 3-D scene transformation matrix.
/*
 *  @param aMatrix: the transformation matrix as a 4x4 array
 */
//------------------------------------------------------------------------------
void setSceneTransformationMatrix(const std::vector<std::vector<float> >& aMatrix);


//------------------------------------------------------------------------------
/// Set the transformation matrix of the scengraph's root node.
/*
 *  @param aMatrix: the transformation matrix as a 4x4 array
 */
//------------------------------------------------------------------------------
void setRootTransformationMatrix(const std::vector<std::vector<float> >& aMatrix);


//------------------------------------------------------------------------------
/// Set the transformation matrix of a given node.
/*
 *  @param aLabel:	the label of the polygon mesh
 *  @param aMatrix: the transformation matrix as a 4x4 array
 */
//------------------------------------------------------------------------------
void setNodeTransformationMatrix(const char* aLabel, const std::vector<std::vector<float> >& aMatrix);


//------------------------------------------------------------------------------
/// Set the Hounsfield value corresponding to the material properties of a polygon mesh.
/*
 *  @param aLabel:	the label of the polygon mesh
 *  @param HU:		the Hounsfield value
 */
//------------------------------------------------------------------------------
void setHU(const char* aLabel, short HU);




//------------------------------------------------------------------------------
/// Set the Hounsfield value corresponding to the material properties of a polygon mesh.
/*
 *  @param aLabel:	the label of the polygon mesh
 *  @param HU:		the Hounsfield value
 */
//------------------------------------------------------------------------------
void setHounsfieldUnit(const char* aLabel, short HU);




//------------------------------------------------------------------------------
/// Set the Hounsfield value corresponding to the material properties of a polygon mesh.
/*
 *  @param aLabel:	the label of the polygon mesh
 *  @param HU:		the Hounsfield value
 */
//------------------------------------------------------------------------------
void setHounsfieldValue(const char* aLabel, short HU);


//------------------------------------------------------------------------------
/// Set the chemical element (or element) corresponding to the material properties of a polygon mesh.
/*
 *  @param aLabel:	the label of the polygon mesh
 *  @param Z:		the atomic number (or Z number) corresponding to the element
 */
//------------------------------------------------------------------------------
void setElement(const char* aLabel, unsigned short Z);


//------------------------------------------------------------------------------
/// Set the chemical element (or element) corresponding to the material properties of a polygon mesh.
/*
 *  @param aLabel:	the label of the polygon mesh
 *  @param aName:	the symbol or name corresponding to the element
 */
//------------------------------------------------------------------------------
void setElement(const char* aLabel, const std::string& aName);


//------------------------------------------------------------------------------
/// Set the mixture corresponding to the material properties of a polygon mesh.
/// Don't forget to set the density of the material.
/*
 *  @param aLabel:		the label of the polygon mesh
 *  @param aMixture:	the details about the mixture. It is given as a sequence
 *  					of element symbol & relative weight, e.g.
 *  					Ti90Al6V4.
 */
//------------------------------------------------------------------------------
void setMixture(const char* aLabel, const std::string& aMixture);


//------------------------------------------------------------------------------
/// Set the compound corresponding to the material properties of a polygon mesh.
/// Don't forget to set the density of the material.
/*
 *  @param aLabel:		the label of the polygon mesh
 *  @param aCompound:	the details about the compound. It is given as a sequence
 *  					of element symbol & number of atoms, e.g.
 *  					H2O for water and SiC for silicon carbide.
 */
//------------------------------------------------------------------------------
void setCompound(const char* aLabel, const std::string& aCompound);


//------------------------------------------------------------------------------
/// Set the mass attenuation coefficient corresponding to the material properties of a polygon mesh.
/// Don't forget to set the density of the material.
/*
 *  @param aLabel:			the label of the polygon mesh
 *  @param aCoefficient:	the mass attenuation coefficient
 *  @param aUnit:			the unit corresponding to aCoefficient.
 *                          Acceptable values are:
 *                          "cm2/g" and "cm2.g-1"
 */
//------------------------------------------------------------------------------
void setMassAttenuationCoefficient(const char* aLabel, double aCoefficient,
		   const char* aUnit);


//------------------------------------------------------------------------------
/// Set the linear attenuation coefficient corresponding to the material properties of a polygon mesh.
/*
 *  @param aLabel:			the label of the polygon mesh
 *  @param aCoefficient:	the liner attenuation coefficient
 *  @param aUnit:			the unit corresponding to aCoefficient.
 *                          Acceptable values are:
 *                          "cm-1"
 */
//------------------------------------------------------------------------------
void setLinearAttenuationCoefficient(const char* aLabel, double aCoefficient,
		   const char* aUnit);


//------------------------------------------------------------------------------
/// Set the linear attenuation coefficient corresponding to the material properties of a polygon mesh.
/*
 *  @param aLabel:			the label of the polygon mesh
 *  @param aCoefficient:	the liner attenuation coefficient
 *  @param aUnit:			the unit corresponding to aCoefficient.
 *                          Acceptable values are:
 *                          "cm-1"
 */
//------------------------------------------------------------------------------
void setMu(const char* aLabel, double aCoefficient,
		   const char* aUnit);


//------------------------------------------------------------------------------
/// Set the density corresponding to the material properties of a polygon mesh.
/*
 *  @param aLabel:			the label of the polygon mesh
 *  @param aDensity:	the liner attenuation coefficient
 *  @param aUnit:			the unit corresponding to aDensity.
 *                          Acceptable values are:
 *                          "g/cm3" and "g.cm-3"
 */
//------------------------------------------------------------------------------
void setDensity(const char* aLabel, double aDensity,
		   const char* aUnit);


//------------------------------------------------------------------------------
/// Create an OpenGL context (the window won't be shown).
/*
 *  @param aWindowID:	the numerical ID of the context to create
 *                      (default value: -1, means that the ID will be automatically generated)
 */
//------------------------------------------------------------------------------
void createOpenGLContext(int aWindowID = -1);


//------------------------------------------------------------------------------
/// Create an OpenGL context and display it in a window
/*
 *  @param aWindowID:	the numerical ID of the context to create
 *                      (default value: -1, means that the ID will be automatically generated)
 */
//------------------------------------------------------------------------------
void createWindow(int aWindowID = -1, int aVisibilityFlag = 0);


//------------------------------------------------------------------------------
/// Create an OpenGL context and display it in a window
/*
 *  @param aWidth:  	the number of pixels along the X-axis
 *  @param aHeight:		the number of pixels along the Y-axis
 *  @param aWindowID:	the numerical ID of the corresponding context
 *                      (default value: -1, means that the active context will be used)
 */
//------------------------------------------------------------------------------
void setWindowSize(int aWidth, int aHeight, int aWindowID = -1);


//------------------------------------------------------------------------------
/// 3-D visualisation of the 3-D scene (source, detector, and scanned objects).
/// Note that there is no interactive loop running.
/*
 *  @param aWindowID:	the numerical ID of the corresponding context
 *                      (default value: -1, means that the active context will be used)
 */
//------------------------------------------------------------------------------
void displayScene(int aWindowID = -1);


//------------------------------------------------------------------------------
/// 3-D visualisation of the 3-D scene (source, detector, and scanned objects).
/// Note that there is interactive loop running.
/// Keys are:
///		- Q/Escape: to quit the event loop (does not close the window)
///		- B: display/hide the X-ray beam
///		- W: display the polygon meshes in solid or wireframe
///		- N: display the X-ray image in negative or positive
///		- L: switch lighting on/off
///		- D: display/hide the X-ray detector
///		- V: display/hide normal vectors
/*
 *  @param aWindowID:	the numerical ID of the corresponding context
 *                      (default value: -1, means that the active context will be used)
 */
//------------------------------------------------------------------------------
void renderLoop(int aWindowID = -1);


//------------------------------------------------------------------------------
/// Make an OpenGL context visible and display a window.
/*
 *  @param aWindowID:	the numerical ID of the corresponding context
 *                      (default value: -1, means that the active context will be used)
 */
//------------------------------------------------------------------------------
void showWindow(int aWindowID = -1);


//------------------------------------------------------------------------------
/// Hide a window.
/*
 *  @param aWindowID:	the numerical ID of the corresponding context
 *                      (default value: -1, means that the active context will be used)
 */
//------------------------------------------------------------------------------
void hideWindow(int aWindowID = -1);


//------------------------------------------------------------------------------
/// Close and destroy a given window or OpenGL context.
//------------------------------------------------------------------------------
void destroyWindow(int aWindowID = -1);


//------------------------------------------------------------------------------
/// Close and destroy all the windows and OpenGL contexts that have been created.
//------------------------------------------------------------------------------
void destroyAllWindows(void);


//------------------------------------------------------------------------------
/// Use a point source.
//------------------------------------------------------------------------------
void usePointSource();


//------------------------------------------------------------------------------
/// Use a parallel source (e.g. to mimic a source that is extremely far from the scanned object and detector).
//------------------------------------------------------------------------------
void useParallelBeam();


//------------------------------------------------------------------------------
/// Use a parallel source (e.g. to mimic a source that is extremely far from the scanned object and detector).
//------------------------------------------------------------------------------
void useParallelSource();


//------------------------------------------------------------------------------
/// Empty the beam spectrum.
//------------------------------------------------------------------------------
void resetBeamSpectrum();


//------------------------------------------------------------------------------
/// Use a monochromatic beam spectrum (i.e. one single energy).
/*
 *  @param anEnergy:			the incident photon energy
 *  @param aUnitOfEnergy:		the unit of energy corresponding to anEnergy.
 *  							Acceptable values are:
 *  							"electronvolt", "eV",
 *  							"kiloelectronvolt", "keV",
 *  							"megaelectronvolt", "MeV"
 *  @param aNumberOfPhotons:	the number of incident photons
 */
//------------------------------------------------------------------------------
void setMonoChromatic(double anEnergy,
		              const char* aUnitOfEnergy,
					  double aNumberOfPhotons);


//------------------------------------------------------------------------------
/// Add an energy bin to the beam spectrum (useful to generate polychromatism).
/*
 *  @param anEnergy:			the incident photon energy
 *  @param aUnitOfEnergy:		the unit of energy corresponding to anEnergy.
 *  							Acceptable values are:
 *  							"electronvolt", "eV",
 *  							"kiloelectronvolt", "keV",
 *  							"megaelectronvolt", "MeV"
 *  @param aNumberOfPhotons:	the number of incident photons
 */
//------------------------------------------------------------------------------
void addEnergyBinToSpectrum(double anEnergy,
							const char* aUnitOfEnergy,
							double aNumberOfPhotons);


//------------------------------------------------------------------------------
/// Save the last X-ray image that has been computed.
/*
 *  @param aFileName:	the name of the output file
 *  					(default: 0 means that the filename will be
 *  					automatically generated and the file saved in
 *  					the current path)
 */
//------------------------------------------------------------------------------
void saveLastXRayImage(const char* aFileName = 0);


//------------------------------------------------------------------------------
/// Save the last L-buffer that has been computed.
/*
 *  @param aFileName:	the name of the output file
 *  					(default: 0 means that the filename will be
 *  					automatically generated and the file saved in
 *  					the current path)
 */
//------------------------------------------------------------------------------
void saveLastLBuffer(const char* aFileName = 0);


//------------------------------------------------------------------------------
/// Save the last sinogram that has been computed.
/*
 *  @param aFileName:	the name of the output file
 *  					(default: 0 means that the filename will be
 *  					automatically generated and the file saved in
 *  					the current path)
 */
//------------------------------------------------------------------------------
void saveLastSinogram(const char* aFileName = 0);


//------------------------------------------------------------------------------
/// Enable artefact filtering on GPU (it is fast but can be inaccurate).
//------------------------------------------------------------------------------
void enableArtefactFilteringOnGPU(void);


//------------------------------------------------------------------------------
/// Enable artefact filtering on GPU (it is fast but can be inaccurate).
//------------------------------------------------------------------------------
void enableArtifactFilteringOnGPU(void);


//------------------------------------------------------------------------------
/// Enable artefact filtering on CPU (can be extremely slow as it makes use of ray-tracing).
//------------------------------------------------------------------------------
void enableArtefactFilteringOnCPU(void);


//------------------------------------------------------------------------------
/// Enable artefact filtering on CPU (can be extremely slow as it makes use of ray-tracing).
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
/// Accessor on the numerical value corresponding to a unit of energy.
/*
 *  @param aUnitOfEnergy:		the unit of energy.
 *  							Acceptable values are:
 *  							"electronvolt", "eV",
 *  							"kiloelectronvolt", "keV",
 *  							"megaelectronvolt", "MeV"
 *  @return the corresponding numerical value
 */
//------------------------------------------------------------------------------
double getUnitOfEnergy(const char* aUnitOfEnergy);


//------------------------------------------------------------------------------
/// Accessor on the numerical value corresponding to a unit of length.
/*
 *  @param aUnitOfLength:		the unit of length.
 *  							Acceptable values are:
 *                          	"um", "micrometre",  "micrometer",
 *                          	"mm", "millimetre",  "millimeter",
 *                          	"cm", "centimetre",  "centimeter",
 *                      	    "dm",  "decimetre",  "decimeter",
 *                     		    "m",   "metre",      "meter",
 *                    	        "dam", "decametre",  "decameter",
 *                   	        "hm",  "hectometre", "hectometer",
 *                              "km",  "kilometre",  "kilometer"
 *  @return the corresponding numerical value
 */
//------------------------------------------------------------------------------
double getUnitOfLength(const char* aUnitOfLength);


//------------------------------------------------------------------------------
/// Compute the X-ray projection corresponding to the environment that
/// has previously been set, i.e.
/// 	- Detector position
/// 	- Detector orientation
/// 	- Detector size and resolution
/// 	- Source position
/// 	- Source shape
///		- Beam spectrum
///		- Scanned object geometries
///		- Scanned object material properties
/*
 *  @return the corresponding X-ray image
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > computeXRayImage();


//------------------------------------------------------------------------------
/// Compute the L-buffer of a polygon mesh corresponding to the environment that
/// has previously been set, i.e.
/// 	- Detector position
/// 	- Detector orientation
/// 	- Detector size and resolution
/// 	- Source position
/// 	- Source shape
///		- Scanned object geometry
/*
 *  @param aLabel:	the label of the polygon mesh
 *  @return the corresponding L-buffer
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > computeLBuffer(const char* aLabel);


//------------------------------------------------------------------------------
/// Compute the sinogram corresponding to the environment that
/// has previously been set, i.e.
/// 	- Detector position
/// 	- Detector orientation
/// 	- Detector size and resolution
/// 	- Source position
/// 	- Source shape
///		- Beam spectrum
///		- Scanned object geometries
///		- Scanned object material properties
/// Note that the rotation vector is the same as the up-vector of the detector.
/*
 *  @param x:   the position of the rotation centre along the X-axis
 *  @param y:   the position of the rotation centre along the Y-axis
 *  @param z:   the position of the rotation centre along the Z-axis
 *  @param aUnitOfLength:	the unit of length corresponding to the x, y and z
 *                          parameters. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *  @param aNumberOfAngle:	the total number of projections in the sinogram
 *  @param anAngleOffset:	the angle in degrees between two successive projections
 *  @return the corresponding sinogram
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > computeSinogram(double x, double y, double z, const char* aUnitOfLength,
        unsigned int aNumberOfAngle,
        double anAngleOffset);


//------------------------------------------------------------------------------
/// Accessor on the last X-ray image that has been computed.
/*
 *  @return the last X-ray image
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > getLastXRayImage();


//------------------------------------------------------------------------------
/// Accessor on the last L-buffer that has been computed.
/*
 *  @return the last L-buffer
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > getLastLBuffer();


//------------------------------------------------------------------------------
/// Accessor on the last sinogram that has been computed.
/*
 *  @return the last sinogram
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > getLastSinogram();


//------------------------------------------------------------------------------
/// Accessor on the width of a given image.
/*
 *  @param anImage:	the image
 *  @return the width of the image (in number of pixels)
 */
//------------------------------------------------------------------------------
int getImageWidth(const std::vector<std::vector<float> >& anImage);


//------------------------------------------------------------------------------
/// Accessor on the height of a given image.
/*
 *  @param anImage:	the image
 *  @return the height of the image (in number of pixels)
 */
//------------------------------------------------------------------------------
int getImageHeght(const std::vector<std::vector<float> >& anImage);


//------------------------------------------------------------------------------
/// Accessor on the j-th row of pixels of a given image.
/*
 *  @param anImage:	the image
 *  @param j:	the row number
 *  @return the j-th row of pixels
 */
//------------------------------------------------------------------------------
std::vector<float> getImageRow(const std::vector<std::vector<float> >& anImage, int j);


#endif
