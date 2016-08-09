#
#
# Copyright (c) 2016, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


project(gVirtualXRay-tutorial_01_qt5)


# Set new policy ###############################################################
cmake_policy(SET CMP0020 NEW)


IF (WIN32)
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


# Qt5 #########################################################################
find_package(Qt5Widgets)
find_package(Qt5OpenGL)

IF(Qt5Widgets_FOUND AND Qt5OpenGL_FOUND)
    SET(CMAKE_AUTOMOC ON)
    SET(CMAKE_INCLUDE_CURRENT_DIR ON)
    SET(optionalLibs ${optionalLibs} Qt5::Widgets Qt5::OpenGL)

    # Create the main program ######################################################
    add_executable(tutorial_01_qt5 tutorial_01_qt5.cxx buildCube.cxx)
    target_link_libraries(tutorial_01_qt5 ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})

    INSTALL(TARGETS tutorial_01_qt5 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/tutorials/bin  COMPONENT tutorials)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/tutorials COMPONENT tutorials)

    IF (gVirtualXRay_BINARY_DIR)
        ADD_DEPENDENCIES(tutorial_01_qt5   gVirtualXRay)
    ENDIF (gVirtualXRay_BINARY_DIR)

ELSE(Qt5Widgets_FOUND AND Qt5OpenGL_FOUND)
    MESSAGE(WARNING "Qt5 not found, ${PROJECT_NAME} will not be built.")
ENDIF(Qt5Widgets_FOUND AND Qt5OpenGL_FOUND)
