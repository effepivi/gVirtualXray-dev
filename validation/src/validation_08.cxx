/*

Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*	@file		validation_08.cxx
*
*	@brief		Main test program.
*
*	@version	1.0
*
*	@date		11/11/2013
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
*				http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
*				BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifdef HAS_GLEW
#include <GL/glew.h>
#endif

#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <memory>
#include <cstdlib>
#include <sstream>
#include <ctime>

#include <itkMacro.h>
#include <itkImage.h>
#include <itkImageFileWriter.h>

#include <itkBinaryMask3DMeshSource.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkMesh.h>
#include <itkQuadEdgeMesh.h>
#include <itkMeshFileWriter.h>
#include <itkQuadEdgeMeshDecimationCriteria.h>
/*
#ifdef HAS_VTK
#include <itkImageToVTKImageFilter.h>

#include <vtkVersion.h>
#include <vtkImageViewer.h>
#include <vtkImageMapper3D.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkImageActor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#endif
*/

#define GLFW_INCLUDE_GLCOREARB 1

#include <GLFW/glfw3.h>

#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#ifndef GVXR_POLYGON_MESH_H
#include "gVirtualXRay/PolygonMesh.h"
#endif

#ifndef GVXR_CUBE_MESH_H
#include "gVirtualXRay/CubeMesh.h"
#endif

#ifndef GVXR_XRAY_BEAM_H
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef GVXR_XRAY_DETECTOR_H
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef GVXR_MATRIX4X4_H
#include "gVirtualXRay/Matrix4x4.h"
#endif

#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef GVXR_SHADER_H
#include "gVirtualXRay/Shader.h"
#endif

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

#include "display_gl2.frag.h"
#include "display_gl2.vert.h"

#include "display_gl3.frag.h"
#include "display_gl3.vert.h"


//******************************************************************************
//  Defines
//******************************************************************************
#define PREFIX "../data/validation_08"



//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//	Constant variables
//******************************************************************************
const GLfloat g_rotation_speed(2.0);

const int NONE(0);
const int SCENE(1);
const int OBJECT(2);
const int DETECTOR(3);

const float g_incident_energy(0.08 * MeV);
const VEC2 g_resolution(0.5 * mm, 1.0 * mm);
const Vec2ui g_number_of_pixels(367, 128);
const VEC3 g_source_position(   0.0,  10.0 * cm, 0.0);
const VEC3 g_detector_position( 0.0, -10.0 * cm, 0.0);
const VEC3 g_detector_up_vector(0.0, 0.0, 1.0);

const double g_initial_fovy(45);                              // Field of view along the y-axis
const double g_initial_near(5.0 * cm);                        // Near clipping plane
const double g_initial_far(500.0 * cm);                       // Far clipping plane

const unsigned int g_angle_number(360);
const RATIONAL_NUMBER g_angle_step(0.5);


//******************************************************************************
//	Global variables
//******************************************************************************
GLsizei g_main_window_width(1280 / 2.0);
GLsizei g_main_window_height(800 / 2.0);
GLFWwindow* g_p_main_window_id(0);
GLfloat g_zoom(60.0 * cm);
GLfloat g_eye_offset(2.0 * cm);

int g_button(-1);
int g_button_state(-1);
bool g_use_arc_ball(false);
GLint g_last_x_position(0);
GLint g_last_y_position(0);
GLint g_current_x_position(0);
GLint g_current_y_position(0);

bool g_display_beam(true);

Matrix4x4<GLfloat> g_scene_rotation_matrix;
Matrix4x4<GLfloat> g_detector_rotation_matrix;
Matrix4x4<GLfloat> g_sample_rotation_matrix;

std::vector<float> g_p_vertex_data;
std::vector<unsigned int> g_p_index_data;

PolygonMesh g_polygon_mesh;

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;
Shader g_display_shader;

bool g_is_xray_image_up_to_date(false);
int g_rotation_mode(NONE);
bool g_display_wireframe(false);
bool g_display_outside_polygon(true);


//******************************************************************************
//	Function declaration
//******************************************************************************
void createPhantom();
void initGLEW();
void initGL();
void display();
void framebufferSizeCallback(GLFWwindow* apWindow, int aWidth, int aHeight);
void keyCallback(GLFWwindow* apWindow, int aKey, int aScanCode, int anAction, int aModifierKey);
void mouseButtonCallback(GLFWwindow* apWindow, int aButton, int aButtonState, int aModifierKey);
void cursorPosCallback(GLFWwindow* apWindow, double x, double y);
void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset);
void quit();
void loadSTLFile();
void loadDetector();
void loadSource();
void loadXRaySimulator();
Vec3<GLfloat> getArcballVector(int x, int y);
void computeRotation(MATRIX4& aRotationMatrix);
void updateXRayImage();
void errorCallback(int error, const char* description);
void draw();


//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
	try
	{
	    // Initialize random seed
	    srand(time(0));

	    // Set an error callback
	    glfwSetErrorCallback(errorCallback);

	    // Register the exit callback
		atexit(quit);

		// Initialize GLFW
	    if (!glfwInit())
	    {
	    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: cannot initialise GLFW.");
	    }

	    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	    // Enable quad-buffering if possible
	    glfwWindowHint(GLFW_STEREO, GL_TRUE);

	    // Enable anti-aliasing
	    glfwWindowHint(GLFW_SAMPLES, 4);

	    // Create a windowed mode window and its OpenGL context
	    g_p_main_window_id = glfwCreateWindow(g_main_window_width, g_main_window_height, "GPU X-Ray Demo", NULL, NULL);
	   
        // Window cannot be created 
        if (!g_p_main_window_id)
	    {
            // Disable quad-buffering
            glfwWindowHint(GLFW_STEREO, GL_FALSE);

            // Create the window
	        g_p_main_window_id = glfwCreateWindow(g_main_window_width, g_main_window_height, "GPU X-Ray Demo", NULL, NULL);
        }
        
        // Window cannot be created 
        if (!g_p_main_window_id)
	    {
	        glfwTerminate();
	    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: cannot create a GLFW windowed mode window and its OpenGL context.");
	    }

	    // Make the window's context current
	    glfwMakeContextCurrent(g_p_main_window_id);

        // Initialise GLEW
        initGLEW();

        // Check the OpenGL version
        std::cout << "GL:\t" << glGetString(GL_VERSION) << std::endl;

        // Check the GLSL version
        std::cout << "GLSL:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

        // Initialise OpenGL
		initGL();

		// Check the current FBO
		checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Initialize GLFL callback
		glfwSetKeyCallback(g_p_main_window_id, keyCallback);
		glfwSetFramebufferSizeCallback(g_p_main_window_id, framebufferSizeCallback);
		glfwSetMouseButtonCallback(g_p_main_window_id, mouseButtonCallback);
		glfwSetCursorPosCallback(g_p_main_window_id, cursorPosCallback);
		glfwSetScrollCallback(g_p_main_window_id, scrollCallback);

	    // Create the phantom
	    createPhantom();

		// Display the mu coefficients
		std::cout << "Linear attenuation coefficient at:\t" << g_incident_energy / keV << " keV (" << g_incident_energy / MeV << "MeV)" << std::endl;
		std::cout << "Mu(HU(0):     \t" << g_xray_renderer.getMuFromHU(0, g_incident_energy) / (1.0 / cm)      << " cm-1" << std::endl;
		std::cout << "Mu(HU(-1000): \t" << g_xray_renderer.getMuFromHU(-1000, g_incident_energy) / (1.0 / cm)      << " cm-1" << std::endl;

		// Load the data
		loadDetector();
		loadSource();
		loadXRaySimulator();

		// Add the geometry to the X-ray renderer
		loadSTLFile();
		g_xray_renderer.addInnerSurface(&g_polygon_mesh);

		// Update the X-ray image
		updateXRayImage();

		// Set the projection matrix
		GLint width(0);
		GLint height(0);
		glfwGetFramebufferSize(g_p_main_window_id, &width, &height);
		framebufferSizeCallback(g_p_main_window_id, width, height);

        std::cout << "Angular step: " << g_angle_step << " degrees" << std::endl;
        std::cout << "Angular span: [" << 0 << ", " << g_angle_step * (g_angle_number - 1) << "] degrees" << std::endl;

        // Compute the sinogram
        g_xray_renderer.computeSinogram(g_sample_rotation_matrix, VEC3(), g_angle_number, g_angle_step);

        // Save the sinogram
        g_xray_renderer.printSinogram(PREFIX"/simulation_sinogram.mhd");

        // Compute the projection set
        g_xray_renderer.computeProjectionSet(g_sample_rotation_matrix, VEC3(), g_angle_number, g_angle_step);

        // Save the projection set
        g_xray_renderer.printProjectionSet(PREFIX"/simulation_projection_set.mhd");

        // Launch the event loop
	    while (!glfwWindowShouldClose(g_p_main_window_id))
	    {
	    	// Render here
	    	display();

	        // Swap front and back buffers
	        glfwSwapBuffers(g_p_main_window_id);

	        // Poll for and process events
	        glfwPollEvents();
	    }
	}
	// Catch exception if any
	catch(itk::ExceptionObject& error)
	{
		std::cerr << error << std::endl;
	}
	catch (const std::exception& error)
	{
	    std::cerr << error.what() << std::endl;
	}

	// Close the window and shut GLFW
	if (g_p_main_window_id)
	{
		glfwDestroyWindow(g_p_main_window_id);
		g_p_main_window_id = 0;
		glfwTerminate();
	}

	// Return an exit code
	return (EXIT_SUCCESS);
}


//------------------
void createPhantom()
//------------------
{
	typedef itk::Image<short, 3> ImageType;
	typedef itk::ImageFileWriter <ImageType> VolumeWriterType;

	ImageType::Pointer p_phantom(ImageType::New());

	VolumeWriterType::Pointer p_writer(VolumeWriterType::New());
	p_writer->SetInput(p_phantom);
	p_writer->SetFileName(PREFIX"/phantom.mhd");

	ImageType::SizeType	size;
	size[0] = 256;
	size[1] = 256;
	size[2] = 128;

	ImageType::IndexType start;
	start[0] = 0;
	start[1] = 0;
	start[2] = 0;

	ImageType::RegionType region;
	region.SetSize(size);
	region.SetIndex(start);

	p_phantom->SetRegions(region);
	p_phantom->Allocate();

	double spacing[3];
	spacing[0] = 0.5;
	spacing[1] = 0.5;
	spacing[2] = 1.0;
	p_phantom->SetSpacing(spacing);

	double origin[3];
	origin[0] = 0;
	origin[1] = 0;
	origin[2] = 0;
	p_phantom->SetOrigin(origin);

	p_phantom->FillBuffer(-1000);

	// Set pixels in a square to one value
	unsigned int slice_size(size[2] / 4);
	unsigned int start_slice((size[2]-slice_size) / 2);

	unsigned int step_number(6);


	unsigned int step_height(((size[0] - (size[0] / 2) / 2)) / step_number);

	for(unsigned int s = start_slice; s < start_slice + slice_size; s++)
	{
		for (unsigned int step(0); step < step_number; ++step)
		{
			unsigned int row_size(size[1] / 2);
			unsigned int start_row((size[1]-row_size) / 4);
			start_row += step * step_height;

			for(unsigned int r = start_row ; r < start_row + step_height; r++)
			{
				unsigned int column_size(size[0] / 2);
				unsigned int start_column((size[0]-column_size) / 4);

				unsigned int step_width(step_height * (1 + step));
				for(unsigned int c = start_column; c < start_column + step_width; c++)
				{
					ImageType::IndexType pixelIndex;
					pixelIndex[0] = r;
					pixelIndex[1] = c;
					pixelIndex[2] = s;

					p_phantom->SetPixel(pixelIndex, 0);
				}
			}
		}
	}
	p_writer->Update();



	typedef unsigned char OutputVoxelDataType;
	typedef itk::Image<OutputVoxelDataType, 3> OutputVolumeType;
	typedef itk::QuadEdgeMesh<float, 3> MeshType;

	typedef itk::BinaryThresholdImageFilter<ImageType, OutputVolumeType> BinaryThresholdType;
	typedef itk::BinaryMask3DMeshSource<OutputVolumeType, MeshType> IsoSurfaceExtracterType;

	BinaryThresholdType::Pointer p_threshold_filter(BinaryThresholdType::New());
	IsoSurfaceExtracterType::Pointer p_iso_surface_extracter(IsoSurfaceExtracterType::New());

	// Create the binary mask
	p_threshold_filter->SetInput(p_phantom);
	p_threshold_filter->SetLowerThreshold(-500);
	p_threshold_filter->SetOutsideValue(0);
	p_threshold_filter->SetInsideValue(1);
	p_threshold_filter->Update();

	typedef itk::ImageFileWriter <OutputVolumeType> MaskVolumeType;
	MaskVolumeType::Pointer p_mask_writer(MaskVolumeType::New());
	p_mask_writer->SetInput(p_threshold_filter->GetOutput());
	p_mask_writer->SetFileName(PREFIX"/phantom_mask.mhd");
	p_mask_writer->Update();

	// Extract the isosurface
	p_iso_surface_extracter->SetInput(p_threshold_filter->GetOutput());
	p_iso_surface_extracter->SetObjectValue(1);
	p_iso_surface_extracter->Update();

	// Get the polygon mesh
	MeshType::Pointer p_mesh(p_iso_surface_extracter->GetOutput());

	// There are triangles
	g_p_vertex_data.clear();
	g_p_index_data.clear();
	if (p_mesh->GetNumberOfCells() && p_mesh->GetNumberOfPoints())
	{
		g_p_vertex_data.resize(p_mesh->GetNumberOfPoints() * 3, 0);
		g_p_index_data.resize(p_mesh->GetNumberOfCells() * 3, 0);

		MeshType::PointsContainer* p_itk_point_set = p_mesh->GetPoints();
		MeshType::CellsContainer* p_itk_cell_set = p_mesh->GetCells();

		// Copy points
		for(MeshType::PointsContainer::Iterator ite(p_itk_point_set->Begin());
			ite != p_itk_point_set->End();
			++ite)
		{
			// Get the point index from the point container iterator
			int idx = ite.Index();

			// Get the point
			MeshType::PointType p = ite.Value(); // access the point

			// Copy the point
			g_p_vertex_data[idx * 3 + 0] = p[0];
			g_p_vertex_data[idx * 3 + 1] = p[1];
			g_p_vertex_data[idx * 3 + 2] = p[2];
		}

		typedef MeshType::CellType	CellType;
		typedef CellType::PointIdIterator	PointIdIterator;

		for(MeshType::CellsContainer::Iterator ite(p_itk_cell_set->Begin());
			ite != p_itk_cell_set->End();
			++ite)
		{
			// Get the cell index from the point container iterator
			int idx = ite.Index();

			// Get the cell
			CellType* p_cell = ite.Value();

			// Copy point indices
			int i(0);
			for (PointIdIterator ite_point_idx(p_cell->PointIdsBegin());
				ite_point_idx != p_cell->PointIdsEnd();
				++ite_point_idx, ++i)
			{
				unsigned int point_id = *ite_point_idx;
				g_p_index_data[idx * 3 + i] = point_id;
			}
		}
	}
	g_polygon_mesh.setInternalData(GL_TRIANGLES,
			&g_p_vertex_data[0],
			g_p_vertex_data.size() / 3,
			GL_FLOAT,
			&g_p_index_data[0],
			g_p_index_data.size(),
			GL_UNSIGNED_INT,
			true,
			GL_STATIC_DRAW);

	typedef itk::MeshFileWriter< MeshType > MeshWriterType;
	MeshWriterType::Pointer writer(MeshWriterType::New());
	writer->SetFileTypeAsBINARY ();
	writer->SetFileName(PREFIX"/step_wedge.stl");
	writer->SetInput(p_iso_surface_extracter->GetOutput());
	try
	{
		writer->Update();
	}
	catch( itk::ExceptionObject & error )
	{
		g_polygon_mesh.setFileName(PREFIX"/step_wedge.stl");
		g_polygon_mesh.writeSTLFile();
	}
}


//----------------------------------------------------
void errorCallback(int error, const char* description)
//----------------------------------------------------
{
	std::cerr << "GLFW error: " << description << std::endl;
}


//---------
void quit()
//---------
{
	if (g_p_main_window_id)
	{
		glfwDestroyWindow(g_p_main_window_id);
        g_p_main_window_id = 0;
        glfwTerminate();
	}
}


//-------------
void initGLEW()
//-------------
{
#if defined(_WIN32) || defined(_WIN64)
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::stringstream error_message;
		error_message << "ERROR: cannot initialise GLEW:\t" << glewGetErrorString(err);

        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
	}
#endif
}


//-----------
void initGL()
//-----------
{
	std::cout << "OpenGL renderer:   " << glGetString(GL_RENDERER)   << std::endl;
	std::cout << "OpenGL version:    " << glGetString(GL_VERSION)    << std::endl;
	std::cout << "OpenGL vender:     " << glGetString(GL_VENDOR)     << std::endl;
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Enable the Z-buffer
	glEnable(GL_DEPTH_TEST);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Set the background colour
	glClearColor(0.5, 0.5, 0.5, 1.0);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	glEnable(GL_MULTISAMPLE);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Initialise the shaders
	char* p_vertex_shader(0);
	char* p_fragment_shader(0);

	int z_lib_return_code_vertex(0);
	int z_lib_return_code_fragment(0);

	std::string vertex_shader;
	std::string fragment_shader;

	// Display shader
	if (useOpenGL3_2OrAbove())
	{
	    z_lib_return_code_vertex   = inflate(g_display_gl3_vert, sizeof(g_display_gl3_vert),   &p_vertex_shader);
	    z_lib_return_code_fragment = inflate(g_display_gl3_frag, sizeof(g_display_gl3_frag), &p_fragment_shader);
	}
	else
	{
	    z_lib_return_code_vertex   = inflate(g_display_gl2_vert, sizeof(g_display_gl2_vert),   &p_vertex_shader);
	    z_lib_return_code_fragment = inflate(g_display_gl2_frag, sizeof(g_display_gl2_frag), &p_fragment_shader);
	}

	vertex_shader   = p_vertex_shader;
	fragment_shader = p_fragment_shader;
	delete [] p_vertex_shader;     p_vertex_shader = 0;
	delete [] p_fragment_shader; p_fragment_shader = 0;

	if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
	}
	g_display_shader.setLabels("display.vert", "display.frag");
	g_display_shader.loadSource(vertex_shader, fragment_shader);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//-----------
void render()
//-----------
{
	try
	{
		// Handle for shader variables
		GLuint handle(0);

		// Enable back face culling
		pushEnableDisableState(GL_CULL_FACE);
	    pushEnableDisableState(GL_DEPTH_TEST);
	    pushEnableDisableState(GL_BLEND);

	    glEnable(GL_DEPTH_TEST);
	    glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// Enable the shader
		pushShaderProgram();
		g_display_shader.enable();
		GLint shader_id(g_display_shader.getProgramHandle());

		// Define the light colours here
		GLfloat light_global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
		GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
		GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

		handle = glGetUniformLocation(shader_id, "light_global_ambient");
		glUniform4fv(handle, 1, &light_global_ambient[0]);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		handle = glGetUniformLocation(shader_id, "light_ambient");
		glUniform4fv(handle, 1, &light_ambient[0]);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		handle = glGetUniformLocation(shader_id, "light_diffuse");
		glUniform4fv(handle, 1, &light_diffuse[0]);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		handle = glGetUniformLocation(shader_id, "light_specular");
		glUniform4fv(handle, 1, &light_specular[0]);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		VEC3 light_position(0, 0, g_zoom);
		handle = glGetUniformLocation(shader_id, "light_position");
		glUniform4f(handle, light_position.getX(), light_position.getY(), light_position.getZ(), 1.0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Check the status of OpenGL and of the current FBO
		checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Store the current transformation matrix
		pushModelViewMatrix();

		// Rotate the sample
		g_current_modelview_matrix *= g_scene_rotation_matrix;

		handle = glGetUniformLocation(shader_id, "g_projection_matrix");
		glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_projection_matrix.get());
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		handle = glGetUniformLocation(shader_id, "g_modelview_matrix");
		glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_modelview_matrix.get());
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		MATRIX4 normal_matrix(g_current_modelview_matrix);
		handle = glGetUniformLocation(shader_id, "g_normal_matrix");
		glUniformMatrix3fv(handle, 1, GL_FALSE, normal_matrix.get3x3());
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		GLint lighting;
		lighting = 1;
		handle = glGetUniformLocation(shader_id,"g_use_lighting");
		glUniform1iv(handle, 1, &lighting);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Store the current transformation matrix
		pushModelViewMatrix();

		g_current_modelview_matrix *= g_sample_rotation_matrix;
		applyModelViewMatrix();

        // Get the material
        Material& material(g_polygon_mesh.getMaterial());

        // Define colours inshader
        handle = glGetUniformLocation(shader_id, "material_ambient");
        glUniform4fv(handle, 1, &(material.getAmbientColour()[0]));
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "material_diffuse");
        glUniform4fv(handle, 1, &(material.getDiffuseColour()[0]));
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "material_specular");
        glUniform4fv(handle, 1, &(material.getSpecularColour()[0]));
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "material_shininess");
        glUniform1f(handle, material.getShininess());
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        if (g_display_wireframe)
        {
            g_polygon_mesh.displayWireFrame();
        }
        else
        {
            g_polygon_mesh.display();
        }

        // Restore the current transformation matrix
		popModelViewMatrix();

		// Define colours
		{
			const GLfloat material_ambient[]  = {0.0, 0.39225,  0.39225,  1.0};
			const GLfloat material_diffuse[]  = {0.0, 0.70754,  0.70754,  1.0};
			const GLfloat material_specular[] = {0.0, 0.708273, 0.708273, 1.0};
			const GLfloat material_shininess = 50.2;

			handle = glGetUniformLocation(shader_id, "material_ambient");
			glUniform4fv(handle, 1, &material_ambient[0]);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			handle = glGetUniformLocation(shader_id, "material_diffuse");
			glUniform4fv(handle, 1, &material_diffuse[0]);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			handle = glGetUniformLocation(shader_id, "material_specular");
			glUniform4fv(handle, 1, &material_specular[0]);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			handle = glGetUniformLocation(shader_id, "material_shininess");
			glUniform1fv(handle, 1, &material_shininess);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		}

		// Display the source
		g_xray_detector.displaySource();

		// Display the X-Ray image
	    glDisable(GL_CULL_FACE);
		g_xray_renderer.display();

		// Display the beam
		if (g_display_beam)
		{
			const GLfloat material_ambient[]  = {0.75, 0, 0.5, 0.1};
			handle = glGetUniformLocation(shader_id, "material_ambient");
			glUniform4fv(handle, 1, &material_ambient[0]);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			lighting = 0;
			handle = glGetUniformLocation(shader_id,"g_use_lighting");
			glUniform1iv(handle, 1, &lighting);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

			g_xray_detector.displayBeam();
		}

		// Disable the shader
		popShaderProgram();

		// Restore the current transformation matrix
		popModelViewMatrix();

		// Restore the attributes
        popEnableDisableState();
        popEnableDisableState();
        popEnableDisableState();

		// Check the status of OpenGL and of the current FBO
		checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
	// Catch exception if any
	catch (const std::exception& error)
	{
		std::cerr << error.what() << std::endl;
	}
}


//------------
void display()
//------------
{
	// Store the current transformation matrix
	pushModelViewMatrix();
	pushProjectionMatrix();

	// Update the X-ray image
	updateXRayImage();

    draw();

	// Restore the current transformation matrix
	popModelViewMatrix();
	popProjectionMatrix();
}


//-------------------------------------------------------------------------------
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
//-------------------------------------------------------------------------------
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(g_p_main_window_id, GL_TRUE);
			break;

		case GLFW_KEY_B:
			g_display_beam = !g_display_beam;
			break;

		case GLFW_KEY_N:
			g_xray_renderer.useNegativeFilteringFlag(!g_xray_renderer.getNegativeFilteringFlag());
			break;

		case GLFW_KEY_1:
			// This is a point source
			if (g_xray_detector.getSourceShape() != XRayDetector::PARALLEL)
			{
				// Use a parallel source
				g_xray_detector.setParallelBeam();
			}
			// This is a cubic source
			else
			{
				g_xray_detector.setPointSource();
			}

			// The X-ray image is not up-to-date
			g_is_xray_image_up_to_date = false;
			break;

		case GLFW_KEY_2:
			// This is not a line source
			if (g_xray_detector.getSourceShape() != XRayDetector::LINE)
			{
				g_xray_detector.setLineSource(g_xray_detector.getXraySourceCentre(), VEC3(1, 0, 0), 16, 5.0*mm);
			}
			// This is a cubic source
			else
			{
				g_xray_detector.setPointSource();
			}

			// The X-ray image is not up-to-date
			g_is_xray_image_up_to_date = false;
			break;

		case GLFW_KEY_3:
			{
				// This is not a square source
				if (g_xray_detector.getSourceShape() != XRayDetector::SQUARE)
				{
					g_xray_detector.setSquareSource(g_xray_detector.getXraySourceCentre(), 4, 5.0*mm);
				}
				// This is a cubic source
				else
				{
					g_xray_detector.setPointSource();
				}

				// The X-ray image is not up-to-date
				g_is_xray_image_up_to_date = false;
			}
			break;

		case GLFW_KEY_4:
			{
				// This is not a cubic source
				if (g_xray_detector.getSourceShape() != XRayDetector::CUBE)
				{
					g_xray_detector.setCubicSource(g_xray_detector.getXraySourceCentre(), 3, 5.0*mm);
				}
				// This is a cubic source
				else
				{
					g_xray_detector.setPointSource();
				}

				// The X-ray image is not up-to-date
				g_is_xray_image_up_to_date = false;
			}
			break;

		case GLFW_KEY_T:
			{
				switch (g_rotation_mode)
				{
				case SCENE:
					std::cout << "Rotation: select the object." << std::endl;
					g_rotation_mode = OBJECT;
					break;

				case OBJECT:
					std::cout << "Rotation: select the detector." << std::endl;
					g_rotation_mode = DETECTOR;
					break;

				case DETECTOR:
					std::cout << "Rotation: select the scene." << std::endl;
					g_rotation_mode = SCENE;
					break;

				default:
					std::cout << "Rotation: select the scene." << std::endl;
					g_rotation_mode = SCENE;
					break;
				}
			}
			break;

		case GLFW_KEY_W:
			g_display_wireframe = !g_display_wireframe;
			break;

		case GLFW_KEY_V:
			g_display_outside_polygon = !g_display_outside_polygon;
			break;

		case GLFW_KEY_KP_ADD:
			g_zoom += 1.0 * cm;
			framebufferSizeCallback(g_p_main_window_id, g_main_window_width, g_main_window_height);
			std::cout << "Zoom in cm)" << g_zoom / cm << std::endl;
			break;

		case GLFW_KEY_MINUS:
		case GLFW_KEY_KP_SUBTRACT:
			g_zoom -= 1.0 * cm;
			framebufferSizeCallback(g_p_main_window_id, g_main_window_width, g_main_window_height);
			std::cout << "Zoom in cm)" << g_zoom / cm << std::endl;
			break;

		default:
			std::cout << "Key: " << key << std::endl;
			break;
		}
	}
}


//-----------------------------------------------------------------------
void framebufferSizeCallback(GLFWwindow* apWindow, int width, int height)
//-----------------------------------------------------------------------
{
	glViewport(0, 0, width, height);

	if (height == 0)
	{
		// Prevent divide by 0
		height = 1;
	}

	g_main_window_width = width;
	g_main_window_height = height;

	double screen_aspect_ratio(double(g_main_window_width) / double(g_main_window_height));

	loadPerspectiveProjectionMatrix(g_initial_fovy, screen_aspect_ratio, g_initial_near, g_initial_far);
	/*loadOrthoProjectionMatrix(-width / 2.0, width / 2.0,
			-height / 2.0, height / 2.0,
			0.01, g_initial_far);
	 */

	loadLookAtModelViewMatrix(0.0, 0.0, g_zoom,
			0.0, 0.0, 0.0,
			0.0, 1.0, 0.0);
}


//--------------------------------------------
void mouseButtonCallback(GLFWwindow* apWindow,
						 int aButton,
						 int aButtonState,
						 int aModifierKey)
//--------------------------------------------
{
	g_button = aButton;
	g_button_state = aButtonState;

	// Select the right transformation
	if (g_rotation_mode == NONE)
	{
		switch (g_button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			g_rotation_mode = SCENE;
			break;

		case GLFW_MOUSE_BUTTON_RIGHT:
			g_rotation_mode = OBJECT;
			break;

		case GLFW_MOUSE_BUTTON_MIDDLE:
			g_rotation_mode = DETECTOR;
			break;

		default:
			g_rotation_mode = SCENE;
			break;
		}
	}

	// Use the arc ball
	if (g_button_state == GLFW_PRESS)
	{
		g_use_arc_ball = true;
	}
	// Stop using the arc ball
	else
	{
		g_use_arc_ball = false;
		g_rotation_mode = NONE;
	}

	double xpos(0);
	double ypos(0);
	glfwGetCursorPos (apWindow, &xpos, &ypos);

	g_last_x_position = xpos;
	g_last_y_position = ypos;

	g_current_x_position = xpos;
	g_current_y_position = ypos;
}


//--------------------------------------------------------------
void cursorPosCallback(GLFWwindow* apWindow, double x, double y)
//--------------------------------------------------------------
{
	g_current_x_position = x;
	g_current_y_position = y;

	// Update the rotation matrix if needed
	switch (g_rotation_mode)
	{
	case SCENE:
		computeRotation(g_scene_rotation_matrix);
		break;

	case DETECTOR:
		computeRotation(g_detector_rotation_matrix);
		g_xray_detector.setRotationMatrix(g_detector_rotation_matrix);

		// The X-ray image is not up-to-date
		g_is_xray_image_up_to_date = false;
		break;

	case OBJECT:
		computeRotation(g_sample_rotation_matrix);

		// The X-ray image is not up-to-date
		g_is_xray_image_up_to_date = false;
		break;

	default:
		break;
	}
}


//-----------------------------------------------------------------------
void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset)
//-----------------------------------------------------------------------
{
	// Scrolling along the Y-axis
	if (fabs(yoffset) > EPSILON)
	{
		g_use_arc_ball = false;
		g_zoom += yoffset * cm;
		framebufferSizeCallback(apWindow, g_main_window_width, g_main_window_height);
	}
}


//----------------
void loadSTLFile()
//----------------
{
    // Set geometry
	g_polygon_mesh.setHounsfieldValue(0);

	float red((float(rand()) / (RAND_MAX)) + 1);
	float green((float(rand()) / (RAND_MAX)) + 1);
	float blue((float(rand()) / (RAND_MAX)) + 1);
	float alpha(1.0);

	VEC3 base_colour(red, green, blue);
	base_colour.normalise();

	g_polygon_mesh.getMaterial().setAmbientColour( base_colour.getX() * 0.19225, base_colour.getY() * 0.19225, base_colour.getZ() * 0.19225, alpha);
	g_polygon_mesh.getMaterial().setDiffuseColour( base_colour.getX() * 0.50754, base_colour.getY() * 0.50754, base_colour.getZ() * 0.50754, alpha);
	g_polygon_mesh.getMaterial().setSpecularColour(base_colour.getX() * 0.50827, base_colour.getY() * 0.50827, base_colour.getZ() * 0.50827, alpha);
	g_polygon_mesh.getMaterial().setShininess(50);

    // Compute the translation vector
    VEC3 translation_vector(g_polygon_mesh.getMinCorner() + (g_polygon_mesh.getMaxCorner() - g_polygon_mesh.getMinCorner()) / 2.0);
    g_sample_rotation_matrix = MATRIX4::buildTranslationMatrix(-translation_vector);

    // The X-ray image is not up-to-date
    g_is_xray_image_up_to_date = false;
}


//-----------------
void loadDetector()
//-----------------
{
	g_xray_detector.setResolutionInUnitsOfLengthPerPixel(g_resolution);
	g_xray_detector.setNumberOfPixels(g_number_of_pixels);
	g_xray_detector.setDetectorPosition(g_detector_position);
	g_xray_detector.setUpVector(g_detector_up_vector);

	// The X-ray image is not up-to-date
	g_is_xray_image_up_to_date = false;
}


//---------------
void loadSource()
//---------------
{
	// Set the energy
	g_xray_beam.initialise(g_incident_energy);

	// Set the source position
	g_xray_detector.setXrayPointSource(g_source_position);

	// Use a parallel source
	g_xray_detector.setParallelBeam();

	// The X-ray image is not up-to-date
	g_is_xray_image_up_to_date = false;
}


//----------------------
void loadXRaySimulator()
//----------------------
{
	// Initialise the X-ray renderer
    g_xray_renderer.initialise(XRayRenderer::OPENGL,
            GL_RGB16F,
            &g_xray_detector,
            &g_xray_beam);

	// The X-ray image is not up-to-date
	g_is_xray_image_up_to_date = false;
}


//------------------------------------------
Vec3<GLfloat> getArcballVector(int x, int y)
//------------------------------------------
{
	Vec3<GLfloat> P(2.0 * float(x) / float(g_main_window_width) - 1.0,
			2.0 * float(y) / float(g_main_window_height) - 1.0,
			0);

	P.setY(-P.getY());

	float OP_squared = P.getX() * P.getX() + P.getY() * P.getY();
	if (OP_squared <= 1.0)
	{
		P.setZ(sqrt(1.0 - OP_squared));  // Pythagore
	}
	else
	{
		P.normalise();  // nearest point
	}

	return (P);
}


//--------------------------------
float radian2degree(float anAngle)
//--------------------------------
{
	return (180.0 * anAngle / gVirtualXRay::PI);
}


//--------------------------------------------
void computeRotation(MATRIX4& aRotationMatrix)
//--------------------------------------------
{
	if (g_use_arc_ball)
	{
		if (g_current_x_position != g_last_x_position || g_current_y_position != g_last_y_position)
		{
			Vec3<GLfloat> va(getArcballVector(g_last_x_position,    g_last_y_position));
			Vec3<GLfloat> vb(getArcballVector(g_current_x_position, g_current_y_position));

			float angle(g_rotation_speed * radian2degree(acos(std::min(1.0, va.dotProduct(vb)))));

			Vec3<GLfloat> axis_in_camera_coord(va ^ vb);
			//axis_in_camera_coord.normalize();

			Matrix4x4<GLfloat> camera2object(aRotationMatrix.getInverse());
			Vec3<GLfloat> axis_in_object_coord = camera2object * axis_in_camera_coord;
			//axis_in_object_coord.normalize();

			Matrix4x4<GLfloat> rotation_matrix;
			rotation_matrix.rotate(angle, axis_in_object_coord);
			aRotationMatrix = aRotationMatrix * rotation_matrix;

			g_last_x_position = g_current_x_position;
			g_last_y_position = g_current_y_position;
		}
	}
}


//--------------------
void updateXRayImage()
//--------------------
{
	// The X-ray image is not up-to-date
	if (!g_is_xray_image_up_to_date)
	{
		// Compute the X-Ray image
		g_xray_renderer.computeImage(g_sample_rotation_matrix);

		// Normalise the X-ray image
		g_xray_renderer.normalise();

		// The X-ray image is up-to-date
		g_is_xray_image_up_to_date = true;
	}
}


//---------
void draw()
//---------
{
	// Get the current draw buffer if needed
	GLint back_buffer(0);
	glGetIntegerv(GL_DRAW_BUFFER, &back_buffer);

	// Clear the buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Display the 3D scene
	render();

	// Make sure all the OpenGL code is done
	glFinish();

	// Restore the draw buffer if needed
	glDrawBuffer(back_buffer);
}
