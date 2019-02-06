********************************************************************************
*              gVirtualXRay: Virtual X-Ray Imaging Library on GPU              *
********************************************************************************



********************************************************************************
* Description                                                                  *
********************************************************************************

This project is focused on developing new software technologies for simulating 
X-ray images on the graphics processor unit (GPU) using OpenGL. It supports 
`old' OpenGL implementation as well as modern OpenGL core profile (OpenGL 3.2+). 
No deprecated function in OpenGL has been used. The library takes care of 
matrix transformations, matrix stacks, etc.

The source code of gVirtualXRay's is available under the BSD 3-Clause License. 
For details on use and redistribution please refer to 
http://opensource.org/licenses/BSD-3-Clause.

gVirtualXRay has been successfully tested on the following platforms (compilers):

 -   MS Windows 7, MS Visual C++ 2013
 -   MS Windows 8, MS Visual C++ 2010
 -   GNU/Linux openSUSE 12.3 (x86_64), g++ 4.7
 -   Mac OS X 10.8, Apple LLVM 4.2
 -   Mac OS X 10.8, g++ 4.7
 -   MS Windows 7, cross compilation using MXE on GNU/Linux openSUSE 12.3 (x86_64) and Mac OS X 10.8 (note that the tutorials do not compile in this configuration)
 -   MS Windows 8, cross compilation using MXE on GNU/Linux openSUSE 12.3 (x86_64) and Mac OS X 10.8 (note that the tutorials do not compile in this configuration)

It should be possible to build it on other platforms, but this has not been tested.

gVirtualXRay has been successfully tested on the following platforms (graphics card):

 -   NVIDIA GeForce 320M (laptop, Mac OS X)
 -   Intel HD Graphics 4000 (laptop, Mac OS X)
 -   AMD Radeon X1950pro (PC, GNU/Linux & Windows 7)
 -   NVIDIA GeForce 680 (PC, GNU/Linux)

It should be possible to run it on other platforms, but this has not been tested.


********************************************************************************
* Paper                                                                        *
********************************************************************************

Details about (most of) the methods used and implemented in gVirtualXRay can 
be found in these papers:
F. P. Vidal, and P.-F. Villard. ``Development and validation of real-time 
simulation of X-ray imaging with respiratory motion''. 
Computerized Medical Imaging and Graphics, 49:1-15, April 2016. 
DOI: 10.1016/j.compmedimag.2015.12.002
PMID: 26773644

Abstract: We present a framework that combines evolutionary optimisation, 
soft tissue modelling and ray tracing on GPU to simultaneously compute 
the respiratory motion and X-ray imaging in real-time. Our aim is to provide 
validated building blocks with high fidelity to closely match both the human 
physiology and the physics of X-rays. A CPU-based set of algorithms is 
presented to model organ behaviours during respiration. Soft tissue deformation 
is computed with an extension of the Chain Mail method. Rigid elements move 
according to kinematic laws. A GPU-based surface rendering method is proposed 
to compute the X-ray image using the Beer-Lambert law. It is provided as 
an open-source library. A quantitative validation study is provided 
to objectively assess the accuracy of both components: (i) the respiration 
against anatomical data, and (ii) the X-ray against the Beer-Lambert law and 
the results of Monte Carlo simulations. Our implementation can be used in 
various applications, such as interactive medical virtual environment 
to train percutaneous transhepatic cholangiography in interventional 
radiology, 2D/3D registration, computation of digitally reconstructed 
radiograph, simulation of 4D sinograms to test tomography 
reconstruction tools.

Keywords: 
 -   Deterministic simulation (ray-tracing); 
 -   Digitally reconstructed radiograph; 
 -   Imaging guidance; 
 -   Interventional radiology training; 
 -   Medical virtual environment; 
 -   Respiration simulation; 
 -   X-ray simulation

There are other scientific papers of interest. For more details, visit our 
publication page at http://gvirtualxray.sourceforge.net/publications.php

Copyright (c) 2013-2017 Dr Franck P. Vidal, Bangor University, f.vidal@bangor.ac.uk
