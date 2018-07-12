import gvxrPython as gvxr
#gvxr.createWindow()
#gvxr.createWindow()
#gvxr.createWindow()
#gvxr.createWindow()
#gvxr.createWindow(7)

#gvxr.destroyWindow(5)
#gvxr.destroyWindow(2)
#
#gvxr.setWindowSize(128, 128, 0)
#gvxr.setWindowSize(128, 512, 1)
#gvxr.setWindowSize(512, 128, 2)
#gvxr.setWindowSize(512, 512, 3)
#gvxr.setWindowSize(256, 256, 7)

#gvxr.destroyAllWindows()

# Create an OpenGL context
print("Create an OpenGL context")
gvxr.createWindow();
gvxr.setWindowSize(512, 512);


# Set up the beam
print("Set up the beam")
gvxr.setSourcePosition(0.0,  500.0, 0.0);
gvxr.usePointSource();
#gvxr.useParallelBeam();
gvxr.setMonoChromatic(0.08);

# Set up the detector
print("Set up the detector");
gvxr.setDetectorPosition(0.0, -250.0, 0.0);
gvxr.setDetectorUpVector(0, 0, 1);
gvxr.setDetectorNumberOfPixels(250, 250);
gvxr.setDetectorPixelSize(1.0, 1.0);

# Load the data
print("Load the data");
gvxr.loadMeshFile("hand.stl");

# Compute an X-ray image
print("Compute an X-ray image");
gvxr.computeXRayImage();

# Save the last image into a file
print("Save the last image into a file");
gvxr.saveLastXRayImage();


exit()

