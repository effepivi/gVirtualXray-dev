
// File: index.xml

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
* `aParent` :  
    the parent of the node in the scenegraph (default: \"root\")  
";

%feature("docstring") disableArtefactFiltering "

Disable artefact filtering altogether.  
";

%feature("docstring") addEnergyBinToSpectrum "

Add an energy bin to the beam spectrum (useful to generate polychromatism).  
";

%feature("docstring") moveToCenter "

Move the scenegraph to the centre.  
";

%feature("docstring") moveToCenter "

Move a polygon mesh to the centre.  
";

%feature("docstring") saveLastSinogram "

Save the last sinogram that has been computed.  
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
* `aParent` :  
    the parent of the node in the scenegraph (default: \"root\")  
";

%feature("docstring") getImageRow "

Accessor on the j-th row of pixels of a given image.  
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

%feature("docstring") getLastXRayImage "

Accessor on the last X-ray image that has been computed.  
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

%feature("docstring") setMixture "

Set the mixture corresponding to the material properties of a polygon mesh.
Don't forget to set the density of the material.  
";

%feature("docstring") setNodeTransformationMatrix "

Set the transformation matrix of a given node.  
";

%feature("docstring") displayNode "

Display the scenegraph node using OpenGL.  
";

%feature("docstring") createWindow "

Create an OpenGL context and display it in a window.  
";

%feature("docstring") saveLastLBuffer "

Save the last L-buffer that has been computed.  
";

%feature("docstring") invertNormalVectors "

Invert the normal vectors of a given polygon mesh.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  
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

%feature("docstring") destroyAllWindows "

Close and destroy all the windows and OpenGL contexts that have been created.  
";

%feature("docstring") getDetectorPosition "

Accessor on the position of the X-ray detector.  

Parameters
----------
* `aUnitOfLength` :  
    the unit of length corresponding to the returned value. Acceptable values
    are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  

Returns
-------
the source position in a given unit of length  
";

%feature("docstring") getElementSymbol "

Accessor on the chemical element' symbol depending on its Z number.  
";

%feature("docstring") loadSpectrumFromTSV "

Load the incident beam energy spectrum from a TSV file.  
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

%feature("docstring") getMassAttenuationCoefficient "

Accessor on the mass attenuation coefficient at a given energy corresponding to
the material properties of a polygon mesh.  
";

%feature("docstring") getSourcePosition "

Accessor on the position of the X-ray source.  

Parameters
----------
* `aUnitOfLength` :  
    the unit of length corresponding to the returned value. Acceptable values
    are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  

Returns
-------
the source position in a given unit of length  
";

%feature("docstring") setMonoChromatic "

Use a monochromatic beam spectrum (i.e. one single energy).  
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

%feature("docstring") setDetectorNumberOfPixels "

Set the number of pixels of the X-ray detector.  

Parameters
----------
* `aWidth` :  
    the number of pixels along the X-axis  
* `aHeight` :  
    the number of pixels along the Y-axis  
";

%feature("docstring") getUnitOfEnergy "

Accessor on the numerical value corresponding to a unit of energy.  
";

%feature("docstring") setLinearAttenuationCoefficient "

Set the linear attenuation coefficient corresponding to the material properties
of a polygon mesh.  
";

%feature("docstring") getImageHeght "

Accessor on the height of a given image.  
";

%feature("docstring") createOpenGLContext "

Create an OpenGL context (the window won't be shown).  
";

%feature("docstring") scaleNode "

Scale a polygon mesh.  
";

%feature("docstring") enableArtifactFilteringOnGPU "

Enable artefact filtering on GPU (it is fast but can be inaccurate).  
";

%feature("docstring") moveToCentre "

Move the scenegraph to the centre.  
";

%feature("docstring") moveToCentre "

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

%feature("docstring") getDiffuseColour "

Accessor on the diffuse colour of the material of a given polygon mesh.  

Returns
-------
the diffuse colour  
";

%feature("docstring") getLastSinogram "

Accessor on the last sinogram that has been computed.  
";

%feature("docstring") getPhtotonCountEnergyBins "

Accessor on the number of photons per energy bin of the beam spectrum.  
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

%feature("docstring") addPolygonMeshAsOuterSurface "

Add a polygon mesh, given its label, to the X-ray renderer as an outer surface.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh in the scenegraph  
";

%feature("docstring") getElementName "

Accessor on the chemical element's name depending on its Z number.  
";

%feature("docstring") setHounsfieldValue "

Set the Hounsfield value corresponding to the material properties of a polygon
mesh.  
";

%feature("docstring") disableArtifactFiltering "

Disable artefact filtering altogether.  
";

%feature("docstring") setColor "

Set the color of a given polygon mesh.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  
* `R` :  
    the red channel  
* `G` :  
    the green channel  
* `B` :  
    the blue channel  
* `A` :  
    the alpha channel  
";

%feature("docstring") scaleScene "

Scale the 3-D scene (all the polygon meshes will be affected but not modified).  
";

%feature("docstring") saveLastXRayImage "

Save the last X-ray image that has been computed.  
";

%feature("docstring") useParallelBeam "

Use a parallel source (e.g. to mimic a source that is extremely far from the
scanned object and detector).  
";

%feature("docstring") hideWindow "

Hide a window.  
";

%feature("docstring") getSceneTransformationMatrix "

Accessor on the 3-D scene transformation matrix.  
";

%feature("docstring") getNodeTransformationMatrix "

Accessor on the transformation matrix of a given node.  
";

%feature("docstring") getDetectorSize "

Accessor on the size of the X-ray detector.  

Parameters
----------
* `aUnitOfLength` :  
    the unit of length corresponding to the returned value. Acceptable values
    are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  

Returns
-------
the size in a given unit of length  
";

%feature("docstring") setHU "

Set the Hounsfield value corresponding to the material properties of a polygon
mesh.  
";

%feature("docstring") setMu "

Set the linear attenuation coefficient corresponding to the material properties
of a polygon mesh.  
";

%feature("docstring") getChildLabel "

Accessor on the i-th child's label in the list of children of a given node in
the scenegraph.  

Parameters
----------
* `aLabel` :  
    the label in the scenegraph  
* `i` :  
    the ID of the child  

Returns
-------
the label corresponding to the i-th child  
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

%feature("docstring") useParallelSource "

Use a parallel source (e.g. to mimic a source that is extremely far from the
scanned object and detector).  
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

%feature("docstring") getLastLBuffer "

Accessor on the last L-buffer that has been computed.  
";

%feature("docstring") emptyMesh "

Create an empty polygon mesh and set its label in the scenegraph (i.e.
identifier). Note that it is not added to the X-ray renderer.  

Parameters
----------
* `aLabel` :  
    the label in the scenegraph  
* `aParent` :  
    the parent of the node in the scenegraph (default: \"root\")  
";

%feature("docstring") getShininess "

Accessor on the shininess of the material of a given polygon mesh.  

Returns
-------
the shininess coefficient  
";

%feature("docstring") usePointSource "

Use a point source.  
";

%feature("docstring") getDetectorUpVector "

Accessor on the up-vector defining the orientation of the X-ray detector.  

Returns
-------
the up-vector  
";

%feature("docstring") setColour "

Set the colour of a given polygon mesh.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  
* `R` :  
    the red channel  
* `G` :  
    the green channel  
* `B` :  
    the blue channel  
* `A` :  
    the alpha channel  
";

%feature("docstring") getAmbientColour "

Accessor on the ambient colour of the material of a given polygon mesh.  

Returns
-------
the ambient colour  
";

%feature("docstring") setWindowSize "

Create an OpenGL context and display it in a window.  
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

%feature("docstring") enableArtefactFilteringOnGPU "

Enable artefact filtering on GPU (it is fast but can be inaccurate).  
";

%feature("docstring") getElementDensity "

Accessor on the chemical element's density.  
";

%feature("docstring") getElementDensity "

Accessor on the chemical element's density.  
";

%feature("docstring") enableArtefactFilteringOnCPU "

Enable artefact filtering on CPU (can be extremely slow as it makes use of ray-
tracing).  
";

%feature("docstring") getNumberOfChildren "

Get the number of children of a node in the scenegraph.  

Parameters
----------
* `aLabel` :  
    the label in the scenegraph  

Returns
-------
the number of chidren  
";

%feature("docstring") setDensity "

Set the density corresponding to the material properties of a polygon mesh.  
";

%feature("docstring") resetSceneTransformation "

Replace the 3-D transformation matrix of the overall 3-D scene by an identity
matrix.  
";

%feature("docstring") getMaterialLabel "

Accessor on the material label of a polygon mesh.  
";

%feature("docstring") translateNode "

Translate a polygon mesh.  
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

%feature("docstring") enableArtifactFilteringOnCPU "

Enable artefact filtering on CPU (can be extremely slow as it makes use of ray-
tracing).  
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

%feature("docstring") getImageWidth "

Accessor on the width of a given image.  
";

%feature("docstring") removePolygonMeshesFromSceneGraph "

Empty the scenegraph. Note that it also empties the X-ray renderer from all its
meshes.  
";

%feature("docstring") setElement "

Set the chemical element (or element) corresponding to the material properties
of a polygon mesh.  
";

%feature("docstring") setElement "

Set the chemical element (or element) corresponding to the material properties
of a polygon mesh.  
";

%feature("docstring") getUnitOfLength "

Accessor on the numerical value corresponding to a unit of length.  
";

%feature("docstring") getLinearAttenuationCoefficient "

Accessor on the linear attenuation coefficient at a given energy corresponding
to the material properties of a polygon mesh.  
";

%feature("docstring") addPolygonMeshAsInnerSurface "

Add a polygon mesh, given its label, to the X-ray renderer as an inner surface.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh in the scenegraph  
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

%feature("docstring") getDensity "

Accessor on the density corresponding to the material properties of a polygon
mesh.  
";

%feature("docstring") removePolygonMeshesFromXRayRenderer "

Empty the X-ray renderer from all its meshes. Note that the meshes are kept in
the scenegraph.  
";

%feature("docstring") setHounsfieldUnit "

Set the Hounsfield value corresponding to the material properties of a polygon
mesh.  
";

%feature("docstring") getSpecularColour "

Accessor on the specular colour of the material of a given polygon mesh.  

Returns
-------
the specular colour  
";

%feature("docstring") destroyWindow "

Close and destroy a given window or OpenGL context.  
";

%feature("docstring") setRootTransformationMatrix "

Set the transformation matrix of the scengraph's root node.  
";

%feature("docstring") getDetectorNumberOfPixels "

Accessor on the number of pixels of the X-ray detector.  

Returns
-------
the size of the detector in number of pixels  
";

%feature("docstring") getRootTransformationMatrix "

Accessor on the transformation matrix of the scengraph's root node.  
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

%feature("docstring") getElementAtomicNumber "

Accessor on the chemical element's Z number.  
";

%feature("docstring") getEnergyBins "

Accessor on the energy bins of the beam spectrum.  
";

%feature("docstring") showWindow "

Make an OpenGL context visible and display a window.  
";

%feature("docstring") resetBeamSpectrum "

Empty the beam spectrum.  
";

