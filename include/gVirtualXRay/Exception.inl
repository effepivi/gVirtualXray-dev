/**
********************************************************************************
*
*	@file		Exception.inl
*
*	@brief		Generic class to handle exceptions.
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
#include <sstream>


//******************************************************************************
//	define
//******************************************************************************


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//-----------------------------------------------------------
inline std::ostream& operator<<(std::ostream& anOutputStream,
								const Exception& anException)
//-----------------------------------------------------------
{
	anOutputStream << anException.what();
	
	return (anOutputStream);
}


//------------------------------------------------------
inline Exception::Exception(const char* aFileName,
							const char* aFunctionName,
							int anErrorLine,
							const char* anErrorMessage):
//------------------------------------------------------
		std::exception()
//------------------------------------------------------
{
	std::stringstream error_message;

	if (anErrorMessage)
	{
		error_message << "Error: " << anErrorMessage << std::endl; 
	}
	else
	{
		error_message << "Error: " << std::endl; 
	}
	error_message << "\t- in File: " << aFileName << std::endl; 
	error_message << "\t- in Function: " << aFunctionName << std::endl; 
	error_message << "\t- at Line: " << anErrorLine << std::endl;
	
	m_error_message = error_message.str();
}


//-------------------------------------------------------------
inline Exception::Exception(const char* aFileName,
							const char* aFunctionName,
							int anErrorLine,
							const std::string& anErrorMessage):
//-------------------------------------------------------------
		std::exception()
//-------------------------------------------------------------
{
	std::stringstream error_message;
	error_message << "Error: " << anErrorMessage << std::endl;
	error_message << "\t- in File: " << aFileName << std::endl;
	error_message << "\t- in Function: " << aFunctionName << std::endl;
	error_message << "\t- at Line: " << anErrorLine << std::endl;

	m_error_message = error_message.str();
}


//------------------------------------
inline Exception::~Exception() throw()
//------------------------------------
{}


//------------------------------------------------
inline const char* Exception::what() const throw()
//------------------------------------------------
{
	return (m_error_message.data());
}


} // namespace gVirtualXRay
