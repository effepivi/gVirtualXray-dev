//******************************************************************************
//  Include
//******************************************************************************
#include <iostream>
#include <iomanip>

#include <fstream>
#include <vector>
#include <cmath>
#include <numeric>

#include "gtest/gtest.h"

#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef __Image_h
#include "gVirtualXRay/Image.h"
#endif

#ifndef __SceneGraphNode_h
#include "gVirtualXRay/SceneGraphNode.h"
#endif

#ifndef __CylinderMesh_h
#include "gVirtualXRay/CylinderMesh.h"
#endif

#ifndef __CubeMesh_h
#include "gVirtualXRay/CubeMesh.h"
#endif

/*
#ifndef __InvalidInternalFormalException_h
#include "gVirtualXRay/InvalidInternalFormalException.h"
#endif

#ifndef __PhotonCrossSection_h
#include "gVirtualXRay/PhotonCrossSection.h"
#endif

#ifndef __InvalidImageSizeException_h
#include "gVirtualXRay/InvalidImageSizeException.h"
#endif

#ifndef __NoDetectorException_h
#include "gVirtualXRay/NoDetectorException.h"
#endif

#ifndef __NoBeamException_h
#include "gVirtualXRay/NoBeamException.h"
#endif
*/
/*
#ifndef __utils_h
#include "utils.h"
#endif
*/

//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Function declaration
//******************************************************************************


//------------------------------------
TEST(TestAABB, TestAABB)
//------------------------------------
{
    // Set the geometry
    SceneGraphNode scene_graph("root", NULL);
    scene_graph.addChild("inside", CylinderMesh<float>(20, 3.0 * cm, 1.0 * cm));
    scene_graph.addChild("outside", CubeMesh<float>(5.0 * cm));

    SceneGraphNode& cylinder_node = scene_graph.getChild("inside");
    // SceneGraphNode& cube_node = scene_graph.getChild("outside");

    scene_graph.setLocalTransformation(MATRIX4::buildRotationMatrix(70, 1, 0, 0));
    //cylinder_node.setLocalTransformation(MATRIX4::buildRotationMatrix(-70, 0, 1, 0));
    Image<float> image(100, 100);
    for (int x = 0; x < 100; ++x)
    {
        for (int z = 0; z < 100; ++z)
        {
            std::map<RATIONAL_NUMBER, VEC3> p_intersection_set = cylinder_node.rayIntersectAABB(
                VEC3((-5. + 10. * float(x) / 99.) * cm, -10.0 * cm, (-5. + 10. * float(z) / 99.) * cm),
                VEC3(0.0, 1.0, 0.0));

            for (std::map<RATIONAL_NUMBER, VEC3>::const_iterator ite = p_intersection_set.begin();
                ite != p_intersection_set.end();
                ++ite)
            {
                image(x, z, 0) = ite->first;
                //                std::cout << ite->first << "\t" << ite->second << std::endl;
            }
        }
    }
    image.saveMHD("test.mhd");

    // Image();
    // Image(const Image<T>& anImage);
    // Image(const char* aFileName);
    // Image(const std::string& aFileName);
    // Image(const T* apData,
    //              unsigned int aWidth,
    //              unsigned int aHeight,
    //              unsigned int aNumberOfSlices = 1,
    //              const VEC3& aVoxelSize = VEC3(1, 1, 1));
    // Image(unsigned int aWidth,
    //              unsigned int aHeight,
    //              unsigned int aNumberOfSlices = 1,
    //              T aDefaultValue = 0);
}
