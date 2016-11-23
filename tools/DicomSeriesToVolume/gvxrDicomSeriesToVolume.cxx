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
*	@file		gvxrDicomSeriesToVolume.cxx
*
*	@brief		Command line tool to save a DICOM series of 2D slices into
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
#include <itkImage.h>
#include <itkImageSeriesReader.h>
#include <itkGDCMSeriesFileNames.h>
#endif

#include "cmdline.h"

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


//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
    try
        {
        std::string directory;
        std::string output_file_name;
        
        // Read the command line arguments
        gengetopt_args_info ai;
        if (cmdline_parser (argc, argv, &ai) != 0)
        {
	    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: cannot read the command line arguments.");
        }

        if (ai.directory_given)
            {
            directory = ai.directory_arg;
            }
        
        if (ai.output_given)
            {
            output_file_name = ai.output_arg;
            }
        
        // Set some types
        typedef itk::Image< float, 3 >  ImageType;
        typedef itk::ImageSeriesReader< ImageType > ReaderType;
        typedef itk::GDCMSeriesFileNames InputNamesGeneratorType;

        // Create the file names
        InputNamesGeneratorType::Pointer p_name_generator(InputNamesGeneratorType::New());
        p_name_generator->SetInputDirectory(directory);
        const ReaderType::FileNamesContainer& p_name_set(p_name_generator->GetInputFileNames());
        std::string pixel_type(getPixelType(p_name_set[0]));

        if (pixel_type == "short")
            {
            // Create an image
            Image<short> temp_image;
            
            // Load the series
            temp_image.loadDicomSeries(directory);
            
            // Save the volume into a file
            temp_image.save(output_file_name);
            }
        else if (pixel_type == "unsigned short")
            {
            // Create an image
            Image<unsigned short> temp_image;
            
            // Load the series
            temp_image.loadDicomSeries(directory);
            
            // Save the volume into a file
            temp_image.save(output_file_name);
            }
        else if (pixel_type == "int")
            {
            // Create an image
            Image<int> temp_image;
            
            // Load the series
            temp_image.loadDicomSeries(directory);
            
            // Save the volume into a file
            temp_image.save(output_file_name);
            }
        else if (pixel_type == "unsigned int")
            {
            // Create an image
            Image<unsigned int> temp_image;
            
            // Load the series
            temp_image.loadDicomSeries(directory);
            
            // Save the volume into a file
            temp_image.save(output_file_name);
            }
        else if (pixel_type == "char")
            {
            // Create an image
            Image<char> temp_image;
            
            // Load the series
            temp_image.loadDicomSeries(directory);
            
            // Save the volume into a file
            temp_image.save(output_file_name);
            }
        else if (pixel_type == "unsigned char")
            {
            // Create an image
            Image<unsigned char> temp_image;
            
            // Load the series
            temp_image.loadDicomSeries(directory);
            
            // Save the volume into a file
            temp_image.save(output_file_name);
            }
        else if (pixel_type == "float")
            {
            // Create an image
            Image<float> temp_image;
            
            // Load the series
            temp_image.loadDicomSeries(directory);
            
            // Save the volume into a file
            temp_image.save(output_file_name);
            }
        else if (pixel_type == "double")
            {
            // Create an image
            Image<double> temp_image;
            
            // Load the series
            temp_image.loadDicomSeries(directory);
            
            // Save the volume into a file
            temp_image.save(output_file_name);
            }
        else
            {
	    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: unknown pixel type.");
            }
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
