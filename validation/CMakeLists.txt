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


# Project name #################################################################
project (validation)


# Initialise some states by default ############################################
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/version.cmake)


# Configure the project ########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../cmake/configureGlfw.cmake)


# Create the RESOURCES ###########################################################
SET (RESOURCES_SOURCES
    shaders/display_gl2.frag
    shaders/display_gl2.vert
    shaders/display_gl3.frag
    shaders/display_gl3.vert
)


INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/CreateHeaderFiles.cmake)
CreateHeaderFiles("." "BinaryValidationFiles" "${RESOURCES_SOURCES}")
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# Copy data files into the build directory (useful for debugging) #############
IF (WIN32)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/validation_01/)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/validation_02/)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/validation_03/)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/validation_04/)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/validation_05/)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/validation_06/)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/validation_07/)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/validation_08/)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/validation_09/)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/validation_10/)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/validation_11/)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/validation_12/)
    CONFIGURE_FILE (data/cylinder_bin.stl ${CMAKE_CURRENT_BINARY_DIR}/data/cylinder_bin.stl COPYONLY)
    CONFIGURE_FILE (data/validation_10/beam_spectrum.csv ${CMAKE_CURRENT_BINARY_DIR}/data/validation_10/beam_spectrum.csv COPYONLY)
ELSE (WIN32)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../data/validation_01/)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../data/validation_02/)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../data/validation_03/)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../data/validation_04/)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../data/validation_05/)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../data/validation_06/)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../data/validation_07/)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../data/validation_08/)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../data/validation_09/)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../data/validation_10/)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../data/validation_11/)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../data/validation_12/)
    CONFIGURE_FILE (data/cylinder_bin.stl ${CMAKE_CURRENT_BINARY_DIR}/../data/cylinder_bin.stl COPYONLY)
    CONFIGURE_FILE (data/validation_10/beam_spectrum.csv ${CMAKE_CURRENT_BINARY_DIR}/../data/validation_10/beam_spectrum.csv COPYONLY)
ENDIF (WIN32)


# Check if GLFW is enable #####################################################
IF (USE_SYSTEM_GLFW)
    FIND_PACKAGE( glfw3   REQUIRED )
ENDIF (USE_SYSTEM_GLFW)

IF (glfw3_FOUND)
    INCLUDE_DIRECTORIES(${GLFW3_INCLUDE_DIR})
    SET(requiredLibs ${requiredLibs} ${GLFW3_LIBRARY})
ELSE (glfw3_FOUND)
    MESSAGE(WARNING "GLFW3 not found, (${PROJECT_NAME}) will be disable.")
ENDIF (glfw3_FOUND)


# Copy data files into the build directory (useful for debugging) #############
IF (WIN32)
    CONFIGURE_FILE (data/cylinder_bin.stl ${CMAKE_CURRENT_BINARY_DIR}/../data/validation_04/../cylinder_bin.stl COPYONLY)
ELSE (WIN32)
    CONFIGURE_FILE (data/cylinder_bin.stl ${CMAKE_CURRENT_BINARY_DIR}/../data/validation_04/../cylinder_bin.stl COPYONLY)
ENDIF (WIN32)

# Create the main programs #####################################################
IF (glfw3_FOUND)
ADD_EXECUTABLE (validation_01 src/validation_01.cxx)
TARGET_LINK_LIBRARIES (validation_01 gVirtualXRay ${optionalLibs} ${requiredLibs})
INSTALL(TARGETS validation_01 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/bin COMPONENT validation)
INSTALL(DIRECTORY 
		DESTINATION "${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/data/validation_01"
		DIRECTORY_PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_WRITE GROUP_READ GROUP_EXECUTE WORLD_WRITE WORLD_READ WORLD_EXECUTE)

IF (gVirtualXRay_BINARY_DIR)
     ADD_DEPENDENCIES(validation_01   gVirtualXRay)
ENDIF (gVirtualXRay_BINARY_DIR)
    
ADD_EXECUTABLE (validation_02 src/validation_02.cxx)
TARGET_LINK_LIBRARIES (validation_02 gVirtualXRay ${optionalLibs} ${requiredLibs})
INSTALL(TARGETS validation_02 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/bin COMPONENT validation)
INSTALL(DIRECTORY 
		DESTINATION "${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/data/validation_02"
		DIRECTORY_PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_WRITE GROUP_READ GROUP_EXECUTE WORLD_WRITE WORLD_READ WORLD_EXECUTE)
    
IF (gVirtualXRay_BINARY_DIR)
     ADD_DEPENDENCIES(validation_02   gVirtualXRay)
ENDIF (gVirtualXRay_BINARY_DIR)

ADD_EXECUTABLE (validation_03 src/validation_03.cxx)
TARGET_LINK_LIBRARIES (validation_03 gVirtualXRay ${optionalLibs} ${requiredLibs})
ADD_DEPENDENCIES(validation_03 BinaryValidationFiles)
INSTALL(TARGETS validation_03 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/bin COMPONENT validation)
INSTALL(DIRECTORY 
		DESTINATION "${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/data/validation_03"
		DIRECTORY_PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_WRITE GROUP_READ GROUP_EXECUTE WORLD_WRITE WORLD_READ WORLD_EXECUTE)
INSTALL(FILES data/cylinder_bin.stl DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/data COMPONENT validation)

IF (gVirtualXRay_BINARY_DIR)
     ADD_DEPENDENCIES(validation_03   gVirtualXRay)
ENDIF (gVirtualXRay_BINARY_DIR)

ADD_EXECUTABLE (validation_04 src/validation_04.cxx)
TARGET_LINK_LIBRARIES (validation_04 gVirtualXRay ${optionalLibs} ${requiredLibs})
ADD_DEPENDENCIES(validation_04 BinaryValidationFiles)
INSTALL(TARGETS validation_04 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/bin COMPONENT validation)
INSTALL(DIRECTORY 
		DESTINATION "${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/data/validation_04"
		DIRECTORY_PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_WRITE GROUP_READ GROUP_EXECUTE WORLD_WRITE WORLD_READ WORLD_EXECUTE)
    
IF (gVirtualXRay_BINARY_DIR)
     ADD_DEPENDENCIES(validation_04   gVirtualXRay)
ENDIF (gVirtualXRay_BINARY_DIR)

ADD_EXECUTABLE (validation_05 src/validation_05.cxx)
TARGET_LINK_LIBRARIES (validation_05 gVirtualXRay ${optionalLibs} ${requiredLibs})
ADD_DEPENDENCIES(validation_05 BinaryValidationFiles)
INSTALL(TARGETS validation_05 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/bin COMPONENT validation)
INSTALL(DIRECTORY 
		DESTINATION "${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/data/validation_05"
		DIRECTORY_PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_WRITE GROUP_READ GROUP_EXECUTE WORLD_WRITE WORLD_READ WORLD_EXECUTE)

IF (gVirtualXRay_BINARY_DIR)
     ADD_DEPENDENCIES(validation_05   gVirtualXRay)
ENDIF (gVirtualXRay_BINARY_DIR)

ADD_EXECUTABLE (validation_06 src/validation_06.cxx)
TARGET_LINK_LIBRARIES (validation_06 gVirtualXRay ${optionalLibs} ${requiredLibs})
ADD_DEPENDENCIES(validation_06 BinaryValidationFiles)
INSTALL(TARGETS validation_06 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/bin COMPONENT validation)
INSTALL(DIRECTORY 
		DESTINATION "${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/data/validation_06"
		DIRECTORY_PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_WRITE GROUP_READ GROUP_EXECUTE WORLD_WRITE WORLD_READ WORLD_EXECUTE)

IF (gVirtualXRay_BINARY_DIR)
     ADD_DEPENDENCIES(validation_06   gVirtualXRay)
ENDIF (gVirtualXRay_BINARY_DIR)

ADD_EXECUTABLE (validation_07 src/validation_07.cxx)
TARGET_LINK_LIBRARIES (validation_07 gVirtualXRay ${optionalLibs} ${requiredLibs})
ADD_DEPENDENCIES(validation_07 BinaryValidationFiles)
INSTALL(TARGETS validation_07 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/bin COMPONENT validation)
INSTALL(DIRECTORY 
		DESTINATION "${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/data/validation_07"
		DIRECTORY_PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_WRITE GROUP_READ GROUP_EXECUTE WORLD_WRITE WORLD_READ WORLD_EXECUTE)

IF (gVirtualXRay_BINARY_DIR)
     ADD_DEPENDENCIES(validation_07   gVirtualXRay)
ENDIF (gVirtualXRay_BINARY_DIR)

ADD_EXECUTABLE (validation_08 src/validation_08.cxx)
TARGET_LINK_LIBRARIES (validation_08 gVirtualXRay ${optionalLibs} ${requiredLibs})
ADD_DEPENDENCIES(validation_08 BinaryValidationFiles)
INSTALL(TARGETS validation_08 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/bin COMPONENT validation)
INSTALL(DIRECTORY 
        DESTINATION "${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/data/validation_08"
        DIRECTORY_PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_WRITE GROUP_READ GROUP_EXECUTE WORLD_WRITE WORLD_READ WORLD_EXECUTE)

IF (gVirtualXRay_BINARY_DIR)
    ADD_DEPENDENCIES(validation_08   gVirtualXRay)
ENDIF (gVirtualXRay_BINARY_DIR)


ADD_EXECUTABLE (validation_09 src/validation_09.cxx)
TARGET_LINK_LIBRARIES (validation_09 gVirtualXRay ${optionalLibs} ${requiredLibs})
ADD_DEPENDENCIES(validation_09 BinaryValidationFiles)
INSTALL(TARGETS validation_09 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/bin COMPONENT validation)
INSTALL(DIRECTORY 
		DESTINATION "${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/data/validation_09"
		DIRECTORY_PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_WRITE GROUP_READ GROUP_EXECUTE WORLD_WRITE WORLD_READ WORLD_EXECUTE)

IF (gVirtualXRay_BINARY_DIR)
     ADD_DEPENDENCIES(validation_09   gVirtualXRay)
ENDIF (gVirtualXRay_BINARY_DIR)

ADD_EXECUTABLE (validation_10 src/validation_10.cxx)
TARGET_LINK_LIBRARIES (validation_10 gVirtualXRay ${optionalLibs} ${requiredLibs})
ADD_DEPENDENCIES(validation_10 BinaryValidationFiles)
INSTALL(TARGETS validation_10 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/bin COMPONENT validation)
INSTALL(DIRECTORY 
		DESTINATION "${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/data/validation_10"
		DIRECTORY_PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_WRITE GROUP_READ GROUP_EXECUTE WORLD_WRITE WORLD_READ WORLD_EXECUTE)

IF (gVirtualXRay_BINARY_DIR)
    ADD_DEPENDENCIES(validation_10   gVirtualXRay)
ENDIF (gVirtualXRay_BINARY_DIR)


ADD_EXECUTABLE (validation_11 src/validation_11.cxx)
TARGET_LINK_LIBRARIES (validation_11 gVirtualXRay ${optionalLibs} ${requiredLibs})
ADD_DEPENDENCIES(validation_11 BinaryValidationFiles)
INSTALL(TARGETS validation_11 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/bin COMPONENT validation)
INSTALL(DIRECTORY 
                DESTINATION "${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/data/validation_11"
                DIRECTORY_PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_WRITE GROUP_READ GROUP_EXECUTE WORLD_WRITE WORLD_READ WORLD_EXECUTE)

IF (gVirtualXRay_BINARY_DIR)
    ADD_DEPENDENCIES(validation_11   gVirtualXRay)
ENDIF (gVirtualXRay_BINARY_DIR)


ADD_EXECUTABLE (validation_12 src/validation_12.cxx)
TARGET_LINK_LIBRARIES (validation_12 gVirtualXRay ${optionalLibs} ${requiredLibs})
ADD_DEPENDENCIES(validation_12 BinaryValidationFiles)
INSTALL(TARGETS validation_12 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/bin COMPONENT validation)
INSTALL(DIRECTORY
                DESTINATION "${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/validation/data/validation_12"
                DIRECTORY_PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_WRITE GROUP_READ GROUP_EXECUTE WORLD_WRITE WORLD_READ WORLD_EXECUTE)

IF (gVirtualXRay_BINARY_DIR)
    ADD_DEPENDENCIES(validation_12   gVirtualXRay)
ENDIF (gVirtualXRay_BINARY_DIR)


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
ENDIF (glfw3_FOUND)

