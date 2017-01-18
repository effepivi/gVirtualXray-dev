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


project(gVirtualXRay-ArtefactInTomographyReconstruction)

# Initialise some states by default ############################################
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/version.cmake)


# Create the RESOURCES ###########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../cmake/resources.cmake)

SET (RESOURCES_SOURCES ${RESOURCES_SOURCES}
    STL/atlas_C1.stl   
    STL/axis_C2.stl   
    STL/brain.stl   
    STL/cervical_vertebra_C3.stl   
    STL/cervical_vertebra_C4.stl   
    STL/cervical_vertebra_C5.stl   
    STL/cervical_vertebra_C6.stl   
    STL/cranium.stl   
    STL/frontal_sinus.stl
    STL/grey_matter.stl
    STL/hyoid_bone.stl
    STL/larynx.stl
    STL/left_eye.stl
    STL/left_lens.stl
    STL/left_maxillary_sinus.stl
    STL/left_palatine_tonsil.stl
    STL/left_styloid_process.stl
    STL/lingual_tonsil.stl
    STL/mandible.stl
    STL/right_eye.stl
    STL/right_lens.stl
    STL/right_maxillary_sinus.stl
    STL/right_palatine_tonsil.stl
    STL/right_styloid_process.stl
    STL/thyroid_gland.stl   
    )


CreateHeaderFiles("." "BinaryArtefactInTomographyReconstructionFiles" "${RESOURCES_SOURCES}")


# Headers of the project #######################################################
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})


# Configure the project ########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/configureGlfw.cmake)


# Copy data files into the build directory (useful for debugging) #############
IF (WIN32)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/beam_spectrum.csv              ${CMAKE_CURRENT_BINARY_DIR}/data/beam_spectrum.csv COPYONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../data/arial.ttf              ${CMAKE_CURRENT_BINARY_DIR}/data/arial.ttf COPYONLY)
ELSE (WIN32)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/beam_spectrum.csv              ${CMAKE_CURRENT_BINARY_DIR}/../data/beam_spectrum.csv COPYONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../data/arial.ttf              ${CMAKE_CURRENT_BINARY_DIR}/../data/arial.ttf COPYONLY)
ENDIF (WIN32)


# Check if GLFW is enable #####################################################
IF (NOT glfw3_FOUND)
    MESSAGE(WARNING "GLFW3 not found, the demo (${PROJECT_NAME}) will be disable.")
ENDIF (NOT glfw3_FOUND)


FIND_LIBRARY(FFTW3_LIBRARIES NAMES fftw3 libfftw3 libfftw3_omp)
IF (FFTW3_LIBRARIES)
    SET(requiredLibs ${requiredLibs} ${FFTW3_LIBRARIES})
ELSE ()
    MESSAGE(WARNING "FFTW3 not found, the demo (${PROJECT_NAME}) will be disable.")
ENDIF ()


# Create the main program ######################################################
IF (glfw3_FOUND AND FFTW3_LIBRARIES)
add_executable(demo-artefact-in-tomography-reconstruction FilteredBackProjection.cxx)
target_link_libraries(demo-artefact-in-tomography-reconstruction ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})
ADD_DEPENDENCIES(demo-artefact-in-tomography-reconstruction BinaryArtefactInTomographyReconstructionFiles)

    if (gVirtualXRay_BINARY_DIR)
         ADD_DEPENDENCIES(demo-artefact-in-tomography-reconstruction   gVirtualXRay)
    endif (gVirtualXRay_BINARY_DIR)

IF (NOT APPLE)
    INSTALL(TARGETS demo-artefact-in-tomography-reconstruction RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos/bin COMPONENT demos)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../data/arial.ttf DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos/data COMPONENT demos)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos COMPONENT demos)
ELSE (NOT APPLE)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos COMPONENT demos)

    SET (MACOSX_RPATH 1)
    SET(ArtefactInTomographyReconstruction_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/FilteredBackProjection.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/../build/DragonDemo.icns
        ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/../data/arial.ttf
    )

    ADD_EXECUTABLE (ArtefactInTomographyReconstruction MACOSX_BUNDLE ${ArtefactInTomographyReconstruction_SRCS})
    TARGET_LINK_LIBRARIES (ArtefactInTomographyReconstruction gVirtualXRay ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(ArtefactInTomographyReconstruction BinaryArtefactInTomographyReconstructionFiles)

    set(MACOSX_BUNDLE_BUNDLE_NAME ArtefactInTomographyReconstruction)
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "ArtefactInTomographyReconstruction")
    SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/../build/DragonDemo.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set(MACOSX_BUNDLE_ICON_FILE DragonDemo.icns)
    set(MACOSX_BUNDLE_INFO_STRING "${MAIN_PROJECT_VERSION}, ${COPYRIGHT_STRING}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${gVirtualXRay_VERSION_MAJOR}.${gVirtualXRay_VERSION_MINOR}")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${MAIN_PROJECT_VERSION}")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "${MAIN_PROJECT_VERSION}")
    set(MACOSX_BUNDLE_COPYRIGHT "${COPYRIGHT_STRING}")

    SET(APPS "\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos/ArtefactInTomographyReconstruction.app")

    INSTALL(TARGETS ArtefactInTomographyReconstruction 
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
ENDIF ()

