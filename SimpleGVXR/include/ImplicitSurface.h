
#ifndef __SGVXR_ImplicitSurface_h
#define __SGVXR_ImplicitSurface_h


#include <string>
#include <vector>


//------------------------------------------------------------------------------
/// Get the number of implicit surfaces.
/**
*  @return the number of implicit surfaces
*/
//------------------------------------------------------------------------------
unsigned int getNumberOfImplicitSurfaces();


std::vector<std::string> getImplicitSurfaceLabelSet();


bool implicitSurfaceExists(const std::string& aLabel);


void createImplicitSurface(const std::string& aLabel);


void destroyImplicitSurface(const std::string& aLabel);


void destroyAllImplicitSurfaces();


void addControlPoint(const std::string& aLabel,
    float aPositionX,
    float aPositionY,
    float aPositionZ,
    const std::string& aUnitOfLength,
    const std::string& aDentityFunction,
    float aParameter1,
    float aParameter2 = 0.0
);


void addControlLineSegment(const std::string& aLabel,
    float aPosition1X,
    float aPosition1Y,
    float aPosition1Z,
    float aPosition2X,
    float aPosition2Y,
    float aPosition2Z,
    const std::string& aUnitOfLength,
    const std::string& aDentityFunction,
    float aParameter1,
    float aParameter2 = 0.0
);


unsigned int getNumberOfControlPoints(const std::string& aLabel);


unsigned int getNumberOfControlLineSegments(const std::string& aLabel);


void setControlPointParameters(const std::string& aLabel,
    unsigned int aPointID,
    const std::string& aDentityFunction,
    float aParameter1,
    float aParameter2 = 0.0
);
                              

void setControlLineSegmentParameters(const std::string& aLabel,
    unsigned int aLineSegmentID,
    const std::string& aDentityFunction,
    float aParameter1,
    float aParameter2 = 0.0
);


void voxelise(const std::string& aLabel,
    unsigned int aNumberOfVoxelX,
    unsigned int aNumberOfVoxelY,
    unsigned int aNumberOfVoxelZ,
    float aCentrePositionX,
    float aCentrePositionY,
    float aCentrePositionZ,
    float aVoxelWidth,
    float aVoxelHeight,
    float aVoxelDepth,
    const std::string& aUnitOfLength);


void voxelize(const std::string& aLabel,
    unsigned int aNumberOfVoxelX,
    unsigned int aNumberOfVoxelY,
    unsigned int aNumberOfVoxelZ,
    float aCentrePositionX,
    float aCentrePositionY,
    float aCentrePositionZ,
    float aVoxelWidth,
    float aVoxelHeight,
    float aVoxelDepth,
    const std::string& aUnitOfLength);


std::vector<std::vector<std::vector<float>>> getLastVoxelisation(const std::string& aLabel);


std::vector<std::vector<std::vector<float>>> getLastVoxelization(const std::string& aLabel);


//--------------------------------------------------------------------------
/// Apply the marching cubes algorithm to extract the polygon mesh
/**
    *  @param anIsoValue:    the iso value
    */
//--------------------------------------------------------------------------
void createMeshFromImplicitSurface(const std::string& aLabel,
    float anIsoValue,
    const std::string& aParent = "root");



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