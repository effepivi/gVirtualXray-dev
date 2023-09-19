/*

Copyright (c) 2023, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*	@file		XRayBeam.cxx
*
*	@brief		Class to manage X-Ray beams. The beam spectrum is discretised
*				into energy channels.
*
*	@version    1.0
*
*	@date		19/09/2023
*
*	@author		Dr Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*   http://www.fpvidal.net/, Sept 2023, 2023, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifndef __XRayBeam_h
#include <gVirtualXRay/XRayBeam.h>
#endif

#ifdef HAS_PythonLibs
    #ifndef __Utilities_h
    #include "gVirtualXRay/Utilities.h"
    #endif

    #include "getSpectrum.py.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
using namespace gVirtualXRay;


//-------------------------------------------------------
void XRayBeam::initialise(const char* aFileName,
						  const RATIONAL_NUMBER& anUnit,
						  unsigned int aMergeChannelFlag)
//-------------------------------------------------------
{
	// Empty the current beam spectrum
	m_beam_spectrum.clear();

	// Open the file
	std::ifstream beam_spectrum_stream(aFileName);

	// The file is not open
	if (!beam_spectrum_stream.is_open())
	{
		throw FileDoesNotExistException(__FILE__, __FUNCTION__, __LINE__, aFileName);
	}

	// Load the file
	SpectrumRecord temp;
	while (beam_spectrum_stream >> temp)
	{
		// The record is valid
		if (!beam_spectrum_stream.eof())
		{
			// The number of photons is greater than the threshold
			if (temp.m_photon_number > m_number_of_photons_lower_threshold)
			{
				// Apply the unit
				temp.m_energy *= anUnit;

				// Store the data
				m_beam_spectrum.push_back(temp);
			}
		}
	}

	// Merge channels
	if (aMergeChannelFlag > 1)
	{
		// Merge channels
		unsigned int counter(0);
		temp.m_photon_number = 0;
		temp.m_energy = 0;

		// The set of new records
		std::vector<SpectrumRecord> new_beam_spectrum;

		// Process all the spectrum
		for (std::vector<SpectrumRecord>::iterator ite(m_beam_spectrum.begin());
			ite != m_beam_spectrum.end();
			++ite)
		{
			// Add the current 'old' bin to the 'new' bin
			temp.m_photon_number += ite->m_photon_number;
			temp.m_energy += ite->m_energy * ite->m_photon_number;
			counter++;

			// Store the new bin
			if (counter == aMergeChannelFlag)
			{
				// Compute the average the energy
				temp.m_energy /= temp.m_photon_number;

				// Add the bin
				new_beam_spectrum.push_back(temp);

				// Reset counters
				counter = 0;
				temp.m_photon_number = 0;
				temp.m_energy = 0;
			}
		}

		// Add remaining channels if any
		if (counter)
		{
			temp.m_energy /= temp.m_photon_number;
			new_beam_spectrum.push_back(temp);
		}

		// Store the new spectrum
		m_beam_spectrum = new_beam_spectrum;
	}

    // Sort the spectrum
    sort();

    // Reset the total number of photons per pixel
    m_number_of_photons_per_pixel = 0;

    // Proceed each energy channel
    for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_beam_spectrum.begin());
        ite_beam_spectrum != m_beam_spectrum.end();
        ++ite_beam_spectrum)
    {
        m_number_of_photons_per_pixel += ite_beam_spectrum->m_photon_number;
    }
}


//------------------------
void XRayBeam::normalise()
//------------------------
{
	// Compute the total incident energy
	RATIONAL_NUMBER total_energy(0);
	for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_beam_spectrum.begin());
		ite_beam_spectrum != m_beam_spectrum.end();
		++ite_beam_spectrum)
	{
		total_energy += ite_beam_spectrum->m_energy * ite_beam_spectrum->m_photon_number;
	}

	// The energy is positive
	if (total_energy > 0)
	{
		// Normalise the number of photons
		for (std::vector<SpectrumRecord>::iterator ite_beam_spectrum(m_beam_spectrum.begin());
			ite_beam_spectrum != m_beam_spectrum.end();
			++ite_beam_spectrum)
		{
			ite_beam_spectrum->m_photon_number /= total_energy;
		}
	}

    // Reset the total number of photons per pixel
    m_number_of_photons_per_pixel = 0;

    // Proceed each energy channel
    for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_beam_spectrum.begin());
        ite_beam_spectrum != m_beam_spectrum.end();
        ++ite_beam_spectrum)
    {
        m_number_of_photons_per_pixel += ite_beam_spectrum->m_photon_number;
    }
}


//----------------------------------------------------------------
void XRayBeam::setPhotonCountingBands(RATIONAL_NUMBER aMinEnergy,
                                      RATIONAL_NUMBER aMaxEnergy,
                                      unsigned int aNumberOfBands)
//----------------------------------------------------------------
{
    // Reset the array
    if (m_beam_spectrum_for_photon_count_detector.size())
    {
        mergePhotonCountingBands();
    }

    // Ignore this functionality if there is one band only
    if (aNumberOfBands > 1)
    {
        // Get the min and max energies
        RATIONAL_NUMBER step = (aMaxEnergy - aMinEnergy) / aNumberOfBands;
        RATIONAL_NUMBER max_energy_current_band = aMinEnergy;

        std::vector<SpectrumRecord>::const_iterator next_record_ite = m_beam_spectrum.begin();

        // Process every channel in the spectrum
        for (int i = 0; i < aNumberOfBands; ++i)
        {
            // Add a new band
            m_beam_spectrum_for_photon_count_detector.push_back(std::vector<SpectrumRecord>());
            m_photon_count_bands.push_back(max_energy_current_band);

            max_energy_current_band += step;
            RATIONAL_NUMBER last_recorded_energy = 0;

            // Process all the channel of the energy band
            while (next_record_ite != m_beam_spectrum.end() && last_recorded_energy < max_energy_current_band)
            {
                if (next_record_ite->m_energy < max_energy_current_band)
                {
                    m_beam_spectrum_for_photon_count_detector.back().push_back(*next_record_ite);
                    last_recorded_energy = next_record_ite->m_energy;

                    ++next_record_ite;
                }
                // Exit the while loop
                else
                {
                    last_recorded_energy = max_energy_current_band;
                }
            }

            // The last band is empty
            if (m_beam_spectrum_for_photon_count_detector.back().size() == 0)
            {
                // Drop the last band as it's empty
                m_beam_spectrum_for_photon_count_detector.pop_back();
                m_photon_count_bands.pop_back();
            }
        }
    }
}


//---------------------------------------
void XRayBeam::mergePhotonCountingBands()
//---------------------------------------
{
    // The functionality is in use
    if (getNumberOfPhotonCountingBands())
    {
        // Empty the spectrum
        m_beam_spectrum.clear();

        // Update the total number of photons in the spectrum
        m_number_of_photons_per_pixel = 0;

        // Process all the bands
        for (std::vector< std::vector<SpectrumRecord> >::const_iterator ite_band(m_beam_spectrum_for_photon_count_detector.begin());
            ite_band != m_beam_spectrum_for_photon_count_detector.end();
            ++ite_band)
        {
            // Process all the bins
            for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(ite_band->begin());
                ite_beam_spectrum != ite_band->end();
                ++ite_beam_spectrum)
            {
                // Add the bin
                m_beam_spectrum.push_back(*ite_beam_spectrum);

                // Update the total number of photons in the band
                m_number_of_photons_per_pixel += ite_beam_spectrum->m_photon_number;
            }
        }

        // Destroy the bands
        m_beam_spectrum_for_photon_count_detector.clear();
        m_photon_count_bands.clear();
    }
}


//-------------------------
void XRayBeam::initPython()
//-------------------------
{
#ifdef HAS_PythonLibs
    Py_Initialize();

    m_has_spekpy = false;
    m_has_xpecgen = false;
    m_use_spekpy = false;
    m_use_xpecgen = false;
    
    char* p_python_script(0);
    int z_lib_return_code(0);

    z_lib_return_code = inflate(g_getSpectrum_py, sizeof(g_getSpectrum_py), &p_python_script);


	PyRun_SimpleString("import sys\nprint(sys.version)");

    m_p_global = PyDict_New();

    // Create a new module object
    m_p_module = PyModule_New("mymod");
    PyModule_AddStringConstant(m_p_module, "__file__", "");

    // Get the dictionary object from my module so I can pass this to PyRun_String
    m_p_local = PyModule_GetDict(m_p_module);

    // Define my function in the newly created module
    PyObject* p_value = PyRun_String(p_python_script,
        Py_file_input, m_p_global, m_p_local);

    // p_value would be null if the Python syntax is wrong, for example
    if (p_value == NULL)
    {
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }

        LOGGER.logWarning("The Python syntax is wrong. This function ") << 
            __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;

        return;
    }

    // p_value is the result of the executing code, 
    // chuck it away because we've only declared a function
    Py_DECREF(p_value);

    // Get a pointer to the function I just defined
    PyObject* p_func = PyObject_GetAttrString(m_p_module, "hasSpekpy");

    //Double check we have actually found it and it is callable
    if (!p_func || !PyCallable_Check(p_func))
    {
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }

        LOGGER.logWarning("Cannot find function \"hasSpekpy\". This function ") << 
            __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;

        m_has_spekpy = false;
    }

    // Call my function
    p_value = PyObject_CallObject(p_func, nullptr);

    if (PyLong_AsLong(p_value) == 1)
    {
        m_has_spekpy = true;
    }
    else
    {
        m_has_spekpy = false;
    }


    // Get a pointer to the function I just defined
    p_func = PyObject_GetAttrString(m_p_module, "hasXpecgen");

    //Double check we have actually found it and it is callable
    if (!p_func || !PyCallable_Check(p_func))
    {
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }

        LOGGER.logWarning("Cannot find function \"hasXpecgen\". This function ") << 
            __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;
    }

    // Call my function
    p_value = PyObject_CallObject(p_func, nullptr);

    if (PyLong_AsLong(p_value) == 1)
    {
        m_has_xpecgen = true;
    }
    else
    {
        m_has_xpecgen = false;
    }

    Py_DECREF(p_func);

    // Get a pointer to the function I just defined
    m_p_func_spekpy = PyObject_GetAttrString(m_p_module, "getSpectrumSpekpy");

    //Double check we have actually found it and it is callable
    if (!m_p_func_spekpy || !PyCallable_Check(m_p_func_spekpy))
    {
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }

        m_has_spekpy = false;
        LOGGER.logWarning("Cannot find function \"getSpectrumSpekpy\". This function ") << 
            __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;
    }

    // Get a pointer to the function I just defined
    m_p_func_xpecgen = PyObject_GetAttrString(m_p_module, "getSpectrumXpecgen");

    //Double check we have actually found it and it is callable
    if (!m_p_func_xpecgen || !PyCallable_Check(m_p_func_xpecgen))
    {
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }

        m_has_xpecgen = false;
        LOGGER.logWarning("Cannot find function \"getSpectrumXpecgen\". This function ") << 
            __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;
    }

	PyRun_SimpleString("import sys\nprint(sys.version)");

    useSpekpy();

#endif
}


//----------------------------
void XRayBeam::releasePython()
//----------------------------
{
    m_has_spekpy = false;
    m_has_xpecgen = false;

#ifdef HAS_PythonLibs

    // if ()
    // {
    //     Py_DECREF();
    //      = 0;
    // }

    if (m_p_func_spekpy)
    {
        Py_DECREF(m_p_func_spekpy);
        m_p_func_spekpy = 0;
    }

    if (m_p_func_xpecgen)
    {
        Py_DECREF(m_p_func_xpecgen);
        m_p_func_xpecgen = 0;
    }

    if (m_p_local)
    {
        Py_DECREF(m_p_local);
        m_p_local = 0;
    }

    if (m_p_module)
    {
        Py_DECREF(m_p_module);
        m_p_module = 0;
    }

    if (m_p_global)
    {
        Py_DECREF(m_p_global);
        m_p_global = 0;
    }


    Py_Finalize();
#endif
}


//----------------------------------------
void XRayBeam::createSpectrumFromVoltage()
//----------------------------------------
{
#ifdef HAS_PythonLibs
    // PythonLibs are supported but Spekpy and Xpecgen are not installed
    if (!m_has_spekpy && !m_has_xpecgen)
    {
        LOGGER.logWarning("PythonLibs are supported but Spekpy and Xpecgen are not installed. This function ") << 
            __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;
    }
    // PythonLibs are supported.
    // Spekpy and/or Xpecgen are installed
    else
    {
        if (m_voltage > 0)
        {
            // Clear the current spectrum
            m_beam_spectrum.clear();
            m_beam_spectrum_for_photon_count_detector.clear();
            m_photon_count_bands.clear();
            m_number_of_photons_per_pixel = 0;

            // Select the right Python function
            PyObject* p_func = 0;
            if (m_use_spekpy)
            {
                p_func = m_p_func_spekpy;
                std::cout << "SPEKPY" << std::endl;
            }
            else if (m_use_xpecgen)
            {
                p_func = m_p_func_xpecgen;
                                std::cout << "xpecgen" << std::endl;

            }

            // k, f, unit
            // Call my function

            PyObject* p_args = PyLong_FromDouble(m_voltage / kV);
            // PyObject* p_value = PyObject_CallObject(m_p_func_spekpy, nullptr);
            // int p_result = PyLong_AsLong(p_value);
            // std::cout<< p_value<< std::endl;

            // Get a pointer to the function I just defined
            p_func = PyObject_GetAttrString(m_p_module, "getSpectrumSpekpy");

            //Double check we have actually found it and it is callable
            if (!p_func || !PyCallable_Check(p_func))
            {
                if (PyErr_Occurred())
                {
                    PyErr_Print();
                }

                LOGGER.logWarning("Cannot find function \"hasXpecgen\". This function ") << 
                    __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;
            }

            // Call my function
            PyObject* p_value = PyObject_CallFunctionObjArgs(p_func, p_args);
            if (PyErr_Occurred())
            {
                PyErr_Print();

                LOGGER.logWarning("Error calling this function ") << 
                    __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;
            }

            if (p_value)
            {
                if (PyTuple_Check(p_value))
                {
                    if (PyTuple_Size(p_value) == 2)
                    {
                        PyObject* p_k = PyTuple_GetItem(p_value, 0);
                        if (PyErr_Occurred())
                        {
                            PyErr_Print();

                            LOGGER.logWarning("Error calling this function ") << 
                                __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;
                        }

                        PyObject* p_f = PyTuple_GetItem(p_value, 1);
                        if (PyErr_Occurred())
                        {
                            PyErr_Print();

                            LOGGER.logWarning("Error calling this function ") << 
                                __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;
                        }
                        
                        if (PyList_Check(p_k) && PyList_Check(p_f))
                        {
                            Py_ssize_t number_of_bins = PyList_Size(p_k);

                            if (number_of_bins == PyList_Size(p_f))
                            {
                                for (unsigned int i = 0; i < number_of_bins; ++i)
                                {
                                    double energy;
                                    double count;

                                    PyObject* p_p_k = PyList_GetItem(p_k, i);
                                    if (PyErr_Occurred())
                                    {
                                        PyErr_Print();

                                        LOGGER.logWarning("Error calling this function ") << 
                                            __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;
                                    }

                                    PyObject* p_p_f = PyList_GetItem(p_f, i);
                                    if (PyErr_Occurred())
                                    {
                                        PyErr_Print();

                                        LOGGER.logWarning("Error calling this function ") << 
                                            __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;
                                    }

                                    energy = PyFloat_AsDouble(p_p_k);
                                    if (PyErr_Occurred())
                                    {
                                        PyErr_Print();

                                        LOGGER.logWarning("Error calling this function ") << 
                                            __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;
                                    }

                                    count = PyFloat_AsDouble(p_p_f);
                                    if (PyErr_Occurred())
                                    {
                                        PyErr_Print();

                                        LOGGER.logWarning("Error calling this function ") << 
                                            __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;
                                    }

                                    // if (p_p_k) Py_DECREF(p_p_k);
                                    // if (p_p_f) Py_DECREF(p_p_f);


                                    addChannel(count, energy * keV);
                                }

                                if (p_k) Py_DECREF(p_k);
                                if (p_f) Py_DECREF(p_f);

                            }
                            else
                            {
                                LOGGER.logWarning("Error calling this function ") << 
                                    __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;
                            }
                        }
                        else
                        {
                            LOGGER.logWarning("Error calling this function ") << 
                                __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;
                        }
                    }
                    else
                    {
                        LOGGER.logWarning("Error calling this function ") << 
                            __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;
                    }
                }
                else
                {
                    LOGGER.logWarning("Error calling this function ") << 
                        __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;
                }
            }
            else
            {
                LOGGER.logWarning("Error calling this function ") << 
                    __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;
            }
            

            // if (p_func) Py_DECREF(p_func);
            if (p_args) Py_DECREF(p_args);
            if (p_value) Py_DECREF(p_value);

            // Sort the spectrum
            sort();

            // Reset the total number of photons per pixel
            m_number_of_photons_per_pixel = 0;

            // Proceed each energy channel
            for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_beam_spectrum.begin());
                ite_beam_spectrum != m_beam_spectrum.end();
                ++ite_beam_spectrum)
            {
                m_number_of_photons_per_pixel += ite_beam_spectrum->m_photon_number;
            }
        }
    }
#else
		LOGGER.logWarning("PythonLibs support was disabled when the library was compiled. This function ") << 
            __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;
#endif
}
