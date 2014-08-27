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
#   @date       25/08/2014
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


SET (WEBSITE http://sourceforge.net/projects/gvirtualxray/)


# Initialise some states by default ############################################
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_BUILD_TYPE Release CACHE STRING "Debug|Release|RelWithDebInfo|MinSizeRel")


# Minimum CMake version ########################################################
cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)

IF(COMMAND cmake_policy)
       cmake_policy(SET CMP0011 NEW)
ENDIF(COMMAND cmake_policy)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
#SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../build/modules)


# Project name #################################################################
project (tutorials)
set (tutorials_VERSION_MAJOR 1)
set (tutorials_VERSION_MINOR 0)
set (tutorials_VERSION_PATCH 0)


# Headers of the project #######################################################
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
IF(NOT APPLE)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/OpenGL4)
ENDIF(NOT APPLE)


IF (WIN32)
	ADD_DEFINITIONS(-DGLEW_STATIC)
	ADD_DEFINITIONS(-DFREEGLUT_STATIC)
    SET (CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lwinmm")
ENDIF (WIN32)


# gVirtualXRay ################################################################
find_package(gVirtualXRay REQUIRED)
IF(gVirtualXRay_FOUND)
    INCLUDE_DIRECTORIES(${gVirtualXRay_INCLUDE_DIR})
    SET(requiredLibs ${requiredLibs} ${gVirtualXRay_LIBRARY})
ELSE(gVirtualXRay_FOUND)
    MESSAGE(FATAL_ERROR "gVirtualXRay not found.")
ENDIF(gVirtualXRay_FOUND)


# ZLIB #########################################################################
FIND_PACKAGE(ZLIB REQUIRED)
IF(ZLIB_FOUND)
	INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIRS})
	SET(requiredLibs ${requiredLibs} ${ZLIB_LIBRARIES})
ELSE(ZLIB_FOUND)
	MESSAGE("ZLIB not found.")
ENDIF(ZLIB_FOUND)


# GLEW ########################################################################
FIND_PACKAGE(GLEW REQUIRED)
IF(GLEW_FOUND)
    INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})
    SET(requiredLibs ${requiredLibs} ${GLEW_LIBRARY})
ELSE(GLUT_FOUND)
    MESSAGE(FATAL_ERROR "GLEW not found.")
ENDIF(GLEW_FOUND)


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


# GLUT ########################################################################
FIND_PACKAGE(GLUT)
IF(GLUT_FOUND)
    INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})
    SET(optionalLibs ${optionalLibs} ${GLUT_LIBRARIES})
ELSE(GLUT_FOUND)
    MESSAGE(WARNING "GLUT not found.")
ENDIF(GLUT_FOUND)


# OpenGL ######################################################################
FIND_PACKAGE(OpenGL REQUIRED)
IF(OPENGL_FOUND)
    INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR} ${GLU_INCLUDE_PATH})
    SET(requiredLibs ${requiredLibs} ${OPENGL_LIBRARIES})
ELSE(OPENGL_FOUND)
    MESSAGE(FATAL_ERROR "OpenGL not found.")
ENDIF(OPENGL_FOUND)


# ITK ##########################################################################
IF(USE_ITK)
	FIND_PACKAGE(ITK)
	IF(ITK_FOUND)
		ADD_DEFINITIONS(-DHAS_ITK)
		INCLUDE(${USE_ITK_FILE})
		SET(requiredLibs ${requiredLibs} ${ITK_LIBRARIES})

		IF(ITKVtkGlue_LOADED)
			FIND_PACKAGE(VTK REQUIRED)
			IF(VTK_FOUND)
				INCLUDE(${VTK_USE_FILE})
				LINK_DIRECTORIES(${VTK_LIBRARY_DIRS})
				SET(optionalLibs ${optionalLibs} ${VTK_LIBRARIES})
			ELSE(VTK_FOUND)
				MESSAGE(WARNING "VTK not found.")
			ENDIF(VTK_FOUND)
		ENDIF(ITKVtkGlue_LOADED)
	ELSE(ITK_FOUND)
		MESSAGE(WARNING "ITK not found, tutorial 3 will be disable.")
	ENDIF(ITK_FOUND)
ELSE(USE_ITK)
	MESSAGE(WARNING "ITK is not enable, tutorial 3 will be disable.")
ENDIF(USE_ITK)


# Check if GLUT is enable #####################################################
IF(NOT glfw_FOUND AND NOT GLUT_FOUND)
    MESSAGE(FATAL_ERROR "GLFW and GLUT were not found. No tutorial can be built.")
ENDIF(NOT glfw_FOUND AND NOT GLUT_FOUND)


# Create the RESOURCES ###########################################################
SET (RESOURCES_SOURCES
    shaders/tutorial_02_gl2.frag
    shaders/tutorial_02_gl2.vert
    shaders/tutorial_02_gl3.frag
    shaders/tutorial_02_gl3.vert
    shaders/tutorial_03_gl2.frag
    shaders/tutorial_03_gl2.vert
    shaders/tutorial_03_gl3.frag
    shaders/tutorial_03_gl3.vert
)


FIND_PACKAGE(GZIP REQUIRED)
IF(NOT GZIP_FOUND)
    MESSAGE(FATAL_ERROR "GZIP not found.")
ENDIF(NOT GZIP_FOUND)


INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/CreateHeaderFiles.cmake)
CreateHeaderFiles("." "BinaryFiles" "${RESOURCES_SOURCES}")


# Create the main programs #####################################################
IF(glfw_FOUND)
    ADD_EXECUTABLE (tutorial_00_glfw src/tutorial_00_glfw.cxx)
    TARGET_LINK_LIBRARIES (tutorial_00_glfw ${optionalLibs} ${requiredLibs})

    ADD_EXECUTABLE (tutorial_01_glfw src/tutorial_01_glfw.cxx src/buildCube.cxx)
    TARGET_LINK_LIBRARIES (tutorial_01_glfw ${optionalLibs} ${requiredLibs})

    ADD_EXECUTABLE (tutorial_02_glfw src/tutorial_02_glfw.cxx src/buildCube.cxx)
    TARGET_LINK_LIBRARIES (tutorial_02_glfw ${optionalLibs} ${requiredLibs})

    IF (USE_ITK AND ITK_FOUND)
        ADD_EXECUTABLE (tutorial_03_glfw src/tutorial_03_glfw.cxx src/buildCube.cxx)
        TARGET_LINK_LIBRARIES (tutorial_03_glfw ${optionalLibs} ${requiredLibs})
    ENDIF (USE_ITK AND ITK_FOUND)
ENDIF(glfw_FOUND)


IF(GLUT_FOUND)
    ADD_EXECUTABLE (tutorial_00_glut src/tutorial_00_glut.cxx)
    TARGET_LINK_LIBRARIES (tutorial_00_glut ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

    ADD_EXECUTABLE (tutorial_01_glut src/tutorial_01_glut.cxx src/buildCube.cxx)
    TARGET_LINK_LIBRARIES (tutorial_01_glut ${optionalLibs} ${requiredLibs})

    ADD_EXECUTABLE (tutorial_02_glut src/tutorial_02_glut.cxx src/buildCube.cxx)
    TARGET_LINK_LIBRARIES (tutorial_02_glut ${optionalLibs} ${requiredLibs})

    IF (USE_ITK AND ITK_FOUND)
        ADD_EXECUTABLE (tutorial_03_glut src/tutorial_03_glut.cxx src/buildCube.cxx)
        TARGET_LINK_LIBRARIES (tutorial_03_glut ${optionalLibs} ${requiredLibs})
    ENDIF (USE_ITK AND ITK_FOUND)
ENDIF(GLUT_FOUND)


# Make sure the source is generated before the executable builds.
IF(glfw_FOUND)
    ADD_DEPENDENCIES(tutorial_02_glfw BinaryFiles)

    IF (USE_ITK AND ITK_FOUND)
        ADD_DEPENDENCIES(tutorial_03_glfw BinaryFiles)
    ENDIF (USE_ITK AND ITK_FOUND)
ENDIF(glfw_FOUND)


IF(GLUT_FOUND)
    ADD_DEPENDENCIES(tutorial_02_glut BinaryFiles)

    IF (USE_ITK AND ITK_FOUND)
        ADD_DEPENDENCIES(tutorial_03_glut BinaryFiles)
    ENDIF (USE_ITK AND ITK_FOUND)
ENDIF(GLUT_FOUND)

# Set the package name
SET (CPACK_PACKAGE_NAME "gVirtualXRay-tutorials")

# Handling of shared libraries
#SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS library_name)
#INCLUDE(InstallRequiredSystemLibraries)

# Set the package version
SET(CPACK_PACKAGE_VERSION_MAJOR ${tutorials_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${tutorials_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${tutorials_VERSION_PATCH})
SET(CPACK_PACKAGE_VERSION "${tutorials_VERSION_MAJOR}.${tutorials_VERSION_MINOR}.${tutorials_VERSION_PATCH}")

SET(CPACK_PACKAGE_VENDOR "gVirtualXRay team")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "gVirtualXRay - Installation")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "gVirtualXRay/tutorials")
    
SET(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE.txt)

IF(glfw_FOUND)
    INSTALL(TARGETS tutorial_01_glfw tutorial_02_glfw RUNTIME DESTINATION bin COMPONENT tutorials)

    IF (USE_ITK AND ITK_FOUND)
        INSTALL(TARGETS tutorial_03_glfw RUNTIME DESTINATION bin COMPONENT tutorials)
    ENDIF (USE_ITK AND ITK_FOUND)
ENDIF(glfw_FOUND)


IF(GLUT_FOUND)
    INSTALL(TARGETS tutorial_01_glut tutorial_02_glut RUNTIME DESTINATION bin COMPONENT tutorials)

    IF (USE_ITK AND ITK_FOUND)
        INSTALL(TARGETS tutorial_03_glut RUNTIME DESTINATION bin COMPONENT tutorials)
    ENDIF (USE_ITK AND ITK_FOUND)
ENDIF(GLUT_FOUND)


INSTALL(FILES ../LICENSE.txt DESTINATION .)

IF(APPLE)
    set(CPACK_BINARY_DRAGNDROP ON)
ENDIF(APPLE)

include(CPack)

