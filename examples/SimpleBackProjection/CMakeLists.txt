project(gVirtualXRay-SimpleBackProjection)

# Initialise some states by default ############################################
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../../build/version.cmake)


# Create the RESOURCES ###########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../resources.cmake)
CreateHeaderFiles("." "BinarySimpleBackProjectionFiles" "${RESOURCES_SOURCES}")


# Headers of the project #######################################################
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})


# Configure the project ########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../config.cmake)


# Check if GLFW is enable #####################################################
IF(NOT glfw_FOUND)
    MESSAGE(FATAL_ERROR "GLFW was not found. The demo cannot be built.")
ENDIF(NOT glfw_FOUND)


# ITK #########################################################################
FIND_PACKAGE (ITK)
IF (ITK_FOUND)
	ADD_DEFINITIONS(-DHAS_ITK)
	INCLUDE( ${ITK_USE_FILE} )
	set (optionalLibs ${optionalLibs} ${ITK_LIBRARIES})
ELSE(ITK_FOUND)
    MESSAGE(FATAL_ERROR "ITK not found. The demo cannot be built.")
ENDIF(ITK_FOUND)


# Create the main program ######################################################
add_executable(demo-simple-back-projection SimpleBackProjection.cxx)
target_link_libraries(demo-simple-back-projection ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})
ADD_DEPENDENCIES(welsh_dragon_glut BinarySimpleBackProjectionFiles)
INSTALL(TARGETS demo-simple-back-projection RUNTIME DESTINATION demos/SimpleBackProjection COMPONENT demos)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION demos/SimpleBackProjection COMPONENT demos)

IF (APPLE)
    SET (MACOSX_RPATH 1)
    SET(SimpleBackProjection_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/SimpleBackProjection.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/../build/DragonDemo.icns
        ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/../data/cylinder_bin.stl
    )

    ADD_EXECUTABLE (SimpleBackProjection MACOSX_BUNDLE ${SimpleBackProjection_SRCS})
    TARGET_LINK_LIBRARIES (SimpleBackProjection gVirtualXRay ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(SimpleBackProjection BinarySimpleBackProjectionFiles)

    set(MACOSX_BUNDLE_BUNDLE_NAME SimpleBackProjection)
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "SimpleBackProjection")
    SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/../build/DragonDemo.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set(MACOSX_BUNDLE_ICON_FILE DragonDemo.icns)
    set(MACOSX_BUNDLE_INFO_STRING "${MAIN_PROJECT_VERSION}, ${COPYRIGHT_STRING}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${gVirtualXRay_VERSION_MAJOR}.${gVirtualXRay_VERSION_MINOR}")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${MAIN_PROJECT_VERSION}")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "${MAIN_PROJECT_VERSION}")
    set(MACOSX_BUNDLE_COPYRIGHT "${COPYRIGHT_STRING}")

    SET(APPS "\${CMAKE_INSTALL_PREFIX}/SimpleBackProjection.app")

    INSTALL(TARGETS SimpleBackProjection 
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
