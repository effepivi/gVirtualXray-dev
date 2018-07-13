from PIL import Image
import numpy as np

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
gvxr.setSourcePosition(0.0,  0.0, -40.0, "cm");
gvxr.usePointSource();
#gvxr.useParallelBeam();
gvxr.setMonoChromatic(0.08, "MeV", 1000);

# Set up the detector
print("Set up the detector");
gvxr.setDetectorPosition(0.0, 0.0, 10.0, "cm");
gvxr.setDetectorUpVector(0, 1, 0);
gvxr.setDetectorNumberOfPixels(640, 320);
gvxr.setDetectorPixelSize(1.0, 1.0, "mm");

# Load the data
print("Load the data");
gvxr.loadMeshFile("dragon", "welsh-dragon-small.stl", "mm");
print("Move the mesh to the centre");
gvxr.moveToCentre("dragon");
print("Move the mesh to the center");
gvxr.moveToCenter("dragon");
print("Set the mesh's Hounsfield unit");
gvxr.setHU("dragon", 1000)

# Compute an X-ray image
print("Compute an X-ray image");
gvxr.computeXRayImage();

# Save the last image into a file
print("Save the last image into a file");
gvxr.saveLastXRayImage();

np_image = gvxr.getNumpyImage();
img = Image.fromarray(np_image)
img.save('my.png')
img.show()

#exit()

