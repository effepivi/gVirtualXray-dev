#ifndef __gvxrWrapper_h
#define __gvxrWrapper_h

#include <vector>

// For polygon meshes
#ifndef __PolygonMesh_h
#include "gVirtualXRay/PolygonMesh.h"
#endif


/*
class PyPolygonMesh//: public boost::python::object
{
public:
	PyPolygonMesh();
	PyPolygonMesh(const PolygonMesh& aMesh);


	//void setMaterial(const boost::python::list& aMaterial);

	void setDensity(double aDensity, const char* aUnitOfDensity);
	double getDensity();

	double getMassAttenuationCoefficient(double anEnergy, const char* aUnitOfEnergy) const;
	double getLinearAttenuationCoefficient(double anEnergy, const char* aUnitOfEnergy) const;

	void setLinearAttenuationCoefficient(double aMu, const char* aUnit);

	const void* getMesh() const;
	void* getMesh();

	void translate(double x, double y, double z, const char* aUnit);
	void scale(double x, double y, double z);
	void rotate(double x, double y, double z, double aRotationAngleInDegrees);

	void saveSTLFile(const std::string& aFileName);

	/*PyPolygonMesh& operator=(const PyPolygonMesh& aMesh);

	PyPolygonMesh operator+(const PyPolygonMesh& aMesh) const;
	PyPolygonMesh operator-(const PyPolygonMesh& aMesh) const;

	PyPolygonMesh& operator+=(const PyPolygonMesh& aMesh);
	PyPolygonMesh& operator-=(const PyPolygonMesh& aMesh);

private:
	PolygonMesh* m_p_polygon_mesh;
};
*/
/*
    // Expose C++ to Python

    boost::python::def("getSceneTransformationMatrix", getSceneTransformationMatrix);
    boost::python::def("getRootTransformationMatrix",  getRootTransformationMatrix);
    boost::python::def("getNodeTransformationMatrix",  getNodeTransformationMatrix);
    boost::python::def("setSceneTransformationMatrix", setSceneTransformationMatrix);
    boost::python::def("setRootTransformationMatrix",  setRootTransformationMatrix);
    boost::python::def("setNodeTransformationMatrix",  setNodeTransformationMatrix);

    boost::python::def("scaleNode",                 scaleNode);
    boost::python::def("rotateNode",                rotateNode);
    boost::python::def("translateNode",             translateNode);
    boost::python::def("resetSceneTransformation", resetSceneTransformation);

    boost::python::def("scaleScene",                scaleScene);
    boost::python::def("rotateScene",               rotateScene);
    boost::python::def("translateScene",            translateScene);

    boost::python::def("getMeshLabelSet",           getMeshLabelSet);

    boost::python::def("makeCube",     makeCube,     makeCube_overloads());
    boost::python::def("makeCylinder", makeCylinder, makeCylinder_overloads());

    boost::python::def("displayScene", displayScene, displayScene_overloads());
    boost::python::def("renderLoop",   renderLoop,   renderLoop_overloads());


    boost::python::def("emptyMesh",                     emptyMesh);

*/


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
void addPolygonMeshAsInnerSurface(const gVirtualXRay::PolygonMesh& aMesh, const std::string& aLabel);
void addPolygonMeshAsOuterSurface(const gVirtualXRay::PolygonMesh& aMesh, const std::string& aLabel);
void removePolygonMeshes();
void moveToCentre(const char* aLabel = 0);
void moveToCenter(const char* aLabel = 0);
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
void saveLastLBuffer(const char* aFileName = 0);
void saveLastSinogram(const char* aFileName = 0);
void enableArtefactFilteringOnGPU(void);
void enableArtifactFilteringOnGPU(void);
void enableArtefactFilteringOnCPU(void);
void enableArtifactFilteringOnCPU(void);
void disableArtefactFiltering(void);
void disableArtifactFiltering(void);

std::vector<std::vector<float> > computeXRayImage();
std::vector<std::vector<float> > computeLBuffer(const char* aLabel);
std::vector<std::vector<float> > computeSinogram(double x, double y, double z, const char* aUnitOfLength,
        unsigned int aNumberOfAngle,
        double anAngleOffset);

#endif


