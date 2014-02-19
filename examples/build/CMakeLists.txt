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

IF(APPLE)
	OPTION(USE_BUNDLE "Use Apple Bundle." ON)
ENDIF(APPLE)
OPTION(USE_FLTK "Use FLTK." ON)
OPTION(USE_GLUT "Use GLUT." OFF)


# Headers of the project #######################################################
INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}")
INCLUDE_DIRECTORIES("../include")


# FLTK ########################################################################
IF (USE_FLTK)
FIND_PACKAGE(FLTK REQUIRED)
if(FLTK_FOUND)
        ADD_DEFINITIONS(-DHAS_FLTK)
        INCLUDE_DIRECTORIES(${FLTK_INCLUDE_DIR})
        SET(requiredLibs ${requiredLibs} ${FLTK_LIBRARIES})
ELSE(FLTK_FOUND)
        MESSAGE(FATAL_ERROR "FLTK not found.")
endif(FLTK_FOUND)
ENDIF (USE_FLTK)


# OpenGL ######################################################################
FIND_PACKAGE(OpenGL REQUIRED)
IF(OPENGL_FOUND)
	INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR} ${GLU_INCLUDE_PATH})
	SET(requiredLibs ${requiredLibs} ${OPENGL_LIBRARIES} ${GLU_LIBRARY})
ELSE(OPENGL_FOUND)
	MESSAGE(FATAL_ERROR "OpenGL not found.")
ENDIF(OPENGL_FOUND)


# GLUT ########################################################################
IF (USE_GLUT)
FIND_PACKAGE(GLUT REQUIRED)
IF(GLUT_FOUND)
        ADD_DEFINITIONS(-DHAS_GLUT)
        INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})
        SET(requiredLibs ${requiredLibs} ${GLUT_glut_LIBRARY})
ELSE(GLUT_FOUND)
        MESSAGE(FATAL_ERROR "GLUT not found.")
ENDIF(GLUT_FOUND)
ENDIF (USE_GLUT)


FIND_PACKAGE(GLEW REQUIRED)
IF(GLEW_FOUND)
	INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})
	SET(requiredLibs ${requiredLibs} ${GLEW_LIBRARY})
ELSE(GLEW_FOUND)
	MESSAGE(FATAL_ERROR "GLEW not found.")
ENDIF(GLEW_FOUND)


# BOOST #######################################################################
FIND_PACKAGE(Boost REQUIRED COMPONENTS system filesystem program_options)
if(Boost_FOUND)
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
	LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
	SET(requiredLibs ${requiredLibs} ${Boost_LIBRARIES})
ELSE(Boost_FOUND)
	MESSAGE(FATAL_ERROR "Boost not found.")
ENDIF(Boost_FOUND)


# Create the RESOURCES ###########################################################
SET (RESOURCES_SOURCES ../../examples/shaders/display.frag
	../../examples/shaders/display.vert
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


# Create the the main programs #################################################
REMOVE_DEFINITIONS(-DHAS_BUNDLE)
ADD_EXECUTABLE (test_01 ../src/test_01.cxx)
TARGET_LINK_LIBRARIES (test_01 gVirtualXRay ${requiredLibs} ${optionalLibs})

ADD_EXECUTABLE (welsh_dragon ../src/welsh_dragon.cxx)
TARGET_LINK_LIBRARIES (welsh_dragon gVirtualXRay ${requiredLibs} ${optionalLibs})

ADD_EXECUTABLE (test_mu_water ../src/test_mu_water.cxx)
TARGET_LINK_LIBRARIES (test_mu_water gVirtualXRay ${requiredLibs} ${optionalLibs})

IF(USE_BUNDLE)

	ADD_EXECUTABLE (DragonDemo MACOSX_BUNDLE ../src/welsh_dragon.cxx ../data/welsh-dragon-small.stl)
        ADD_DEFINITIONS(-DHAS_BUNDLE)
	SET_SOURCE_FILES_PROPERTIES (
		../data/welsh-dragon-small.stl
		PROPERTIES
		MACOSX_PACKAGE_LOCATION examples/data
	)

#MACOSX_BUNDLE_INFO_STRING
#MACOSX_BUNDLE_ICON_FILE
#MACOSX_BUNDLE_GUI_IDENTIFIER
#MACOSX_BUNDLE_LONG_VERSION_STRING
#MACOSX_BUNDLE_BUNDLE_NAME
#MACOSX_BUNDLE_SHORT_VERSION_STRING
#MACOSX_BUNDLE_BUNDLE_VERSION
#MACOSX_BUNDLE_COPYRIGHT

	TARGET_LINK_LIBRARIES (DragonDemo gVirtualXRay ${requiredLibs} ${optionalLibs})
ENDIF(USE_BUNDLE)


