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


PROJECT (gvxrWrappers)

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
    add_library(gvxrWrappers gvxrWrapper.cxx)
    target_link_libraries(gvxrWrappers ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})

    # Set up public headers for swig-example
    set_target_properties(gvxrWrappers PROPERTIES PUBLIC_HEADER "gvxrWrappers.h")

	ADD_DEPENDENCIES(gvxrWrappers BinaryWrapper)

	IF (gVirtualXRay_BINARY_DIR)
	     ADD_DEPENDENCIES (gVirtualXRay gvxrWrappers)
	ENDIF (gVirtualXRay_BINARY_DIR)

    install(TARGETS gvxrWrappers
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
    #option(BUILD_PERL    "Build Perl SWIG module" OFF)
    #option(BUILD_PHP7    "Build PHP7 SWIG module" OFF)
    option(BUILD_PYTHON2 "Build Python2 SWIG module" OFF)
    option(BUILD_PYTHON3 "Build Python3 SWIG module" OFF)
    option(BUILD_R       "Build R SWIG module" OFF)
    option(BUILD_RUBY    "Build Ruby SWIG module" OFF)
    #option(BUILD_SCILAB  "Build Scilab SWIG module" ON)
    #option(BUILD_TCL    "Build TCL SWIG module" OFF)
    
    #    FindTCL.cmake
    #    FindPerl.cmake
    #    FindPerlLibs.cmake
    #    FindLua.cmake
    #    FindMatlab.cmake
    #    FindJava.cmake

    IF (BUILD_PYTHON2 AND BUILD_PYTHON3)
        MESSAGE(FATAL_ERROR "Both variables BUILD_PYTHON2 and BUILD_PYTHON3 are ON. Choose either Python2 or Python3, not both.")
    ENDIF (BUILD_PYTHON2 AND BUILD_PYTHON3)

    IF (BUILD_JAVA)
        add_subdirectory(java)
    ENDIF (BUILD_JAVA)

    IF (BUILD_OCTAVE)
        add_subdirectory(octave)
    ENDIF (BUILD_OCTAVE)

    IF (BUILD_PHP7)
    #    add_subdirectory(php7)
    ENDIF (BUILD_PHP7)

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

    IF (BUILD_SCILAB)
        #add_subdirectory(scilab)
    ENDIF (BUILD_SCILAB)





    #FIND_PACKAGE(PythonLibs)
    #INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

    #INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

    #SET(CMAKE_SWIG_FLAGS "")

    #SWIG_LINK_LIBRARIES(gvxrPythonWrapper ${PYTHON_LIBRARIES})
ENDIF (SWIG_FOUND)

#FIND_PACKAGE(PythonInterp 3)

#if (PYTHONINTERP_FOUND)
#    FIND_PACKAGE(Boost COMPONENTS python-py3 numpy-py3)
#    FIND_PACKAGE(PythonLibs 3)

#    IF (PythonLibs_FOUND)
#        IF (Boost_FOUND)
#        	IF (NOT "${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}" VERSION_LESS "1.63")
        
#	            message(STATUS "Build bindings for Python-${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}")
	
	
#	            INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS})
	            
#	            PYTHON_ADD_MODULE(gvxrPython gvxrPython.cxx)
#	            target_link_libraries(gvxrPython ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs} ${PYTHON_LIBRARIES} ${Boost_LIBRARIES}
#	)
	
#	            FILE(COPY gvxrPythonLoadStlFile.py                   DESTINATION .)
#	            FILE(COPY gvxrPythonLoadSceneGraphFile.py            DESTINATION .)
#	            FILE(COPY ../../examples/data/welsh-dragon-small.stl DESTINATION .)


#            ELSE ()
#                message(WARNING "Boost (${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}) is too old. Use at least 1.63")
#            ENDIF ()
#        ELSE (Boost_FOUND)
#            message(WARNING "Boost::Python not found")
#        ENDIF (Boost_FOUND)
#    ELSE (PythonLibs_FOUND)
#        message(WARNING "PythonLibs not found")
#    ENDIF (PythonLibs_FOUND)
# else (PYTHONINTERP_FOUND)
#    message(WARNING "Python not found")
#endif(PYTHONINTERP_FOUND)

#message(STATUS "PYTHON_LIBRARIES = ${PYTHON_LIBRARIES}")
#message(STATUS "PYTHON_EXECUTABLE = ${PYTHON_EXECUTABLE}")
#message(STATUS "PYTHON_INCLUDE_DIRS = ${PYTHON_INCLUDE_DIRS}")
#message(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")

# Check if GLFW is enable #####################################################
#IF (glfw3_FOUND)
#    ADD_DEFINITIONS(-DHAS_GLFW)
#ELSE (glfw3_FOUND)
#	MESSAGE (WARNING "GLFW3 not found, the tool (gvxrWrappers) will be disable.")
#ENDIF (glfw3_FOUND)


# Create the main program ######################################################
#ADD_EXECUTABLE (gvxrDicomSeriesToVolume gvxrDicomSeriesToVolume.cxx cmdline.h cmdline.c)
#TARGET_LINK_LIBRARIES (gvxrDicomSeriesToVolume ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})


#INSTALL (TARGETS gvxrDicomSeriesToVolume RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/bin  COMPONENT tools)
#INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/bin/ COMPONENT tools)

#SET (CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt)

#INCLUDE (CPack)
