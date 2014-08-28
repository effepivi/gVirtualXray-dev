################################################################################
#
#	@file		CMakeLists.txt
#
#	@brief		Configuration file to generate Makefiles, 
#			Project/Solution files for MS VC++, Xcode projects, etc.
#			 using the CMake tool. (see http://www.cmake.org/)
#
#	@version	1.0
#
#	@date		15/12/2013
#
#	@author		Dr Franck P. Vidal
#
#	@section	License
#				BSD 3-Clause License.
#
#				For details on use and redistribution please refer
#				to http://opensource.org/licenses/BSD-3-Clause
#
#	@section	Copyright
#				(c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
#				http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
#				BSD 3-Clause License
#
################################################################################


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


SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/modules)


# Make include files in paths like /usr/local/include have preference before
# files in OSX frameworks. 
SET(CMAKE_FIND_FRAMEWORK "LAST")


# Project name #################################################################
PROJECT(gVirtualXRay)
set (gVirtualXRay_VERSION_MAJOR 1)
set (gVirtualXRay_VERSION_MINOR 0)
set (gVirtualXRay_VERSION_PATCH 0)


# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
	"${PROJECT_SOURCE_DIR}/gVirtualXRayConfig.h.in"
	"${PROJECT_BINARY_DIR}/gVirtualXRayConfig.h"
)


# Enable all compiler warnings #################################################
#IF(NOT WIN32)
#	add_definitions( -Wall )
#ENDIF(NOT WIN32)


# Libraries ####################################################################
SET(optionalLibs)
SET(requiredLibs)

SET (gVirtualXRay_BASE_DIR ${PROJECT_SOURCE_DIR})
SET (gVirtualXRay_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
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
	../src/Graphics/VBO.cxx

    # Utilities
	../src/Utilities/FBOException.cxx
	../src/Utilities/OpenGLException.cxx
	../src/Utilities/OpenGLUtilities.cxx
	../src/Utilities/Utilities.cxx

)


# Add header files to the project
SET (HXX_SOURCES
	# gVirtualXRay
	${CMAKE_CURRENT_BINARY_DIR}/gVirtualXRayConfig.h

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

	# Math
	../include/gVirtualXRay/Matrix4x4.h
	../include/gVirtualXRay/Matrix4x4.inl
	../include/gVirtualXRay/Vec2.h
	../include/gVirtualXRay/Vec2.inl
	../include/gVirtualXRay/Vec3.h
	../include/gVirtualXRay/Vec3.inl

	# Utilities
	../include/gVirtualXRay/Exception.h
	../include/gVirtualXRay/Exception.inl
	../include/gVirtualXRay/FBOException.h
	../include/gVirtualXRay/FileDoesNotExistException.h
	../include/gVirtualXRay/FileDoesNotExistException.inl
	../include/gVirtualXRay/OpenGLException.h
	../include/gVirtualXRay/OpenGLUtilities.h
	../include/gVirtualXRay/OpenGLUtilities.inl
	../include/gVirtualXRay/OutOfBoundsException.h
	../include/gVirtualXRay/OutOfBoundsException.inl
	../include/gVirtualXRay/OutOfMemoryException.h
	../include/gVirtualXRay/OutOfMemoryException.inl
	../include/gVirtualXRay/Types.h
	../include/gVirtualXRay/Units.h
	../include/gVirtualXRay/Utilities.h
)


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
INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}")
INCLUDE_DIRECTORIES("../include")
IF (NOT APPLE)
    INCLUDE_DIRECTORIES("../include/OpenGL4")
ENDIF(NOT APPLE)
#INCLUDE_DIRECTORIES("/usr/local/include")


# GZIP ########################################################################
FIND_PACKAGE(GZIP REQUIRED)
IF(NOT GZIP_FOUND)
	MESSAGE(FATAL_ERROR "GZIP not found.")
ENDIF(NOT GZIP_FOUND)


# OpenGL ######################################################################
FIND_PACKAGE(OpenGL REQUIRED)
IF(OPENGL_FOUND)
	INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR} ${GLU_INCLUDE_PATH})
	SET(requiredLibs ${requiredLibs} ${OPENGL_LIBRARIES} ${GLU_LIBRARY})
ELSE(OPENGL_FOUND)
	MESSAGE(FATAL_ERROR "OpenGL not found.")
ENDIF(OPENGL_FOUND)


FIND_PACKAGE(GLEW REQUIRED)
IF(GLEW_FOUND)
	ADD_DEFINITIONS(-DGLEW_STATIC)
    ADD_DEFINITIONS(-DHAS_GLEW)
    INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})
    LINK_DIRECTORIES(${GLEW_LIBRARY_DIR})
    SET(requiredLibs ${requiredLibs} ${GLEW_LIBRARY})
ELSE(GLEW_FOUND)
    MESSAGE(FATAL_ERROR "GLEW not found.")
ENDIF(GLEW_FOUND)


# BOOST #######################################################################
FIND_PACKAGE(Boost QUIET REQUIRED)
if(Boost_FOUND)
	SET( Boost_USE_STATIC_LIBS OFF )
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
	LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
	SET(requiredLibs ${requiredLibs} ${Boost_LIBRARIES})
ELSE(Boost_FOUND)
	MESSAGE(FATAL_ERROR "Boost not found.")
ENDIF(Boost_FOUND)


# FreeImage ####################################################################
#FIND_PACKAGE(FreeImage)
#IF(FREEIMAGE_FOUND)
#	ADD_DEFINITIONS(-DHAS_FREEIMAGE)
#	INCLUDE_DIRECTORIES(${FREEIMAGE_INCLUDE_DIR})
#	SET(optionalLibs ${optionalLibs} ${FREEIMAGE_LIBRARY})
#ELSE(FREEIMAGE_FOUND)
#	MESSAGE("FREEIMAGE not found.")
#ENDIF(FREEIMAGE_FOUND)


# ITK ##########################################################################
FIND_PACKAGE(ITK)
IF(ITK_FOUND)
	ADD_DEFINITIONS(-DHAS_ITK)
	INCLUDE(${USE_ITK_FILE})
	SET(optionalLibs ${optionalLibs} ${ITK_LIBRARIES})

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
	MESSAGE(WARNING "ITK not found.")
ENDIF(ITK_FOUND)


# ZLIB #########################################################################
FIND_PACKAGE(ZLIB REQUIRED)
IF(ZLIB_FOUND)
	INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIRS})
	SET(requiredLibs ${requiredLibs} ${ZLIB_LIBRARIES})
ELSE(ZLIB_FOUND)
	MESSAGE("ZLIB not found.")
ENDIF(ZLIB_FOUND)


# Create the library ###########################################################
ADD_LIBRARY (gVirtualXRay STATIC ${SOURCES})


# Create the RESOURCES ###########################################################
SET (RESOURCES_SOURCES
	../resources/cumulated_l_buffer_gl2.frag
	../resources/cumulated_l_buffer_gl2.vert
	../resources/cumulated_l_buffer_gl3.frag
	../resources/cumulated_l_buffer_gl3.vert
	../resources/l_buffer_gl2.frag
	../resources/l_buffer_gl2.vert
	../resources/l_buffer_gl3.frag
	../resources/l_buffer_gl3.vert
	../resources/MUx_Dx_gl2.frag
	../resources/MUx_Dx_gl2.vert
	../resources/MUx_Dx_gl3.frag
	../resources/MUx_Dx_gl3.vert
	../resources/outer_surface_l_buffer_gl2.frag
	../resources/outer_surface_l_buffer_gl2.vert
	../resources/outer_surface_l_buffer_gl3.frag
	../resources/outer_surface_l_buffer_gl3.vert
	../resources/xray_attenuation_gl2.frag
	../resources/xray_attenuation_gl2.vert
	../resources/xray_attenuation_gl3.frag
	../resources/xray_attenuation_gl3.vert
	../resources/xray_display_gl2.frag
	../resources/xray_display_gl2.vert
	../resources/xray_display_gl3.frag
	../resources/xray_display_gl3.vert
	../resources/mu_water.csv
)


FIND_PACKAGE(GZIP REQUIRED)
IF(NOT GZIP_FOUND)
	MESSAGE(FATAL_ERROR "GZIP not found.")
ENDIF(NOT GZIP_FOUND)


INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/CreateHeaderFiles.cmake)
CreateHeaderFiles(".." "BinaryLibFiles" "${RESOURCES_SOURCES}")


# Linkage of the target ########################################################
set_target_properties(gVirtualXRay PROPERTIES LINK_SEARCH_START_STATIC ON)
TARGET_LINK_LIBRARIES (gVirtualXRay ${requiredLibs} ${optionalLibs})


# Make sure the source is generated before the executable builds.
ADD_DEPENDENCIES(gVirtualXRay BinaryLibFiles)


# Doxygen documentation ########################################################
IF(WIN32)
	FIND_PACKAGE(Doxygen) 
	IF (DOXYGEN_FOUND)
		ADD_CUSTOM_TARGET(html-doc ${DOXYGEN_EXECUTABLE} ../doc/Doxyfile) 
	ELSE (DOXYGEN_FOUND)
    	MESSAGE(STATUS "WARNING: Doxygen not found - Reference manual will not be created")
	ENDIF (DOXYGEN_FOUND)
ELSE(WIN32)
	ADD_CUSTOM_TARGET(html-doc doxygen ../doc/Doxyfile) 
ENDIF(WIN32)


# Examples #####################################################################
option(BUILD_EXAMPLES "Build examples." ON)
IF (BUILD_EXAMPLES)
   	MESSAGE(STATUS "Build examples.")

	add_subdirectory(../examples ${CMAKE_CURRENT_BINARY_DIR}/examples_bin)
ELSE (BUILD_EXAMPLES)
   	MESSAGE("Do not build examples.")
ENDIF (BUILD_EXAMPLES)


# Tutorials ####################################################################
option(BUILD_TUTORIALS "Build tutorials." ON)
IF (BUILD_TUTORIALS)
   	MESSAGE(STATUS "Build tutorials.")

	add_subdirectory(../tutorials ${CMAKE_CURRENT_BINARY_DIR}/tutorials_bin)
ELSE (BUILD_TUTORIALS)
   	MESSAGE("Do not build tutorials.")
ENDIF (BUILD_TUTORIALS)

INSTALL (TARGETS                               gVirtualXRay ARCHIVE DESTINATION library/lib                  COMPONENT libraries)
INSTALL (FILES                               ${HXX_SOURCES}         DESTINATION library/include/gVirtualXRay COMPONENT headers)
INSTALL (FILES ../LICENSE.txt DESTINATION library)
INSTALL (FILES ../LICENSE.txt DESTINATION .)

SET (CPACK_PACKAGE_VERSION_MAJOR ${gVirtualXRay_VERSION_MAJOR})
SET (CPACK_PACKAGE_VERSION_MINOR ${gVirtualXRay_VERSION_MINOR})
SET (CPACK_PACKAGE_VERSION_PATCH ${gVirtualXRay_VERSION_PATCH})
SET (CPACK_PACKAGE_VERSION "${gVirtualXRay_VERSION_MAJOR}.${gVirtualXRay_VERSION_MINOR}.${gVirtualXRay_VERSION_PATCH}")

SET (CPACK_PACKAGE_VENDOR "gVirtualXRay team")
SET (CPACK_PACKAGE_DESCRIPTION_SUMMARY "gVirtualXRay - Installation")
SET (CPACK_PACKAGE_INSTALL_DIRECTORY "gVirtualXRay")
    
SET (CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE.txt)

SET (CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "Libraries")
SET (CPACK_COMPONENT_HEADERS_DISPLAY_NAME "C++ Headers")

SET (CPACK_COMPONENT_LIBRARIES_DESCRIPTION "Static libraries used to build programs with gVirtualXRay")
SET (CPACK_COMPONENT_HEADERS_DESCRIPTION "C/C++ header files for use with gVirtualXRay")

SET (CPACK_COMPONENT_LIBRARIES_GROUP "Development")
SET (CPACK_COMPONENT_HEADERS_GROUP "Development")

SET (CPACK_COMPONENT_GROUP_DEVELOPMENT_DESCRIPTION "All of the tools you'll ever need to develop software with gVirtualXRay")

SET (CPACK_COMPONENT_LIBRARIES_INSTALL_TYPES Developer Full)
SET (CPACK_COMPONENT_HEADERS_INSTALL_TYPES Developer Full)

IF (NOT CPack_CMake_INCLUDED)
    INCLUDE(CPack)
ENDIF (NOT CPack_CMake_INCLUDED)

