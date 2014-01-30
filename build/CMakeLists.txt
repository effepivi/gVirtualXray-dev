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
	../include/Graphics/framebufferObject.h
	../include/Graphics/Shader.h
	../include/Graphics/Shader.inl
	../include/Graphics/TriangleMesh.h
	../include/Graphics/TriangleMesh.inl
	../include/Graphics/VBO.h
	../include/Graphics/VBO.inl

	# Math
	../include/Math/Matrix4x4.h
	../include/Math/Matrix4x4.inl
	../include/Math/Vec2.h
	../include/Math/Vec2.inl
	../include/Math/Vec3.h
	../include/Math/Vec3.inl

	# Utilities
	../include/Utilities/Exception.h
	../include/Utilities/Exception.inl
	../include/Utilities/FBOException.h
	../include/Utilities/FBOException.inl
	../include/Utilities/FileDoesNotExistException.h
	../include/Utilities/FileDoesNotExistException.inl
	../include/Utilities/OpenGLException.h
	../include/Utilities/OpenGLException.inl
	../include/Utilities/OpenGLUtilities.h
	../include/Utilities/OpenGLUtilities.inl
	../include/Utilities/OutOfBoundsException.h
	../include/Utilities/OutOfBoundsException.inl
	../include/Utilities/OutOfMemoryException.h
	../include/Utilities/OutOfMemoryException.inl
	../include/Utilities/Types.h
	../include/Utilities/Units.h
)


# Create the shaders ###########################################################
SET (SHADER_SOURCES ../shaders/cumulated_l_buffer.frag
	../shaders/cumulated_l_buffer.vert
	../shaders/l_buffer.frag
	../shaders/l_buffer.vert
	../shaders/MUx_Dx.frag
	../shaders/MUx_Dx.vert
	../shaders/outer_surface_l_buffer.frag
	../shaders/outer_surface_l_buffer.vert
	../shaders/xray_attenuation.frag
	../shaders/xray_attenuation.vert
	../shaders/xray_display.frag
	../shaders/xray_display.vert
)


SET (SHADER_OBJECTS cumulated_l_buffer.frag.o 
	cumulated_l_buffer.vert.o
	l_buffer.frag.o
	l_buffer.vert.o
	MUx_Dx.frag.o
	MUx_Dx.vert.o
	outer_surface_l_buffer.frag.o
	outer_surface_l_buffer.vert.o
	xray_attenuation.frag.o
	xray_attenuation.vert.o
	xray_display.frag.o
	xray_display.vert.o
)


foreach(F ${SHADER_SOURCES})
	# Get the file name
	GET_FILENAME_COMPONENT(FILE_NAME ${F} NAME)

	add_custom_command(OUTPUT ${FILE_NAME}.tmp
                   COMMAND objcopy -I binary -O elf64-x86-64 -B i386 ${F} ${FILE_NAME}.tmp
                   COMMAND ${CMAKE_COMMAND} -E copy ${FILE_NAME}.tmp ${FILE_NAME}.o
						 COMMAND ar -r libgVirtualXRay.a ${FILE_NAME}.o
						 COMMAND ${CMAKE_RANLIB} libgVirtualXRay.a
                   DEPENDS ${F} gVirtualXRay 
	)

	add_custom_target(${FILE_NAME}.o ALL DEPENDS ${FILE_NAME}.tmp)
	set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${FILE_NAME}.o)
endforeach()


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
INCLUDE_DIRECTORIES(../include)
INCLUDE_DIRECTORIES(../include/gVirtualXRay)
INCLUDE_DIRECTORIES(../include/Graphics)
INCLUDE_DIRECTORIES(../include/Math)
INCLUDE_DIRECTORIES(../include/Utilities)
INCLUDE_DIRECTORIES(/usr/local/include)


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


# Create the library ###########################################################
ADD_LIBRARY (gVirtualXRay STATIC ${SOURCES})


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

