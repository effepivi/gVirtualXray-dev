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


PROJECT (SimpleGVXR)

INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/version.cmake)


# Initialise some states by default ############################################
SET (BUILD_SHARED_LIBS OFF)


# Minimum CMake version ########################################################
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 11) # C++11
set(CMAKE_CXX_STANDARD_REQUIRED ON) # C++11 is required (I.e. not optional)
set(CMAKE_CXX_EXTENSIONS OFF) # without compiler extensions like gnu++11

OPTION(USE_SYSTEM_gVirtualXRay   "Use System gVirtualXRay"      OFF)


# for superbuild ###############################################################
IF (NOT USE_SYSTEM_gVirtualXRay)
		SET (USE_SUPERBUILD ON)
		INCLUDE(ExternalProject)
ENDIF (NOT USE_SYSTEM_gVirtualXRay)


# Find gVirtualXRay ############################################################
IF (NOT USE_SYSTEM_gVirtualXRay)

		#INCLUDE(cmake/Build_gVirtualXRay.cmake)
		#build_gVirtualXRay( gVirtualXRay ${PROJECT_BINARY_DIR} )


		message(STATUS ${CMAKE_SOURCE_DIR}/cmake/gVirtualXRay)
		# The first external project will be built at *configure stage*
		file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/gVirtualXRay)

		execute_process(
		    COMMAND ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR}/cmake/gVirtualXRay
		    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/gVirtualXRay
				RESULT_VARIABLE gvxr_config_result
				)

		message(STATUS "gVirtualXRay config result: ${gvxr_config_result}")

		IF (MSVC)
				execute_process(
				    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/gVirtualXRay --target gVirtualXRay
				    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/gVirtualXRay
						RESULT_VARIABLE gvxr_build_result
						)
		ELSE (MSVC)
				execute_process(
				    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/gVirtualXRay --target gVirtualXRay -- -j12
				    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/gVirtualXRay
						RESULT_VARIABLE gvxr_build_result
						)
		ENDIF (MSVC)

		message(STATUS "gVirtualXRay build result: ${gvxr_build_result}")

		SET (gVirtualXRay_DIR ${CMAKE_BINARY_DIR}/gVirtualXRay/install/gVirtualXRay-1.1.0)
		message(STATUS "gVirtualXRay install: ${gVirtualXRay_DIR}")

ENDIF (NOT USE_SYSTEM_gVirtualXRay)

FIND_PACKAGE(gVirtualXRay REQUIRED)
INCLUDE(${gVirtualXRay_USE_FILE})








OPTION(USE_SYSTEM_GLFW       "Use System GLFW"       OFF)









# Create the RESOURCES #########################################################
FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Bin2C.cmake DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/cmake)
SET (RESOURCES_SOURCES
		"${CMAKE_CURRENT_SOURCE_DIR}/display_gl2.vert"
		"${CMAKE_CURRENT_SOURCE_DIR}/display_gl2.frag"
		"${CMAKE_CURRENT_SOURCE_DIR}/display_gl3.vert"
		"${CMAKE_CURRENT_SOURCE_DIR}/display_gl3.frag"
)

INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/cmake/CreateHeaderFiles.cmake)
CreateHeaderFiles("cmake" "BinaryWrapper" "${RESOURCES_SOURCES}")


# Headers of the project #######################################################
SET (SimpleGVXR_BASE_DIR       ${CMAKE_CURRENT_SOURCE_DIR})
SET (SimpleGVXR_INCLUDE_DIR    ${CMAKE_CURRENT_SOURCE_DIR})
SET (SimpleGVXR_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_BINARY_DIR}")


# Create the library ###########################################################
add_library(SimpleGVXR SimpleGVXR.cxx)




IF (NOT USE_SYSTEM_GLFW)
		SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
		INCLUDE(cmake/BuildGlfw)
		build_glfw( glfw ${PROJECT_BINARY_DIR} )
		ADD_DEPENDENCIES(SimpleGVXR glfw)

ENDIF (NOT USE_SYSTEM_GLFW)


include(cmake/configureGlfw)


target_link_libraries(SimpleGVXR ${GLFW3_LIBRARY} ${gVirtualXRay_LIBRARY} ${gVirtualXRay_LIBRARIES})

ADD_DEPENDENCIES(SimpleGVXR BinaryWrapper)



SET (SimpleGVXR_LIBRARY ${PROJECT_BINARY_DIR}/libSimpleGVXR.a)


IF (UNIX)
	IF (APPLE)

	    SET (requiredLibs ${requiredLibs} "-framework CoreFoundation")

	    IF(${CMAKE_GENERATOR} STREQUAL "Xcode")
	        SET (SimpleGVXR_LIBRARY
	            optimized ${PROJECT_BINARY_DIR}/Release/libSimpleGVXR.a
	            debug     ${PROJECT_BINARY_DIR}/Debug/libSimpleGVXR.a
	        )
	    ENDIF ()
	ELSE (APPLE)
		SET (requiredLibs ${requiredLibs} dl)
	ENDIF (APPLE)
ENDIF ()

if (WIN32 AND ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    SET (SimpleGVXR_LIBRARY optimized ${PROJECT_BINARY_DIR}/Release/SimpleGVXR.lib
                              debug     ${PROJECT_BINARY_DIR}/Debug/SimpleGVXR.lib
    )
ENDIF ()
SET (SimpleGVXR_LIBRARIES ${optionalLibs} ${SimpleGVXR_LIBRARY} ${gVirtualXRay_LIBRARY} ${gVirtualXRay_LIBRARIES})


# Set up public headers for SimpleGVXR #########################################
set_target_properties(SimpleGVXR PROPERTIES PUBLIC_HEADER "SimpleGVXR.h")
SET_PROPERTY(SOURCE SimpleGVXR-doc.i PROPERTY CPLUSPLUS ON)


# Generate SimpleGVXRConfig.cmake ############################################
set(SimpleGVXR_USE_FILE "${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/UseSimpleGVXR.cmake")
configure_file(SimpleGVXRConfig.cmake.in CMakeFiles/SimpleGVXRConfig.cmake @ONLY)

install(FILES
		${CMAKE_CURRENT_SOURCE_DIR}/UseSimpleGVXR.cmake
		${CMAKE_CURRENT_SOURCE_DIR}/SimpleGVXR-doc.i
		${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/SimpleGVXRConfig.cmake
    DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}
    COMPONENT Development)


# Installation #################################################################
install(TARGETS SimpleGVXR
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/bin
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/lib
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/include)
