/**
********************************************************************************
*
*	@file		Shader.inl
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
#ifndef EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace Graphics {
using namespace Utilities;


//----------------------------
inline Shader::Shader():
//----------------------------
	m_vertex_source(0),
	m_vertex_source_number_of_lines(0),
	
	m_fragment_source(0),
	m_fragment_source_number_of_lines(0),

	m_valid(false),

	m_shader_program_id(0),
	m_vertex_shader_id(0),
	m_fragment_shader_id(0)
//----------------------------
{
}


//----------------------
inline Shader::~Shader()
//----------------------
{
	reset();
}


//--------------------------------------------------------------------
inline void Shader::setLabels(const std::string& aVertexShaderLabel,
							  const std::string& aFragmentShaderLabel)
//--------------------------------------------------------------------
{
	m_vertex_shader_label   = aVertexShaderLabel;
	m_fragment_shader_label = aFragmentShaderLabel;
}


//--------------------------------------------------
inline unsigned int Shader::getProgramHandle() const
//--------------------------------------------------
{
	return (m_shader_program_id);
}


//---------------------------------
inline bool Shader::isValid() const
//---------------------------------
{
	return (m_valid);
}


//---------------------------------------------
inline void Shader::releaseVertexSourceMemory()
//---------------------------------------------
{
	releaseSourceMemory(m_vertex_source_number_of_lines, m_vertex_source);
}


//-----------------------------------------------
inline void Shader::releaseFragmentSourceMemory()
//-----------------------------------------------
{
	releaseSourceMemory(m_fragment_source_number_of_lines, m_fragment_source);
}


} // namespace Graphics
