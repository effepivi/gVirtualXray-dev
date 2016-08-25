/*

Copyright (c) 2016, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*   @file       Utilities.inl
*
*   @brief      Some utility functions that do not fit anywhere else.
*
*   @version    1.0
*
*   @date       25/08/2016
*
*   @author     Dr Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*   http://www.fpvidal.net/, Dec 2014, 2014, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <cstring>


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//--------------------------------------
inline bool isMHA(const char* aFileName)
//--------------------------------------
{
    unsigned int file_name_size(std::strlen(aFileName));
    if (file_name_size < 5)
        {
        return (false);
        }
    else if (aFileName[file_name_size - 4] != '.')
        {
        return (false);
        }
    else if (aFileName[file_name_size - 3] != 'm' &&
            aFileName[file_name_size - 3] != 'M')
        {
        return (false);
        }
    else if (aFileName[file_name_size - 2] != 'h' &&
            aFileName[file_name_size - 2] != 'H')
        {
        return (false);
        }
    else if (aFileName[file_name_size - 1] != 'a' &&
            aFileName[file_name_size - 1] != 'A')
        {
        return (false);
        }

    return (true);
}


//--------------------------------------
inline bool isMHD(const char* aFileName)
//--------------------------------------
{
    unsigned int file_name_size(std::strlen(aFileName));

    if (file_name_size < 5)
        {
        return (false);
        }
    else if (aFileName[file_name_size - 4] != '.')
        {
        return (false);
        }
    else if (aFileName[file_name_size - 3] != 'm' &&
            aFileName[file_name_size - 3] != 'M')
        {
        return (false);
        }
    else if (aFileName[file_name_size - 2] != 'h' &&
            aFileName[file_name_size - 2] != 'H')
        {
        return (false);
        }
    else if (aFileName[file_name_size - 1] != 'd' &&
            aFileName[file_name_size - 1] != 'D')
        {
        return (false);
        }

    return (true);
}


//--------------------------------------
inline bool isDCM(const char* aFileName)
//--------------------------------------
{
    unsigned int file_name_size(std::strlen(aFileName));

    if (file_name_size < 5)
        {
        return (false);
        }
    else if (aFileName[file_name_size - 4] != '.')
        {
        return (false);
        }
    else if (aFileName[file_name_size - 3] != 'd' &&
            aFileName[file_name_size - 3] != 'D')
        {
        return (false);
        }
    else if (aFileName[file_name_size - 2] != 'c' &&
            aFileName[file_name_size - 2] != 'C')
        {
        return (false);
        }
    else if (aFileName[file_name_size - 1] != 'm' &&
            aFileName[file_name_size - 1] != 'M')
        {
        return (false);
        }

    return (true);
}


//---------------------------------------------
inline bool isMHA(const std::string& aFileName)
//---------------------------------------------
{
    return (isMHA(aFileName.data()));
}


//---------------------------------------------
inline bool isMHD(const std::string& aFileName)
//---------------------------------------------
{
    return (isMHD(aFileName.data()));
}


//---------------------------------------------
inline bool isDCM(const std::string& aFileName)
//---------------------------------------------
{
    return (isDCM(aFileName.data()));
}


//----------------------------------------------
inline bool isJPEG(const std::string& aFileName)
//----------------------------------------------
{
    return (isJPEG(aFileName.data()));
}


//----------------------------------------------
inline bool isTIFF(const std::string& aFileName)
//----------------------------------------------
{
    return (isTIFF(aFileName.data()));
}


} // namespace gVirtualXRay
