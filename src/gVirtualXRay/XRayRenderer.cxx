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


//******************************************************************************
//	Include
//******************************************************************************
#include <cfloat>
#include <math.h>
#include <cstring>

#ifdef HAS_ITK
#include <itkImageFileWriter.h>
#endif

#ifndef XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

#ifndef OUT_OF_MEMORY_EXCEPTION_H
#include "gVirtualXRay/OutOfMemoryException.h"
#endif

#ifndef OUT_OF_BOUNDS_EXCEPTION_H
#include "gVirtualXRay/OutOfBoundsException.h"
#endif

#ifndef FILE_DOES_NOT_EXIST_EXCEPTION_H
#include "gVirtualXRay/FileDoesNotExistException.h"
#endif

#ifndef OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#include "MUx_Dx.frag.h"
#include "MUx_Dx.vert.h"

#include "cumulated_l_buffer.frag.h"
#include "cumulated_l_buffer.vert.h"

#include "l_buffer.frag.h"
#include "l_buffer.vert.h"

#include "outer_surface_l_buffer.frag.h"
#include "outer_surface_l_buffer.vert.h"

#include "xray_attenuation.frag.h"
#include "xray_attenuation.vert.h"

#include "xray_display.frag.h"
#include "xray_display.vert.h"

#include "mu_water.csv.h"


//******************************************************************************
//	define
//******************************************************************************
// Counters
#define XRAY_DETECTOR_2D_FBO_NUMBER     3
#define XRAY_DETECTOR_3D_FBO_NUMBER     1

// 2D FBOs first, then 3D FBOs
#define XRAY_DETECTOR_LBUFFER_FBO_ID                     0
#define XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID          1
#define XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID              2
#define XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID                  3

// 2D textures first, then 3D textures
#define XRAY_DETECTOR_LBUFFER_TEXTURE_NAME_ID            0
#define XRAY_DETECTOR_CUMULATED_LBUFFERS_TEXTURE_NAME_ID 1
#define XRAY_DETECTOR_ENERGY_FLUENCE_TEXTURE_NAME_ID     2
#define XRAY_DETECTOR_SUM_MUx_Dx_TEXTURE_NAME_ID         3

#define XRAY_DETECTOR_LBUFFER_PBO_ID                     0
#define XRAY_DETECTOR_ENERGY_FLUENCE_PBO_ID              1

#define XRAY_DETECTOR_LBUFFER_SHADER_ID                  0
#define XRAY_DETECTOR_CUMULATED_LBUFFER_SHADER_ID        1
#define XRAY_DETECTOR_OUTER_SURFACE_LBUFFER_SHADER_ID    2
#define XRAY_DETECTOR_SUM_MUx_Dx_SHADER_ID               3
#define XRAY_DETECTOR_ENERGY_FLUENCE_SHADER_ID           4
#define XRAY_DETECTOR_DISPLAY_SHADER_ID                  5


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//-------------------------------
XRayRenderer::XRayRenderer():
//-------------------------------
		m_p_detector(0),
		m_p_xray_beam(0),
		m_p_outer_surface(0),
		m_computing_mode(OPENGL),
		m_p_pbo_set(XRAY_DETECTOR_PBO_NUMBER, 0),
		m_p_texture_name_set(XRAY_DETECTOR_2D_TEXTURE_NUMBER + XRAY_DETECTOR_3D_TEXTURE_NUMBER, 0),
		m_shader_set(XRAY_DETECTOR_SHADER_NUMBER),
		m_p_temp_raw_data(0),
#ifdef HAS_ITK
		m_p_l_buffer_image(ImageType::New()),
		m_p_sum_mux_dx_image(ImageType::New()),
		m_p_energy_fluence_image(ImageType::New()),
#endif // HAS_ITK
		m_shift_filter(0),
		m_scale_filter(1),
		m_negative_display_flag(1),
		m_use_L_buffer_artefact_filtering(true),
		m_L_buffer_empty(false)
//-------------------------------
{
}


//---------------------------
XRayRenderer::~XRayRenderer()
//---------------------------
{
	reset();
}


//----------------------------------------------------------------------------
void XRayRenderer::initialise(ComputingMode aComputingMode,
							  int anInternalTextureFormat,
							  const XRayDetector* apDetector,
							  const XRayBeam* apBeamSpectrum)
//----------------------------------------------------------------------------
{
	// Check the texture format
	if (anInternalTextureFormat != GL_RGB16F_ARB &&
			anInternalTextureFormat != GL_RGB32F_ARB)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid texture format, it has to be GL_RGB16F_ARB or GL_RGB32F_ARB.");
	}

	// Reset the X-ray detector
	reset();

	// Set the detector
	m_p_detector = const_cast<XRayDetector*>(apDetector);

	// Load mu water table
	loadMuWater();

	// Set the beam spectrum
	m_p_xray_beam = const_cast<XRayBeam*>(apBeamSpectrum);

	// Set the computing mode
	m_computing_mode = aComputingMode;

#ifdef HAS_CUDA
	// Initialise CUDA if needed
	if (m_computing_mode == CUDA)
	{
		if (InitialiseCUDA())
		{
			std::cout << "CUDA: initialised." << std::endl;
		}
		else
		{
			std::cout << "CUDA ERROR: cannot initialise CUDA, set GPU computing using OpenGL." << std::endl;
			m_computing_mode = OPENGL;
		}
	}
#endif // HAS_CUDA

	// Check that the GPU is suitable
	checkGPUCapabilities();

	// Initialise shaders
	initShaders();

	// Create FBOs
	createFBOs();

	// Initialise textures
	init2DTextures(anInternalTextureFormat);
	init3DData(anInternalTextureFormat);

	// Initialise FBOs
	init2DFBO(XRAY_DETECTOR_LBUFFER_FBO_ID, XRAY_DETECTOR_LBUFFER_TEXTURE_NAME_ID);
	init2DFBO(XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID, XRAY_DETECTOR_CUMULATED_LBUFFERS_TEXTURE_NAME_ID);

	switch (m_computing_mode)
	{
	case OPENGL:
		init2DFBO(XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID, XRAY_DETECTOR_ENERGY_FLUENCE_TEXTURE_NAME_ID);
		init3DFBOs();
		break;

	case CUDA:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "CUDA is not supported.");
		break;

	case OPENCL:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "OpenCL is not supported.");
		break;

	default:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
		break;
	}

#ifdef HAS_ITK
	// Compute the size of a pixel
	ImageType::SpacingType pixel_size;
	pixel_size[0] = m_p_detector->getSizeInUnitOfLength().getX() / m_p_detector->getNumberOfPixels().getX();
	pixel_size[1] = m_p_detector->getSizeInUnitOfLength().getY() / m_p_detector->getNumberOfPixels().getY();
	pixel_size[2] = 1;

	// Convert the size in mm
	pixel_size[0] /= mm;
	pixel_size[1] /= mm;

	// Create a new image
	ImageType::IndexType start;
	start[0] = 0;
	start[1] = 0;
	start[2] = 0;

	ImageType::SizeType l_buffer_size;
	ImageType::SizeType sum_mu_x_d_x_size;
	ImageType::SizeType energy_fluence_size;

	l_buffer_size[0] = sum_mu_x_d_x_size[0] = energy_fluence_size[0] = m_p_detector->getNumberOfPixels().getX();

	l_buffer_size[1] = sum_mu_x_d_x_size[1] = energy_fluence_size[1] = m_p_detector->getNumberOfPixels().getY();

	l_buffer_size[2]       = 3;
	sum_mu_x_d_x_size[2]   = m_p_xray_beam->getEnergyChannelNumber();
	energy_fluence_size[2] = 3;

	ImageType::RegionType l_buffer_region;
	ImageType::RegionType sum_mu_x_d_x_region;
	ImageType::RegionType energy_fluence_region;
	l_buffer_region.SetSize(l_buffer_size);
	sum_mu_x_d_x_region.SetSize(sum_mu_x_d_x_size);
	energy_fluence_region.SetSize(energy_fluence_size);

	l_buffer_region.SetIndex(start);
	sum_mu_x_d_x_region.SetIndex(start);
	energy_fluence_region.SetIndex(start);

	// Allocate memory
	m_p_l_buffer_image->SetRegions(l_buffer_region);
	m_p_sum_mux_dx_image->SetRegions(sum_mu_x_d_x_region);
	m_p_energy_fluence_image->SetRegions(energy_fluence_region);

	m_p_l_buffer_image->Allocate();
	m_p_sum_mux_dx_image->Allocate();
	m_p_energy_fluence_image->Allocate();

	// Set the pixel size
	m_p_l_buffer_image->SetSpacing(pixel_size);
	m_p_sum_mux_dx_image->SetSpacing(pixel_size);
	m_p_energy_fluence_image->SetSpacing(pixel_size);
#endif // HAS_ITK

	// Create the VBO
	updateVBO();

	// Allocate memory
	unsigned int depth(3);
	if (depth < m_p_xray_beam->getEnergyChannelNumber())
	{
		depth = m_p_xray_beam->getEnergyChannelNumber();
	}
	m_p_temp_raw_data = new PixelType[m_p_detector->getNumberOfPixels().getX() *
	                                  m_p_detector->getNumberOfPixels().getY() * depth];

	// The memory is not allocated
	if (!m_p_temp_raw_data)
	{
		throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
	}
}


//------------------------------
void XRayRenderer::loadMuWater()
//------------------------------
{
	// Clear the data
	m_mu_water_set.clear();

	char* p_dest(0);
	int z_lib_return_code = inflate(g_mu_water_csv, sizeof(g_mu_water_csv), &p_dest);

	// It does not exist
	if (!z_lib_return_code)
	{
		delete [] p_dest;
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "No mu water was decoded.");
	}

	// Open the file
	std::stringstream mu_stream;
	mu_stream << (char*)p_dest;
	delete [] p_dest;

	// It does not exist
	if (!mu_stream.str().size())
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "No mu water was provided.");
	}

	// Ignore the first line
	char first_line[1024];
	mu_stream.getline(first_line, 1024);

	// Process every record
	AttenuationCoefficient temp;
	while (!mu_stream.eof())
	{
		// Load the record
		mu_stream >> temp;

		// The record is valid
		if (!mu_stream.eof())
		{
			// Store the record
			m_mu_water_set.push_back(temp);
		}
	}
}


//------------------------
void XRayRenderer::reset()
//------------------------
{
	// Delete PBOs
	for (std::vector<GLuint>::iterator ite(m_p_pbo_set.begin());
			ite != m_p_pbo_set.end();
			++ite)
	{
		// Delete the PBO
		try
		{
			if (*ite != 0)
			{
				deletePBO(*ite);
			}
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}

		// Make sure the value is reset
		*ite = 0;
	}

	// Delete FBOs
	try
	{
		deleteFBOs();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	// Delete the textures
	for (std::vector<GLuint>::iterator ite(m_p_texture_name_set.begin());
			ite != m_p_texture_name_set.end();
			++ite)
	{
		try
		{
			if (*ite != 0)
			{
				glDeleteTextures(1, &(*ite));
				*ite = 0;
			}
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	// Delete the shaders
	for (std::vector<Shader>::iterator ite(m_shader_set.begin());
			ite != m_shader_set.end();
			++ite)
	{
		try
		{
			ite->reset();
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	// Reset values
	m_p_detector = 0;
	m_p_xray_beam = 0;
	m_p_inner_surface_set.clear();
	m_p_outer_surface = 0;
	m_computing_mode = OPENGL;
	m_mu_water_set.clear();
	m_p_fbo_set.clear();
	m_L_buffer_empty = false;

	if (m_p_temp_raw_data)
	{
		delete [] m_p_temp_raw_data;
		m_p_temp_raw_data = 0;
	}
}


//--------------------------------
bool XRayRenderer::isReady() const
//--------------------------------
{
	// There is no error so far
	bool error(false);

	if (m_p_detector)
	{
		// Check the distance source/detector
		if ((m_p_detector->getXraySourceCentre() - m_p_detector->getDetectorPosition()).length() < std::numeric_limits<RATIONAL_NUMBER>::epsilon())
		{
			std::cerr << "WARNING: the distance from the source to the far clipping plane is null." << std::endl;
			error = true;
		}

	    // Check the detector size
	    if (!m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY())
		{
			std::cerr << "WARNING: the detector size is invalid." << std::endl;
			error = true;
		}
	}
	else
	{
		std::cerr << "WARNING: the X-ray detector has not been registered." << std::endl;
		error = true;
	}

	// Check the shaders
	for (std::vector<Shader>::const_iterator ite(m_shader_set.begin());
			ite != m_shader_set.end();
			++ite)
	{
		// The shader is not valid
		if (!ite->isValid())
		{
			std::cerr << "WARNING: at least one of the shader programs is not valid." << std::endl;
			error = true;
		}
	}

	// Check the FBOs
	for (std::vector<FramebufferObject*>::const_iterator ite(m_p_fbo_set.begin());
			ite != m_p_fbo_set.end();
			++ite)
	{
		// The FBO is not valid
		FramebufferObject* p_fbo(const_cast<FramebufferObject*>(*ite));
		if (!p_fbo->IsValid())
		{
			std::cerr << "WARNING: at least one of the FBOs is not valid." << std::endl;
			error = true;
		}
	}

	// Check the geometries
	if (!m_p_outer_surface && !m_p_inner_surface_set.size())
	{
		std::cerr << "WARNING: there is no 3D object to render." << std::endl;
		error = true;
	}

	return (!error);
}


//---------------------------------------
void XRayRenderer::checkGPUCapabilities()
//---------------------------------------
{
	// Check for OpenGL version
	if (!GLEW_VERSION_2_0)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "GPU capability error: your graphic card driver does not support OpenGL 2.0.");
	}

	if (!glewIsSupported("GL_VERSION_2_0"))
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "GPU capability error: your graphic card driver does not support OpenGL 2.0.");
	}

	/*if (!glewIsSupported("GL_VERSION_3_2"))
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "GPU capability error: your graphic card driver does not support OpenGL 2.0.");
	}*/

	// Check for OpenGL extensions
	if (!GLEW_ARB_texture_rectangle)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "GPU capability error: your graphic card driver does not support rectangular textures (ARB_texture_rectangle).");
	}

	if (!GLEW_ARB_framebuffer_object)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "GPU capability error: your graphic card driver does not support framebuffer objects (EXT_framebuffer_object).");
	}

	if (!GLEW_ARB_texture_float)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "GPU capability error: your graphic card driver does not support floating point textures (ARB_texture_float).");
	}

	if (!GLEW_ARB_shader_objects || !GLEW_ARB_fragment_program || !GLEW_ARB_vertex_program || !GLEW_ARB_fragment_shader || !GLEW_ARB_vertex_shader)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "GPU capability error: your graphic card driver does not support shader programs (ARB_shader_objects, ARB_fragment_program, ARB_vertex_program, ARB_fragment_shader or ARB_vertex_shader).");
	}

	if (!GLEW_ARB_texture_non_power_of_two)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "GPU capability error: your graphic card driver does not support non power of two textures (ARB_texture_non_power_of_two).");
	}

	if (!GLEW_ARB_pixel_buffer_object)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "GPU capability error: your graphic card driver does not support pixel buffer objects (PBOs) (GL_ARB_pixel_buffer_object).");
	}
}


//----------------------------------------------------------------------------
void XRayRenderer::computeImage(const MATRIX4& aModellingTransformationMatrix)
//----------------------------------------------------------------------------
{
	// The renderer is ready
	if (isReady())
	{
		// Wait for all OpenGL execution to be completed
		glFinish();

		// Store OpenGL states
		GLint p_viewport[4];
		glGetIntegerv(GL_VIEWPORT, p_viewport);

		//glPushAttrib(GL_COLOR_BUFFER_BIT);

		// Store matrices
		pushProjectionMatrix();
		pushModelViewMatrix();

		// Update the viewport
		glViewport(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY());

		// Reset the attenuation
		resetEnergyFluence();

		// Clear the FBO
		resetSumMuxDx();

		// Process every sample of the X-ray source
		for (unsigned int sample_id(0);
				sample_id < m_p_detector->getNumberOfSourceSamples();
				++sample_id)
		{
			{
				// Get the position of the current sample
				VEC3 xray_source_sample_centre(m_p_detector->getXraySourceCentre());
				VEC3 xray_source_sample_position(m_p_detector->getSourceSamplePosition(sample_id));
				VEC3 xray_source_detector_translation(xray_source_sample_position - xray_source_sample_centre);

				// Only update the cummulated L-buffers if the outer surface exists
				if (m_p_outer_surface)
				{
					reset2DBuffer(XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID);
				}

				// For each object
				int object_number(0);
				for (std::vector<TriangleMesh*>::const_iterator ite(m_p_inner_surface_set.begin());
					ite != m_p_inner_surface_set.end();
					++ite, ++object_number)
				{
					// Compute its L-buffer
					computeLBuffer(*(*ite), xray_source_detector_translation, aModellingTransformationMatrix);

					// Integrate the energy for every channel
					RATIONAL_NUMBER HU((*ite)->getHounsfieldValue());
					computeSumMUxDx(HU, object_number);

					// Only update the cummulated L-buffers if the outer surface exists
					if (m_p_outer_surface)
					{
						computeCumulatedLBuffers(object_number);
					}
				}

				// Compute the L-buffer of the outer surface that wraps the inner objects
				if (m_p_outer_surface)
				{
					// Compute its L-buffer
					computeLBuffer(*m_p_outer_surface, xray_source_detector_translation, aModellingTransformationMatrix);

					// Integrate the energy for every channel
					RATIONAL_NUMBER HU(m_p_outer_surface->getHounsfieldValue());
					computeSumMUxDx(HU, 1);
				}

				computeIntegratedEnergy();
			}
		}

		// Restore the matrices
		popProjectionMatrix();
		popModelViewMatrix();

		// Restore OpenGL states
		//glPopAttrib();
		glViewport(p_viewport[0], p_viewport[1], p_viewport[2], p_viewport[3]);

		// Wait for all OpenGL execution to be completed
		glFinish();
	}
	// The renderer is not ready
	else
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "The renderer is not ready to simulate X-ray images.");
	}
}


//------------------------------
void XRayRenderer::initShaders()
//------------------------------
{
	// Store the current shader
	GLint current_program(0);
	glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);


	// Initialise the shaders
	char* p_vertex_shader(0);
	char* p_fragment_shader(0);

	int z_lib_return_code_vertex(0);
	int z_lib_return_code_fragment(0);

	std::string vertex_shader;
	std::string fragment_shader;

	// L-buffer
	z_lib_return_code_vertex   = inflate(g_l_buffer_vert, sizeof(g_l_buffer_vert),   &p_vertex_shader);
	z_lib_return_code_fragment = inflate(g_l_buffer_frag, sizeof(g_l_buffer_frag), &p_fragment_shader);

	vertex_shader   = p_vertex_shader;
	fragment_shader = p_fragment_shader;
	delete [] p_vertex_shader;     p_vertex_shader = 0;
	delete [] p_fragment_shader; p_fragment_shader = 0;

	if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
	}
	m_shader_set[XRAY_DETECTOR_LBUFFER_SHADER_ID].loadSource(vertex_shader, fragment_shader);

	// Cumulated L-buffer
	z_lib_return_code_vertex   = inflate(g_cumulated_l_buffer_vert, sizeof(g_cumulated_l_buffer_vert),   &p_vertex_shader);
	z_lib_return_code_fragment = inflate(g_cumulated_l_buffer_frag, sizeof(g_cumulated_l_buffer_frag), &p_fragment_shader);

	vertex_shader   = p_vertex_shader;
	fragment_shader = p_fragment_shader;
	delete [] p_vertex_shader;     p_vertex_shader = 0;
	delete [] p_fragment_shader; p_fragment_shader = 0;

	if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
	}
	m_shader_set[XRAY_DETECTOR_CUMULATED_LBUFFER_SHADER_ID].loadSource(vertex_shader, fragment_shader);

	// Outer L-buffer
	z_lib_return_code_vertex   = inflate(g_outer_surface_l_buffer_vert, sizeof(g_outer_surface_l_buffer_vert),   &p_vertex_shader);
	z_lib_return_code_fragment = inflate(g_outer_surface_l_buffer_frag, sizeof(g_outer_surface_l_buffer_frag), &p_fragment_shader);

	vertex_shader   = p_vertex_shader;
	fragment_shader = p_fragment_shader;
	delete [] p_vertex_shader;     p_vertex_shader = 0;
	delete [] p_fragment_shader; p_fragment_shader = 0;

	if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
	}
	m_shader_set[XRAY_DETECTOR_OUTER_SURFACE_LBUFFER_SHADER_ID].loadSource(vertex_shader, fragment_shader);

	// Mu X Dx
	z_lib_return_code_vertex   = inflate(g_MUx_Dx_vert, sizeof(g_MUx_Dx_vert),   &p_vertex_shader);
	z_lib_return_code_fragment = inflate(g_MUx_Dx_frag, sizeof(g_MUx_Dx_frag), &p_fragment_shader);

	vertex_shader   = p_vertex_shader;
	fragment_shader = p_fragment_shader;
	delete [] p_vertex_shader;     p_vertex_shader = 0;
	delete [] p_fragment_shader; p_fragment_shader = 0;

	if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
	}
	m_shader_set[XRAY_DETECTOR_SUM_MUx_Dx_SHADER_ID].loadSource(vertex_shader, fragment_shader);

	// Energy fluence
	z_lib_return_code_vertex   = inflate(g_xray_attenuation_vert, sizeof(g_xray_attenuation_vert),   &p_vertex_shader);
	z_lib_return_code_fragment = inflate(g_xray_attenuation_frag, sizeof(g_xray_attenuation_frag), &p_fragment_shader);

	vertex_shader   = p_vertex_shader;
	fragment_shader = p_fragment_shader;
	delete [] p_vertex_shader;     p_vertex_shader = 0;
	delete [] p_fragment_shader; p_fragment_shader = 0;

	if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
	}
	m_shader_set[XRAY_DETECTOR_ENERGY_FLUENCE_SHADER_ID].loadSource(vertex_shader, fragment_shader);

	// Display
	z_lib_return_code_vertex   = inflate(g_xray_display_vert, sizeof(g_xray_display_vert),   &p_vertex_shader);
	z_lib_return_code_fragment = inflate(g_xray_display_frag, sizeof(g_xray_display_frag), &p_fragment_shader);

	vertex_shader   = p_vertex_shader;
	fragment_shader = p_fragment_shader;
	delete [] p_vertex_shader;     p_vertex_shader = 0;
	delete [] p_fragment_shader; p_fragment_shader = 0;

	if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
	}
	m_shader_set[XRAY_DETECTOR_DISPLAY_SHADER_ID].loadSource(vertex_shader, fragment_shader);

	// Restore the current shader
	glUseProgram(current_program);
}


//-----------------------------
void XRayRenderer::createFBOs()
//-----------------------------
{
	// Delete the FBOs if they already exist
	deleteFBOs();

	// Create the FBO (the number depends on the computing mode)
	switch (m_computing_mode)
	{
	case OPENGL:
		for (int i(0); i < XRAY_DETECTOR_2D_FBO_NUMBER + XRAY_DETECTOR_3D_FBO_NUMBER; ++i)
		{
			m_p_fbo_set.push_back(new FramebufferObject);
		}
		break;

	case CUDA:
#ifdef HAS_CUDA
		m_p_fbo_set.resize(XRAY_DETECTOR_2D_FBO_NUMBER - 1);
#endif // HAS_CUDA
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "CUDA is not supported.");
		break;

	case OPENCL:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "OpenCL is not supported.");
		break;

	default:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
		break;
	}
}


//----------------------------------------
void XRayRenderer::createPBO(GLuint& aPBO)
//----------------------------------------
{
	// Store the current buffer
	GLint current_buffer(0);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &current_buffer);

	// Set up vertex data parameter
	GLuint texture_size(m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());
	GLfloat* p_data(new GLfloat[texture_size]);

	// create buffer object
	glGenBuffers(1, &aPBO);
	glBindBuffer(GL_ARRAY_BUFFER, aPBO);
	glBufferData(GL_ARRAY_BUFFER, texture_size * sizeof(GLfloat), p_data, GL_DYNAMIC_DRAW);
	delete [] p_data;

#ifdef HAS_CUDA
	// register this buffer object with CUDA
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	cudaGLRegisterBufferObject(aPBO);
	if (CheckCudaError())
	{
		std::cerr << "FATAL ERROR: cannot bind a PBO to CUDA." << std::endl;
		return (false);
	}
#endif // HAS_CUDA

	// Restore the current buffer
	glBindBuffer(GL_ARRAY_BUFFER, current_buffer);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//----------------------------------------
void XRayRenderer::deletePBO(GLuint& aPBO)
//----------------------------------------
{
	// Store the current buffer
	GLint current_buffer(0);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &current_buffer);

	// Check the value
	if (current_buffer < 0)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid index.");
	}

	// Unbind the buffer if necessary
	if (GLuint(current_buffer) == aPBO)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

#ifdef HAS_CUDA
	// Unregister this buffer object with CUDA
	cudaGLUnregisterBufferObject(aPBO);
	if (CheckCudaError())
	{
		return_value = false;
	}
#endif // HAS_CUDA

	// Destroy the buffer
	glBindBuffer(GL_ARRAY_BUFFER, aPBO);
	glDeleteBuffers(1, &aPBO);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Unbind the buffer if necessary
	if (GLuint(current_buffer) == aPBO)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, current_buffer);
	}
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	aPBO = 0;
}


//--------------------------------------------------------
void XRayRenderer::init3DData(int anInternalTextureFormat)
//--------------------------------------------------------
{
	// Store the texturing flags
	pushTexture3D();

	switch (m_computing_mode)
	{
		case OPENGL:
		{
			// Generate the texture name.
			glGenTextures(1, &m_p_texture_name_set[XRAY_DETECTOR_2D_TEXTURE_NUMBER]);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			// Get the best anisotropic filtering level
			GLint max_anisotropic_fliter_level(1);
			glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropic_fliter_level);

			// Set the texture properties
			glBindTexture(GL_TEXTURE_3D, m_p_texture_name_set[XRAY_DETECTOR_2D_TEXTURE_NUMBER]);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropic_fliter_level);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			GLint width(0);
			glTexImage3D(GL_PROXY_TEXTURE_3D, 0, anInternalTextureFormat, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), m_p_xray_beam->getEnergyChannelNumber(), 0, GL_LUMINANCE, GL_FLOAT, 0);
			glGetTexLevelParameteriv(GL_PROXY_TEXTURE_3D, 0, GL_TEXTURE_WIDTH, &width);
			if ((unsigned int)(width) == m_p_detector->getNumberOfPixels().getX())
			{
				glTexImage3D(GL_TEXTURE_3D, 0, anInternalTextureFormat, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), m_p_xray_beam->getEnergyChannelNumber(), 0, GL_LUMINANCE, GL_FLOAT, 0);
			}
			else
			{
				std::stringstream error_message;
				error_message << "Cannot create a " << "(" << m_p_detector->getNumberOfPixels().getX() << ", " << m_p_detector->getNumberOfPixels().getY()  << ", " << m_p_xray_beam->getEnergyChannelNumber() << ") floating point render texture.";
				throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
			}

			// Check if any OpenGL error had occured
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			break;
		}

		case CUDA:
			throw Exception(__FILE__, __FUNCTION__, __LINE__, "CUDA is not supported.");
			break;

		case OPENCL:
			throw Exception(__FILE__, __FUNCTION__, __LINE__, "OpenCL is not supported.");
			break;

		default:
			throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
			break;
	}

	// Restore the texturing flags
	popTexture();
}


//------------------------------------------------------------------------------
void XRayRenderer::init2DFBO(unsigned int aFBOIndex, unsigned int aTextureIndex)
//------------------------------------------------------------------------------
{
	// Save the current FBO
	pushFBO();

	// Bind the FBO if needed
	m_p_fbo_set[aFBOIndex]->Bind();
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Attach color texture to FBO
	m_p_fbo_set[aFBOIndex]->AttachTexture(GL_TEXTURE_2D, m_p_texture_name_set[aTextureIndex], GL_COLOR_ATTACHMENT0_EXT);
	//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, m_2D_texture_target, m_p_texture_name_set[i], 0);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Change draw/read buffers
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Check the FBO
	if (!m_p_fbo_set[aFBOIndex]->IsValid())
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "The FBO has not been successfully created.");
	}

	// Clear the FBO
	//Reset2DBuffer(aFBOIndex);

	// Restore the current FBO
	popFBO();
}


//-----------------------------
void XRayRenderer::init3DFBOs()
//-----------------------------
{
	// Save the current FBO
	pushFBO();

	// Initialise FBOs
	for (int i(0); i < XRAY_DETECTOR_3D_FBO_NUMBER; ++i)
	{
		// Bind the FBO
		m_p_fbo_set[XRAY_DETECTOR_2D_FBO_NUMBER + i]->Bind();
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Attach color texture to FBO
		m_p_fbo_set[XRAY_DETECTOR_2D_FBO_NUMBER + i]->AttachTexture(GL_TEXTURE_3D, m_p_texture_name_set[XRAY_DETECTOR_2D_TEXTURE_NUMBER + i], GL_COLOR_ATTACHMENT0_EXT, 0, 0);
		//glFramebufferTexture3DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, m_3D_texture_target, m_p_texture_name_set[XRAY_DETECTOR_2D_TEXTURE_NUMBER + i], 0, 0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Change draw/read buffers
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
		glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Check the FBO
		if (!m_p_fbo_set[XRAY_DETECTOR_2D_FBO_NUMBER + i]->IsValid())
		{
			throw Exception(__FILE__, __FUNCTION__, __LINE__, "The FBO has not been successfully created.");
		}

		// Clear the FBO
		//Reset3DBuffer(XRAY_DETECTOR_2D_FBO_NUMBER + i);
	}

	// Restore the current FBO
	popFBO();
}


//-------------------------------------------------------------
void XRayRenderer::create2DTexture(GLuint* apTextureID,
								   int anInternalTextureFormat)
//-------------------------------------------------------------
{
	// Store the texturing flags
	pushTexture2D();
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Generate the texture name.
	glGenTextures(1, apTextureID);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Get the best anisotropic filtering level
	GLint max_anisotropic_fliter_level(1);
	glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropic_fliter_level);

	// Set the texture properties
	glBindTexture(GL_TEXTURE_2D, *apTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropic_fliter_level);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	GLint width(0);
	glTexImage2D(GL_PROXY_TEXTURE_2D, 0, anInternalTextureFormat, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), 0, GL_RGBA, GL_FLOAT, 0);
	glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	if ((unsigned int)(width) == m_p_detector->getNumberOfPixels().getX())
	{
		glTexImage2D(GL_TEXTURE_2D, 0, anInternalTextureFormat, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), 0, GL_RGBA, GL_FLOAT, 0);
	}
	else
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create a floating point render texture.");
	}

	// Check if any OpenGL error had occurred
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Restore the texturing flags
	popTexture();
}


//---------------------------------------------------
void XRayRenderer::reset2DBuffer(GLuint aFrameBuffer)
//---------------------------------------------------
{
	// Save the current FBO
	pushFBO();

	// Bind frame buffer object
	m_p_fbo_set[aFrameBuffer]->Bind();

	// Store the current clear color
	GLfloat p_clear_color[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, p_clear_color);

	// Save the viwport
	GLint p_viewport[4];
	glGetIntegerv(GL_VIEWPORT, &p_viewport[0]);

	// Change the viewport
	glViewport(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY());

	// Make sure the clear color is black (to reset FBOs to 0)
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Clear the frame buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// Wait for all OpenGL execution to be completed
	glFinish();

	// Restore
	glClearColor(p_clear_color[0], p_clear_color[1], p_clear_color[2], p_clear_color[3]);

	// Restore the viewport
	glViewport(p_viewport[0], p_viewport[1], p_viewport[2], p_viewport[3]);

	// Check if any OpenGL error had occured
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Restore the current FBO
	popFBO();
}


//-------------------------------
void XRayRenderer::resetLBuffer()
//-------------------------------
{
	// The L-buffer is not empty
	if (!m_L_buffer_empty)
	{
		switch (m_computing_mode)
		{
		case OPENGL:
			reset2DBuffer(XRAY_DETECTOR_LBUFFER_FBO_ID);
			break;

		case CUDA:
			throw Exception(__FILE__, __FUNCTION__, __LINE__, "CUDA is not supported.");
			break;

		case OPENCL:
			throw Exception(__FILE__, __FUNCTION__, __LINE__, "OpenCL is not supported.");
			break;

		default:
			throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
			break;
		}
	}

	// The L-buffer is now empty
	m_L_buffer_empty = true;
}


//-------------------------------------
void XRayRenderer::resetEnergyFluence()
//-------------------------------------
{
	// The energy fluence is not empty
	if (!m_energy_fluence_empty)
	{
		switch (m_computing_mode)
		{
		case OPENGL:
			reset2DBuffer(XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID);
			break;

		case CUDA:
			throw Exception(__FILE__, __FUNCTION__, __LINE__, "CUDA is not supported.");
			break;

		case OPENCL:
			throw Exception(__FILE__, __FUNCTION__, __LINE__, "OpenCL is not supported.");
			break;

		default:
			throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
			break;
		}
	}

	// The energy fluence is now empty
	m_energy_fluence_empty = true;
}


//--------------------------------
void XRayRenderer::resetSumMuxDx()
//--------------------------------
{
	switch (m_computing_mode)
	{
	case OPENGL:
		reset2DBuffer(XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID);
		break;

	case CUDA:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "CUDA is not supported.");
		break;

	case OPENCL:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "OpenCL is not supported.");
		break;

	default:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
		break;
	}
}


//------------------------------------------------------------------------------
void XRayRenderer::computeLBuffer(const TriangleMesh& anObject,
								  const VEC3& aXRaySourceDetectorTranslation,
							  	  const MATRIX4& aModellingTransformationMatrix)
//------------------------------------------------------------------------------
{
	// This is an internal mesh
	if (&anObject != m_p_outer_surface)
	{
		computeInnerSurfaceLBuffer(anObject, aXRaySourceDetectorTranslation, aModellingTransformationMatrix);
	}
	// This is the external mesh
	else
	{
		computeOuterSurfaceLBuffer(anObject, aXRaySourceDetectorTranslation, aModellingTransformationMatrix);
	}
}


//------------------------------------------------
void XRayRenderer::printFBO(GLuint aFBOID,
							const char* aFileName)
//------------------------------------------------
{
	// The FBO does not exist
	if (aFBOID >= m_p_fbo_set.size())
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	// Save the current FBO
	pushFBO();

	switch (aFBOID)
	{
	case XRAY_DETECTOR_LBUFFER_FBO_ID:
		// Bind frame buffer object
		m_p_fbo_set[aFBOID]->Bind();

		// Read data from the FBO
		glReadPixels(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), GL_RED,  GL_FLOAT,  m_p_temp_raw_data);
		glReadPixels(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), GL_GREEN, GL_FLOAT, m_p_temp_raw_data + m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());
		glReadPixels(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), GL_BLUE, GL_FLOAT,  m_p_temp_raw_data + 2 * m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());
		break;

	case XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID:
		// Read data from the FBO
		for (unsigned int slice_number(0);
				slice_number < m_p_xray_beam->getEnergyChannelNumber();
				++slice_number)
		{
			// Bind default frame buffer
			FramebufferObject::Disable();

			// Attach the texture
			m_p_fbo_set[XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID]->UnattachAll();
			m_p_fbo_set[XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID]->AttachTexture(GL_TEXTURE_3D, m_p_texture_name_set[XRAY_DETECTOR_SUM_MUx_Dx_TEXTURE_NAME_ID], GL_COLOR_ATTACHMENT0_EXT, 0, slice_number);

			// Bind the FBO
			m_p_fbo_set[XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID]->Bind();
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			glReadPixels(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), GL_RED,  GL_FLOAT, m_p_temp_raw_data + slice_number * m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());
		}
		break;

	case XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID:
	case XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID:
		// Bind frame buffer object
		m_p_fbo_set[aFBOID]->Bind();

		// Read data from the FBO
		glReadPixels(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), GL_RED,  GL_FLOAT, m_p_temp_raw_data);
		glReadPixels(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), GL_GREEN, GL_FLOAT, m_p_temp_raw_data + m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());
		glReadPixels(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), GL_BLUE, GL_FLOAT,  m_p_temp_raw_data + 2 * m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());
		break;

	default:
		{
			throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented, please contact Dr FP Vidal.");
		}
		break;
	}

	// Check OpenGL
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Restore the current FBO
	popFBO();

#ifdef HAS_ITK
	// The filename is not empty
	if (aFileName)
	{
		// Create the file writer
		typedef itk::ImageFileWriter<ImageType> ImageWriter;
		ImageWriter::Pointer p_writer(ImageWriter::New());

		// Get the destination pointer
		PixelType* p_pixel_set(0);

		// Get the raw pointer
		// Set the file writer
		switch (aFBOID)
		{
		case XRAY_DETECTOR_LBUFFER_FBO_ID:
			p_pixel_set = m_p_l_buffer_image->GetBufferPointer();
			memcpy(p_pixel_set, m_p_temp_raw_data, sizeof(PixelType) * m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY() * 3);

			p_writer->SetInput(m_p_l_buffer_image);
			break;

		case XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID:
			p_pixel_set = m_p_sum_mux_dx_image->GetBufferPointer();
			memcpy(p_pixel_set, m_p_temp_raw_data, sizeof(PixelType) * m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY() * m_p_xray_beam->getEnergyChannelNumber());

			p_writer->SetInput(m_p_sum_mux_dx_image);
			break;

		case XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID:
		case XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID:
			p_pixel_set = m_p_energy_fluence_image->GetBufferPointer();
			memcpy(p_pixel_set, m_p_temp_raw_data, sizeof(PixelType) * m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY() * 3) ;

			p_writer->SetInput(m_p_energy_fluence_image);
			break;

		default:
			{
				throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented, please contact Dr FP Vidal.");
			}
			break;
		}

		// Set the writer
		p_writer->SetFileName(aFileName);

		// Write the data into a file
		p_writer->Update();
	}
#endif // HAS_ITK
}


//------------------------------------------------------
void XRayRenderer::print2DTexture(GLuint aTextureID,
								  const char* aFileName)
//------------------------------------------------------
{
	// The texture does not exist
	if (aTextureID >= m_p_texture_name_set.size())
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	// Store the texturing flags
	pushTexture2D();
	pushTexture3D();

	GLint texture_id(m_p_texture_name_set[aTextureID]);
	switch (aTextureID)
	{
	case XRAY_DETECTOR_LBUFFER_TEXTURE_NAME_ID:
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, m_p_temp_raw_data);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_BLUE, GL_FLOAT, m_p_temp_raw_data + m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());
		break;

	case XRAY_DETECTOR_SUM_MUx_Dx_TEXTURE_NAME_ID:
		glBindTexture(GL_TEXTURE_3D, texture_id);
		glGetTexImage(GL_TEXTURE_3D, 0, GL_RED, GL_FLOAT, m_p_temp_raw_data);
		break;

	case XRAY_DETECTOR_ENERGY_FLUENCE_TEXTURE_NAME_ID:
	case XRAY_DETECTOR_CUMULATED_LBUFFERS_TEXTURE_NAME_ID:
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, m_p_temp_raw_data);
		break;

	default:
		{
			throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented, please contact Dr FP Vidal.");
		}
		break;
	}

	// Restore the texturing flags
	popTexture();
	popTexture();

#ifdef HAS_ITK
	// The filename is not empty
	if (aFileName)
	{
		// Create the file writer
		typedef itk::ImageFileWriter<ImageType> ImageWriter;
		ImageWriter::Pointer p_writer(ImageWriter::New());

		// Get the destination pointer
		PixelType* p_pixel_set(0);

		// Get the raw pointer
		// Set the file writer
		switch (aTextureID)
		{
		case XRAY_DETECTOR_LBUFFER_TEXTURE_NAME_ID:
			p_pixel_set = m_p_l_buffer_image->GetBufferPointer();
			memcpy(p_pixel_set, m_p_temp_raw_data, sizeof(PixelType) * m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY() * 3);

			p_writer->SetInput(m_p_l_buffer_image);
			break;

		case XRAY_DETECTOR_SUM_MUx_Dx_TEXTURE_NAME_ID:
			p_pixel_set = m_p_sum_mux_dx_image->GetBufferPointer();
			memcpy(p_pixel_set, m_p_temp_raw_data, sizeof(PixelType) * m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY() * m_p_xray_beam->getEnergyChannelNumber());

			p_writer->SetInput(m_p_sum_mux_dx_image);
			break;

		case XRAY_DETECTOR_ENERGY_FLUENCE_TEXTURE_NAME_ID:
		case XRAY_DETECTOR_CUMULATED_LBUFFERS_TEXTURE_NAME_ID:
			p_pixel_set = m_p_energy_fluence_image->GetBufferPointer();
			memcpy(p_pixel_set, m_p_temp_raw_data, sizeof(PixelType) * m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());

			p_writer->SetInput(m_p_energy_fluence_image);
			break;

		default:
			{
				throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented, please contact Dr FP Vidal.");
			}
			break;
		}

		// Set the writer
		p_writer->SetFileName(aFileName);

		// Write the data into a file
		p_writer->Update();
	}
#endif // HAS_ITK
}


//----------------------------------------------------
void XRayRenderer::printLBuffer(const char* aFileName)
//----------------------------------------------------
{
	//print2DTexture(XRAY_DETECTOR_LBUFFER_TEXTURE_NAME_ID, aFileName);
	printFBO(XRAY_DETECTOR_LBUFFER_FBO_ID, aFileName);
}


//-------------------------------------------------------------
void XRayRenderer::printCumulatedLBuffer(const char* aFileName)
//-------------------------------------------------------------
{
	//print2DTexture(XRAY_DETECTOR_LBUFFER_TEXTURE_NAME_ID, aFileName);
	printFBO(XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID, aFileName);
}


//--------------------------------------------------
void XRayRenderer::printSumMuxDx(const char* aFileName)
//--------------------------------------------------
{
	//print2DTexture(XRAY_DETECTOR_SUM_MUx_Dx_TEXTURE_NAME_ID, aFileName);
	printFBO(XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID, aFileName);
}


//----------------------------------------------------------
void XRayRenderer::printEnergyFluence(const char* aFileName)
//----------------------------------------------------------
{
	//print2DTexture(XRAY_DETECTOR_ENERGY_FLUENCE_TEXTURE_NAME_ID, aFileName);
	printFBO(XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID, aFileName);
}


//---------------------------------------------------
void XRayRenderer::display2DTexture(GLint aTexutreID)
//---------------------------------------------------
{
	// Store the texturing flags
	pushTexture2D();

	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, m_p_texture_name_set[aTexutreID]);

	// Display the detector
	m_p_detector->displayDetector();

	// Store the texturing flags
	popTexture();

	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//---------------------------------
void XRayRenderer::displayLBuffer()
//---------------------------------
{
	display2DTexture(XRAY_DETECTOR_LBUFFER_TEXTURE_NAME_ID);
}


//---------------------------------------
void XRayRenderer::displayEnergyFluence()
//---------------------------------------
{
	display2DTexture(XRAY_DETECTOR_ENERGY_FLUENCE_TEXTURE_NAME_ID);
}


//------------------------------------------------------------------------------------------
void XRayRenderer::computeInnerSurfaceLBuffer(const TriangleMesh& anObject,
											  const VEC3& aXRaySourceSampleOffset,
											  const MATRIX4& aModellingTransformationMatrix)
//------------------------------------------------------------------------------------------
{
	// Save the current FBO
	pushFBO();

	// Store the state
	pushEnableDisableState(GL_BLEND);
	pushEnableDisableState(GL_DEPTH_TEST);

	// Save the viwport
	GLint p_viewport[4];
	glGetIntegerv(GL_VIEWPORT, &p_viewport[0]);

	// Change the viewport
	glViewport(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY());

	// Reset its L-buffer
	resetLBuffer();

	// Bind frame buffer object
	m_p_fbo_set[XRAY_DETECTOR_LBUFFER_FBO_ID]->Bind();
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Disable depth buffer
	glDisable(GL_DEPTH_TEST);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Activate the first shader
	pushShaderProgram();
	m_shader_set[XRAY_DETECTOR_LBUFFER_SHADER_ID].enable();
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Values to pass to the shader
	GLhandleARB program_handle(glGetHandleARB(GL_PROGRAM_OBJECT_ARB));

	VEC3 source_sample_position(m_p_detector->getXraySourceCentre() + aXRaySourceSampleOffset);

	GLuint handle(glGetUniformLocation(program_handle, "g_projection_type"));
	if (!m_p_detector->useParallelBeam())
	{
		glUniform1i(handle, 0);

		handle = glGetUniformLocation(program_handle, "g_source_position");
		glUniform3f(handle, source_sample_position.getX(), source_sample_position.getY(), source_sample_position.getZ());
	}
	else
	{
		glUniform1i(handle, 1);

		VEC3 ray_direction((m_p_detector->getDetectorPosition() - source_sample_position).normal());

		handle = glGetUniformLocation(program_handle, "g_ray_direction");
		glUniform3f(handle, ray_direction.getX(), ray_direction.getY(), ray_direction.getZ());
	}

	//loadOrthoProjectionMatrix(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Load uniform variables to the GPU
	handle = glGetUniformLocation(program_handle, "g_modelling_matrix");
	glUniformMatrix4fv(handle, 1, GL_FALSE, aModellingTransformationMatrix.get());
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	/*MATRIX4 normal_matrix(aModellingTransformationMatrix);
	handle = glGetUniformLocation(program_handle, "g_normal_matrix");
	glUniformMatrix3fv(handle, 1, GL_FALSE, normal_matrix.get3x3());
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
*/
	GLfloat p_float_4[4];
	m_p_detector->getPlaneEquation(p_float_4[0], p_float_4[1], p_float_4[2], p_float_4[3]);
	handle = glGetUniformLocation(program_handle,"g_detector_plane");
	glUniform4fv(handle, 1, p_float_4);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	p_float_4[0] = m_p_detector->getDetectorPosition().getX();
	p_float_4[1] = m_p_detector->getDetectorPosition().getY();
	p_float_4[2] = m_p_detector->getDetectorPosition().getZ();
	handle = glGetUniformLocation(program_handle,"g_detector_center");
	glUniform3fv(handle, 1, p_float_4);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	VEC3 up_vector(-(m_p_detector->getRotationMatrix() * m_p_detector->getUpVector()));
	handle = glGetUniformLocation(program_handle,"g_detector_up_vector");
	glUniform3f(handle, up_vector.getX(), up_vector.getY(), up_vector.getZ());
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	VEC3 right_vector(-(m_p_detector->getRotationMatrix() * m_p_detector->getRightVector()));
	right_vector.normalise();
	handle = glGetUniformLocation(program_handle,"g_detector_right_vector");
	glUniform3f(handle, right_vector.getX(), right_vector.getY(), right_vector.getZ());
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(program_handle,"g_detector_width_in_units_of_length");
	glUniform1f(handle, m_p_detector->getSizeInUnitOfLength().getX());
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(program_handle,"g_detector_height_in_units_of_length");
	glUniform1f(handle, m_p_detector->getSizeInUnitOfLength().getY());
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(program_handle,"g_to_cm_scale");
	glUniform1f(handle, GLfloat(1.0 / cm));
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Display the object
	anObject.display();

	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Restore the shader program
	popShaderProgram();
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Restore the viewport
	glViewport(p_viewport[0], p_viewport[1], p_viewport[2], p_viewport[3]);

	// Restore the current FBO
	popFBO();

	// Restore the state
	popEnableDisableState();
	popEnableDisableState();

	// Wait for all OpenGL execution to be completed
	glFinish();

	// The L-buffer is not empty
	m_L_buffer_empty = false;
}


//------------------------------------------------------------------------------------------
void XRayRenderer::computeOuterSurfaceLBuffer(const TriangleMesh& anObject,
											  const VEC3& aXRaySourceSampleOffset,
											  const MATRIX4& aModellingTransformationMatrix)
//------------------------------------------------------------------------------------------
{
	// Save the viwport
	GLint p_viewport[4];
	glGetIntegerv(GL_VIEWPORT, &p_viewport[0]);

	// Change the viewport
	glViewport(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY());

	// Reset its L-buffer
	resetLBuffer();

	// Compute the L-buffer of the external surface
	computeInnerSurfaceLBuffer(anObject, aXRaySourceSampleOffset, aModellingTransformationMatrix);

	// Substract the L-buffer of the internal surfaces
	// Save the current FBO
	pushFBO();

	// Store the texturing flags
	pushTexture2D();

	// Store the state
	pushEnableDisableState(GL_BLEND);

	// Bind frame buffer object
	m_p_fbo_set[XRAY_DETECTOR_LBUFFER_FBO_ID]->Bind();
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Store the transformation matrices
	pushProjectionMatrix();
	pushModelViewMatrix();

	// Set the projection matrix
	loadIdentityProjectionMatrix();
	loadOrthoProjectionMatrix(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

	loadIdentityModelViewMatrix();

	// Enable the shader
	pushShaderProgram();
	m_shader_set[XRAY_DETECTOR_OUTER_SURFACE_LBUFFER_SHADER_ID].enable();
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Values to pass to the shader
	GLhandleARB program_handle(glGetHandleARB(GL_PROGRAM_OBJECT_ARB));

	GLint saved_texture;
	glGetIntegerv(GL_ACTIVE_TEXTURE_ARB, &saved_texture);

	GLint handle(glGetUniformLocation(program_handle, "g_cumulated_l_buffer_texture"));
	glUniform1i(handle, saved_texture - GL_TEXTURE0);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(program_handle, "g_use_artefact_filtering");
	glUniform1i(handle, m_use_L_buffer_artefact_filtering);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(program_handle, "g_x_offset");
	glUniform1f(handle, 1.0 / m_p_detector->getNumberOfPixels().getX());
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(program_handle, "g_y_offset");
	glUniform1f(handle, 1.0 / m_p_detector->getNumberOfPixels().getY());
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(program_handle, "g_projection_matrix");
	glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_projection_matrix.get());
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(program_handle, "g_modelview_matrix");
	glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_modelview_matrix.get());
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	glBindTexture(GL_TEXTURE_2D, m_p_texture_name_set[XRAY_DETECTOR_CUMULATED_LBUFFERS_TEXTURE_NAME_ID]);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	// Display the textured square
	m_detector_temporary_geometry.display();
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Restore the transformation matrices
	popProjectionMatrix();
	popModelViewMatrix();

	// Restore the FBO
	popFBO();

	// Store the state
	popEnableDisableState();

	// Restore the texturing flags
	popTexture();

	// Restore OpenGL states
	glViewport(p_viewport[0], p_viewport[1], p_viewport[2], p_viewport[3]);

	// Restore the shader program
	popShaderProgram();
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Wait for all OpenGL execution to be completed
	glFinish();
}


//---------------------------------------------------------
void XRayRenderer::computeCumulatedLBuffers(int anObjectID)
//---------------------------------------------------------
{
	// Save the current FBO
	pushFBO();

	// Store the texturing flags
	pushTexture2D();

	// Store the state
	pushEnableDisableState(GL_TEXTURE_2D);
	pushEnableDisableState(GL_BLEND);

	// Bind frame buffer object
	m_p_fbo_set[XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID]->Bind();
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Store the transformation matrices
	pushProjectionMatrix();
	pushModelViewMatrix();

	// Set the projection matrix
	loadIdentityProjectionMatrix();
	loadOrthoProjectionMatrix(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

	loadIdentityModelViewMatrix();

	// Enable the shader
	pushShaderProgram();
	m_shader_set[XRAY_DETECTOR_CUMULATED_LBUFFER_SHADER_ID].enable();
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Values to pass to the shader
	GLhandleARB program_handle(glGetHandleARB(GL_PROGRAM_OBJECT_ARB));

	GLint saved_texture;
	glGetIntegerv(GL_ACTIVE_TEXTURE_ARB, &saved_texture);

	GLint handle(glGetUniformLocation(program_handle, "g_l_buffer_texture"));
	glUniform1i(handle, saved_texture - GL_TEXTURE0);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(program_handle, "g_use_artefact_filtering");
	glUniform1i(handle, m_use_L_buffer_artefact_filtering);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(program_handle, "g_x_offset");
	glUniform1f(handle, 1.0 / m_p_detector->getNumberOfPixels().getX());
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(program_handle, "g_y_offset");
	glUniform1f(handle, 1.0 / m_p_detector->getNumberOfPixels().getY());
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(program_handle, "g_projection_matrix");
	glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_projection_matrix.get());
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(program_handle, "g_modelview_matrix");
	glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_modelview_matrix.get());
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	glBindTexture(GL_TEXTURE_2D, m_p_texture_name_set[XRAY_DETECTOR_LBUFFER_TEXTURE_NAME_ID]);

	// Enable blending
	bool current_blending_status(false);
	if (anObjectID != 0)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		current_blending_status = true;
	}
	// Disable blending
	else
	{
		glDisable(GL_BLEND);
		current_blending_status = false;
	}

	// Display the textured square
	m_detector_temporary_geometry.display();
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Restore the transformation matrices
	popProjectionMatrix();
	popModelViewMatrix();

	// Restore the FBO
	popFBO();

	// Store the state
	popEnableDisableState();
	popEnableDisableState();

	// Restore the texturing flags
	popTexture();

	// Restore the shader program
	popShaderProgram();
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Wait for all OpenGL execution to be completed
	glFinish();
}


//------------------------------------------------------------
void XRayRenderer::computeSumMUxDx(GLfloat HU, int anObjectID)
//------------------------------------------------------------
{
	// Save the current FBO
	pushFBO();

	// Store the texturing flags
	pushTexture2D();

	// Store the state
	pushEnableDisableState(GL_BLEND);

	switch (m_computing_mode)
	{
	case OPENGL:
		{
			// Bind frame buffer object
			m_p_fbo_set[XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID]->Bind();
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			// Store the transformation matrices
			pushProjectionMatrix();
			pushModelViewMatrix();

			// Display a textured square
			loadIdentityProjectionMatrix();
			loadOrthoProjectionMatrix(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

			loadIdentityModelViewMatrix();

			// Enable the shader
			pushShaderProgram();
			m_shader_set[XRAY_DETECTOR_SUM_MUx_Dx_SHADER_ID].enable();
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			// Values to pass to the shader
			GLhandleARB program_handle(glGetHandleARB(GL_PROGRAM_OBJECT_ARB));

			GLint saved_texture;
			glGetIntegerv(GL_ACTIVE_TEXTURE_ARB, &saved_texture);

			GLint handle(glGetUniformLocation(program_handle, "g_l_buffer_texture"));
			glUniform1i(handle, saved_texture - GL_TEXTURE0);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			handle = glGetUniformLocation(program_handle, "g_use_artefact_filtering");
			glUniform1i(handle, m_use_L_buffer_artefact_filtering);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			handle = glGetUniformLocation(program_handle, "g_x_offset");
			glUniform1f(handle, 1.0 / m_p_detector->getNumberOfPixels().getX());
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			handle = glGetUniformLocation(program_handle, "g_y_offset");
			glUniform1f(handle, 1.0 / m_p_detector->getNumberOfPixels().getY());
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			handle = glGetUniformLocation(program_handle, "g_projection_matrix");
			glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_projection_matrix.get());
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			handle = glGetUniformLocation(program_handle, "g_modelview_matrix");
			glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_modelview_matrix.get());
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			// Rebind textures
			// Cumulated L-buffers
			glActiveTexture(saved_texture);
			glBindTexture(GL_TEXTURE_2D, m_p_texture_name_set[XRAY_DETECTOR_LBUFFER_TEXTURE_NAME_ID]);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			// This is not the first object, enable blending
			if (anObjectID > 0)
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE);
			}
			// This is the first object, disable blending if needed
			else
			{
				// Blending is enable, disable it
				glDisable(GL_BLEND);
			}

			// Proceed each energy channel
			for (unsigned int current_slice(0);
					current_slice < m_p_xray_beam->getEnergyChannelNumber();
					++current_slice)
			{
				// Get the energy and the corresponding attenuation coefficient
				RATIONAL_NUMBER mu_water(getMuWaterLogLinearInterpolation(m_p_xray_beam->getEnergyChannel(current_slice).getPhotonEnergy()));
				RATIONAL_NUMBER mu(convertHounsfieldValue2Mu(HU, mu_water));

				// Bind the corresponding slice of the 3D texture
				if (m_p_xray_beam->getEnergyChannelNumber() > 1)
				{
					// Bind default frame buffer
					FramebufferObject::Disable();

					// Attach the texture
					m_p_fbo_set[XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID]->UnattachAll();
					m_p_fbo_set[XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID]->AttachTexture(GL_TEXTURE_3D, m_p_texture_name_set[XRAY_DETECTOR_SUM_MUx_Dx_TEXTURE_NAME_ID], GL_COLOR_ATTACHMENT0_EXT, 0, current_slice);

					// Bind the FBO
					m_p_fbo_set[XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID]->Bind();
					checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
				}

				// Set the attenuation coefficient
				//glBlendColor(1.0, 1.0, 1.0, mu);
				// Convert mu in cm2/g
				mu /= cm2 / g;
				handle = glGetUniformLocation(program_handle, "g_attenuation_coefficient");
				glUniform1f(handle, mu);
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

				// Display the textured square
				m_detector_temporary_geometry.display();
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
			}

			// Restore the transformation matrices
			popProjectionMatrix();
			popModelViewMatrix();

			// Restore the shader program
			popShaderProgram();
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			// Wait for all OpenGL execution to be completed
			glFinish();
		}
		break;

	case CUDA:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "CUDA is not supported.");
		break;

	case OPENCL:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "OpenCL is not supported.");
		break;

	default:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
		break;
	}

	// Restore the FBO
	popFBO();

	// Restore the texturing flags
	popTexture();

	// Restore the state
	popEnableDisableState();
}


//------------------------------------------
void XRayRenderer::computeIntegratedEnergy()
//------------------------------------------
{
	// Save the current FBO
	pushFBO();

	// Store the texturing flags
	pushTexture3D();

	// Store the state
	pushEnableDisableState(GL_TEXTURE_3D);
	pushEnableDisableState(GL_BLEND);

	switch (m_computing_mode)
	{
	case OPENGL:
		{
			// Disable blending
			if (m_energy_fluence_empty)
			{
				glDisable(GL_BLEND);
			}

			// Bind frame buffer object
			m_p_fbo_set[XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID]->Bind();
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			// Activate shaders
			pushShaderProgram();
			m_shader_set[XRAY_DETECTOR_ENERGY_FLUENCE_SHADER_ID].enable();
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			// Values to pass to the shader
			GLhandleARB program_handle(glGetHandleARB(GL_PROGRAM_OBJECT_ARB));

			GLint saved_texture;
			glGetIntegerv(GL_ACTIVE_TEXTURE_ARB, &saved_texture);

			GLint handle(glGetUniformLocation(program_handle, "g_sum_mu_x_dx_texture"));
			glUniform1i(handle, saved_texture - GL_TEXTURE0);

			// Rebind textures
			// Cumulated L-buffers
			glActiveTexture(saved_texture);
			glEnable(GL_TEXTURE_3D);
			glBindTexture(GL_TEXTURE_3D, m_p_texture_name_set[XRAY_DETECTOR_SUM_MUx_Dx_TEXTURE_NAME_ID]);

			// Store the transformation matrices
			pushProjectionMatrix();
			pushModelViewMatrix();

			// Display a textured square
			loadIdentityProjectionMatrix();
			loadOrthoProjectionMatrix(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

			loadIdentityModelViewMatrix();

			handle = glGetUniformLocation(program_handle, "g_projection_matrix");
			glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_projection_matrix.get());
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			handle = glGetUniformLocation(program_handle, "g_modelview_matrix");
			glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_modelview_matrix.get());
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			// Proceed each energy channel
			for (unsigned int current_slice(0);
					current_slice < m_p_xray_beam->getEnergyChannelNumber();
					++current_slice)
			{
				if (!m_energy_fluence_empty)
				{
					// Enable blending
					glEnable(GL_BLEND);
					glBlendFunc(GL_ONE, GL_ONE);
					checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
				}

				handle = glGetUniformLocation(program_handle, "g_n_input_energy");
				RATIONAL_NUMBER photon_number(m_p_xray_beam->getEnergyChannel(current_slice).getPhotonNumber());
				RATIONAL_NUMBER photon_energy(m_p_xray_beam->getEnergyChannel(current_slice).getPhotonEnergy() / MeV);
				RATIONAL_NUMBER input_energy(photon_number * photon_energy / RATIONAL_NUMBER(m_p_detector->getNumberOfSourceSamples()));
				glUniform1f(handle, input_energy);

				// Transform the texture coordinates
				GLdouble z((0.5 + current_slice) / GLdouble(m_p_xray_beam->getEnergyChannelNumber()));
				MATRIX4 translation_matrix(MATRIX4::buildTranslationMatrix(0, 0, z));
				handle = glGetUniformLocation(program_handle, "g_text_coord_matrix");
				glUniformMatrix4fv(handle, 1, GL_FALSE, translation_matrix.get());
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

				// Display the textured square
				m_detector_temporary_geometry.display();

				// The energy fluence is not empty
				m_energy_fluence_empty = false;
			}

			// Restore the transformation matrices
			popProjectionMatrix();
			popModelViewMatrix();

			// Restore the shader program
			popShaderProgram();
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			// Wait for all OpenGL execution to be completed
			glFinish();
		}
		break;

	case CUDA:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "CUDA is not supported.");
		break;

	case OPENCL:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "OpenCL is not supported.");
		break;

	default:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
		break;
	}

	// Restore the texturing flags
	popTexture();

	// Restore the state
	popEnableDisableState();
	popEnableDisableState();

	// Restore the FBO
	popFBO();
}


//----------------------------
void XRayRenderer::normalise()
//----------------------------
{
	// Get the energy fluence map
	printFBO(XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID, 0);

	// Get image statistics
	PixelType min_value(FLT_MAX);
	PixelType max_value(-FLT_MAX);
	for (unsigned int i(0);
			i < m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY();
			++i)
	{
		PixelType pixel_value(m_p_temp_raw_data[i]);
		if (min_value > pixel_value) min_value = pixel_value;
		if (max_value < pixel_value) max_value = pixel_value;
	}

	// Make sure the min value is not negative (in which case it is an artefact due to the dot product sign)
	if (min_value < 0)
	{
		min_value = 0;
	}

	// Compute the filter parameters
	double range(fabs(max_value - min_value));
	if (range > EPSILON)
	{
		m_shift_filter = -min_value;
		m_scale_filter = 1.0 / range;
	}
	// The image is homogeneous
	else
	{
		m_shift_filter = 0;
		m_scale_filter = 1;
	}
}


//--------------------------
void XRayRenderer::display()
//--------------------------
{
	// Activate shaders
	pushShaderProgram();
	m_shader_set[XRAY_DETECTOR_DISPLAY_SHADER_ID].enable();
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Values to pass to the shader
	GLhandleARB program_handle(glGetHandleARB(GL_PROGRAM_OBJECT_ARB));

	// Get the current texture unit
	GLint saved_texture;
	glGetIntegerv(GL_ACTIVE_TEXTURE_ARB, &saved_texture);

	// Pass the current texture unit to the shader
	GLint handle(glGetUniformLocation(program_handle, "g_attenuation"));
	glUniform1i(handle, saved_texture - GL_TEXTURE0);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Pass the negative rendering mode to the shader
	handle = glGetUniformLocation(program_handle, "g_display_negative_image");
	glUniform1i(handle, m_negative_display_flag);

	// Pass the shift parameter of the shift-scale filter to the shader
	handle = glGetUniformLocation(program_handle, "g_shift_filter");
	glUniform1f(handle, m_shift_filter);

	// Pass the scale parameter of the shift-scale filter to the shader
	handle = glGetUniformLocation(program_handle, "g_scale_filter");
	glUniform1f(handle, m_scale_filter);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(program_handle, "g_projection_matrix");
	glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_projection_matrix.get());
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(program_handle, "g_modelview_matrix");
	glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_modelview_matrix.get());
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Display the texture
	displayEnergyFluence();

	// Restore the shader program
	popShaderProgram();
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Wait for all OpenGL execution to be completed
	glFinish();
}


//--------------------------------------------------------------------------------------------------
RATIONAL_NUMBER XRayRenderer::getMuWaterLinearInterpolation(const RATIONAL_NUMBER& anIncidentEnergy)
//--------------------------------------------------------------------------------------------------
{
	// The table of mu water is empty
	if (m_mu_water_set.size() == 0)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "The mu water table is empty.");
	}

	// Get an interpolated value
	RATIONAL_NUMBER mu_water(0);

	// The required energy is too low, force to the minimum value
	if (m_mu_water_set.front().getEnergy() >= anIncidentEnergy)
	{
		mu_water = m_mu_water_set.front().getAttenuationCoefficient();
	}
	// The required energy is too high, force to the maximum value
	else if (m_mu_water_set.back().getEnergy() <= anIncidentEnergy)
	{
		mu_water = m_mu_water_set.back().getAttenuationCoefficient();
	}
	// Use an interpolated value
	else
	{
		for (std::vector<AttenuationCoefficient>::const_iterator ite(m_mu_water_set.begin() + 1);
			ite != m_mu_water_set.end();
			++ite)
		{
			if (ite->getEnergy() >= anIncidentEnergy)
			{
				// Get the interpolated value
				mu_water = (((ite - 1)->getAttenuationCoefficient()) + ((ite->getAttenuationCoefficient()) - ((ite - 1)->getAttenuationCoefficient())) * ((anIncidentEnergy - (ite - 1)->getEnergy()) / (ite->getEnergy() - (ite - 1)->getEnergy())));

				// Return mu value
				return (mu_water);
			}
		}
	}

	// Return mu value
	return (mu_water);
}


//-----------------------------------------------------------------------------------------------------
RATIONAL_NUMBER XRayRenderer::getMuWaterLogLinearInterpolation(const RATIONAL_NUMBER& anIncidentEnergy)
//-----------------------------------------------------------------------------------------------------
{
	// The table of mu water is empty
	if (m_mu_water_set.size() == 0)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "The mu water table is empty.");
	}

	// Get an interpolated value
	RATIONAL_NUMBER mu_water(0);

	// The required energy is too low, force to the minimum value
	if (m_mu_water_set.front().getEnergy() >= anIncidentEnergy)
	{
		mu_water = m_mu_water_set.front().getAttenuationCoefficient();
	}
	// The required energy is too high, force to the maximum value
	else if (m_mu_water_set.back().getEnergy() <= anIncidentEnergy)
	{
		mu_water = m_mu_water_set.back().getAttenuationCoefficient();
	}
	// Use an interpolated value
	else
	{
		for (std::vector<AttenuationCoefficient>::const_iterator ite(m_mu_water_set.begin() + 1);
			ite != m_mu_water_set.end();
			++ite)
		{
			if (ite->getEnergy() >= anIncidentEnergy)
			{
				// Get the interpolated value
				mu_water = exp(log((ite - 1)->getAttenuationCoefficient()) + (log(ite->getAttenuationCoefficient()) - log((ite - 1)->getAttenuationCoefficient())) * ((anIncidentEnergy - (ite - 1)->getEnergy()) / (ite->getEnergy() - (ite - 1)->getEnergy())));

				// Return mu value
				return (mu_water);
			}
		}
	}

	// Return mu value
	return (mu_water);
}


//----------------------------
void XRayRenderer::updateVBO()
//----------------------------
{
	// Vertices
	std::vector<GLfloat> p_vertices;

	p_vertices.push_back(0);
	p_vertices.push_back(0);
	p_vertices.push_back(0);

	p_vertices.push_back(1);
	p_vertices.push_back(0);
	p_vertices.push_back(0);

	p_vertices.push_back(1);
	p_vertices.push_back(1);
	p_vertices.push_back(0);

	p_vertices.push_back(0);
	p_vertices.push_back(1);
	p_vertices.push_back(0);


	// Texture coordinates
	std::vector<GLfloat> p_texture_coordinates;

	p_texture_coordinates.push_back(0);
	p_texture_coordinates.push_back(0);
	p_texture_coordinates.push_back(0);

	p_texture_coordinates.push_back(1);
	p_texture_coordinates.push_back(0);
	p_texture_coordinates.push_back(0);

	p_texture_coordinates.push_back(1);
	p_texture_coordinates.push_back(1);
	p_texture_coordinates.push_back(0);

	p_texture_coordinates.push_back(0);
	p_texture_coordinates.push_back(1);
	p_texture_coordinates.push_back(0);

	m_detector_temporary_geometry.setVertexData(GL_QUADS,
			p_vertices.size() / 3, 3, &p_vertices[0],
			0, 0, 0,
			p_texture_coordinates.size() / 3, 3, &p_texture_coordinates[0]);
}
