#
#
# Copyright (c) 2016, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


project(gVirtualXRay-AnatomicalData-qt5)

# Initialise some states by default ############################################
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/version.cmake)


# Create the RESOURCES ###########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../cmake/resources.cmake)
CreateHeaderFiles("." "BinaryAnatomicalDataQt5Files" "${RESOURCES_SOURCES}")


# Headers of the project #######################################################
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/data)


# Copy data files into the build directory (useful for debugging) #############
IF (WIN32)
    CONFIGURE_FILE (../data/arial.ttf ${CMAKE_CURRENT_BINARY_DIR}/../data/arial.ttf COPYONLY)
ELSE (WIN32)
    CONFIGURE_FILE (../data/arial.ttf ${CMAKE_CURRENT_BINARY_DIR}/../data/arial.ttf COPYONLY)
ENDIF (WIN32)


# Configure the project ########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../cmake/config.cmake)


# Qt5 #########################################################################
FIND_PACKAGE(Qt5Widgets)
FIND_PACKAGE(Qt5OpenGL)
    
IF(Qt5Widgets_FOUND AND Qt5OpenGL_FOUND)
    SET(CMAKE_AUTOMOC ON)
    SET(CMAKE_INCLUDE_CURRENT_DIR ON)
    SET(optionalLibs ${optionalLibs} Qt5::Widgets Qt5::OpenGL)
ELSE(Qt5Widgets_FOUND AND Qt5OpenGL_FOUND)
    MESSAGE(WARNING "Qt5 not found, the demo (${PROJECT_NAME}) will be disable.")
ENDIF(Qt5Widgets_FOUND AND Qt5OpenGL_FOUND)


# Create the main program ######################################################
IF(Qt5Widgets_FOUND AND Qt5OpenGL_FOUND)
add_executable(demo-anatomical-data-qt5 AnatomicalData-qt5.cxx GLWidget.h GLWidget.cxx)
target_link_libraries(demo-anatomical-data-qt5 ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})
ADD_DEPENDENCIES(demo-anatomical-data-qt5 BinaryAnatomicalDataQt5Files)

    if (gVirtualXRay_BINARY_DIR)
         ADD_DEPENDENCIES(demo-anatomical-data-qt5   gVirtualXRay)
    endif (gVirtualXRay_BINARY_DIR)

IF (NOT APPLE)
    INSTALL(TARGETS demo-anatomical-data-qt5 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos/bin COMPONENT demos)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../data/arial.ttf DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos/data COMPONENT demos)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos COMPONENT demos)
ELSE (NOT APPLE)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos COMPONENT demos)
    
    SET (MACOSX_RPATH 1)
    SET(AnatomicalData-qt5_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/AnatomicalData-qt5.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/GLWidget.h
        ${CMAKE_CURRENT_SOURCE_DIR}/GLWidget.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/../build/DragonDemo.icns
        ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/../data/arial.ttf
    )

    ADD_EXECUTABLE (AnatomicalData-qt5 MACOSX_BUNDLE ${AnatomicalData-qt5_SRCS})
    TARGET_LINK_LIBRARIES (AnatomicalData-qt5 gVirtualXRay ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(AnatomicalData-qt5 BinaryAnatomicalDataQt5Files)

    set(MACOSX_BUNDLE_BUNDLE_NAME AnatomicalData-qt5)
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "AnatomicalData-qt5")
    SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/../build/DragonDemo.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set(MACOSX_BUNDLE_ICON_FILE DragonDemo.icns)
    set(MACOSX_BUNDLE_INFO_STRING "${MAIN_PROJECT_VERSION}, ${COPYRIGHT_STRING}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${gVirtualXRay_VERSION_MAJOR}.${gVirtualXRay_VERSION_MINOR}")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${MAIN_PROJECT_VERSION}")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "${MAIN_PROJECT_VERSION}")
    set(MACOSX_BUNDLE_COPYRIGHT "${COPYRIGHT_STRING}")

    SET(APPS "\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos/AnatomicalData-qt5.app")

    INSTALL(TARGETS AnatomicalData-qt5 
        BUNDLE DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos COMPONENT Runtime
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos/bin COMPONENT Runtime
    )

    SET_SOURCE_FILES_PROPERTIES (
        ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/../data/arial.ttf
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
ENDIF(Qt5Widgets_FOUND AND Qt5OpenGL_FOUND)


