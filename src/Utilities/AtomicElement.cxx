/*

Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


/**
********************************************************************************
*
*   @file       AtomicElement.cxx
*
*   @brief      Class to manage elements in material.
*
*   @version    1.0
*
*   @date       13/10/2014
*
*   @author     Dr Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>

#ifndef GVXR_ATOMIC_ELEMENT_H
#include "gVirtualXRay/AtomicElement.h"
#endif

#ifndef GVXR_CONSTANT_VALUES_H
#include "gVirtualXRay/ConstantValues.h"
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

#ifndef UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#ifdef gVirtualXRay_XCOM_PATH
#include "MDATX3.001.h"
#include "MDATX3.006.h"
#include "MDATX3.007.h"
#include "MDATX3.008.h"

#include "MDATX3.011.h"
#include "MDATX3.012.h"
#include "MDATX3.015.h"
#include "MDATX3.016.h"
#include "MDATX3.017.h"
#include "MDATX3.018.h"
#include "MDATX3.019.h"

#include "MDATX3.020.h"
#include "MDATX3.026.h"
#include "MDATX3.022.h"
#include "MDATX3.029.h"

#include "MDATX3.030.h"

#include "MDATX3.047.h"

#include "MDATX3.050.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
using namespace gVirtualXRay;


//------------------------------------
void AtomicElement::loadDataFromXCOM()
//------------------------------------
{
    // The atomic number is valid
    if (1 <= m_atomic_number && m_atomic_number <= 100)
    {
        std::string xcom_database_path;

        // The XCOM database path is set
#ifdef gVirtualXRay_XCOM_PATH
        xcom_database_path = gVirtualXRay_XCOM_PATH;
#endif

        // The environment variable may overwrite gVirtualXRay_XCOM_PATH
        char* p_environment_variable = std::getenv("XCOM_PATH");
        if (p_environment_variable)
        {
            xcom_database_path = p_environment_variable;
        }

        // XCOM database not installed
        if (xcom_database_path == "XCOM_PATH-NOTFOUND")
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "The XCOM database is not installed. You can find it on the NIST's website at http://www.nist.gov/pml/data/xcom/index.cfm If you install it, you will need to re-compile the library.");
        }

        if (xcom_database_path.size())
        {
            // Clear the current list of attenuation coefficients
            m_p_mass_attenuation_coefficient_set.clear();

            // Set the file name
            std::stringstream file_name;
            file_name << xcom_database_path << "/" << "MDATX3." << std::setw(3) << std::setfill('0') << m_atomic_number;

            // Open the file
            std::ifstream file_stream(file_name.str().data());
            std::stringstream input_stream;

            // The file is open
            bool open_stream(false);
            if (file_stream.is_open())
            {
                // Load the stream
                std::string temp;
                while (!file_stream.eof())
                {
                    // Load the record
                    file_stream >> temp;

                    // The record is valid
                    if (!file_stream.eof())
                    {
                        // Store the record
                        input_stream << temp << "\t";
                    }
                }

                open_stream = true;
            }
            // The file is not open
            // Find the data from the header file is possible
#ifdef gVirtualXRay_XCOM_PATH
            if (!open_stream)
            {
                // Initialise the buffer
                char* p_buffer(0);

                int z_lib_return_code(0);

                std::string element_data;

                unsigned char const* p_compressed_data(0);
                unsigned int p_compressed_size(0);

                switch (m_atomic_number)
                {
                    case 1:
                        p_compressed_data = g_MDATX3_001;
                        p_compressed_size = sizeof(g_MDATX3_001);
                        break;

                    case 6:
                        p_compressed_data = g_MDATX3_006;
                        p_compressed_size = sizeof(g_MDATX3_006);
                        break;

                    case 7:
                        p_compressed_data = g_MDATX3_007;
                        p_compressed_size = sizeof(g_MDATX3_007);
                        break;

                    case 8:
                        p_compressed_data = g_MDATX3_008;
                        p_compressed_size = sizeof(g_MDATX3_008);
                        break;

                    case 11:
                        p_compressed_data = g_MDATX3_011;
                        p_compressed_size = sizeof(g_MDATX3_011);
                        break;

                    case 12:
                        p_compressed_data = g_MDATX3_012;
                        p_compressed_size = sizeof(g_MDATX3_012);
                        break;

                    case 15:
                        p_compressed_data = g_MDATX3_015;
                        p_compressed_size = sizeof(g_MDATX3_015);
                        break;

                    case 16:
                        p_compressed_data = g_MDATX3_016;
                        p_compressed_size = sizeof(g_MDATX3_016);
                        break;

                    case 17:
                        p_compressed_data = g_MDATX3_017;
                        p_compressed_size = sizeof(g_MDATX3_017);
                        break;

                    case 18:
                        p_compressed_data = g_MDATX3_018;
                        p_compressed_size = sizeof(g_MDATX3_018);
                        break;

                    case 19:
                        p_compressed_data = g_MDATX3_019;
                        p_compressed_size = sizeof(g_MDATX3_019);
                        break;

                    case 20:
                        p_compressed_data = g_MDATX3_020;
                        p_compressed_size = sizeof(g_MDATX3_020);
                        break;

                    case 22:
                        p_compressed_data = g_MDATX3_022;
                        p_compressed_size = sizeof(g_MDATX3_022);
                        break;

                    case 26:
                        p_compressed_data = g_MDATX3_026;
                        p_compressed_size = sizeof(g_MDATX3_026);
                        break;

                    case 29:
                        p_compressed_data = g_MDATX3_029;
                        p_compressed_size = sizeof(g_MDATX3_029);
                        break;

                    case 30:
                        p_compressed_data = g_MDATX3_030;
                        p_compressed_size = sizeof(g_MDATX3_030);
                        break;

                    case 47:
                        p_compressed_data = g_MDATX3_047;
                        p_compressed_size = sizeof(g_MDATX3_047);
                        break;

                    case 50:
                        p_compressed_data = g_MDATX3_050;
                        p_compressed_size = sizeof(g_MDATX3_050);
                        break;

                    default:
                        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unknown element.");
                }

                z_lib_return_code = inflate(p_compressed_data, p_compressed_size,   &p_buffer);

                element_data   = p_buffer;
                delete [] p_buffer;

                p_buffer = 0;

                if (z_lib_return_code <= 0 || !element_data.size())
                {
                    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the element data using ZLib.");
                }

                input_stream << element_data;

                open_stream = true;
            }
#endif // gVirtualXRay_XCOM_PATH

            // The strem is open
            if (open_stream)
            {
                unsigned int atomic_number;
                unsigned int energy_channel_number;
                unsigned int repeat;
                int temp_int;
                double temp_double;
                std::string temp_string;
                double molar_mass;

                input_stream >> atomic_number >> molar_mass;
                input_stream >> repeat >> energy_channel_number;

                if (atomic_number != m_atomic_number)
                {
                    std::stringstream error_message;
                    error_message << "The atomic number of the element (" << m_atomic_number << ") does not match the one given by he XCOM database (" << atomic_number << ").";
                    throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
                }

                std::vector<double> coherent_scattering_set(energy_channel_number);
                std::vector<double> incoherent_scattering_set(energy_channel_number);
                std::vector<double> photo_electric_absorption_set(energy_channel_number);
                std::vector<double> pair_production_in_nuclear_field_set(energy_channel_number);
                std::vector<double> pair_production_in_electron_field_set(energy_channel_number);
                std::vector<double> total_attenuation_without_coherent_scattering(energy_channel_number);

                m_p_energy_set.clear();
                m_p_mass_attenuation_coefficient_set.clear();

                m_p_energy_set.resize(energy_channel_number);
                m_p_mass_attenuation_coefficient_set.resize(energy_channel_number);

                for (unsigned int i(0); i < repeat; ++i)
                {
                    input_stream >> temp_int;
                }

                for (unsigned int i(0); i < repeat; ++i)
                {
                    input_stream >> temp_string;
                }

                for (unsigned int i(0); i < repeat; ++i)
                {
                    input_stream >> temp_double;
                }

                for (unsigned int i(0); i < energy_channel_number; ++i)
                {
                    double energy;
                    input_stream >> energy;
                    energy /= 10e5;
                    energy *= MeV;

                    m_p_energy_set[i] = energy;
                }

                for (unsigned int i(0); i < energy_channel_number; ++i)
                {
                    input_stream >> coherent_scattering_set[i];
                }

                for (unsigned int i(0); i < energy_channel_number; ++i)
                {
                    input_stream >> incoherent_scattering_set[i];
                }

                for (unsigned int i(0); i < energy_channel_number; ++i)
                {
                    input_stream >> photo_electric_absorption_set[i];
                }

                for (unsigned int i(0); i < energy_channel_number; ++i)
                {
                    input_stream >> pair_production_in_nuclear_field_set[i];
                }

                for (unsigned int i(0); i < energy_channel_number; ++i)
                {
                    input_stream >> pair_production_in_electron_field_set[i];
                }

                for (unsigned int i(0); i < energy_channel_number; ++i)
                {
                    // Attenuation in b/atom
                    double attenuation =
                            incoherent_scattering_set[i] +
                            photo_electric_absorption_set[i] +
                            pair_production_in_nuclear_field_set[i] +
                            pair_production_in_electron_field_set[i];

                    // Attenuation in cm2/g
                    total_attenuation_without_coherent_scattering[i] = attenuation * ((N_Avogadro / molar_mass) * 1.0E-24);
                }

                for (unsigned int i(0); i < energy_channel_number; ++i)
                {
                    // Attenuation in b/atom
                    double attenuation =
                            coherent_scattering_set[i] +
                            incoherent_scattering_set[i] +
                            photo_electric_absorption_set[i] +
                            pair_production_in_nuclear_field_set[i] +
                            pair_production_in_electron_field_set[i];

                    // Attenuation in cm2/g
                    attenuation *= (N_Avogadro / molar_mass) * 1.0E-24;
                    attenuation *= cm2 / g;
                    m_p_mass_attenuation_coefficient_set[i] = attenuation;
                }
            }
            // The file is not open
            else
            {
                std::stringstream error_message;
                error_message << "ERROR: Cannot open file \"" << file_name.str() << "\". The XCOM database is supposed to be installed in \"" << xcom_database_path << "\". Check your installation. The database can be found on the NIST's website at: http://physics.nist.gov/PhysRefData/Xcom/Text/download.html";
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
            }
        }
    }
}


//-----------------------------------------------------------------
std::ostream& gVirtualXRay::operator<<(std::ostream& anOutputSream,
                                       const AtomicElement& anElement)
//-----------------------------------------------------------------
{
    anOutputSream << "Element: "                 << anElement.getName() << "(" << anElement.getSymbol() << ")" << std::endl;
    anOutputSream << "\tatomic number (Z): "     << anElement.getAtomicNumber()           << std::endl;
    anOutputSream << "\tmolar mass (in g/mol): " << anElement.getMolarMass() / (g / mole) << std::endl;
    anOutputSream << "\tdensity (in g/cm3): "    << anElement.getDensity() / (g/cm3)      << std::endl;
    anOutputSream << "\tstate: "                 << anElement.getState()                  << std::endl;

    return (anOutputSream);
}


//----------------------------------------------------------------
std::istream& gVirtualXRay::operator>>(std::istream& anInputSream,
                                       AtomicElement& anElement)
//----------------------------------------------------------------
{
    std::string temp_string;
    unsigned short temp_short;
    double temp_float;

    anInputSream >> temp_string;
    anElement.setName(temp_string);

    anInputSream >> temp_string;
    anElement.setSymbol(temp_string);

    anInputSream >> temp_short;
    anElement.setAtomicNumber(temp_short);

    anInputSream >> temp_float;
    temp_float *= g / mole;
    anElement.setMolarMass(temp_float);

    anInputSream >> temp_float;
    temp_float *= g / cm3;
    anElement.setDensity(temp_float);

    anInputSream >> temp_string;
    anElement.setState(temp_string);


    // Load the data from the XCOM database if possible
    anElement.loadDataFromXCOM();

    return (anInputSream);
}
