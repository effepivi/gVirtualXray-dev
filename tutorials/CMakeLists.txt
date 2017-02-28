#
#
# Copyright (c) 2017, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


project(gVirtualXRay-tutorials)

if(NOT gVirtualXRay_BINARY_DIR)
    find_package(gVirtualXRay REQUIRED)
    include(${gVirtualXRay_USE_FILE})
else()
    set(gVirtualXRay_DIR ${gVirtualXRay_BINARY_DIR}/CMakeFiles)
    find_package(gVirtualXRay REQUIRED)
endif()


# Copy data files into the build directory (useful for debugging) #############
IF (NOT WIN32)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run.sh ${CMAKE_CURRENT_BINARY_DIR}/run.sh COPYONLY)
ENDIF (NOT WIN32)


IF (USE_FLTK)
    add_subdirectory(tutorial_00_fltk)
    add_subdirectory(tutorial_01_fltk)
    add_subdirectory(tutorial_02_fltk)
    add_subdirectory(tutorial_03_fltk)
    add_subdirectory(tutorial_04_fltk)
    add_subdirectory(tutorial_05_fltk)
    add_subdirectory(tutorial_06_fltk)
    add_subdirectory(tutorial_07_fltk)
    add_subdirectory(tutorial_08_fltk)
ENDIF ()

IF (USE_GLFW)
    add_subdirectory(tutorial_00_glfw)
    add_subdirectory(tutorial_01_glfw)
    add_subdirectory(tutorial_02_glfw)
    add_subdirectory(tutorial_03_glfw)
    #add_subdirectory(tutorial_04_glfw)
    #add_subdirectory(tutorial_05_glfw)
    add_subdirectory(tutorial_06_glfw)
    #add_subdirectory(tutorial_07_glfw)
    #add_subdirectory(tutorial_08_glfw)
ENDIF ()

IF (USE_GLUT)
    add_subdirectory(tutorial_00_glut)
    add_subdirectory(tutorial_01_glut)
    add_subdirectory(tutorial_02_glut)
    add_subdirectory(tutorial_03_glut)
    #add_subdirectory(tutorial_04_glut)
    #add_subdirectory(tutorial_05_glut)
    #add_subdirectory(tutorial_06_glut)
    #add_subdirectory(tutorial_07_glut)
    #add_subdirectory(tutorial_08_glut)
ENDIF ()


IF (USE_QT4)
    add_subdirectory(tutorial_00_qt4)
    add_subdirectory(tutorial_01_qt4)
    add_subdirectory(tutorial_02_qt4)
    add_subdirectory(tutorial_03_qt4)
    add_subdirectory(tutorial_04_qt4)
    add_subdirectory(tutorial_05_qt4)
    add_subdirectory(tutorial_06_qt4)
    add_subdirectory(tutorial_07_qt4)
    add_subdirectory(tutorial_08_qt4)
ENDIF ()

IF (USE_QT5)
    add_subdirectory(tutorial_00_qt5)
    add_subdirectory(tutorial_01_qt5)
    add_subdirectory(tutorial_02_qt5)
    add_subdirectory(tutorial_03_qt5)
    add_subdirectory(tutorial_04_qt5)
    add_subdirectory(tutorial_05_qt5)
    add_subdirectory(tutorial_06_qt5)
    add_subdirectory(tutorial_07_qt5)
    add_subdirectory(tutorial_08_qt5)
ENDIF ()


INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/tutorials COMPONENT tutorials)

