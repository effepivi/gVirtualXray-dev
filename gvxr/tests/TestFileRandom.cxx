#include <omp.h>
#include <bits/stdc++.h>
#include <cmath>
#include <algorithm>

#include "gtest/gtest.h"

#ifndef GVXR_RANDOM_H
#include "gVirtualXRay/Random.h"
#endif


//------------------------------------
TEST(TestFileRandom, Test1)
//------------------------------------
{

    float mu = 2.5;
    float sigma = 0.5;
    size_t max_size = 10000000;

    std::vector<float> p_temp(max_size);

    unsigned int initial_seed = time(0);

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

    float average = 0.0;
    #pragma omp parallel for reduction(+: average)
    for (auto ite: p_temp)
    {
        average += ite;
    }
    average /= p_temp.size();

    float variance = 0.0;
    #pragma omp parallel for reduction(+: variance)
    for (auto ite: p_temp)
    {
        variance += (ite - average) * (ite - average);
    }
    variance /= p_temp.size();

    float std_dev = std::sqrt(variance);

    float min_value = p_temp[0];
    float max_value = p_temp[0];

    #pragma omp parallel for reduction(min: min_value) reduction(max: max_value)
    for (int i = 1; i < p_temp.size(); ++i)
    {
        min_value = std::min(min_value, p_temp[i]);
        max_value = std::max(max_value, p_temp[i]);
    }        

    std::cout << std::endl;
    std::cout << "min_value:\t" << min_value << std::endl;
    std::cout << "max_value:\t" << max_value << std::endl;
    std::cout << "average:\t" << average << std::endl;
    std::cout << "variance:\t" << variance << std::endl;
    std::cout << "std_dev:\t" << std_dev << std::endl;
}
