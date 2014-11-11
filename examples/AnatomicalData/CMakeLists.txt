project(gVirtualXRay-AnatomicalData)

# Initialise some states by default ############################################
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../../build/version.cmake)


# Create the RESOURCES ###########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../resources.cmake)
CreateHeaderFiles("." "BinaryAnatomicalDataFiles" "${RESOURCES_SOURCES}")


# Headers of the project #######################################################
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})


# Configure the project ########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../config.cmake)


# Check if GLFW is enable #####################################################
IF(NOT glfw_FOUND)
    MESSAGE(FATAL_ERROR "GLFW was not found. The demo cannot be built.")
ENDIF(NOT glfw_FOUND)


# Create the main program ######################################################
add_executable(demo-anatomical-data AnatomicalData.cxx)
target_link_libraries(demo-anatomical-data ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})
ADD_DEPENDENCIES(demo-anatomical-data BinaryAnatomicalDataFiles)
INSTALL(TARGETS demo-anatomical-data RUNTIME DESTINATION demos/AnatomicalData/bin  COMPONENT demos)
#INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../data/cylinder_bin.stl DESTINATION demos/AnatomicalData/data COMPONENT demos)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION demos/AnatomicalData COMPONENT demos)

IF (APPLE)
    SET (MACOSX_RPATH 1)
    SET(AnatomicalData_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/AnatomicalData.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/../build/DragonDemo.icns
        ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/data/skin_bin.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/spine.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/liver.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/lung.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/diaph4.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib1L.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib1R.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib2L.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib2R.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib3L.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib3R.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib4L.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib4R.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib5L.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib5R.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib6L.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib6R.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib7L.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib7R.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib8L.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib8R.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib9L.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib9R.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib10L.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib10R.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib11L.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib11R.stl
    )

    ADD_EXECUTABLE (AnatomicalData MACOSX_BUNDLE ${AnatomicalData_SRCS})
    TARGET_LINK_LIBRARIES (AnatomicalData gVirtualXRay ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(AnatomicalData BinaryAnatomicalDataFiles)

    set(MACOSX_BUNDLE_BUNDLE_NAME AnatomicalData)
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "AnatomicalData")
    SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/../build/DragonDemo.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set(MACOSX_BUNDLE_ICON_FILE DragonDemo.icns)
    set(MACOSX_BUNDLE_INFO_STRING "${MAIN_PROJECT_VERSION}, ${COPYRIGHT_STRING}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${gVirtualXRay_VERSION_MAJOR}.${gVirtualXRay_VERSION_MINOR}")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${MAIN_PROJECT_VERSION}")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "${MAIN_PROJECT_VERSION}")
    set(MACOSX_BUNDLE_COPYRIGHT "${COPYRIGHT_STRING}")

    SET(APPS "\${CMAKE_INSTALL_PREFIX}/AnatomicalData.app")

    INSTALL(TARGETS AnatomicalData 
        BUNDLE DESTINATION . COMPONENT Runtime
        RUNTIME DESTINATION bin COMPONENT Runtime
    )

    SET_SOURCE_FILES_PROPERTIES (
        ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/data/skin_bin.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/spine.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/liver.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/lung.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/diaph4.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib1L.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib1R.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib2L.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib2R.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib3L.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib3R.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib4L.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib4R.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib5L.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib5R.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib6L.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib6R.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib7L.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib7R.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib8L.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib8R.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib9L.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib9R.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib10L.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib10R.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib11L.stl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/rib11R.stl
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
ENDIF(APPLE)

SET (CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt)

include(CPack)
