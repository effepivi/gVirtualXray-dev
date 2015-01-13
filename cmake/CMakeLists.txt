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
PROJECT(gVirtualXRay)

INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/version.cmake)

# Initialise some states by default ############################################
set(BUILD_SHARED_LIBS OFF)


# Minimum CMake version ########################################################
IF(WIN32)
	cmake_minimum_required(VERSION 2.6.0)
ENDIF(WIN32)

IF(COMMAND cmake_policy)
       cmake_policy(SET CMP0003 NEW)
       cmake_policy(SET CMP0013 NEW)
ENDIF(COMMAND cmake_policy)


# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()


SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})


# Make include files in paths like /usr/local/include have preference before
# files in OSX frameworks. 
SET(CMAKE_FIND_FRAMEWORK "LAST")


# Enable all compiler warnings #################################################
#IF(NOT WIN32)
#	add_definitions( -Wall )
#ENDIF(NOT WIN32)


# Libraries ####################################################################
SET(optionalLibs)
SET(requiredLibs)

SET (gVirtualXRay_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET (gVirtualXRay_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
SET (gVirtualXRay_LIBRARY ${PROJECT_BINARY_DIR}/libgVirtualXRay.a)


# Library source code ##########################################################
SET (CXX_SOURCES
	# gVirtualXRay
	../src/gVirtualXRay/XRayDetector.cxx
    ../src/gVirtualXRay/XRayRenderer.cxx

	# Graphics
	../src/Graphics/CubeMesh.cxx
	../src/Graphics/framebufferObject.cpp
	../src/Graphics/ImplicitSurface.cxx
	../src/Graphics/OpenGL2VBO.cxx
	../src/Graphics/OpenGL3VBO.cxx
	../src/Graphics/PolygonMesh.cxx
	../src/Graphics/Shader.cxx
	../src/Graphics/SphereMesh.cxx
	../src/Graphics/StereoHelper.cxx
	../src/Graphics/TextRenderer.cxx
	../src/Graphics/VBO.cxx

    # Utilities
	../src/Utilities/AtomicElement.cxx
	../src/Utilities/ElementSet.cxx
	../src/Utilities/FBOException.cxx
	../src/Utilities/MaterialSet.cxx
	../src/Utilities/OpenGLException.cxx
	../src/Utilities/OpenGLUtilities.cxx
	../src/Utilities/TissueMaterial.cxx
	../src/Utilities/Utilities.cxx
)


# Add header files to the project
SET (HXX_SOURCES
	# gVirtualXRay
	${CMAKE_CURRENT_BINARY_DIR}/gVirtualXRay/gVirtualXRayConfig.h

	../include/gVirtualXRay/AttenuationCoefficient.h
	../include/gVirtualXRay/AttenuationCoefficient.inl
	../include/gVirtualXRay/SpectrumRecord.h
	../include/gVirtualXRay/SpectrumRecord.inl
	../include/gVirtualXRay/XRayBeam.h
	../include/gVirtualXRay/XRayBeam.inl
	../include/gVirtualXRay/XRayDetector.h
	../include/gVirtualXRay/XRayDetector.inl
	../include/gVirtualXRay/XRayRenderer.h
	../include/gVirtualXRay/XRayRenderer.inl

	# Graphics
	../include/gVirtualXRay/CubeMesh.h
	../include/gVirtualXRay/CubeMesh.inl
	../include/gVirtualXRay/framebufferObject.h
	../include/gVirtualXRay/ImplicitSurface.h
   	../include/gVirtualXRay/ImplicitSurface.inl
	../include/gVirtualXRay/Material.h
    ../include/gVirtualXRay/Material.inl
	../include/gVirtualXRay/OpenGL2VBO.h
	../include/gVirtualXRay/OpenGL2VBO.inl
	../include/gVirtualXRay/OpenGL3VBO.h
	../include/gVirtualXRay/OpenGL3VBO.inl
	../include/gVirtualXRay/PolygonMesh.h
	../include/gVirtualXRay/PolygonMesh.inl
	../include/gVirtualXRay/Shader.h
	../include/gVirtualXRay/Shader.inl
	../include/gVirtualXRay/StereoHelper.h
	../include/gVirtualXRay/StereoHelper.inl
	../include/gVirtualXRay/SphereMesh.h
	../include/gVirtualXRay/SphereMesh.inl
	../include/gVirtualXRay/VBO.h
	../include/gVirtualXRay/VBO.inl
	../include/gVirtualXRay/TextRenderer.h
	../include/gVirtualXRay/TextRenderer.inl

	# Math
	../include/gVirtualXRay/Matrix4x4.h
	../include/gVirtualXRay/Matrix4x4.inl
	../include/gVirtualXRay/Vec2.h
	../include/gVirtualXRay/Vec2.inl
	../include/gVirtualXRay/Vec3.h
	../include/gVirtualXRay/Vec3.inl

	# Utilities
	../include/gVirtualXRay/AtomicElement.h
	../include/gVirtualXRay/AtomicElement.inl
	../include/gVirtualXRay/ConstantValues.h
	../include/gVirtualXRay/ElementSet.h
	../include/gVirtualXRay/ElementSet.inl
	../include/gVirtualXRay/Exception.h
	../include/gVirtualXRay/Exception.inl
	../include/gVirtualXRay/FBOException.h
	../include/gVirtualXRay/FileDoesNotExistException.h
	../include/gVirtualXRay/FileDoesNotExistException.inl
	../include/gVirtualXRay/MaterialSet.h
	../include/gVirtualXRay/MaterialSet.inl
	../include/gVirtualXRay/OpenGLException.h
	../include/gVirtualXRay/OpenGLUtilities.h
	../include/gVirtualXRay/OpenGLUtilities.inl
	../include/gVirtualXRay/OutOfBoundsException.h
	../include/gVirtualXRay/OutOfBoundsException.inl
	../include/gVirtualXRay/OutOfMemoryException.h
	../include/gVirtualXRay/OutOfMemoryException.inl
	../include/gVirtualXRay/TissueMaterial.h
	../include/gVirtualXRay/TissueMaterial.inl
	../include/gVirtualXRay/Types.h
	../include/gVirtualXRay/Units.h
	../include/gVirtualXRay/Utilities.h
)
SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/../include)


# Add source to the project ####################################################
IF(WIN32)
	IF(NOT CYGWIN)
		SET(SOURCES ${CXX_SOURCES} ${HXX_SOURCES})
	ELSE(NOT CYGWIN)
		SET(SOURCES ${CXX_SOURCES})
	ENDIF(NOT CYGWIN)
ELSE(WIN32)
	IF(APPLE)
		SET(SOURCES ${CXX_SOURCES} ${HXX_SOURCES})
	ELSE(APPLE)
		SET(SOURCES ${CXX_SOURCES})
	ENDIF(APPLE)
ENDIF(WIN32)


# Headers of the project #######################################################
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../include)
IF (NOT APPLE)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../include/OpenGL4)
ENDIF(NOT APPLE)
#INCLUDE_DIRECTORIES(/usr/local/include)


# GZIP ########################################################################
FIND_PACKAGE(GZIP REQUIRED)
IF(NOT GZIP_FOUND)
	MESSAGE(FATAL_ERROR "GZIP not found.")
ENDIF(NOT GZIP_FOUND)


# OpenGL ######################################################################
FIND_PACKAGE(OpenGL REQUIRED)
IF(OPENGL_FOUND)
    INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR} ${GLU_INCLUDE_PATH})
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${GLU_INCLUDE_PATH})
    SET(requiredLibs ${requiredLibs} ${OPENGL_LIBRARIES} ${GLU_LIBRARY})
ELSE(OPENGL_FOUND)
    MESSAGE(FATAL_ERROR "OpenGL not found.")
ENDIF(OPENGL_FOUND)


FIND_PACKAGE(GLEW REQUIRED)
IF(GLEW_FOUND)
    ADD_DEFINITIONS(-DGLEW_STATIC)

    SET(HAS_GLEW ON)
    ADD_DEFINITIONS(-DHAS_GLEW)
    
    INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} ${GLEW_INCLUDE_DIR})
    LINK_DIRECTORIES(${GLEW_LIBRARY_DIR})
    SET (gVirtualXRay_LIBRARY_DIRS ${gVirtualXRay_LIBRARY_DIRS} ${GLEW_LIBRARY_DIR})
    SET(requiredLibs ${requiredLibs} ${GLEW_LIBRARY})
ELSE(GLEW_FOUND)
    SET(HAS_GLEW OFF)
    MESSAGE(FATAL_ERROR "GLEW not found.")
ENDIF(GLEW_FOUND)


# BOOST #######################################################################
FIND_PACKAGE(Boost QUIET REQUIRED)
if(Boost_FOUND)
	SET( Boost_USE_STATIC_LIBS OFF )
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
	SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})
	LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
	SET (gVirtualXRay_LIBRARY_DIRS ${gVirtualXRay_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS})
	SET(requiredLibs ${requiredLibs} ${Boost_LIBRARIES})
ELSE(Boost_FOUND)
	MESSAGE(FATAL_ERROR "Boost not found.")
ENDIF(Boost_FOUND)


# FreeImage ####################################################################
#FIND_PACKAGE(FreeImage)
#IF(FREEIMAGE_FOUND)
#	SET(HAS_FREEIMAGE ON)
#	ADD_DEFINITIONS(-DHAS_FREEIMAGE)
#	INCLUDE_DIRECTORIES(${FREEIMAGE_INCLUDE_DIR})
#	SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} ${FREEIMAGE_INCLUDE_DIR})
#	SET(optionalLibs ${optionalLibs} ${FREEIMAGE_LIBRARY})
#ELSE(FREEIMAGE_FOUND)
#	MESSAGE("FREEIMAGE not found.")
	SET(HAS_FREEIMAGE OFF)
#ENDIF(FREEIMAGE_FOUND)


# ITK ##########################################################################
FIND_PACKAGE(ITK)
IF(ITK_FOUND)
	SET (HAS_ITK ON)
	ADD_DEFINITIONS(-DHAS_ITK)
	INCLUDE(${USE_ITK_FILE})
	SET(optionalLibs ${optionalLibs} ${ITK_LIBRARIES})

	IF(ITKVtkGlue_LOADED)
		FIND_PACKAGE(VTK REQUIRED)
		IF(VTK_FOUND)
			SET (HAS_VTK ON)
			ADD_DEFINITIONS(-DHAS_VTK)
			INCLUDE(${VTK_USE_FILE})
			LINK_DIRECTORIES(${VTK_LIBRARY_DIRS})
			SET (gVirtualXRay_LIBRARY_DIRS ${gVirtualXRay_LIBRARY_DIRS} ${VTK_LIBRARY_DIRS})
			SET(optionalLibs ${optionalLibs} ${VTK_LIBRARIES})
		ELSE(VTK_FOUND)
			SET (HAS_VTK OFF)
			MESSAGE(WARNING "VTK not found.")
		ENDIF(VTK_FOUND)
	ELSE(ITKVtkGlue_LOADED)
		SET (HAS_VTK OFF)
	ENDIF(ITKVtkGlue_LOADED)
ELSE(ITK_FOUND)
	MESSAGE(WARNING "ITK not found.")
	SET (HAS_ITK OFF)
ENDIF(ITK_FOUND)


# ZLIB #########################################################################
FIND_PACKAGE(ZLIB REQUIRED)
IF(ZLIB_FOUND)
    INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIRS})
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS})
    SET(requiredLibs ${requiredLibs} ${ZLIB_LIBRARIES})
ELSE(ZLIB_FOUND)
    MESSAGE("ZLIB not found.")
ENDIF(ZLIB_FOUND)


# FREETYPE ####################################################################
FIND_PACKAGE(Freetype)
IF(FREETYPE_FOUND)
    SET (HAS_FREETYPE ON)
    ADD_DEFINITIONS(-DHAS_FREETYPE)
    INCLUDE_DIRECTORIES(${FREETYPE_INCLUDE_DIRS})
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS})
    SET(optionalLibs ${optionalLibs} ${FREETYPE_LIBRARIES})

    FIND_PACKAGE(PNG)
    IF (PNG_FOUND)
        ADD_DEFINITIONS(${PNG_DEFINITIONS})
        INCLUDE_DIRECTORIES(${PNG_INCLUDE_DIRS})
        SET(optionalLibs ${optionalLibs} ${PNG_LIBRARIES})
    ELSE (PNG_FOUND)
        MESSAGE(WARNING "PNG not found.")
    ENDIF (PNG_FOUND)

    FIND_PACKAGE(BZip2)
    IF (BZIP2_FOUND)
        INCLUDE_DIRECTORIES(${BZIP2_INCLUDE_DIR})
        SET(optionalLibs ${optionalLibs} ${BZIP2_LIBRARIES})
    ELSE (BZIP2_FOUND)
        MESSAGE(WARNING "BZip2 not found.")
    ENDIF (BZIP2_FOUND)

ELSE(FREETYPE_FOUND)
    SET (HAS_FREETYPE OFF)
    MESSAGE(WARNING "Freetype not found.")
ENDIF(FREETYPE_FOUND)


# XCOM Database ################################################################

# XCOM: Photon Cross Sections Database

# http://www.nist.gov/pml/data/xcom/

# http://physics.nist.gov/PhysRefData/Xcom/Text/download.html


find_path (XCOM_PATH MDATX3.001 MDATX3.100 NO_DEFAULT_PATH)
IF (NOT XCOM_PATH)
	MESSAGE(WARNING "XCOM, Photon Cross Sections Database, not found. Its use will be disable. You can find it on the NIST's website at: http://physics.nist.gov/PhysRefData/Xcom/Text/download.html")
ENDIF(NOT XCOM_PATH)


# configure a header file to pass some of the CMake settings
# to the source code
IF (XCOM_PATH)
    SET (HAS_XCOM ON)
    configure_file (
        "${CMAKE_CURRENT_SOURCE_DIR}/gVirtualXRayConfigXCOM.h.in"
        "${PROJECT_BINARY_DIR}/gVirtualXRay/gVirtualXRayConfig.h"
    }})
ELSE (XCOM_PATH)
    SET (HAS_XCOM OFF)
    configure_file (
        "${CMAKE_CURRENT_SOURCE_DIR}/gVirtualXRayConfigNoXCOM.h.in"
        "${PROJECT_BINARY_DIR}/gVirtualXRay/gVirtualXRayConfig.h"
    )
ENDIF (XCOM_PATH)


# Create the library ###########################################################
ADD_LIBRARY (gVirtualXRay STATIC ${SOURCES})


# Create the RESOURCES ###########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/CreateHeaderFiles.cmake)

SET (RESOURCES_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/clean_l_buffer_gl2.frag
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/clean_l_buffer_gl2.vert
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/clean_l_buffer_gl3.frag
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/clean_l_buffer_gl3.vert
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/cumulated_l_buffer_gl2.frag
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/cumulated_l_buffer_gl2.vert
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/cumulated_l_buffer_gl3.frag
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/cumulated_l_buffer_gl3.vert
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/l_buffer_gl2.frag
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/l_buffer_gl2.vert
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/l_buffer_gl3.frag
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/l_buffer_gl3.vert
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/MUx_Dx_gl2.frag
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/MUx_Dx_gl2.vert
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/MUx_Dx_gl3.frag
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/MUx_Dx_gl3.vert
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/outer_surface_l_buffer_gl2.frag
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/outer_surface_l_buffer_gl2.vert
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/outer_surface_l_buffer_gl3.frag
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/outer_surface_l_buffer_gl3.vert
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/xray_attenuation_gl2.frag
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/xray_attenuation_gl2.vert
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/xray_attenuation_gl3.frag
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/xray_attenuation_gl3.vert
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/xray_display_gl2.frag
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/xray_display_gl2.vert
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/xray_display_gl3.frag
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/xray_display_gl3.vert
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/correlation_CT_number_and_tissue_material.csv
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/mu_water.csv
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/element.csv
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/text_display_gl2.frag
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/text_display_gl2.vert
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/text_display_gl3.frag
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/text_display_gl3.vert
	${CMAKE_CURRENT_SOURCE_DIR}/../resources/FreeSans.ttf
)
CreateHeaderFiles("." "BinaryLibFiles" "${RESOURCES_SOURCES}")


IF (XCOM_PATH)
    SET (XCOM_SOURCES
        ${XCOM_PATH}/MDATX3.001
        ${XCOM_PATH}/MDATX3.006
        ${XCOM_PATH}/MDATX3.007
        ${XCOM_PATH}/MDATX3.008
        ${XCOM_PATH}/MDATX3.011
        ${XCOM_PATH}/MDATX3.012
        ${XCOM_PATH}/MDATX3.015
        ${XCOM_PATH}/MDATX3.016
        ${XCOM_PATH}/MDATX3.017
        ${XCOM_PATH}/MDATX3.018
        ${XCOM_PATH}/MDATX3.019
        ${XCOM_PATH}/MDATX3.020
        ${XCOM_PATH}/MDATX3.026
        ${XCOM_PATH}/MDATX3.022
        ${XCOM_PATH}/MDATX3.029
        ${XCOM_PATH}/MDATX3.030
        ${XCOM_PATH}/MDATX3.047
        ${XCOM_PATH}/MDATX3.050
    )

	CreateHeaderFiles("." "XCOMLibFiles" "${XCOM_SOURCES}")
ENDIF (XCOM_PATH)


# Linkage of the target ########################################################
set_target_properties(gVirtualXRay PROPERTIES LINK_SEARCH_START_STATIC ON)
TARGET_LINK_LIBRARIES (gVirtualXRay ${requiredLibs} ${optionalLibs})
SET (gVirtualXRay_LIBRARIES ${requiredLibs} ${optionalLibs} gVirtualXRay)


# Make sure the source is generated before the executable builds.
ADD_DEPENDENCIES(gVirtualXRay BinaryLibFiles)

IF (XCOM_PATH)
    ADD_DEPENDENCIES(gVirtualXRay XCOMLibFiles)
ENDIF(XCOM_PATH)


# Generate gVirtualXRayConfig.cmake for the install tree #######################
SET (gVirtualXRay_INSTALL_PACKAGE_DIR "lib/cmake/${PROJECT_NAME}-${gVirtualXRay_VERSION_MAJOR}.${gVirtualXRay_VERSION_MINOR}")
SET (gVirtualXRay_USE_FILE "${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/UsegVirtualXRay.cmake")
SET (gVirtualXRay_INCLUDE_DIRS "${gVirtualXRay_INCLUDE_DIRS} ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/library/include/")
SET (gVirtualXRay_LIBRARY_DIRS "${gVirtualXRay_LIBRARY_DIRS} ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/library/lib")
CONFIGURE_FILE (${CMAKE_CURRENT_SOURCE_DIR}/gVirtualXRayConfig.cmake.in ${gVirtualXRay_BINARY_DIR}/CMakeFiles/gVirtualXRayConfig.cmake @ONLY)
CONFIGURE_FILE (${CMAKE_CURRENT_SOURCE_DIR}/UsegVirtualXRay.cmake.in ${gVirtualXRay_BINARY_DIR}/CMakeFiles/UsegVirtualXRay.cmake @ONLY)
INSTALL(FILES ${gVirtualXRay_BINARY_DIR}/CMakeFiles/gVirtualXRayConfig.cmake DESTINATION ${gVirtualXRay_INSTALL_PACKAGE_DIR})
INSTALL(FILES ${gVirtualXRay_BINARY_DIR}/CMakeFiles/UsegVirtualXRay.cmake    DESTINATION ${gVirtualXRay_INSTALL_PACKAGE_DIR})
INSTALL(FILES ${gVirtualXRay_BINARY_DIR}/CMakeFiles/UsegVirtualXRay.cmake    DESTINATION "${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}")

# Doxygen documentation ########################################################
IF(WIN32)
	FIND_PACKAGE(Doxygen) 
	IF (DOXYGEN_FOUND)
		ADD_CUSTOM_TARGET(html-doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/../doc/Doxyfile) 
	ELSE (DOXYGEN_FOUND)
    	MESSAGE(STATUS "WARNING: Doxygen not found - Reference manual will not be created")
	ENDIF (DOXYGEN_FOUND)
ELSE(WIN32)
	ADD_CUSTOM_TARGET(html-doc doxygen ${CMAKE_CURRENT_SOURCE_DIR}/../doc/Doxyfile) 
ENDIF(WIN32)


# Examples #####################################################################
option(BUILD_EXAMPLES "Build examples." ON)
IF (BUILD_EXAMPLES)
   	MESSAGE(STATUS "Build examples.")

	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../examples ${CMAKE_CURRENT_BINARY_DIR}/examples_bin)
ELSE (BUILD_EXAMPLES)
   	MESSAGE("Do not build examples.")
ENDIF (BUILD_EXAMPLES)


# Tutorials ####################################################################
option(BUILD_TUTORIALS "Build tutorials." ON)
IF (BUILD_TUTORIALS)
   	MESSAGE(STATUS "Build tutorials.")

	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../tutorials ${CMAKE_CURRENT_BINARY_DIR}/tutorials_bin)
ELSE (BUILD_TUTORIALS)
   	MESSAGE("Do not build tutorials.")
ENDIF (BUILD_TUTORIALS)


# Unit tests ###################################################################
option(BUILD_TESTS "Build unit tests." OFF)
IF (BUILD_TESTS)
	enable_testing ()
   	
	MESSAGE(STATUS "Build unit tests.")

	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../test ${CMAKE_CURRENT_BINARY_DIR}/test_bin)
ELSE (BUILD_TESTS)
   	MESSAGE("Do not build unit tests.")
ENDIF (BUILD_TESTS)


# Installation #################################################################
INSTALL (TARGETS gVirtualXRay ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/library/lib                  COMPONENT libraries)
INSTALL (FILES ${HXX_SOURCES}         DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/library/include/gVirtualXRay COMPONENT headers)
INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/library     COMPONENT libraries)
INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR})


SET (CPACK_PACKAGE_FILE_NAME gVirtualXRay)
SET (CPACK_PACKAGE_ICON ${CMAKE_CURRENT_SOURCE_DIR}/doc/logo.ico)

IF (APPLE)
	SET (CPACK_BUNDLE_NAME gVirtualXRay)
	SET (CPACK_BUNDLE_ICON ${CMAKE_CURRENT_SOURCE_DIR}/examples/build/DragonDemo.icns)
	#SET (CPACK_BUNDLE_PLIST - path to a file that will become the bundle plist.
	#SET (CPACK_BUNDLE_STARTUP_COMMAND - path to a file that will be executed when the user opens the bundle. Could be a shell-script or a binary. 
ENDIF(APPLE)


SET(CPACK_SOURCE_IGNORE_FILES "~$")
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../cmake     cmake)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../include   include)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../src       src)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../resources resources)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../examples  examples)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../tutorials tutorials)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../doc/dist_doc1    .)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../doc/dist_doc2   doc)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../doc/html        doc/html)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../doc/TUTORIALS   doc/tutorials)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../doc/screenshots doc/screenshots)

SET (CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "Libraries")
SET (CPACK_COMPONENT_HEADERS_DISPLAY_NAME "C++ Headers")

SET (CPACK_COMPONENT_LIBRARIES_DESCRIPTION "Static libraries used to build programs with gVirtualXRay")
SET (CPACK_COMPONENT_HEADERS_DESCRIPTION "C/C++ header files for use with gVirtualXRay")

SET (CPACK_COMPONENT_LIBRARIES_GROUP "Development")
SET (CPACK_COMPONENT_HEADERS_GROUP "Development")

SET (CPACK_COMPONENT_GROUP_DEVELOPMENT_DESCRIPTION "All of the tools you'll ever need to develop software with gVirtualXRay")

SET (CPACK_COMPONENT_LIBRARIES_INSTALL_TYPES Developer Full)
SET (CPACK_COMPONENT_HEADERS_INSTALL_TYPES Developer Full)


IF (WIN32)
    SET (CPACK_GENERATOR NSIS)
    SET (CPACK_NSIS_PACKAGE_NAME ${MAIN_PACKAGE_NAME})
    SET (CPACK_NSIS_DISPLAY_NAME ${MAIN_PACKAGE_NAME})
    SET (CPACK_NSIS_CONTACT ${EMAIL})
    SET (CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${MAIN_PACKAGE_NAME}-${PROJECT_VERSION}")
    SET (CPACK_NSIS_MENU_LINKS "${MAIN_WEBSITE}" "Homepage for ${MAIN_PACKAGE_NAME}")
ENDIF (WIN32)


IF (NOT CPack_CMake_INCLUDED)
    INCLUDE(CPack)
ENDIF (NOT CPack_CMake_INCLUDED)

