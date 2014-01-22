/**
********************************************************************************
*
*	@file		XRayDetector.h
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


#ifndef XRAY_DETECTOR_H
#define XRAY_DETECTOR_H


//******************************************************************************
//	Include
//******************************************************************************
#include <GL/glew.h>

#ifndef TYPES_H
#include "Types.h"
#endif

#ifndef MATRIX4X4_H
#include "Matrix4x4.h"
#endif

#ifndef VBO_H
#include "VBO.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {
	using namespace Utilities;
	using namespace Graphics;


//==============================================================================
/**
*	@class	XRayDetector
*	@brief	XRayDetector is a class to handle a X-ray detector.
*/
//==============================================================================
class XRayDetector
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	typedef enum SourceTypeTag
	{
		POINT,
		CONE,
		SQUARE,
		CUBE,
		LINE,
		PARALLEL,
		UNKNOWN,
	} SourceType;


	//--------------------------------------------------------------------------
	///	Default constructor.
	//--------------------------------------------------------------------------
	XRayDetector();
	

	//--------------------------------------------------------------------------
	///	Display the detector using OpenGL.
	//--------------------------------------------------------------------------
	void displayDetector();


	//--------------------------------------------------------------------------
	///	Display the source using OpenGL.
	//--------------------------------------------------------------------------
	void displaySource() const;


	//--------------------------------------------------------------------------
	///	Display the beam using OpenGL.
	//--------------------------------------------------------------------------
	void displayBeam();


	//--------------------------------------------------------------------------
	///	Set the pixel resolution (in units of length per pixel,
	///	i.e. the inverse of the pixel size)
	/**
	*	@param aResolution:	the pixel resolution
	*/
	//--------------------------------------------------------------------------
	void setResolutionInUnitsOfLengthPerPixel(const RATIONAL_NUMBER& aResolution);


	//--------------------------------------------------------------------------
	///	Set the number of pixels.
	/**
	*	@param aSizeInPixels:	the number of pixels
	*/
	//--------------------------------------------------------------------------
	void setNumberOfPixels(const Vec2ui& aSizeInPixels);


	//--------------------------------------------------------------------------
	///	Set the position of the X-ray source.
	/**
	*	@param aXraySourcePosition:	the position of the X-ray source
	*/
	//--------------------------------------------------------------------------
	void setXrayPointSource(const VEC3& aXraySourcePosition);


	//--------------------------------------------------------------------------
	///	Set the position of the X-ray detector.
	/**
	*	@param aDetectorPosition:	the position of the X-ray detector
	*/
	//--------------------------------------------------------------------------
	void setDetectorPosition(const VEC3& aDetectorPosition);


	void setCubicSource(const VEC3& aXraySourcePosition,
			UINT aNumberOfSamplesAlongOneDirection,
			RATIONAL_NUMBER aLength);


	void setSquareSource(const VEC3& aXraySourcePosition,
			UINT aNumberOfSamplesAlongOneDirection,
			RATIONAL_NUMBER aLength);


	void setLineSource(const VEC3& aXraySourcePosition,
			const VEC3& aDirection,
			UINT aNumberOfSamplesAlongOneDirection,
			RATIONAL_NUMBER aLength);


	//--------------------------------------------------------------------------
	///	Set the up vector that defines the detector's orientation.
	/**
	*	@param anUpVector:	the up vector
	*/
	//--------------------------------------------------------------------------
	void setUpVector(const VEC3& anUpVector);


	//--------------------------------------------------------------------------
	///	Set the rotation matrix.
	/**
	*	@param aMatrix:	the rotation matrix
	*/
	//--------------------------------------------------------------------------
	void setRotationMatrix(const MATRIX4& aMatrix);


	//--------------------------------------------------------------------------
	///	Set the projection mode.
	/**
	*	@aFlag: it is true if a parallel beam is used (e.g. orthographic projection).
	*			it is false if perspective projection is used.
	*			(default value: true)
	*/
	//--------------------------------------------------------------------------
    void setParallelBeam();


	//--------------------------------------------------------------------------
	///	Set the projection mode.
	/**
	*	@aFlag: it is true if perspective projection is used.
	*			it is false if a parallel beam is used (e.g. orthographic projection).
	*			(default value: true)
	*/
	//--------------------------------------------------------------------------
    void setConeBeam();


	//--------------------------------------------------------------------------
	///	Set the projection mode.
	/**
	*	@aFlag: it is true if perspective projection is used.
	*			it is false if a parallel beam is used (e.g. orthographic projection).
	*			(default value: true)
	*/
	//--------------------------------------------------------------------------
    void setPointSource();


    //--------------------------------------------------------------------------
	///	Set the distance from the source to the near clipping plane
	///	(always positive)
	/**
	*	@param aProjectionZNear:	the distance from the source to the near clipping plane
	*/
	//--------------------------------------------------------------------------
	void setProjectionZNear(const RATIONAL_NUMBER& aProjectionZNear);


	//--------------------------------------------------------------------------
	///	Set the distance from the source to the far clipping plane
	///	(always positive)
	/**
	*	@param aProjectionZFar:	the distance from the source to the far clipping plane
	*/
	//--------------------------------------------------------------------------
	void setProjectionZFar(const RATIONAL_NUMBER& aProjectionZFar);
	

	//--------------------------------------------------------------------------
	///	Accessor on the number of pixels.
	/**
	*	@return the number of pixels
	*/
	//--------------------------------------------------------------------------
	const Vec2ui& getNumberOfPixels() const;


	//--------------------------------------------------------------------------
	///	Accessor on the field of view angle, in degrees, in the y direction.
	/**
	*	@return the field of view angle in the y direction
	*/
	//--------------------------------------------------------------------------
	RATIONAL_NUMBER getFovy() const;


	//--------------------------------------------------------------------------
	///	Accessor on the size of the detector (in unit of length).
	/**
	*	@return the size of the detector
	*/
	//--------------------------------------------------------------------------
	const VEC2& getSizeInUnitOfLength() const;


	//--------------------------------------------------------------------------
	///	Accessor on the distance from the source to the near clipping plane
	///	(always positive)
	/**
	*	@return the distance from the source to the near clipping plane
	*/
	//--------------------------------------------------------------------------
	RATIONAL_NUMBER getProjectionZNear() const;


	//--------------------------------------------------------------------------
	///	Accessor on the distance from the source to the far clipping plane
	///	(always positive)
	/**
	*	@return the distance from the source to the far clipping plane
	*/
	//--------------------------------------------------------------------------
	RATIONAL_NUMBER getProjectionZFar() const;


	//--------------------------------------------------------------------------
	///	Accessor on the detector position.
	/**
	*	@return the detector position
	*/
	//--------------------------------------------------------------------------
	const VEC3& getDetectorPosition() const;


	//--------------------------------------------------------------------------
	///	Accessor on the centre of the source.
	/**
	*	@return the source position
	*/
	//--------------------------------------------------------------------------
	const VEC3& getXraySourceCentre() const;


	UINT getNumberOfSourceSamples()const;
	const VEC3& getSourceSamplePosition(UINT aSampleID) const;


	//--------------------------------------------------------------------------
	///	Accessor on the up vector, which defines the orientation of the detector.
	/**
	*	@return the up vector
	*/
	//--------------------------------------------------------------------------
	const VEC3& getUpVector() const;


	//--------------------------------------------------------------------------
	///	Accessor on the rotation matrix.
	/**
	*	@return the rotation matrix
	*/
	//--------------------------------------------------------------------------
	const MATRIX4& getRotationMatrix() const;


	SourceType getSourceType() const;


	//--------------------------------------------------------------------------
	///	Accessor on the flag about projection.
	/**
	*	@return true if a parallel beam is used (e.g. orthographic projection),
	*			false if perspective projection is used
	*/
	//--------------------------------------------------------------------------
	bool useParallelBeam() const;


//******************************************************************************
protected:
	//--------------------------------------------------------------------------
	///	Compute the current value of the field of view.
	//--------------------------------------------------------------------------
	void updateFovy();


	//--------------------------------------------------------------------------
	///	Compute the size of the detector.
	//--------------------------------------------------------------------------
	void updateSizeInUnitOfLength();


	//--------------------------------------------------------------------------
	///	Compute the distance between the source and the detector.
	//--------------------------------------------------------------------------
	void updateDistanceSourceDetector();


	//--------------------------------------------------------------------------
	///	Update the VBOs.
	//--------------------------------------------------------------------------
	void updateVBOs();


	//--------------------------------------------------------------------------
	///	Update the VBO of the detector.
	//--------------------------------------------------------------------------
	void updateDetectorVBO();


	//--------------------------------------------------------------------------
	///	Update the VBO of the parallel beam.
	//--------------------------------------------------------------------------
	void updateParallelBeamVBO();


	//--------------------------------------------------------------------------
	///	Update the VBO of the cone beam.
	//--------------------------------------------------------------------------
	void updateConeBeamVBO();


 	/// The position of the detector centre
	VEC3            m_detector_position;
	

	/// The position of the source centre
	VEC3            m_xray_source_centre;


	/// The sample positions of the source
	std::vector<VEC3> m_p_xray_source_position_set;
	

	/// The "up" vector the the detector
	VEC3            m_up_vector;
	

	/// The rotation matrix
	MATRIX4 		m_rotation_matrix;


	/// The distance from the source to the near clipping plane (always positive)
	RATIONAL_NUMBER m_projection_z_near;
	

	/// The distance from the source to the far clipping plane (always positive)
	RATIONAL_NUMBER m_projection_z_far;
	

	/// The size of the detector
	VEC2            m_size_in_unit_of_length;
	

	/// Resolution (in unit of length per pixel)
	RATIONAL_NUMBER m_resolution_in_unit_of_length_per_pixel;
	

	/// Distance between the source and the detector
	RATIONAL_NUMBER m_distance_source_detector;
	

	/// Specifies the field of view angle, in degrees, in the y direction
	RATIONAL_NUMBER m_field_of_view_y;
	

	/// Size of the detector (in number of pixels)
	Vec2ui          m_size_in_number_of_pixels;


	/// A flag set to true when the VBOs need to be updated
	bool m_VBOs_need_update;


	/// The triangle mesh corresponding to the detector
	VBO m_detector_geometry;


	/// The triangle mesh corresponding to the parallel beam
	VBO m_parallel_beam;


	/// The triangle mesh corresponding to the cone beam
	VBO m_cone_beam;


	/// The triangle mesh corresponding to the parallel beam highlight
	VBO m_parallel_beam_highlight;


	/// The triangle mesh corresponding to the cone beam highlight
	VBO m_cone_beam_highlight;


	/// Source type
	SourceType m_source_type;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "XRayDetector.inl"


#endif // XRAY_DETECTOR_H
