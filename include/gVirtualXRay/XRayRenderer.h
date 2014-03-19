/**
********************************************************************************
*
*	@file		XRayRenderer.h
*
*	@brief		Class to compute and renderer X-ray images on GPU.
*
*	@version	1.0
*
*	@date		01/12/2013
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


#ifndef XRAY_RENDERER_H
#define XRAY_RENDERER_H


//******************************************************************************
//	Include
//******************************************************************************
#include <vector>

#ifdef HAS_ITK
#include <itkImage.h>
#include <itkRGBPixel.h>
#endif

#ifndef TRIANGLE_MESH_H
#include "gVirtualXRay/TriangleMesh.h"
#endif

#ifndef XRAY_DETECTOR_H
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef XRAY_BEAM_H
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef SHADER_H
#include "gVirtualXRay/Shader.h"
#endif

#ifndef ATTENUATION_COEFFICIENT_H
#include "gVirtualXRay/AttenuationCoefficient.h"
#endif

#ifndef VBO_H
#include "gVirtualXRay/VBO.h"
#endif


//******************************************************************************
//	Definitions
//******************************************************************************
#define XRAY_DETECTOR_PBO_NUMBER        2	///< The total number of PBOs
#define XRAY_DETECTOR_2D_TEXTURE_NUMBER 3	///< The total number of 2D textures
#define XRAY_DETECTOR_3D_TEXTURE_NUMBER 1	///< The total number of 3D textures
#define XRAY_DETECTOR_SHADER_NUMBER     6	///< The total number of program shaders


class FramebufferObject;


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {
	using namespace Graphics;


//==============================================================================
/**
*	@class	XRayRenderer
*	@brief	XRayRenderer is a class to compute and renderer X-ray images on GPU.
*/
//==============================================================================
class XRayRenderer
//------------------------------------------------------------------------------
{
//******************************************************************************
protected:
	typedef RATIONAL_NUMBER PixelType; ///< Pixel type
#ifdef HAS_ITK
	typedef itk::Image<PixelType, 3> ImageType; ///< Image type
#endif // HAS_ITK


//******************************************************************************
public:
	//--------------------------------------------------------------------------
	///	Type to store the computation engine (OpenGL, CUDA, OpenCL, etc.).
	//--------------------------------------------------------------------------
	typedef enum ComputingModeTag
	{
		OPENGL,	///< Computation using OpenGL
		CUDA,	///< Computation using CUDA
		OPENCL	///< Computation using OpenCL
	} ComputingMode;


	//--------------------------------------------------------------------------
	///	Default constructor.
	//--------------------------------------------------------------------------
	XRayRenderer();
	

	//--------------------------------------------------------------------------
	///	Destructor.
	//--------------------------------------------------------------------------
	~XRayRenderer();


	//--------------------------------------------------------------------------
	///	Initialise the renderer.
	/**
	*	@param aComputingMode:	the computing mode (OPENGL, CUDA, or OPENCL)
	*	@param anInternalTextureFormat:	the internal format in which OpenGL textures will be stored (GL_RGB16F_ARB, or GL_RGB32F_ARB)
	*	@param apDetector:	the X-ray detector that will be used
	*	@param apBeamSpectrum:	the X-ray beam spectrum
	*/
	//--------------------------------------------------------------------------
	void initialise(ComputingMode aComputingMode,
			int anInternalTextureFormat,
			const XRayDetector* apDetector,
			const XRayBeam* apBeamSpectrum);


	//--------------------------------------------------------------------------
	///	Reset the renderer (release memory on the host and the device,
	///	and restore default values).
	//--------------------------------------------------------------------------
	void reset();


	//--------------------------------------------------------------------------
	///	Check if the renderer is ready to simulate X-ray images.
	/**
	*	@return true if the renderer is ready, false if it is not
	*/
	//--------------------------------------------------------------------------
	bool isReady() const;


	//--------------------------------------------------------------------------
	///	Add a 3D object to be rendered. It will be used as an internal surface.
	/**
	*	@param apSurface:	the 3D object
	*/
	//--------------------------------------------------------------------------
	void addInnerSurface(const TriangleMesh* apSurface);


	//--------------------------------------------------------------------------
	///	Add a 3D object to be rendered. It will be used as an external surface.
	/**
	*	@param apSurface:	the 3D object
	*/
	//--------------------------------------------------------------------------
	void addOuterSurface(const TriangleMesh* apSurface);


	//--------------------------------------------------------------------------
	///	Compute the L-buffer of a 3D object
	/**
	*	@param anObject:	the 3D object to process
	*	@param aXRaySourceDetectorTranslation:	the translation vector to use when the source is not aligned with the detector
	*	@param aModellingTransformationMatrix:	the transformation matrix of the 3D object
	*/
	//--------------------------------------------------------------------------
	void computeLBuffer(const TriangleMesh& anObject,
			  const VEC3& aXRaySourceDetectorTranslation,
			  const MATRIX4& aModellingTransformationMatrix);


	//--------------------------------------------------------------------------
	///	Compute the X-ray image.
	/**
	*	@param aModellingTransformationMatrix:	a modelling transformation
	*											matrix that will be used on all
	*											the 3D objects
	*/
	//--------------------------------------------------------------------------
	void computeImage(const MATRIX4& aModellingTransformationMatrix = MATRIX4());


	//--------------------------------------------------------------------------
	///	Save a FBO into a file.
	/**
	*	@param aFBOID:	the ID of the FBO
	*	@param aFileName:	the file name
	*/
	//--------------------------------------------------------------------------
	void printFBO(unsigned int aFBOID, const char* aFileName);


	//--------------------------------------------------------------------------
	///	Save a texture into a file.
	/**
	*	@param aTextureID:	the ID of the texture
	*	@param aFileName:	the file name
	*/
	//--------------------------------------------------------------------------
	void print2DTexture(unsigned int aTextureID, const char* aFileName);


	//--------------------------------------------------------------------------
	///	Save the current L-buffer into a file.
	/**
	*	@param aFileName:	the file name
	*/
	//--------------------------------------------------------------------------
	void printLBuffer(const char* aFileName);


	//--------------------------------------------------------------------------
	///	Save the current cumulated L-buffer into a file.
	/**
	*	@param aFileName:	the file name
	*/
	//--------------------------------------------------------------------------
	void printCumulatedLBuffer(const char* aFileName);


	//--------------------------------------------------------------------------
	///	Save the current Mux Dx into a file.
	/**
	*	@param aFileName:	the file name
	*/
	//--------------------------------------------------------------------------
	void printSumMuxDx(const char* aFileName);


	//--------------------------------------------------------------------------
	///	Save the current energy fluence into a file.
	/**
	*	@param aFileName:	the file name
	*/
	//--------------------------------------------------------------------------
	void printEnergyFluence(const char* aFileName);


	//--------------------------------------------------------------------------
	///	Display a 2D texture using OpenGL.
	//--------------------------------------------------------------------------
	void display2DTexture(unsigned int aTexutreID);


	//--------------------------------------------------------------------------
	///	Display the L-buffer using OpenGL.
	//--------------------------------------------------------------------------
	void displayLBuffer();


	//--------------------------------------------------------------------------
	///	Display the energy fluence using OpenGL.
	//--------------------------------------------------------------------------
	void displayEnergyFluence();


	//--------------------------------------------------------------------------
	///	Normalise the final result to display in OpenGL.
	//--------------------------------------------------------------------------
	void normalise();


	//--------------------------------------------------------------------------
	///	Display the X-ray image using OpenGL.
	//--------------------------------------------------------------------------
	void display();


	//--------------------------------------------------------------------------
	///	Get the attenuation coefficient of water for a given energy.
	///	The value is computed using linear interpolation.
	/**
	*	@param anIncidentEnergy:	the energy
	*	@return the corresponding attenuation coefficient of water
	*/
	//--------------------------------------------------------------------------
	RATIONAL_NUMBER getMuWaterLinearInterpolation(const RATIONAL_NUMBER& anIncidentEnergy);


	//--------------------------------------------------------------------------
	///	Get the attenuation coefficient of water for a given energy.
	///	The value is computed using linear interpolation on a logarithmic scale.
	/**
	*	@param anIncidentEnergy:	the energy
	*	@return the corresponding attenuation coefficient of water
	*/
	//--------------------------------------------------------------------------
	RATIONAL_NUMBER getMuWaterLogLinearInterpolation(const RATIONAL_NUMBER& anIncidentEnergy);


	//--------------------------------------------------------------------------
	///	Convert a Hounsfield value into an attenuation coefficient.
	/**
	*	@param HU:	the Hounsfield value to convert
	*	@param mu_water:	the attenuation coefficient of water at a given energy
	*	@return the corresponding attenuation coefficient
	*/
	//--------------------------------------------------------------------------
	RATIONAL_NUMBER convertHounsfieldValue2Mu(const RATIONAL_NUMBER& HU,
			const RATIONAL_NUMBER& mu_water) const;


	//--------------------------------------------------------------------------
	///	Display the final image in negative or not.
	/**
	*	@param aFlag:	true is the final image is displayed in negative, false it is not (default value: true)
	*/
	//--------------------------------------------------------------------------
	void useNegativeFilteringFlag(bool aFlag = true);


	//--------------------------------------------------------------------------
	///	Accessor on the flag control the display of the final in negative.
	/**
	*	@return true if the final image is displayed in negative, false it is not
	*/
	//--------------------------------------------------------------------------
	bool getNegativeFilteringFlag() const;


	//--------------------------------------------------------------------------
	///	Output the talbe of attenuation coefficients of water into a data stream.
	/**
	*	@param anOutputStream:	the output data stream
	*/
	//--------------------------------------------------------------------------
	void printMuWaterTable(std::ostream& anOutputStream);


	//--------------------------------------------------------------------------
	///	Accessor on the number of inner surfaces (e.g. an internal organ in case of a medical application).
	/**
	*	@return the number of inner surfaces
	*/
	//--------------------------------------------------------------------------
	unsigned int getNumberOfInnerSurfaces() const;


	//--------------------------------------------------------------------------
	///	Accessor on the number of outer surfaces (e.g. the skin surface in case of a medical application).
	///	This value is either equal to 0 or 1.
	/**
	*	@return the number of outer surfaces
	*/
	//--------------------------------------------------------------------------
	unsigned int getNumberOfOuterSurfaces() const;


	//--------------------------------------------------------------------------
	///	Display a given inner surface (e.g. an internal organ in case of a medical application).
	/**
	*	@param anIndex:	the index of the inner surface to display
	*/
	//--------------------------------------------------------------------------
	void displayInnerSurface(unsigned int anIndex) const;


	//--------------------------------------------------------------------------
	///	Display the outer surface (e.g. the skin surface in case of a medical application).
	//--------------------------------------------------------------------------
	void displayOuterSurface() const;


	//--------------------------------------------------------------------------
	///	Accessor on the smallest pixel intensity in the latest simulated X-ray image.
	/**
	*	@return the min value in simulated X-ray image
	*/
	//--------------------------------------------------------------------------
	RATIONAL_NUMBER getMinPixelValue() const;


	//--------------------------------------------------------------------------
	///	Accessor on the greatest pixel intensity in the latest simulated X-ray image.
	/**
	*	@return the max value in simulated X-ray image
	*/
	//--------------------------------------------------------------------------
	RATIONAL_NUMBER getMaxPixelValue() const;


//******************************************************************************
protected:
	//--------------------------------------------------------------------------
	///	Load the table of water attenuation coefficients.
	//--------------------------------------------------------------------------
	void loadMuWater();


	//--------------------------------------------------------------------------
	///	Check that the device supports all the required OpenGL extensions
	///	that are needed.
	//--------------------------------------------------------------------------
	void checkGPUCapabilities();


	//--------------------------------------------------------------------------
	///	Initialise the shaders.
	//--------------------------------------------------------------------------
	void initShaders();


	//--------------------------------------------------------------------------
	///	Create the FBOs.
	//--------------------------------------------------------------------------
	void createFBOs();


	//--------------------------------------------------------------------------
	///	Delete the FBOs.
	//--------------------------------------------------------------------------
	void deleteFBOs();


	//--------------------------------------------------------------------------
	///	Create a PBO.
	/**
	*	@param aPBO:	ID of the PBO to create
	*/
	//--------------------------------------------------------------------------
	void createPBO(unsigned int& aPBO);


	//--------------------------------------------------------------------------
	///	Delete a PBO.
	/**
	*	@param aPBO:	ID of the PBO to delete
	*/
	//--------------------------------------------------------------------------
	void deletePBO(unsigned int& aPBO);


	//--------------------------------------------------------------------------
	///	Initialise the PBOs.
	//--------------------------------------------------------------------------
	void initialisePBOs();


	//--------------------------------------------------------------------------
	///	Initialise the 2D textures.
	/**
	*	@param anInternalTextureFormat:	the internal format in which OpenGL textures will be stored (GL_RGB16F_ARB, or GL_RGB32F_ARB)
	*/
	//--------------------------------------------------------------------------
	void init2DTextures(int anInternalTextureFormat);


	//--------------------------------------------------------------------------
	///	Initialise the 3D textures.
	/**
	*	@param anInternalTextureFormat:	the internal format in which OpenGL textures will be stored (GL_RGB16F_ARB, or GL_RGB32F_ARB)
	*/
	//--------------------------------------------------------------------------
	void init3DData(int anInternalTextureFormat);


	//--------------------------------------------------------------------------
	///	Initialise a FBO (e.g. attach textures, etc.).
	/**
	*	@param aFBOIndex:	the FBO to initialise
	*	@param aTextureIndex:	the texture to attach
	*/
	//--------------------------------------------------------------------------
	void init2DFBO(unsigned int aFBOIndex, unsigned int aTextureIndex);


	//--------------------------------------------------------------------------
	///	Initialise a FBO with a 3D texture.
	//--------------------------------------------------------------------------
	void init3DFBOs();


	//--------------------------------------------------------------------------
	///	Create a 2D texture.
	/**
	*	@param apTextureID:	the address on the ID of the texture
	*	@param anInternalTextureFormat:	the internal format in which the texture will be stored in the device
	*/
	//--------------------------------------------------------------------------
	void create2DTexture(unsigned int* apTextureID,
			int anInternalTextureFormat);


	//--------------------------------------------------------------------------
	///	Reset the value of a FBO to 0.
	/**
	*	@param aFrameBuffer:	the FBO to reset
	*/
	//--------------------------------------------------------------------------
	void reset2DBuffer(unsigned int aFrameBuffer);


	//--------------------------------------------------------------------------
	///	Reset the L-buffer FBO to 0.
	//--------------------------------------------------------------------------
	void resetLBuffer();


	//--------------------------------------------------------------------------
	///	Reset the energy fluence FBO to 0.
	//--------------------------------------------------------------------------
	void resetEnergyFluence();


	//--------------------------------------------------------------------------
	///	Reset the Mu_x * D_x FBO to 0.
	//--------------------------------------------------------------------------
	void resetSumMuxDx();


	//--------------------------------------------------------------------------
	///	Compute the L-buffer of an internal object.
	/**
	*	@param anObject:						the 3D object to process
	*	@param aXRaySourceSampleOffset:			the translation vector to use when the source is not aligned with the detector
	*	@param aModellingTransformationMatrix:	the transformation matrix of the 3D object
	*/
	//--------------------------------------------------------------------------
	void computeInnerSurfaceLBuffer(const TriangleMesh& anObject,
			const VEC3& aXRaySourceSampleOffset,
			const MATRIX4& aModellingTransformationMatrix);


	//--------------------------------------------------------------------------
	///	Compute the L-buffer of an external object.
	/**
	*	@param anObject:						the 3D object to process
	*	@param aXRaySourceSampleOffset:			the translation vector to use when the source is not aligned with the detector
	*	@param aModellingTransformationMatrix:	the transformation matrix of the 3D object
	*/
	//--------------------------------------------------------------------------
	void computeOuterSurfaceLBuffer(const TriangleMesh& anObject,
			const VEC3& aXRaySourceSampleOffset,
			const MATRIX4& aModellingTransformationMatrix);


	//--------------------------------------------------------------------------
	///	Update the L-buffer of inner surfaces.
	///	It integrates the L-buffers of all the inner surfaces.
	/**
	*	@param anObjectID:	the ID of the surface to integrate. If anObjectID is
	*						equal to zero, the cumulated L-buffer is
	*						re-initialsed with the L-buffer of this surface.
	*						If anObjectID is greater than zero, L-buffer of this
	*						surface is integrated into the cumulated L-buffer.
	*/
	//--------------------------------------------------------------------------
	void updateCumulatedLBuffers(unsigned int anObjectID);


	//--------------------------------------------------------------------------
	///	Compute t.
	/**
	*	@param HU:
	*	@param anObjectID:
	*/
	//--------------------------------------------------------------------------
	void computeSumMUxDx(RATIONAL_NUMBER HU, unsigned int anObjectID);


	//--------------------------------------------------------------------------
	///	Compute the energy fluence.
	//--------------------------------------------------------------------------
	void computeIntegratedEnergy();


	//--------------------------------------------------------------------------
	///	Update the VBO of the geometry.
	//--------------------------------------------------------------------------
	void updateVBO();


	/// The X-ray detector
	XRayDetector* m_p_detector;


	/// The X-ray beam
	XRayBeam* m_p_xray_beam;


	/// Set of 3D geometries
	std::vector<TriangleMesh*> m_p_inner_surface_set;


	/// The 3D geometry that will wrap the ones in m_p_inner_surface_set
	TriangleMesh* m_p_outer_surface;


	/// The computing mode (OPENGL, CUDA, or OPENCL)
	ComputingMode m_computing_mode;


	/// Set of PBOs
	std::vector<unsigned int>        m_p_pbo_set;


	/// Set of textures
	std::vector<unsigned int> m_p_texture_name_set;


	/// Set of shaders
	std::vector<Shader> m_shader_set;


	/// The raw data of the last FBO that was loaded in the main memory
	PixelType* m_p_temp_raw_data;


	/// The table of water energy fluence coefficients
	std::vector<AttenuationCoefficient> m_mu_water_set;


	/// The set of FBOs
	std::vector<FramebufferObject*> m_p_fbo_set;


#ifdef HAS_ITK
	/// L-buffer
	ImageType::Pointer m_p_l_buffer_image;


	/// L-buffer
	ImageType::Pointer m_p_sum_mux_dx_image;


	/// EnergyFluence
	ImageType::Pointer m_p_energy_fluence_image;
#endif // HAS_ITK


	/// Min value in simulated X-ray image
	RATIONAL_NUMBER m_min_value_in_simulated_xray_image;


	/// Max value in simulated X-ray image
	RATIONAL_NUMBER m_max_value_in_simulated_xray_image;


	/// Flag to display the result in negative or not
	unsigned int m_negative_display_flag;


	/// Flag set to true to use L-buffer artefact filtering
	bool m_use_L_buffer_artefact_filtering;


	/// Flag use for lazy evaluation of the L-Buffer
	bool m_L_buffer_empty;


	/// Flag use for lazy evaluation and blending of the energy fluence
	bool m_energy_fluence_empty;


	/// The VBO used to display a rectangle corresponding to the detector
	VBO m_detector_temporary_geometry;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "XRayRenderer.inl"


#endif // XRAY_RENDERER_H
