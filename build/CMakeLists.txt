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


# Minimum CMake version ########################################################
IF(WIN32)
	cmake_minimum_required(VERSION 2.6.0)
ENDIF(WIN32)

IF(COMMAND cmake_policy)
       cmake_policy(SET CMP0003 NEW)
       cmake_policy(SET CMP0013 NEW)
ENDIF(COMMAND cmake_policy)


SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/modules)


# Make include files in paths like /usr/local/include have preference before
# files in OSX frameworks. 
SET(CMAKE_FIND_FRAMEWORK "LAST")


# Project name #################################################################
PROJECT(gVirtualXRay)
set (gVirtualXRay_VERSION_MAJOR 1)
set (gVirtualXRay_VERSION_MINOR 0)


# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
	"${PROJECT_SOURCE_DIR}/gVirtualXRayConfig.h.in"
	"${PROJECT_BINARY_DIR}/gVirtualXRayConfig.h"
)


# Enable all compiler warnings #################################################
add_definitions( -Wall )


# Libraries ####################################################################
SET(optionalLibs)
SET(requiredLibs)


# Options ######################################################################
OPTION(USE_ITK   "Use ITK if possible." OFF)


# Enable Stereo using OpenGL's quad buffering ##################################
IF (NOT APPLE)
	ADD_DEFINITIONS(-DUSE_STEREO)
ENDIF (NOT APPLE)


# Library source code ##########################################################
SET (CXX_SOURCES
	# gVirtualXRay
	../src/gVirtualXRay/XRayDetector.cxx
	../src/gVirtualXRay/XRayRenderer.cxx

	# Graphics
	../src/Graphics/framebufferObject.cpp
	../src/Graphics/Shader.cxx
	../src/Graphics/StereoHelper.cxx
	../src/Graphics/TriangleMesh.cxx
	../src/Graphics/VBO.cxx

    # Utilities
    ../src/Utilities/OpenGLUtilities.cxx
	../src/Utilities/Utilities.cxx

)


# Add header files to the project
SET (HXX_SOURCES
	# gVirtualXRay
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
	../include/gVirtualXRay/framebufferObject.h
	../include/gVirtualXRay/Shader.h
	../include/gVirtualXRay/Shader.inl
	../include/gVirtualXRay/StereoHelper.h
	../include/gVirtualXRay/TriangleMesh.h
	../include/gVirtualXRay/TriangleMesh.inl
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
	../include/gVirtualXRay/FBOException.inl
	../include/gVirtualXRay/FileDoesNotExistException.h
	../include/gVirtualXRay/FileDoesNotExistException.inl
	../include/gVirtualXRay/OpenGLException.h
	../include/gVirtualXRay/OpenGLException.inl
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
#INCLUDE_DIRECTORIES("/usr/local/include")


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
	INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})
	SET(requiredLibs ${requiredLibs} ${GLEW_LIBRARY})
ELSE(GLEW_FOUND)
	MESSAGE(FATAL_ERROR "GLEW not found.")
ENDIF(GLEW_FOUND)


# FreeImage ####################################################################
FIND_PACKAGE(FreeImage)
IF(FREEIMAGE_FOUND)
	ADD_DEFINITIONS(-DHAS_FREEIMAGE)
	INCLUDE_DIRECTORIES(${FREEIMAGE_INCLUDE_DIR})
	SET(optionalLibs ${optionalLibs} ${FREEIMAGE_LIBRARY})
ELSE(FREEIMAGE_FOUND)
	MESSAGE("FREEIMAGE not found.")
ENDIF(FREEIMAGE_FOUND)


# ITK ##########################################################################
FIND_PACKAGE(ITK)
IF(USE_ITK)
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
				MESSAGE("VTK not found.")
			ENDIF(VTK_FOUND)
		ENDIF(ITKVtkGlue_LOADED)
	ELSE(ITK_FOUND)
		MESSAGE("ITK not found.")
	ENDIF(ITK_FOUND)
ENDIF(USE_ITK)


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
SET (RESOURCES_SOURCES ../resources/cumulated_l_buffer.frag
	../resources/cumulated_l_buffer.vert
	../resources/l_buffer.frag
	../resources/l_buffer.vert
	../resources/MUx_Dx.frag
	../resources/MUx_Dx.vert
	../resources/outer_surface_l_buffer.frag
	../resources/outer_surface_l_buffer.vert
	../resources/xray_attenuation.frag
	../resources/xray_attenuation.vert
	../resources/xray_display.frag
	../resources/xray_display.vert
	../resources/mu_water.csv
)


foreach(F ${RESOURCES_SOURCES})
	# Get the file name
	GET_FILENAME_COMPONENT(FILE_NAME ${F} NAME)

	STRING(REGEX REPLACE "\\." "_" VARIABLE_NAME ${FILE_NAME})
	add_custom_command(OUTPUT ${FILE_NAME}.tmp
			#COMMAND ${CMAKE_COMMAND} -E copy ${F} ${FILE_NAME}.tmp
			COMMAND gzip -c -9 ${F} > ${FILE_NAME}.tmp
			COMMAND bin2c --type char -st --name g_${VARIABLE_NAME} ${FILE_NAME}.tmp > ${FILE_NAME}.h
			DEPENDS ${F}.h
	)

	add_custom_target(${FILE_NAME}.h ALL DEPENDS ${FILE_NAME}.tmp)
	add_dependencies(gVirtualXRay ${FILE_NAME}.h)

	set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${FILE_NAME}.tmp)
	set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${FILE_NAME}.h)
endforeach()


# Linkage of the target ########################################################
TARGET_LINK_LIBRARIES (gVirtualXRay ${requiredLibs} ${optionalLibs})


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

	add_subdirectory(../examples/build ${CMAKE_CURRENT_BINARY_DIR}/examples_bin)
ELSE (BUILD_EXAMPLES)
    	MESSAGE("Do not build examples.")
ENDIF (BUILD_EXAMPLES)


install (TARGETS gVirtualXRay DESTINATION lib)
install (FILES ${HXX_SOURCES} DESTINATION include/gVirtualXRay)
install (FILES "${PROJECT_BINARY_DIR}/gVirtualXRayConfig.h" DESTINATION include/gVirtualXRay)

