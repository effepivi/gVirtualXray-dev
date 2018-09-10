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

#ifndef __Utilities_h
#include "gVirtualXRay/Utilities.h"
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


//-----------------------------------------------------------------
inline void PolygonMesh::setHounsfieldValue(short aHounsfieldValue)
//-----------------------------------------------------------------
{
    // Set the Hounsfield value
    m_hounsfield_value = aHounsfieldValue;

    // The HU value will be used instead of tissue material properties or attenuation coefficient
    m_use_hounsfield_value = true;
    m_use_tissue_properties = false;
}


//-------------------------------------------------------------------------
inline void PolygonMesh::setTissueMaterial(const TissueMaterial& aMaterial)
//-------------------------------------------------------------------------
{
    // Set the tissue material properties
    m_tissue_properties = aMaterial;

    // The tissue material properties will be used instead of HU or attenuation coefficient
    m_use_hounsfield_value = false;
    m_use_tissue_properties = true;
}

//-------------------------------------------------------------------------
inline void PolygonMesh::setTissueDensity( double density )
//-------------------------------------------------------------------------
{
  //Only if there is one
  if ( m_use_tissue_properties )
    // Set the tissue material properties
    m_tissue_properties.setDensity( density );
}

//-------------------------------------------------------------------------
inline double PolygonMesh::getTissueDensity( )
//-------------------------------------------------------------------------
{
  //Only if there is one
  if ( m_use_tissue_properties )
    // Set the tissue material properties
    return m_tissue_properties.getDensity(  );

  return 1;
}


//-----------------------------------------------------------------------
inline void PolygonMesh::setAttenuationCoefficient(const RATIONAL_NUMBER&
        anAttenuationCoefficient)
//-----------------------------------------------------------------------
{
    // Set the attenuation coefficient
    m_attenuation_coefficient = anAttenuationCoefficient;

    // The attenuation coefficient will be used instead of HU or tissue material properties
    m_use_hounsfield_value = false;
    m_use_tissue_properties = false;
}


//--------------------------------------------------------
inline bool PolygonMesh::getUseHounsfieldValueFlag() const
//--------------------------------------------------------
{
    // Return true if the Hounsfield value is used,
    // false if the attenuation coefficient is used
    return (m_use_hounsfield_value);
}


//--------------------------------------------------------
inline bool PolygonMesh::getUseTissueMaterialFlag() const
//--------------------------------------------------------
{
    // Return true if the Hounsfield value is used,
    // false if the attenuation coefficient is used
    return (m_use_tissue_properties);
}


//--------------------------------------------------
inline short PolygonMesh::getHounsfieldValue() const
//--------------------------------------------------
{
    // Return the Hounsfield value
    return (m_hounsfield_value);
}

//-----------------------------------------------------------------
inline const TissueMaterial& PolygonMesh::getTissueMaterial() const
//-----------------------------------------------------------------
{
    // Return the Hounsfield value
    return (m_tissue_properties);
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
    //return (m_p_face_normal_set.size() / 3);
	return 0;
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
	if (anIndex >= getFaceNumber())
	{
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	if (m_number_of_indices)
	{
    	VEC3 temp = (getVertexNormal(getIndex(anIndex * 3 + 0)) +
    				getVertexNormal(getIndex(anIndex * 3 + 1)) +
					getVertexNormal(getIndex(anIndex * 3 + 2))) / 3.0;

        VEC3 normal((getVertex(getIndex(anIndex * 3 + 2)) - getVertex(getIndex(anIndex * 3 + 0)))^(getVertex(getIndex(anIndex * 3 + 2)) - getVertex(getIndex(anIndex * 3 + 1))));
        normal.normalize();

        if (sgn(normal.dotProduct(temp)) > 0)
        {
        	return (normal);
        }
        else
        {
        	return (-normal);
        }
	}
	else
	{
        VEC3 normal((getVertex(anIndex * 3 + 2) - getVertex(anIndex * 3 + 0)) ^ (getVertex(anIndex * 3 + 2) - getVertex(3 + 1)));
        normal.normalize();

/*        if (sgn(normal.dotProduct(temp)) > 0)
        {
        	return (normal);
        }
        else
        {
        	return (-normal);
        }
*/

    	return (-normal);
	}
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
    applyScale(aScale, aScale, aScale);
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

    
//----------------------------------------------------------------
inline void PolygonMesh::scale(RATIONAL_NUMBER x, RATIONAL_NUMBER y, RATIONAL_NUMBER z)
//----------------------------------------------------------------
{
    if (m_number_of_vertices)
        {
    	VEC3 scaling_factors(x, y, z);
        for (unsigned int i(0); i < m_number_of_vertices; i++)
            {
            VEC3 vertex(getVertex(i).elementWiseProduct(scaling_factors));
            setVertex(i, vertex);
            }

        m_min_corner = m_min_corner.elementWiseProduct(scaling_factors);
        m_max_corner = m_max_corner.elementWiseProduct(scaling_factors);

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


//----------------------------------------------------------
inline void PolygonMesh::setFaceNormal(unsigned int anIndex,
                                       const VEC3& aVertex)
//----------------------------------------------------------
{
    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Do Not Use Face Normal");
/*
    if (getFaceNormalNumber() <= anIndex && m_p_face_normal_set.size())
        {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }

    m_p_face_normal_set[anIndex * 3 + 0] = aVertex.getX();
    m_p_face_normal_set[anIndex * 3 + 1] = aVertex.getY();
    m_p_face_normal_set[anIndex * 3 + 2] = aVertex.getZ();*/
}


//------------------------------------------------------------
inline void PolygonMesh::setVertexNormal(unsigned int anIndex,
                                         const VEC3& aVertex)
//------------------------------------------------------------
{
    if (getVertexNormalNumber() <= anIndex && m_p_vertex_normal_set.size())
        {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }

    m_p_vertex_normal_set[anIndex * 3 + 0] = aVertex.getX();
    m_p_vertex_normal_set[anIndex * 3 + 1] = aVertex.getY();
    m_p_vertex_normal_set[anIndex * 3 + 2] = aVertex.getZ();
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
        
        for (unsigned int i(0); i < getFaceNormalNumber(); i++)
            {
            VEC3 normal_vector(rotation_matrix * getFaceNormal(i));
            setFaceNormal(i, normal_vector);
            }

        for (unsigned int i(0); i < getVertexNormalNumber(); i++)
            {
            VEC3 normal_vector(rotation_matrix * getVertexNormal(i));
            setVertexNormal(i, normal_vector);
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
        
        for (unsigned int i(0); i < getFaceNormalNumber(); i++)
            {
            VEC3 normal_vector(aTransformationMatrix * getFaceNormal(i));
            normal_vector.normalize();
            setFaceNormal(i, normal_vector);
            }

        for (unsigned int i(0); i < getVertexNormalNumber(); i++)
            {
            VEC3 normal_vector(aTransformationMatrix * getVertexNormal(i));
            normal_vector.normalize();
            setVertexNormal(i, normal_vector);
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

/*
    for (std::vector<RATIONAL_NUMBER>::iterator ite(
                        m_p_face_normal_set.begin());
            ite != m_p_face_normal_set.end();
            ++ite)
        {
        *ite = -(*ite);
        }
*/
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


inline void PolygonMesh::setTransformationMatrix(const MATRIX4& aTransformationMatrix)
{
	m_local_to_global_transformation = aTransformationMatrix;
}


inline const MATRIX4& PolygonMesh::getTransformationMatrix() const
{
	return (m_local_to_global_transformation);
}


inline MATRIX4& PolygonMesh::getTransformationMatrix()
{
	return (m_local_to_global_transformation);
}


} // namespace gVirtualXRay
