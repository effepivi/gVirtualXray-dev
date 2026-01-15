---
title: "gVirtualXray" # short title
search_exclude: true # set to false if you want this page to show up in search results (it's 'true for the template as we don't want that in search results)
description: "Virtual X-Ray Imaging Library on GPU" # a description of the page - A brief description of the exemplar or use case and which problem it solves and which domains (or if its cross domain) it applies to. 
contributors: [Prof Franck P. Vidal, Dr Andréas Meuleman, Dr Aaron Sújar, Mr Iwan T. Mitchell] # a comma separated list of contributors' names, as found in _data/CONTRIBUTORS.yml (add yourself to the files if you do not have an entry)
page_id: # unique page id, ideally lowercase words separated by underscore(s) - for example page_id of 'Galaxy' could be galaxy
type: research_software_story
---

<!-- Please keep all sections and fill them in.
If this is not possible for any reason - you may remove them (you might need to explain to the Editorial Board in your pull request why certain sections are not present).
The text describing what is needed in the sections can be removed.
All comment sections can be removed before your submit a pull request.
-->

<!-- Once you have completed your research community entry - please add it to _data/sidebars/main.yml under the Research Software Stories entry in alphabetical order.
This comment can be deleted in your final page.
-->


## The Problem <!-- do not delete this heading and write your text below it -->
### Virtual X-Ray Imaging Library on GPU

<!-- Describe the problem(s) your software is trying to solve for your community. -->
[gVirtualXray](https://gvirtualxray.sourceforge.io/) (gVXR) is an open-source framework that extends the Beer–Lambert law to simulate X-ray images in real time on a graphics processor unit (GPU) using triangular meshes. 
Simulations generated with gVXR have been benchmarked with clinically realistic phantoms (i.e. complex structures and materials) using Monte Carlo (MC) simulations, real radiographs and real digitally reconstructed radiographs (DRRs), and X-ray computed tomography (XCT). 

## The Community <!-- do not delete this heading and write your text below it -->
<!-- ### Add a short one-two liner subtitle about the community/communities that use or will potentially use this software -->

<!-- Describe the community that uses your software -->

<!-- - Describe how the development is organised (e.g. is it one person, many, how are they organised and/or governed) -->
[gVXR](https://gvirtualxray.sourceforge.io/) was created by Franck Vidal at [Bangor University](https://www.bangor.ac.uk/) in 2013. Now at the 
He actively maintains the code and regularly add new features for the [Science and Technology Facilities Council](https://www.stfc.ac.uk/) (STFC).

<!-- - Who are the users - how experienced are they in using your software? -->

[gVXR](https://gvirtualxray.sourceforge.io/) is an application programming interface (API). It is for software developers who wish to simulate realistic X-ray images in realtime when photon scattering is negligible. It takes into account properties of the X-ray source, sample and detector.

It has been used in a wide range of applications, including real-time medical simulators, proposing a new densitometric radiographic modality in clinical imaging, studying noise removal techniques in fluoroscopy, teaching particle physics and X-ray imaging to undergraduate students in engineering, and XCT to masters students, predicting image quality and artifacts in material science, etc. 
gVXR has also been used to produce a high number of realistic simulated images in optimisation problems and to train machine learning algorithms.


<!-- If programming is not your thing, check out [WebCT](https://webct.io/), a feature-rich environment for previewing and simulating X-ray scans on the web browser. -->


## Technical Aspects <!-- do not delete this heading and write your text below it -->

- What is the tier(s) of software involved (e.g. analysis code, prototype tools, research software infrastructure (including services))

- Technical specifications:
   <!-- - language(s) -->
- [gVXR](https://gvirtualxray.sourceforge.io/)'s core is written in C++ using the OpenGL Shading Language (GLSL) to leverage the GPU. 
-  SimpleGVXR is a smaller library build on the top of it. SimpleGVXR makes use of [SWIG](https://swig.org/) to provide wrappers to Python, R, Ruby, Tcl, C#, Java, and GNU Octave.
- To simplify the setting up of a simulation, a user-friendly JSON file format has been designed for Python users.
- A digital twin framework has been added to make it easy to simulate X-ray images with twins of actual devices.

   - **any measures of the codebase (e.g. size, complexity, activity, how long it's been running)**

gVXR is cross-platform. 
It runs on:
- MS Windows (Intel architecture only), 
- GNU/Linux (Intel architecture & ARM architecture), and 
- MacOS computers (Intel architecture only). 

It supports GPUs from any manufacturer. 
It can even run on platforms without GPUs (in this case, be patient as the CPU will be used). 

gVXR is scalable. 
It runs on:
- [Raspberry Pi](https://www.raspberrypi.com/),
- laptops,
- desktop PCs,
- supercomputers, and
- cloud infrastructures, including:
   - STFC Cloud,
   - [Google Colab](https://colab.research.google.com/), and
   - [Code Ocean](https://codeocean.com/capsule/0412345/tree/v1).

Containerization using [Docker](https://www.docker.com/) is even possible.

### Libraries and Systems <!-- do not delete this heading and write your text below it -->

- List any domain/community specific libraries and systems you make use of in your software solutions.

C++ 17
ZLIB (required) https://www.zlib.net/zlib_license.html
XRAYLIB (required) https://github.com/tschoonj/xraylib/blob/master/license_all.txt
XRayphysics MIT https://github.com/kylechampley/XrayPhysics/blob/main/LICENSE
ASSIMP (required) 3-clause BSD https://github.com/assimp/assimp/blob/master/LICENSE
GZIP (required) (note that we do not distribute it and it is not required to use gVXR)
OpenGL (required) leverage GPU
GLEW (required) https://github.com/nigels-com/glew/blob/master/LICENSE.txt

OpenMP (optional to speed up code running on CPU in parallel, typically the image processing filters and the implicit modelling)
LibTIFF (optional to save TIFF files) https://gitlab.com/libtiff/libtiff/-/blob/master/LICENSE.md
GDCM (optional to save DICOM files) BSD license https://github.com/malaterre/GDCM/blob/master/README.Copyright.txt
ITK (optional to filter images) Apache  https://github.com/InsightSoftwareConsortium/ITK/blob/master/LICENSE
FFTW3 (for a naive implementation of the filtered back projection computed tomography reconstruction on CPU)
FREETYPE (to display text on top of the visualisation window) BSD-style license  https://gitlab.freedesktop.org/freetype/freetype/-/blob/master/docs/FTL.TXT
EGL (required to create an OpenGL context on GNU/Linux) https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/egl/main/eglconfig.h
GDM (required to create an OpenGL context on GNU/Linux) https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/gbm/main/gbm.h
GLX (required to create an OpenGL context on GNU/Linux) https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/glx/glxconfig.h
X11 (required to create an OpenGL context on GNU/Linux) https://github.com/mirror/libX11?tab=License-1-ov-file#readme
GLFW3 (to create an OpenGL context on Windows, GNU/Linux and MacOS -- required for unit tests and for Python wrapper)
Gtest (required for unit tests) BSD-like  https://www.glfw.org/license.html
Doxygen (required for generating the documentation)
Sphinx (required for formatting the documentation)







## Software Practices <!-- do not delete this heading and write your text below it -->
### Add a short one-two liner subtitle about which software practices are being used and their benefits to the user community

- What practices do you use to develop the software (e.g. version control, contribution policy, licensing, other practices)?
- What benefits have the developers gained in building their software by applying these software quality practices?

## Community <!-- do not delete this heading and write your text below it -->
### Add a short one-two liner subtitle about how the software is being adopted for usage and how a community is being built around it

- How do you onboard new developers?
- How do you onboard new users?
   - Explain your main user stories (the main types of users, what they want to achieve and what the benefit to them is) 

## Tools <!-- do not delete this heading and write your text below it -->
### Add a short one-two liner subtitle about the other tools that help in improving your research software  

- List any tools that you use to help you improve or assess the software quality of your code and a short description of how they help your software. Please refer to [_data/tool_and_resource_list.yml](https://github.com/EVERSE-ResearchSoftware/RSQKit/blob/main/_data/tool_and_resource_list.yml) file in the RSQKit repository and add any ones that are missing.
- What has been the benefit to the project of using these tools?

## FAIR & Open <!-- do not delete this heading and write your text below it -->
### Add a short one-two liner subtitle about how your research software is aligned with FAIR and/or Open practices

 - FAIR: see the [FAIR software page in RSQKit](https://everse.software/RSQKit/fair_rs) and [Five recommendations for FAIR software](https://fair-software.eu/) for inspiration - e.g. are there a repository and a license, do you have a community registry/catalogue where the code is available, is it citable, do you have a “good software” checklist?
 - Open: cover how you follow open source practices and open development practices?
 - What has been the benefit of following these open source and open development practices?

## Documentation <!-- do not delete this heading and write your text below it -->
### Add a short one-two liner subtitle about how documenting the research software helps the user community using the research software

 - How is the documentation for your software presented and maintained?

## Sustainability <!-- do not delete this heading and write your text below it -->
### Add a short one-two liner subtitle about how is the research software being made sustainable

- How is the software development lifecycle managed, e.g. are there any governance structures in place?
- How active is the project? 
- How is it funded?
- How is the software maintained during and beyond the end of funding?

## References <!-- do not delete this heading and write your text below it -->

If this page has been inspired or derived from other resources, make sure to reference them here.
