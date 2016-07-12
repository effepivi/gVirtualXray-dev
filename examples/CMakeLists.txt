#
#
# Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
# http://www.fpvidal.net/
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
#
# 3. Neither the name of the Bangor University nor the names of its contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


project(gVirtualXRay-demos)
cmake_minimum_required(VERSION 2.6)

INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/version.cmake)

find_package(gVirtualXRay REQUIRED)

OPTION (USE_ITK  "Build the demos that make use of the InsightToolkit (ITK)" OFF)
OPTION (USE_FLTK "Build the demos that make use of the Fast Light Toolkit (FLTK)" OFF)
OPTION (USE_GLUT "Build the demos that make use of the OpenGL Utility Toolkit (GLUT)" OFF)
OPTION (USE_GLFW "Build the demos that make use of GLFW" ON)

add_subdirectory(STL-File)

IF (USE_FLTK)
    add_subdirectory(GUI-FLTK)
ENDIF(USE_FLTK)

IF (USE_ITK)
    add_subdirectory(SimpleBackProjection)
    add_subdirectory(Tomography)
    add_subdirectory(AnatomicalData)
    add_subdirectory(Iso-Surfaces)
ENDIF(USE_ITK)

#IF (USE_GLFW)
#    add_subdirectory(Intersections)
#ENDIF (USE_GLFW)

#add_subdirectory(VisualArts)

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos COMPONENT demos)

