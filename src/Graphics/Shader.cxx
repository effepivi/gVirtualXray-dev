/**
********************************************************************************
*
*	@file		Shader.cxx
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



//******************************************************************************
//	Include
//******************************************************************************
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstring>

#ifndef SHADER_H
#include "gVirtualXRay/Graphics/Shader.h"
#endif

#ifndef FILE_DOES_NOT_EXIST_EXCEPTION_H
#include "gVirtualXRay/Utilities/FileDoesNotExistException.h"
#endif

#ifndef OUT_OF_MEMORY_EXCEPTION_H
#include "gVirtualXRay/Utilities/OutOfMemoryException.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace Graphics;


//******************************************************************************
//	Static members
//******************************************************************************
bool Shader::m_shader_capable = false;


//------------------
void Shader::reset()
//------------------
{
	// Get the current program
	GLint current_shader(0);

	if (m_shader_program_id)
	{
		glGetIntegerv(GL_CURRENT_PROGRAM, &current_shader);
	}

	// The current program is the same as the one that being deleted.
	// Make sure it is disable.
	if (m_shader_program_id && current_shader == m_shader_program_id)
	{
		disable();
	}

	// Delete the fragment shader
	if (m_fragment_shader_id)
	{
		glDeleteShader(m_fragment_shader_id);
		m_fragment_shader_id = 0;
	}

	// Delete the vertex shader
	if (m_vertex_shader_id)
	{
		glDeleteShader(m_vertex_shader_id);
		m_vertex_shader_id = 0;
	}

	// Delete the shader program
	if (m_shader_program_id)
	{
		glDeleteProgram(m_shader_program_id);
		m_shader_program_id = 0;
	}

	// Release memory
	releaseVertexSourceMemory();
	releaseFragmentSourceMemory();

	m_valid = false;
}


//----------------------------------------------------------------
void Shader::loadFiles(const std::string& aVertexShaderFilename,
					   const std::string& aFragmentShaderFilename)
//----------------------------------------------------------------
{
	// Reset the shader and restore the default values
	reset();

	// Create the shader program
	m_shader_program_id = glCreateProgram();

	// Create the shader program has not been created
	if (!m_shader_program_id)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create a shader program.");
	}

	loadVertexShaderFile(aVertexShaderFilename);
	loadFragmentShaderFile(aFragmentShaderFilename);
	linkProgram();

	m_valid = true;

	m_shader_capable = true;
}


//-------------------------------------------------------------------
void Shader::loadSource(const std::string& aVertexShaderSourceCode,
						const std::string& aFragmentShaderSourceCode)
//-------------------------------------------------------------------
{
	// Reset the shader and restore the default values
	reset();

	// Create the shader program
	m_shader_program_id = glCreateProgram();

	// Create the shader program has not been created
	if (!m_shader_program_id)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create a shader program.");
	}

	loadVertexShaderSource(aVertexShaderSourceCode);
	loadFragmentShaderSource(aFragmentShaderSourceCode);
	linkProgram();
	
	m_valid = true;

	m_shader_capable = true;
}


//-------------------------------------------------------------------------
void Shader::loadVertexShaderFile(const std::string& aVertexShaderFilename)
//-------------------------------------------------------------------------
{
	// Store the name of the file
	m_vertex_shader_filename = aVertexShaderFilename;

	// The name has changed, therefore the shader is not loaded
	m_valid = false;

	// Release the memory if needed
	releaseVertexSourceMemory();

	// Load the file
	m_vertex_source_number_of_lines = loadFile(m_vertex_shader_filename, m_vertex_source);

	// Compile the shader
	m_vertex_shader_id = compileShader(GL_VERTEX_SHADER, m_vertex_source_number_of_lines, m_vertex_source);
}


//-----------------------------------------------------------------------------
void Shader::loadFragmentShaderFile(const std::string& aFragmentShaderFilename)
//-----------------------------------------------------------------------------
{
	// Store the name of the file
	m_fragment_shader_filename = aFragmentShaderFilename;

	// The name has changed, therefore the shader is not loaded
	m_valid = false;

	// Release the memory if needed
	releaseFragmentSourceMemory();

	// Load the file
	m_fragment_source_number_of_lines = loadFile(m_fragment_shader_filename, m_fragment_source);

	// Compile the shader
	m_fragment_shader_id = compileShader(GL_FRAGMENT_SHADER, m_fragment_source_number_of_lines, m_fragment_source);
}


//-----------------------------------------------------------------------------
void Shader::loadVertexShaderSource(const std::string& aVertexShaderSourceCode)
//-----------------------------------------------------------------------------
{
	// Store the name of the file
	m_vertex_shader_filename = "Built in.";

	// The name has changed, therefore the shader is not loaded
	m_valid = false;

	// Release the memory if needed
	releaseVertexSourceMemory();

	// Load the file
	m_vertex_source = new GLchar*[1];
	m_vertex_source[0] = new GLchar[aVertexShaderSourceCode.size() + 1];
	strcpy(m_vertex_source[0], aVertexShaderSourceCode.c_str());

	// Count the number of new line characters ('\n')
	m_vertex_source_number_of_lines = 1;

	// Compile the shader
	m_vertex_shader_id = compileShader(GL_VERTEX_SHADER, m_vertex_source_number_of_lines, m_vertex_source);
}


//---------------------------------------------------------------------------------
void Shader::loadFragmentShaderSource(const std::string& aFragmentShaderSourceCode)
//---------------------------------------------------------------------------------
{
	// Store the name of the file
	m_fragment_shader_filename = "Built in.";

	// The name has changed, therefore the shader is not loaded
	m_valid = false;

	// Release the memory if needed
	releaseFragmentSourceMemory();

	// Load the file
	m_fragment_source = new GLchar*[1];
	m_fragment_source[0] = new GLchar[aFragmentShaderSourceCode.size() + 1];
	strcpy(m_fragment_source[0], aFragmentShaderSourceCode.c_str());

	// Count the number of new line characters ('\n')
	m_fragment_source_number_of_lines = 1;

	// Compile the shader
	m_fragment_shader_id = compileShader(GL_FRAGMENT_SHADER, m_fragment_source_number_of_lines, m_fragment_source);
}


//------------------------------------------------------
int Shader::loadFile(const std::string& aShaderFilename,
					 GLchar**& aShaderSource)
//------------------------------------------------------
{
	int line_number(0);

	// Open the file
	std::ifstream input_stream(aShaderFilename.data());

	// Check if the file is open
	if (!input_stream.is_open())
	{
		throw FileDoesNotExistException(__FILE__, __FUNCTION__, __LINE__, aShaderFilename);
	}
	
	// Count the number of new line characters ('\n')
	line_number = count(std::istreambuf_iterator<char>(input_stream), std::istreambuf_iterator<char>(), '\n');

	// Count the last line
	++line_number;

	try
	{
		// Allocate the memory
		aShaderSource = new char * [line_number];

		// Initialise the memory
		std::fill_n(aShaderSource, line_number, static_cast<char*>(0));


		// Read the data from the beginning of the file
		input_stream.seekg(std::ios_base::beg);

		std::string line;
		for (int allocated_size(0);
				allocated_size < line_number;
				++allocated_size)
		{
			// Read the current line
			getline(input_stream, line);

			// Add the end of line character
			line += "\n";

			// Allocate the memory to store the line
			aShaderSource[allocated_size] = new char [line.size() + 1];

			// Copy the line
			strcpy(aShaderSource[allocated_size], line.c_str());
		}
	}
	// There was a memory allocation error
	catch (const std::bad_alloc &)
	{
		// Release the memory that has been successfully allocated
		releaseSourceMemory(line_number, aShaderSource);

		// Close the file
		input_stream.close();

		// Throw an exception
		throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
	}

	// Close the file
	input_stream.close();

	return (line_number);
}



//---------------------------------------------------------------------------------------
GLuint Shader::compileShader(GLenum aShaderType, int aLineNumber, GLchar** aShaderSource)
//---------------------------------------------------------------------------------------
{
	GLuint shader_handle(0);
	GLint compile_status(GL_TRUE);
	GLint log_size(0);
	char* p_log_message(0);

	shader_handle = glCreateShader(aShaderType);

	if (!shader_handle)
	{
		switch (aShaderType)
		{
		case GL_VERTEX_SHADER:
			throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create the vertex shader.");
			break;

		case GL_FRAGMENT_SHADER:
			throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create the vertex shader.");
			break;

		default:
			throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create shader with an unknown type.");
			break;
		}
	}

	// Assign the source code to the shader
	glShaderSource(shader_handle, aLineNumber, (const GLchar**) aShaderSource, NULL);

	// Compile the source code
	glCompileShader(shader_handle);

	// Check the compilation status
	glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &compile_status);

	// there was at least one error
	if (compile_status != GL_TRUE)
	{
		// Get the size of the log message
		glGetProgramiv(m_shader_program_id, GL_INFO_LOG_LENGTH, &log_size);

		// Allocate memory to store the log message
		if (log_size)
		{
			p_log_message = new char[log_size + 1];
		}

		// The memory was not allocated
		if (!p_log_message)
		{
			std::stringstream error_message;
			error_message << "Shader program linkage error in:" << std::endl;
			error_message << *aShaderSource;
			throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
		}

		// Get the log message
		glGetProgramInfoLog(m_shader_program_id, log_size, &log_size, p_log_message);

		std::stringstream error_message;
		error_message << "Shader program linkage error (see below)." << std::endl;
		error_message << p_log_message;

		// Release the memory used to store the log message
		delete [] p_log_message;

		// Delete the shader
		glDeleteShader(shader_handle);

		throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
	}

	// Attache the shader to the program
	if (shader_handle)
	{
		glAttachShader(m_shader_program_id, shader_handle);
	}
	
	return (shader_handle);
}


//------------------------
void Shader::linkProgram()
//------------------------
{
	GLint link_status(0);
	GLint log_size(0);
	char* p_log_message(0);

	// Linkage
	glLinkProgram(m_shader_program_id);

	// Check the status of the linkage
	glGetProgramiv(m_shader_program_id, GL_LINK_STATUS, &link_status);

	// There was at least one error
	if (link_status != GL_TRUE)
	{
		// Get the size of the log message
		glGetProgramiv(m_shader_program_id, GL_INFO_LOG_LENGTH, &log_size);

		// Allocate memory to store the log message
		p_log_message = new char[log_size + 1];

		// The memory was not allocated
		if (!p_log_message)
		{
			throw Exception(__FILE__, __FUNCTION__, __LINE__, "Shader program linkage error.");
		}

		// Get the log message
		glGetProgramInfoLog(m_shader_program_id, log_size, &log_size, p_log_message);

		std::stringstream error_message;
		error_message << "Shader program linkage error (see below)." << std::endl;
		error_message << p_log_message;

		// Release the memory used to store the log message
		delete [] p_log_message;
		
		throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
	}
}


//-------------------------------------------------------------------------
void Shader::releaseSourceMemory(int& aLineNumber, GLchar**& aShaderSource)
//-------------------------------------------------------------------------
{
	if (aShaderSource)
	{
		for (int i(0); i < aLineNumber; ++i)
		{
			if (aShaderSource[i])
			{
				delete [] aShaderSource[i];
				aShaderSource[i] = 0;
			}
		}

		delete [] aShaderSource;
		aShaderSource = 0;
	}
}
