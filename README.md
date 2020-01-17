# gVirtualXRay: Virtual X-Ray Imaging Library on GPU

This project is focused on developing new software technologies for simulating X-ray images on the graphics processor unit (GPU) using OpenGL. It supports ‘old’ OpenGL implementation as well as modern OpenGL core profile (OpenGL 3.2+). No deprecated function in OpenGL has been used. The library takes care of matrix transformations, matrix stacks, etc.

X-ray simulations created with gVirtualXRay have been used in various applications, including:
- medical VR simulator,
- micro-CT in material science application,
- teaching particle physics to undergraduate students.
Please check gVirtualXRay's website at [http://gvirtualxray.sourceforge.net/](http://gvirtualxray.sourceforge.net/) for more
information.

The source code of gVirtualXRay’s is available under the BSD 3-Clause License. For details on use and redistribution please refer to [LICENSE.txt](LICENSE.txt) and [http://opensource.org/licenses/BSD-3-Clause](http://opensource.org/licenses/BSD-3-Clause).


## Build and test status of the trunk

gVirtualXRay has been successfully tested on the following platforms (compilers):

- MS Windows 10, MS Visual C++ 16 2019
- MS Windows 10, MS Visual C++ 15 2017
- GNU/Linux openSUSE Leap 15.0 (x86_64), g++ 7.4.1
- GNU/Linux openSUSE Leap 15.1 (x86_64), g++ 8.?.?
- Mac OS X 10.8, Apple LLVM 4.2
- Mac OS X 10.8, g++ 4.7
- MS Windows 10, cross compilation using MXE on GNU/Linux openSUSE Leap 15.0 (x86_64) and Mac OS X 10.8

It should be possible to build it on other platforms, but this has not been tested.

gVirtualXRay has been successfully tested on the following platforms (graphics cards):

- NVIDIA GeForce 320M (laptop, Mac OS X)
- Intel HD Graphics 4000 (laptop, Mac OS X)
- AMD Radeon X1950pro (PC, GNU/Linux & Windows 7)
- NVIDIA GeForce 680 (PC, GNU/Linux)

It should be possible to run it on other platforms, but this has not been tested.
