//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

// #include <iostream>
// #include <iomanip>
//
// #include <fstream>
// #include <vector>
// #include <cmath>
// #include <numeric>
#include <memory>
//
#include "gtest/gtest.h"


#ifndef GVXR_IMAGE_H
#include "gVirtualXRay/Image.h"
#endif

#ifndef GVXR_SCENEGRAPH_NODE_H
#include "gVirtualXRay/SceneGraphNode.h"
#endif

#ifndef GVXR_CYLINDER_MESH_H
#include "gVirtualXRay/CylinderMesh.h"
#endif

#ifndef GVXR_CUBE_MESH_H
#include "gVirtualXRay/CubeMesh.h"
#endif

/*
#ifndef GVXR_INVALID_INTERNAL_FORMAT_EXCEPTION_H
#include "gVirtualXRay/InvalidInternalFormalException.h"
#endif

#ifndef GVXR_PHOTON_CROSS_SECTION_H
#include "gVirtualXRay/PhotonCrossSection.h"
#endif

#ifndef GVXR_INVALID_IMAGE_SIZE_EXCEPTION_H
#include "gVirtualXRay/InvalidImageSizeException.h"
#endif

#ifndef GVXR_NO_DETECTOR_EXCEPTION_H
#include "gVirtualXRay/NoDetectorException.h"
#endif

#ifndef GVXR_NO_BEAM_EXCEPTION_H
#include "gVirtualXRay/NoBeamException.h"
#endif
*/
/*
#ifndef GVXR_utils_h
#include "utils.h"
#endif
*/



//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Class declaration
//******************************************************************************


//******************************************************************************
//  Ray / AABB intersection tests on a cube
//  The cube is centred on the origin of its node, with an edge of 5 cm
//  (i.e. its AABB is [-2.5 cm, 2.5 cm]^3 in local coordinates).
//  rayIntersectAABB returns at most one hit (the entry point):
//      key:   distance along the ray (in the local system)
//      value: hit position (in the local system)
//******************************************************************************
class TestAABBCube : public ::testing::Test
{
protected:
    static constexpr double EPS = 1e-3;

    void SetUp() override
    {
        m_p_root = std::make_unique<SceneGraphNode>("root", nullptr);
        m_p_root->addChild("cube", CubeMesh<float>(5.0 * cm));
    }

    SceneGraphNode& cube() { return m_p_root->getChild("cube"); }

    std::unique_ptr<SceneGraphNode> m_p_root;
};


//******************************************************************************
//  Ray / AABB intersection tests on a cylinder
//  The cylinder is centred on the origin of its node. Its axis is along Y.
//  Its height is 6 cm and its radius 2 cm, i.e. its AABB is
//  [-2 cm, 2 cm] x [-3 cm, 3 cm] x [-2 cm, 2 cm] in local coordinates.
//  Note that rayIntersectAABB tests the bounding box, not the cylinder itself.
//******************************************************************************
class TestAABBCylinder : public ::testing::Test
{
protected:
    static constexpr double EPS = 1e-3;

    void SetUp() override
    {
        m_p_root = std::make_unique<SceneGraphNode>("root", nullptr);
        m_p_root->addChild("cylinder", CylinderMesh<float>(20, 6.0 * cm, 2.0 * cm));
    }

    SceneGraphNode& cylinder() { return m_p_root->getChild("cylinder"); }

    std::unique_ptr<SceneGraphNode> m_p_root;
};


//******************************************************************************
//  Function declaration
//******************************************************************************


//----------------------------------
TEST_F(TestAABBCube, HitFromOutside)
//----------------------------------
{
    auto hits = cube().rayIntersectAABB(VEC3(0.0, -10.0 * cm, 0.0), VEC3(0.0, 1.0, 0.0));

    ASSERT_EQ(hits.size(), 1u);
    EXPECT_NEAR(hits.begin()->first, 7.5 * cm, EPS);        // 10 cm - 2.5 cm
    EXPECT_NEAR(hits.begin()->second[0], 0.0, EPS);
    EXPECT_NEAR(hits.begin()->second[1], -2.5 * cm, EPS);   // On the -Y face
    EXPECT_NEAR(hits.begin()->second[2], 0.0, EPS);
}


//---------------------------------
TEST_F(TestAABBCube, MissBesideBox)
//---------------------------------
{
    auto hits = cube().rayIntersectAABB(VEC3(5.0 * cm, -10.0 * cm, 0.0), VEC3(0.0, 1.0, 0.0));
    EXPECT_TRUE(hits.empty());
}


//--------------------------------
TEST_F(TestAABBCube, BoxBehindRay)
//--------------------------------
{
    auto hits = cube().rayIntersectAABB(VEC3(0.0, -10.0 * cm, 0.0), VEC3(0.0, -1.0, 0.0));
    EXPECT_TRUE(hits.empty());
}


//--------------------------------
TEST_F(TestAABBCube, OriginInside)
//--------------------------------
{
    auto hits = cube().rayIntersectAABB(VEC3(0.0, 0.0, 0.0), VEC3(0.0, 1.0, 0.0));

    ASSERT_EQ(hits.size(), 1u);
    EXPECT_NEAR(hits.begin()->first, 0.0, EPS);
}


//---------------------------------------------------
TEST_F(TestAABBCube, AxisParallelRayInsideFaceBounds)
//---------------------------------------------------
{
    // The ray direction has two null components. It is just inside the box.
    auto hits = cube().rayIntersectAABB(VEC3(2.4 * cm, -10.0 * cm, 2.4 * cm), VEC3(0.0, 1.0, 0.0));
    ASSERT_EQ(hits.size(), 1u);
    EXPECT_NEAR(hits.begin()->first, 7.5 * cm, EPS);

    // Just outside the box
    hits = cube().rayIntersectAABB(VEC3(2.6 * cm, -10.0 * cm, 0.0), VEC3(0.0, 1.0, 0.0));
    EXPECT_TRUE(hits.empty());
}


//-------------------------------
TEST_F(TestAABBCube, DiagonalRay)
//-------------------------------
{
    VEC3 direction(1.0, 1.0, 0.0);
    direction.normalise();

    // Aimed at the centre: it enters through the corner edge region at
    // (-2.5, -2.5) cm, i.e. at a distance of (10 - 2.5) * sqrt(2) cm
    auto hits = cube().rayIntersectAABB(VEC3(-10.0 * cm, -10.0 * cm, 0.0), direction);

    ASSERT_EQ(hits.size(), 1u);
    EXPECT_NEAR(hits.begin()->first, 7.5 * std::sqrt(2.0) * cm, EPS);
    EXPECT_NEAR(hits.begin()->second[0], -2.5 * cm, EPS);
    EXPECT_NEAR(hits.begin()->second[1], -2.5 * cm, EPS);
}


//----------------------------------
TEST_F(TestAABBCube, TranslatedNode)
//----------------------------------
{
    cube().setLocalTransformation(MATRIX4::buildTranslationMatrix(10.0 * cm, 0.0, 0.0));

    // Aimed at the translated cube
    auto hits = cube().rayIntersectAABB(VEC3(10.0 * cm, -10.0 * cm, 0.0), VEC3(0.0, 1.0, 0.0));
    ASSERT_EQ(hits.size(), 1u);
    EXPECT_NEAR(hits.begin()->first, 7.5 * cm, EPS);

    // Aimed at where the cube was before the translation
    hits = cube().rayIntersectAABB(VEC3(0.0, -10.0 * cm, 0.0), VEC3(0.0, 1.0, 0.0));
    EXPECT_TRUE(hits.empty());
}


//-------------------------------------------
TEST_F(TestAABBCube, Rotated90DegreesAroundZ)
//-------------------------------------------
{
    m_p_root->setLocalTransformation(MATRIX4::buildRotationMatrix(90.0, 0.0, 0.0, 1.0));

    // A cube is invariant by a 90 degree rotation
    auto hits = cube().rayIntersectAABB(VEC3(-10.0 * cm, 0.0, 0.0), VEC3(1.0, 0.0, 0.0));
    ASSERT_EQ(hits.size(), 1u);
    EXPECT_NEAR(hits.begin()->first, 7.5 * cm, EPS);
}


//-------------------------------------------
TEST_F(TestAABBCube, Rotated45DegreesAroundZ)
//-------------------------------------------
{
    m_p_root->setLocalTransformation(MATRIX4::buildRotationMatrix(45.0, 0.0, 0.0, 1.0));

    // The cube is now a diamond in the XY plane: |x| + |y| <= 2.5 * sqrt(2) cm.
    // At y = 3 cm, the diamond extends to x = -(2.5 * sqrt(2) - 3) cm.
    const double expected_x = -(2.5 * std::sqrt(2.0) - 3.0) * cm;

    auto hits = cube().rayIntersectAABB(VEC3(-10.0 * cm, 3.0 * cm, 0.0), VEC3(1.0, 0.0, 0.0));
    ASSERT_EQ(hits.size(), 1u);
    EXPECT_NEAR(hits.begin()->first, 10.0 * cm + expected_x, EPS);   // Distance from x = -10 cm

    // Outside the diamond (it would have hit the unrotated cube's bounding corner)
    hits = cube().rayIntersectAABB(VEC3(-10.0 * cm, 3.6 * cm, 0.0), VEC3(1.0, 0.0, 0.0));
    EXPECT_TRUE(hits.empty());
}


//-------------------------------------------
TEST_F(TestAABBCube, TranslationThenRotation)
//-------------------------------------------
{
    // With column vectors, "translate then rotate" is R * T:
    // the cube is moved to (10, 0, 0) cm, then the whole thing is rotated by
    // 90 degrees around Z. Its centre ends up at (0, 10, 0) cm.
    const MATRIX4 translation = MATRIX4::buildTranslationMatrix(10.0 * cm, 0.0, 0.0);
    const MATRIX4 rotation = MATRIX4::buildRotationMatrix(90.0, 0.0, 0.0, 1.0);
    cube().setLocalTransformation(rotation * translation);

    // Aimed at the cube's final position, from above: it hits the +Y face
    // (y = 12.5 cm) after 20 - 12.5 = 7.5 cm
    auto hits = cube().rayIntersectAABB(VEC3(0.0, 20.0 * cm, 0.0), VEC3(0.0, -1.0, 0.0));
    ASSERT_EQ(hits.size(), 1u);
    EXPECT_NEAR(hits.begin()->first, 7.5 * cm, EPS);

    // In the local system, the hit is on the +X face
    EXPECT_NEAR(hits.begin()->second[0], 2.5 * cm, EPS);
    EXPECT_NEAR(hits.begin()->second[1], 0.0, EPS);
    EXPECT_NEAR(hits.begin()->second[2], 0.0, EPS);

    // Aimed at where the cube would be if the order was reversed: (10, 0, 0) cm
    hits = cube().rayIntersectAABB(VEC3(10.0 * cm, 20.0 * cm, 0.0), VEC3(0.0, -1.0, 0.0));
    EXPECT_TRUE(hits.empty());
}


//-------------------------------------------
TEST_F(TestAABBCube, RotationThenTranslation)
//-------------------------------------------
{
    // With column vectors, "rotate then translate" is T * R:
    // the cube is rotated by 90 degrees around Z on itself, then moved to
    // (10, 0, 0) cm.
    const MATRIX4 translation = MATRIX4::buildTranslationMatrix(10.0 * cm, 0.0, 0.0);
    const MATRIX4 rotation = MATRIX4::buildRotationMatrix(90.0, 0.0, 0.0, 1.0);
    cube().setLocalTransformation(translation * rotation);

    // Aimed at the cube's final position, from below: it hits the -Y face
    // (y = -2.5 cm) after 20 - 2.5 = 17.5 cm
    auto hits = cube().rayIntersectAABB(VEC3(10.0 * cm, -20.0 * cm, 0.0), VEC3(0.0, 1.0, 0.0));
    ASSERT_EQ(hits.size(), 1u);
    EXPECT_NEAR(hits.begin()->first, 17.5 * cm, EPS);

    // In the local system, the hit is on the -X face
    EXPECT_NEAR(hits.begin()->second[0], -2.5 * cm, EPS);
    EXPECT_NEAR(hits.begin()->second[1], 0.0, EPS);
    EXPECT_NEAR(hits.begin()->second[2], 0.0, EPS);

    // Aimed at where the cube would be if the order was reversed: (0, 10, 0) cm
    hits = cube().rayIntersectAABB(VEC3(0.0, -20.0 * cm, 0.0), VEC3(0.0, 1.0, 0.0));
    EXPECT_TRUE(hits.empty());
}


//------------------------------------
TEST_F(TestAABBCylinder, HitAlongAxis)
//------------------------------------
{
    // From below, along the cylinder's axis: it hits the bottom, y = -3 cm
    auto hits = cylinder().rayIntersectAABB(VEC3(0.0, -10.0 * cm, 0.0), VEC3(0.0, 1.0, 0.0));

    ASSERT_EQ(hits.size(), 1u);
    EXPECT_NEAR(hits.begin()->first, 7.0 * cm, EPS);
    EXPECT_NEAR(hits.begin()->second[0], 0.0, EPS);
    EXPECT_NEAR(hits.begin()->second[1], -3.0 * cm, EPS);
    EXPECT_NEAR(hits.begin()->second[2], 0.0, EPS);
}


//----------------------------------------------
TEST_F(TestAABBCylinder, HitPerpendicularToAxis)
//----------------------------------------------
{
    // Along X: it hits the side, x = -2 cm (the radius)
    auto hits = cylinder().rayIntersectAABB(VEC3(-10.0 * cm, 0.0, 0.0), VEC3(1.0, 0.0, 0.0));
    ASSERT_EQ(hits.size(), 1u);
    EXPECT_NEAR(hits.begin()->first, 8.0 * cm, EPS);
    EXPECT_NEAR(hits.begin()->second[0], -2.0 * cm, EPS);

    // Along Z
    hits = cylinder().rayIntersectAABB(VEC3(0.0, 0.0, -10.0 * cm), VEC3(0.0, 0.0, 1.0));
    ASSERT_EQ(hits.size(), 1u);
    EXPECT_NEAR(hits.begin()->first, 8.0 * cm, EPS);
    EXPECT_NEAR(hits.begin()->second[2], -2.0 * cm, EPS);
}


//------------------------------------------
TEST_F(TestAABBCylinder, MissAboveAndBeside)
//------------------------------------------
{
    // Above the cylinder (taller than 3 cm)
    EXPECT_TRUE(cylinder().rayIntersectAABB(VEC3(-10.0 * cm, 3.5 * cm, 0.0), VEC3(1.0, 0.0, 0.0)).empty());

    // Beside the cylinder (wider than the radius)
    EXPECT_TRUE(cylinder().rayIntersectAABB(VEC3(2.5 * cm, -10.0 * cm, 0.0), VEC3(0.0, 1.0, 0.0)).empty());

    // Pointing away
    EXPECT_TRUE(cylinder().rayIntersectAABB(VEC3(0.0, -10.0 * cm, 0.0), VEC3(0.0, -1.0, 0.0)).empty());
}


//------------------------------------
TEST_F(TestAABBCylinder, OriginInside)
//------------------------------------
{
    auto hits = cylinder().rayIntersectAABB(VEC3(0.0, 0.0, 0.0), VEC3(0.0, 1.0, 0.0));

    ASSERT_EQ(hits.size(), 1u);
    EXPECT_NEAR(hits.begin()->first, 0.0, EPS);
}


//---------------------------------------------------
TEST_F(TestAABBCylinder, BoxCornerOutsideTheCylinder)
//---------------------------------------------------
{
    // (1.9, 1.9) cm is in the AABB's footprint, but outside the circle of
    // radius 2 cm (1.9 * sqrt(2) = 2.69 cm). The AABB test is expected to report a hit.
    auto hits = cylinder().rayIntersectAABB(VEC3(1.9 * cm, -10.0 * cm, 1.9 * cm), VEC3(0.0, 1.0, 0.0));

    ASSERT_EQ(hits.size(), 1u);
    EXPECT_NEAR(hits.begin()->first, 7.0 * cm, EPS);
}


//--------------------------------------
TEST_F(TestAABBCylinder, RotatedAroundX)
//--------------------------------------
{
    // A rotation of 90 degrees around X turns the cylinder's axis from Y to Z.
    // Its extent is now 2 cm along Y and 3 cm along Z.
    cylinder().setLocalTransformation(MATRIX4::buildRotationMatrix(90.0, 1.0, 0.0, 0.0));

    // Along Z (the new axis): it hits at z = -3 cm, instead of -2 cm when not rotated
    auto hits = cylinder().rayIntersectAABB(VEC3(0.0, 0.0, -10.0 * cm), VEC3(0.0, 0.0, 1.0));
    ASSERT_EQ(hits.size(), 1u);
    EXPECT_NEAR(hits.begin()->first, 7.0 * cm, EPS);

    // Along Y: it hits at y = -2 cm, instead of -3 cm when not rotated
    hits = cylinder().rayIntersectAABB(VEC3(0.0, -10.0 * cm, 0.0), VEC3(0.0, 1.0, 0.0));
    ASSERT_EQ(hits.size(), 1u);
    EXPECT_NEAR(hits.begin()->first, 8.0 * cm, EPS);

    // At z = 2.5 cm and y = 0, it is inside the rotated extent (3 cm), but would
    // be outside if the cylinder was not rotated
    hits = cylinder().rayIntersectAABB(VEC3(-10.0 * cm, 0.0, 2.5 * cm), VEC3(1.0, 0.0, 0.0));
    EXPECT_EQ(hits.size(), 1u);
}


//---------------------------------------------
TEST_F(TestAABBCylinder, TranslatedThenRotated)
//---------------------------------------------
{
    // Rotate by 90 degrees around X (axis along Z), then move by 10 cm along Z.
    // With column vectors, it is T * R. The cylinder now spans z in [7, 13] cm.
    const MATRIX4 translation = MATRIX4::buildTranslationMatrix(0.0, 0.0, 10.0 * cm);
    const MATRIX4 rotation = MATRIX4::buildRotationMatrix(90.0, 1.0, 0.0, 0.0);
    cylinder().setLocalTransformation(translation * rotation);

    auto hits = cylinder().rayIntersectAABB(VEC3(0.0, 0.0, -10.0 * cm), VEC3(0.0, 0.0, 1.0));
    ASSERT_EQ(hits.size(), 1u);
    EXPECT_NEAR(hits.begin()->first, 17.0 * cm, EPS);   // 7 - (-10)

    // The cylinder is no longer at the origin
    EXPECT_TRUE(cylinder().rayIntersectAABB(VEC3(0.0, -10.0 * cm, 0.0), VEC3(0.0, 1.0, 0.0)).empty());
}
