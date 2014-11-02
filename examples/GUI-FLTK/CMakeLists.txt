project(gVirtualXRay-GUI-FLTK)

# Initialise some states by default ############################################
set(CMAKE_BUILD_TYPE Release CACHE STRING "Debug|Release|RelWithDebInfo|MinSizeRel")
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

