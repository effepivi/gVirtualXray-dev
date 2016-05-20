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


project(gVirtualXRay-tutorial_00_glfw)


# Fix linkage problem with windows and cross-compilation #######################
IF (WIN32)
    ADD_DEFINITIONS(-DGLEW_STATIC)
    ADD_DEFINITIONS(-DFREEGLUT_STATIC)
    SET (CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lwinmm")
ENDIF (WIN32)


# gVirtualXRay #################################################################
FIND_PACKAGE(gVirtualXRay REQUIRED)
IF(gVirtualXRay_FOUND)
    INCLUDE_DIRECTORIES(${gVirtualXRay_INCLUDE_DIR})
    SET(requiredLibs ${requiredLibs} ${gVirtualXRay_LIBRARY})
ELSE(gVirtualXRay_FOUND)
    MESSAGE(FATAL_ERROR "gVirtualXRay not found.")
ENDIF(gVirtualXRay_FOUND)


# OpenGL ######################################################################
FIND_PACKAGE(OpenGL REQUIRED)
IF(OPENGL_FOUND)
    INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR} ${GLU_INCLUDE_PATH})
    SET(requiredLibs ${requiredLibs} ${OPENGL_LIBRARIES})
ELSE(OPENGL_FOUND)
    MESSAGE(FATAL_ERROR "OpenGL not found.")
ENDIF(OPENGL_FOUND)


# GLFW ########################################################################
FIND_PACKAGE(glfw)
IF(glfw_FOUND)
    INCLUDE_DIRECTORIES(${GLFW_INCLUDE_DIR})
    LINK_DIRECTORIES(${GLFW_LIBRARY_DIR})
    SET(optionalLibs ${optionalLibs} ${GLFW_LIBRARY} ${GLFW_LIBRARIES})

    IF (APPLE)
        FIND_LIBRARY(COCOA_LIBRARY Cocoa)
        FIND_LIBRARY(IOKIT_LIBRARY IOKit)
        FIND_LIBRARY(QUARTZ_LIBRARY Quartz)
        FIND_LIBRARY(CORE_VIDEO_LIBRARY CoreVideo)

        SET(optionalLibs ${optionalLibs} ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${QUARTZ_LIBRARY} ${CORE_VIDEO_LIBRARY})
    ENDIF(APPLE)

    IF (UNIX)
        FIND_PACKAGE (X11 REQUIRED)
        iF (X11_FOUND)
            INCLUDE_DIRECTORIES(${X11_INCLUDE_DIRS})
            SET(optionalLibs ${optionalLibs} ${X11_X11_LIB} ${X11_Xrandr_LIB} ${X11_Xinput_LIB} ${X11_Xxf86vm_LIB} ${X11_Xcursor_LIB} ${X11_Xinerama_LIB})
        ELSE (X11_FOUND)
            MESSAGE(FATAL_ERROR "X11 not found.")
        ENDIF (X11_FOUND)
    ENDIF(UNIX)
ELSE(glfw_FOUND)
    MESSAGE(FATAL_ERROR "GLFW not found.")
ENDIF(glfw_FOUND)


# Create the main program ######################################################
add_executable(tutorial_00_glfw tutorial_00_glfw.cxx )
target_link_libraries(tutorial_00_glfw ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})

INSTALL(TARGETS tutorial_00_glfw RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/tutorials/bin  COMPONENT tutorials)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/tutorials COMPONENT tutorials)


