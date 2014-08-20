/**
********************************************************************************
*
*	@file		XRayDetector.h
*
*	@brief		Class to handle virtual X-Ray detectors.
*
*	@version	1.2
*
*	@date		30/11/2013
*
*	@author		Dr Franck P. Vidal
*
*	@todo		Create a class to draw spheres
*				Use this class to display point sources (see line 80 in file XRayDetector.inl)
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


#ifndef GVXR_XRAY_DETECTOR_H
#define GVXR_XRAY_DETECTOR_H


//******************************************************************************
//	Include
//******************************************************************************
#include <boost/scoped_ptr.hpp>

#ifndef TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef GVXR_MATRIX4X4_H
#include "gVirtualXRay/Matrix4x4.h"
#endif

#ifndef GVXR_VBO_H
#include "gVirtualXRay/VBO.h"
#endif

#ifndef GVXR_SPHERE_MESH_H
#include "gVirtualXRay/SphereMesh.h"
#endif

#ifndef GVXR_UBE_MESH_H
#include "gVirtualXRay/CubeMesh.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


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
	///	SourceShape is a type that defines the shape of X-ray sources.
	typedef enum SourceShapeTag
	{
		POINT,        ///< Infinitively small point source
		SQUARE,       ///< Source whose shape corresponds to a square
		CUBE,         ///< Source whose shape corresponds to a cube
		LINE,         ///< Source whose shape corresponds to a line segment
		PARALLEL,     ///< Parallel source, i.e. a point source infinitely far away
		UNKNOWN_SHAPE ///< Unknown type
	} SourceShape;


	//--------------------------------------------------------------------------
	///	Default constructor.
	//--------------------------------------------------------------------------
	XRayDetector();
	

	//--------------------------------------------------------------------------
	///	Destructor.
	//--------------------------------------------------------------------------
	~XRayDetector();


	//--------------------------------------------------------------------------
	///	Display the detector using OpenGL.
	//--------------------------------------------------------------------------
	void displayDetector();


	//--------------------------------------------------------------------------
	///	Display the source using OpenGL.
	//--------------------------------------------------------------------------
	void displaySource();


	//--------------------------------------------------------------------------
	///	Display the beam using OpenGL.
	//--------------------------------------------------------------------------
	void displayBeam();


    //--------------------------------------------------------------------------
    /// Set the pixel resolution (in units of length per pixel,
    /// i.e. the inverse of the pixel size)
    /**
    *   @param aResolution: the pixel resolution
    */
    //--------------------------------------------------------------------------
    void setResolutionInUnitsOfLengthPerPixel(const RATIONAL_NUMBER& aResolution);


    //--------------------------------------------------------------------------
    /// Set the pixel resolution (in units of length per pixel,
    /// i.e. the inverse of the pixel size)
    /**
    *   @param aResolution: the pixel resolution
    */
    //--------------------------------------------------------------------------
    void setResolutionInUnitsOfLengthPerPixel(const VEC2& aResolution);


	//--------------------------------------------------------------------------
	///	Set the number of pixels.
	/**
	*	@param aSizeInPixels:	the number of pixels
	*/
	//--------------------------------------------------------------------------
	void setNumberOfPixels(const Vec2ui& aSizeInPixels);


	//--------------------------------------------------------------------------
	///	Set the position of the X-ray detector.
	/**
	*	@param aDetectorPosition:	the position of the X-ray detector
	*/
	//--------------------------------------------------------------------------
	void setDetectorPosition(const VEC3& aDetectorPosition);


	//--------------------------------------------------------------------------
	///	Set the position of the X-ray source.
	/**
	*	@param aXraySourcePosition:	the position of the X-ray source
	*/
	//--------------------------------------------------------------------------
	void setXrayPointSource(const VEC3& aXraySourcePosition);


	//--------------------------------------------------------------------------
	///	Set the shape of the source as a cube.
	/**
	*	@param aXraySourcePosition:	the position of the centre of the source
	*	@param aNumberOfSamplesAlongOneDirection:	the number of point sources along one axis (in total there will be aNumberOfSamplesAlongOneDirection^3 point sources in the cube)
	*	@param aLength:	the length of the cube
	*/
	//--------------------------------------------------------------------------
	void setCubicSource(const VEC3& aXraySourcePosition,
			unsigned int aNumberOfSamplesAlongOneDirection,
			RATIONAL_NUMBER aLength);


	//--------------------------------------------------------------------------
	///	Set the shape of the source as a square.
	/**
	*	@param aXraySourcePosition:	the position of the centre of the source
	*	@param aNumberOfSamplesAlongOneDirection:	the number of point sources along one axis (in total there will be aNumberOfSamplesAlongOneDirection^2 point sources in the square)
	*	@param aLength:	the length of the square
	*/
	//--------------------------------------------------------------------------
	void setSquareSource(const VEC3& aXraySourcePosition,
			unsigned int aNumberOfSamplesAlongOneDirection,
			RATIONAL_NUMBER aLength);


	//--------------------------------------------------------------------------
	///	Set the shape of the source as a line segment.
	/**
	*	@param aXraySourcePosition:	the position of the centre of the source
	*	@param aDirection:	the direction vector of the line
	*	@param aNumberOfSamplesAlongOneDirection:	the number of point sources
	*	@param aLength:	the length of the line
	*/
	//--------------------------------------------------------------------------
	void setLineSource(const VEC3& aXraySourcePosition,
			const VEC3& aDirection,
			unsigned int aNumberOfSamplesAlongOneDirection,
			RATIONAL_NUMBER aLength);


	//--------------------------------------------------------------------------
	///	Set the position of the X-ray source.
	//--------------------------------------------------------------------------
	void setXrayPointSource();


	//--------------------------------------------------------------------------
	///	Set the shape of the source as a cube.
	//--------------------------------------------------------------------------
	void setCubicSource();


	//--------------------------------------------------------------------------
	///	Set the shape of the source as a square.
	//--------------------------------------------------------------------------
	void setSquareSource();


	//--------------------------------------------------------------------------
	///	Set the shape of the source as a line segment.
	//--------------------------------------------------------------------------
	void setLineSource();


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
	///	Set the projection mode to parallel beam.
	//--------------------------------------------------------------------------
    void setParallelBeam();


	//--------------------------------------------------------------------------
	///	Set the projection mode to cone beam (same as point source).
	//--------------------------------------------------------------------------
    void setConeBeam();


	//--------------------------------------------------------------------------
	///	Set the projection mode to point source (same as cone beam).
	//--------------------------------------------------------------------------
    void setPointSource();


	//--------------------------------------------------------------------------
	///	Accessor on the number of pixels.
	/**
	*	@return the number of pixels
	*/
	//--------------------------------------------------------------------------
	const Vec2ui& getNumberOfPixels() const;


	//--------------------------------------------------------------------------
	///	Accessor on the size of the detector (in unit of length).
	/**
	*	@return the size of the detector
	*/
	//--------------------------------------------------------------------------
	const VEC2& getSizeInUnitOfLength() const;


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


	unsigned int getNumberOfSourceSamples()const;
	const VEC3& getSourceSamplePosition(unsigned int aSampleID) const;


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


	//--------------------------------------------------------------------------
	///	Accessor on the vector normal to the detector.
	//--------------------------------------------------------------------------
	const VEC3& getNormal() const;


	//--------------------------------------------------------------------------
	///	Accessor on the cross product between m_look_at_vector and m_up_vector.
	/**
	*	@return the cross product
	*/
	//--------------------------------------------------------------------------
	const VEC3& getRightVector() const;


	//--------------------------------------------------------------------------
	///	Accessor on the plane equation (Ax + By + Cz + D = 0).
	/**
	*	@param A: the 1st component of the plane equation
	*	@param B: the 2nd component of the plane equation
	*	@param C: the 3rd component of the plane equation
	*	@param D: the 4th component of the plane equation
	*/
	//--------------------------------------------------------------------------
	const void getPlaneEquation(RATIONAL_NUMBER& A,
			RATIONAL_NUMBER& B,
			RATIONAL_NUMBER& C,
			RATIONAL_NUMBER& D);


	//--------------------------------------------------------------------------
	///	Accessor on the shape of the source.
	/**
	*	@return true if a parallel beam is used (e.g. orthographic projection),
	*			false if perspective projection is used
	*/
	//--------------------------------------------------------------------------
	SourceShape getSourceShape() const;


	const std::vector<VEC3>& getSourcePositionSet() const;


	void setSourcePositions(const std::vector<VEC3>& aSourcePostionSet);


	//--------------------------------------------------------------------------
	///	Accessor on the flag about projection.
	/**
	*	@return true if a parallel beam is used (e.g. orthographic projection),
	*			false if perspective projection is used
	*/
	//--------------------------------------------------------------------------
	bool useParallelBeam() const;



	//--------------------------------------------------------------------------
	///	Accessor on the view along the Y-axis.
	/**
	*	@return the view along the Y-axis
	*/
	//--------------------------------------------------------------------------
	const RATIONAL_NUMBER& getFOVY() const;


//******************************************************************************
protected:
	//--------------------------------------------------------------------------
	///	Compute the size of the detector.
	//--------------------------------------------------------------------------
	void updateSizeInUnitOfLength();


	//--------------------------------------------------------------------------
	///	Compute the distance between the source and the detector, etc.
	//--------------------------------------------------------------------------
	void updateInternalValues();


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


	//--------------------------------------------------------------------------
	///	Update the view along the Y-axis.
	//--------------------------------------------------------------------------
	void updateFOVY();


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


	/// The size of the detector
	VEC2            m_size_in_unit_of_length;
	

	/// Resolution (in unit of length per pixel)
	VEC2 m_resolution_in_unit_of_length_per_pixel;
	

	/// Distance between the source and the detector
	RATIONAL_NUMBER m_distance_source_detector;
	

	/// Size of the detector (in number of pixels)
	Vec2ui          m_size_in_number_of_pixels;


	/// A flag set to true when the VBOs need to be updated
	bool m_VBOs_need_update;


	/// The triangle mesh corresponding to the detector
	boost::scoped_ptr<VBO> m_detector_geometry;


	/// The triangle mesh corresponding to the parallel beam
	boost::scoped_ptr<VBO> m_parallel_beam;


	/// The triangle mesh corresponding to the cone beam
	boost::scoped_ptr<VBO> m_cone_beam;


	/// The triangle mesh corresponding to the parallel beam highlight
	boost::scoped_ptr<VBO> m_parallel_beam_highlight;


	/// The triangle mesh corresponding to the cone beam highlight
	boost::scoped_ptr<VBO> m_cone_beam_highlight;


	/// Polygon mesh of the source
	SphereMesh m_source_mesh;


	/// Source type
	SourceShape m_source_shape;


	/// The vector normal to the detector
	VEC3 m_normal;


	/// The viewing direction (i.e. the unit vector from the X-ray source to the centre of the detector)
	VEC3 m_look_at_vector;


	/// The cross product between m_look_at_vector and m_up_vector
	VEC3 m_right_vector;


	/// Field of view along the Y-axis
	RATIONAL_NUMBER m_fovy;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "XRayDetector.inl"


#endif // XRAY_DETECTOR_H
