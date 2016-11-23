/*

Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*	@file		gvxrMonoEnergy.cxx
*
*	@brief		Command line tool to convert a series of 2D slices into
*               a 3D volume.
*
*	@version	1.0
*
*	@date		23/11/2016
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
*				http://www.fpvidal.net/, Nov 2016, 2016, version 1.1,
*				BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <string>

#ifdef HAS_ITK
#include <itkMacro.h>
#endif

/*
#include <stdlib.h>
#include <ctime>
#include <algorithm>
*/
#include "cmdline.h"
/*
#ifdef HAS_BUNDLE
#include <boost/filesystem.hpp>
#endif

#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>

#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#ifndef GVXR_POLYGON_MESH_H
#include "gVirtualXRay/PolygonMesh.h"
#endif

#ifndef GVXR_CUBE_MESH_H
#include "gVirtualXRay/CubeMesh.h"
#endif

#ifndef GVXR_XRAY_BEAM_H
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef GVXR_XRAY_DETECTOR_H
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef GVXR_MATRIX4X4_H
#include "gVirtualXRay/Matrix4x4.h"
#endif

#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef GVXR_SHADER_H
#include "gVirtualXRay/Shader.h"
#endif

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif*/

#ifndef __Image_h
#include "gVirtualXRay/Image.h"
#endif

#ifndef __Exception_h
#include "gVirtualXRay/Exception.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//	Global variables
//******************************************************************************


//******************************************************************************
//	Function declaration
//******************************************************************************


//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
    try
        {
        std::string pattern;
        int first_slice;
        int last_slice;
        std::string output_file_name;
        
        // Read the command line arguments
        gengetopt_args_info ai;
        if (cmdline_parser (argc, argv, &ai) != 0)
        {
	    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: cannot read the command line arguments.");
        }

        if (ai.pattern_given)
            {
            pattern = ai.pattern_arg;
            }
        
        if (ai.first_slice_given)
            {
            first_slice = ai.first_slice_arg;
            }
        
        if (ai.last_slice_given)
            {
            last_slice = ai.last_slice_arg;
            }

        if (ai.output_given)
            {
            output_file_name = ai.output_arg;
            }
        
        // Create an image
        Image<float> temp_image;
        
        // Load the series
        temp_image.loadSeries(pattern, first_slice, last_slice, 1);
        
        // Save the volume into a file
        temp_image.save(output_file_name);
        }
	// Catch exception if any
#ifdef HAS_ITK
    catch( itk::ExceptionObject & err )
        {
        std::cerr << err << std::endl;
        }
#endif
	catch (const std::exception& error)
        {
		std::cerr << error.what() << std::endl;
        }

	// Return an exit code
	return (EXIT_SUCCESS);
}
