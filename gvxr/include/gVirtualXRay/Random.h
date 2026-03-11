#ifndef GVXR_RANDOM_H
#define GVXR_RANDOM_H

#include <utility>
	
// S1, S2, S3, and M are all constants, and z is part of the
// private per-thread generator state.
unsigned TausStep(unsigned int& z, int S1, int S2, int S3, unsigned int M);

// A and C are constants
unsigned LCGStep(unsigned int& z, unsigned int A, unsigned int C);

float HybridTaus(unsigned int& z1, unsigned int& z2, unsigned int& z3, unsigned int& z4);


//"mu" is the mean of the distribution, and "sigma" is the standard deviation.
// Using box muller method
std::pair<float, float> generateGaussianNoise(float mu, float sigma, unsigned int& z1, unsigned int& z2, unsigned int& z3, unsigned int& z4);

// unsigned int generatePoissonNoise(unsigned int);


#include "gVirtualXRay/Random.inl"

#endif
