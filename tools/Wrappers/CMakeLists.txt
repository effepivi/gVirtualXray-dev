#
#
# Copyright (c) 2018, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


PROJECT (SimpleGVXR)

# Initialise some states by default ############################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/version.cmake)
SET (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


# Configure the project ########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/configureGlfw.cmake)


# Headers of the project #######################################################
INCLUDE_DIRECTORIES (${CMAKE_CURRENT_BINARY_DIR})


FIND_PACKAGE(SWIG)

IF (SWIG_FOUND)
    INCLUDE (${SWIG_USE_FILE})

	# Create the RESOURCES ###########################################################
	INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/cmake/CreateHeaderFiles.cmake)
	SET (RESOURCES_SOURCES display_gl2.vert display_gl2.frag display_gl3.vert display_gl3.frag)
	CreateHeaderFiles("cmake" "BinaryWrapper" "${RESOURCES_SOURCES}")

    # Create main and link to liba
    add_library(SimpleGVXR SimpleGVXR.cxx)
    target_link_libraries(SimpleGVXR ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})

    # Set up public headers for swig-example
    set_target_properties(SimpleGVXR PROPERTIES PUBLIC_HEADER "SimpleGVXR.h")

	ADD_DEPENDENCIES(SimpleGVXR BinaryWrapper)

	IF (gVirtualXRay_BINARY_DIR)
	     ADD_DEPENDENCIES (gVirtualXRay SimpleGVXR)
	ENDIF (gVirtualXRay_BINARY_DIR)

    install(TARGETS SimpleGVXR
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/bin
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/lib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/lib/static
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/include)


    set(CMAKE_SWIG_FLAGS "")
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})

    option(BUILD_JAVA    "Build Java SWIG module" OFF)
    #option(BUILD_LUA    "Build Lua SWIG module" OFF)
    #option(BUILD_MATLAB    "Build Matlab SWIG module" OFF)
    option(BUILD_OCTAVE  "Build Octave SWIG module" OFF)
    option(BUILD_PERL    "Build Perl SWIG module" OFF)
    option(BUILD_PYTHON2 "Build Python2 SWIG module" OFF)
    option(BUILD_PYTHON3 "Build Python3 SWIG module" OFF)
    option(BUILD_R       "Build R SWIG module" OFF)
    option(BUILD_RUBY    "Build Ruby SWIG module" OFF)
    #option(BUILD_TCL    "Build TCL SWIG module" OFF)
    
    #    FindTCL.cmake
    #    FindLua.cmake
    #    FindMatlab.cmake

    IF (BUILD_PYTHON2 AND BUILD_PYTHON3)
        MESSAGE(FATAL_ERROR "Both variables BUILD_PYTHON2 and BUILD_PYTHON3 are ON. Choose either Python2 or Python3, not both.")
    ENDIF (BUILD_PYTHON2 AND BUILD_PYTHON3)

    IF (BUILD_LUA)
        add_subdirectory(lua)
    ENDIF (BUILD_LUA)

    IF (BUILD_JAVA)
        add_subdirectory(java)
    ENDIF (BUILD_JAVA)

    IF (BUILD_MATLAB)
        add_subdirectory(matlab)
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

    #IF (BUILD_TCL)
        #add_subdirectory(tcl)
    #ENDIF (BUILD_TCL)
ENDIF (SWIG_FOUND)

