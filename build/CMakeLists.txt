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


# Library source code ##########################################################
SET (CXX_SOURCES
	# gVirtualXRay
	../src/gVirtualXRay/XRayDetector.cxx
	../src/gVirtualXRay/XRayRenderer.cxx

	# Graphics
	../src/Graphics/framebufferObject.cpp
	../src/Graphics/Shader.cxx
	../src/Graphics/TriangleMesh.cxx
	../src/Graphics/VBO.cxx

    # Utilities
    ../src/Utilities/OpenGLUtilities.cxx
)


# Add header files to the project
SET (HXX_SOURCES
	# gVirtualXRay
	../include/gVirtualXRay/SpectrumRecord.h
	../include/gVirtualXRay/SpectrumRecord.inl
	../include/gVirtualXRay/XRayBeam.h
	../include/gVirtualXRay/XRayBeam.inl
	../include/gVirtualXRay/XRayDetector.h
	../include/gVirtualXRay/XRayDetector.inl
	../include/gVirtualXRay/XRayRenderer.h
	../include/gVirtualXRay/XRayRenderer.inl

	# Graphics
	../include/gVirtualXRay/Graphics/framebufferObject.h
	../include/gVirtualXRay/Graphics/Shader.h
	../include/gVirtualXRay/Graphics/Shader.inl
	../include/gVirtualXRay/Graphics/TriangleMesh.h
	../include/gVirtualXRay/Graphics/TriangleMesh.inl
	../include/gVirtualXRay/Graphics/VBO.h
	../include/gVirtualXRay/Graphics/VBO.inl

	# Math
	../include/gVirtualXRay/Math/Matrix4x4.h
	../include/gVirtualXRay/Math/Matrix4x4.inl
	../include/gVirtualXRay/Math/Vec2.h
	../include/gVirtualXRay/Math/Vec2.inl
	../include/gVirtualXRay/Math/Vec3.h
	../include/gVirtualXRay/Math/Vec3.inl

	# Utilities
	../include/gVirtualXRay/Utilities/Exception.h
	../include/gVirtualXRay/Utilities/Exception.inl
	../include/gVirtualXRay/Utilities/FBOException.h
	../include/gVirtualXRay/Utilities/FBOException.inl
	../include/gVirtualXRay/Utilities/FileDoesNotExistException.h
	../include/gVirtualXRay/Utilities/FileDoesNotExistException.inl
	../include/gVirtualXRay/Utilities/OpenGLException.h
	../include/gVirtualXRay/Utilities/OpenGLException.inl
	../include/gVirtualXRay/Utilities/OpenGLUtilities.h
	../include/gVirtualXRay/Utilities/OpenGLUtilities.inl
	../include/gVirtualXRay/Utilities/OutOfBoundsException.h
	../include/gVirtualXRay/Utilities/OutOfBoundsException.inl
	../include/gVirtualXRay/Utilities/OutOfMemoryException.h
	../include/gVirtualXRay/Utilities/OutOfMemoryException.inl
	../include/gVirtualXRay/Utilities/Types.h
	../include/gVirtualXRay/Utilities/Units.h
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


# Libraries ####################################################################
SET(optionalLibs)
SET(requiredLibs)


# Headers of the project #######################################################
INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}")
INCLUDE_DIRECTORIES("../include")
INCLUDE_DIRECTORIES("/usr/local/include")


# OpenGL ######################################################################
FIND_PACKAGE(OpenGL REQUIRED)
IF(OPENGL_FOUND)
	INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR} ${GLU_INCLUDE_PATH})
	SET(requiredLibs ${requiredLibs} ${OPENGL_LIBRARIES} ${GLU_LIBRARY})
ELSE(OPENGL_FOUND)
	MESSAGE(FATAL_ERROR "OpenGL not found.")
ENDIF(OPENGL_FOUND)


FIND_PACKAGE(GLUT REQUIRED)
IF(GLUT_FOUND)
	INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})
	SET(requiredLibs ${requiredLibs} ${GLUT_glut_LIBRARY})
ELSE(GLUT_FOUND)
	MESSAGE(FATAL_ERROR "GLUT not found.")
ENDIF(GLUT_FOUND)


FIND_PACKAGE(GLEW REQUIRED)
IF(GLEW_FOUND)
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
IF(ITK_FOUND)
	ADD_DEFINITIONS(-DHAS_ITK)
	INCLUDE(${USE_ITK_FILE})
	SET(optionalLibs ${optionalLibs} ${ITK_LIBRARIES})
ELSE(ITK_FOUND)
	MESSAGE("ITK not found.")
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


SET (RESOURCES cumulated_l_buffer.frag.h
	cumulated_l_buffer.vert.h
	l_buffer.frag.h
	l_buffer.vert.h
	MUx_Dx.frag.h
	MUx_Dx.vert.h
	outer_surface_l_buffer.frag.h
	outer_surface_l_buffer.vert.h
	xray_attenuation.frag.h
	xray_attenuation.vert.h
	xray_display.frag.h
	xray_display.vert.h
	mu_water.csv.h
)


foreach(F ${RESOURCES_SOURCES})
	# Get the file name
	GET_FILENAME_COMPONENT(FILE_NAME ${F} NAME)

	IF(UNIX)
		IF(APPLE)
		       STRING(REGEX REPLACE "\\." "_" VARIABLE_NAME ${FILE_NAME})
			add_custom_command(OUTPUT ${FILE_NAME}.tmp
				#COMMAND ${CMAKE_COMMAND} -E copy ${F} ${FILE_NAME}.tmp
				COMMAND gzip -c -9 ${F} > ${FILE_NAME}.tmp
				COMMAND bin2c --type char -st --name g_${VARIABLE_NAME} ${FILE_NAME}.tmp > ${FILE_NAME}.h
				DEPENDS ${F}.h
			)

			add_custom_target(${FILE_NAME}.h ALL DEPENDS ${FILE_NAME}.tmp)
			add_dependencies(gVirtualXRay ${FILE_NAME}.h)

			MESSAGE("add_dependencies(gVirtualXRay ${FILE_NAME}.h)")

		ELSE(APPLE)
			add_custom_command(OUTPUT ${FILE_NAME}.tmp
				COMMAND ${CMAKE_OBJCOPY} -I binary -O elf64-x86-64 -B i386 ${F} ${FILE_NAME}.tmp
				COMMAND ${CMAKE_COMMAND} -E copy ${FILE_NAME}.tmp ${FILE_NAME}.o
				COMMAND ar -r libgVirtualXRay.a ${FILE_NAME}.o
				COMMAND ${CMAKE_RANLIB} libgVirtualXRay.a
				DEPENDS ${F} gVirtualXRay 
			)
		ENDIF(APPLE)
	ENDIF(UNIX)

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
