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
#               http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
#               BSD 3-Clause License
#
################################################################################


# Project name #################################################################
project (validation)
set (validation_VERSION_MAJOR 1)
set (validation_VERSION_MINOR 0)
set (validation_VERSION_PATCH 0)


SET (PROJECT_NAME            "gVirtualXRay")
SET (EMAIL                   "franck.p.vidal@fpvidal.net")
SET (validation_WEBSITE         "http://gvirtualxray.sourceforge.net/validation/")
SET (validation_PACKAGE_NAME    "${PROJECT_NAME} -- Validation Programs")
SET (validation_PROJECT_VERSION "${validation_VERSION_MAJOR}.${validation_VERSION_MINOR}.${validation_VERSION_PATCH}")


# Initialise some states by default ############################################
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_BUILD_TYPE Release CACHE STRING "Debug|Release|RelWithDebInfo|MinSizeRel")


# Minimum CMake version ########################################################
cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)

IF(COMMAND cmake_policy)
       cmake_policy(SET CMP0011 NEW)
ENDIF(COMMAND cmake_policy)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})


# gVirtualXRay ################################################################
find_package(gVirtualXRay REQUIRED)
IF(gVirtualXRay_FOUND)
    INCLUDE_DIRECTORIES(${gVirtualXRay_INCLUDE_DIR})
    LINK_DIRECTORIES(${gVirtualXRay_LIBRARY_DIR})
    SET(requiredLibs ${requiredLibs} ${gVirtualXRay_LIBRARY})
ELSE(gVirtualXRay_FOUND)
    MESSAGE(FATAL_ERROR "gVirtualXRay not found.")
ENDIF(gVirtualXRay_FOUND)


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
            SET(optionalLibs ${optionalLibs} ${X11_X11_LIB} ${X11_Xrandr_LIB} ${X11_Xinput_LIB} ${X11_Xxf86vm_LIB})
        ELSE (X11_FOUND)
            MESSAGE(FATAL_ERROR "X11 not found.")
        ENDIF (X11_FOUND)
    ENDIF(UNIX)
ELSE(glfw_FOUND)
    MESSAGE(WARNING "GLFW not found.")
ENDIF(glfw_FOUND)


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
    INSTALL(TARGETS validation_01 RUNTIME DESTINATION validation/bin COMPONENT validation)

    ADD_EXECUTABLE (validation_02 src/validation_02.cxx)
    TARGET_LINK_LIBRARIES (validation_02 gVirtualXRay ${optionalLibs} ${requiredLibs})
    INSTALL(TARGETS validation_02 RUNTIME DESTINATION validation/bin COMPONENT validation)

    ADD_EXECUTABLE (validation_03 src/validation_03.cxx)
    TARGET_LINK_LIBRARIES (validation_03 gVirtualXRay ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(validation_03 BinaryValidationFiles)
    INSTALL(TARGETS validation_03 RUNTIME DESTINATION validation/bin COMPONENT validation)

    ADD_EXECUTABLE (validation_04 src/validation_04.cxx)
    TARGET_LINK_LIBRARIES (validation_04 gVirtualXRay ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(validation_04 BinaryValidationFiles)
    INSTALL(TARGETS validation_04 RUNTIME DESTINATION validation/bin COMPONENT validation)

    ADD_EXECUTABLE (validation_05 src/validation_05.cxx)
    TARGET_LINK_LIBRARIES (validation_05 gVirtualXRay ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(validation_05 BinaryValidationFiles)
    INSTALL(TARGETS validation_05 RUNTIME DESTINATION validation/bin COMPONENT validation)

    ADD_EXECUTABLE (validation_06 src/validation_06.cxx)
    TARGET_LINK_LIBRARIES (validation_06 gVirtualXRay ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(validation_06 BinaryValidationFiles)
    INSTALL(TARGETS validation_06 RUNTIME DESTINATION validation/bin COMPONENT validation)

    INSTALL(FILES data/cylinder_bin.stl DESTINATION validation/data COMPONENT validation)
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

INSTALL(FILES ../LICENSE.txt DESTINATION validation)

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

