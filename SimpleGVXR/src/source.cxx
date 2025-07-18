/**
********************************************************************************
*
*   @file       source.cxx
*
*   @brief      Expose here all the functions of SimpleGVXR related to 
*               the X-ray source. It includes the beam spectrum.
*
*   @version    1.0
*
*   @date       14/05/2025
*
*   @author     Prof Franck P. Vidal
*
*   @todo       The use of PythonLibs is disabled by default. All the functions related to the setting of the spectrum using voltage, mAs, tube angle and beam filtration may not have any visible effect.
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
#include <map>

#ifndef __Logger_h
#include "gVirtualXRay/Logger.h"
#endif

#ifndef __XRayBeam_h
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef __XRayDetector_h
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef __XRayRenderer_h
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef __Logger_h
#include <gVirtualXRay/Logger.h>
#endif

#ifndef __SGVXR_Internal_h
#include "internal.h"
#endif

#ifndef __SGVXR_Utils_h
#include "Utils.h"
#endif

#ifndef __SGVXR_Detector_h
#include "detector.h"
#endif

#ifndef __SGVXR_Source_h
#include "source.h"
#endif


//******************************************************************************
//  Global variables
//******************************************************************************
extern std::map<std::string, unsigned int> g_warning_message_counters;
extern gVirtualXRay::XRayDetector g_xray_detector; // X-ray detector
extern gVirtualXRay::XRayRenderer g_xray_renderer; // X-ray graphics engine

gVirtualXRay::XRayBeam g_xray_beam;         // X-ray beam

bool g_beam_shape_initialised = false;
bool g_beam_energy_initialised = false;
bool g_source_position_initialised = false;


//------------------------------------------------------
void setSourcePosition(double x,
                       double y,
                       double z,
                       const std::string& aUnitOfLength)
//------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    // Save the current X-ray source shape
    gVirtualXRay::XRayDetector::SourceShape source_shape_backup = g_xray_detector.getSourceShape();

    // Update the position
    g_xray_detector.setXrayPointSource(
            gVirtualXRay::VEC3(x * unit_of_length, y * unit_of_length,
                    z * unit_of_length));

    // The position has been initialised
    g_source_position_initialised = true;

    // Restore the X-ray source shape
    switch (source_shape_backup)
    {
      case gVirtualXRay::XRayDetector::POINT:
        g_xray_detector.setXrayPointSource();
        break;

      case gVirtualXRay::XRayDetector::PARALLEL:
        g_xray_detector.setParallelBeam();
        break;

      case gVirtualXRay::XRayDetector::SQUARE:
        throw gVirtualXRay::Exception(__FILE__,
                                      __FUNCTION__,
                                      __LINE__,
                                      "This method should not be used with a square source, call `setFocalSpotAsSquare` instead.");

      case gVirtualXRay::XRayDetector::RECTANGLE:
        throw gVirtualXRay::Exception(__FILE__,
                                      __FUNCTION__,
                                      __LINE__,
                                      "This method should not be used with a square source, call `setFocalSpotAsRectangle` instead.");

      case gVirtualXRay::XRayDetector::CUBE:
        throw gVirtualXRay::Exception(__FILE__,
                                      __FUNCTION__,
                                      __LINE__,
                                      "This method should not be used with a cube source, call `setFocalSpot` instead");

      case gVirtualXRay::XRayDetector::LINE:
        throw gVirtualXRay::Exception(__FILE__,
                                      __FUNCTION__,
                                      __LINE__,
                                      "This method should not be used with a line source.");

      default: // This case should never occur, but just in case
        g_xray_detector.setXrayPointSource();
    }

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}


//--------------------------
std::string getSourceShape()
//--------------------------
{
    switch (g_xray_detector.getSourceShape())
    {
    case gVirtualXRay::XRayDetector::POINT:
        return std::string("POINT");

    case gVirtualXRay::XRayDetector::PARALLEL:
        return std::string("PARALLEL");

    case gVirtualXRay::XRayDetector::SQUARE:
        return std::string("SQUARE");

    case gVirtualXRay::XRayDetector::CUBE:
        return std::string("CUBE");

    case gVirtualXRay::XRayDetector::LINE:
        return std::string("LINE");

    default: // This case should never occur, but just in case
        return std::string("UNKNOWN");
    }
}


//---------------------------------------------------------------------
std::vector<double> getSourcePosition(const std::string& aUnitOfLength)
//---------------------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    std::vector<double> position(3);

    position[0] = g_xray_detector.getXraySourceCentre()[0] / unit_of_length;
    position[1] = g_xray_detector.getXraySourceCentre()[1] / unit_of_length;
    position[2] = g_xray_detector.getXraySourceCentre()[2] / unit_of_length;

    return (position);
}


//-------------------
void usePointSource()
//-------------------
{
    g_xray_detector.setXrayPointSource();
    g_beam_shape_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}


//----------------
void useConeBeam()
//----------------
{
    usePointSource();
}


//--------------------
void useParallelBeam()
//--------------------
{
    // Use a parallel source
    g_xray_detector.setParallelBeam();
    g_beam_shape_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}


//----------------------
void useParallelSource()
//----------------------
{
    useParallelBeam();
}


//------------------------
bool isUsingParallelBeam()
//------------------------
{
    gVirtualXRay::XRayDetector::SourceShape source_shape = g_xray_detector.getSourceShape();

    return source_shape == gVirtualXRay::XRayDetector::PARALLEL;
}


//--------------------------
bool isUsingParallelSource()
//--------------------------
{
    return isUsingParallelBeam();
}


//-----------------------
bool isUsingPointSource()
//-----------------------
{
    gVirtualXRay::XRayDetector::SourceShape source_shape = g_xray_detector.getSourceShape();

    return source_shape == gVirtualXRay::XRayDetector::POINT;
}


//--------------------
bool isUsingConeBeam()
//--------------------
{
    gVirtualXRay::XRayDetector::SourceShape source_shape = g_xray_detector.getSourceShape();

    return source_shape == gVirtualXRay::XRayDetector::POINT ||
        source_shape == gVirtualXRay::XRayDetector::SQUARE || 
        source_shape == gVirtualXRay::XRayDetector::RECTANGLE || 
        source_shape == gVirtualXRay::XRayDetector::CUBE ||
        source_shape == gVirtualXRay::XRayDetector::LINE;
}


//---------------------
bool isUsingFocalSpot()
//---------------------
{
    gVirtualXRay::XRayDetector::SourceShape source_shape = g_xray_detector.getSourceShape();

    return source_shape == gVirtualXRay::XRayDetector::SQUARE || 
        source_shape == gVirtualXRay::XRayDetector::RECTANGLE || 
        source_shape == gVirtualXRay::XRayDetector::CUBE ||
        source_shape == gVirtualXRay::XRayDetector::LINE;
}


//--------------------------------------------------------------------------------------------
void setFocalSpot(float x, float y, float z, float l, const std::string& aUnitOfLength, int i)
//--------------------------------------------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    if (i % 2 == 0)
    {
        gVirtualXRay::LOGGER.logWarning("i (") << i << ") must be an odd number, " <<
            i+1 << " will be used instead." << std::endl;
    }

    g_xray_detector.setCubicSource(gVirtualXRay::VEC3(x * unit_of_length, y * unit_of_length, z * unit_of_length),
                      i,
                      l * unit_of_length);

    g_beam_shape_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}


//---------------------------------------------------------
void setFocalSpotAsSquare(float x, float y, float z,
                          float l,
                          const std::string& aUnitOfLength,
                          int i)
//---------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    if (i % 2 == 0)
    {
        gVirtualXRay::LOGGER.logWarning("i (") << i << ") must be an odd number, " <<
            i+1 << " will be used instead." << std::endl;
    }

    g_xray_detector.setSquareSource(gVirtualXRay::VEC3(x * unit_of_length, y * unit_of_length, z * unit_of_length),
                      i,
                      l * unit_of_length);

    g_beam_shape_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}


//------------------------------------------------------------
void setFocalSpotAsRectangle(float x, float y, float z,
                             float l, float k,
                             const std::string& aUnitOfLength,
                             int i, int j)
//------------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    if (i % 2 == 0)
    {
        gVirtualXRay::LOGGER.logWarning("i (") << i << ") must be an odd number, " <<
            i+1 << " will be used instead." << std::endl;
    }

    if (j % 2 == 0)
    {
        gVirtualXRay::LOGGER.logWarning("j (") << j << ") must be an odd number, " <<
            j+1 << " will be used instead." << std::endl;
    }

    g_xray_detector.setRectangleSource(gVirtualXRay::VEC3(x * unit_of_length, y * unit_of_length, z * unit_of_length),
                      i, j,
                      l * unit_of_length,
                      k * unit_of_length);

    g_beam_shape_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}


//------------------------------------------------------------------------------
void rotateFocalSpot(const double& aRotationAngle, double u, double v, double w)
//------------------------------------------------------------------------------
{
    g_xray_detector.rotateFocalSpot(aRotationAngle, gVirtualXRay::VEC3(u, v, w));
}


//---------------------------------------------
void rotateSource(const double& aRotationAngle,
                  double x, double y, double z,
                  double u, double v, double w)
//---------------------------------------------
{
    g_xray_detector.rotateSource(aRotationAngle,
        gVirtualXRay::VEC3(x, y, z),
        gVirtualXRay::VEC3(u, v, w));
}


//--------------------------------------------------
void resetBeamSpectrum(bool aClearTubeParameterFlag)
//--------------------------------------------------
{
    if (aClearTubeParameterFlag) g_xray_beam.clear();
    else g_xray_beam.clearSpectrumOnly();

    g_beam_energy_initialised = false;
}

//-----------------------------------------------------
void setMonoChromatic(double anEnergy,
                      const std::string& aUnitOfEnergy,
                      double aNumberOfPhotons)
//-----------------------------------------------------
{
    if (!g_warning_message_counters.count("setMonoChromatic"))
    {
        gVirtualXRay::LOGGER.logWarning(std::string(__FUNCTION__) + 
            " is deprecated. It will be removed in a future release. "
            "Choose setMonoChromaticPerPixelAtSDD or setMonoChromaticPerCm2At1m. "
            "Note that setMonoChromatic and setMonoChromaticPerPixelAtSDD are equivalent.") << std::endl;

        g_warning_message_counters["setMonoChromatic"] = 1;
    }

    setMonoChromaticPerPixelAtSDD(anEnergy, aUnitOfEnergy, aNumberOfPhotons);
}


//------------------------------------------------------------------
void setMonoChromaticPerPixelAtSDD(double anEnergy,
                                   const std::string& aUnitOfEnergy,
                                   double aNumberOfPhotons)
//------------------------------------------------------------------
{
    g_xray_beam.clear();
    addEnergyBinToSpectrumPerPixelAtSDD(anEnergy, aUnitOfEnergy, aNumberOfPhotons);
}


//---------------------------------------------------------------
void setMonoChromaticPerCm2At1m(double anEnergy,
                                const std::string& aUnitOfEnergy,
                                double aNumberOfPhotons)
//---------------------------------------------------------------
{
    g_xray_beam.clear();
    addEnergyBinToSpectrumPerCm2At1m(anEnergy, aUnitOfEnergy, aNumberOfPhotons);
}


//-----------------------------------------------------------
void addEnergyBinToSpectrum(double anEnergy,
                            const std::string& aUnitOfEnergy,
                            double aNumberOfPhotons)
//-----------------------------------------------------------
{
    if (!g_warning_message_counters.count("addEnergyBinToSpectrum"))
    {
        gVirtualXRay::LOGGER.logWarning(std::string(__FUNCTION__) + 
            " is deprecated. It will be removed in a future release. "
            "Choose addEnergyBinToSpectrumPerPixelAtSDD or addEnergyBinToSpectrumPerCm2At1m. "
            "Note that addEnergyBinToSpectrum and addEnergyBinToSpectrumPerPixelAtSDD are equivalent.") << std::endl;

        g_warning_message_counters["addEnergyBinToSpectrum"] = 1;
    }

    addEnergyBinToSpectrumPerPixelAtSDD(anEnergy, aUnitOfEnergy, aNumberOfPhotons);
}


//------------------------------------------------------------------------
void addEnergyBinToSpectrumPerPixelAtSDD(double anEnergy,
                                         const std::string& aUnitOfEnergy,
                                         double aNumberOfPhotons)
//------------------------------------------------------------------------
{
    if (g_xray_beam.addChannelPerPixelAtSDD(aNumberOfPhotons,
        anEnergy * getUnitOfEnergy(aUnitOfEnergy)))
    {
        g_beam_energy_initialised = true;

        // Initialise the X-ray renderer if needed and if possible
        initialiseXRayRenderer();
    }
}


//---------------------------------------------------------------------
void addEnergyBinToSpectrumPerCm2At1m(double anEnergy,
                                      const std::string& aUnitOfEnergy,
                                      double aNumberOfPhotons)
//---------------------------------------------------------------------
{
    if (g_xray_beam.addChannelPerCm2At1m(aNumberOfPhotons,
        anEnergy * getUnitOfEnergy(aUnitOfEnergy)))
    {
        g_beam_energy_initialised = true;

        // Initialise the X-ray renderer if needed and if possible
        initialiseXRayRenderer();
    }
}


//--------------------------------------------------------
void loadSpectrumFromTSV(const std::string& aFileName,
    const std::string& aUnitOfEnergy, bool aNormalisationFlag)
//--------------------------------------------------------
{
    if (!g_warning_message_counters.count("loadSpectrumFromTSV"))
    {
        gVirtualXRay::LOGGER.logWarning(std::string(__FUNCTION__) + 
            " is deprecated. It will be removed in a future release. "
            "Choose loadSpectrumFromTSVPerPixelAtSDD or loadSpectrumFromTSVPerCm2At1m. "
            "Note that loadSpectrumFromTSV and loadSpectrumFromTSVPerPixelAtSDD are equivalent.") << std::endl;

        g_warning_message_counters["loadSpectrumFromTSV"] = 1;
    }

    loadSpectrumFromTSVPerPixelAtSDD(aFileName, aUnitOfEnergy, aNormalisationFlag);
}


//---------------------------------------------------------------------
void loadSpectrumFromTSVPerPixelAtSDD(const std::string& aFileName,
                                      const std::string& aUnitOfEnergy,
                                      bool aNormalisationFlag)
//---------------------------------------------------------------------
{
    g_xray_beam.clear();

    g_xray_beam.initialisePerPixelAtSDD(aFileName.c_str(), 
        getUnitOfEnergy(aUnitOfEnergy));

    if (aNormalisationFlag) {
        g_xray_beam.normalise();
    }

    g_beam_energy_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}


//------------------------------------------------------------------
void loadSpectrumFromTSVPerCm2At1m(const std::string& aFileName,
                                   const std::string& aUnitOfEnergy,
                                   bool aNormalisationFlag)
//------------------------------------------------------------------
{
    g_xray_beam.clear();

    g_xray_beam.initialisePerCm2At1m(aFileName.c_str(), 
        getUnitOfEnergy(aUnitOfEnergy));

    if (aNormalisationFlag) {
        g_xray_beam.normalise();
    }

    g_beam_energy_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}


//-----------------------------------------------------------------
std::vector<double> getEnergyBins(const std::string& aUnitOfEnergy)
//-----------------------------------------------------------------
{
    std::vector<double> p_temp;

    for (unsigned int i = 0; i < g_xray_beam.getEnergyChannelNumber(); ++i) {
        p_temp.push_back(
        g_xray_beam.getEnergyChannelPerCm2At1m(i, &g_xray_detector).getPhotonEnergy()
        / getUnitOfEnergy(aUnitOfEnergy));
    }

    return (p_temp);
}


//--------------------------------------------
std::vector<double> getPhotonCountEnergyBins()
//--------------------------------------------
{
    gVirtualXRay::LOGGER.logWarning(std::string(__FUNCTION__) + 
        " is deprecated. It will be removed in a future release. "
        "Choose getPhotonCountsPerPixelAtSDD or getPhotonCountsPerCm2At1m. "
        "Note that getEnergyChannel and getPhotonCountsPerPixelAtSDD are equivalent.") << std::endl;

    return getPhotonCountsPerPixelAtSDD();
}


//--------------------------------------------
std::vector<double> getPhotonCountsPerCm2At1m()
//--------------------------------------------
{
    std::vector<double> p_temp;

    for (unsigned int i = 0; i < g_xray_beam.getEnergyChannelNumber(); ++i) {
        p_temp.push_back(g_xray_beam.getEnergyChannelPerCm2At1m(i, &g_xray_detector).getPhotonNumber());
    }

    return (p_temp);
}


//------------------------------------------------
std::vector<double> getPhotonCountsPerPixelAtSDD()
//------------------------------------------------
{
    std::vector<double> p_temp;

    for (unsigned int i = 0; i < g_xray_beam.getEnergyChannelNumber(); ++i) {
        p_temp.push_back(g_xray_beam.getEnergyChannelPerPixelAtSDD(i, &g_xray_detector).getPhotonNumber());
    }

    return (p_temp);
}


//-----------------------------------------
double getTotalEnergyWithDetectorResponse()
//-----------------------------------------
{
    return g_xray_renderer.getTotalEnergyWithDetectorResponse();
}


//--------------------------------------------
double getTotalEnergyWithoutDetectorResponse()
//--------------------------------------------
{
    return g_xray_renderer.getTotalEnergyWithoutDetectorResponse();
}


//--------------------------------------------------------------------------------
void setVoltage(const float& aVoltage, const std::string& aUnitOfElectriPotential)
//--------------------------------------------------------------------------------
{
    g_xray_beam.setVoltage(aVoltage * getUnitOfElectriPotential(aUnitOfElectriPotential));
}


//----------------------------------------------------------
float getVoltage(const std::string& aUnitOfElectriPotential)
//----------------------------------------------------------
{
    return g_xray_beam.getVoltage() / getUnitOfElectriPotential(aUnitOfElectriPotential);
}


//------------------------------
void setmAs(const float& an_mAs)
//------------------------------
{
    g_xray_beam.setmAs(an_mAs);
}


//------------
float getmAs()
//------------
{
    return g_xray_beam.getmAs();
}


//-------------------------------------
void setTubeAngle(const float& anAngle)
//-------------------------------------
{
    g_xray_beam.setTubeAngle(anAngle);
}


//------------------
float getTubeAngle()
//------------------
{
    return g_xray_beam.getTubeAngle();
}


//--------------------
void clearFiltration()
//--------------------
{
    g_xray_beam.clearFiltration();
}


//-----------------------------------------------------------------------------------
void setFiltration(const std::vector<std::pair<unsigned int, float> >& aSetOfFilters,
                    const std::string& aUnitOfLength)
//-----------------------------------------------------------------------------------
{
    std::vector<std::pair<unsigned int, gVirtualXRay::RATIONAL_NUMBER> > p_filtration;

    for (std::vector<std::pair<unsigned int, float> >::const_iterator ite(aSetOfFilters.begin());
        ite != aSetOfFilters.end();
        ++ite)
    {
        p_filtration.push_back(std::pair<unsigned int, float>(ite->first, ite->second * getUnitOfLength(aUnitOfLength)));
    }

    g_xray_beam.setFiltration(p_filtration);
}


//----------------------------------------------
void addFilter(const std::string& aMaterial,
               const double& aThickness,
               const std::string& aUnitOfLength)
//----------------------------------------------
{
    g_xray_beam.addFilter(aMaterial, aThickness * getUnitOfLength(aUnitOfLength));
}


//----------------------------------------------
void addFilter(unsigned int aMaterial,
               const double& aThickness,
               const std::string& aUnitOfLength)
//----------------------------------------------
{
    g_xray_beam.addFilter(aMaterial, aThickness * getUnitOfLength(aUnitOfLength));

}


//-----------------------------------------------
std::vector<unsigned int> getFiltrationMaterial()
//-----------------------------------------------
{
    std::vector<unsigned int> p_filtration;

    for (std::vector<std::pair<unsigned int, gVirtualXRay::RATIONAL_NUMBER> >::const_iterator ite(g_xray_beam.getFiltration().begin());
        ite != g_xray_beam.getFiltration().end();
        ++ite)
    {
        p_filtration.push_back(ite->first);
    }

    return p_filtration;
}


//-------------------------------------------------------------------------
std::vector<float> getFiltrationThickness(const std::string& aUnitOfLength)
//-------------------------------------------------------------------------
{
    std::vector<float> p_filtration;

    for (std::vector<std::pair<unsigned int, gVirtualXRay::RATIONAL_NUMBER> >::const_iterator ite(g_xray_beam.getFiltration().begin());
        ite != g_xray_beam.getFiltration().end();
        ++ite)
    {
        p_filtration.push_back(ite->second / getUnitOfLength(aUnitOfLength));
    }

    return p_filtration;
}


//--------------
bool hasSpekpy()
//--------------
{
    return g_xray_beam.hasSpekpy();
}


//---------------
bool hasXpecgen()
//---------------
{
    return g_xray_beam.hasXpecgen();
}


//--------------
void useSpekpy()
//--------------
{
    g_xray_beam.useSpekpy();
}


//---------------
void useXpecgen()
//---------------
{
    g_xray_beam.useXpecgen();
}


//----------------
bool usingSpekpy()
//----------------
{
    return g_xray_beam.usingSpekpy();
}


//-----------------
bool usingXpecgen()
//-----------------
{
    return g_xray_beam.usingXpecgen();
}


//-----------------------
void enablePoissonNoise()
//-----------------------
{
    g_xray_beam.enablePoissonNoise();
}


//------------------------
void disablePoissonNoise()
//------------------------
{
    g_xray_beam.disablePoissonNoise();
}


//----------------------------------------------------
void setNumberOfPhotons(unsigned int aNumberOfPhotons)
//----------------------------------------------------
{
    setNumberOfPhotonsPerPixelAtSDD(aNumberOfPhotons);
}


//-----------------------------------------------------------------
void setNumberOfPhotonsPerPixelAtSDD(unsigned int aNumberOfPhotons)
//-----------------------------------------------------------------
{
    g_xray_beam.setNumberOfPhotonsPerPixelAtSDD(aNumberOfPhotons);
}


//--------------------------------------------------------
void setNumberOfPhotonsPerCm2At1m(double aNumberOfPhotons)
//--------------------------------------------------------
{
    g_xray_beam.setNumberOfPhotonsPerCm2At1m(aNumberOfPhotons);
}


//-------------------------------
unsigned int getNumberOfPhotons()
//-------------------------------
{
    return getNumberOfPhotonsPerPixelAtSDD();
}


//--------------------------------------------
unsigned int getNumberOfPhotonsPerPixelAtSDD()
//--------------------------------------------
{
    return g_xray_beam.getNumberOfPhotonsPerPixelAtSDD(&g_xray_detector);
}


//-----------------------------------------
unsigned int getNumberOfPhotonsPerCm2At1m()
//-----------------------------------------
{
    return g_xray_beam.getNumberOfPhotonsPerCm2At1m(&g_xray_detector);
}


//------------------------------
void usePoissonNoise(bool aFlag)
//------------------------------
{
    g_xray_beam.usePoissonNoise(aFlag);
}


//--------------------
bool usePoissonNoise()
//--------------------
{
    return (g_xray_beam.usePoissonNoise());
}
