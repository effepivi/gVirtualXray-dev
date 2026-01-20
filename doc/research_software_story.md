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
<!-- ### Virtual X-Ray Imaging Library on GPU -->

<!-- Describe the problem(s) your software is trying to solve for your community. -->
[gVirtualXray](https://gvirtualxray.sourceforge.io/) (gVXR) is an open-source framework that extends the Beer–Lambert law to simulate realistic X-ray images in realtime on a graphics processor unit (GPU) using triangular meshes. 
Simulations generated with gVXR have been benchmarked with clinically realistic phantoms (i.e. complex structures and materials) using Monte Carlo (MC) simulations, real radiographs and real digitally reconstructed radiographs (DRRs), and X-ray computed tomography (XCT). 


## The Community <!-- do not delete this heading and write your text below it -->
<!-- ### Add a short one-two liner subtitle about the community/communities that use or will potentially use this software -->

<!-- Describe the community that uses your software -->
Today, gVXR is widely used in the UK and further afield, across academia, national facilities, and industry. 
It has been used in a [wide range of applications](https://gvirtualxray.sourceforge.io/applications/), including real-time medical simulators, proposing a new densitometric radiographic modality in clinical imaging, studying noise removal techniques in fluoroscopy, teaching particle physics and X-ray imaging to undergraduate students in engineering, and XCT to masters students, predicting image quality and artifacts in material science, etc. gVXR has also been used to produce a high number of realistic simulated images in optimisation problems and to train machine learning algorithms. A review paper published in 2025 on [*X-ray simulations with gVXR in education, digital twining, experiment planning, and data analysis*](https://doi.org/10.1016/j.nimb.2025.165804) demonstrates its growing community and scientific impact.


<!-- - Describe how the development is organised (e.g. is it one person, many, how are they organised and/or governed) -->
[gVXR](https://gvirtualxray.sourceforge.io/) was initially created by Franck Vidal at [Bangor University](https://www.bangor.ac.uk/) in December 2013. 
<!-- Final year students, MSc students and PhD students at Bangor University have been contributing to gVXR under Franck's supervision.  -->
Since October 2023, he actively maintains the code and regularly add new features for the [Science and Technology Facilities Council](https://www.stfc.ac.uk/) (STFC).

<!-- - Who are the users - how experienced are they in using your software? -->
[gVXR](https://gvirtualxray.sourceforge.io/) is an application programming interface (API). It is for software developers or scientists who wish to simulate realistic X-ray images in realtime when photon scattering is negligible. 
It takes into account properties of the X-ray source, sample and detector. 

<!-- If programming is not your thing, check out [WebCT](https://webct.io/), a feature-rich environment for previewing and simulating X-ray scans on the web browser. -->


## Technical Aspects <!-- do not delete this heading and write your text below it -->

- What is the tier(s) of software involved (e.g. analysis code, prototype tools, research software infrastructure (including services))

- Technical specifications:
   <!-- - language(s) -->
- [gVXR](https://gvirtualxray.sourceforge.io/)'s core is written in C++ using the OpenGL Shading Language (GLSL) to leverage the GPU. 
-  SimpleGVXR is a smaller library built on the top of it. SimpleGVXR makes use of [SWIG](https://swig.org/) to provide wrappers to Python, R, Ruby, Tcl, C#, Java, and GNU Octave.
- To simplify the setting up of a simulation, a user-friendly JSON file format has been designed for Python users.
- A digital twin framework has been added to make it easy to simulate X-ray images with twins of actual devices. It is only available in Python.

   - **any measures of the codebase (e.g. size, complexity, activity, how long it's been running)**

gVXR is cross-platform. 
It runs on:
- MS Windows (Intel architecture only), 
- GNU/Linux (Intel architecture & ARM architecture), and 
- MacOS computers (Intel architecture only). 
Other architectures may be compatible, but they have not been tested.

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

<!-- - List any domain/community specific libraries and systems you make use of in your software solutions. -->

- Required:
   - [C++ 17](https://en.cppreference.com/w/cpp/17.html): **Find why C++ 17 is used**
   - [GLEW](http://glew.sourceforge.net/): 
   provides efficient run-time mechanisms for determining which OpenGL extensions are supported on the target platform, and exposes OpenGL core and extension functionality in a single header file.
   [[Licence]](https://github.com/nigels-com/glew/blob/master/LICENSE.txt) **CHECK WHAT TYPE IT IS**
   - [GLFW3]():   **ADD URL**
   is a platform-independent API for creating windows and OpenGL contexts on MS Windows, macOS and GNU/Linux and other Unix-like systems. On GNU/Linux both X11 and Wayland are supported.
   [[zlib/libpng license]](https://www.glfw.org/license.html)
   - [GZIP](): **ADD URL**
   compresses the shader files to make the binary distributions smaller. 
   Note that we do not distribute it. It is not required to use gVXR, just to build it.
   [[GNU licence]]() **ADD LICENCE URL**
   - [OpenGL](https://www.opengl.org/):
   leverages GPU 
   [Open standard]
   - [XRAYLIB](): **ADD URL**
   To compute mass attenuation coefficients. 
   [[Licence]](https://github.com/tschoonj/xraylib/blob/master/license_all.txt) **CHECK WHAT TYPE IT IS**
   - [XRayphysics](): **ADD URL**
   generates spectra for X-ray tubes with a tungsten target. **CAN WE USE OTHER TARGETS?**
   [[MIT licence]](https://github.com/kylechampley/XrayPhysics/blob/main/LICENSE)
   - [ZLIB]():
   Decompress shaders at runtime.
   [[Licence]](https://www.zlib.net/zlib_license.html) **CHECK WHAT TYPE IT IS**

- Optional:
   - ASSIMP:
   to load polygon meshes from popular file formats.
   [[3-clause BSD licence]](https://github.com/assimp/assimp/blob/master/LICENSE)
   - Doxygen:
   to generate the technical documentation from the C++ header files.
   [[Licence]]() **ADD LICENCE URL**  **CHECK WHAT TYPE IT IS**
   - EGL:
   to create an OpenGL context on GNU/Linux.
    [[Licence]](https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/egl/main/eglconfig.h)    **CHECK WHAT TYPE IT IS**
   - FFTW3: 
   performs the fast Fourier transform and inverse transform to implement a naive filtered back projection computed tomography reconstruction on CPU.
   [[Licence]]() **ADD LICENCE URL**  **CHECK WHAT TYPE IT IS**
   - FREETYPE:
   to display text on top of the visualisation window.
   [[BSD-style license]](https://gitlab.freedesktop.org/freetype/freetype/-/blob/master/docs/FTL.TXT)
   - GDCM: 
   to save DICOM files.
   [[BSD license]](https://github.com/malaterre/GDCM/blob/master/README.Copyright.txt)
   - GDM:
   to create an OpenGL context on GNU/Linux.
   [[Licence]](https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/gbm/main/gbm.h)
   - GLX:
   to create an OpenGL context on GNU/Linux.
   [[Licence]](https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/glx/glxconfig.h)
   - Google Test (Gtest):
   to run unit tests.
   [[Licence]]() **ADD LICENCE URL**  **CHECK WHAT TYPE IT IS**
   - ITK:
   to perform some image filtering tasks.
   [[Apache licence]](https://github.com/InsightSoftwareConsortium/ITK/blob/master/LICENSE)
   - LibTIFF:
   to save TIFF files.
   [[Licence]](https://gitlab.com/libtiff/libtiff/-/blob/master/LICENSE.md)  **CHECK WHAT TYPE IT IS**
   - OpenMP:
   to speed up code running on CPU in parallel, typically the image processing filters and the implicit modelling.
   [Open standard]
   - [OpenSceneGraph](): **ADD URL**
   [[Licence]]() **ADD LICENCE URL**  **CHECK WHAT TYPE IT IS**
   - Sphinx:
   to format the technical documentation generated using Doxygen
   [[Licence]]() **ADD LICENCE URL**  **CHECK WHAT TYPE IT IS**
   - X11:
   to create an OpenGL context on GNU/Linux.
   [[Licence]](https://github.com/mirror/libX11?tab=License-1-ov-file#readme) **CHECK WHAT TYPE IT IS**


## Software Practices <!-- do not delete this heading and write your text below it -->
### Add a short one-two liner subtitle about which software practices are being used and their benefits to the user community

- What practices do you use to develop the software (e.g. version control, contribution policy, licensing, other practices)?
2013 --> 20?? SVN
20?? to date Git

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
