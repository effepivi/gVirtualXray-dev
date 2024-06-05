#ifndef __SGVXR_Utils_h
#include "Utils.h"
#endif

#ifndef __ConstantValues_h
#include "gVirtualXRay/ConstantValues.h"
#endif

#ifndef __Units_h
#include "gVirtualXRay/Units.h"
#endif

#ifndef __XRayRenderer_h
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef __Exception_h
#include "gVirtualXRay/Exception.h"
#endif


//******************************************************************************
//  Namespace
//******************************************************************************
//using namespace std;


//******************************************************************************
//  Global variables
//******************************************************************************
extern gVirtualXRay::XRayRenderer g_xray_renderer; // X-ray graphics engine


//******************************************************************************
//  Function definitions
//******************************************************************************


//--------------------------------
float radian2degree(float anAngle)
//--------------------------------
{
    return (180.0 * anAngle / gVirtualXRay::Pi);
}


//-----------------------------------------------
double getUnitOfEnergy(const std::string& aUnitOfEnergy)
//-----------------------------------------------
{
    double unit_of_energy = 0;

    if (aUnitOfEnergy == "megaelectronvolt" || aUnitOfEnergy == "MeV") {
    unit_of_energy = gVirtualXRay::MeV;
    } else if (aUnitOfEnergy == "kiloelectronvolt" || aUnitOfEnergy == "keV") {
    unit_of_energy = gVirtualXRay::keV;
    } else if (aUnitOfEnergy == "electronvolt" || aUnitOfEnergy == "eV") {
    unit_of_energy = gVirtualXRay::eV;
    } else {
    throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
    "ERROR: Unknown unit of energy.");
    }

    return (unit_of_energy);
}


//-----------------------------------------------
double getUnitOfLength(const std::string& aUnitOfLength)
//-----------------------------------------------
{
    double unit_of_length = 0;

    if ((aUnitOfLength == "kilometer") || (aUnitOfLength == "kilometre")
    || (aUnitOfLength == "km")) {
    unit_of_length = gVirtualXRay::km;
    } else if ((aUnitOfLength == "hectometer")
    || (aUnitOfLength == "hectometre") || (aUnitOfLength == "hm")) {
    unit_of_length = gVirtualXRay::hm;
    } else if ((aUnitOfLength == "decameter") || (aUnitOfLength == "decametre")
    || (aUnitOfLength == "dam")) {
    unit_of_length = gVirtualXRay::dam;
    } else if ((aUnitOfLength == "meter") || (aUnitOfLength == "metre")
    || (aUnitOfLength == "m")) {
    unit_of_length = gVirtualXRay::m;
    } else if ((aUnitOfLength == "decimeter") || (aUnitOfLength == "decimetre")
    || (aUnitOfLength == "dm")) {
    unit_of_length = gVirtualXRay::dm;
    } else if ((aUnitOfLength == "centimeter")
    || (aUnitOfLength == "centimetre") || (aUnitOfLength == "cm")) {
    unit_of_length = gVirtualXRay::cm;
    } else if ((aUnitOfLength == "millimeter")
    || (aUnitOfLength == "millimetre") || (aUnitOfLength == "mm")) {
    unit_of_length = gVirtualXRay::mm;
    } else if ((aUnitOfLength == "micrometer")
    || (aUnitOfLength == "micrometre") || (aUnitOfLength == "um")) {
    unit_of_length = gVirtualXRay::um;
    } else {
    throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
    "ERROR: Unknown unit of length.");
    }

    return (unit_of_length);
}


//---------------------------------------------------------------------------
double getUnitOfElectriPotential(const std::string& aUnitOfElectriPotential)
//---------------------------------------------------------------------------
{
    double unit_of_electric_potential = 0;

    if ((aUnitOfElectriPotential == "V") || (aUnitOfElectriPotential == "volt"))
    {
        unit_of_electric_potential = gVirtualXRay::V;
    }
    else if ((aUnitOfElectriPotential == "kV") || (aUnitOfElectriPotential == "kilovolt"))
    {
        unit_of_electric_potential = gVirtualXRay::kV;
    }
    else if ((aUnitOfElectriPotential == "MV") || (aUnitOfElectriPotential == "megavolt"))
    {
        unit_of_electric_potential = gVirtualXRay::MV;
    }
    else
    {
        throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
            "ERROR: Unknown unit of electric potential.");
    }

    return (unit_of_electric_potential);
}


//-------------------------------------
void enableArtefactFilteringOnGPU(void)
//-------------------------------------
{
    g_xray_renderer.enableArtifactFilteringOnGPU();
}

//-------------------------------------
void enableArtifactFilteringOnGPU(void)
//-------------------------------------
{
    enableArtefactFilteringOnGPU();
}

//-------------------------------------
void enableArtefactFilteringOnCPU(void)
//-------------------------------------
{
    g_xray_renderer.enableArtifactFilteringOnCPU();
}

//-------------------------------------
void enableArtifactFilteringOnCPU(void)
//-------------------------------------
{
    enableArtefactFilteringOnCPU();
}


//---------------------------------
void disableArtefactFiltering(void)
//---------------------------------
{
    g_xray_renderer.disableArtifactFilteringOnGPU();
    g_xray_renderer.disableArtifactFilteringOnCPU();
}

//---------------------------------
void disableArtifactFiltering(void)
//---------------------------------
{
    disableArtefactFiltering();
}
