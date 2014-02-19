/**
********************************************************************************
*
*	@file		OpenGLUtilities.inl
*
*	@brief		Some utility functions about OpenGL.
*
*	@version	1.0
*
*	@date		07/11/2013
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
#include<GL/glew.h>
#ifdef __APPLE__
#include<OpenGL/gl.h>
#else
#include<GL/gl.h>
#endif

#include <cmath>

#ifndef OPENGL_EXCEPTION_H
#include "gVirtualXRay/OpenGLException.h"
#endif

#ifndef OUT_OF_BOUNDS_EXCEPTION_H
#include "gVirtualXRay/OutOfBoundsException.h"
#endif

#ifndef FBO_EXCEPTION_H
#include "gVirtualXRay/FBOException.h"
#endif

#ifndef TYPES_H
#include "gVirtualXRay/Types.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace Utilities {


//-----------------------------------------------------------
inline void checkOpenGLErrorStatus(const char* aFileName,
								   const char* aFunctionName,
								   int aLineNumber)
//-----------------------------------------------------------
{
	// Get the OpenGL error code
	GLenum error_code(glGetError());

	// There was an error
	if (error_code != GL_NO_ERROR)
	{
		throw OpenGLException(aFileName, aFunctionName, aLineNumber, error_code);
	}
}


//--------------------------------------------------------
inline void checkFBOErrorStatus(const char* aFileName,
								const char* aFunctionName,
								int aLineNumber)
//--------------------------------------------------------
{
	// Get the OpenGL error code
	GLenum error_code(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT));

	// There was an error
	if (error_code != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		throw FBOException(aFileName, aFunctionName, aLineNumber, error_code);
	}
}


//--------------------------------
inline void pushProjectionMatrix()
//--------------------------------
{
	g_p_projection_matrix_stack.push_back(g_current_projection_matrix);
}


//-------------------------------
inline void pushModelViewMatrix()
//-------------------------------
{
	g_p_modelview_matrix_stack.push_back(g_current_modelview_matrix);
}


//-------------------
inline void pushFBO()
//-------------------
{
	// Save the current FBO
	GLint fbo_id(0);
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo_id);

	g_frame_buffer_object_stack.push_back(fbo_id);
}


//-------------------------
inline void pushTexture1D()
//-------------------------
{
	// Save the current texture
	GLint current_texture_id(0);
	glGetIntegerv(GL_TEXTURE_BINDING_1D, &current_texture_id);

	// Get the current 1D texture
	std::pair<GLint, GLint> current_texture_pair(GL_TEXTURE_1D, current_texture_id);

	// Add the last texture
	g_texture_stack.push_back(current_texture_pair);
}


//-------------------------
inline void pushTexture2D()
//-------------------------
{
	// Save the current texture
	GLint current_texture_id(0);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &current_texture_id);

	// Get the current 2D texture
	std::pair<GLint, GLint> current_texture_pair(GL_TEXTURE_2D, current_texture_id);

	// Add the last texture
	g_texture_stack.push_back(current_texture_pair);
}


//--------------------------------
inline void pushTextureRectangle()
//--------------------------------
{
	// Save the current texture
	GLint current_texture_id(0);
	glGetIntegerv(GL_TEXTURE_BINDING_RECTANGLE, &current_texture_id);

	// Get the current 2D texture
	std::pair<GLint, GLint> current_texture_pair(GL_TEXTURE_RECTANGLE, current_texture_id);

	// Add the last texture
	g_texture_stack.push_back(current_texture_pair);
}


//-------------------------
inline void pushTexture3D()
//-------------------------
{
	// Save the current texture
	GLint current_texture_id(0);
	glGetIntegerv(GL_TEXTURE_BINDING_3D, &current_texture_id);

	// Get the current 3D texture
	std::pair<GLint, GLint> current_texture_pair(GL_TEXTURE_3D, current_texture_id);

	// Add the last texture
	g_texture_stack.push_back(current_texture_pair);
}


//---------------------------------------------------
inline void pushEnableDisableState(GLenum aParameter)
//---------------------------------------------------
{
	GLboolean state;
	glGetBooleanv(aParameter, &state);

	g_enable_disable_state_stack.push_back(std::pair<GLenum, GLboolean>(aParameter, state));
}


//-----------------------------
inline void pushShaderProgram()
//-----------------------------
{
	GLint current_shader(0);
	glGetIntegerv(GL_CURRENT_PROGRAM, &current_shader);

	g_shader_program_stack.push_back(current_shader);
}


//-------------------------------
inline void popProjectionMatrix()
//-------------------------------
{
	// Make sure the stack is not empty
	if (g_p_projection_matrix_stack.size() == 0)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}
	
	// Restore the matrix
	g_current_projection_matrix = g_p_projection_matrix_stack.back();

	// Remove the last matrix
	g_p_projection_matrix_stack.pop_back();
}


//------------------------------
inline void popModelViewMatrix()
//------------------------------
{
	// Make sure the stack is not empty
	if (g_p_modelview_matrix_stack.size() == 0)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}
	
	// Restore the matrix
	g_current_modelview_matrix = g_p_modelview_matrix_stack.back();
	applyModelViewMatrix();

	// Remove the last matrix
	g_p_modelview_matrix_stack.pop_back();
}


//------------------
inline void popFBO()
//------------------
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
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, previous_fbo_id);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
	
	// Remove the last FBO
	g_frame_buffer_object_stack.pop_back();
}


//----------------------------
inline void popShaderProgram()
//----------------------------
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


//----------------------------------------
inline void loadIdentityProjectionMatrix()
//----------------------------------------
{
	g_current_projection_matrix.loadIdentity();
}


//---------------------------------------
inline void loadIdentityModelViewMatrix()
//---------------------------------------
{
	g_current_modelview_matrix.loadIdentity();
}


//--------------------------------------------------------
inline MATRIX4 buildOrthoProjectionMatrix(GLdouble left,
										  GLdouble right,
										  GLdouble bottom,
										  GLdouble top,
										  GLdouble nearPlane,
										  GLdouble farPlane)
//--------------------------------------------------------
{
	GLdouble t_x(-(right + left)   / (right - left));
	GLdouble t_y(-(top   + bottom) / (top   - bottom));
	GLdouble t_z(-(farPlane   + nearPlane)   / (farPlane   - nearPlane));

	return (MATRIX4(			
		2.0 / (right - left),
		0.0,
		0.0,
		0.0,
		
		0.0,
		2.0 / (top   - bottom),
		0.0,
		0.0,

		0.0,
		0.0,
		-2.0 / (farPlane   - nearPlane),
		0.0,

		t_x,
		t_y,
		t_z,
		1.0)
	);
}

//----------------------------------------------------
inline void loadOrthoProjectionMatrix(GLdouble left,
									  GLdouble right,
									  GLdouble bottom,
									  GLdouble top,
									  GLdouble nearPlane,
									  GLdouble farPlane)
//----------------------------------------------------
{
	g_current_projection_matrix = buildOrthoProjectionMatrix(left, right, bottom, top, nearPlane, farPlane);
}


//---------------------------------------------------------
inline void loadFrustumProjectionMatrix(GLdouble left,
										GLdouble right,
										GLdouble bottom,
										GLdouble top,
										GLdouble nearPlane,
										GLdouble farPlane)
//---------------------------------------------------------
{
	g_current_projection_matrix = buildFrustumProjectionMatrix(left, right, bottom, top, nearPlane, farPlane);
}


//-------------------------------------------------------------
inline MATRIX4 buildFrustumProjectionMatrix(GLdouble left,
										    GLdouble right,
										    GLdouble bottom,
										    GLdouble top,
										    GLdouble nearPlane,
										    GLdouble farPlane)
//-------------------------------------------------------------
{
	GLdouble a( (right    + left)        / (right    - left));
	GLdouble b( (top      + bottom)      / (top      - bottom));
	GLdouble c(-(farPlane + nearPlane)   / (farPlane - nearPlane));
	GLdouble d(-(2.0 * farPlane * nearPlane)   / (farPlane - nearPlane));

	return (MATRIX4(			
		2.0 * nearPlane / (right - left),
		0.0,
		0.0,
		0.0,
		
		0.0,
		2.0 * nearPlane / (top   - bottom),
		0.0,
		0.0,

		a,
		b,
		c,
		-1.0,

		0.0,
		0.0,
		d,
		0.0)
	);
}


//--------------------------------------------------------
inline void multiplyOrthoProjectionMatrix(GLdouble left,
										  GLdouble right,
										  GLdouble bottom,
										  GLdouble top,
										  GLdouble nearPlane,
										  GLdouble farPlane)
//--------------------------------------------------------
{
	g_current_projection_matrix *= buildOrthoProjectionMatrix(left, right, bottom, top, nearPlane, farPlane);
}


//--------------------------------------------------------------
inline MATRIX4 buildPerspectiveProjectionMatrix(GLdouble fovy,
												GLdouble aspect,
												GLdouble zNear,
												GLdouble zFar)
//--------------------------------------------------------------
{
	GLdouble f(tan(Utilities::PI_2 - (((fovy * Utilities::PI) / 180.0) / 2.0)));

	return (MATRIX4(			
		f / aspect,
		0.0,
		0.0,
		0.0,
			
		0.0,
		f,
		0.0,
		0.0,

		0.0,
		0.0,
		(zFar + zNear) / (zNear - zFar),
		-1.0,

		0.0,
		0.0,
		(2.0 * zFar * zNear) / (zNear - zFar),
		0.0)
	);
}


//----------------------------------------------------------
inline void loadPerspectiveProjectionMatrix(GLdouble fovy,
											GLdouble aspect,
											GLdouble zNear,
											GLdouble zFar)
//----------------------------------------------------------
{
	g_current_projection_matrix = buildPerspectiveProjectionMatrix(fovy, aspect, zNear, zFar);
}


//--------------------------------------------------------------
inline void multiplyPerspectiveProjectionMatrix(GLdouble fovy,
												GLdouble aspect,
												GLdouble zNear,
												GLdouble zFar)
//--------------------------------------------------------------
{	
	g_current_projection_matrix *= buildPerspectiveProjectionMatrix(fovy, aspect, zNear, zFar);
}


inline MATRIX4 buildLookAtModelViewMatrix(const VEC3& eye,
										  const VEC3& center,
										  const VEC3& up)
{
	// Forward vector: centre - eye
	VEC3 forward(center - eye);
	forward.normalize();

	// Up vector
	VEC3 up1(up.normal());

	// Side vector: forward x up
	VEC3 side(forward ^ up);
	
	// New up vector: side x forward
	VEC3 u(side.normal() ^ forward);
		

	MATRIX4 matrix( 	
		side.getX(),
		u.getX(),
		-forward.getX(),
		0.0,
	
		side.getY(),
		u.getY(),
		-forward.getY(),
		0.0,
	
		side.getZ(),
		u.getZ(),
		-forward.getZ(),
		0.0,
	
		0.0,
		0.0,
		0.0,
		1.0);
	
	matrix.translate(-eye.getX(), -eye.getY(), -eye.getZ());
	
	return (matrix);
}

inline MATRIX4 buildLookAtModelViewMatrix(GLdouble eyeX,
										  GLdouble eyeY,
										  GLdouble eyeZ,
										  GLdouble centerX,
										  GLdouble centerY,
										  GLdouble centerZ,
										  GLdouble upX,
										  GLdouble upY,
										  GLdouble upZ)
{
	return (buildLookAtModelViewMatrix(VEC3(eyeX, eyeY, eyeZ), VEC3(centerX, centerY, centerZ), VEC3(upX, upY, upZ)));
}


//-------------------------------------------------------
inline void loadLookAtModelViewMatrix(const VEC3& eye,
									  const VEC3& center,
									  const VEC3& up)
//-------------------------------------------------------
{	
	g_current_modelview_matrix = buildLookAtModelViewMatrix(eye, center, up);
}


//-----------------------------------------------------
inline void loadLookAtModelViewMatrix(GLdouble eyeX,
									  GLdouble eyeY,
									  GLdouble eyeZ,
									  GLdouble centerX,
									  GLdouble centerY,
									  GLdouble centerZ,
									  GLdouble upX,
									  GLdouble upY,
									  GLdouble upZ)
//-----------------------------------------------------
{	
	g_current_modelview_matrix = buildLookAtModelViewMatrix(
			eyeX, eyeY, eyeZ,
			centerX, centerY, centerZ,
			upX, upY, upZ);
}


//------------------------------------------
inline void loadProjectionMatrixFromOpenGL()
//------------------------------------------
{
	float opengl_matrix_data[16];
	glGetFloatv(GL_PROJECTION_MATRIX, opengl_matrix_data);
	g_current_projection_matrix = opengl_matrix_data;
}


//-----------------------------------------
inline void loadModelViewMatrixFromOpenGL()
//-----------------------------------------
{
	float opengl_matrix_data[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, opengl_matrix_data);
	g_current_modelview_matrix = opengl_matrix_data;
}


} // namespace Utilities
