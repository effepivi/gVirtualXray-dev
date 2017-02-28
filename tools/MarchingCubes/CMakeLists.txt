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


PROJECT (gvxrMarchingCubes)

# Initialise some states by default ############################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/version.cmake)


# Configure the project ########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/configureGlfw.cmake)


# Headers of the project #######################################################
INCLUDE_DIRECTORIES (${CMAKE_CURRENT_BINARY_DIR})


# Check if GLFW is enable #####################################################
IF (glfw3_FOUND)
    ADD_DEFINITIONS(-DHAS_GLFW)
ELSE (glfw3_FOUND)
    MESSAGE (WARNING "GLFW3 not found, the tool (${PROJECT_NAME}) will be disable.")
ENDIF (glfw3_FOUND)


# Create the RESOURCES ###########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/cmake/CreateHeaderFiles.cmake)
SET (RESOURCES_SOURCES display_gl2.vert display_gl2.frag display_gl3.vert display_gl3.frag)
CreateHeaderFiles("." "BinaryMarchingCubesFiles" "${RESOURCES_SOURCES}")


# Create the main program ######################################################
ADD_EXECUTABLE (gvxrMarchingCubes gvxrMarchingCubes.cxx cmdline.h cmdline.c)
TARGET_LINK_LIBRARIES (gvxrMarchingCubes ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})
ADD_DEPENDENCIES(gvxrMarchingCubes BinaryMarchingCubesFiles)

IF (gVirtualXRay_BINARY_DIR)
    ADD_DEPENDENCIES (gvxrMarchingCubes   gVirtualXRay)
ENDIF (gVirtualXRay_BINARY_DIR)

INSTALL (TARGETS gvxrMarchingCubes RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/bin  COMPONENT tools)
INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/bin/ COMPONENT tools)

SET (CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt)

INCLUDE (CPack)

