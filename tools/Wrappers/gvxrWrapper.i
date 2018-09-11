/* */

%init
%{
    extern void initPython(void);
%}

%module example
%{
    extern void initPython(void);
    /* Put headers and other declarations here */
/*    extern void createWindow",  createWindow,  createWindow_overloads());
    extern void setWindowSize", setWindowSize, setWindowSize_overloads());
*/
    /*extern void destroyWindow();*/
    extern void destroyAllWindows(void);

   /* extern void computeXRayImage();
    extern void computeLBuffer();
    extern void computeSinogram();
    */

    /*extern void saveLastXRayImage(const char* aFileName);*/

    extern void usePointSource(void);
    extern void useParallelBeam(void);
    extern void useParallelSource(void);

    extern void resetBeamSpectrum(void);
 /*   extern void setMonoChromatic",          setMonoChromatic, setMonoChromatic_overloads()); */
    extern void addEnergyBinToSpectrum(double anEnergy, const char* aUnitOfEnergy, double aNumberOfPhotons);

    extern void setSourcePosition(double x, double y, double z, const char* aUnitOfLength);
    extern void setDetectorPosition(double x, double y, double z, const char* aUnitOfLength);
    extern void setDetectorUpVector(double x, double y, double z);
    extern void setDetectorNumberOfPixels(int aWidth, int aHeight);
    extern void setDetectorPixelSize(double aWidth, double aHeight, const char* aUnitOfLength);

    extern void loadMeshFile(const char* aLabel, const char* aFileName, const char* aUnitOfLength);
    extern void loadSceneGraph(const char* aFileName, const char* aUnitOfLength);

    /*extern void moveToCenter",              moveToCentre, moveToCentre_overloads());
    extern void moveToCentre",              moveToCentre, moveToCentre_overloads());*/
    /*extern void addPolygonMeshAsInnerSurface();
    extern void addPolygonMeshAsOuterSurface();*/
    extern void removePolygonMeshes(void);

    /*extern void getSceneTransformationMatrix();
    extern void getRootTransformationMatrix();
    extern void getNodeTransformationMatrix();
    extern void setSceneTransformationMatrix();
    extern void setRootTransformationMatrix();
    extern void setNodeTransformationMatrix();*/

    extern void scaleNode(const char* aLabel, double x, double y, double z, const char* aUnitOfLength);
    extern void rotateNode(const char* aLabel, double angle, double x, double y, double );
    extern void translateNode(const char* aLabel, double x, double y, double z, const char* aUnitOfLength);
    extern void resetSceneTransformation(void);

    extern void scaleScene(double x, double y, double z, const char* aUnitOfLength);
    extern void rotateScene(double angle, double x, double y, double z);
    extern void translateScene(double x, double y, double z, const char* aUnitOfLength);

    extern void setHU(const char* aLabel, double aHU);
    extern void setMassAttenuationCoefficient(const char* aLabel, double anAttenuationCoefficient);
    extern void setLinearAttenuationCoefficient(const char* aLabel, double anAttenuationCoefficient);
    /*extern void getMeshLabelSet();*/
        
   /* extern void makeCube",     makeCube,     makeCube_overloads());
    extern void makeCylinder", makeCylinder, makeCylinder_overloads());

    extern void saveLastXRayImage", saveLastXRayImage, saveLastXRayImage_overloads());

    extern void displayScene", displayScene, displayScene_overloads());
    extern void renderLoop",   renderLoop,   renderLoop_overloads());


    extern void emptyMesh",                     emptyMesh);
*/





    extern double getUnitOfLength(const char* aUnitOfLength);
    extern double getUnitOfEnergy(const char* aUnitOfEnergy);

    extern void disableArtefactFilteringOnGPU(void);
    extern void disableArtifactFilteringOnGPU(void);
    extern void disableArtefactFilteringOnCPU(void);
    extern void disableArtifactFilteringOnCPU(void);
    extern void enableArtefactFilteringOnGPU(void);
    extern void enableArtifactFilteringOnGPU(void);
    extern void enableArtefactFilteringOnCPU(void);
    extern void enableArtifactFilteringOnCPU(void);
%}

    extern void initPython(void);

    /* Put headers and other declarations here */
/*    extern void createWindow",  createWindow,  createWindow_overloads());
    extern void setWindowSize", setWindowSize, setWindowSize_overloads());
*/
    /*extern void destroyWindow();*/
    extern void destroyAllWindows(void);

   /* extern void computeXRayImage();
    extern void computeLBuffer();
    extern void computeSinogram();
    */

    /*extern void saveLastXRayImage(const char* aFileName);*/

    extern void usePointSource(void);
    extern void useParallelBeam(void);
    extern void useParallelSource(void);

    extern void resetBeamSpectrum(void);
 /*   extern void setMonoChromatic",          setMonoChromatic, setMonoChromatic_overloads()); */
    extern void addEnergyBinToSpectrum(double anEnergy, const char* aUnitOfEnergy, double aNumberOfPhotons);

    extern void setSourcePosition(double x, double y, double z, const char* aUnitOfLength);
    extern void setDetectorPosition(double x, double y, double z, const char* aUnitOfLength);
    extern void setDetectorUpVector(double x, double y, double z);
    extern void setDetectorNumberOfPixels(int aWidth, int aHeight);
    extern void setDetectorPixelSize(double aWidth, double aHeight, const char* aUnitOfLength);

    extern void loadMeshFile(const char* aLabel, const char* aFileName, const char* aUnitOfLength);
    extern void loadSceneGraph(const char* aFileName, const char* aUnitOfLength);

    /*extern void moveToCenter",              moveToCentre, moveToCentre_overloads());
    extern void moveToCentre",              moveToCentre, moveToCentre_overloads());*/
    /*extern void addPolygonMeshAsInnerSurface();
    extern void addPolygonMeshAsOuterSurface();*/
    extern void removePolygonMeshes(void);

    /*extern void getSceneTransformationMatrix();
    extern void getRootTransformationMatrix();
    extern void getNodeTransformationMatrix();
    extern void setSceneTransformationMatrix();
    extern void setRootTransformationMatrix();
    extern void setNodeTransformationMatrix();*/

    extern void scaleNode(const char* aLabel, double x, double y, double z, const char* aUnitOfLength);
    extern void rotateNode(const char* aLabel, double angle, double x, double y, double );
    extern void translateNode(const char* aLabel, double x, double y, double z, const char* aUnitOfLength);
    extern void resetSceneTransformation(void);

    extern void scaleScene(double x, double y, double z, const char* aUnitOfLength);
    extern void rotateScene(double angle, double x, double y, double z);
    extern void translateScene(double x, double y, double z, const char* aUnitOfLength);

    extern void setHU(const char* aLabel, double aHU);
    extern void setMassAttenuationCoefficient(const char* aLabel, double anAttenuationCoefficient);
    extern void setLinearAttenuationCoefficient(const char* aLabel, double anAttenuationCoefficient);
    /*extern void getMeshLabelSet();*/
        
   /* extern void makeCube",     makeCube,     makeCube_overloads());
    extern void makeCylinder", makeCylinder, makeCylinder_overloads());

    extern void saveLastXRayImage", saveLastXRayImage, saveLastXRayImage_overloads());

    extern void displayScene", displayScene, displayScene_overloads());
    extern void renderLoop",   renderLoop,   renderLoop_overloads());


    extern void emptyMesh",                     emptyMesh);
*/





extern double getUnitOfLength(const char* aUnitOfLength);
extern double getUnitOfEnergy(const char* aUnitOfEnergy);

extern void disableArtefactFilteringOnGPU(void);
extern void disableArtifactFilteringOnGPU(void);
extern void disableArtefactFilteringOnCPU(void);
extern void disableArtifactFilteringOnCPU(void);
extern void enableArtefactFilteringOnGPU(void);
extern void enableArtifactFilteringOnGPU(void);
extern void enableArtefactFilteringOnCPU(void);
extern void enableArtifactFilteringOnCPU(void);


/*

    boost::python::class_<PyPolygonMesh>("PyPolygonMesh")
                .def("setMaterial", &PyPolygonMesh::setMaterial)
		        .def("setDensity", &PyPolygonMesh::setDensity)
		        .def("getDensity", &PyPolygonMesh::getDensity)
		        .def("getMassAttenuationCoefficient", &PyPolygonMesh::getMassAttenuationCoefficient)
		        .def("getLinearAttenuationCoefficient", &PyPolygonMesh::getLinearAttenuationCoefficient)
				.def("setLinearAttenuationCoefficient", &PyPolygonMesh::setLinearAttenuationCoefficient)
				.def("translate"                    , &PyPolygonMesh::translate)
				.def("scale"                    , &PyPolygonMesh::scale)
				.def("rotate"                    , &PyPolygonMesh::rotate)
				.def("saveSTLFile"                    , &PyPolygonMesh::saveSTLFile)
		        .def(boost::python::self +  boost::python::self)
		        .def(boost::python::self -  boost::python::self)
		        .def(boost::python::self += boost::python::self)
		        .def(boost::python::self - boost::python::self)
/*		        .def("operator-", &PyPolygonMesh::operator-)
		        .def("operator+=", &PyPolygonMesh::operator+=)
		        .def("operator-=", &PyPolygonMesh::operator-=)
*/
    ;
