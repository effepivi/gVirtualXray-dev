/**
********************************************************************************
*
*	@file		Shader.h
*
*	@brief		Class to handle GLSL shaders.
*
*	@version	1.0
*
*	@date		29/11/2013
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

#ifndef SHADER_H
#define SHADER_H


//******************************************************************************
//	Include
//******************************************************************************
#include <string>
#include <GL/glew.h>


//******************************************************************************
//	define
//******************************************************************************


//******************************************************************************
//	namespace
//******************************************************************************
namespace Graphics {


//==============================================================================
/**
*	@class	Shader
*	@brief	Shader is a class to handle shaders written in GLSL.
*/
//==============================================================================
class Shader
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
	//--------------------------------------------------------------------------
	///	Default constructor.
	//--------------------------------------------------------------------------
	Shader();


	//--------------------------------------------------------------------------
	///	Destructor.
	//--------------------------------------------------------------------------
	~Shader();


	//--------------------------------------------------------------------------
	///	Enable the current shader
	//--------------------------------------------------------------------------
	void enable() const;


	//--------------------------------------------------------------------------
	///	Disable the current shader (if any)
	//--------------------------------------------------------------------------
	static void disable();


	//--------------------------------------------------------------------------
	///	Reset (delete the current shader if it is loaded, and restore default
	///	values).
	//--------------------------------------------------------------------------
	void reset();


	//--------------------------------------------------------------------------
	///	Load the shader from files.
	/**
	*	@param aVertexShaderFilename: file name of the vertex shader
	*	@param aFragmentShaderFilename: file name of the fragment shader
	*/
	//--------------------------------------------------------------------------
	void loadFiles(const std::string& aVertexShaderFilename,
			const std::string& aFragmentShaderFilename);


	//--------------------------------------------------------------------------
	///	Load the vertex shader from a file
	/**
	*	@param aVertexShaderFilename: file name of the vertex shader
	*/
	//--------------------------------------------------------------------------
	void loadVertexShaderFile(const std::string& aVertexShaderFilename);


	//--------------------------------------------------------------------------
	///	Load the fragment shader from a file
	/**
	*	@param aFragmentShaderFilename: file name of the fragment shader
	*/
	//--------------------------------------------------------------------------
	void loadFragmentShaderFile(const std::string& aFragmentShaderFilename);


	void loadSource(const std::string& aVertexShaderSourceCode,
			const std::string& aFragmentShaderSourceCode);


	//--------------------------------------------------------------------------
	///	Load the vertex shader from a string
	/**
	*	@param aVertexShaderSourceCode: the vertex shader source code
	*/
	//--------------------------------------------------------------------------
	void loadVertexShaderSource(const std::string& aVertexShaderSourceCode);


	//--------------------------------------------------------------------------
	///	Load the fragment shader from a string
	/**
	*	@param aFragmentShaderSourceCode: the fragment shader source code
	*/
	//--------------------------------------------------------------------------
	void loadFragmentShaderSource(const std::string& aFragmentShaderSourceCode);


	//--------------------------------------------------------------------------
	///	Accessor on the shader program handle
	/**
	*	@return the shader program handle
	*/
	//--------------------------------------------------------------------------
	GLuint getProgramHandle() const;


	//--------------------------------------------------------------------------
	///	Accessor on the validity status of the shader
	/**
	*	@return true if the shader is valid, false if it is not
	*/
	//--------------------------------------------------------------------------
	bool isValid() const;


//******************************************************************************
protected:
	//--------------------------------------------------------------------------
	///	Load a text file and store its content
	/**
	*	@param aShaderFilename: file name of the vertex or fragment shader
	*	@param aShaderSource: source code of the vertex or fragment shader
	*	@return the number of lines in the file
	*/
	//--------------------------------------------------------------------------
	int loadFile(const std::string& aShaderFilename, GLchar**& aShaderSource);

	
	//--------------------------------------------------------------------------
	///	Compile the vertex shader or the fragment shader
	/**
	*	@param aShaderType: type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
	*	@param aLineNumber: the number of lines of the source code
	*	@param aShaderSource: source code of the vertex or fragment shader
	*	@return the corresponding ID
	*/
	//--------------------------------------------------------------------------
	GLuint compileShader(GLenum aShaderType, int aLineNumber, GLchar** aShaderSource);


	//--------------------------------------------------------------------------
	///	Link the vertex and fragment programs
	//--------------------------------------------------------------------------
	void linkProgram();


	//--------------------------------------------------------------------------
	///	Release the memory held by the vertex shader
	//--------------------------------------------------------------------------
	void releaseVertexSourceMemory();


	//--------------------------------------------------------------------------
	///	Release the memory held by the fragment shader
	//--------------------------------------------------------------------------
	void releaseFragmentSourceMemory();


	//--------------------------------------------------------------------------
	///	Release the memory held by the vertex shader or the fragment shader
	/**
	*	@param aLineNumber: the number of lines of the source code
	*	@param aShaderSource: source code of the vertex or fragment shader
	*/
	//--------------------------------------------------------------------------
	void releaseSourceMemory(int& aLineNumber, GLchar**& aShaderSource);


	/// Flag set to true when a shader if fully valid
	static bool m_shader_capable;


	/// Name of the file containing the vertex shader
	std::string m_vertex_shader_filename;


	/// Name of the file containing the fragment shader
	std::string m_fragment_shader_filename;


	/// Source code of the vertex shader
	GLchar **m_vertex_source;


	/// Number of line of the vertex shader
	int m_vertex_source_number_of_lines;


	/// Source code of the fragment shader
	GLchar **m_fragment_source;


	/// Number of line of the fragment shader
	int m_fragment_source_number_of_lines;


	/// Flag set to true if the vertex shader,
	/// the fragment shader and
	/// the shader program are all valid
	bool m_valid;
	

	/// ID of the shader program
	GLuint m_shader_program_id;


	/// ID of the vertex shader
	GLuint m_vertex_shader_id;


	/// ID of the fragment shader
	GLuint m_fragment_shader_id;
};


} // namespace Graphics


//******************************************************************************
#include "Shader.inl"


#endif // SHADER_H

