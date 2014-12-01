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


################################################################################
#
#   @file       CMakeLists.txt
#
#   @brief      Configuration file to generate Makefiles, 
#               Project/Solution files for MS VC++, Xcode projects, etc.
#               using the CMake tool. (see http://www.cmake.org/)
#
#   @version    1.0
#
#   @date       01/12/2013
#
#   @author     Dr Franck P. Vidal
#
#   @section    License
#               BSD 3-Clause License.
#
#               For details on use and redistribution please refer
#               to http://opensource.org/licenses/BSD-3-Clause
#
#   @section    Copyright
#               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
#               http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
#               BSD 3-Clause License
#
################################################################################


# Project name #################################################################
project (validation)


# Initialise some states by default ############################################
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../build/version.cmake)
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/config.cmake)


# Minimum CMake version ########################################################
cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)

IF(COMMAND cmake_policy)
       cmake_policy(SET CMP0011 NEW)
ENDIF(COMMAND cmake_policy)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})


# gVirtualXRay ################################################################
find_package(gVirtualXRay REQUIRED)
IF(gVirtualXRay_FOUND)
    INCLUDE_DIRECTORIES(${gVirtualXRay_INCLUDE_DIR})
    LINK_DIRECTORIES(${gVirtualXRay_LIBRARY_DIR})
    SET(requiredLibs ${requiredLibs} ${gVirtualXRay_LIBRARY})
ELSE(gVirtualXRay_FOUND)
    MESSAGE(FATAL_ERROR "gVirtualXRay not found.")
ENDIF(gVirtualXRay_FOUND)


# ITK #########################################################################
FIND_PACKAGE (ITK)
IF (ITK_FOUND)
	ADD_DEFINITIONS(-DHAS_ITK)
	INCLUDE( ${ITK_USE_FILE} )
	set (optionalLibs ${optionalLibs} ${ITK_LIBRARIES})
ELSE(ITK_FOUND)
    MESSAGE(WARNING "ITK not found. The 8-th validation progam cannot be built.")
ENDIF(ITK_FOUND)


# VTK #########################################################################
#FIND_PACKAGE (VTK)
#IF (VTK_FOUND)
#	ADD_DEFINITIONS(-DHAS_VTK)
#	INCLUDE( ${VTK_USE_FILE} )
#	set (optionalLibs ${optionalLibs} ${VTK_LIBRARIES})
#ELSE(VTK_FOUND)
#    MESSAGE(WARNING "VTK not found. It will be disabled.")
#ENDIF(VTK_FOUND)


# Create the RESOURCES ###########################################################
SET (RESOURCES_SOURCES
    shaders/display_gl2.frag
    shaders/display_gl2.vert
    shaders/display_gl3.frag
    shaders/display_gl3.vert
)


INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/CreateHeaderFiles.cmake)
CreateHeaderFiles("." "BinaryValidationFiles" "${RESOURCES_SOURCES}")


# Create the main programs #####################################################
IF(glfw_FOUND)
    ADD_EXECUTABLE (validation_01 src/validation_01.cxx)
    TARGET_LINK_LIBRARIES (validation_01 gVirtualXRay ${optionalLibs} ${requiredLibs})
    INSTALL(TARGETS validation_01 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/bin COMPONENT validation)

    ADD_EXECUTABLE (validation_02 src/validation_02.cxx)
    TARGET_LINK_LIBRARIES (validation_02 gVirtualXRay ${optionalLibs} ${requiredLibs})
    INSTALL(TARGETS validation_02 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/bin COMPONENT validation)

    ADD_EXECUTABLE (validation_03 src/validation_03.cxx)
    TARGET_LINK_LIBRARIES (validation_03 gVirtualXRay ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(validation_03 BinaryValidationFiles)
    INSTALL(TARGETS validation_03 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/bin COMPONENT validation)

    ADD_EXECUTABLE (validation_04 src/validation_04.cxx)
    TARGET_LINK_LIBRARIES (validation_04 gVirtualXRay ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(validation_04 BinaryValidationFiles)
    INSTALL(TARGETS validation_04 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/bin COMPONENT validation)

    ADD_EXECUTABLE (validation_05 src/validation_05.cxx)
    TARGET_LINK_LIBRARIES (validation_05 gVirtualXRay ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(validation_05 BinaryValidationFiles)
    INSTALL(TARGETS validation_05 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/bin COMPONENT validation)

    ADD_EXECUTABLE (validation_06 src/validation_06.cxx)
    TARGET_LINK_LIBRARIES (validation_06 gVirtualXRay ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(validation_06 BinaryValidationFiles)
    INSTALL(TARGETS validation_06 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/bin COMPONENT validation)

    ADD_EXECUTABLE (validation_07 src/validation_07.cxx)
    TARGET_LINK_LIBRARIES (validation_07 gVirtualXRay ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(validation_07 BinaryValidationFiles)
    INSTALL(TARGETS validation_07 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/bin COMPONENT validation)

	IF (ITK_FOUND)
	    ADD_EXECUTABLE (validation_08 src/validation_08.cxx)
	    TARGET_LINK_LIBRARIES (validation_08 gVirtualXRay ${optionalLibs} ${requiredLibs})
	    ADD_DEPENDENCIES(validation_08 BinaryValidationFiles)
	    INSTALL(TARGETS validation_08 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/bin COMPONENT validation)
	ENDIF (ITK_FOUND)

    ADD_EXECUTABLE (validation_09 src/validation_09.cxx)
    TARGET_LINK_LIBRARIES (validation_09 gVirtualXRay ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(validation_09 BinaryValidationFiles)
    INSTALL(TARGETS validation_09 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/bin COMPONENT validation)

    INSTALL(FILES data/cylinder_bin.stl DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/data COMPONENT validation)
ENDIF(glfw_FOUND)




# Set the package name
SET (CPACK_PACKAGE_NAME "gVirtualXRay-validation")

# Handling of shared libraries
#SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS library_name)
#INCLUDE(InstallRequiredSystemLibraries)

# Set the package version
SET(CPACK_PACKAGE_VERSION_MAJOR ${validation_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${validation_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${validation_VERSION_PATCH})
SET(CPACK_PACKAGE_VERSION "${validation_VERSION_MAJOR}.${validation_VERSION_MINOR}.${validation_VERSION_PATCH}")

SET(CPACK_PACKAGE_VENDOR "gVirtualXRay team")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "gVirtualXRay - Installation")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "gVirtualXRay/validation")
    
SET(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE.txt)

INSTALL(FILES ../LICENSE.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation)

SET (CPACK_COMPONENT_validation_DISPLAY_NAME "Demo programs")
SET (CPACK_COMPONENT_validation_DESCRIPTION "All of the demos and example programs using gVirtualXRay")
SET (CPACK_COMPONENT_validation_INSTALL_TYPES Demos Full)


IF(APPLE)
    set(CPACK_BINARY_DRAGNDROP ON)
ENDIF(APPLE)


IF (WIN32)
    SET (CPACK_GENERATOR NSIS)
    SET (CPACK_NSIS_PACKAGE_NAME ${EXAMPLE_PACKAGE_NAME})
    SET (CPACK_NSIS_DISPLAY_NAME ${EXAMPLE_PACKAGE_NAME})
    SET (CPACK_NSIS_CONTACT ${EMAIL})
    SET (CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${EXAMPLE_PACKAGE_NAME}-${EXAMPLE_PROJECT_VERSION}")
    SET (CPACK_NSIS_MENU_LINKS "${EXAMPLE_WEBSITE}" "Homepage for ${EXAMPLE_PACKAGE_NAME}")
ENDIF (WIN32)


IF (NOT CPack_CMake_INCLUDED)
    INCLUDE(CPack)
ENDIF (NOT CPack_CMake_INCLUDED)

