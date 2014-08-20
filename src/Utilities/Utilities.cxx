/**
********************************************************************************
*
*	@file		Utilities.cxx
*
*	@brief		Some utility functions that do not fit anywhere else.
*
*	@version	1.2
*
*	@date		12/02/2014
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


#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif


//******************************************************************************
//	Include
//******************************************************************************
#include <zlib.h>
#include <string.h>


//------------------------------------------------------------------
int gVirtualXRay::inflate(const void *src, int srcLen, char** apDst)
//------------------------------------------------------------------
{
	// See http://stackoverflow.com/questions/4901842/in-memory-decompression-with-zlib
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

    err = inflateInit2(&strm, (15 + 32)); //15 window bits, and the +32 tells zlib to to detect if using gzip or zlib
    if (err == Z_OK) {
        err = inflate(&strm, Z_FINISH);
        if (err == Z_STREAM_END) {
            ret = strm.total_out;
        }
        else {
             inflateEnd(&strm);
             delete [] *apDst;
             *apDst = 0;
             return (err);
        }
    }
    else {
        inflateEnd(&strm);
        delete [] *apDst;
        *apDst = 0;
        return (err);
    }

    inflateEnd(&strm);
    return (ret);
}
