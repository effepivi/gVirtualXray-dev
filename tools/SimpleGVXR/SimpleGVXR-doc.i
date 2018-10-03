
// File: index.xml

// File: namespaceg_virtual_x_ray.xml

// File: namespacestd.xml

// File: _simple_g_v_x_r_8cxx.xml

%feature("docstring") std::setWindowSize "

Create an OpenGL context and display it in a window.  
";

%feature("docstring") std::setDetectorNumberOfPixels "

Set the number of pixels of the X-ray detector.  

Parameters
----------
* `aWidth` :  
    the number of pixels along the X-axis  
* `aHeight` :  
    the number of pixels along the Y-axis  
";

%feature("docstring") std::setSceneTransformationMatrix "

Set the 3-D scene transformation matrix.  
";

%feature("docstring") std::setDetectorUpVector "

Set the up-vector defining the orientation of the X-ray detector.  

Parameters
----------
* `x` :  
    the component of the vector along the X-axis  
* `y` :  
    the component of the vector along the Y-axis  
* `z` :  
    the component of the vector along the Z-axis  
";

%feature("docstring") std::setMixture "

Set the mixture corresponding to the material properties of a polygon mesh.
Don't forget to set the density of the material.  
";

%feature("docstring") std::getImageWidth "

Accessor on the width of a given image.  
";

%feature("docstring") std::g_display_detector "
";

%feature("docstring") std::disableArtifactFiltering "

Disable artefact filtering altogether.  
";

%feature("docstring") std::g_last_y_position "
";

%feature("docstring") std::destroyAllWindows "

Close and destroy all the windows and OpenGL contexts that have been created.  
";

%feature("docstring") std::useParallelBeam "

Use a parallel source (e.g. to mimic a source that is extremely far from the
scanned object and detector).  
";

%feature("docstring") std::translateScene "

Translate the 3-D scene (all the polygon meshes will be affected but not
modified).  
";

%feature("docstring") std::setSourcePosition "

Set the position of the X-ray source.  

Parameters
----------
* `x` :  
    the position along the X-axis  
* `y` :  
    the position along the Y-axis  
* `z` :  
    the position along the Z-axis  
* `aUnitOfLength` :  
    the unit of length corresponding to the x, y and z parameters. Acceptable
    values are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  
";

%feature("docstring") std::makeCylinder "

Create a cylinder and set its label in the scenegraph (i.e. identifier). Note
that it is not added to the X-ray renderer.  

Parameters
----------
* `aLabel` :  
    the label in the scenegraph  
* `aNumberOfSectors` :  
    the number of sector defining the mesh  
* `aHeight` :  
    the height of the cylinder  
* `aRadius` :  
    the radius of the cylinder  
* `aUnitOfLength` :  
    the unit of length corresponding to aHeight and aRadius. Acceptable values
    are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\" (default value: \"cm\")  
";

%feature("docstring") std::usePointSource "

Use a point source.  
";

%feature("docstring") std::subtractMesh "

Subtract a polygon mesh (aSource) from another one (aDestination). It
corresponds to the operation as follows: aDestination -= aSource. Both aSource
and aDestination have to already exist in the scenegraph.  

Parameters
----------
* `aDestination` :  
    the polygon mesh that is going to be modified  
* `aSource` :  
    the polygon mesh that is going to be subtracted from aDestination  
";

%feature("docstring") std::ROTATION_SPEED "
";

%feature("docstring") std::resetBeamSpectrum "

Empty the beam spectrum.  
";

%feature("docstring") std::g_initial_far "
";

%feature("docstring") std::setDetectorPosition "

Set the position of the X-ray detector.  

Parameters
----------
* `x` :  
    the position along the X-axis  
* `y` :  
    the position along the Y-axis  
* `z` :  
    the position along the Z-axis  
* `aUnitOfLength` :  
    the unit of length corresponding to the x, y and z parameters. Acceptable
    values are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  
";

%feature("docstring") std::g_zoom "
";

%feature("docstring") std::getMajorVersionOfSimpleGVXR "

Accessor on the major version of SimpleGVXR. This number is changed when
incompatible API changes have been made.  

Returns
-------
the major version number  
";

%feature("docstring") std::scaleScene "

Scale the 3-D scene (all the polygon meshes will be affected but not modified).  
";

%feature("docstring") std::g_button "
";

%feature("docstring") std::keyCallback "
";

%feature("docstring") std::g_display_wireframe "
";

%feature("docstring") std::getUnitOfLength "

Accessor on the numerical value corresponding to a unit of length.  
";

%feature("docstring") std::computeXRayImage "

Compute the X-ray projection corresponding to the environment that has
previously been set, i.e.  

*   Detector position  
*   Detector orientation  
*   Detector size and resolution  
*   Source position  
*   Source shape  
*   Beam spectrum  
*   Scanned object geometries  
*   Scanned object material properties  
";

%feature("docstring") std::loadMeshFile "

Load a polygon mesh from a file, set its label in the scenegraph (i.e.
identifier) and add it to the X-ray renderer.  

Parameters
----------
* `aLabel` :  
    the label in the scenegraph  
* `aFileName` :  
    the file where the polygon mesh data is stored  
* `aUnitOfLength` :  
    the unit of length corresponding to the data stored in the file. Acceptable
    values are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  
";

%feature("docstring") std::getVectorImage "
";

%feature("docstring") std::getMeshLabel "

Accessor on the i-th label in the list of polygon meshes in the scenegraph.  

Parameters
----------
* `i` :  
    the ID of the polygon mesh  

Returns
-------
the label corresponding to the i-th polygon mesh  
";

%feature("docstring") std::setHounsfieldUnit "

Set the Hounsfield value corresponding to the material properties of a polygon
mesh.  
";

%feature("docstring") std::loadSceneGraph "

Load a scenegraph from a file and add its polygon meshes to the X-ray renderer.  

Parameters
----------
* `aFileName` :  
    the file where the polygon mesh data is stored  
* `aUnitOfLength` :  
    the unit of length corresponding to the data stored in the file. Acceptable
    values are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  
";

%feature("docstring") std::g_current_y_position "
";

%feature("docstring") std::initViewer "
";

%feature("docstring") std::scaleNode "

Scale a polygon mesh.  
";

%feature("docstring") std::g_initial_near "
";

%feature("docstring") std::setDensity "

Set the density corresponding to the material properties of a polygon mesh.  
";

%feature("docstring") std::getLastXRayImage "

Accessor on the last X-ray image that has been computed.  
";

%feature("docstring") std::setNodeTransformationMatrix "

Set the transformation matrix of a given node.  
";

%feature("docstring") std::Vector2gvxrMatrix "
";

%feature("docstring") std::saveLastXRayImage "

Save the last X-ray image that has been computed.  
";

%feature("docstring") std::getPatchVersionOfSimpleGVXR "

Accessor on the minor version of SimpleGVXR. This number is changed when bug
fixes have been added in a backwards-compatible manner.  

Returns
-------
the patch version number  
";

%feature("docstring") std::computeRotation "
";

%feature("docstring") std::setRootTransformationMatrix "

Set the transformation matrix of the scengraph's root node.  
";

%feature("docstring") std::mouseButtonCallback "
";

%feature("docstring") std::g_display_beam "
";

%feature("docstring") std::render "
";

%feature("docstring") std::getArcballVector "
";

%feature("docstring") std::removePolygonMeshesFromSceneGraph "

Empty the scenegraph. Note that it also empties the X-ray renderer from all its
meshes.  
";

%feature("docstring") std::getLastLBuffer "

Accessor on the last L-buffer that has been computed.  
";

%feature("docstring") std::createOpenGLContext "

Create an OpenGL context (the window won't be shown).  
";

%feature("docstring") std::getLastSinogram "

Accessor on the last sinogram that has been computed.  
";

%feature("docstring") std::getSceneTransformationMatrix "

Accessor on the 3-D scene transformation matrix.  
";

%feature("docstring") std::moveToCenter "

Move the scenegraph to the centre.  
";

%feature("docstring") std::moveToCenter "

Move a polygon mesh to the centre.  
";

%feature("docstring") std::hideWindow "

Hide a window.  
";

%feature("docstring") std::g_use_arc_ball "
";

%feature("docstring") std::initGLFW "
";

%feature("docstring") std::getMeshLabelSetSize "

Accessor on the number of polygon meshes in the scenegraph.  

Returns
-------
the number of polygon meshes in the scenegraph  
";

%feature("docstring") std::removePolygonMeshesFromXRayRenderer "

Empty the X-ray renderer from all its meshes. Note that the meshes are kept in
the scenegraph.  
";

%feature("docstring") std::getMinorVersionOfSimpleGVXR "

Accessor on the minor version of SimpleGVXR. This number is changed when new
functionalities have been added in a backwards-compatible manner.  

Returns
-------
the minor version number  
";

%feature("docstring") std::g_button_state "
";

%feature("docstring") std::initialiseXRayRenderer "
";

%feature("docstring") std::addMesh "

Add a polygon mesh (aSource) to another one (aDestination). It corresponds to
the operation as follows: aDestination += aSource. Both aSource and aDestination
have to already exist in the scenegraph.  

Parameters
----------
* `aDestination` :  
    the polygon mesh that is going to be modified  
* `aSource` :  
    the polygon mesh that is going to be added to aDestination  
";

%feature("docstring") std::computeLBuffer "

Compute the L-buffer of a polygon mesh corresponding to the environment that has
previously been set, i.e.  

*   Detector position  
*   Detector orientation  
*   Detector size and resolution  
*   Source position  
*   Source shape  
*   Scanned object geometry  
";

%feature("docstring") std::getRootTransformationMatrix "

Accessor on the transformation matrix of the scengraph's root node.  
";

%feature("docstring") std::initGL "
";

%feature("docstring") std::g_last_x_position "
";

%feature("docstring") std::setElement "

Set the chemical element (or element) corresponding to the material properties
of a polygon mesh.  
";

%feature("docstring") std::setElement "

Set the chemical element (or element) corresponding to the material properties
of a polygon mesh.  
";

%feature("docstring") std::showWindow "

Make an OpenGL context visible and display a window.  
";

%feature("docstring") std::getImageHeght "

Accessor on the height of a given image.  
";

%feature("docstring") std::makeCube "

Create a cube and set its label in the scenegraph (i.e. identifier). Note that
it is not added to the X-ray renderer.  

Parameters
----------
* `aLabel` :  
    the label in the scenegraph  
* `aLength` :  
    the size of an edge of the cube  
* `aUnitOfLength` :  
    the unit of length corresponding to aLength. Acceptable values are: \"um\",
    \"micrometre\", \"micrometer\", \"mm\", \"millimetre\", \"millimeter\",
    \"cm\", \"centimetre\", \"centimeter\", \"dm\", \"decimetre\",
    \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\", \"decametre\",
    \"decameter\", \"hm\", \"hectometre\", \"hectometer\", \"km\",
    \"kilometre\", \"kilometer\" (default value: \"cm\")  
";

%feature("docstring") std::setMassAttenuationCoefficient "

Set the mass attenuation coefficient corresponding to the material properties of
a polygon mesh. Don't forget to set the density of the material.  
";

%feature("docstring") std::computeSinogram "

Compute the sinogram corresponding to the environment that has previously been
set, i.e.  

*   Detector position  
*   Detector orientation  
*   Detector size and resolution  
*   Source position  
*   Source shape  
*   Beam spectrum  
*   Scanned object geometries  
*   Scanned object material properties Note that the rotation vector is the same
    as the up-vector of the detector.  
";

%feature("docstring") std::g_current_x_position "
";

%feature("docstring") std::renderLoop "

3-D visualisation of the 3-D scene (source, detector, and scanned objects). Note
that there is interactive loop running. Keys are:  

*   Q/Escape: to quit the event loop (does not close the window)  
*   B: display/hide the X-ray beam  
*   W: display the polygon meshes in solid or wireframe  
*   N: display the X-ray image in negative or positive  
*   L: switch lighting on/off  
*   D: display/hide the X-ray detector  
*   V: display/hide normal vectors  
";

%feature("docstring") std::cursorPosCallback "
";

%feature("docstring") std::displayScene "

3-D visualisation of the 3-D scene (source, detector, and scanned objects). Note
that there is no interactive loop running.  
";

%feature("docstring") std::quit "
";

%feature("docstring") std::addPolygonMeshAsOuterSurface "

Add a polygon mesh, given its label, to the X-ray renderer as an outer surface.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh in the scenegraph  
";

%feature("docstring") std::getVersionOfSimpleGVXR "

Accessor on the full string version of SimpleGVXR.  

Returns
-------
the full version number  
";

%feature("docstring") std::createWindow "

Create an OpenGL context and display it in a window.  
";

%feature("docstring") std::setDetectorPixelSize "

Set the pixel size.  

Parameters
----------
* `aWidth` :  
    the pixel size along the X-axis  
* `aHeight` :  
    the pixel size along the Y-axis  
* `aUnitOfLength` :  
    the unit of length corresponding to the aWidth and aHeight parameters.
    Acceptable values are: \"um\", \"micrometre\", \"micrometer\", \"mm\",
    \"millimetre\", \"millimeter\", \"cm\", \"centimetre\", \"centimeter\",
    \"dm\", \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  
";

%feature("docstring") std::g_use_lighing "
";

%feature("docstring") std::rotateScene "

Rotate the 3-D scene (all the polygon meshes will be affected but not modified).  
";

%feature("docstring") std::saveLastSinogram "

Save the last sinogram that has been computed.  
";

%feature("docstring") std::setLinearAttenuationCoefficient "

Set the linear attenuation coefficient corresponding to the material properties
of a polygon mesh.  
";

%feature("docstring") std::resetSceneTransformation "

Replace the 3-D transformation matrix of the overall 3-D scene by an identity
matrix.  
";

%feature("docstring") std::gvxrMatrix2Vector "
";

%feature("docstring") std::setMonoChromatic "

Use a monochromatic beam spectrum (i.e. one single energy).  
";

%feature("docstring") std::radian2degree "
";

%feature("docstring") std::moveToCentre "

Move the scenegraph to the centre.  
";

%feature("docstring") std::moveToCentre "

Move a polygon mesh to the centre.  
";

%feature("docstring") std::setHU "

Set the Hounsfield value corresponding to the material properties of a polygon
mesh.  
";

%feature("docstring") std::g_display_normal_vectors "
";

%feature("docstring") std::destroyWindow "

Close and destroy a given window or OpenGL context.  
";

%feature("docstring") std::useParallelSource "

Use a parallel source (e.g. to mimic a source that is extremely far from the
scanned object and detector).  
";

%feature("docstring") std::g_initial_fovy "
";

%feature("docstring") std::enableArtifactFilteringOnGPU "

Enable artefact filtering on GPU (it is fast but can be inaccurate).  
";

%feature("docstring") std::getNodeTransformationMatrix "

Accessor on the transformation matrix of a given node.  
";

%feature("docstring") std::addPolygonMeshAsInnerSurface "

Add a polygon mesh, given its label, to the X-ray renderer as an inner surface.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh in the scenegraph  
";

%feature("docstring") std::disableArtefactFiltering "

Disable artefact filtering altogether.  
";

%feature("docstring") std::errorCallback "
";

%feature("docstring") std::setMu "

Set the linear attenuation coefficient corresponding to the material properties
of a polygon mesh.  
";

%feature("docstring") std::saveLastLBuffer "

Save the last L-buffer that has been computed.  
";

%feature("docstring") std::rotateNode "

Rotate a polygon mesh.  
";

%feature("docstring") std::emptyMesh "

Create an empty polygon mesh and set its label in the scenegraph (i.e.
identifier). Note that it is not added to the X-ray renderer.  

Parameters
----------
* `aLabel` :  
    the label in the scenegraph  
";

%feature("docstring") std::getImageRow "

Accessor on the j-th row of pixels of a given image.  
";

%feature("docstring") std::enableArtefactFilteringOnGPU "

Enable artefact filtering on GPU (it is fast but can be inaccurate).  
";

%feature("docstring") std::scrollCallback "
";

%feature("docstring") std::enableArtifactFilteringOnCPU "

Enable artefact filtering on CPU (can be extremely slow as it makes use of ray-
tracing).  
";

%feature("docstring") std::setCompound "

Set the compound corresponding to the material properties of a polygon mesh.
Don't forget to set the density of the material.  
";

%feature("docstring") std::addEnergyBinToSpectrum "

Add an energy bin to the beam spectrum (useful to generate polychromatism).  
";

%feature("docstring") std::enableArtefactFilteringOnCPU "

Enable artefact filtering on CPU (can be extremely slow as it makes use of ray-
tracing).  
";

%feature("docstring") std::translateNode "

Translate a polygon mesh.  
";

%feature("docstring") std::framebufferSizeCallback "
";

%feature("docstring") std::getMeshLabelSet "

Accessor on the list of labels of polygon meshes in the scenegraph.  

Returns
-------
the list of labels  
";

%feature("docstring") std::setHounsfieldValue "

Set the Hounsfield value corresponding to the material properties of a polygon
mesh.  
";

%feature("docstring") std::getUnitOfEnergy "

Accessor on the numerical value corresponding to a unit of energy.  
";

// File: _simple_g_v_x_r_8h.xml

%feature("docstring") makeCylinder "

Create a cylinder and set its label in the scenegraph (i.e. identifier). Note
that it is not added to the X-ray renderer.  

Parameters
----------
* `aLabel` :  
    the label in the scenegraph  
* `aNumberOfSectors` :  
    the number of sector defining the mesh  
* `aHeight` :  
    the height of the cylinder  
* `aRadius` :  
    the radius of the cylinder  
* `aUnitOfLength` :  
    the unit of length corresponding to aHeight and aRadius. Acceptable values
    are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\" (default value: \"cm\")  
";

%feature("docstring") getSceneTransformationMatrix "

Accessor on the 3-D scene transformation matrix.  
";

%feature("docstring") getRootTransformationMatrix "

Accessor on the transformation matrix of the scengraph's root node.  
";

%feature("docstring") getNodeTransformationMatrix "

Accessor on the transformation matrix of a given node.  
";

%feature("docstring") usePointSource "

Use a point source.  
";

%feature("docstring") setHU "

Set the Hounsfield value corresponding to the material properties of a polygon
mesh.  
";

%feature("docstring") setDensity "

Set the density corresponding to the material properties of a polygon mesh.  
";

%feature("docstring") removePolygonMeshesFromXRayRenderer "

Empty the X-ray renderer from all its meshes. Note that the meshes are kept in
the scenegraph.  
";

%feature("docstring") setDetectorPixelSize "

Set the pixel size.  

Parameters
----------
* `aWidth` :  
    the pixel size along the X-axis  
* `aHeight` :  
    the pixel size along the Y-axis  
* `aUnitOfLength` :  
    the unit of length corresponding to the aWidth and aHeight parameters.
    Acceptable values are: \"um\", \"micrometre\", \"micrometer\", \"mm\",
    \"millimetre\", \"millimeter\", \"cm\", \"centimetre\", \"centimeter\",
    \"dm\", \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  
";

%feature("docstring") setMassAttenuationCoefficient "

Set the mass attenuation coefficient corresponding to the material properties of
a polygon mesh. Don't forget to set the density of the material.  
";

%feature("docstring") setSourcePosition "

Set the position of the X-ray source.  

Parameters
----------
* `x` :  
    the position along the X-axis  
* `y` :  
    the position along the Y-axis  
* `z` :  
    the position along the Z-axis  
* `aUnitOfLength` :  
    the unit of length corresponding to the x, y and z parameters. Acceptable
    values are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  
";

%feature("docstring") resetSceneTransformation "

Replace the 3-D transformation matrix of the overall 3-D scene by an identity
matrix.  
";

%feature("docstring") useParallelSource "

Use a parallel source (e.g. to mimic a source that is extremely far from the
scanned object and detector).  
";

%feature("docstring") getImageRow "

Accessor on the j-th row of pixels of a given image.  
";

%feature("docstring") setSceneTransformationMatrix "

Set the 3-D scene transformation matrix.  
";

%feature("docstring") makeCube "

Create a cube and set its label in the scenegraph (i.e. identifier). Note that
it is not added to the X-ray renderer.  

Parameters
----------
* `aLabel` :  
    the label in the scenegraph  
* `aLength` :  
    the size of an edge of the cube  
* `aUnitOfLength` :  
    the unit of length corresponding to aLength. Acceptable values are: \"um\",
    \"micrometre\", \"micrometer\", \"mm\", \"millimetre\", \"millimeter\",
    \"cm\", \"centimetre\", \"centimeter\", \"dm\", \"decimetre\",
    \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\", \"decametre\",
    \"decameter\", \"hm\", \"hectometre\", \"hectometer\", \"km\",
    \"kilometre\", \"kilometer\" (default value: \"cm\")  
";

%feature("docstring") loadSceneGraph "

Load a scenegraph from a file and add its polygon meshes to the X-ray renderer.  

Parameters
----------
* `aFileName` :  
    the file where the polygon mesh data is stored  
* `aUnitOfLength` :  
    the unit of length corresponding to the data stored in the file. Acceptable
    values are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  
";

%feature("docstring") createWindow "

Create an OpenGL context and display it in a window.  
";

%feature("docstring") computeXRayImage "

Compute the X-ray projection corresponding to the environment that has
previously been set, i.e.  

*   Detector position  
*   Detector orientation  
*   Detector size and resolution  
*   Source position  
*   Source shape  
*   Beam spectrum  
*   Scanned object geometries  
*   Scanned object material properties  
";

%feature("docstring") translateNode "

Translate a polygon mesh.  
";

%feature("docstring") setMonoChromatic "

Use a monochromatic beam spectrum (i.e. one single energy).  
";

%feature("docstring") computeLBuffer "

Compute the L-buffer of a polygon mesh corresponding to the environment that has
previously been set, i.e.  

*   Detector position  
*   Detector orientation  
*   Detector size and resolution  
*   Source position  
*   Source shape  
*   Scanned object geometry  
";

%feature("docstring") getLastLBuffer "

Accessor on the last L-buffer that has been computed.  
";

%feature("docstring") loadMeshFile "

Load a polygon mesh from a file, set its label in the scenegraph (i.e.
identifier) and add it to the X-ray renderer.  

Parameters
----------
* `aLabel` :  
    the label in the scenegraph  
* `aFileName` :  
    the file where the polygon mesh data is stored  
* `aUnitOfLength` :  
    the unit of length corresponding to the data stored in the file. Acceptable
    values are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  
";

%feature("docstring") addPolygonMeshAsOuterSurface "

Add a polygon mesh, given its label, to the X-ray renderer as an outer surface.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh in the scenegraph  
";

%feature("docstring") getUnitOfLength "

Accessor on the numerical value corresponding to a unit of length.  
";

%feature("docstring") getLastXRayImage "

Accessor on the last X-ray image that has been computed.  
";

%feature("docstring") setDetectorNumberOfPixels "

Set the number of pixels of the X-ray detector.  

Parameters
----------
* `aWidth` :  
    the number of pixels along the X-axis  
* `aHeight` :  
    the number of pixels along the Y-axis  
";

%feature("docstring") setHounsfieldValue "

Set the Hounsfield value corresponding to the material properties of a polygon
mesh.  
";

%feature("docstring") getUnitOfEnergy "

Accessor on the numerical value corresponding to a unit of energy.  
";

%feature("docstring") setDetectorUpVector "

Set the up-vector defining the orientation of the X-ray detector.  

Parameters
----------
* `x` :  
    the component of the vector along the X-axis  
* `y` :  
    the component of the vector along the Y-axis  
* `z` :  
    the component of the vector along the Z-axis  
";

%feature("docstring") getImageHeght "

Accessor on the height of a given image.  
";

%feature("docstring") setMixture "

Set the mixture corresponding to the material properties of a polygon mesh.
Don't forget to set the density of the material.  
";

%feature("docstring") setNodeTransformationMatrix "

Set the transformation matrix of a given node.  
";

%feature("docstring") createOpenGLContext "

Create an OpenGL context (the window won't be shown).  
";

%feature("docstring") enableArtifactFilteringOnCPU "

Enable artefact filtering on CPU (can be extremely slow as it makes use of ray-
tracing).  
";

%feature("docstring") scaleNode "

Scale a polygon mesh.  
";

%feature("docstring") setCompound "

Set the compound corresponding to the material properties of a polygon mesh.
Don't forget to set the density of the material.  
";

%feature("docstring") getVersionOfSimpleGVXR "

Accessor on the full string version of SimpleGVXR.  

Returns
-------
the full version number  
";

%feature("docstring") moveToCentre "

Move the scenegraph to the centre.  
";

%feature("docstring") moveToCentre "

Move a polygon mesh to the centre.  
";

%feature("docstring") getImageWidth "

Accessor on the width of a given image.  
";

%feature("docstring") getMeshLabel "

Accessor on the i-th label in the list of polygon meshes in the scenegraph.  

Parameters
----------
* `i` :  
    the ID of the polygon mesh  

Returns
-------
the label corresponding to the i-th polygon mesh  
";

%feature("docstring") removePolygonMeshesFromSceneGraph "

Empty the scenegraph. Note that it also empties the X-ray renderer from all its
meshes.  
";

%feature("docstring") disableArtifactFiltering "

Disable artefact filtering altogether.  
";

%feature("docstring") setElement "

Set the chemical element (or element) corresponding to the material properties
of a polygon mesh.  
";

%feature("docstring") setElement "

Set the chemical element (or element) corresponding to the material properties
of a polygon mesh.  
";

%feature("docstring") moveToCenter "

Move the scenegraph to the centre.  
";

%feature("docstring") moveToCenter "

Move a polygon mesh to the centre.  
";

%feature("docstring") rotateScene "

Rotate the 3-D scene (all the polygon meshes will be affected but not modified).  
";

%feature("docstring") addMesh "

Add a polygon mesh (aSource) to another one (aDestination). It corresponds to
the operation as follows: aDestination += aSource. Both aSource and aDestination
have to already exist in the scenegraph.  

Parameters
----------
* `aDestination` :  
    the polygon mesh that is going to be modified  
* `aSource` :  
    the polygon mesh that is going to be added to aDestination  
";

%feature("docstring") displayScene "

3-D visualisation of the 3-D scene (source, detector, and scanned objects). Note
that there is no interactive loop running.  
";

%feature("docstring") emptyMesh "

Create an empty polygon mesh and set its label in the scenegraph (i.e.
identifier). Note that it is not added to the X-ray renderer.  

Parameters
----------
* `aLabel` :  
    the label in the scenegraph  
";

%feature("docstring") setDetectorPosition "

Set the position of the X-ray detector.  

Parameters
----------
* `x` :  
    the position along the X-axis  
* `y` :  
    the position along the Y-axis  
* `z` :  
    the position along the Z-axis  
* `aUnitOfLength` :  
    the unit of length corresponding to the x, y and z parameters. Acceptable
    values are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  
";

%feature("docstring") getMeshLabelSet "

Accessor on the list of labels of polygon meshes in the scenegraph.  

Returns
-------
the list of labels  
";

%feature("docstring") getLastSinogram "

Accessor on the last sinogram that has been computed.  
";

%feature("docstring") translateScene "

Translate the 3-D scene (all the polygon meshes will be affected but not
modified).  
";

%feature("docstring") renderLoop "

3-D visualisation of the 3-D scene (source, detector, and scanned objects). Note
that there is interactive loop running. Keys are:  

*   Q/Escape: to quit the event loop (does not close the window)  
*   B: display/hide the X-ray beam  
*   W: display the polygon meshes in solid or wireframe  
*   N: display the X-ray image in negative or positive  
*   L: switch lighting on/off  
*   D: display/hide the X-ray detector  
*   V: display/hide normal vectors  
";

%feature("docstring") getMeshLabelSetSize "

Accessor on the number of polygon meshes in the scenegraph.  

Returns
-------
the number of polygon meshes in the scenegraph  
";

%feature("docstring") enableArtefactFilteringOnCPU "

Enable artefact filtering on CPU (can be extremely slow as it makes use of ray-
tracing).  
";

%feature("docstring") subtractMesh "

Subtract a polygon mesh (aSource) from another one (aDestination). It
corresponds to the operation as follows: aDestination -= aSource. Both aSource
and aDestination have to already exist in the scenegraph.  

Parameters
----------
* `aDestination` :  
    the polygon mesh that is going to be modified  
* `aSource` :  
    the polygon mesh that is going to be subtracted from aDestination  
";

%feature("docstring") disableArtefactFiltering "

Disable artefact filtering altogether.  
";

%feature("docstring") addEnergyBinToSpectrum "

Add an energy bin to the beam spectrum (useful to generate polychromatism).  
";

%feature("docstring") saveLastSinogram "

Save the last sinogram that has been computed.  
";

%feature("docstring") setHounsfieldUnit "

Set the Hounsfield value corresponding to the material properties of a polygon
mesh.  
";

%feature("docstring") destroyWindow "

Close and destroy a given window or OpenGL context.  
";

%feature("docstring") setMu "

Set the linear attenuation coefficient corresponding to the material properties
of a polygon mesh.  
";

%feature("docstring") saveLastLBuffer "

Save the last L-buffer that has been computed.  
";

%feature("docstring") saveLastXRayImage "

Save the last X-ray image that has been computed.  
";

%feature("docstring") setRootTransformationMatrix "

Set the transformation matrix of the scengraph's root node.  
";

%feature("docstring") getPatchVersionOfSimpleGVXR "

Accessor on the minor version of SimpleGVXR. This number is changed when bug
fixes have been added in a backwards-compatible manner.  

Returns
-------
the patch version number  
";

%feature("docstring") getMinorVersionOfSimpleGVXR "

Accessor on the minor version of SimpleGVXR. This number is changed when new
functionalities have been added in a backwards-compatible manner.  

Returns
-------
the minor version number  
";

%feature("docstring") setLinearAttenuationCoefficient "

Set the linear attenuation coefficient corresponding to the material properties
of a polygon mesh.  
";

%feature("docstring") scaleScene "

Scale the 3-D scene (all the polygon meshes will be affected but not modified).  
";

%feature("docstring") getMajorVersionOfSimpleGVXR "

Accessor on the major version of SimpleGVXR. This number is changed when
incompatible API changes have been made.  

Returns
-------
the major version number  
";

%feature("docstring") rotateNode "

Rotate a polygon mesh.  
";

%feature("docstring") computeSinogram "

Compute the sinogram corresponding to the environment that has previously been
set, i.e.  

*   Detector position  
*   Detector orientation  
*   Detector size and resolution  
*   Source position  
*   Source shape  
*   Beam spectrum  
*   Scanned object geometries  
*   Scanned object material properties Note that the rotation vector is the same
    as the up-vector of the detector.  
";

%feature("docstring") enableArtefactFilteringOnGPU "

Enable artefact filtering on GPU (it is fast but can be inaccurate).  
";

%feature("docstring") destroyAllWindows "

Close and destroy all the windows and OpenGL contexts that have been created.  
";

%feature("docstring") useParallelBeam "

Use a parallel source (e.g. to mimic a source that is extremely far from the
scanned object and detector).  
";

%feature("docstring") addPolygonMeshAsInnerSurface "

Add a polygon mesh, given its label, to the X-ray renderer as an inner surface.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh in the scenegraph  
";

%feature("docstring") showWindow "

Make an OpenGL context visible and display a window.  
";

%feature("docstring") resetBeamSpectrum "

Empty the beam spectrum.  
";

%feature("docstring") setWindowSize "

Create an OpenGL context and display it in a window.  
";

%feature("docstring") hideWindow "

Hide a window.  
";

%feature("docstring") enableArtifactFilteringOnGPU "

Enable artefact filtering on GPU (it is fast but can be inaccurate).  
";

