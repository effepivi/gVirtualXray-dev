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


#ifndef __SGVXR_Sample_h
#define __SGVXR_Sample_h


/**
********************************************************************************
*
*   @file       sample.h
*
*   @brief      Expose here all the functions of SimpleGVXR related to 
*               the sample. It includes its geometry and material composition.
*
*   @version    1.0
*
*   @date       10/07/2024
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
/// Load a polygon mesh from a file, set its label in the scenegraph
/// (i.e. identifier) and add it to the X-ray renderer.
/**
 *  @param aLabel:          the label in the scenegraph
 *  @param aFileName:       the file where the polygon mesh data is stored
 *  @param aUnitOfLength:   the unit of length corresponding to the data stored
 *                          in the file. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *  @param addToRendererAsInnerSurface: a flag to add the mesh to the X-ray renderer (as an inner surface) (default value: true)
 *  @param aParent: the parent of the node in the scenegraph (default: "root")
 */
//------------------------------------------------------------------------------
void loadMeshFile(const std::string& aLabel,
        const std::string& aFileName,
        const std::string& aUnitOfLength,
        bool addToRendererAsInnerSurface = true,
        const std::string& aParent = "root");


//------------------------------------------------------------------------------
/// Accessor on the filename of the mesh.
/**
 *  @param aLabel:          the label in the scenegraph
 *  @return the filename
 */
//------------------------------------------------------------------------------
std::string getMeshFilename(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Accessor on the unit of length of the mesh.
/**
 *  @param aLabel:          the label in the scenegraph
 *  @return the unit of length
 */
//------------------------------------------------------------------------------
std::string getMeshUnitOfLength(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Load a scenegraph from a file and add its polygon meshes
/// to the X-ray renderer.
/**
 *  @param aFileName:       the file where the polygon mesh data is stored
 *  @param aUnitOfLength:   the unit of length corresponding to the data stored
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
void loadSceneGraph(const std::string& aFileName,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Accessor on the number of primitives of a given node.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @return the number of primitives
 */
//------------------------------------------------------------------------------
unsigned int getNumberOfPrimitives(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Accessor on the number of face normal vectors of a given node.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @return the number of face normal vectors
 */
//------------------------------------------------------------------------------
unsigned int getNumberOfFaceNormalVectors(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Accessor on the number of vertex normal vectors of a given node.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @return the number of vertex normal vectors
 */
//------------------------------------------------------------------------------
unsigned int getNumberOfVertexNormalVectors(const std::string& aLabel);


//--------------------------------------------------------------------------
/// Accessor on the number of verticesof a given node.
/**
 * @param aLabel:  the label in the scenegraph
 * @return the number of vertices
 */
//--------------------------------------------------------------------------
unsigned int getVertexNumber(const std::string& aLabel);


//--------------------------------------------------------------------------
/// Accessor on the number of indicesof a given node.
/**
 * @param aLabel:  the label in the scenegraph
 * @return the number of indices
 */
//--------------------------------------------------------------------------
unsigned int getIndexNumber(const std::string& aLabel);


//--------------------------------------------------------------------------
/// Accessor on the vertices of a given node.
/**
 * @param aLabel:  the label in the scenegraph
 * @return the vertex
 */
//--------------------------------------------------------------------------
std::vector<float> getVertexSet(const std::string& aLabel);


std::vector<float> getFaceNormalVectorSet(const std::string& aLabel);


std::vector<float> getVertexNormalVectorSet(const std::string& aLabel);


std::vector<int> getIndexSet(const std::string& aLabel);


bool isInnerSurface(const std::string& aLabel);
bool isOuterSurface(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Create an empty polygon mesh and set its label in the scenegraph
/// (i.e. identifier). Note that it is not added to the X-ray renderer.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @param aParent: the parent of the node in the scenegraph (default: "root")
 */
//------------------------------------------------------------------------------
void emptyMesh(const std::string& aLabel,
        const std::string& aParent = "root");


//------------------------------------------------------------------------------
/// Create a cube, centred on (0, 0, 0) and set its label in the scenegraph (i.e. identifier).
/// Note that it is not added to the X-ray renderer.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @param aLength: the size of an edge of the cube (default value: 1)
 *  @param aUnitOfLength:   the unit of length corresponding to aLength.
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
        const std::string& aUnitOfLength = "cm",
        const std::string& aParent = "root");


//------------------------------------------------------------------------------
/// Create a cuboid, centred on (0, 0, 0) and set its label in the scenegraph (i.e. identifier).
/// Note that it is not added to the X-ray renderer.
/**
 *  @param aLabel:  the label in the scenegraph
 *   @param aWidth:   the width of the cuboid, i.e. its length along the X-axis (default value: 1)
 *   @param aHeight:  the height of the cuboid, i.e. its length along the Y-axis (default value: 1)
 *   @param aDepth:   the depth of the cuboid, i.e. its length along the Z-axis (default value: 1)
 *  @param aUnitOfLength:   the unit of length corresponding to aLength.
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
void makeCuboid(const std::string& aLabel,
        double aWidth = 1.0,
        double aHeight = 1.0,
        double aDepth = 1.0,
        const std::string& aUnitOfLength = "cm",
        const std::string& aParent = "root");


//------------------------------------------------------------------------------
/// Create a cylinder and set its label in the scenegraph (i.e. identifier).
/// Note that it is not added to the X-ray renderer.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @param aNumberOfSectors:    the number of sectors defining the mesh (default value: 10)
 *  @param aHeight: the height of the cylinder (default value: 1.0)
 *  @param aRadius: the radius of the cylinder (default value: 0.5)
 *  @param aUnitOfLength:   the unit of length corresponding to aHeight and
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
                  const std::string& aUnitOfLength = "cm",
                  const std::string& aParent = "root");


//------------------------------------------------------------------------------
/// Create a sphere and set its label in the scenegraph (i.e. identifier).
/// Note that it is not added to the X-ray renderer.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @param aNumberOfRings:    the number of rings defining the mesh (default value: 10)
 *  @param aNumberOfSectors:    the number of sectors defining the mesh (default value: 10)
 *  @param aRadius: the radius of the sphere (default value: 0.5)
 *  @param aUnitOfLength:   the unit of length corresponding to aRadius. 
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
void makeSphere(const std::string& aLabel,
                unsigned int aNumberOfRings = 10,
                unsigned int aNumberOfSectors = 10,
                double aRadius = 0.5,
                const std::string& aUnitOfLength = "cm",
                const std::string& aParent = "root");


//------------------------------------------------------------------------------
/// Create a step wedge and set its label in the scenegraph (i.e. identifier).
/// Note that it is not added to the X-ray renderer.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @param aNumberOfSteps:   the number of steps in the stepwedge (default value: 10)
 *  @param aTotalLength:  the length of the step wedge, i.e. its length along the X-axis (default value: 10)
 *  @param aWidth:   the width of the step wedge, i.e. its length along the Y-axis (default value: 1)
 *  @param aFirstStepHeight:  the height of the first step on the step wedge, i.e. its length along the Z-axis (default value: 1)
 *  @param anOtherStepHeight:  the height of the rest of the steps on the step wedge, i.e. its length along the Z-axis (default value: 0.1)
 *  @param aUnitOfLength:   the unit of length corresponding to aRadius. 
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
void makeStepWedge(const std::string& aLabel,
                   unsigned int aNumberOfSteps = 2,
                   float aTotalLength = 10.0,
                   float aWidth = 1.0,
                   float aFirstStepHeight = 1.0,
                   float anOtherStepHeight = 0.1,
                   const std::string& aUnitOfLength = "cm",
                   const std::string& aParent = "root");


//------------------------------------------------------------------------------
/// Create an iso-surface from a 3D image and set its label in the scenegraph (i.e. identifier).
/// Note that it is not added to the X-ray renderer.
/**
*  @param aLabel:  the label in the scenegraph
*  @param anIsoValue: the iso-value
*  @param aVoxelDataSet:  the voxel dataset
*  @param aWidth:  the number of voxel along the X-axis
*  @param aHeight:  the number of voxel along the Y-axis
*  @param aDepth:  the number of voxel along the Z-axis
*  @param aSpacingX: the space between the centre of two successive voxels along the X-axis
*  @param aSpacingY: the space between the centre of two successive voxels along the Y-axis
*  @param aSpacingZ: the space between the centre of two successive voxels along the Z-axis
*  @param aUnitOfLength:   the unit of length corresponding to aSpacingX, aSpacingY and aSpacingZ.
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
void makeIsoSurface(const std::string& aLabel,
    short anIsoValue,
    const std::vector<short>& aVoxelDataSet,
    unsigned int aWidth,
    unsigned int aHeight,
    unsigned int aDepth,
    double aSpacingX,
    double aSpacingY,
    double aSpacingZ,
    const std::string& aUnitOfLength = "cm",
    const std::string& aParent = "root");


void makeTriangularMesh(const std::string& aLabel,
		const std::vector<float>& aVertexSet,
	    const std::string& aUnitOfLength = "cm",
		const std::string& aParent = "root");


void makeTriangularMesh(const std::string& aLabel,
		const std::vector<float>& aVertexSet,
		const std::vector<int>& aTriangleIndexSet,
	    const std::string& aUnitOfLength = "cm",
		const std::string& aParent = "root");


//------------------------------------------------------------------------------
/// Create a cylinder in which spheres are embedded. The cylinder is centred on
/// (0, 0, 0) and the spheres are randomly positioned in the cylinder.
/// Note that it is not added to the X-ray renderer.
/**
 *  @param aCylinderLabel:  the cylinder label in the scenegraph
 *  @param aCylinderNumberOfSectors:    the number of sectors defining the mesh
 *  @param aCylinderRadius:  the radius of the cylinder
 *  @param aCylinderHeight:  the height of the cylinder
 *
 *  @param aSphereLabel:            the sphere label in the scenegraph
 *  @param aSphereNumberOfRings:    the number of rings defining the mesh
 *  @param aSphereNumberOfSectors:  the number of sectors defining the mesh
 *  @param aSphereRadius:  the radius of the cylinder
 *
 *  @param aPercentVolume the percentage of the cylinder volume occupied by spheres
 *
 *  @param aUnitOfLength:   the unit of length corresponding to aCylinderRadius, aCylinderHeight, and aSphereRadius.
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
 *
 *  @param aParent: the parent of the node in the scenegraph (default: "root")
 *  
 *  @return the number of spheres
 */
//------------------------------------------------------------------------------
unsigned int makeSpheresInCylinder(
        const std::string& aCylinderLabel,
        unsigned int aCylinderNumberOfSectors,
        double aCylinderRadius,
        double aCylinderHeight,
        const std::string& aSphereLabel,
        unsigned int aSphereNumberOfRings,
        unsigned int aSphereNumberOfSectors,
        double aSphereRadius,
        double aPercentVolume,
        const std::string& aUnitOfLength = "cm",
        const std::string& aParent = "root");


//------------------------------------------------------------------------------
/// Add a polygon mesh (aSource) to another one (aDestination).
/// It corresponds to the operation as follows: aDestination += aSource.
/// Both aSource and aDestination have to already exist in the scenegraph.
/**
 *  @param aDestination:    the polygon mesh that is going to be modified
 *  @param aSource:         the polygon mesh that is going to be added
 *                          to aDestination
 */
//------------------------------------------------------------------------------
void addMesh(const std::string& aDestination, const std::string& aSource);


//------------------------------------------------------------------------------
/// Subtract a polygon mesh (aSource) from another one (aDestination).
/// It corresponds to the operation as follows: aDestination -= aSource.
/// Both aSource and aDestination have to already exist in the scenegraph.
/**
 *  @param aDestination:    the polygon mesh that is going to be modified
 *  @param aSource:         the polygon mesh that is going to be subtracted
 *                          from aDestination
 */
//------------------------------------------------------------------------------
void subtractMesh(const std::string& aDestination, const std::string& aSource);


//------------------------------------------------------------------------------
/// Get the number of children of a node in the scenegraph.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @return the number of chidren
 */
//------------------------------------------------------------------------------
unsigned int getNumberOfChildren(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Accessor on the i-th child's label in the list of children of a given node
/// in the scenegraph.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @param i:   the ID of the child
 *  @return the label corresponding to the i-th child
 */
//------------------------------------------------------------------------------
std::string getChildLabel(const std::string& aLabel, unsigned int i);


//------------------------------------------------------------------------------
/// Accessor on the local transformation matrix of a given node in
/// the scenegraph. If the node does not exist, the identity matrix is returned.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @return the corresponding local transformation matrix
 */
//------------------------------------------------------------------------------
std::vector<double> getLocalTransformationMatrix(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Set the local transformation matrix of a given node in the scenegraph.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @param aMatrix: the local transformation matrix
 */
//------------------------------------------------------------------------------
void setLocalTransformationMatrix(const std::string& aLabel,
    const std::vector<double>& aMatrix);


//------------------------------------------------------------------------------
/// Apply the current local transformation to all the vertices of a given node in the scenegraph.
/// Note the the local transformation is then reset to the identity matrix.
/**
 *  @param aLabel:  the label in the scenegraph
 */
//------------------------------------------------------------------------------
void applyCurrentLocalTransformation(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Access the bounding box of a given node and all its children (if any).
/// The bounding box is given in the world coordinate system.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @param aUnitOfLength:   the unit of length corresponding to the bounding box.
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
 *  @return the bounding box as: min_x, min_y, min_z, max_x, max_y, max_z
 */
//------------------------------------------------------------------------------
std::vector<double> getNodeAndChildrenBoundingBox(const std::string& aLabel,
    const std::string& aUnitOfLength = "cm");


//------------------------------------------------------------------------------
/// Access the bounding box of a given node (without its children).
/// The bounding box is given in the world coordinate system.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @param aUnitOfLength:   the unit of length corresponding to the bounding box.
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
 *  @return the bounding box as: min_x, min_y, min_z, max_x, max_y, max_z
 */
//------------------------------------------------------------------------------
std::vector<double> getNodeOnlyBoundingBox(const std::string& aLabel,
    const std::string& aUnitOfLength = "cm");


//------------------------------------------------------------------------------
/// Invert the normal vectors of a given polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 */
//------------------------------------------------------------------------------
void invertNormalVectors(const std::string& aLabel);


//--------------------------------------------------------------------------
/// Check if the normal vectors have been inverted.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @return true if the normal vectors have been inverted; false otherwise.
 */
//--------------------------------------------------------------------------
bool hasNormalVectorsInverted(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Set the color of a given polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param R:   the red channel
 *  @param G:   the green channel
 *  @param B:   the blue channel
 *  @param A:   the alpha channel
 */
//------------------------------------------------------------------------------
void setColor(const std::string& aLabel, float R, float G, float B, float A);


//------------------------------------------------------------------------------
/// Set the colour of a given polygon mesh. It will be applied to the ambiant, 
/// diffuse and specular colours.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param R:   the red channel
 *  @param G:   the green channel
 *  @param B:   the blue channel
 *  @param A:   the alpha channel
 */
//------------------------------------------------------------------------------
void setColour(const std::string& aLabel, float R, float G, float B, float A);


//------------------------------------------------------------------------------
/// Set the ambiant colour of a given polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param R:   the red channel
 *  @param G:   the green channel
 *  @param B:   the blue channel
 *  @param A:   the alpha channel
 */
//------------------------------------------------------------------------------
void setAmbiantColour(const std::string& aLabel, float R, float G, float B, float A);


//------------------------------------------------------------------------------
/// Accessor on the ambient colour of the material of a given polygon mesh.
/**
 *  @return the ambient colour
 */
//------------------------------------------------------------------------------
std::vector<float> getAmbientColour(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Set the diffuse colour of a given polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param R:   the red channel
 *  @param G:   the green channel
 *  @param B:   the blue channel
 *  @param A:   the alpha channel
 */
//------------------------------------------------------------------------------
void setDiffuseColour(const std::string& aLabel, float R, float G, float B, float A);


//------------------------------------------------------------------------------
/// Accessor on the diffuse colour of the material of a given polygon mesh.
/**
 *  @return the diffuse colour
 */
//------------------------------------------------------------------------------
std::vector<float> getDiffuseColour(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Set the specular colour of a given polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param R:   the red channel
 *  @param G:   the green channel
 *  @param B:   the blue channel
 *  @param A:   the alpha channel
 */
//------------------------------------------------------------------------------
void setSpecularColour(const std::string& aLabel, float R, float G, float B, float A);


//------------------------------------------------------------------------------
/// Accessor on the specular colour of the material of a given polygon mesh.
/**
 *  @return the specular colour
 */
//------------------------------------------------------------------------------
std::vector<float> getSpecularColour(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Set the shininess of the material of a given polygon mesh.
/**
 *  @param: aShininessValue the shininess coefficient
 */
//------------------------------------------------------------------------------
void setShininess(const std::string& aLabel, float aShininessValue);


//------------------------------------------------------------------------------
/// Accessor on the shininess of the material of a given polygon mesh.
/**
 *  @return the shininess coefficient
 */
//------------------------------------------------------------------------------
float getShininess(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Add a polygon mesh, given its label, to the X-ray renderer as
/// an inner surface.
/**
 *  @param aLabel:  the label of the polygon mesh in the scenegraph
 */
//------------------------------------------------------------------------------
void addPolygonMeshAsInnerSurface(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Add a polygon mesh, given its label, to the X-ray renderer as
/// an outer surface.
/**
 *  @param aLabel:  the label of the polygon mesh in the scenegraph
 */
//------------------------------------------------------------------------------
void addPolygonMeshAsOuterSurface(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Empty the scenegraph. Note that it also empties the X-ray renderer
/// from all its meshes.
//------------------------------------------------------------------------------
void removePolygonMeshesFromSceneGraph();


//------------------------------------------------------------------------------
/// Empty the X-ray renderer from all its meshes.
/// Note that the meshes are kept in the scenegraph.
//------------------------------------------------------------------------------
void removePolygonMeshesFromXRayRenderer();


#endif
