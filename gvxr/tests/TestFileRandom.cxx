#include <omp.h>
#include <bits/stdc++.h>
#include <cmath>
#include <algorithm>
#include <chrono>

#include "gtest/gtest.h"

#ifndef GVXR_RANDOM_H
#include "gVirtualXRay/Random.h"
#endif


//******************************************************************************
//  Namespaces
//******************************************************************************
using namespace std::chrono;

typedef std::chrono::time_point<std::chrono::high_resolution_clock>  TimePoint;


//------------------------------------
TEST(TestFileRandom, Test1)
//------------------------------------
{

    float mu = 2.5;
    float sigma = 0.5;
    size_t max_size = 4000 * 4000;

    std::vector<float> p_temp(max_size);

    unsigned int initial_seed = time(0);

    TimePoint start_gaussian_noise_cpu;
    TimePoint stop_gaussian_noise_cpu;

    TimePoint start_average_cpu;
    TimePoint stop_average_cpu;

    TimePoint start_variance_cpu;
    TimePoint stop_variance_cpu;

    TimePoint start_min_max_cpu;
    TimePoint stop_min_max_cpu;
    

    TimePoint start_gaussian_noise_openmp_cpu;
    TimePoint stop_gaussian_noise_openmp_cpu;

    TimePoint start_average_openmp_cpu;
    TimePoint stop_average_openmp_cpu;

    TimePoint start_variance_openmp_cpu;
    TimePoint stop_variance_openmp_cpu;

    TimePoint start_min_max_openmp_cpu;
    TimePoint stop_min_max_openmp_cpu;
    
    
    TimePoint start_gaussian_noise_openmp_gpu;
    TimePoint stop_gaussian_noise_openmp_gpu;

    TimePoint start_average_openmp_gpu;
    TimePoint stop_average_openmp_gpu;

    TimePoint start_variance_openmp_gpu;
    TimePoint stop_variance_openmp_gpu;

    TimePoint start_min_max_openmp_gpu;
    TimePoint stop_min_max_openmp_gpu;
    
    // CPU without OpenMP
    {
        start_gaussian_noise_cpu = high_resolution_clock::now();

        unsigned int z1 = initial_seed + 0;
        unsigned int z2 = initial_seed + 1;
        unsigned int z3 = initial_seed + 2;
        unsigned int z4 = initial_seed + 3;

        for (int i = 0; i < max_size / 2; ++i)
        {
            std::pair<float, float> random_floats = generateGaussianNoise(mu, sigma, z1, z2, z3, z4);
            p_temp[i * 2 + 0] = random_floats.first;
            p_temp[i * 2 + 1] = random_floats.second;
        }
        stop_gaussian_noise_cpu = high_resolution_clock::now();

        start_average_cpu = high_resolution_clock::now();
        float average = 0.0;
        for (auto ite: p_temp)
        {
            average += ite;
        }
        average /= p_temp.size();
        stop_average_cpu = high_resolution_clock::now();

        start_variance_cpu = high_resolution_clock::now();
        float variance = 0.0;
        for (auto ite: p_temp)
        {
            variance += (ite - average) * (ite - average);
        }
        variance /= p_temp.size();
        stop_variance_cpu = high_resolution_clock::now();

        float std_dev = std::sqrt(variance);

        float min_value = p_temp[0];
        float max_value = p_temp[0];

        start_min_max_cpu = high_resolution_clock::now();
        for (int i = 1; i < p_temp.size(); ++i)
        {
            min_value = std::min(min_value, p_temp[i]);
            max_value = std::max(max_value, p_temp[i]);
        }        
        stop_min_max_cpu = high_resolution_clock::now();

        std::cout << "CPU:\t" << std::endl;
        std::cout << "\tmin_value:\t" << min_value << std::endl;
        std::cout << "\tmax_value:\t" << max_value << std::endl;
        std::cout << "\taverage:\t" << average << std::endl;
        std::cout << "\tvariance:\t" << variance << std::endl;
        std::cout << "\tstd_dev:\t" << std_dev << std::endl << std::endl;
    }
    
    // CPU with OpenMP
    {
        start_gaussian_noise_openmp_cpu = high_resolution_clock::now();
        #pragma omp parallel
        {
            // Get the thread ID
            int thread_id = omp_get_thread_num();

            // omp_get_num_threads() * 4 + 
            unsigned int z1 = initial_seed + thread_id * 4 + 0;
            unsigned int z2 = initial_seed + thread_id * 4 + 1;
            unsigned int z3 = initial_seed + thread_id * 4 + 2;
            unsigned int z4 = initial_seed + thread_id * 4 + 3;

            #pragma omp parallel for
            for (int i = 0; i < max_size / 2; ++i)
            {
                std::pair<float, float> random_floats = generateGaussianNoise(mu, sigma, z1, z2, z3, z4);
                p_temp[i * 2 + 0] = random_floats.first;
                p_temp[i * 2 + 1] = random_floats.second;
            }
        }
        stop_gaussian_noise_openmp_cpu = high_resolution_clock::now();



        start_average_openmp_cpu = high_resolution_clock::now();
        float average = 0.0;
        #pragma omp parallel for reduction(+: average)
        for (auto ite: p_temp)
        {
            average += ite;
        }
        average /= p_temp.size();
        stop_average_openmp_cpu = high_resolution_clock::now();

        start_variance_openmp_cpu = high_resolution_clock::now();
        float variance = 0.0;
        #pragma omp parallel for reduction(+: variance)
        for (auto ite: p_temp)
        {
            variance += (ite - average) * (ite - average);
        }
        variance /= p_temp.size();
        stop_variance_openmp_cpu = high_resolution_clock::now();

        float std_dev = std::sqrt(variance);

        float min_value = p_temp[0];
        float max_value = p_temp[0];

        start_min_max_openmp_cpu = high_resolution_clock::now();
        #pragma omp parallel for reduction(min: min_value) reduction(max: max_value)
        for (int i = 1; i < p_temp.size(); ++i)
        {
            min_value = std::min(min_value, p_temp[i]);
            max_value = std::max(max_value, p_temp[i]);
        }        
        stop_min_max_openmp_cpu = high_resolution_clock::now();

        std::cout << "OpenMP\tCPU:\t" << std::endl;
        std::cout << "\tmin_value:\t" << min_value << std::endl;
        std::cout << "\tmax_value:\t" << max_value << std::endl;
        std::cout << "\taverage:\t" << average << std::endl;
        std::cout << "\tvariance:\t" << variance << std::endl;
        std::cout << "\tstd_dev:\t" << std_dev << std::endl << std::endl;
    }



    std::cout << "Gaussian noise\t\tCPU\t" << duration_cast<milliseconds>(stop_gaussian_noise_cpu - start_gaussian_noise_cpu).count() << std::endl;
    std::cout << "Average\t\tCPU\t" << duration_cast<milliseconds>(stop_average_cpu - start_average_cpu).count() << std::endl;
    std::cout << "Variance\t\tCPU\t" << duration_cast<milliseconds>(stop_variance_cpu - start_variance_cpu).count() << std::endl;
    std::cout << "Min/Max\t\tCPU\t" << duration_cast<milliseconds>(stop_min_max_cpu - start_min_max_cpu).count() << std::endl << std::endl;

    std::cout << "Gaussian noise\tOpenMP\tCPU\t" << duration_cast<milliseconds>(stop_gaussian_noise_openmp_cpu - start_gaussian_noise_openmp_cpu).count() << std::endl;
    std::cout << "Average\tOpenMP\tCPU\t" << duration_cast<milliseconds>(stop_average_openmp_cpu - start_average_openmp_cpu).count() << std::endl;
    std::cout << "Variance\tOpenMP\tCPU\t" << duration_cast<milliseconds>(stop_variance_openmp_cpu - start_variance_openmp_cpu).count() << std::endl;
    std::cout << "Min/Max\tOpenMP\tCPU\t" << duration_cast<milliseconds>(stop_min_max_openmp_cpu - start_min_max_openmp_cpu).count() << std::endl << std::endl;
}
