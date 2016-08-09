Supports any windowing framework that can create an OpenGL context

Works on Windows (native) (cross compilation from Linux and Mac OSX), Linux, and Mac OS X.

Compiled on Mac OS X 10.8.5 using Apple LLVM version 5.1 (clang-503.0.40) for Mac OS X and Makefiles
   - Tutorials that works:
       everything except
           tutorial_01_qt4 and 5: GLSL: ERROR: 0:2: '' :  version '150' is not supported
           tutorial_02_qt4 and 5: GLSL: ERROR: 0:2: '' :  version '150' is not supported
           tutorial_03_qt4 and 5: GLSL: ERROR: 0:2: '' :  version '150' is not supported

   - Validation that works:
       everything except 
           validation_04: 
Error: Not implemented, please contact Dr FP Vidal.
	- in File: /Users/fpvidal/Documents/gvirtualxray-code/branches/zlibIntegration/cmake/../include/gVirtualXRay/XRayRenderer.inl
	- in Function: printSinogram
	- at Line: 605

           validation_06: 
Error: The number of projections is zero.
	- in File: /Users/fpvidal/Documents/gvirtualxray-code/branches/zlibIntegration/src/gVirtualXRay/XRayRenderer.cxx
	- in Function: computeProjectionSet
	- at Line: 609

           validation_07:
Error: The number of projections is zero.
	- in File: /Users/fpvidal/Documents/gvirtualxray-code/branches/zlibIntegration/src/gVirtualXRay/XRayRenderer.cxx
	- in Function: computeProjectionSet
	- at Line: 609

           validation_08 and validation_10:
 Use of ITK not replaced yet

           validation_09:
Error: The number of projections is zero.
	- in File: /Users/fpvidal/Documents/gvirtualxray-code/branches/zlibIntegration/src/gVirtualXRay/XRayRenderer.cxx
	- in Function: computeProjectionSet
	- at Line: 609


   - Examples that work:
AnatomicalData-glfw.app
demo-anatomical-data-glfw
GUI-FLTK.app
gVirtualXRay-gui
Intersections.app
demo-intersections
STL-File-GLFW
welsh_dragon_glfw
welsh_dragon_glut

   - Examples that do not work:

STL-File-GLUT.app

demo-anatomical-data-qt4
AnatomicalData-qt4.app
FBO error: the target is the default framebuffer, but the default framebuffer does not exist.
	- in File: /Users/fpvidal/Documents/gvirtualxray-code/branches/zlibIntegration/examples/AnatomicalData-qt4/GLWidget.cxx
	- in Function: initializeGL
	- at Line: 289

demo-anatomical-data-qt4-more-realistic
AnatomicalData-qt4-moreRealistic.app
ERROR: 0:66: '' :  version '150' is not supported

demo-anatomical-data-qt5
AnatomicalData-qt5.app
Segmentation fault: 11


Compiled on Mac OS X 10.8.5 using Apple LLVM version 5.1 (clang-503.0.40) for Mac OS X and Xcode
Compiled on Mac OS X 10.8.5 using gcc-4.9 (Homebrew gcc49 4.9.0) for Mac OS X
Compiled on Mac OS X 10.8.5 using i686-w64-mingw32 (from MXE) for Windows
Compiled on ?? using ?? for ??

Tutorials using:
- GLUT
- GLFW3
- FLTK
- Qt4
- Qt5

Includes a small 3D library to create FBO, VBO, matrix stacks, viewing transformations, etc.


