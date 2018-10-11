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


# Configure the project ########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/configureGlfw.cmake)


# Headers of the project #######################################################
INCLUDE_DIRECTORIES (${CMAKE_CURRENT_BINARY_DIR})


	# Create the RESOURCES ###########################################################
	INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/cmake/CreateHeaderFiles.cmake)
	SET (RESOURCES_SOURCES display_gl2.vert display_gl2.frag display_gl3.vert display_gl3.frag)
	CreateHeaderFiles("cmake" "BinaryWrapper" "${RESOURCES_SOURCES}")

    # Create main and link to liba
    add_library(SimpleGVXR SimpleGVXR.cxx)
    target_link_libraries(SimpleGVXR ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})

    # Set up public headers for swig-example
    set_target_properties(SimpleGVXR PROPERTIES PUBLIC_HEADER "SimpleGVXR.h")
    SET_PROPERTY(SOURCE SimpleGVXR-doc.i PROPERTY CPLUSPLUS ON)

	ADD_DEPENDENCIES(SimpleGVXR BinaryWrapper)

	IF (gVirtualXRay_BINARY_DIR)
	     ADD_DEPENDENCIES (SimpleGVXR gVirtualXRay)
	ENDIF (gVirtualXRay_BINARY_DIR)

    install(TARGETS SimpleGVXR
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/bin
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/lib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/lib/static
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/include)


