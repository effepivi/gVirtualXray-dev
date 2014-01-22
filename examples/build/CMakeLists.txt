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
#	@date		01/12/2013
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


# Project name #################################################################
project (examples)


# Headers of the project #######################################################


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


# Create the the main program ##################################################
ADD_EXECUTABLE (test_01 ../src/test_01.cxx)


 # Linkage of the target #######################################################
TARGET_LINK_LIBRARIES (test_01 gVirtualXRay ${requiredLibs} ${optionalLibs})


# Create the the main program ##################################################
ADD_EXECUTABLE (test_mu_water ../src/test_mu_water.cxx)


 # Linkage of the target #######################################################
TARGET_LINK_LIBRARIES (test_mu_water gVirtualXRay ${requiredLibs} ${optionalLibs})


# Create the the main program ##################################################
#ADD_EXECUTABLE (test_02 ../src/test_02.cxx)


# Linkage of the target ########################################################
#TARGET_LINK_LIBRARIES (test_02 gVirtualXRay ${requiredLibs} ${optionalLibs})
