
#ifndef __SGVXR_ImplicitSurface_h
#define __SGVXR_ImplicitSurface_h

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
    );

#endif