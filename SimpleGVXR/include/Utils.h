#ifndef __SGVXR_Utils_h
#define __SGVXR_Utils_h


#include <string>


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



#endif
