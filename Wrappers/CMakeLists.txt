#
#
# Copyright (c) 2019, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


PROJECT (gvxrWrappers)


INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.cmake)


# Initialise some states by default ############################################
SET (BUILD_SHARED_LIBS OFF)


# Minimum CMake version ########################################################
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 11) # C++11
set(CMAKE_CXX_STANDARD_REQUIRED ON) # C++11 is required (I.e. not optional)
set(CMAKE_CXX_EXTENSIONS OFF) # without compiler extensions like gnu++11


if(${CMAKE_VERSION} VERSION_LESS "3.8.0")
    message("Please consider to switch to CMake 3.8.0")
endif()


cmake_policy(SET CMP0078 NEW) # SWIG


OPTION(USE_SYSTEM_SimpleGVXR   "Use System SimpleGVXR"      OFF)


# for superbuild ###############################################################
IF (NOT USE_SYSTEM_SimpleGVXR)
		SET (USE_SUPERBUILD ON)
		INCLUDE(ExternalProject)
ENDIF (NOT USE_SYSTEM_SimpleGVXR)


# Find SimpleGVXR ############################################################
IF (NOT USE_SYSTEM_SimpleGVXR)

		#INCLUDE(cmake/Build_SimpleGVXR.cmake)
		#build_SimpleGVXR( SimpleGVXR ${PROJECT_BINARY_DIR} )


		message(STATUS ${CMAKE_SOURCE_DIR}/cmake/SimpleGVXR)
		# The first external project will be built at *configure stage*
		file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/SimpleGVXR)

		execute_process(
		    COMMAND ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR}/cmake/SimpleGVXR
		    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/SimpleGVXR
				RESULT_VARIABLE gvxr_config_result
				)

		message(STATUS "SimpleGVXR config result: ${gvxr_config_result}")

		execute_process(
		    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/SimpleGVXR --target SimpleGVXR -- -j12
		    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/SimpleGVXR
				RESULT_VARIABLE gvxr_build_result
				)

				message(STATUS "SimpleGVXR build result: ${gvxr_build_result}")

    SET (gVirtualXRay_DIR ${CMAKE_BINARY_DIR}/SimpleGVXR/build/gVirtualXRay/install/gVirtualXRay-1.1.0)
		SET (SimpleGVXR_DIR ${CMAKE_BINARY_DIR}/SimpleGVXR/install/SimpleGVXR-1.0.0)
		message(STATUS "SimpleGVXR install: ${gVirtualXRay_DIR}")

ENDIF (NOT USE_SYSTEM_SimpleGVXR)


FIND_PACKAGE(SimpleGVXR REQUIRED)
include(${SimpleGVXR_USE_FILE})


# SWIG ########################################################################
FIND_PACKAGE(SWIG)

# It has not been found
IF (NOT SWIG_FOUND)
    # It is a Windows system, download its corresponding binaries
    IF (WIN32)
        file(DOWNLOAD https://kent.dl.sourceforge.net/project/swig/swigwin/swigwin-3.0.12/swigwin-3.0.12.zip ${PROJECT_BINARY_DIR}/swigwin-3.0.12.zip)

        # Unzip swigwin-3.0.12.zip
        execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${PROJECT_BINARY_DIR}/swigwin-3.0.12.zip
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR})

        # Set the corresponding variables for CMake
        SET (SWIG_EXECUTABLE ${PROJECT_BINARY_DIR}/swigwin-3.0.12/swig.exe CACHE FILEPATH "Where swig.exe is" FORCE)
        SET (SWIG_DIR        ${PROJECT_BINARY_DIR}/swigwin-3.0.12/Lib      CACHE PATH     "Where Swig is"     FORCE)

        # Try again, but make it required this time
        FIND_PACKAGE(SWIG REQUIRED)

    # It is not a Windows system, install it
    ELSE (WIN32)
        MESSAGE(WARNING "SWIG not found, please install it first.")
    ENDIF (WIN32)
ENDIF (NOT SWIG_FOUND)

# SWIG has been found
IF (SWIG_FOUND)
    INCLUDE (${SWIG_USE_FILE})

    SET (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

    set(CMAKE_SWIG_FLAGS "")
    #include_directories(${CMAKE_CURRENT_SOURCE_DIR})
    #include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../SimpleGVXR)

    option(BUILD_CSHARP  "Build C# SWIG module" OFF)
    #option(BUILD_LUA     "Build Lua SWIG module" OFF)
    option(BUILD_JAVA    "Build Java SWIG module" OFF)
    option(BUILD_MATLAB  "Build Matlab SWIG module" OFF)
    option(BUILD_OCTAVE  "Build Octave SWIG module" OFF)
    option(BUILD_PERL    "Build Perl SWIG module" OFF)
    option(BUILD_PYTHON2 "Build Python2 SWIG module" OFF)
    option(BUILD_PYTHON3 "Build Python3 SWIG module" OFF)
    option(BUILD_R       "Build R SWIG module" OFF)
    option(BUILD_RUBY    "Build Ruby SWIG module" OFF)
    option(BUILD_TCL     "Build TCL SWIG module" OFF)

    IF (BUILD_PYTHON2 AND BUILD_PYTHON3)
        MESSAGE(FATAL_ERROR "Both variables BUILD_PYTHON2 and BUILD_PYTHON3 are ON. Choose either Python2 or Python3, not both.")
    ENDIF (BUILD_PYTHON2 AND BUILD_PYTHON3)

    IF (BUILD_CSHARP)
        add_subdirectory(csharp)
    ENDIF (BUILD_CSHARP)

    IF (BUILD_LUA)
        MESSAGE(STATUS "Lua wrapper not implemented as yet, Franck is working on it.")
        #add_subdirectory(lua)
    ENDIF (BUILD_LUA)

    IF (BUILD_JAVA)
        add_subdirectory(java)
    ENDIF (BUILD_JAVA)

    IF (BUILD_MATLAB)
        MESSAGE(STATUS "Matlab wrapper not implemented as yet, Franck is working on it.")
        #add_subdirectory(matlab)
    ENDIF (BUILD_MATLAB)

    IF (BUILD_OCTAVE)
        add_subdirectory(octave)
    ENDIF (BUILD_OCTAVE)

    IF (BUILD_PERL)
        add_subdirectory(perl)
    ENDIF (BUILD_PERL)

    IF (BUILD_PYTHON2)
        add_subdirectory(python2)
    ENDIF (BUILD_PYTHON2)

    IF (BUILD_PYTHON3)
        add_subdirectory(python3)
    ENDIF (BUILD_PYTHON3)

    IF (BUILD_R)
        add_subdirectory(R)
    ENDIF (BUILD_R)

    IF (BUILD_RUBY)
        add_subdirectory(ruby)
    ENDIF (BUILD_RUBY)

    IF (BUILD_TCL)
        add_subdirectory(Tcl)
    ENDIF (BUILD_TCL)
ENDIF (SWIG_FOUND)
