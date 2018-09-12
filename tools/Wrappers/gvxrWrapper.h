#ifndef __gvxrWrapper_h
#define __gvxrWrapper_h



/*

#ifdef HAS_GLEW
#include <GL/glew.h>
#endif

#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#define GLFW_INCLUDE_GLCOREARB 1

#include <GLFW/glfw3.h>


#include <cstdlib>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <memory>

#include <Python.h>


// To perform the computations of the X-ray simulation
#ifndef __XRayRenderer_h
#include "gVirtualXRay/XRayRenderer.h"
#endif

// To load a scenegraph
#ifndef __SceneGraphBinder_h
#include "gVirtualXRay/SceneGraphBinder.h"
#endif

// To load a scenegraph using ASSIMP
#ifndef __AssimpSceneGraphBinder_h
#include "gVirtualXRay/AssimpSceneGraphBinder.h"
#endif

// To load a scenegraph using OpenSceneGraph
#ifndef __OpenSceneGraphBinder_h
#include "gVirtualXRay/OpenSceneGraphBinder.h"
#endif

#ifndef __CubeMesh_h
#include "gVirtualXRay/CubeMesh.h"
#endif

#ifndef __CylinderMesh_h
#include "gVirtualXRay/CylinderMesh.h"
#endif

// Shaders for rendering
#include "display_gl2.frag.h"
#include "display_gl2.vert.h"
#include "display_gl3.frag.h"
#include "display_gl3.vert.h"


*/



/*
// For Python
#include <boost/python.hpp>

// For Numpy
#include <boost/python/numpy.hpp>

// For tuples
#include <boost/python/tuple.hpp>

// For operators
#include <boost/python/operators.hpp>

// For exceptions
#ifndef __Exception_h
#include "gVirtualXRay/Exception.h"
#endif

// For types such as VEC3, MATRIX4
#ifndef __Types_h
#include "gVirtualXRay/Types.h"
#endif

// For polygon meshes
#ifndef __PolygonMesh_h
#include "gVirtualXRay/PolygonMesh.h"
#endif

// To set the energy of the photons in the incident beam
#ifndef __XRayBeam_h
#include "gVirtualXRay/XRayBeam.h"
#endif

// To set the properties of the X-ray detector
#ifndef __XRayDetector_h
#include "gVirtualXRay/XRayDetector.h"
#endif


// To initialise GLEW, to define matrix stacks, to define state stacks, etc.
#ifndef __OpenGLUtilities_h
#include "gVirtualXRay/OpenGLUtilities.h"
#endif



*/
//using namespace std;
//using namespace gVirtualXRay;
/*namespace np = boost::python::numpy;
*/







void createWindow(int aWindowID = -1, bool aVisibilityFlag = false);
void createOpenGLContext(int aWindowID = -1);
void setWindowSize(int aWidth, int aHeight, int aWindowID = -1);
void destroyWindow(int aWindowID = -1);


void destroyAllWindows(void);
//np::ndarray computeXRayImage();
//np::ndarray computeLBuffer(const char* aLabel);

//np::ndarray computeSinogram(double x, double y, double z, const char* aUnitOfLength,
//                                   unsigned int aNumberOfAngle,
//                                   double anAngleOffset);


void usePointSource(void);
void useParallelBeam(void);
void useParallelSource(void);
void resetBeamSpectrum(void);
//void setMonoChromatic(double anEnergy, const char* aUnitOfEnergy, double aNumberOfPhotons = 1);
void addEnergyBinToSpectrum(double anEnergy, const char* aUnitOfEnergy,  double aNumberOfPhotons);
void setSourcePosition(double x, double y, double z, const char* aUnitOfLength);
void setDetectorPosition(double x, double y, double z, const char* aUnitOfLength);
void setDetectorUpVector(double x, double y, double z);
void setDetectorNumberOfPixels(int aWidth, int aHeight);
void setDetectorPixelSize(double aWidth, double aHeight, const char* aUnitOfLength);
//void saveLastXRayImage(const char* aFileName = 0);

//np::ndarray getNumpyImage(const Image<float>& anImage);

void displayScene(int aWindowID = -1);
//void renderLoop(int aWindowID = -1, PyObject* CallBack = 0);

void loadMeshFile(const char* aLabel,
        const char* aFileName,
        const char* aUnitOfLength);
void loadSceneGraph(const char* aFileName,
        const char* aUnitOfLength);
void moveToCentre(const char* aLabel = 0);
//void addPolygonMeshAsInnerSurface(const PyPolygonMesh& aMesh, const std::string& aLabel);
//void addPolygonMeshAsOuterSurface(const PyPolygonMesh& aMesh, const std::string& aLabel);
void removePolygonMeshes();

//np::ndarray getSceneTransformationMatrix();
//np::ndarray getRootTransformationMatrix();
//np::ndarray getNodeTransformationMatrix(const char* aLabel);

//void setSceneTransformationMatrix(const np::ndarray& aMatrix);
//void setRootTransformationMatrix(const np::ndarray& aMatrix);
//void setNodeTransformationMatrix(const char* aLabel, const np::ndarray& aMatrix);


void scaleNode(const char* aLabel, double x, double y, double z,
        const char* aUnitOfLength);
void rotateNode(const char* aLabel, double angle, double x, double y, double z);
void translateNode(const char* aLabel, double x, double y, double z,
        const char* aUnitOfLength);

void resetSceneTransformation();
void scaleScene(double x, double y, double z,
        const char* aUnitOfLength);
void rotateScene(double angle, double x, double y, double z);
void translateScene(double x, double y, double z,
        const char* aUnitOfLength);


void setHU(const char* aLabel, double aHU);
void setMassAttenuationCoefficient(const char* aLabel,
        double anAttenuationCoefficient);
void setLinearAttenuationCoefficient(const char* aLabel,
        double anAttenuationCoefficient);
//boost::python::tuple getMeshLabelSet();

/*PyPolygonMesh makeCube(double aLength = 1.0, const char* aUnitOfLength = "cm");
PyPolygonMesh makeCylinder(unsigned int aNumberOfSectors = 10,
        double aHeight = 1.0,
        double aRadius = 0.5,
		const char* aUnitOfLength = "cm");
*/



void initialiseXRayRenderer();


void quit();
void initViewer();
void initGL();
void errorCallback(int error, const char* description);
void draw();
void render();



double getUnitOfLength(const char* aUnitOfLength);
double getUnitOfEnergy(const char* aUnitOfEnergy);

void disableArtefactFilteringOnGPU(void);
void disableArtifactFilteringOnGPU(void);
void disableArtefactFilteringOnCPU(void);
void disableArtifactFilteringOnCPU(void);
void enableArtefactFilteringOnGPU(void);
void enableArtifactFilteringOnGPU(void);
void enableArtefactFilteringOnCPU(void);
void enableArtifactFilteringOnCPU(void);


#endif


