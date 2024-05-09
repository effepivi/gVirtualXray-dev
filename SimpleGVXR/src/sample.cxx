#ifndef __ImplicitSurface_h
#include "gVirtualXRay/ImplicitSurface.h"
#endif

#ifndef __Exception_h
#include "gVirtualXRay/Exception.h"
#endif
#ifndef __SimpleGVXR_h
#include "SimpleGVXR.h"
#endif

#ifndef __internal_h
#include "internal.h"
#endif

#ifndef __sample_h
#include "sample.h"
#endif


std::vector<std::vector<std::vector<float>>> computeDensity(unsigned int aNumberOfVoxelX,
    unsigned int aNumberOfVoxelY,
    unsigned int aNumberOfVoxelZ,
    float aCentrePositionX,
    float aCentrePositionY,
    float aCentrePositionZ,
    float aVoxelWidth,
    float aVoxelHeight,
    float aVoxelDepth,
    const std::string& aUnitOfLength,
    const std::vector<float>& aControlPointSet,
    const std::vector<std::string>& aControlPointDensityFunctionTypeSet,
    const std::vector<float>& aControlPointDensityFunctionParameterSet,
    const std::vector<float>& aControlLineSet,
    const std::vector<std::string>& aControlLineDensityFunctionTypeSet,
    const std::vector<float>& aControlLineDensityFunctionParameterSet
    )
{

    // Look for errors
    if (aControlPointSet.size() % 3 != 0)
    {
        throw gVirtualXRay::Exception(__FILE__,
                                      __FUNCTION__,
                                      __LINE__,
                                      "Each control point should have 3 components, x, y and z coordinates. It's not the case right now");
    }

    if (aControlPointSet.size() / 3 != aControlPointDensityFunctionTypeSet.size())
    {
        throw gVirtualXRay::Exception(__FILE__,
                                      __FUNCTION__,
                                      __LINE__,
                                      "There should be the same number of control points as types of density functions. It's not the case right now");
    }

    gVirtualXRay::ImplicitSurface implicit_surface;

    float unit_of_length = getUnitOfLength(aUnitOfLength);

    int parameter_set_i = 0;
    for (int i = 0; i < aControlPointDensityFunctionTypeSet.size(); ++i)
    {
        std::string density_function_type = aControlPointDensityFunctionTypeSet[i];
        std::transform(density_function_type.begin(), density_function_type.end(), density_function_type.begin(), ::toupper);
        gVirtualXRay::VEC3 position(aControlPointSet[i * 3], aControlPointSet[i * 3 + 1], aControlPointSet[i * 3 + 2]);
        position *= unit_of_length;

        if (density_function_type == "SPHERE")
        {
            float parameter = aControlPointDensityFunctionParameterSet[parameter_set_i++];

            implicit_surface.addControlPoint(gVirtualXRay::ImplicitSurface::DensityFunctionParameterType(gVirtualXRay::ImplicitSurface::SPHERE, parameter),
                position);

            std::cout << "Control point: " << position << " SPHERE " << parameter << std::endl;
        }
        else if (density_function_type == "BLOBBY_MOLECULE" || density_function_type == "BLOBBY MOLECULE")
        {
            float a = aControlPointDensityFunctionParameterSet[parameter_set_i++];
            float b = aControlPointDensityFunctionParameterSet[parameter_set_i++];

            implicit_surface.addControlPoint(gVirtualXRay::ImplicitSurface::DensityFunctionParameterType(gVirtualXRay::ImplicitSurface::BLOBBY_MOLECULE, a, b),
                position);
        }
        else if (density_function_type == "META_BALL" || density_function_type == "META BALL" || density_function_type == "METABALL")
        {
            float a = aControlPointDensityFunctionParameterSet[parameter_set_i++];
            float b = aControlPointDensityFunctionParameterSet[parameter_set_i++];

            implicit_surface.addControlPoint(gVirtualXRay::ImplicitSurface::DensityFunctionParameterType(gVirtualXRay::ImplicitSurface::META_BALL, a, b),
                position);

            std::cout << "Control point: " << position << " META_BALL " << a << "   " << b << std::endl;
        }
        else if (density_function_type == "SOFT_OBJECT" || density_function_type == "SOFT OBJECT")
        {
            float a = aControlPointDensityFunctionParameterSet[parameter_set_i++];
            float b = aControlPointDensityFunctionParameterSet[parameter_set_i++];

            implicit_surface.addControlPoint(gVirtualXRay::ImplicitSurface::DensityFunctionParameterType(gVirtualXRay::ImplicitSurface::SOFT_OBJECT, a, b),
                position);
        }
        else
        {
            throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "Make sure the density function type of all the control points are SPHERE, BLOBBY_MOLECULE, META_BALL or SOFT_OBJECT.");
        }
    }

    parameter_set_i = 0;
    for (int i = 0; i < aControlLineDensityFunctionTypeSet.size(); ++i)
    {
        std::string density_function_type = aControlLineDensityFunctionTypeSet[i];
        std::transform(density_function_type.begin(), density_function_type.end(), density_function_type.begin(), ::toupper);
        gVirtualXRay::VEC3 position1(aControlLineSet[i * 6], aControlLineSet[i * 6 + 1], aControlLineSet[i * 6 + 2]);
        gVirtualXRay::VEC3 position2(aControlLineSet[i * 6 + 3], aControlLineSet[i * 6 + 4], aControlLineSet[i * 6 + 5]);
        position1 *= unit_of_length;
        position2 *= unit_of_length;

        if (density_function_type == "SPHERE")
        {
            float parameter = aControlLineDensityFunctionParameterSet[parameter_set_i++];

            implicit_surface.addControlLine(gVirtualXRay::ImplicitSurface::DensityFunctionParameterType(gVirtualXRay::ImplicitSurface::SPHERE, parameter),
                position1, position2);
        }
        else if (density_function_type == "BLOBBY_MOLECULE" || density_function_type == "BLOBBY MOLECULE")
        {
            float a = aControlLineDensityFunctionParameterSet[parameter_set_i++];
            float b = aControlLineDensityFunctionParameterSet[parameter_set_i++];

            implicit_surface.addControlLine(gVirtualXRay::ImplicitSurface::DensityFunctionParameterType(gVirtualXRay::ImplicitSurface::BLOBBY_MOLECULE, a, b),
                position1, position2);
        }
        else if (density_function_type == "META_BALL" || density_function_type == "META BALL" || density_function_type == "METABALL")
        {
            float a = aControlLineDensityFunctionParameterSet[parameter_set_i++];
            float b = aControlLineDensityFunctionParameterSet[parameter_set_i++];

            implicit_surface.addControlLine(gVirtualXRay::ImplicitSurface::DensityFunctionParameterType(gVirtualXRay::ImplicitSurface::META_BALL, a, b),
                position1, position2);
        }
        else if (density_function_type == "SOFT_OBJECT" || density_function_type == "SOFT OBJECT")
        {
            float a = aControlLineDensityFunctionParameterSet[parameter_set_i++];
            float b = aControlLineDensityFunctionParameterSet[parameter_set_i++];

            implicit_surface.addControlLine(gVirtualXRay::ImplicitSurface::DensityFunctionParameterType(gVirtualXRay::ImplicitSurface::SOFT_OBJECT, a, b),
                position1, position2);
        }
        else
        {
            throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "Make sure the density function type of all the control lines are SPHERE, BLOBBY_MOLECULE, META_BALL or SOFT_OBJECT.");
        }
    }
    implicit_surface.voxelise(gVirtualXRay::Vec3ui(aNumberOfVoxelX, aNumberOfVoxelY, aNumberOfVoxelZ),
            gVirtualXRay::VEC3(aCentrePositionX, aCentrePositionY, aCentrePositionZ) * unit_of_length,
            gVirtualXRay::VEC3(aVoxelWidth, aVoxelHeight, aVoxelDepth) * unit_of_length);

    return getVectorImage3D(implicit_surface.getVolume());
}
