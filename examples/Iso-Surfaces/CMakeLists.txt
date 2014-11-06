project(gVirtualXRay-Iso-Surfaces)

# Initialise some states by default ############################################
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../../build/version.cmake)


# Create the RESOURCES ###########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../resources.cmake)
CreateHeaderFiles("." "BinaryIsoSurfaceFiles" "${RESOURCES_SOURCES}")


# Headers of the project #######################################################
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})


# Configure the project ########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../config.cmake)


# Check if GLFW is enable #####################################################
IF(NOT glfw_FOUND)
    MESSAGE(FATAL_ERROR "GLFW was not found. The demo cannot be built.")
ENDIF(NOT glfw_FOUND)


# Create the main program ######################################################
add_executable(test_implicit_surface test_implicit_surface.cxx)
target_link_libraries(test_implicit_surface ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})
ADD_DEPENDENCIES(welsh_dragon_glut BinaryIsoSurfaceFiles)
INSTALL(TARGETS test_implicit_surface RUNTIME DESTINATION demos/Iso-Surfaces/bin  COMPONENT demos)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION demos/Iso-Surfaces COMPONENT demos)

IF (APPLE)
    SET (MACOSX_RPATH 1)
    SET(Iso-Surfaces_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/test_implicit_surface.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/../build/DragonDemo.icns
        ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt
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

    SET(APPS "\${CMAKE_INSTALL_PREFIX}/Iso-Surfaces.app")

    INSTALL(TARGETS Iso-Surfaces 
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
