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


project(gVirtualXRay-tutorial_02_glut)


# Create the RESOURCES ###########################################################
SET (RESOURCES_SOURCES
    tutorial_02_gl2.frag
    tutorial_02_gl2.vert
)

INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../CreateHeaderFiles.cmake)

CreateHeaderFiles("." "Binarytutorial_02_glutFiles" "${RESOURCES_SOURCES}")


# Headers of the project #######################################################
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})


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


# GLUT ########################################################################
IF (USE_SYSTEM_GLUT)
    FIND_PACKAGE( GLUT   REQUIRED )
ENDIF (USE_SYSTEM_GLUT)

IF(GLUT_FOUND)
    INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})
    SET(optionalLibs ${optionalLibs} ${GLUT_LIBRARIES})
ELSE(GLUT_FOUND)
    MESSAGE(WARNING "GLUT not found, the tutorial (${PROJECT_NAME}) will not be built.")
ENDIF(GLUT_FOUND)


# Create the main program ######################################################
IF(GLUT_FOUND)
    add_executable(tutorial_02_glut tutorial_02_glut.cxx buildCube.cxx)
    target_link_libraries(tutorial_02_glut ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(tutorial_02_glut Binarytutorial_02_glutFiles)

    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../doc/TUTORIALS/tutorial-02-glut/tutorial-02-glut.pdf 
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/tutorials
        COMPONENT tutorials)
    INSTALL(TARGETS tutorial_02_glut RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/tutorials/bin  COMPONENT tutorials)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/tutorials COMPONENT tutorials)

    IF (gVirtualXRay_BINARY_DIR)
        ADD_DEPENDENCIES(tutorial_02_glut   gVirtualXRay)
    ENDIF (gVirtualXRay_BINARY_DIR)

ELSE(GLUT_FOUND)
    MESSAGE(WARNING "GLUT not found, ${PROJECT_NAME} will not be built.")
ENDIF(GLUT_FOUND)

