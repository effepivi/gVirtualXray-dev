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


#ifndef __SGVXR_Source_h
#define __SGVXR_Source_h


/**
********************************************************************************
*
*   @file       source.h
*
*   @brief      Expose here all the functions of SimpleGVXR related to 
*               the X-ray source. It includes the beam spectrum.
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




//------------------------------------------------------------------------------
/// Set the position of the X-ray source.
/**
 *  @param x:   the position along the X-axis
 *  @param y:   the position along the Y-axis
 *  @param z:   the position along the Z-axis
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
void setSourcePosition(double x,
    double y,
    double z,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Accessor on the shape of the X-ray source.
/*
 *  @return the shape of the X-ray source (e.g. POINT, PARALLEL, SQUARE, CUBE, LINE,  UNKNOWN)
 */
//------------------------------------------------------------------------------
std::string getSourceShape();


//------------------------------------------------------------------------------
/// Accessor on the position of the X-ray source.
/**
 *  @param aUnitOfLength:   the unit of length corresponding to
 *                          the returned value. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *  @return the source position in a given unit of length
 */
//------------------------------------------------------------------------------
std::vector<double> getSourcePosition(const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Use a point source, i.e. a cone-beam geometry.
//------------------------------------------------------------------------------
void usePointSource();


//------------------------------------------------------------------------------
/// Use a cone-beam geometry, i.e. a point source.
//------------------------------------------------------------------------------
void useConeBeam();


//------------------------------------------------------------------------------
/// Use a parallel source (e.g. to mimic a source that is extremely far from
/// the scanned object and detector).
//------------------------------------------------------------------------------
void useParallelBeam();


//------------------------------------------------------------------------------
/// Use a parallel source (e.g. to mimic a source that is extremely far from
/// the scanned object and detector).
//------------------------------------------------------------------------------
void useParallelSource();



//------------------------------------------------------------------------------
/// Check if the beam shape is parallel (e.g. to mimic a source that is 
/// extremely far from the scanned object and detector).
/**
 *  @return true if the beam shape is parallel, false otherwise
 */
//------------------------------------------------------------------------------
bool isUsingParallelBeam();


//------------------------------------------------------------------------------
/// Check if the beam shape is parallel (e.g. to mimic a source that is 
/// extremely far from the scanned object and detector).
/**
 *  @return true if the beam shape is parallel, false otherwise
 */
//------------------------------------------------------------------------------
bool isUsingParallelSource();


//------------------------------------------------------------------------------
/// Check if the beam shape is cone-beam and the source an infinitively 
/// small point.
/**
 *  @return true if an infinitively small point source is used, false otherwise
 */
//------------------------------------------------------------------------------
bool isUsingPointSource();


//------------------------------------------------------------------------------
/// Check if the beam shape is cone-beam. Note that an infinitively small point
/// source or a finite size focal spot may have been used.
/**
 *  @return true if a cone-beam shape is used, false otherwise
 */
//------------------------------------------------------------------------------
bool isUsingConeBeam();


//------------------------------------------------------------------------------
/// Check if the beam shape is cone-beam and the source a finite size 
/// focal spot.
/**
 *  @return true if a finite size focal spot is used, false otherwise
 */
//------------------------------------------------------------------------------
bool isUsingFocalSpot();


//------------------------------------------------------------------------------
/// Set the size of the focal spot of the X-ray generator. A cube shape is used.
/// The volume of the cube is sampled by i^3 point sources.
/**
 *  @param x:   the position along the X-axis
 *  @param y:   the position along the Y-axis
 *  @param z:   the position along the Z-axis
 *  @param l:   the lenght of the cube along the 3 axes
 *  @param aUnitOfLength:   the unit of length. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *  @param i:   the number of point souces used along each axes.
 */
//------------------------------------------------------------------------------
void setFocalSpot(float x, float y, float z, float l, const std::string& aUnitOfLength,
  int i);


//------------------------------------------------------------------------------
/// Set the size of the focal spot of the X-ray generator.
/// A square shape is used. The area of the square is sampled by
/// i*i point sources.
/**
 *  @param x:   the position along the X-axis
 *  @param y:   the position along the Y-axis
 *  @param z:   the position along the Z-axis
 *  @param l:   the length of the square
 *  @param aUnitOfLength:   the unit of length. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *  @param i:   the number of point souces used along each axes.
 */
//------------------------------------------------------------------------------
void setFocalSpotAsSquare(float x, float y, float z,
    float l,
    const std::string& aUnitOfLength,
    int i);


//------------------------------------------------------------------------------
/// Set the size of the focal spot of the X-ray generator.
/// A rectangle shape is used. The area of the rectangle is sampled by
/// i*j point sources.
/**
 *  @param x:   the position along the X-axis
 *  @param y:   the position along the Y-axis
 *  @param z:   the position along the Z-axis
 *  @param l:   the length of the rectangle along the axis defined by
 *              the cross product between the source-detector centres and
 *              the detector up-vector
 *  @param k:   the height of the source along the detector up-vector direction
 *  @param aUnitOfLength:   the unit of length. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *  @param i:   the number of point sources along the axis defined by the cross
 *              product between the source-detector centres and
 *              the detector up-vector.
 *  @param j:   the number of point sources along the detector up-vector direction.
 */
//------------------------------------------------------------------------------
void setFocalSpotAsRectangle(float x, float y, float z,
    float l, float k,
    const std::string& aUnitOfLength,
    int i, int j);


//--------------------------------------------------------------------------
/// Rotate the focal spot around its centre.
/**
*  @param aRotationAngle: the rotation angle in degrees
*  @param aRotationAxis: the rotaton axis
*/
//--------------------------------------------------------------------------
void rotateFocalSpot(const double& aRotationAngle, double u, double v, double w);


//--------------------------------------------------------------------------
/// Rotate the focal spot around an arbitrary point.
/**
*  @param aRotationAngle: the rotation angle in degrees
*  @param aRotationCentre: the rotaton centre
*  @param aRotationAxis: the rotaton axis
*/
//--------------------------------------------------------------------------
void rotateSource(const double& aRotationAngle,
    double x, double y, double z,
    double u, double v, double w);


//------------------------------------------------------------------------------
/// Empty the beam spectrum.
/**
*  @param aClearTubeParameterFlag: if the flag is true, then the X-ray tubes parameters are reset too
*/
//------------------------------------------------------------------------------
void resetBeamSpectrum(bool aClearTubeParameterFlag = true);


//------------------------------------------------------------------------------
/// Use a monochromatic beam spectrum (i.e. one single energy).
/**
 *  @param anEnergy:            the incident photon energy
 *  @param aUnitOfEnergy:       the unit of energy corresponding to anEnergy.
 *                              Acceptable values are:
 *                              "electronvolt", "eV",
 *                              "kiloelectronvolt", "keV",
 *                              "megaelectronvolt", "MeV"
 *  @param aNumberOfPhotons:    the number of incident photons
 */
//------------------------------------------------------------------------------
void setMonoChromatic(double anEnergy,
                      const std::string& aUnitOfEnergy,
                      double aNumberOfPhotons);


//------------------------------------------------------------------------------
/// Add an energy bin to the beam spectrum (useful to generate polychromatism).
/**
 *  @param anEnergy:            the incident photon energy
 *  @param aUnitOfEnergy:       the unit of energy corresponding to anEnergy.
 *                              Acceptable values are:
 *                              "electronvolt", "eV",
 *                              "kiloelectronvolt", "keV",
 *                              "megaelectronvolt", "MeV"
 *  @param aNumberOfPhotons:    the number of incident photons
 */
//------------------------------------------------------------------------------
void addEnergyBinToSpectrum(double anEnergy,
                            const std::string& aUnitOfEnergy,
                            double aNumberOfPhotons);


//------------------------------------------------------------------------------
/// Load the incident beam energy spectrum from a TSV file.
/**
 *  @param aFileName:           name of the file to load
 *  @param aUnitOfEnergy:       the unit of energy corresponding to anEnergy.
 *                              Acceptable values are:
 *                              "electronvolt", "eV",
 *                              "kiloelectronvolt", "keV",
 *                              "megaelectronvolt", "MeV"
 *  @param aNormalisationFlag:  true to normalise the total energy to 1,
 *                              false otherwise
 */
//------------------------------------------------------------------------------
void loadSpectrumFromTSV(const std::string& aFileName,
                         const std::string& aUnitOfEnergy,
                         bool aNormalisationFlag);


//------------------------------------------------------------------------------
/// Accessor on the energy bins of the beam spectrum.
/**
 *  @param aUnitOfEnergy:   the unit of energy corresponding to
 *                          the returned vector. Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @return the list of energy bins in aUnitOfEnergy
 */
//------------------------------------------------------------------------------
std::vector<double> getEnergyBins(const std::string& aUnitOfEnergy);


//------------------------------------------------------------------------------
/// Accessor on the number of photons per energy bin of the beam spectrum.
/**
 *  @return the list of number of photons
 */
//------------------------------------------------------------------------------
std::vector<double> getPhotonCountEnergyBins();


//--------------------------------------------------------------------------
/// Accessor on a total energy of the beam when the detector response is applied.
/**
 *  @return the total energy
 */
//--------------------------------------------------------------------------
double getTotalEnergyWithDetectorResponse();


void setPhotonCountingBands(double aMinEnergy,
    double aMaxEnergy,
    const std::string& aUnitOfEnergy,
    unsigned int aNumberOfBands);


unsigned int getNumberOfPhotonCountingBands();
void mergePhotonCountingBands();
std::vector<double> getPhotonCountingBands();


void setVoltage(const float& aVoltage, const std::string& aUnitOfElectriPotential);
float getVoltage(const std::string& aUnitOfElectriPotential);

void setmAs(const float& an_mAs);
float getmAs();

void setTubeAngle(const float& anAngle);
float getTubeAngle();


void clearFiltration();
void setFiltration(const std::vector<std::pair<unsigned int, float> >& aSetOfFilters,
    const std::string& aUnitOfLength);

void addFilter(const std::string& aMaterial, const double& aThickness, const std::string& aUnitOfLength);
void addFilter(unsigned int aMaterial, const double& aThickness, const std::string& aUnitOfLength);
std::vector<unsigned int> getFiltrationMaterial();
std::vector<float> getFiltrationThickness(const std::string& aUnitOfLength);

bool hasSpekpy();
bool hasXpecgen();
void useSpekpy();
void useXpecgen();
bool usingSpekpy();
bool usingXpecgen();


//--------------------------------------------------------------------------
/// Enable Poisson noise for the energy fluence.
//--------------------------------------------------------------------------
void enablePoissonNoise();


//--------------------------------------------------------------------------
/// Disable Poisson noise for the energy fluence.
//--------------------------------------------------------------------------
void disablePoissonNoise();


//--------------------------------------------------------------------------
/// Set the total number of photons per pixel.
/**
*  @param aNumberOfPhotons: the number of photons
*/
//--------------------------------------------------------------------------
void setNumberOfPhotons(unsigned int aNumberOfPhotons);


//--------------------------------------------------------------------------
/// Set the total number of photons per pixel.
/**
*  @param aNumberOfPhotons: the number of photons
*/
//--------------------------------------------------------------------------
void setNumberOfPhotonsPerPixel(unsigned int aNumberOfPhotons);


//--------------------------------------------------------------------------
/// Set the total number of photons per square centimetre.
/**
*  @param aNumberOfPhotons: the number of photons
*/
//--------------------------------------------------------------------------
void setNumberOfPhotonsPerCM2(double aNumberOfPhotons);


//--------------------------------------------------------------------------
/// Accessor on the total number of photons per pixel.
/**
*  @return the number of photons
*/
//--------------------------------------------------------------------------
unsigned int getNumberOfPhotons();


//--------------------------------------------------------------------------
/// Accessor on the total number of photons per pixel.
/**
*  @return the number of photons
*/
//--------------------------------------------------------------------------
unsigned int getNumberOfPhotonsPerPixel();


//--------------------------------------------------------------------------
/// Accessor on the total number of photons per square centimetre.
/**
*  @return the number of photons
*/
//--------------------------------------------------------------------------
unsigned int getNumberOfPhotonsPerCM2();


//--------------------------------------------------------------------------
/// Enable or disable Poisson noise for the energy fluence.
/**
*  @param aFlag: if true, then enable Poisson noise. If false, then disable it
*/
//--------------------------------------------------------------------------
void usePoissonNoise(bool aFlag);


//--------------------------------------------------------------------------
/// Check if Poisson noise for the energy fluence is enable or disable.
/**
*  @return true if Poisson noise is enable, or false if Poisson noise is disable
*/
//--------------------------------------------------------------------------
bool usePoissonNoise();

#endif
