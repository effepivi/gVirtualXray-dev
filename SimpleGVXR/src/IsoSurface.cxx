/*

Copyright (c) 2025-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
UK Research and Innovation, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the UK Research and Innovation nor the names of its contributors
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
*   @file       IsoSurface.cxx
*
*   @brief      Expose here all the functions of SimpleGVXR related to 
*               iso-surfacing.
*
*   @version    1.0
*
*   @date       21/07/2025
*
*   @author     Prof Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, July 2025-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef __OutOfBoundsException_h
#include "gVirtualXRay/OutOfBoundsException.h"
#endif

#ifndef __Image_h
#include "gVirtualXRay/Image.h"
#endif

#ifndef __Types_h
#include "gVirtualXRay/Types.h"
#endif

#ifndef __SceneGraphBinder_h
#include "gVirtualXRay/SceneGraphBinder.h"
#endif

#ifndef __SceneGraphNode_h
#include "gVirtualXRay/SceneGraphNode.h"
#endif

#ifndef __PolygonMesh_h
#include "gVirtualXRay/PolygonMesh.h"
#endif

#ifndef __SGVXR_Internal_h
#include "internal.h"
#endif

#ifndef __SGVXR_Utils_h
#include "Utils.h"
#endif

#ifndef __SGVXR_IsoSurface_h
#include "IsoSurface.h"
#endif


//******************************************************************************
//  Namespace
//******************************************************************************


//******************************************************************************
//  Global variables
//******************************************************************************
extern std::shared_ptr<gVirtualXRay::SceneGraphBinder> g_p_scene_graph;


//******************************************************************************
//  Function definitions
//******************************************************************************


//------------------------------------------------------------------------------
void makeIsoSurface(const std::string& aMeshLabel,
                    const std::vector<std::vector<std::vector<float>>> apVolume,
                    float anIsoValue,
                    float aCentrePositionX,
                    float aCentrePositionY,
                    float aCentrePositionZ,
                    float aVoxelWidth,
                    float aVoxelHeight,
                    float aVoxelDepth,
                    const std::string& aUnitOfLength,
                    const std::string& aParent,
                    unsigned char aVerboseLevel)
//------------------------------------------------------------------------------
{
    float unit_of_length = getUnitOfLength(aUnitOfLength);
    float scaling = getUnitOfLength(aUnitOfLength) / getUnitOfLength("mm");

    unsigned int number_of_slices = apVolume.size();

    if (number_of_slices == 0)
    {
        gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    unsigned int number_of_rows = apVolume[0].size();

    if (number_of_rows == 0)
    {
        gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    unsigned int number_of_cols = apVolume[0][0].size();

    if (number_of_cols == 0)
    {
        gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    // Create the image
    gVirtualXRay::Image<float> temp(getImageVector3D(apVolume));

    temp.setSpacing(gVirtualXRay::VEC3(aVoxelWidth, aVoxelHeight, aVoxelDepth) * unit_of_length);

    // Create the mesh
    gVirtualXRay::PolygonMesh mesh_data = temp.marchingCubes(anIsoValue, 0, false, aVerboseLevel);
    
    // Translate the mesh
    //  minus half the size of the volume plus half the size of a voxel
    gVirtualXRay::VEC3 translation_vector(
        (-aVoxelWidth * (number_of_cols + 2) / 2.0 + aVoxelWidth / 2.0) * scaling,
        (-aVoxelHeight * (number_of_rows + 2) / 2.0 + aVoxelHeight / 2.0) * scaling,
        (-aVoxelDepth * (number_of_slices + 2) / 2.0 + aVoxelDepth / 2.0) * scaling
    );

    mesh_data.applyTranslation(translation_vector);

    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aMeshLabel);

    // The node is already in the scene graph and replace the mesh
    if (p_node)
    {
        p_node->setGeometry(mesh_data);
    }
    // Create the node in the scene graph and add the mesh
    else
    {
        g_p_scene_graph->addPolygonMesh(
            aMeshLabel,
            mesh_data,
            gVirtualXRay::VEC3(),
            aParent);
    }
}


//------------------------------------------------------------------------------
void makeIsoSurface(const std::string& aMeshLabel,
                    const std::vector<std::vector<std::vector<float>>> apVolume,
                    float aLowerIsoValue,
                    float anUpperIsoValue,
                    float aCentrePositionX,
                    float aCentrePositionY,
                    float aCentrePositionZ,
                    float aVoxelWidth,
                    float aVoxelHeight,
                    float aVoxelDepth,
                    const std::string& aUnitOfLength,
                    const std::string& aParent,
                    unsigned char aVerboseLevel)
//------------------------------------------------------------------------------
{
    float unit_of_length = getUnitOfLength(aUnitOfLength);
    float scaling = getUnitOfLength(aUnitOfLength) / getUnitOfLength("mm");

    unsigned int number_of_slices = apVolume.size();

    if (number_of_slices == 0)
    {
        gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    unsigned int number_of_rows = apVolume[0].size();

    if (number_of_rows == 0)
    {
        gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    unsigned int number_of_cols = apVolume[0][0].size();

    if (number_of_cols == 0)
    {
        gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    // Create the image
    gVirtualXRay::Image<float> temp(getImageVector3D(apVolume));

    temp.setSpacing(gVirtualXRay::VEC3(aVoxelWidth, aVoxelHeight, aVoxelDepth) * unit_of_length);

    // Create the mesh
    gVirtualXRay::PolygonMesh mesh_data = temp.marchingCubes(aLowerIsoValue, anUpperIsoValue, true, aVerboseLevel);
    
    // Translate the mesh
    //  minus half the size of the volume plus half the size of a voxel
    gVirtualXRay::VEC3 translation_vector(
        (-aVoxelWidth * (number_of_cols + 2) / 2.0 + aVoxelWidth / 2.0) * scaling,
        (-aVoxelHeight * (number_of_rows + 2) / 2.0 + aVoxelHeight / 2.0) * scaling,
        (-aVoxelDepth * (number_of_slices + 2) / 2.0 + aVoxelDepth / 2.0) * scaling
    );

    mesh_data.applyTranslation(translation_vector);

    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aMeshLabel);

    // The node is already in the scene graph and replace the mesh
    if (p_node)
    {
        p_node->setGeometry(mesh_data);
    }
    // Create the node in the scene graph and add the mesh
    else
    {
        g_p_scene_graph->addPolygonMesh(
            aMeshLabel,
            mesh_data,
            gVirtualXRay::VEC3(),
            aParent);
    }
}


// //---------------------------------------------------
// void makeIsoSurface(const std::string& aMeshLabel,
//                     float* apVolume,
//                     unsigned int aNumberOfVoxelX,
//                     unsigned int aNumberOfVoxelY,
//                     unsigned int aNumberOfVoxelZ,
//                     float anIsoValue,
//                     float aCentrePositionX,
//                     float aCentrePositionY,
//                     float aCentrePositionZ,
//                     float aVoxelWidth,
//                     float aVoxelHeight,
//                     float aVoxelDepth,
//                     const std::string& aUnitOfLength,
//                     const std::string& aParent,
//                     unsigned char aVerboseLevel)
// //---------------------------------------------------
// {}


// //---------------------------------------------------
// void makeIsoSurface(const std::string& aMeshLabel,
//                     float* apVolume,
//                     unsigned int aNumberOfVoxelX,
//                     unsigned int aNumberOfVoxelY,
//                     unsigned int aNumberOfVoxelZ,
//                     float aLowerIsoValue,
//                     float anUpperIsoValue,
//                     float aCentrePositionX,
//                     float aCentrePositionY,
//                     float aCentrePositionZ,
//                     float aVoxelWidth,
//                     float aVoxelHeight,
//                     float aVoxelDepth,
//                     const std::string& aUnitOfLength,
//                     const std::string& aParent,
//                     unsigned char aVerboseLevel)
// //---------------------------------------------------
// {}
