/**
********************************************************************************
*
*   @file       AtomicElement.cxx
*
*   @brief      Class to manage elements in material.
*
*   @version    1.2
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
*               http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
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

#ifndef GVXR_CONSTANT_VALUES_H
#include "gVirtualXRay/ConstantValues.h"
#endif

#ifndef GVXR_ATOMIC_ELEMENT_H
#include "gVirtualXRay/AtomicElement.h"
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
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
        // The XCOM database path is set
        std::string xcom_database_path(gVirtualXRay_XCOM_PATH);
        if (xcom_database_path.size() && xcom_database_path != "XCOM_PATH-NOT_FOUND")
        {
            // Clear the current list of attenuation coefficients
            m_p_mass_attenuation_coefficient_set.clear();

            // Set the file name
            std::stringstream file_name;
            file_name << gVirtualXRay_XCOM_PATH << "/" << "MDATX3." << std::setw(3) << std::setfill('0') << m_atomic_number;

            // Open the file
            std::ifstream file_stream(file_name.str().data());

            // The file is open
            if (file_stream.is_open())
            {
                unsigned int atomic_number;
                unsigned int energy_channel_number;
                unsigned int repeat;
                int temp_int;
                double temp_double;
                std::string temp_string;
                double molar_mass;

                file_stream >> atomic_number >> molar_mass;
                file_stream >> repeat >> energy_channel_number;

                if (atomic_number != m_atomic_number)
                {
                    std::stringstream error_message;
                    error_message << "The atomic number of the element (" << m_atomic_number << ") does not match the one given by he XCOM database (" << atomic_number << ").";
                    throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
                }

                /*std::cout << "Attenuation table for " << m_name << std::endl;
                std::cout << "Symbol " << m_symbol << std::endl;
                std::cout << "Z " << m_atomic_number << std::endl;
                std::cout << energy_channel_number << " Energy channels in total" << std::endl;
*/
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
                    file_stream >> temp_int;
                }

                for (unsigned int i(0); i < repeat; ++i)
                {
                    file_stream >> temp_string;
                }

                for (unsigned int i(0); i < repeat; ++i)
                {
                    file_stream >> temp_double;
                }

                for (unsigned int i(0); i < energy_channel_number; ++i)
                {
                    double energy;
                    file_stream >> energy;
                    energy /= 10e5;
                    energy *= MeV;

                    m_p_energy_set[i] = energy;
                }

                for (unsigned int i(0); i < energy_channel_number; ++i)
                {
                    file_stream >> coherent_scattering_set[i];
                }

                for (unsigned int i(0); i < energy_channel_number; ++i)
                {
                    file_stream >> incoherent_scattering_set[i];
                }

                for (unsigned int i(0); i < energy_channel_number; ++i)
                {
                    file_stream >> photo_electric_absorption_set[i];
                }

                for (unsigned int i(0); i < energy_channel_number; ++i)
                {
                    file_stream >> pair_production_in_nuclear_field_set[i];
                }

                for (unsigned int i(0); i < energy_channel_number; ++i)
                {
                    file_stream >> pair_production_in_electron_field_set[i];
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
/*
                for (unsigned int i(0); i < energy_channel_number; ++i)
                {
                    // Attenuation in b/atom
                    double attenuation =
                            incoherent_scattering_set[i] +
                            photo_electric_absorption_set[i] +
                            pair_production_in_nuclear_field_set[i] +
                            pair_production_in_electron_field_set[i];

                    // Attenuation in cm2/g
                    attenuation *= (N_Avogadro / molar_mass) * 1.0E-24;

                    std::cout << std::scientific << m_p_energy_set[i] / MeV << "\t" <<
                            coherent_scattering_set[i] << "\t" <<
                            incoherent_scattering_set[i] << "\t" <<
                            photo_electric_absorption_set[i] << "\t" <<
                            pair_production_in_nuclear_field_set[i] << "\t" <<
                            pair_production_in_electron_field_set[i] << "\t" <<
                            m_p_mass_attenuation_coefficient_set[i] / (cm2/g) << "\t" <<
                            attenuation << std::endl;
                }*/
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
