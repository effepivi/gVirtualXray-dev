#ifndef __gvxrWrapper_h
#define __gvxrWrapper_h

void createOpenGLContext(int aWindowID = -1);
void createWindow(int aWindowID = -1, int aVisibilityFlag = 0);
void setWindowSize(int aWidth, int aHeight, int aWindowID = -1);
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

#endif


