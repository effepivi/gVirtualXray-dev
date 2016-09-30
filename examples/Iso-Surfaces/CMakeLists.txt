#
#
# Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


project(gVirtualXRay-Iso-Surfaces)

# Initialise some states by default ############################################
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/version.cmake)


# Headers of the project #######################################################
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})


# Configure the project ########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/configureGlfw.cmake)


# Create the RESOURCES ###########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../cmake/resources.cmake)
CreateHeaderFiles("." "BinaryIsoSurfaceFiles" "${RESOURCES_SOURCES}")


# Copy data files into the build directory (useful for debugging) #############
IF (WIN32)
    CONFIGURE_FILE (../data/arial.ttf ${CMAKE_CURRENT_BINARY_DIR}/../data/arial.ttf COPYONLY)
ELSE (WIN32)
    CONFIGURE_FILE (../data/arial.ttf ${CMAKE_CURRENT_BINARY_DIR}/../data/arial.ttf COPYONLY)
ENDIF (WIN32)


# Check if GLFW is enable #####################################################
IF (USE_SYSTEM_GLFW)
    FIND_PACKAGE( glfw3   REQUIRED )
ENDIF (USE_SYSTEM_GLFW)

IF (glfw3_FOUND)
    INCLUDE_DIRECTORIES(${GLFW3_INCLUDE_DIR})
    SET(requiredLibs ${requiredLibs} ${GLFW3_LIBRARY})
ELSE (glfw3_FOUND)
    MESSAGE(WARNING "GLFW3 not found, the demo (${PROJECT_NAME}) will be disable.")
ENDIF (glfw3_FOUND)


# Create the main program ######################################################
add_executable(test_implicit_surface test_implicit_surface.cxx)
target_link_libraries(test_implicit_surface ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})
ADD_DEPENDENCIES(test_implicit_surface BinaryIsoSurfaceFiles)

    if (gVirtualXRay_BINARY_DIR)
         ADD_DEPENDENCIES(test_implicit_surface   gVirtualXRay)
    endif (gVirtualXRay_BINARY_DIR)

IF (NOT APPLE)
    INSTALL(TARGETS test_implicit_surface RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos/bin  COMPONENT demos)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../data/arial.ttf DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos/data COMPONENT demos)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos COMPONENT demos)
ELSE (NOT APPLE)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos COMPONENT demos)

    SET (MACOSX_RPATH 1)
    SET(Iso-Surfaces_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/test_implicit_surface.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/../build/DragonDemo.icns
        ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/../data/arial.ttf
    )

    ADD_EXECUTABLE (Iso-Surfaces MACOSX_BUNDLE ${Iso-Surfaces_SRCS})
    TARGET_LINK_LIBRARIES (Iso-Surfaces gVirtualXRay ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(Iso-Surfaces BinaryGUIFLTKFiles)

    set(MACOSX_BUNDLE_BUNDLE_NAME Iso-Surfaces)
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "Iso-Surfaces")
    SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/../build/DragonDemo.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set(MACOSX_BUNDLE_ICON_FILE DragonDemo.icns)
    set(MACOSX_BUNDLE_INFO_STRING "${MAIN_PROJECT_VERSION}, ${COPYRIGHT_STRING}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${gVirtualXRay_VERSION_MAJOR}.${gVirtualXRay_VERSION_MINOR}")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${MAIN_PROJECT_VERSION}")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "${MAIN_PROJECT_VERSION}")
    set(MACOSX_BUNDLE_COPYRIGHT "${COPYRIGHT_STRING}")

    SET(APPS "\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos/Iso-Surfaces.app")

    INSTALL(TARGETS Iso-Surfaces 
        BUNDLE DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos COMPONENT Runtime
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos/bin COMPONENT Runtime
    )

    SET_SOURCE_FILES_PROPERTIES (
        ${CMAKE_CURRENT_SOURCE_DIR}/../data/arial.ttf
        ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt
        PROPERTIES
        MACOSX_PACKAGE_LOCATION data
    )

    # directories to look for dependencies
    SET (DIRS /sw/lib /usr/local/lib ${Boost_LIBRARY_DIR})

    SET (LIBRARIES ${requiredLibs} ${optionalLibs})
    INSTALL(CODE "
        include(BundleUtilities)
        fixup_bundle(\"${APPS}\" \"\${LIBRARIES}\" \"${DIRS}\")
        " COMPONENT Runtime)

    set(CPACK_BINARY_DRAGNDROP ON)
ENDIF(NOT APPLE)

SET (CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt)

include(CPack)
