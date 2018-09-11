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
	# Create the RESOURCES ###########################################################
	INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/cmake/CreateHeaderFiles.cmake)
	SET (RESOURCES_SOURCES display_gl2.vert display_gl2.frag display_gl3.vert display_gl3.frag)
	CreateHeaderFiles("cmake" "BinaryWrapper" "${RESOURCES_SOURCES}")

    INCLUDE (${SWIG_USE_FILE})

    SET_PROPERTY(SOURCE gvxrWrapper.i PROPERTY CPLUSPLUS ON)

    option(BUILD_PYTHON2 "Build Python2 SWIG module" ON)
    option(BUILD_PYTHON3 "Build Python3 SWIG module" ON)
    option(BUILD_OCTAVE "Build Octave SWIG module" ON)

    #IF (BUILD_PYTHON2)
    #    find_package(PythonLibs 2 REQUIRED)
    #    include_directories(${PYTHON_INCLUDE_PATH})
    #    SWIG_ADD_LIBRARY(gvxrPythonWrapper2 LANGUAGE python SOURCES gvxrWrapper.i gvxrWrapper.cxx)
    #    SWIG_LINK_LIBRARIES(gvxrPythonWrapper2 ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs} ${PYTHON_LIBRARIES})
	#	ADD_DEPENDENCIES(gvxrPythonWrapper2 BinaryWrapper)
    #ENDIF (BUILD_PYTHON2)
    
    IF (BUILD_PYTHON3)
        find_package(PythonLibs 3 REQUIRED)
        include_directories(${PYTHON_INCLUDE_PATH})
        SWIG_ADD_LIBRARY(gvxrPythonWrapper3 LANGUAGE python SOURCES gvxrWrapper.i gvxrWrapper.cxx)
        SWIG_LINK_LIBRARIES(gvxrPythonWrapper3 ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs} ${PYTHON_LIBRARIES})
		#ADD_DEPENDENCIES(gvxrPythonWrapper3 BinaryWrapper)
    ENDIF (BUILD_PYTHON3)

    #IF (BUILD_OCTAVE)
    #    find_package(Octave REQUIRED)
    #    include_directories(${OCTAVE_INCLUDE_DIRS})
    #    SWIG_ADD_LIBRARY(gvxrOctaveWrapper LANGUAGE octave SOURCES gvxrWrapper.i gvxrWrapper.cxx)
    #    SWIG_LINK_LIBRARIES(gvxrOctaveWrapper ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs} ${OCTAVE_LIBRARIES})
	#	ADD_DEPENDENCIES(gvxrOctaveWrapper BinaryWrapper)
    #ENDIF (BUILD_OCTAVE)



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

#				IF (gVirtualXRay_BINARY_DIR)
#				     ADD_DEPENDENCIES (gvxrPython   gVirtualXRay)
#				ENDIF (gVirtualXRay_BINARY_DIR)

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
#	MESSAGE (WARNING "GLFW3 not found, the tool (${PROJECT_NAME}) will be disable.")
#ENDIF (glfw3_FOUND)


# Create the main program ######################################################
#ADD_EXECUTABLE (gvxrDicomSeriesToVolume gvxrDicomSeriesToVolume.cxx cmdline.h cmdline.c)
#TARGET_LINK_LIBRARIES (gvxrDicomSeriesToVolume ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})


#INSTALL (TARGETS gvxrDicomSeriesToVolume RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/bin  COMPONENT tools)
#INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/bin/ COMPONENT tools)

#SET (CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt)

#INCLUDE (CPack)
