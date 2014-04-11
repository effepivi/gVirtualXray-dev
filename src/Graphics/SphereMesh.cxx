/**
********************************************************************************
*
*	@file		SphereMesh.cxx
*
*	@brief		Class to handle 3D meshes of spheres.
*
*	@version	1.0
*
*	@date		22/03/2014
*
*	@author		Dr Franck P. Vidal
*
*	@todo		Make it possible to merge vertices if an array of indices exists
*
*	@section	License
*				BSD 3-Clause License.
*
*				For details on use and redistribution please refer
*				to http://opensource.org/licenses/BSD-3-Clause
*
*	@section	Copyright
*				(c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*				http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
*				BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#if defined(WIN32) || defined(WIN64)
#define _USE_MATH_DEFINES
#endif

#include <cmath>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glcorearb.h>
#endif

#ifndef GVXR_SPHERE_MESH_H
#include "gVirtualXRay/SphereMesh.h"
#endif

#ifndef GVXR_OUT_OF_MEMORY_EXCEPTION_H
#include "gVirtualXRay/OutOfMemoryException.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace Graphics;


//******************************************************************************
//	Constant variables
//******************************************************************************


//******************************************************************************
//	Global variables
//******************************************************************************


//******************************************************************************
//	Function declaration
//******************************************************************************


//----------------------------------------------------
void SphereMesh::create(unsigned int aNumberOfRings,
						unsigned int aNumberOfSectors,
						float aRadius)
//----------------------------------------------------
{
	// Reset the mesh
	reset();
	m_external_data_flag = false;

	const float R(1.0 / float(aNumberOfRings   - 1));
	const float S(1.0 / float(aNumberOfSectors - 1));

    GLfloat* p_vertices = new GLfloat[aNumberOfRings * aNumberOfSectors * 3];
    if (!p_vertices)
    {
    	throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
    }
    m_number_of_vertices = aNumberOfRings * aNumberOfSectors;
    m_p_vertex_set = p_vertices;
    m_vertex_data_type = GL_FLOAT;

	// Set the kind of polygon
	m_polygon_type = GL_TRIANGLES;

    m_p_vertex_normal_set.resize(m_number_of_vertices * 3);

    GLfloat* v(p_vertices);
    GLfloat* n(&m_p_vertex_normal_set[0]);
    for(unsigned int r(0); r < aNumberOfRings; ++r)
	{
    	for(unsigned int s(0); s < aNumberOfSectors; ++s)
    	{
            float const y = sin( -M_PI_2 + M_PI * r * R );
            float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
            float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

            *v++ = x * aRadius;
            *v++ = y * aRadius;
            *v++ = z * aRadius;

            *n++ = x;
            *n++ = y;
            *n++ = z;
    	}
    }

    GLuint* p_indices = new GLuint[aNumberOfRings * aNumberOfSectors * 6];
    if (!p_indices)
    {
    	throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
    }
    m_number_of_indices = aNumberOfRings * aNumberOfSectors * 6;
    m_p_index_set = p_indices;
    m_index_data_type = GL_UNSIGNED_INT;

    GLuint* i(p_indices);
    for (unsigned int r(0); r < aNumberOfRings - 1; ++r)
    {
    	for (unsigned int s(0); s < aNumberOfSectors - 1; ++s)
    	{
            *i++ =  r      * aNumberOfSectors +  s;
            *i++ =  r      * aNumberOfSectors + (s + 1);
            *i++ = (r + 1) * aNumberOfSectors + (s + 1);
            *i++ =  r      * aNumberOfSectors +  s;
            *i++ = (r + 1) * aNumberOfSectors + (s + 1);
            *i++ = (r + 1) * aNumberOfSectors +  s;
    	}
    }
}
