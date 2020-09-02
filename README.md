---
title: "gVirtualXRay: Virtual X-Ray Imaging Library on GPU"
author: Dr Franck P. Vidal (Bangor University)
date: 17th Jan 2020
---

# gVirtualXRay: Virtual X-Ray Imaging Library on GPU

## Links

* [Homepage](http://gvirtualxray.sourceforge.net/)
* [Download](http://gvirtualxray.sourceforge.net/downloads.php)
* [Help](mailto:f <DOT> vidal <AT> bangor <DOT> ac <DOT> uk)
* [Demos](http://gvirtualxray.sourceforge.net/demos.php)
* [Publications](http://gvirtualxray.sourceforge.net/publications.php)

## About

This project is focused on developing new software technologies for simulating X-ray images on the graphics processor unit (GPU) using OpenGL. It supports ‘old’ OpenGL implementation as well as modern OpenGL core profile (OpenGL 3.2+). No deprecated function in OpenGL has been used. The library takes care of matrix transformations, matrix stacks, etc.

X-ray simulations created with gVirtualXRay have been used in various applications, including:

- real-time medical simulations for training [[1](#1),[2](#2),[3](#3)],
- micro-CT in material science application
  - optimisation for reverse engineering of tungsten fibres [[4](#4)]
  - study of artefact causes in X-ray micro-CT [[5](#5)]
- designing new clinical imaging techniques [[6](#6)]
- teaching particle physics to undergraduate students [[7](#7)]
- realistic data acquisition simulation in CT imaging with patient respiration [[1](#1),[8](#8),[9](#9)].

Please check gVirtualXRay's website at [http://gvirtualxray.sourceforge.net/](http://gvirtualxray.sourceforge.net/) for more
information.

## Scientific and Industrial Collaboration

If you are interested in any form of collaboration (e.g. to develop your own application) on a paper or grant proposal, drop me an email [(f DOT vidal AT bangor DOT ac DOT uk)](f <DOT> vidal <AT> bangor <DOT> ac <DOT> uk).

## Copyright

The source code of gVirtualXRay’s is available under the BSD 3-Clause License. For details on use and redistribution please refer to [LICENSE.txt](LICENSE.txt) and [http://opensource.org/licenses/BSD-3-Clause](http://opensource.org/licenses/BSD-3-Clause).

## Build and test status of the trunk

gVirtualXRay may be built from source using [CMake](https://cmake.org).
It has been successfully tested on the following platforms (compilers):

- MS Windows 10, MS Visual C++ 16 2019
- MS Windows 10, MS Visual C++ 15 2017
- GNU/Linux openSUSE Leap 15.0 (x86_64), g++ 7.4.1
- GNU/Linux openSUSE Leap 15.1 (x86_64), g++ 8.?.?
- Mac OS X 10.13.6, Apple LLVM 9.1.0 (clang-902.0.39.2)
- Mac OS X 10.13.6, GNU gcc 8.3.0
- Mac OS X 10.??, g++ ?.?
- MS Windows 10, cross compilation using MXE on GNU/Linux openSUSE Leap 15.0 (x86_64) and Mac OS X 10.13

It should be possible to build it on other platforms, but this has not been tested.

gVirtualXRay has been successfully tested on the following platforms (graphics cards):

- NVIDIA GeForce 320M (laptop, Mac OS X)
- Intel HD Graphics 4000 (laptop, Mac OS X)
- AMD Radeon X1950pro (PC, GNU/Linux & Windows 10)
- NVIDIA Titan Xp (PC, GNU/Linux)

It should be possible to run it on other platforms, but this has not been tested.

## References
1. <a name="1"></a> Sujar, A., Meuleman, A., Villard, P.-F., García, M., & Vidal, F. P. (2017). gVirtualXRay: Virtual X-Ray Imaging Library on GPU. In *Computer Graphics and Visual Computing (CGVC)* (pp. 61–68). [DOI: 10.2312/cgvc.20171279](https://doi.org/10.2312/cgvc.20171279)
2. <a name="2"></a>Sujar, A., Kelly, G., García, M., & Vidal, F. P. (2019). Projectional Radiography Simulator: an Interactive Teaching Tool. In *Computer Graphics and Visual Computing (CGVC)*. [DOI: 10.2312/cgvc.20191267](https://doi.org/10.2312/cgvc.20191267)
3. <a name="3"></a>Zuo, Z., Qian, W. Y., Liao, X., & Heng, P. (2018). Position based catheterization and angiography simulation. In *2018 IEEE 6th International Conference on Serious Games and Applications for Health (SeGAH)* (pp. 1–7). [DOI: 10.1109/SeGAH.2018.8401369](http://dx.doi.org/10.1109/SeGAH.2018.8401369)
4. <a name="4"></a>Wen, T., Mihail, R., Al-maliki, shatha, Letang, J., & Vidal, F. P. (2019). Registration of 3D Triangular Models to 2D X-ray Projections Using Black-box Optimisation and X-ray Simulation. In *Computer Graphics and Visual Computing (CGVC)*. [DOI: 10.2312/cgvc.20191265](http://dx.doi.org/10.2312/cgvc.20191265)
5. <a name="5"></a>Vidal, F. P. (2018). gVirtualXRay -- Fast X-ray Simulation on GPU. In *Workshop on Image-Based Finite Element Method for Industry 2018 (IBFEM-4i 2018)*. [DOI: 10.5281/zenodo.1452506](http://dx.doi.org/10.5281/zenodo.1452506)
6. <a name="6"></a>Albiol, F., Corbi, A., & Albiol, A. (n.d.). Densitometric Radiographic Imaging With Contour Sensors. *IEEE Access*, 7, 18902–18914. [DOI: 10.1109/ACCESS.2019.2895925](http://dx.doi.org/10.1109/ACCESS.2019.2895925)
7. <a name="7"></a>Corbi, A., Burgos, D., Vidal, F. P., Albiol, F., & Albiol, A. (2020). X-ray imaging virtual online laboratory for engineering undergraduates. European Journal of Physics, 41(1), 1--31. [DOI: 10.1088/1361-6404/ab5011](http://dx.doi.org/10.1088/1361-6404/ab5011)
8. <a name="8"></a>Vidal, F. P., & Villard, P.-F. (2016). Development and validation of real-time simulation of X-ray imaging with respiratory motion. *Computerized Medical Imaging and Graphics*. [DOI: 10.1016/j.compmedimag.2015.12.002](http://dx.doi.org/10.1016/j.compmedimag.2015.12.002)
9. <a name="9"></a>Simulated Motion Artefact in Computed Tomography. (2015). *In Eurographics Workshop on Visual Computing for Biology and Medicine*. [DOI: 10.2312/vcbm.20151228](http://dx.doi.org/10.2312/vcbm.20151228).


<small>&copy; Copyright 2020, Dr Franck P. Vidal, School of Computer science and Electronic Engineering, Bangor University. All rights reserved</small>
