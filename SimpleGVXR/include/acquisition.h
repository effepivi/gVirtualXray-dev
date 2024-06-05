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


#ifndef __SGVXR_Acquisition_h
#define __SGVXR_Acquisition_h


/**
********************************************************************************
*
*   @file       acquisition.h
*
*   @brief      Expose here all the functions of SimpleGVXR related to 
*               the X-ray image acquisition, inc. radiographs, path lenghts and CT.
*
*   @version    1.0
*
*   @date       08/05/2024
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
#include <vector>
#include <string>

#ifndef __SGVXR_Utils_h
#include "Utils.h"
#endif


//------------------------------------------------------------------------------
/// Compute the X-ray projection corresponding to the environment that
/// has previously been set, i.e.
/// - Detector position
/// - Detector orientation
/// - Detector size and resolution
/// - Source position
/// - Source shape
/// - Beam spectrum
/// - Scanned object geometries
/// - Scanned object material properties
/**
 *  @param anIntegrateEnergyFlag: if true the energy fluence is returned,
 *                                otherwise the number of photons is returned
 *                                (default value: true)
 *  @return the corresponding X-ray image
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > computeXRayImage(bool anIntegrateEnergyFlag = true);


//------------------------------------------------------------------------------
/// Compute the spectral images corresponding to the environment that
/// has previously been set, i.e.
/// - Detector position
/// - Detector orientation
/// - Detector size and resolution
/// - Source position
/// - Source shape
/// - Beam spectrum
/// - Scanned object geometries
/// - Scanned object material properties
/// Note that there will be one 2D image per energy bin.
/**
 *  @return the corresponding spectral images
 */
//------------------------------------------------------------------------------
std::vector<std::vector<std::vector<float> > > computeSpectralImage();


//------------------------------------------------------------------------------
/// Compute the L-buffer of a polygon mesh corresponding to the environment that
/// has previously been set, i.e.
/// - Detector position
/// - Detector orientation
/// - Detector size and resolution
/// - Source position
/// - Source shape
/// - Scanned object geometry
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @return the corresponding L-buffer
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > computeLBuffer(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Compute the sinogram corresponding to the environment that
/// has previously been set, i.e.
/// - Detector position
/// - Detector orientation
/// - Detector size and resolution
/// - Source position
/// - Source shape
/// - Beam spectrum
/// - Scanned object geometries
/// - Scanned object material properties
/// Note that the rotation vector is the same as the up-vector of the detector.
/**
 *  @param x:   the position of the rotation centre along the X-axis
 *  @param y:   the position of the rotation centre along the Y-axis
 *  @param z:   the position of the rotation centre along the Z-axis
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
 *  @param aNumberOfAngle:  the total number of projections in the sinogram
 *  @param anAngleOffset:   the angle in degrees between
 *                          two successive projections
 *  @return the corresponding sinogram
 */
//------------------------------------------------------------------------------
std::vector<std::vector<std::vector<float> > > computeSinogram(
    double x,
    double y,
    double z,
    const std::string& aUnitOfLength,
    unsigned int aNumberOfAngle,
    double anAngleOffset);


//------------------------------------------------------------------------------
/// Compute the projection set corresponding to the environment that
/// has previously been set, i.e.
/// - Detector position
/// - Detector orientation
/// - Detector size and resolution
/// - Source position
/// - Source shape
/// - Beam spectrum
/// - Scanned object geometries
/// - Scanned object material properties
/// Note that the rotation vector is the same as the up-vector of the detector.
/**
 *  @param x:   the position of the rotation centre along the X-axis
 *  @param y:   the position of the rotation centre along the Y-axis
 *  @param z:   the position of the rotation centre along the Z-axis
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
 *  @param aNumberOfAngle:  the total number of projections in the projection set
 *  @param anAngleOffset:   the angle in degrees between
 *                          two successive projections
 *  @return the corresponding projection set
 */
//------------------------------------------------------------------------------
std::vector<std::vector<std::vector<float> > > computeProjectionSet(
    double x,
    double y,
    double z,
    const std::string& aUnitOfLength,
    unsigned int aNumberOfAngle,
    double anAngleOffset);


//--------------------------------------------------------------------------
/// Compute the X-ray projections for a CT acquisition.
/**
 *  @param aProjectionOutputPath:   the path where the X-ray projections will be saved. 
 *                                  If the path is empty, the data will be stored in the main memory, but not saved on the disk.
 *                                  If the path is provided, the data will be saved on the disk, and the main memory released.
 *  @param aScreenshotOutputPath:   the path where the screenshots will be saved.
 *                                  If kept empty, not screenshot will be saved.
 *  @param aNumberOfProjections:    The total number of projections to simulate.
 *  @param aFirstAngle:             The rotation angle corresponding to the first projection.
 *  @param anIncludeLastAngleFlag:  A boolean flag to include or exclude the last angle. It is used to calculate the angular step between successive projections.
 *  @param aLastAngle:              The rotation angle corresponding to the last projection. Note that depending on the value of anIncludeLastAngleFlag, this angle may be included or excluded
 *  @param aNumberOfWhiteImagesInFlatField: The number of white images used to perform the flat-field correction. If zero, then no correction will be performed.
 *  @param aPositionOfCentreOfRotationX: The location of the rotation centre along the X-axis.
 *  @param aPositionOfCentreOfRotationY: The location of the rotation centre along the Y-axis.
 *  @param aPositionOfCentreOfRotationZ: The location of the rotation centre along the Z-axis.
 *  @param aUnitOfLength: The corresponding unit of length
 *  @param aAxisOfRotationX: The rotation axis along the X-axis.
 *  @param aAxisOfRotationY: The rotation axis along the Y-axis.
 *  @param aAxisOfRotationZ: The rotation axis along the Z-axis.
 *  @param anIntegrateEnergyFlag: if true the energy fluence is returned,
 *                                otherwise the number of photons is returned
 *                                (default value: true)
 *  @param aVerboseLevel:   The verbose level. If greater than zero, details will be outputed (default value = 0)
 */
//--------------------------------------------------------------------------
void computeCTAcquisition(const std::string& aProjectionOutputPath,
                          const std::string& aScreenshotOutputPath,
                          unsigned int aNumberOfProjections,
                          float aFirstAngle,
                          bool anIncludeLastAngleFlag,
                          float aLastAngle,
                          unsigned int aNumberOfWhiteImagesInFlatField,
                          float aPositionOfCentreOfRotationX,
                          float aPositionOfCentreOfRotationY,
                          float aPositionOfCentreOfRotationZ,
                          const std::string& aUnitOfLength,
                          float aAxisOfRotationX,
                          float aAxisOfRotationY,
                          float aAxisOfRotationZ,
                          bool anIntegrateEnergyFlag = true,
                          unsigned int aVerboseLevel = 0
                          );


//--------------------------------------------------------------------------
/// Accessor on the set of successive source positions during the CT scan acquisition.
/**
 *  @return the set of successive source positions
 */
//--------------------------------------------------------------------------
std::vector<std::vector<float>> getSourcePositionSet(const std::string& aUnitOfLength);


//--------------------------------------------------------------------------
/// Accessor on the set of successive detector centre positions during the CT scan acquisition.
/**
 *  @return the set of successive detector centre positions
 */
//--------------------------------------------------------------------------
std::vector<std::vector<float>> getDetectorPositionSet(const std::string& aUnitOfLength);


//--------------------------------------------------------------------------
/// Accessor on the set of successive unit vector that defines the horizontal axis of the detector.
/**
 *  @return the set of successive unit vector that defines the horizontal axis of the detector
 */
//--------------------------------------------------------------------------
std::vector<std::vector<float>> getDetectorRightVectorSet();


//--------------------------------------------------------------------------
/// Accessor on the set of successive unit vector that defines the vertical axis of the detector.
/**
 *  @return the set of successive unit vector that defines the vertical axis of the detector
 */
//--------------------------------------------------------------------------
std::vector<std::vector<float>> getDetectorUpVectorSet();


//--------------------------------------------------------------------------
/// Accessor on the path where X-ray projections are saved during a CT scan 
/// acquisition simulation.
/**
 *  @return the path where X-ray projections are saved
 */
//--------------------------------------------------------------------------
const std::string& getProjectionOutputPathCT();


//--------------------------------------------------------------------------
/// Accessor on the path where screenshots are saved during a CT scan 
/// acquisition simulation.
/**
 *  @return the path where screenshots are saved
 */
//--------------------------------------------------------------------------
const std::string& getScreenshotOutputPathCT();


//--------------------------------------------------------------------------
/// Accessor on the number of X-ray projections computed during a CT scan 
/// acquisition simulation.
/**
 *  @return the number of X-ray projections
 */
//--------------------------------------------------------------------------
unsigned int getNumberOfProjectionsCT();



//--------------------------------------------------------------------------
/// Accessor on the number of X-ray projections needed to compute a CT scan 
/// acquisition that satisfies the Nyquist theorem. Note that the user 
/// may choose a different number.
/**
 *  @return the optimal number of X-ray projections
 */
//--------------------------------------------------------------------------
unsigned int getOptimalNumberOfProjectionsCT();


//--------------------------------------------------------------------------
/// Accessor on the first angle of the CT scan acquisition simulation.
/**
 *  @return the first angle
 */
//--------------------------------------------------------------------------
float getFirstAngleCT();


//--------------------------------------------------------------------------
/// Accessor on the last angle of the CT scan acquisition simulation.
/**
 *  @return the last angle
 */
//--------------------------------------------------------------------------
float getLastAngleCT();


//--------------------------------------------------------------------------
/// Accessor on the flag used to include the last angle or not during 
/// the CT scan acquisition simulation.
/**
 *  @return the flag used to include the last angle or not
 */
//--------------------------------------------------------------------------
bool getIncludeLastAngleFlagCT();


//--------------------------------------------------------------------------
/// Accessor on the number of white images used in the flat-field correction
/// during the CT scan acquisition simulation. Zero means that 
/// the flat-field correction was not used.
/**
 *  @return the number of white images
 */
//--------------------------------------------------------------------------
unsigned int getWhiteImagesInFlatFieldCT();


//--------------------------------------------------------------------------
/// Accessor on the position of the centre of rotation used during 
/// the CT scan acquisition simulation.
/**
 *  @param aUnitOfLength: The corresponding unit of length
 *  @return the position of the centre of rotation
 */
//--------------------------------------------------------------------------
std::vector<float> getCentreOfRotationPositionCT(const std::string& aUnitOfLength);


//--------------------------------------------------------------------------
/// Accessor on the axis of rotation used during the CT scan 
/// acquisition simulation.
/**
 *  @return the axis of rotation
 */
//--------------------------------------------------------------------------
std::vector<float> getRotationAxisCT();


//--------------------------------------------------------------------------
/// Accessor on the set of rotation angles used during the CT scan 
/// acquisition simulation.
/**
 *  @return the set of rotation angles
 */
//--------------------------------------------------------------------------
std::vector<float> getAngleSetCT();


//------------------------------------------------------------------------------
/// Accessor on the last X-ray image that has been computed.
/**
 *  @return the last X-ray image
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > getLastXRayImage();


//------------------------------------------------------------------------------
/// Accessor on the last L-buffer that has been computed.
/**
 *  @return the last L-buffer
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > getLastLBuffer();


//------------------------------------------------------------------------------
/// Accessor on the last sinogram that has been computed.
/**
 *  @return the last sinogram
 */
//------------------------------------------------------------------------------
std::vector<std::vector<std::vector<float> > > getLastSinogram();


//------------------------------------------------------------------------------
/// Accessor on the last projection set that has been computed.
/**
 *  @return the last projection set
 */
//------------------------------------------------------------------------------
std::vector<std::vector<std::vector<float> > > getLastProjectionSet();


#endif
