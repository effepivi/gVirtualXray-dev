/**
********************************************************************************
*
*	@file		XRayDetector.inl
*
*	@brief		Class to handle virtual X-Ray detectors.
*
*	@version	1.0
*
*	@date		30/11/2013
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
*				http://www.fpvidal.net/, Dec 2013, 2013, version 1.0, 
*				BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <fstream>
#include <iostream>

#include<GL/glew.h>
#ifdef __APPLE__
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif

#ifndef OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//----------------------------------------------
inline XRayDetector::XRayDetector():
//----------------------------------------------
	m_projection_z_near(0),
	m_projection_z_far(0),
	m_resolution_in_unit_of_length_per_pixel(0),
	m_distance_source_detector(0),
	m_field_of_view_y(0),
	m_VBOs_need_update(true),
	m_source_type(UNKNOWN)
//----------------------------------------------
{
}


//---------------------------------------------
inline void XRayDetector::displaySource() const
//---------------------------------------------
{
	for (std::vector<VEC3>::const_iterator ite(m_p_xray_source_position_set.begin());
			ite != m_p_xray_source_position_set.end();
			++ite)
	{
		// Store the model/view matrix
		pushModelViewMatrix();

		g_current_modelview_matrix *= MATRIX4::buildTranslationMatrix(ite->getX(), ite->getY(), ite->getZ());
		applyModelViewMatrix();
		
		glutSolidSphere(1.0, 5, 5);
		
		// Restore the model/view matrix
		popModelViewMatrix();
	}
}


//------------------------------------------------------------------------------------------------
inline void XRayDetector::setResolutionInUnitsOfLengthPerPixel(const RATIONAL_NUMBER& aResolution)
//------------------------------------------------------------------------------------------------
{
	// The resolution has changed
	m_resolution_in_unit_of_length_per_pixel = aResolution;
	
	// Update the size of the detector
	updateSizeInUnitOfLength();
	
	// Update the size of the field of view
	updateFovy();
	
	// The VBOs need to be updated
	m_VBOs_need_update = true;
}


//----------------------------------------------------------------------
inline void XRayDetector::setNumberOfPixels(const Vec2ui& aSizeInPixels)
//----------------------------------------------------------------------
{
	// The number of pixels has changed
	m_size_in_number_of_pixels = aSizeInPixels;

	// Update the size of the detector
	updateSizeInUnitOfLength();
	
	// Update the size of the field of view
	updateFovy();
	
	// The VBOs need to be updated
	m_VBOs_need_update = true;
}


//---------------------------------------------------------------------------
inline void XRayDetector::setXrayPointSource(const VEC3& aXraySourcePosition)
//---------------------------------------------------------------------------
{
	// Store the centre position
	m_xray_source_centre = aXraySourcePosition;

	// Clear the sample set
	m_p_xray_source_position_set.clear();

	// Add the centre
	m_p_xray_source_position_set.push_back(m_xray_source_centre);

	// Save the source type
	m_source_type = POINT;	
	
	// Update the distance between the source and the detector, etc.
	updateInternalValues();
	
	// The VBOs need to be updated
	m_VBOs_need_update = true;
}


//--------------------------------------------------------------------------
inline void XRayDetector::setDetectorPosition(const VEC3& aDetectorPosition)
//--------------------------------------------------------------------------
{
	// The position of the source has changed
	m_detector_position = aDetectorPosition;

	// Update the distance between the source and the detector, etc.
	updateInternalValues();
	
	// The VBOs need to be updated
	m_VBOs_need_update = true;
}


//-----------------------------------------------------------
inline void XRayDetector::setUpVector(const VEC3& anUpVector)
//-----------------------------------------------------------
{
	m_up_vector = anUpVector;
	
	// The VBOs need to be updated
	m_VBOs_need_update = true;
}


//-----------------------------------------------------------------
inline void XRayDetector::setRotationMatrix(const MATRIX4& aMatrix)
//-----------------------------------------------------------------
{
	m_rotation_matrix = aMatrix;
	
	// The VBOs need to be updated
	m_VBOs_need_update = true;
}


//-----------------------------------------
inline void XRayDetector::setParallelBeam()
//-----------------------------------------
{
	// Save the source type
	m_source_type = PARALLEL;
	
	// Clear the sample set
	m_p_xray_source_position_set.clear();

	// Add the centre
	m_p_xray_source_position_set.push_back(m_xray_source_centre);
}


//-------------------------------------
inline void XRayDetector::setConeBeam()
//-------------------------------------
{
	// Save the source type
	m_source_type = CONE;
	
	// Clear the sample set
	m_p_xray_source_position_set.clear();

	// Add the centre
	m_p_xray_source_position_set.push_back(m_xray_source_centre);
}


//----------------------------------------
inline void XRayDetector::setPointSource()
//----------------------------------------
{
	setConeBeam();
}


//-----------------------------------------------------------------------------------
inline void XRayDetector::setProjectionZNear(const RATIONAL_NUMBER& aProjectionZNear)
//-----------------------------------------------------------------------------------
{
	m_projection_z_near = aProjectionZNear;
}


//---------------------------------------------------------------------------------
inline void XRayDetector::setProjectionZFar(const RATIONAL_NUMBER& aProjectionZFar)
//---------------------------------------------------------------------------------
{
	m_projection_z_far = aProjectionZFar;
}


//----------------------------------------------------------
inline const Vec2ui& XRayDetector::getNumberOfPixels() const
//----------------------------------------------------------
{
	return (m_size_in_number_of_pixels);
}


//--------------------------------------------------
inline RATIONAL_NUMBER XRayDetector::getFovy() const
//--------------------------------------------------
{
	return (m_field_of_view_y);
}


//------------------------------------------------------------
inline const VEC2& XRayDetector::getSizeInUnitOfLength() const
//------------------------------------------------------------
{
	return (m_size_in_unit_of_length);
}


//-------------------------------------------------------------
inline RATIONAL_NUMBER XRayDetector::getProjectionZNear() const
//-------------------------------------------------------------
{
	return (m_projection_z_near);
}


//------------------------------------------------------------
inline RATIONAL_NUMBER XRayDetector::getProjectionZFar() const
//------------------------------------------------------------
{
	return (m_projection_z_far);
}


//----------------------------------------------------------
inline const VEC3& XRayDetector::getDetectorPosition() const
//----------------------------------------------------------
{
	return (m_detector_position);
}


//----------------------------------------------------------
inline const VEC3& XRayDetector::getXraySourceCentre() const
//----------------------------------------------------------
{
	return (m_xray_source_centre);
}


//--------------------------------------------------------
inline UINT XRayDetector::getNumberOfSourceSamples() const
//--------------------------------------------------------
{
	return (m_p_xray_source_position_set.size());
}


//----------------------------------------------------------------------------
inline const VEC3& XRayDetector::getSourceSamplePosition(UINT aSampleID) const
//----------------------------------------------------------------------------
{
	return (m_p_xray_source_position_set[aSampleID]);
}


//--------------------------------------------------
inline const VEC3& XRayDetector::getUpVector() const
//--------------------------------------------------
{
	return (m_up_vector);
}


//-----------------------------------------------------
inline const VEC3& XRayDetector::getRightVector() const
//-----------------------------------------------------
{
	return (m_right_vector);
}


//-----------------------------------------------------------
inline const MATRIX4& XRayDetector::getRotationMatrix() const
//-----------------------------------------------------------
{
	return (m_rotation_matrix);
}


//-----------------------------------------------------------------
inline XRayDetector::SourceType XRayDetector::getSourceType() const
//-----------------------------------------------------------------
{
	return (m_source_type);
}


//-----------------------------------------------
inline bool XRayDetector::useParallelBeam() const
//-----------------------------------------------
{
	return (m_source_type == PARALLEL);
}


//------------------------------------
inline void XRayDetector::updateFovy()
//------------------------------------
{
	m_field_of_view_y = 2.0 * (180.0  / Utilities::PI) * atan2(RATIONAL_NUMBER(m_size_in_unit_of_length.getY() / 2.0), RATIONAL_NUMBER(m_distance_source_detector));
}


//--------------------------------------------------
inline void XRayDetector::updateSizeInUnitOfLength()
//--------------------------------------------------
{
	m_size_in_unit_of_length.setX(m_resolution_in_unit_of_length_per_pixel * m_size_in_number_of_pixels.getX());
	m_size_in_unit_of_length.setY(m_resolution_in_unit_of_length_per_pixel * m_size_in_number_of_pixels.getY());
}


//----------------------------------------------
inline void XRayDetector::updateInternalValues()
//----------------------------------------------
{
	// The distance between the source and the detector has changed
	m_distance_source_detector = (m_detector_position - m_xray_source_centre).length();

	// Update the size of the field of view
	updateFovy();
	
	// Compute the normal vector
	m_look_at_vector = m_detector_position - m_xray_source_centre;
	m_look_at_vector.normalize();
	m_right_vector = m_look_at_vector ^ m_up_vector;
	m_normal = m_up_vector ^ m_right_vector;
}


//------------------------------------------------------------------
inline const void XRayDetector::getPlaneEquation(RATIONAL_NUMBER& A,
												 RATIONAL_NUMBER& B,
												 RATIONAL_NUMBER& C,
												 RATIONAL_NUMBER& D)
//------------------------------------------------------------------
{
	VEC3 normal(m_rotation_matrix * m_normal);
	A = normal.getX();
	B = normal.getY();
	C = normal.getZ();
	D = A * m_detector_position.getX() + B * m_detector_position.getY() + C * m_detector_position.getZ();
}


inline const VEC3& XRayDetector::getLowerCorner() const
{
	return (m_lower_corner);
}


inline const VEC3& XRayDetector::getUpperCorner() const
{
	return (m_upper_corner);
}


} // namespace gVirtualXRay
