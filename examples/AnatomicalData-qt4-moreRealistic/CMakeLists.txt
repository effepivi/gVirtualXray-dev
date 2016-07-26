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


project(gVirtualXRay-AnatomicalData-qt4-moreRealistic)

# Initialise some states by default ############################################
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/version.cmake)


# Create the RESOURCES ###########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../cmake/resources.cmake)
SET (RESOURCES_SOURCES
    ../shaders/display_gl2.frag
    ../shaders/display_gl2.vert
    ../shaders/display_gl3.frag
    ../shaders/display_gl3.vert
    ../shaders/display_image_gl2.frag
    ../shaders/display_image_gl2.vert
    ../shaders/display_image_gl3.frag
    ../shaders/display_image_gl3.vert
    ../shaders/display_gui.frag
    ../shaders/display_gui.vert
    shaders/display_realistic_gl3.frag
    shaders/display_realistic_gl3.vert
    shaders/depth_map_gl3.frag
    shaders/depth_map_gl3.vert
)
CreateHeaderFiles("." "BinaryAnatomicalDataQt4MoreRealisticFiles" "${RESOURCES_SOURCES}")


# Headers of the project #######################################################
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/data)


# Qt4 #########################################################################
FIND_PACKAGE(Qt4 REQUIRED QtGui QtOpenGL)
IF(Qt4_FOUND)
    SET(CMAKE_AUTOMOC ON)
    SET(CMAKE_INCLUDE_CURRENT_DIR ON)
    SET(optionalLibs ${optionalLibs} Qt4::QtGui Qt4::QtOpenGL)
ELSE(Qt4_FOUND)
    MESSAGE(FATAL_ERROR "Qt4 not found.")
ENDIF(Qt4_FOUND)


# Create the main program ######################################################
add_executable(demo-anatomical-data-qt4-more-realistic 
	AnatomicalData-qt4-moreRealistic.cxx  
	GLWidget.h GLWidget.cxx 
	
	AnatomicalMesh.h AnatomicalMesh.cxx
	InternalOrganMesh.h InternalOrganMesh.cxx 
	SoftTissueMesh.h SoftTissueMesh.cxx 
	
	BoneMesh.h BoneMesh.cxx 
	DiaphragmMesh.h DiaphragmMesh.cxx 
	LiverMesh.h LiverMesh.cxx 
	LungsMesh.h LungsMesh.cxx 
	SkinMesh.h SkinMesh.cxx

	DepthMap.h DepthMap.cxx)

target_link_libraries(demo-anatomical-data-qt4-more-realistic ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})

ADD_DEPENDENCIES(demo-anatomical-data-qt4-more-realistic BinaryAnatomicalDataQt4MoreRealisticFiles)

IF (NOT APPLE)
    INSTALL(TARGETS demo-anatomical-data-qt4-more-realistic RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos/bin COMPONENT demos)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../data/arial.ttf DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos/data COMPONENT demos)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos COMPONENT demos)
ELSE (NOT APPLE)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos COMPONENT demos)
    
    SET (MACOSX_RPATH 1)
    SET(AnatomicalData-qt4-moreRealistic_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/AnatomicalData-qt4-moreRealistic.cxx
        
	${CMAKE_CURRENT_SOURCE_DIR}/GLWidget.h
        ${CMAKE_CURRENT_SOURCE_DIR}/GLWidget.cxx

        ${CMAKE_CURRENT_SOURCE_DIR}/AnatomicalMesh.h
        ${CMAKE_CURRENT_SOURCE_DIR}/AnatomicalMesh.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/InternalOrganMesh.h
        ${CMAKE_CURRENT_SOURCE_DIR}/InternalOrganMesh.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/SoftTissueMesh.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SoftTissueMesh.cxx

        ${CMAKE_CURRENT_SOURCE_DIR}/BoneMesh.h
        ${CMAKE_CURRENT_SOURCE_DIR}/BoneMesh.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/DiaphragmMesh.h
        ${CMAKE_CURRENT_SOURCE_DIR}/DiaphragmMesh.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/LiverMesh.h
        ${CMAKE_CURRENT_SOURCE_DIR}/LiverMesh.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/LungsMesh.h
        ${CMAKE_CURRENT_SOURCE_DIR}/LungsMesh.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/SkinMesh.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SkinMesh.cxx

	${CMAKE_CURRENT_SOURCE_DIR}/DepthMap.h
        ${CMAKE_CURRENT_SOURCE_DIR}/DepthMap.cxx

        ${CMAKE_CURRENT_SOURCE_DIR}/../build/DragonDemo.icns
        ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/../data/arial.ttf
    )

    ADD_EXECUTABLE (AnatomicalData-qt4-moreRealistic MACOSX_BUNDLE ${AnatomicalData-qt4-moreRealistic_SRCS})
    TARGET_LINK_LIBRARIES (AnatomicalData-qt4-moreRealistic gVirtualXRay ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(AnatomicalData-qt4-moreRealistic BinaryAnatomicalDataQt4MoreRealisticFiles)

    set(MACOSX_BUNDLE_BUNDLE_NAME AnatomicalData-qt4-moreRealistic)
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "AnatomicalData-qt4-moreRealistic")
    SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/../build/DragonDemo.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set(MACOSX_BUNDLE_ICON_FILE DragonDemo.icns)
    set(MACOSX_BUNDLE_INFO_STRING "${MAIN_PROJECT_VERSION}, ${COPYRIGHT_STRING}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${gVirtualXRay_VERSION_MAJOR}.${gVirtualXRay_VERSION_MINOR}")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${MAIN_PROJECT_VERSION}")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "${MAIN_PROJECT_VERSION}")
    set(MACOSX_BUNDLE_COPYRIGHT "${COPYRIGHT_STRING}")

    SET(APPS "\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos/AnatomicalData-qt4-moreRealistic.app")

    INSTALL(TARGETS AnatomicalData-qt4-moreRealistic 
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
