project(gVirtualXRay-GUI-FLTK)

# Initialise some states by default ############################################
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../../build/version.cmake)


# Create the RESOURCES ###########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../resources.cmake)
CreateHeaderFiles("." "BinaryGUIFLTKFiles" "${RESOURCES_SOURCES}")


# Headers of the project #######################################################
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})


# Configure the project ########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../config.cmake)


# FLTK ########################################################################
FIND_PACKAGE(FLTK)
if(FLTK_FOUND)
        ADD_DEFINITIONS(-DHAS_FLTK)
        INCLUDE_DIRECTORIES(${FLTK_INCLUDE_DIR})
        SET(optionalLibs ${optionalLibs} ${FLTK_LIBRARIES})
ELSE(FLTK_FOUND)
        MESSAGE(FATAL_ERROR "FLTK not found.")
ENDIF(FLTK_FOUND)


# BOOST #######################################################################
FIND_PACKAGE(Boost QUIET REQUIRED COMPONENTS filesystem system)
if(Boost_FOUND)
	SET( Boost_USE_STATIC_LIBS OFF )
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
	LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
	SET(requiredLibs ${requiredLibs} ${Boost_LIBRARIES})
ELSE(Boost_FOUND)
	MESSAGE(FATAL_ERROR "Boost not found.")
ENDIF(Boost_FOUND)


# Create the GUI files with Fluid ##############################################
IF (FLTK_FOUND)
    SET (FLUID_OUTPUT_FILES ${CMAKE_CURRENT_BINARY_DIR}/gui.h ${CMAKE_CURRENT_BINARY_DIR}/gui.cxx)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
    add_custom_command(OUTPUT ${FLUID_OUTPUT_FILES}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/gui.fl
        COMMAND ${FLTK_FLUID_EXECUTABLE}
        ARGS -c ${CMAKE_CURRENT_SOURCE_DIR}/gui.fl
        COMMENT "Build the GUI using Fluid"
    )

    ADD_CUSTOM_TARGET(FluidFiles ALL echo DEPENDS ${FLUID_OUTPUT_FILES})

    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_BINARY_DIR}/gui.h)
    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_BINARY_DIR}/gui.cxx)
ENDIF (FLTK_FOUND)


# Create the main program ######################################################
IF(WIN32)
    ADD_EXECUTABLE (gVirtualXRay-gui ${CMAKE_CURRENT_BINARY_DIR}/gui.cxx OpenGLWindow.cxx DragonDemo.res)
ELSE(WIN32)
    ADD_EXECUTABLE (gVirtualXRay-gui ${CMAKE_CURRENT_BINARY_DIR}/gui.cxx OpenGLWindow.cxx)
ENDIF(WIN32)
TARGET_LINK_LIBRARIES (gVirtualXRay-gui gVirtualXRay ${optionalLibs} ${requiredLibs})
ADD_DEPENDENCIES(gVirtualXRay-gui BinaryGUIFLTKFiles)
INSTALL(TARGETS gVirtualXRay-gui RUNTIME DESTINATION demos/GUI-FLTK/bin  COMPONENT demos)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../data/welsh-dragon-small.stl DESTINATION demos/GUI-FLTK/data COMPONENT demos)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION demos/GUI-FLTK COMPONENT demos)

IF (APPLE)
    SET (MACOSX_RPATH 1)
    SET(GUI-FLTK_SRCS
        ${CMAKE_CURRENT_BINARY_DIR}/gui.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLWindow.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/../build/DragonDemo.icns
        ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/../data/welsh-dragon-small.stl
    )

    ADD_EXECUTABLE (GUI-FLTK MACOSX_BUNDLE ${GUI-FLTK_SRCS})
    TARGET_LINK_LIBRARIES (GUI-FLTK gVirtualXRay ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(GUI-FLTK BinaryGUIFLTKFiles)

    set(MACOSX_BUNDLE_BUNDLE_NAME GUI-FLTK)
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "GUI-FLTK")
    SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/../build/DragonDemo.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set(MACOSX_BUNDLE_ICON_FILE DragonDemo.icns)
    set(MACOSX_BUNDLE_INFO_STRING "${MAIN_PROJECT_VERSION}, ${COPYRIGHT_STRING}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${gVirtualXRay_VERSION_MAJOR}.${gVirtualXRay_VERSION_MINOR}")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${MAIN_PROJECT_VERSION}")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "${MAIN_PROJECT_VERSION}")
    set(MACOSX_BUNDLE_COPYRIGHT "${COPYRIGHT_STRING}")

    SET(APPS "\${CMAKE_INSTALL_PREFIX}/GUI-FLTK.app")

    INSTALL(TARGETS GUI-FLTK 
        BUNDLE DESTINATION . COMPONENT Runtime
        RUNTIME DESTINATION bin COMPONENT Runtime
    )

    SET_SOURCE_FILES_PROPERTIES (
        ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/../data/welsh-dragon-small.stl
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
