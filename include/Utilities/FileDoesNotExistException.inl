/**
********************************************************************************
*
*	@file		FileDoesNotExistException.inl
*
*	@brief		Class to handle exceptions when trying to open a read-only file
*				that is not accessible.
*
*	@version	1.0
*
*	@date		30/11/2013
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
//	namespace
//******************************************************************************
namespace Utilities {


//-----------------------------------------------------------------------------------------
inline FileDoesNotExistException::FileDoesNotExistException(const char* aFileName,
															const char* aFunctionName,
															int anErrorLine,
															const char* aNameOfFileToOpen):
//-----------------------------------------------------------------------------------------
		Exception(aFileName, aFunctionName, anErrorLine)
//-----------------------------------------------------------------------------------------
{
	std::stringstream error_message;
	error_message << "Error: cannot open the file \"" << aNameOfFileToOpen << "\"" << std::endl; 
	error_message << "\t- in File: " << aFileName << std::endl; 
	error_message << "\t- in Function: " << aFunctionName << std::endl; 
	error_message << "\t- at Line: " << anErrorLine << std::endl;
	
	m_error_message = error_message.str();
}


//------------------------------------------------------------------------------------------------
inline FileDoesNotExistException::FileDoesNotExistException(const char* aFileName,
															const char* aFunctionName,
															int anErrorLine,
															const std::string& aNameOfFileToOpen):
//------------------------------------------------------------------------------------------------
		Exception(aFileName, aFunctionName, anErrorLine)
//------------------------------------------------------------------------------------------------
{
	std::stringstream error_message;
	error_message << "Error: cannot open the file \"" << aNameOfFileToOpen << "\"" << std::endl; 
	error_message << "\t- in File: " << aFileName << std::endl; 
	error_message << "\t- in Function: " << aFunctionName << std::endl; 
	error_message << "\t- at Line: " << anErrorLine << std::endl;
	
	m_error_message = error_message.str();
}


} // namespace Utilities
