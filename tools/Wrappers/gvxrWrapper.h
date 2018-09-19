#ifndef __gvxrWrapper_h
#define __gvxrWrapper_h


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
void moveToCentre(const char* aLabel);
void moveToCenter(const char* aLabel);
void setHU(const char* aLabel, double aHU);
void setMassAttenuationCoefficient(const char* aLabel,
                                   double anAttenuationCoefficient);
void setLinearAttenuationCoefficient(const char* aLabel,
                                     double anAttenuationCoefficient);
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
void enableArtefactFilteringOnGPU(void);
void enableArtifactFilteringOnGPU(void);
void enableArtefactFilteringOnCPU(void);
void enableArtifactFilteringOnCPU(void);
void disableArtefactFiltering(void);
void disableArtifactFiltering(void);

void computeXRayImage();
void computeLBuffer(const char* aLabel);
void computeSinogram(double x, double y, double z, const char* aUnitOfLength,
        unsigned int aNumberOfAngle,
        double anAngleOffset);

#endif


