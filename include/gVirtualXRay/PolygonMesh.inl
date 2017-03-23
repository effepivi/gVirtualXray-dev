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
*   @file       PolygonMesh.inl
*
*   @brief      Class to handle polygon (triangles) meshes.
*
*   @version    1.0
*
*   @date       24/08/2016
*
*   @author     Dr Franck P. Vidal
*
*   @todo       Make it possible to merge vertices if an array of indices exists
*               Convert triangle mesh into arbitrary polygon mesh
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*   http://www.fpvidal.net/, Dec 2014, 2014, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef __OutOfBoundsException_h
#include "gVirtualXRay/OutOfBoundsException.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//--------------------------------
inline PolygonMesh::~PolygonMesh()
//--------------------------------
{
    // Make sure the VBO and the dynamically allocated memory are released
    reset();
}
//--------------------------------------------------------------------------
inline void PolygonMesh::setDisplayFlag(bool display)
//--------------------------------------------------------------------------
{
	//Change the value of the display flag
	m_display_flag = display;
}

//--------------------------------------------------------------------------
inline bool PolygonMesh::getDisplayFlag() const
//--------------------------------------------------------------------------
{
	// Return the display flag value
	return m_display_flag;
}

//---------------------------------------------------------
inline void PolygonMesh::setFilename(const char* aFilename)
//---------------------------------------------------------
{
    // Store the name of the file that contains the polygon mesh
    m_filename = aFilename;
}


//----------------------------------------------------------------
inline void PolygonMesh::setFilename(const std::string& aFilename)
//----------------------------------------------------------------
{
    // Store the name of the file that contains the polygon mesh
    m_filename = aFilename;
}


//---------------------------------------------------------
inline void PolygonMesh::setFileName(const char* aFilename)
//---------------------------------------------------------
{
    // Store the name of the file that contains the polygon mesh
    m_filename = aFilename;
}


//----------------------------------------------------------------
inline void PolygonMesh::setFileName(const std::string& aFilename)
//----------------------------------------------------------------
{
    // Store the name of the file that contains the polygon mesh
    m_filename = aFilename;
}


//--------------------------------------------------------
inline const std::string& PolygonMesh::getFilename() const
//--------------------------------------------------------
{
    // Return the name of the file that contains the polygon mesh
    return (m_filename);
}


//--------------------------------------------------------
inline const std::string& PolygonMesh::getFileName() const
//--------------------------------------------------------
{
    // Return the name of the file that contains the polygon mesh
    return (m_filename);
}


//-------------------------------------
inline void PolygonMesh::moveToCenter()
//-------------------------------------
{
    moveToCentre();
}


//-------------------------------------------------------------------
inline bool PolygonMesh::useVBO(int aTypeOfVBO, int aTypeOfPrimitive)
//-------------------------------------------------------------------
{
    // Create the VBO if needed
    if (!m_p_vbo.get())
        {
        m_p_vbo.reset(createVBO());
        }

    // There is no data to store in a VBO
    if (m_number_of_vertices && (getVertexNormalNumber() ||
            getFaceNormalNumber()))
        {
        // Create a VBO if possible
        updateVBO(aTypeOfVBO, aTypeOfPrimitive);
        }

    return (m_p_vbo->isReady());
}


//--------------------------------------------------------
inline bool PolygonMesh::getUseHounsfieldValueFlag() const
//--------------------------------------------------------
{
    // Return true if the Hounsfield value is used,
    // false if the attenuation coefficient is used
    return (m_use_hounsfield_value);
}


//-----------------------------------------------------------------
inline void PolygonMesh::setHounsfieldValue(short aHounsfieldValue)
//-----------------------------------------------------------------
{
    // Set the Hounsfield value
    m_hounsfield_value = aHounsfieldValue;

    // The Hounsfield value will be used instead of the attenuation coefficient
    m_use_hounsfield_value = true;
}


//-----------------------------------------------------------------------
inline void PolygonMesh::setAttenuationCoefficient(const RATIONAL_NUMBER&
        anAttenuationCoefficient)
//-----------------------------------------------------------------------
{
    // Set the attenuation coefficient
    m_attenuation_coefficient = anAttenuationCoefficient;

    // The attenuation coefficient will be used instead of the Hounsfield value
    m_use_hounsfield_value = false;
}


//--------------------------------------------------
inline short PolygonMesh::getHounsfieldValue() const
//--------------------------------------------------
{
    // Return the Hounsfield value
    return (m_hounsfield_value);
}


//-------------------------------------------------------------------
inline RATIONAL_NUMBER PolygonMesh::getAttenuationCoefficient() const
//-------------------------------------------------------------------
{
    // Return the attenuation coefficient
    return (m_attenuation_coefficient);
}


//--------------------------------------------------
inline const VEC3& PolygonMesh::getMinCorner() const
//--------------------------------------------------
{
    // Return the bounding box lower corner
    return (m_min_corner);
}


//--------------------------------------------------
inline const VEC3& PolygonMesh::getMaxCorner() const
//--------------------------------------------------
{
    // Return the bounding box upper corner
    return (m_max_corner);
}


//----------------------------------------------------------
inline unsigned int PolygonMesh::getFaceNormalNumber() const
//----------------------------------------------------------
{
    // Return the number of normal vectors
    // There are 3 GLfloat values per normal
    return (m_p_face_normal_set.size() / 3);
}


//------------------------------------------------------------
inline unsigned int PolygonMesh::getVertexNormalNumber() const
//------------------------------------------------------------
{
    // Return the number of normal vectors
    // There are 3 GLfloat values per normal
    return (m_p_vertex_normal_set.size() / 3);
}


//------------------------------------------------------
inline unsigned int PolygonMesh::getVertexNumber() const
//------------------------------------------------------
{
    return (m_number_of_vertices);
}


//-----------------------------------------------------
inline unsigned int PolygonMesh::getIndexNumber() const
//-----------------------------------------------------
{
    return (m_number_of_indices);
}


//----------------------------------------------------------------
inline VEC3 PolygonMesh::getFaceNormal(unsigned int anIndex) const
//----------------------------------------------------------------
{
    if (getFaceNormalNumber() <= anIndex)
    {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    return (VEC3(m_p_face_normal_set[anIndex * 3 + 0],
            m_p_face_normal_set[anIndex * 3 + 1],
            m_p_face_normal_set[anIndex * 3 + 2]));
}


//------------------------------------------------------------------
inline VEC3 PolygonMesh::getVertexNormal(unsigned int anIndex) const
//------------------------------------------------------------------
{
    if (getVertexNormalNumber() <= anIndex)
    {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    return (VEC3(m_p_vertex_normal_set[anIndex * 3 + 0],
            m_p_vertex_normal_set[anIndex * 3 + 1],
            m_p_vertex_normal_set[anIndex * 3 + 2]));
}


//-------------------------------------------
inline void PolygonMesh::computeBoundingBox()
//-------------------------------------------
{
    // There are vertices
    if (m_number_of_vertices)
    {
        m_min_corner = getVertex(0);
        m_max_corner = getVertex(0);

        for (unsigned int i(1); i < m_number_of_vertices; ++i)
        {
            VEC3 vertex(getVertex(i));

            if (m_min_corner.getX() > vertex.getX())
                m_min_corner.setX(vertex.getX());

            if (m_max_corner.getX() < vertex.getX())
                m_max_corner.setX(vertex.getX());

            if (m_min_corner.getY() > vertex.getY())
                m_min_corner.setY(vertex.getY());

            if (m_max_corner.getY() < vertex.getY())
                m_max_corner.setY(vertex.getY());

            if (m_min_corner.getZ() > vertex.getZ())
                m_min_corner.setZ(vertex.getZ());

            if (m_max_corner.getZ() < vertex.getZ())
                m_max_corner.setZ(vertex.getZ());
        }

        // Compute diagonal size
        m_bounding_box_diagonal = (m_min_corner - m_max_corner).length();
    }
}


//----------------------------------------------------------------
inline void PolygonMesh::applyScale(const RATIONAL_NUMBER& aScale)
//----------------------------------------------------------------
{
    // There are vertices
    if (m_number_of_vertices)
    {
        for (unsigned int i(0); i < m_number_of_vertices; ++i)
        {
            VEC3 vertex(getVertex(i) * aScale);
            setVertex(i, vertex);
        }

        // Update the bounding box
        m_min_corner *= aScale;
        m_max_corner *= aScale;
        m_bounding_box_diagonal *= aScale;

        // Create the VBO if needed
        if (!m_p_vbo.get())
        {
            m_p_vbo.reset(createVBO());
        }

        // Update the VBO
        if (m_p_vbo->isReady())
        {
            updateVBO(m_p_vbo->getBufferUsageHint(), m_p_vbo->getPrimitiveType());
        }
    }
}

    
//-----------------------------------------------------------------
inline void PolygonMesh::applyScale(const RATIONAL_NUMBER& aScaleX,
                                    const RATIONAL_NUMBER& aScaleY,
                                    const RATIONAL_NUMBER& aScaleZ)
//-----------------------------------------------------------------
{
    // There are vertices
    if (m_number_of_vertices)
    {
        for (unsigned int i(0); i < m_number_of_vertices; ++i)
        {
            VEC3 vertex(getVertex(i));
            
            vertex[0] *= aScaleX;
            vertex[1] *= aScaleY;
            vertex[2] *= aScaleZ;

            setVertex(i, vertex);
        }

        // Update the bounding box
        m_min_corner[0] *= aScaleX;
        m_min_corner[1] *= aScaleY;
        m_min_corner[2] *= aScaleZ;

        m_max_corner[0] *= aScaleX;
        m_max_corner[1] *= aScaleY;
        m_max_corner[2] *= aScaleZ;

        m_bounding_box_diagonal = (m_max_corner - m_min_corner).length();

        // Create the VBO if needed
        if (!m_p_vbo.get())
        {
            m_p_vbo.reset(createVBO());
        }

        // Update the VBO
        if (m_p_vbo->isReady())
        {
            updateVBO(m_p_vbo->getBufferUsageHint(), m_p_vbo->getPrimitiveType());
        }
    }
}
    

//----------------------------------------------------------------
inline void PolygonMesh::translate(const VEC3& aTranslationVector)
//----------------------------------------------------------------
{
    if (m_number_of_vertices)
        {
        for (unsigned int i(0); i < m_number_of_vertices; i++)
            {
            VEC3 vertex(getVertex(i) + aTranslationVector);
            setVertex(i, vertex);
            }

        m_min_corner += aTranslationVector;
        m_max_corner += aTranslationVector;

        // Create the VBO if needed
        if (!m_p_vbo.get())
            {
            m_p_vbo.reset(createVBO());
            }

        // Update the VBO
        if (m_p_vbo->isReady())
            {
            updateVBO(m_p_vbo->getBufferUsageHint(), m_p_vbo->getPrimitiveType());
            }
        }
}

    
//----------------------------------------------------------------------
inline void PolygonMesh::rotate(const VEC3& aRotationAxis,
                                RATIONAL_NUMBER aRotationAngleInDegrees)
//----------------------------------------------------------------------
{
    if (m_number_of_vertices)
        {
        MATRIX4 rotation_matrix(MATRIX4::buildRotationMatrix(
                aRotationAngleInDegrees,
                aRotationAxis));

        for (unsigned int i(0); i < m_number_of_vertices; i++)
            {
            VEC3 vertex(rotation_matrix * getVertex(i));
            setVertex(i, vertex);
            }
        
        // Update the bounding box
        computeBoundingBox();
            
        // Create the VBO if needed
        if (!m_p_vbo.get())
            {
            m_p_vbo.reset(createVBO());
            }
        
        // Update the VBO
        if (m_p_vbo->isReady())
            {
            updateVBO(m_p_vbo->getBufferUsageHint(), m_p_vbo->getPrimitiveType());
            }
        }
}
    
    
//----------------------------------------------------------------------
inline void PolygonMesh::transform(const MATRIX4& aTransformationMatrix)
//----------------------------------------------------------------------
{
    if (m_number_of_vertices)
        {
        for (unsigned int i(0); i < m_number_of_vertices; i++)
            {
            VEC3 vertex(aTransformationMatrix * getVertex(i));
            setVertex(i, vertex);
            }
        
        // Update the bounding box
        computeBoundingBox();
        
        // Create the VBO if needed
        if (!m_p_vbo.get())
            {
            m_p_vbo.reset(createVBO());
            }
        
        // Update the VBO
        if (m_p_vbo->isReady())
            {
            updateVBO(m_p_vbo->getBufferUsageHint(), m_p_vbo->getPrimitiveType());
            }
        }
}

    
//-------------------------------------
inline void PolygonMesh::moveToCentre()
//-------------------------------------
{
    VEC3 translation_vector(m_min_corner + (m_max_corner - m_min_corner) / 2.0);
    translation_vector *= -1.0;

    translate(translation_vector);
}


//--------------------------------------------
inline void PolygonMesh::invertNormalVectors()
//--------------------------------------------
{
    for (std::vector<RATIONAL_NUMBER>::iterator ite(
                            m_p_vertex_normal_set.begin());
            ite != m_p_vertex_normal_set.end();
            ++ite)
        {
        *ite = -(*ite);
        }


    for (std::vector<RATIONAL_NUMBER>::iterator ite(
                        m_p_face_normal_set.begin());
            ite != m_p_face_normal_set.end();
            ++ite)
        {
        *ite = -(*ite);
        }
}


//-----------------------------------------------------
inline const Material& PolygonMesh::getMaterial() const
//-----------------------------------------------------
{
    return (m_material);
}


//-----------------------------------------
inline Material& PolygonMesh::getMaterial()
//-----------------------------------------
{
    return (m_material);
}


} // namespace gVirtualXRay
