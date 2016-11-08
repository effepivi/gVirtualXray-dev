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


/**
********************************************************************************
*
*   @file       ParticleSystem.cxx
*
*   @brief      Class to handle a particle system.
*
*   @version    1.0
*
*   @date       08/11/2016
*
*   @author     Franck Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Nov 2016, 2016, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <cstdlib>
#include <cmath>

#ifndef __ParticleSystem_h
//#include "gVirtualXRay/ParticleSystem.h"
#include "ParticleSystem.h"
#endif

#ifndef __SphereMesh_h
#include "gVirtualXRay/SphereMesh.h"
#endif

#ifndef __Utilities_h
#include "gVirtualXRay/Utilities.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Constant variables
//******************************************************************************


//******************************************************************************
//  Global variables
//******************************************************************************


//******************************************************************************
//  Function declaration
//******************************************************************************


//------------------------------
ParticleSystem::ParticleSystem()
//------------------------------
{
    // Initialise the random seed
    srand (time(NULL));
}


//-------------------------------
ParticleSystem::~ParticleSystem()
//-------------------------------
{
}


//-------------------------------------------------------------------
void ParticleSystem::displayWithSpheres(RATIONAL_NUMBER aRadius) const
//-------------------------------------------------------------------
{
    SphereMesh sphere(10, 10, aRadius);
    sphere.invertNormalVectors();

    // Display all the particles
    for (std::vector<Particle>::const_iterator ite(m_particle_set.begin());
            ite != m_particle_set.end();
            ++ite)
    {
        // The particle is alive
        if (ite->m_alive)
        {
            sphere.moveToCentre();
            sphere.translate(ite->m_position);
            sphere.display();
        }
    }
}


//---------------------------------------------------------
void ParticleSystem::setGravity(const VEC3& aGravityVector)
//---------------------------------------------------------
{
    m_gravity = aGravityVector;
}


//------------------------------------------------------------
void ParticleSystem::setInitialPosition(const VEC3& aPosition)
//------------------------------------------------------------
{
    m_initial_position = aPosition;
}


//------------------------------------------------------
void ParticleSystem::setUpVector(const VEC3& anUpVector)
//------------------------------------------------------
{
    m_up_vector = anUpVector / anUpVector.length();
}


//-----------------------------------------------
void ParticleSystem::setFOV(RATIONAL_NUMBER aFOV)
//-----------------------------------------------
{
    m_field_of_view = aFOV;
}


//----------------------------------------------
void ParticleSystem::setAgeLimit(int anAgeLimit)
//----------------------------------------------
{
    m_age_limit = anAgeLimit;
}


//-------------------------------------------------------
void ParticleSystem::setParticleLimit(int aParticleLimit)
//-------------------------------------------------------
{
    m_particle_limit = aParticleLimit;
}


//------------------------------------------------------------
bool ParticleSystem::addParticle(RATIONAL_NUMBER aProbability)
//------------------------------------------------------------
{
    int dead_particle_id(update());
    bool add_particle(false);

    if ((dead_particle_id == -1 && m_particle_set.size() < m_particle_limit) ||
            dead_particle_id != -1)
    {
        RATIONAL_NUMBER test(double(rand()) / RAND_MAX);
        add_particle = (test < aProbability);

        if (add_particle)
        {
            VEC3 velocity;
            double angle;

            do
            {
                RATIONAL_NUMBER u = 2.0 * double(rand()) / RAND_MAX - 1.0;
                RATIONAL_NUMBER theta = 2.0 * M_PI * double(rand()) / RAND_MAX;

                velocity[0] =   std::sqrt(1.0 - u * u) * std::cos(theta);
                velocity[1] =   std::sqrt(1.0 - u * u) * std::sin(theta);
                velocity[2] =   u;

                angle = std::acos(velocity.dotProduct(m_up_vector));
                angle = radianToDegree(angle);
            } while (angle > (m_field_of_view));

            Particle particle;

            particle.m_age = 0;
            particle.m_position = m_initial_position;
            particle.m_velocity = velocity;
            particle.m_alive = true;

            if (dead_particle_id != -1)
                m_particle_set[dead_particle_id] = particle;
            else
                m_particle_set.push_back(particle);
        }
    }

    m_time++;

    return (add_particle);
}


//-------------------------------------------------------
unsigned int ParticleSystem::getNumberOfParticles() const
//-------------------------------------------------------
{
    return (m_particle_set.size());
}


//-----------------------------------------------------------
unsigned int ParticleSystem::getNumberOfLiveParticles() const
//-----------------------------------------------------------
{
    unsigned int count(0);

    for (std::vector<Particle>::const_iterator ite(m_particle_set.begin());
            ite != m_particle_set.end();
            ++ite)
    {
        // The particle is alive
        if (ite->m_alive)
        {
            ++count;
        }
    }

    return (count);
}


//-----------------------------------------------------------
unsigned int ParticleSystem::getNumberOfDeadParticles() const
//-----------------------------------------------------------
{
    unsigned int count(0);

    for (std::vector<Particle>::const_iterator ite(m_particle_set.begin());
            ite != m_particle_set.end();
            ++ite)
    {
        // The particle is dead
        if (!ite->m_alive)
        {
            ++count;
        }
    }

    return (count);
}


//--------------------------
int ParticleSystem::update()
//--------------------------
{
    int dead_particle_id(-1);
    int id(0);

    for (std::vector<Particle>::iterator ite(m_particle_set.begin());
            ite != m_particle_set.end();
            ++ite, ++id)
    {
        if (m_age_limit >= 0)
        {
            if (ite->m_age++ > m_age_limit)
            {
                ite->m_alive = false;
            }
        }

        if (!ite->m_alive)
        {
            dead_particle_id = id;
        }
        else
        {
            ite->m_position += ite->m_velocity * (3.0 * mm);
        }
    }

    return (dead_particle_id);
}
