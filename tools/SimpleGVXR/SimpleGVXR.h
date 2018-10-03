#ifndef __SimpleGVXR_h
#define __SimpleGVXR_h

#include <vector>
#include <string>

// For polygon meshes
#ifndef __PolygonMesh_h
#include "gVirtualXRay/PolygonMesh.h"
#endif


void setSourcePosition(double x, double y, double z, const char* aUnitOfLength);
void setDetectorPosition(double x, double y, double z, const char* aUnitOfLength);
void setDetectorUpVector(double x, double y, double z);
void setDetectorNumberOfPixels(int aWidth, int aHeight);
void setDetectorPixelSize(double aWidth,
						  double aHeight,
						  const char* aUnitOfLength);

void loadMeshFile(const char* aLabel,
                  const char* aFileName,
                  const char* aUnitOfLength);
void loadSceneGraph(const char* aFileName,
        			const char* aUnitOfLength);

void emptyMesh(const std::string& aLabel);
void makeCube(const std::string& aLabel, double aLength = 1.0, const char* aUnitOfLength = "cm");
void makeCylinder(const std::string& aLabel, unsigned int aNumberOfSectors = 10,
        double aHeight = 1.0,
        double aRadius = 0.5,
		const char* aUnitOfLength = "cm");
void addMesh(const std::string& aDestination, const std::string& aSource);
void subtractMesh(const std::string& aDestination, const std::string& aSource);

std::vector<std::string> getMeshLabelSet();
unsigned int getMeshLabelSetSize();
std::string getMeshLabel(unsigned int i);

void addPolygonMeshAsInnerSurface(const std::string& aLabel);
void addPolygonMeshAsOuterSurface(const std::string& aLabel);
void removePolygonMeshesFromSceneGraph();
void removePolygonMeshesFromXRayRenderer();

void moveToCentre(const char* aLabel = 0);
void moveToCenter(const char* aLabel = 0);

void scaleNode(const char* aLabel, double x, double y, double z, const char* aUnitOfLength);
void rotateNode(const char* aLabel, double angle, double x, double y, double z);
void translateNode(const char* aLabel, double x, double y, double z, const char* aUnitOfLength);

void resetSceneTransformation();
void scaleScene(double x, double y, double z, const char* aUnitOfLength);
void rotateScene(double angle, double x, double y, double z);
void translateScene(double x, double y, double z, const char* aUnitOfLength);

std::vector<std::vector<float> > getSceneTransformationMatrix();
std::vector<std::vector<float> > getRootTransformationMatrix();
std::vector<std::vector<float> > getNodeTransformationMatrix(const char* aLabel);

void setSceneTransformationMatrix(const std::vector<std::vector<float> >& aMatrix);
void setRootTransformationMatrix(const std::vector<std::vector<float> >& aMatrix);
void setNodeTransformationMatrix(const char* aLabel, const std::vector<std::vector<float> >& aMatrix);



void setHU(const char* aLabel, short HU);
void setHounsfieldUnit(const char* aLabel, short HU);
void setHounsfieldValue(const char* aLabel, short HU);

void setElement(const char* aLabel, unsigned short Z);
void setElement(const char* aLabel, const std::string& aName);

void setMixture(const char* aLabel, const std::string& aName);

void setCompound(const char* aLabel, const std::string& aName);

void setMassAttenuationCoefficient(const char* aLabel, double aCoefficient,
		   const char* aUnit);
void setLinearAttenuationCoefficient(const char* aLabel, double aCoefficient,
		   const char* aUnit);
void setMu(const char* aLabel, double aCoefficient,
		   const char* aUnit);
void setDensity(const char* aLabel, double aDensity,
		   const char* aUnit);

void createOpenGLContext(int aWindowID = -1);
void createWindow(int aWindowID = -1, int aVisibilityFlag = 0);
void setWindowSize(int aWidth, int aHeight, int aWindowID = -1);
void displayScene(int aWindowID = -1);
void renderLoop(int aWindowID = -1/*, PyObject* aCallBack*/);
void showWindow(int aWindowID = -1);
void hideWindow(int aWindowID = -1);
void destroyWindow(int aWindowID = -1);
void destroyAllWindows(void);
void usePointSource();
void useParallelBeam();
void useParallelSource();
void resetBeamSpectrum();
void setMonoChromatic(double anEnergy,
		              const char* aUnitOfEnergy,
					  double aNumberOfPhotons);
void addEnergyBinToSpectrum(double anEnergy,
							const char* aUnitOfEnergy,
							double aNumberOfPhotons);
void saveLastXRayImage(const char* aFileName = 0);
void saveLastLBuffer(const char* aFileName = 0);
void saveLastSinogram(const char* aFileName = 0);
void enableArtefactFilteringOnGPU(void);
void enableArtifactFilteringOnGPU(void);
void enableArtefactFilteringOnCPU(void);
void enableArtifactFilteringOnCPU(void);
void disableArtefactFiltering(void);
void disableArtifactFiltering(void);

double getUnitOfEnergy(const char* aUnitOfEnergy);
double getUnitOfLength(const char* aUnitOfLength);

std::vector<std::vector<float> > computeXRayImage();
std::vector<std::vector<float> > computeLBuffer(const char* aLabel);
std::vector<std::vector<float> > computeSinogram(double x, double y, double z, const char* aUnitOfLength,
        unsigned int aNumberOfAngle,
        double anAngleOffset);

std::vector<std::vector<float> > getLastXRayImage();
std::vector<std::vector<float> > getLastLBuffer();
std::vector<std::vector<float> > getLastSinogram();

int getImageWidth(const std::vector<std::vector<float> >& anImage);
int getImageHeght(const std::vector<std::vector<float> >& anImage);
std::vector<float> getImageRow(const std::vector<std::vector<float> >& anImage, int j);

#endif


