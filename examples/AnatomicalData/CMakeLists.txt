project(gVirtualXRay-AnatomicalData)

# Initialise some states by default ############################################
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../../build/version.cmake)


# Create the RESOURCES ###########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../resources.cmake)
CreateHeaderFiles("." "BinaryAnatomicalDataFiles" "${RESOURCES_SOURCES}")


# Headers of the project #######################################################
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/data)


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
INSTALL(TARGETS demo-anatomical-data RUNTIME DESTINATION demos/AnatomicalData COMPONENT demos)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION demos/AnatomicalData COMPONENT demos)

IF (APPLE)
    SET (MACOSX_RPATH 1)
    SET(AnatomicalData_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/AnatomicalData.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/../build/DragonDemo.icns
        ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt
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
