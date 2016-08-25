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
*   file        Utilities.cxx
*
*  @brief       Some utility functions that do not fit anywhere else.
*
*  @version     1.0
*
*  @date        12/02/2014
*
*  @author      Dr Franck P. Vidal
*
*  @section     License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*  @section     Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <zlib.h>
#include <string.h>

#ifndef __Utilities_h
#include "gVirtualXRay/Utilities.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//----------------------------------------------------
int inflate(const void *src, int srcLen, char** apDst)
//----------------------------------------------------
{
    // See http://stackoverflow.com/questions/4901842/
    //                   in-memory-decompression-with-zlib
    int dstLen(50 * srcLen);
    if (*apDst) delete [] *apDst;
    *apDst = new char[dstLen];
    memset(*apDst, '\0', dstLen * sizeof(char));

    z_stream strm  = {0};
    strm.total_in  = strm.avail_in  = srcLen;
    strm.total_out = strm.avail_out = dstLen;
    strm.next_in   = (Bytef *) src;
    strm.next_out  = (Bytef *) (*apDst);

    strm.zalloc = Z_NULL;
    strm.zfree  = Z_NULL;
    strm.opaque = Z_NULL;

    int err = -1;
    int ret = -1;

    //15 window bits, and the +32 tells zlib to to detect if using gzip or zlib
    err = inflateInit2(&strm, (15 + 32));
    if (err == Z_OK)
        {
        err = inflate(&strm, Z_FINISH);

        if (err == Z_STREAM_END)
            {
            ret = strm.total_out;
            }
        else
            {
            inflateEnd(&strm);
            delete [] *apDst;
            *apDst = 0;
            return (err);
            }
        }
    else
        {
        inflateEnd(&strm);
        delete [] *apDst;
        *apDst = 0;
        return (err);
        }

    inflateEnd(&strm);
    return (ret);
}


//--------------------------------
bool isJPEG(const char* aFileName)
//--------------------------------
{
    unsigned int file_name_size(std::strlen(aFileName));

    // There are enough letters in the file name to store x.jpg
    if (file_name_size >= 5)
        {
        // The extension is JPG
        if (aFileName[file_name_size - 4] == '.' &&
                (aFileName[file_name_size - 3] == 'j' ||
                        aFileName[file_name_size - 3] == 'J') &&
                (aFileName[file_name_size - 2] == 'p' ||
                        aFileName[file_name_size - 2] == 'P') &&
                (aFileName[file_name_size - 1] == 'g' ||
                        aFileName[file_name_size - 1] == 'G'))
            {
            return (true);
            }
        }

    // There are enough letters in the file name to store x.jpeg
    if (file_name_size >= 6)
        {
        // The extension is JPEG
        if (aFileName[file_name_size - 5] == '.' &&
                (aFileName[file_name_size - 4] == 'j' ||
                        aFileName[file_name_size - 4] == 'J') &&
                (aFileName[file_name_size - 3] == 'p' ||
                        aFileName[file_name_size - 3] == 'P') &&
                (aFileName[file_name_size - 2] == 'e' ||
                        aFileName[file_name_size - 2] == 'E') &&
                (aFileName[file_name_size - 1] == 'g' ||
                        aFileName[file_name_size - 1] == 'G'))
            {
            return (true);
            }
        }

    return (false);
}


//--------------------------------
bool isTIFF(const char* aFileName)
//--------------------------------
{
    unsigned int file_name_size(std::strlen(aFileName));

    // There are enough letters in the file name to store x.tif
    if (file_name_size >= 5)
        {
        // The extension is TIF
        if (aFileName[file_name_size - 4] == '.' &&
                (aFileName[file_name_size - 3] == 't' ||
                        aFileName[file_name_size - 3] == 'T') &&
                (aFileName[file_name_size - 2] == 'i' ||
                        aFileName[file_name_size - 2] == 'I') &&
                (aFileName[file_name_size - 1] == 'f' ||
                        aFileName[file_name_size - 1] == 'F'))
            {
            return (true);
            }
        }

    // There are enough letters in the file name to store x.tiff
    if (file_name_size >= 6)
        {
        // The extension is TIFF
        if (aFileName[file_name_size - 5] == '.' &&
                (aFileName[file_name_size - 4] == 't' ||
                        aFileName[file_name_size - 4] == 'T') &&
                (aFileName[file_name_size - 3] == 'i' ||
                        aFileName[file_name_size - 3] == 'I') &&
                (aFileName[file_name_size - 2] == 'f' ||
                        aFileName[file_name_size - 2] == 'F') &&
                (aFileName[file_name_size - 1] == 'f' ||
                        aFileName[file_name_size - 1] == 'F'))
            {
            return (true);
            }
        }

    return (false);
}


};
