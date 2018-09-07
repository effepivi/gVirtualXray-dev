#!/usr/bin/env python3

import sys # For the command line arguments
import gvxrPython as gvxr
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mpimg

try:
    # No file name given, or unit of length
    if len(sys.argv) < 3:
        error = "Wrong number of command line arguments.\nUsage:\t" + sys.argv[0] + " filename.stl  unit   (e.g. cm or mm)\nexample\t" + sys.argv[0] + " welsh-dragon-small.obj mm";
        raise Exception(error)

    # Create an OpenGL context
    print("Create an OpenGL context")
    gvxr.createWindow();
    gvxr.setWindowSize(600, 600);

    # Set up the beam
    print("Set up the beam")
    gvxr.setSourcePosition(-20.0,  0.0, 0.0, "cm");
    gvxr.usePointSource();
    #gvxr.useParallelBeam();
    gvxr.setMonoChromatic(80, "keV", 1);

    # Set up the detector
    print("Set up the detector");
    gvxr.setDetectorPosition(10.0, 0.0, 0.0, "cm");
    gvxr.setDetectorUpVector(0, 0, -1);
    gvxr.setDetectorNumberOfPixels(640, 640);
    gvxr.setDetectorPixelSize(0.5, 0.5, "mm");

    # Load the data
    print("Load the data");
    gvxr.loadSceneGraph(sys.argv[1], sys.argv[2]);

    # Set the material properties
    print ("Number of meshes in scenegraph:\t" + str(len(gvxr.getMeshLabelSet())))
    for mesh in gvxr.getMeshLabelSet():
        print("Set the mesh's linear attenuation coefficient");
        gvxr.setAttenuationCoefficient(mesh, 0.5);

    # Move everything to the centre
    gvxr.moveToCentre();

    # Use GPU artefact filtering
    gvxr.enableArtefactFilteringOnGPU();

    # Compute an X-ray image and retrieve the image in Numpy's 2D array format
    print("Compute an X-ray image");
    np_image = gvxr.computeXRayImage();

    gvxr.displayScene();

    plt.imshow(np.log(np_image), cmap="gray");
    plt.show()
    np.savetxt("xray_image.txt", np_image);

    # Display the 3D scene (no event loop)
    # Run an interactive loop 
    # (can rotate the 3D scene and zoom-in)
    # Keys are:
    # Q/Escape: to quit the event loop (does not close the window)
    # B: display/hide the X-ray beam
    # W: display the polygon meshes in solid or wireframe
    # N: display the X-ray image in negative or positive
    # H: display/hide the X-ray detector
    gvxr.renderLoop();


except Exception as error:
    error_type, error_instance, traceback = sys.exc_info();
    error_instance.args = (error_instance.args[0], "")
    #print('Caught this error: ' + repr(error))
    print (error_type, error_instance, traceback)
