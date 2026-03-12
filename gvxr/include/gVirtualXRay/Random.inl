#define _USE_MATH_DEFINES

#include <climits>
#include <cmath>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

inline float mod(float x, float y)
{
    return x - y * floor(x / y);
}

inline float fract(float x)
{
    return x - floor(x);
}


inline float generateUniformNoise(float& s)
{
    return s = fract(sin((s + 0.0001) * 12.9898 + (s + 0.0001) *  78.233) * 43758.5453123);
}


//inline float generateUniformNoise(float u, float v)
//{
//    return fract(sin(u * 12.9898 + v * 78.233) * 43758.5453123);
//}


inline unsigned int getSeed(float s)
{
    return floor(UINT_MAX * generateUniformNoise(s));
}


//inline unsigned int getSeed(float u, float v)
//{
//    return floor(UINT_MAX * generateUniformNoise(u, v));
//}


unsigned TausStep(unsigned int& z, int S1, int S2, int S3, unsigned int M)
{
    unsigned b = (((z << S1) ^ z) >> S2);
    return z = (((z * M) << S3) ^ b);
}

// A and C are constants
inline unsigned LCGStep(unsigned int& z, unsigned int A, unsigned int C)
{
    return z = (A * z + C);
}

inline float HybridTaus(unsigned int& z1, unsigned int& z2, unsigned int& z3, unsigned int& z4)
{
    // Combined period is lcm(p1,p2,p3,p4)~ 2^121
    return 2.3283064365387e-10 * (              // Periods
        TausStep(z1, 13, 19, 12, 4294967294UL) ^  // p1=2^31-1
        TausStep(z2, 2, 25, 4, 4294967288UL) ^    // p2=2^30-1
        TausStep(z3, 3, 11, 17, 4294967280UL) ^   // p3=2^28-1
        LCGStep(z4, 1664525, 1013904223UL)        // p4=2^32
    );
}


inline std::pair<float, float> generateGaussianNoise(float mu, float sigma, 
    unsigned int& z1, unsigned int& z2, unsigned int& z3, unsigned int& z4)
{
    float u0 = HybridTaus(z1, z2, z3, z4);
    float u1 = HybridTaus(z1, z2, z3, z4);
    float r = sqrt(-2 * log(u0));
    float theta = 2 * M_PI * u1;
    return std::make_pair(r * sin(theta) * sigma + mu, r * cos(theta) * sigma + mu);
}

 inline unsigned int generatePoissonNoise(unsigned int aValue, unsigned int aCutOffValue,
     unsigned int& z1, unsigned int& z2, unsigned int& z3, unsigned int& z4, float& z5)
 {
     unsigned int noisy_value = 0;

     if (aValue > 0)
     {
         // Use the Poisson distribution
         if (aValue < aCutOffValue)
         {
             const double L = std::exp(-aValue);
             unsigned long k = 0;
             double p = 1.0;

             do
             {
                 k += 1;

                 p *= generateUniformNoise(z5);

             } while (p > L);

             aValue = k;
         }
         else
         {
             noisy_value = round(generateGaussianNoise(aValue, std::sqrt(aValue), z1, z2, z3, z4).first);
         }
     }

     return noisy_value;
 }

