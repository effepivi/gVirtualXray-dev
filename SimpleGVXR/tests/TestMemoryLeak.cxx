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


/**
********************************************************************************
*
*   @file       TestMeshMemoryLeak.cxx
*
*   @brief      Unit testing: Test if there is any memoryleak in some portion of code.
*
*   @date       25/11/2024
*
*   @author     Prof Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Nov 2024-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <cmath>
#include <vector>
#include <numeric>

#ifdef __linux__
#include <sys/types.h>
#include <sys/sysinfo.h>
#elif WIN32
#include <windows.h>
#include <psapi.h>
#endif


#include "gtest/gtest.h"

#ifndef __Logger_h
#include "gVirtualXRay/Logger.h"
#endif

#ifndef __SimpleGVXR_h
#include "SimpleGVXR.h"
#endif


//******************************************************************************
//  Global variables
//******************************************************************************
int g_main_window_width(1280 / 2.0);
int g_main_window_height(800 / 2.0);

double g_incident_energy_in_keV(80.0);
const double g_scaling_factor(2.0);
double g_resolution_in_mm = (0.15 * g_scaling_factor); // 1mm per pixel
int g_number_of_pixels[] = { int(600 / g_scaling_factor), 1 };
double g_source_position_in_cm[] = { 0.0, 0.0, -10.0 };
double g_detector_position_in_cm[] = { 0.0, 0.0,  10.0 };
double g_detector_up_vector[] = { 0.0, 1.0,  0.0 };

double g_literature_result_in_keV(0.0);
double g_expected_result_in_keV(0.0);


//******************************************************************************
//  Function declaration
//******************************************************************************
void loadSTLFile();
void loadDetector();
void loadSource();
unsigned int processMemoryUsage();


//----------------------------
TEST(TestMemoryLeak, LoadMesh)
//----------------------------
{
    // Make sure the detector has an odd number of columns
    if ((g_number_of_pixels[0] % 2) == 0)
    {
        g_number_of_pixels[0] = g_number_of_pixels[0] + 1;
    }

    // Make sure the detector has an odd number of rows
    if ((g_number_of_pixels[1] % 2) == 0)
    {
        g_number_of_pixels[1] = g_number_of_pixels[1] + 1;
    }



    // Create an OpenGL context
#ifdef __APPLE__
    // OpenGL 4.2 and above are not supported on MacOS (at least on my compute with MacOS Big Sur)
    createWindow(-1, 0, "OPENGL", 4, 1);
#else // __APPLE__
    createWindow(-1, 0, "OPENGL", 4, 5);
#endif // __APPLE__

    setWindowSize(g_main_window_width, g_main_window_height);

    // Load the data
    loadDetector();
    loadSource();

    // Store the memory usage for each iteration
    unsigned int number_of_runs = 100;
    std::vector<unsigned int> p_memory_consumption_set(number_of_runs);

    for (int i = 0; i < number_of_runs; ++i)
    {
        // Destroy the 3D objects
        removePolygonMeshesFromSceneGraph();

        // Load new ones
        loadSTLFile();

        // Add them to the renderer
        addPolygonMeshAsInnerSurface("inside");
        addPolygonMeshAsOuterSurface("outside");
        
        // Probe the memory usage
        p_memory_consumption_set[i] = processMemoryUsage();
    }

    // Get the min and max memory usage
    unsigned int min_value = *min_element(p_memory_consumption_set.begin(), p_memory_consumption_set.end());
    unsigned int max_value = *max_element(p_memory_consumption_set.begin(), p_memory_consumption_set.end());

    // The memory consumption should not vary
    ASSERT_EQ(min_value, max_value);

    // Destroy the windows
    destroyAllWindows();

    // Kill the simulation engine
    terminate();
}

// #ifdef __linux__
// //---------------------------------
// TEST(TestMemoryLeak, CreateWindows)
// //---------------------------------
// {
// // #ifdef __linux__
// //     useLogFile("/dev/null");
// // #endif

//     // Make sure the detector has an odd number of columns
//     if ((g_number_of_pixels[0] % 2) == 0)
//     {
//         g_number_of_pixels[0] = g_number_of_pixels[0] + 1;
//     }

//     // Make sure the detector has an odd number of rows
//     if ((g_number_of_pixels[1] % 2) == 0)
//     {
//         g_number_of_pixels[1] = g_number_of_pixels[1] + 1;
//     }

//     // Store the memory usage for each iteration
//     unsigned int number_of_runs = 5;
//     std::vector<unsigned int> p_memory_consumption_set(number_of_runs);

//     for (int i = 0; i < number_of_runs; ++i)
//     {
//         // Create an OpenGL context
// #ifdef __APPLE__
//         // OpenGL 4.2 and above are not supported on MacOS (at least on my compute with MacOS Big Sur)
//         createWindow(-1, 0, "OPENGL", 4, 1);
// #else // __APPLE__
//         createWindow(-1, 0, "OPENGL", 4, 5);
// #endif // __APPLE__

//         setWindowSize(g_main_window_width, g_main_window_height);
        
//         // Destroy the windows
//         destroyAllWindows();

//         // Kill the simulation engine
//         terminate();

//         // Probe the memory usage
//         p_memory_consumption_set[i] = processMemoryUsage();

//         std::cout << p_memory_consumption_set[i] << std::endl;
//     }

//     // Get the min and max memory usage
//     unsigned int min_value = *min_element(p_memory_consumption_set.begin(), p_memory_consumption_set.end());
//     unsigned int max_value = *max_element(p_memory_consumption_set.begin(), p_memory_consumption_set.end());

//     // The memory consumption should not vary
//     ASSERT_EQ(min_value, max_value);
// }
// #endif

// //-----------------------------
// TEST(TestMemoryLeak, Terminate)
// //-----------------------------
// {
// // #ifdef __linux__
// //     useLogFile("/dev/null");
// // #endif

//     // Make sure the detector has an odd number of columns
//     if ((g_number_of_pixels[0] % 2) == 0)
//     {
//         g_number_of_pixels[0] = g_number_of_pixels[0] + 1;
//     }

//     // Make sure the detector has an odd number of rows
//     if ((g_number_of_pixels[1] % 2) == 0)
//     {
//         g_number_of_pixels[1] = g_number_of_pixels[1] + 1;
//     }

//     // Store the memory usage for each iteration
//     unsigned int number_of_runs = 5;
//     std::vector<unsigned int> p_memory_consumption_set(number_of_runs);

//     for (int i = 0; i < number_of_runs; ++i)
//     {
//         // Create an OpenGL context
//         autoCreateOpenGLContext(true);

//         setWindowSize(g_main_window_width, g_main_window_height);
        
//         // Load the data
//         loadDetector();
//         loadSource();

//         // Destroy the 3D objects
//         removePolygonMeshesFromSceneGraph();

//         // Load new ones
//         loadSTLFile();

//         // Add them to the renderer
//         addPolygonMeshAsInnerSurface("inside");
//         addPolygonMeshAsOuterSurface("outside");

//         // Compute the X-ray image
//         // std::vector<std::vector<float> > x_ray_image = computeXRayImage();

//         // Destroy the windows
//         destroyAllWindows();

//         // Kill the simulation engine
//         // terminate();

//         // Probe the memory usage
//         p_memory_consumption_set[i] = processMemoryUsage();

//         std::cout << p_memory_consumption_set[i] << std::endl;
//     }

//     // Get the min and max memory usage
//     unsigned int min_value = *min_element(p_memory_consumption_set.begin(), p_memory_consumption_set.end());
//     unsigned int max_value = *max_element(p_memory_consumption_set.begin(), p_memory_consumption_set.end());

//     // The memory consumption should not vary
//     ASSERT_EQ(min_value, max_value);
// }


//----------------
void loadSTLFile()
//----------------
{
    // Set the geometry
    makeCube("outside",
        3.0,
        "cm",
        "root");

    makeCylinder("inside",
        20,
        3.0,
        1.0,
        "cm",
        "root");

    // HU values
    double HU_water(0.0);
    double HU_bone(1329.885683);
    double HU_soft_tissue(51.714752);

    setHU( "inside", HU_bone);
    setHU("outside", HU_soft_tissue);
}


//-----------------
void loadDetector()
//-----------------
{
    setDetectorPixelSize(g_resolution_in_mm, g_resolution_in_mm, "mm");

    setDetectorNumberOfPixels(g_number_of_pixels[0], g_number_of_pixels[1]);

    setDetectorPosition(
        g_detector_position_in_cm[0],
        g_detector_position_in_cm[1],
        g_detector_position_in_cm[2],
        "cm");

    setDetectorUpVector(
        g_detector_up_vector[0],
        g_detector_up_vector[1],
        g_detector_up_vector[2]);
}


//---------------
void loadSource()
//---------------
{
    // Set the energy
    setMonoChromatic(g_incident_energy_in_keV, "keV", 1);

    // Set the source position
    usePointSource();
    setSourcePosition(g_source_position_in_cm[0],
        g_source_position_in_cm[1],
        g_source_position_in_cm[2],
        "cm");

    // Use a parallel source
    useParallelBeam();
}


//-------------------------------
unsigned int processMemoryUsage()
//-------------------------------
{
#ifdef __linux__
    // Adapted from https://stackoverflow.com/questions/669438/how-to-get-memory-usage-at-runtime-using-c
    using std::ios_base;
    using std::ifstream;
    using std::string;

    // 'file' stat seems to give the most reliable results
    //
    ifstream stat_stream("/proc/self/stat",ios_base::in);

    // dummy vars for leading entries in stat that we don't care about
    //
    string pid, comm, state, ppid, pgrp, session, tty_nr;
    string tpgid, flags, minflt, cminflt, majflt, cmajflt;
    string utime, stime, cutime, cstime, priority, nice;
    string O, itrealvalue, starttime;

    // the two fields we want
    //
    unsigned long vsize;
    long rss;

    stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
        >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
        >> utime >> stime >> cutime >> cstime >> priority >> nice
        >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

    stat_stream.close();

    return vsize;
#elif WIN32
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
    size_t counsumption = info.PeakWorkingSetSize;
    return info.PeakWorkingSetSize;
#else
    return 0;
#endif
}
