#
#
# Copyright (c) 2020 Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

PROJECT(gVirtualXRay-Superbuild)

SET (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


# Set a default build type if none was specified
IF (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
      message(STATUS "Setting build type to 'Release' as none was specified.")
      SET (CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)

      # Set the possible values of build type for cmake-gui
      set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
ENDIF ()


# C++ version ##################################################################
set(CMAKE_CXX_STANDARD 11) # C++11
set(CMAKE_CXX_STANDARD_REQUIRED ON) # C++11 is required (I.e. not optional)
set(CMAKE_CXX_EXTENSIONS OFF) # without compiler extensions like gnu++11


# Flag required for shared libraries
SET (BUILD_SHARED_LIBS OFF)
IF (UNIX)
    IF (NOT APPLE)
        IF ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
            SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
            SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
        ENDIF ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
    ENDIF (NOT APPLE)
ENDIF (UNIX)


# Make include files in paths like /usr/local/include have preference before
# files in OSX frameworks.
SET (CMAKE_FIND_FRAMEWORK "LAST")


# For superbuild
SET (USE_SUPERBUILD ON)
INCLUDE(ExternalProject)
#INCLUDE(ConfigureExternalProjects)


# Options ######################################################################
INCLUDE (gvxr/cmake/options.cmake)
# SimpleGVXR has no option
INCLUDE (Wrappers/cmake/options.cmake)

# Enable SimpleGVXR if a wrapper is built
IF (BUILD_WRAPPERS)
    MESSAGE (STATUS "BUILD_WRAPPERS is ${BUILD_WRAPPERS}, SimpleGVXR will be built automatically.")
    SET (BUILD_SIMPLEGVXR ON)
# No wrapper, SimpleGVXR can still be built as an optional component
ELSE (BUILD_WRAPPERS)
    option(BUILD_SIMPLEGVXR    "Build SimpleGVXR module" ON)
    MESSAGE (STATUS "BUILD_WRAPPERS is ${BUILD_WRAPPERS}, you can still build SimpleGVXR if you'd like (default is ON)")
ENDIF (BUILD_WRAPPERS)


# CTest and Unit testing #######################################################
# Support for the CTest dashboard testing system
INCLUDE(CTest)

# Enable the unit tests
IF (BUILD_TESTING)
    ENABLE_TESTING()
ENDIF (BUILD_TESTING)


# Google Test ##################################################################
OPTION (USE_SYSTEM_GTEST "Use an external version of GoogleTest. " OFF)
MARK_AS_ADVANCED (USE_SYSTEM_GTEST)
IF (BUILD_TESTING)
    IF (USE_SYSTEM_GTEST)
        FIND_PACKAGE(GTest REQUIRED)
    ELSE(USE_SYSTEM_GTEST)
        INCLUDE(External_GTest)
        MESSAGE (STATUS "Build GTest: ON")
        list(APPEND ${CMAKE_PROJECT_NAME}_DEPENDENCIES GTest)
    ENDIF(USE_SYSTEM_GTEST)
ENDIF (BUILD_TESTING)


ADD_SUBDIRECTORY (gvxr)

# Build SimpleGVXR
IF (BUILD_SIMPLEGVXR OR BUILD_WRAPPERS)
ENDIF (BUILD_SIMPLEGVXR OR BUILD_WRAPPERS)

# Build Wrappers
IF (BUILD_WRAPPERS)
ENDIF (BUILD_WRAPPERS)
