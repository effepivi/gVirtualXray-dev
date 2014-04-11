/**
********************************************************************************
*
*	@file		OpenGLUtilities.cxx
*
*	@brief		Some utility functions about OpenGL.
*
*	@version	1.0
*
*	@date		14/12/2013
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
#ifndef OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef OUT_OF_BOUNDS_EXCEPTION_H
#include "gVirtualXRay/OutOfBoundsException.h"
#endif

#ifdef __APPLE__
#include <OpenGL3.h>
#else
#define GL_GLEXT_PROTOTYPES 1
#include <GL/glcorearb.h>
#include <GL/glext.h>
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace Utilities {


std::vector<MATRIX4> g_p_modelview_matrix_stack;
std::vector<MATRIX4> g_p_projection_matrix_stack;
MATRIX4 g_current_modelview_matrix;
MATRIX4 g_current_projection_matrix;
std::vector<int> g_frame_buffer_object_stack;
std::vector<std::pair<int, int> > g_texture_stack;
std::vector<std::pair<int, bool> > g_enable_disable_state_stack;
std::vector<int> g_shader_program_stack;


//----------------------------------------------------
void checkOpenGLErrorStatus(const char* aFileName,
							const char* aFunctionName,
							int aLineNumber)
//----------------------------------------------------
{
	// Get the OpenGL error code
	GLenum error_code(glGetError());

	// There was an error
	if (error_code != GL_NO_ERROR)
	{
		throw OpenGLException(aFileName, aFunctionName, aLineNumber, error_code);
	}
}


//-------------------------------------------------
void checkFBOErrorStatus(const char* aFileName,
						 const char* aFunctionName,
						 int aLineNumber)
//-------------------------------------------------
{
	// Get the OpenGL error code
#ifdef __APPLE__
	GLenum error_code(glCheckFramebufferStatus(GL_FRAMEBUFFER));
#else
	GLenum error_code(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER));
#endif

	// There was an error
	if (error_code != GL_FRAMEBUFFER_COMPLETE)
	{
		throw FBOException(aFileName, aFunctionName, aLineNumber, error_code);
	}
}


//------------
void pushFBO()
//------------
{
	// Save the current FBO
	GLint fbo_id(0);
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo_id);

	g_frame_buffer_object_stack.push_back(fbo_id);
}


//------------------
void pushTexture1D()
//------------------
{
	// Save the current texture
	GLint current_texture_id(0);
	glGetIntegerv(GL_TEXTURE_BINDING_1D, &current_texture_id);

	// Get the current 1D texture
	std::pair<GLint, GLint> current_texture_pair(GL_TEXTURE_1D, current_texture_id);

	// Add the last texture
	g_texture_stack.push_back(current_texture_pair);
}


//------------------
void pushTexture2D()
//------------------
{
	// Save the current texture
	GLint current_texture_id(0);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &current_texture_id);

	// Get the current 2D texture
	std::pair<GLint, GLint> current_texture_pair(GL_TEXTURE_2D, current_texture_id);

	// Add the last texture
	g_texture_stack.push_back(current_texture_pair);
}


//-------------------------
void pushTextureRectangle()
//-------------------------
{
	// Save the current texture
	GLint current_texture_id(0);
	glGetIntegerv(GL_TEXTURE_BINDING_RECTANGLE, &current_texture_id);

	// Get the current 2D texture
	std::pair<GLint, GLint> current_texture_pair(GL_TEXTURE_RECTANGLE, current_texture_id);

	// Add the last texture
	g_texture_stack.push_back(current_texture_pair);
}


//------------------
void pushTexture3D()
//------------------
{
	// Save the current texture
	GLint current_texture_id(0);
	glGetIntegerv(GL_TEXTURE_BINDING_3D, &current_texture_id);

	// Get the current 3D texture
	std::pair<GLint, GLint> current_texture_pair(GL_TEXTURE_3D, current_texture_id);

	// Add the last texture
	g_texture_stack.push_back(current_texture_pair);
}


//-----------------------------------------
void pushEnableDisableState(int aParameter)
//-----------------------------------------
{
	GLboolean state;
	glGetBooleanv(aParameter, &state);

	g_enable_disable_state_stack.push_back(std::pair<int, bool>(aParameter, state));
}


//----------------------
void pushShaderProgram()
//----------------------
{
	GLint current_shader(0);
	glGetIntegerv(GL_CURRENT_PROGRAM, &current_shader);

	g_shader_program_stack.push_back(current_shader);
}


//-----------
void popFBO()
//-----------
{
	// Make sure the stack is not empty
	if (g_frame_buffer_object_stack.size() == 0)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	// Save the current FBO
	GLint current_fbo_id(0);
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_fbo_id);

	// Get the previous FBO from the stack
	GLint previous_fbo_id(g_frame_buffer_object_stack.back());

	// The two FBOs are different
	if (previous_fbo_id != current_fbo_id)
	{
		// Restore the FBO
		glBindFramebuffer(GL_FRAMEBUFFER, previous_fbo_id);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	// Remove the last FBO
	g_frame_buffer_object_stack.pop_back();
}


//---------------------
void popShaderProgram()
//---------------------
{
	// Make sure the stack is not empty
	if (g_shader_program_stack.size() == 0)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	// Save the current program
	GLint current_shader(0);
	glGetIntegerv(GL_CURRENT_PROGRAM, &current_shader);

	// Get the previous program from the stack
	GLint previous_program(g_shader_program_stack.back());

	// The two programs are different
	if (previous_program != current_shader)
	{
		// Restore the FBO
		glUseProgram(previous_program);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	// Remove the last program
	g_shader_program_stack.pop_back();
}


//---------------
void popTexture()
//---------------
{
	// Make sure the stack is not empty
	if (g_texture_stack.size() == 0)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	// Get the previous texture from the stack
	std::pair<int, int> previous_texture(g_texture_stack.back());

	// Remove the last texture
	g_texture_stack.pop_back();

	// Save the current texture
	GLint current_texture_id(0);

	// Get the texture binding according to the texture type
	switch (previous_texture.first)
	{
	case GL_TEXTURE_1D:
		glGetIntegerv(GL_TEXTURE_BINDING_1D, &current_texture_id);
		break;

	case GL_TEXTURE_2D:
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &current_texture_id);
		break;

	case GL_TEXTURE_3D:
		glGetIntegerv(GL_TEXTURE_BINDING_3D, &current_texture_id);
		break;

	case GL_TEXTURE_RECTANGLE:
		glGetIntegerv(GL_TEXTURE_BINDING_RECTANGLE, &current_texture_id);
		break;

	default:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid texture type");
	}

	// The two textures are different
	if (previous_texture.second != current_texture_id)
	{
		//Restore the texture binding
		switch (previous_texture.first)
		{
		case GL_TEXTURE_1D:
			glBindTexture(GL_TEXTURE_1D, previous_texture.second);
			break;

		case GL_TEXTURE_2D:
			glBindTexture(GL_TEXTURE_2D, previous_texture.second);
			break;

		case GL_TEXTURE_3D:
			glBindTexture(GL_TEXTURE_3D, previous_texture.second);
			break;

		case GL_TEXTURE_RECTANGLE:
			glBindTexture(GL_TEXTURE_RECTANGLE, previous_texture.second);
			break;

		default:
			throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid texture type");
		}
	}
}


//--------------------------
void popEnableDisableState()
//--------------------------
{
	// Make sure the stack is not empty
	if (g_enable_disable_state_stack.size() == 0)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	// Get the previous state from the stack
	std::pair<int, bool> previous_state(g_enable_disable_state_stack.back());

	// Remove the last state
	g_enable_disable_state_stack.pop_back();

	// Save the current state
	GLboolean current_state(0);

	// Get the state according to the state type
	glGetBooleanv(previous_state.first, &current_state);

	// The two states are different
	if (previous_state.second != current_state)
	{
		if (previous_state.second)
		{
			glEnable(previous_state.first);
		}
		else
		{
			glDisable(previous_state.first);
		}
	}
}


//-------------------------
void applyModelViewMatrix()
//-------------------------
{
	// Store the current buffer
	GLint program_handle(0);
	glGetIntegerv(GL_CURRENT_PROGRAM, &program_handle);

	// Load the matrices
	if (program_handle)
	{
		GLuint modelview_matrix_handle(glGetUniformLocation(program_handle, "g_modelview_matrix"));
		glUniformMatrix4fv(modelview_matrix_handle, 1, GL_FALSE, g_current_modelview_matrix.get());
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		MATRIX4 normal_matrix(g_current_modelview_matrix.getInverse().getTranspose());
		GLuint normal_matrix_handle(glGetUniformLocation(program_handle, "g_normal_matrix"));
		glUniformMatrix3fv(normal_matrix_handle, 1, GL_FALSE, normal_matrix.get3x3());
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
}


} // namespace Utilities
