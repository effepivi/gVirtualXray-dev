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


# Initialise some states by default ############################################
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_BUILD_TYPE Release CACHE STRING "Debug|Release|RelWithDebInfo|MinSizeRel")


# Minimum CMake version ########################################################
IF(WIN32)
	cmake_minimum_required(VERSION 2.6.0)
ENDIF(WIN32)


# Enable all compiler warnings #################################################
add_definitions( -Wall )


# Project name #################################################################
project (examples)
set (examples_VERSION_MAJOR 1)
set (examples_VERSION_MINOR 2)
set (examples_VERSION_PATCH 0)

IF(APPLE)
	OPTION(USE_BUNDLE "Use Apple Bundle." ON)
	SET(USE_GLUT 1)
ELSE(APPLE)
	OPTION(USE_FLTK "Use FLTK." ON)
	OPTION(USE_GLUT "Use GLUT." OFF)
ENDIF(APPLE)


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
IF(APPLE)
	FIND_PACKAGE(Boost REQUIRED COMPONENTS system filesystem program_options)
	if(Boost_FOUND)
		INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
		LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
		SET(requiredLibs ${requiredLibs} ${Boost_LIBRARIES})
	ELSE(Boost_FOUND)
		MESSAGE(FATAL_ERROR "Boost not found.")
	ENDIF(Boost_FOUND)
ENDIF(APPLE)


# Create the RESOURCES ###########################################################
SET (RESOURCES_SOURCES ../../examples/shaders/display.frag
	../../examples/shaders/display.vert
)


FIND_PACKAGE(GZIP REQUIRED)
IF(NOT GZIP_FOUND)
	MESSAGE(FATAL_ERROR "GZIP not found.")
ENDIF(NOT GZIP_FOUND)


# portably convert binary file to header
FUNCTION(FILE2C INFILE VARNAME OUTFILE)
	FILE(READ ${INFILE} HEXFILE HEX)
	STRING(LENGTH ${HEXFILE} XRSLEN)
	SET(HEXPOS 0)
	FILE(WRITE ${OUTFILE} "/* generated from ${INFILE}.tmp; do not edit */\n" "const unsigned char ${VARNAME}[] = {")
	
	WHILE(${HEXPOS} LESS ${XRSLEN})
		MATH(EXPR LPOS "${HEXPOS} % 32")
	
		IF(NOT ${LPOS})
			FILE(APPEND ${OUTFILE} "\n")
		ENDIF(NOT ${LPOS})
	
		STRING(SUBSTRING ${HEXFILE} ${HEXPOS} 2 HEXBYTE)
		FILE(APPEND ${OUTFILE} "0x${HEXBYTE}")
		MATH(EXPR HEXPOS "${HEXPOS} + 2")
	
		IF(${HEXPOS} LESS ${XRSLEN})
			FILE(APPEND ${OUTFILE} ",")
		ENDIF(${HEXPOS} LESS ${XRSLEN})
	ENDWHILE(${HEXPOS} LESS ${XRSLEN})
	FILE(APPEND ${OUTFILE} "};\n")
ENDFUNCTION(FILE2C)


foreach(F ${RESOURCES_SOURCES})
	# Get the file name
	GET_FILENAME_COMPONENT(FILE_NAME ${F} NAME)

	execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${F} ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.tmp)

	execute_process(COMMAND ${GZIP_TOOL} --best --force --quiet ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.tmp)

	STRING(REGEX REPLACE "\\." "_" VARIABLE_NAME ${FILE_NAME})
	FILE2C(${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.tmp.gz g_${VARIABLE_NAME} ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.h)

	add_dependencies(gVirtualXRay ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.h)

	set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.tmp)
	set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.tmp.gz)
	set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.h)
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

	ADD_EXECUTABLE (DragonDemo MACOSX_BUNDLE ../src/welsh_dragon.cxx ../data/welsh-dragon-small.stl ../build/DragonDemo.icns)
        ADD_DEFINITIONS(-DHAS_BUNDLE)
	SET_SOURCE_FILES_PROPERTIES (
		../data/welsh-dragon-small.stl
		PROPERTIES
		MACOSX_PACKAGE_LOCATION examples/data
	)

	TARGET_LINK_LIBRARIES (DragonDemo gVirtualXRay ${requiredLibs} ${optionalLibs})

	# Define some settings for the Bundle
	set(MACOSX_BUNDLE_BUNDLE_NAME DragonDemo)
	set(MACOSX_BUNDLE_GUI_IDENTIFIER "DragonDemo")
	SET_SOURCE_FILES_PROPERTIES(../build/DragonDemo.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
	set(MACOSX_BUNDLE_ICON_FILE DragonDemo.icns)
	set(MACOSX_BUNDLE_INFO_STRING "${examples_VERSION_MAJOR}.${examples_VERSION_MINOR}.${examples_VERSION_PATCH}, Copyright 2014 gVirtualXRay team")
	set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${examples_VERSION_MAJOR}.${examples_VERSION_MINOR}")
	set(MACOSX_BUNDLE_LONG_VERSION_STRING "${examples_VERSION_MAJOR}.${examples_VERSION_MINOR}.${examples_VERSION_PATCH}")
	set(MACOSX_BUNDLE_BUNDLE_VERSION "${examples_VERSION_MAJOR}.${examples_VERSION_MINOR}.${examples_VERSION_PATCH}")
	set(MACOSX_BUNDLE_COPYRIGHT "(C) 2013-2014 Franck P. Vidal")
ENDIF(USE_BUNDLE)
