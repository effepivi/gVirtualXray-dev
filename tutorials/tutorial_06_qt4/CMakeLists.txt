#
#
# Copyright (c) 2064, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


project(gVirtualXRay-tutorial_06_qt4)


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


# Qt4 #########################################################################
IF (WIN32)
    # 64 bit version of VC++
    if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
        MESSAGE (WARNING "There is no 64 bit version of Qt4 for MSVC++.")
    else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
        FIND_PACKAGE(Qt4 COMPONENTS QtGui QtOpenGL)
    endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )
else (WIN32)
    FIND_PACKAGE(Qt4 COMPONENTS QtGui QtOpenGL)
endif (WIN32)

IF(Qt4_FOUND)
    SET(CMAKE_AUTOMOC ON)
    SET(CMAKE_INCLUDE_CURRENT_DIR ON)
    SET(optionalLibs ${optionalLibs} Qt4::QtGui Qt4::QtOpenGL)
ELSE(Qt4_FOUND)
    MESSAGE(WARNING "Qt4 not found, the demo (${PROJECT_NAME}) will be disable.")
ENDIF(Qt4_FOUND)


IF(Qt4_FOUND)
    # Create the main program ######################################################
    add_executable(tutorial_06_qt4 tutorial_06_qt4.cxx buildCube.cxx)
    target_link_libraries(tutorial_06_qt4 ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})

    INSTALL(TARGETS tutorial_06_qt4 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/tutorials/bin  COMPONENT tutorials)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/tutorials COMPONENT tutorials)

    IF (gVirtualXRay_BINARY_DIR)
        ADD_DEPENDENCIES(tutorial_06_qt4   gVirtualXRay)
    ENDIF (gVirtualXRay_BINARY_DIR)
ENDIF(Qt4_FOUND)
