/*

Copyright (c) 2017, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*	@file		CylinderMesh.cxx
*
*	@brief		Class to handle 3D meshes of cylinders.
*
*	@version	1.0
*
*	@date		27/05/2017
*
*	@author		Dr Franck P. Vidal
*
*	@section	License
*				BSD 3-Clause License.
*
*				For details on use and redistribution please refer
*				to http://opensource.org/licenses/BSD-3-Clause
*
*	@section	Copyright
*				(c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*				http://www.fpvidal.net/, May 2017, 2017, version 1.0,
*				BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifdef HAS_GLEW
#include "GL/glew.h"
#endif

#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <cmath>

#ifndef __CylinderMesh_h
#include "gVirtualXRay/CylinderMesh.h"
#endif

#ifndef GVXR_OUT_OF_MEMORY_EXCEPTION_H
#include "gVirtualXRay/OutOfMemoryException.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//------------------------------------------------------
void CylinderMesh::create(unsigned int aNumberOfSectors,
                          float aHeight,
                          float aRadius)
//------------------------------------------------------
{
    if (aNumberOfSectors < 3)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid parameter, the number of sectors should be at least 3");
    }

	// Reset the mesh
	reset();
	m_external_data_flag = false;

	m_radius = aRadius;

	m_min_corner = VEC3(-aRadius, -aHeight / 2.0, -aRadius);
    m_max_corner = VEC3( aRadius,  aHeight / 2.0,  aRadius);

	const float S(1.0 / float(aNumberOfSectors - 1));

    m_number_of_vertices = aNumberOfSectors * 2 + 2 + aNumberOfSectors * 2;
    std::vector<float> p_vertex_set;
    std::vector<float> p_normal_set;
    std::vector<unsigned int> p_index_set;

    float theta(0);
    float angle(M_PI * 2.0 / aNumberOfSectors);

    // The side of the cylinder
    for(unsigned int s(0); s < aNumberOfSectors; ++s, theta += angle)
    {
        float const x = cos(theta);
        float const z = sin(theta);

        p_vertex_set.push_back(x * aRadius);
        p_vertex_set.push_back(-aHeight / 2.0);
        p_vertex_set.push_back(z * aRadius);

        p_normal_set.push_back(x);
        p_normal_set.push_back(0);
        p_normal_set.push_back(z);

        p_vertex_set.push_back(x * aRadius);
        p_vertex_set.push_back(aHeight / 2.0);
        p_vertex_set.push_back(z * aRadius);

        p_normal_set.push_back(x);
        p_normal_set.push_back(0);
        p_normal_set.push_back(z);
    }

    // The caps of the cylinder
    p_vertex_set.push_back(0);
    p_vertex_set.push_back(-aHeight / 2.0);
    p_vertex_set.push_back(0);

    p_normal_set.push_back(0);
    p_normal_set.push_back(-1);
    p_normal_set.push_back(0);

    p_vertex_set.push_back(0);
    p_vertex_set.push_back(aHeight / 2.0);
    p_vertex_set.push_back(0);

    p_normal_set.push_back(0);
    p_normal_set.push_back(1);
    p_normal_set.push_back(0);

    for(unsigned int s(0); s < aNumberOfSectors; ++s, theta += angle)
    {
        float const x = cos(theta);
        float const z = sin(theta);

        p_vertex_set.push_back(x * aRadius);
        p_vertex_set.push_back(-aHeight / 2.0);
        p_vertex_set.push_back(z * aRadius);

        p_normal_set.push_back(0);
        p_normal_set.push_back(-1);
        p_normal_set.push_back(0);

        p_vertex_set.push_back(x * aRadius);
        p_vertex_set.push_back(aHeight / 2.0);
        p_vertex_set.push_back(z * aRadius);

        p_normal_set.push_back(0);
        p_normal_set.push_back(1);
        p_normal_set.push_back(0);
    }

    m_number_of_indices = aNumberOfSectors * 6 + aNumberOfSectors * 6;
    GLuint* p_indices = new GLuint[m_number_of_indices];
    if (!p_indices)
    {
    	throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
    }
    m_p_index_set = p_indices;
    m_index_data_type = GL_UNSIGNED_INT;

    for (unsigned int s(0); s < aNumberOfSectors - 1; ++s)
    {
        p_index_set.push_back(s * 2 + 0);
        p_index_set.push_back(s * 2 + 3);
        p_index_set.push_back(s * 2 + 1);

        p_index_set.push_back(s * 2 + 2);
        p_index_set.push_back(s * 2 + 3);
        p_index_set.push_back(s * 2 + 0);
    }

    // The last sector
    p_index_set.push_back(0);
    p_index_set.push_back(1);
    p_index_set.push_back(aNumberOfSectors * 2 - 1);

    p_index_set.push_back(aNumberOfSectors * 2 - 2);
    p_index_set.push_back(0);
    p_index_set.push_back(aNumberOfSectors * 2 - 1);

    // The top and bottom caps
    for (unsigned int s(0); s < aNumberOfSectors - 1; ++s)
        {
        p_index_set.push_back(aNumberOfSectors * 2);
        p_index_set.push_back(aNumberOfSectors * 2 + s * 2 + 2);
        p_index_set.push_back(aNumberOfSectors * 2 + s * 2 + 4);

        p_index_set.push_back(aNumberOfSectors * 2 + 1);
        p_index_set.push_back(aNumberOfSectors * 2 + s * 2 + 2 + 1);
        p_index_set.push_back(aNumberOfSectors * 2 + s * 2 + 4 + 1);
        }

    p_index_set.push_back(aNumberOfSectors * 2);
    p_index_set.push_back(aNumberOfSectors * 2 + 2);
    p_index_set.push_back(aNumberOfSectors * 2 + (aNumberOfSectors - 1) * 2 + 2);

    p_index_set.push_back(aNumberOfSectors * 2 + 1);
    p_index_set.push_back(aNumberOfSectors * 2 + 2 + 1);
    p_index_set.push_back(aNumberOfSectors * 2 + (aNumberOfSectors - 1) * 2 + 2 + 1);

    setInternalData(GL_TRIANGLES,
            &p_vertex_set,
            &p_normal_set,
            &p_index_set,
            false,
            GL_STATIC_DRAW);
}

