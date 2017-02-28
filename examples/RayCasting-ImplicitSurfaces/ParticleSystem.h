/*

Copyright (c) 2016, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


#ifndef __ParticleSystem_h
#define __ParticleSystem_h


/**
********************************************************************************
*
*   @file       ParticleSystem.h
*
*   @brief      Class to handle a particle system.
*
*   @version    1.0
*
*   @date       08/11/2016
*
*   @author     Franck Vidal
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <vector>

#ifndef __Types_h
#include "gVirtualXRay/Types.h"
#endif

#ifndef __Units_h
#include "gVirtualXRay/Units.h"
#endif


//******************************************************************************
//  Namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  ParticleSystem
*   @brief  ParticleSystem is a class to handle a particle system.
*/
//==============================================================================
class ParticleSystem
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    //--------------------------------------------------------------------------
    ParticleSystem();

    
    //--------------------------------------------------------------------------
    /// Destructor
    //--------------------------------------------------------------------------
    virtual ~ParticleSystem();


    //--------------------------------------------------------------------------
    /// Display
    //--------------------------------------------------------------------------
    void displayWithSpheres(RATIONAL_NUMBER aRadius = 1.0 * cm) const;
    
    
    void setGravity(const VEC3& aGravityVector);

    
    void setInitialPosition(const VEC3& aPosition);
    
    
    void setUpVector(const VEC3& anUpVector);

    
    void setFOV(RATIONAL_NUMBER aFOV);
    
    
    void setAgeLimit(int anAgeLimit);
    void setParticleLimit(int aParticleLimit);

    
    bool addParticle(RATIONAL_NUMBER aProbability);
    
    
    unsigned int getNumberOfParticles() const;
    
    
    unsigned int getNumberOfLiveParticles() const;
    
    
    unsigned int getNumberOfDeadParticles() const;
    
    
    class Particle
    {
    public:
      
        int m_age;
        VEC3 m_position;
        VEC3 m_velocity;
        bool m_alive;
    };
    
    
    std::vector<Particle> m_particle_set;

    
//******************************************************************************
protected:
    int update();
    
    
    VEC3 m_gravity;
    VEC3 m_initial_position;
    VEC3 m_up_vector;
    RATIONAL_NUMBER m_field_of_view;
    
    int m_age_limit;
    int m_particle_limit;
    
    int m_time;
};


} //namespace gVirtualXRay


//******************************************************************************
#include "ParticleSystem.inl"


#endif // __ParticleSystem_h
