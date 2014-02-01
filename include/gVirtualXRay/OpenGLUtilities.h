/**
********************************************************************************
*
*	@file		OpenGLUtilities.h
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


#ifndef OPENGL_UTILITIES_H
#define OPENGL_UTILITIES_H


//******************************************************************************
//	Include
//******************************************************************************
#include <vector>

#ifndef MATRIX4X4_H
#include "gVirtualXRay/Matrix4x4.h"
#endif

#ifndef TYPES_H
#include "gVirtualXRay/Types.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace Utilities {


//------------------------------------------------------------------------------
///	Check OpenGL's error status.
/**
*	@param 	aFileName:	name of the source file where OpenGL is checked
*	@param 	aFunctionName:	name of the function where OpenGL is checked
*	@param 	aLineNumber:	name at which OpenGL is checked
*/
//------------------------------------------------------------------------------
void checkOpenGLErrorStatus(const char* aFileName, const char* aFunctionName, int aLineNumber);


//------------------------------------------------------------------------------
///	Check the current FBO's error status.
/**
*	@param 	aFileName:	name of the source file where FBO is checked
*	@param 	aFunctionName:	name of the function where FBO is checked
*	@param 	aLineNumber:	name at which FBO is checked
*/
//------------------------------------------------------------------------------
void checkFBOErrorStatus(const char* aFileName, const char* aFunctionName, int aLineNumber);


//------------------------------------------------------------------------------
///	Add the current matrix to the projection matrix stack.
//------------------------------------------------------------------------------
void pushProjectionMatrix();


//------------------------------------------------------------------------------
///	Add the current matrix to the model/view matrix stack.
//------------------------------------------------------------------------------
void pushModelViewMatrix();


//------------------------------------------------------------------------------
///	Add the current FBO binding to the FBO stack.
//------------------------------------------------------------------------------
void pushFBO();


//------------------------------------------------------------------------------
///	Add the current 1D texture binding to the texture stack.
//------------------------------------------------------------------------------
void pushTexture1D();


//------------------------------------------------------------------------------
///	Add the current 2D texture binding to the texture stack.
//------------------------------------------------------------------------------
void pushTexture2D();


//------------------------------------------------------------------------------
///	Add the current rectangle texture binding to the texture stack.
//------------------------------------------------------------------------------
void pushTextureRectangle();


//------------------------------------------------------------------------------
///	Add the current 3D texture binding to the texture stack.
//------------------------------------------------------------------------------
void pushTexture3D();


void pushEnableDisableState(GLenum aParameter);


//------------------------------------------------------------------------------
///	Restore the matrix from the projection matrix stack,
///	and remove the last matrix of the stack.
//------------------------------------------------------------------------------
void popProjectionMatrix();


//------------------------------------------------------------------------------
///	Restore the matrix from the model.view matrix stack,
///	and remove the last matrix of the stack.
//------------------------------------------------------------------------------
void popModelViewMatrix();


//------------------------------------------------------------------------------
///	Restore the FBO binding from the FBO stack,
///	and remove the last FBO of the stack.
//------------------------------------------------------------------------------
void popFBO();


//------------------------------------------------------------------------------
///	Restore the texture binding from the texture stack,
///	and remove the last texture of the stack.
//------------------------------------------------------------------------------
void popTexture();


void popEnableDisableState();


//------------------------------------------------------------------------------
///	Replace the projection matrix by the identity matrix.
//------------------------------------------------------------------------------
void loadIdentityProjectionMatrix();


//------------------------------------------------------------------------------
///	Replace the model/view matrix by the identity matrix.
//------------------------------------------------------------------------------
void loadIdentityModelViewMatrix();


//------------------------------------------------------------------------------
///	Replace the projection matrix by a perspective projection matrix.
//------------------------------------------------------------------------------
void loadPerspectiveProjectionMatrix(GLdouble  fovy,  GLdouble  aspect,  GLdouble  zNear,  GLdouble  zFar);


void loadOrthoProjectionMatrix(GLdouble left,
        GLdouble right,
        GLdouble bottom,
        GLdouble top,
        GLdouble near,
        GLdouble far);


MATRIX4 buildOrthoProjectionMatrix(GLdouble left,
        GLdouble right,
        GLdouble bottom,
        GLdouble top,
        GLdouble near,
        GLdouble far);


MATRIX4 buildPerspectiveProjectionMatrix(GLdouble  fovy,
		GLdouble aspect,
		GLdouble zNear,
		GLdouble
		zFar);


void loadLookAtModelViewMatrix(const VEC3& eye,
		const VEC3& center,
		const VEC3& up);


void loadLookAtModelViewMatrix(GLdouble eyeX,
		GLdouble eyeY,
		GLdouble eyeZ,
		GLdouble centerX,
		GLdouble centerY,
		GLdouble centerZ,
		GLdouble upX,
		GLdouble upY,
		GLdouble upZ);


MATRIX4 buildLookAtModelViewMatrix(const VEC3& eye,
		  const VEC3& center,
		  const VEC3& up);


MATRIX4 buildLookAtModelViewMatrix(GLdouble eyeX,
		GLdouble eyeY,
		GLdouble eyeZ,
		GLdouble centerX,
		GLdouble centerY,
		GLdouble centerZ,
		GLdouble upX,
		GLdouble upY,
		GLdouble upZ);


void applyModelViewMatrix();


/// The stack of model/view matrices
extern std::vector<MATRIX4> g_p_modelview_matrix_stack;


/// The stack of projection matrix
extern std::vector<MATRIX4> g_p_projection_matrix_stack;


/// The model/view matrices
extern MATRIX4 g_current_modelview_matrix;


/// The projection matrix
extern MATRIX4 g_current_projection_matrix;


/// The stack of FBO binding
extern std::vector<GLint> g_frame_buffer_object_stack;


/// The stack of texture binding
extern std::vector<std::pair<GLenum, GLint> > g_texture_stack;


/// The stack of enable/disable states
extern std::vector<std::pair<GLenum, GLboolean> > g_enable_disable_state_stack;


} // namespace Utilities


//******************************************************************************
#include "OpenGLUtilities.inl"


#endif // OPENGL_UTILITIES_H
