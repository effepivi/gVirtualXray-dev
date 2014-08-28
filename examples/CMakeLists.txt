################################################################################
#
#   @file       CMakeLists.txt
#
#   @brief      Configuration file to generate Makefiles, 
#               Project/Solution files for MS VC++, Xcode projects, etc.
#               using the CMake tool. (see http://www.cmake.org/)
#
#   @version    1.0
#
#   @date       01/12/2013
#
#   @author     Dr Franck P. Vidal
#
#   @section    License
#               BSD 3-Clause License.
#
#               For details on use and redistribution please refer
#               to http://opensource.org/licenses/BSD-3-Clause
#
#   @section    Copyright
#               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
#               http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
#               BSD 3-Clause License
#
################################################################################


SET (WEBSITE http://sourceforge.net/projects/gvirtualxray/)


# Initialise some states by default ############################################
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_BUILD_TYPE Release CACHE STRING "Debug|Release|RelWithDebInfo|MinSizeRel")


# Minimum CMake version ########################################################
cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)

IF(COMMAND cmake_policy)
       cmake_policy(SET CMP0011 NEW)
ENDIF(COMMAND cmake_policy)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})


# Project name #################################################################
project (examples)
set (examples_VERSION_MAJOR 1)
set (examples_VERSION_MINOR 0)
set (examples_VERSION_PATCH 0)


# Enable Stereo using OpenGL's quad buffering ##################################
IF(NOT APPLE)
    ADD_DEFINITIONS(-DUSE_STEREO)
ENDIF(NOT APPLE)


# Headers of the project #######################################################
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
IF(NOT APPLE)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/OpenGL4)
ENDIF(NOT APPLE)


IF (WIN32)
	ADD_DEFINITIONS(-DGLEW_STATIC)
	ADD_DEFINITIONS(-DFREEGLUT_STATIC)
    SET (CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lwinmm")
ENDIF (WIN32)


# gVirtualXRay ################################################################
find_package(gVirtualXRay REQUIRED)
IF(gVirtualXRay_FOUND)
    INCLUDE_DIRECTORIES(${gVirtualXRay_INCLUDE_DIR})
    LINK_DIRECTORIES(${gVirtualXRay_LIBRARY_DIR})
    SET(requiredLibs ${requiredLibs} ${gVirtualXRay_LIBRARY})
ELSE(gVirtualXRay_FOUND)
    MESSAGE(FATAL_ERROR "gVirtualXRay not found.")
ENDIF(gVirtualXRay_FOUND)


# ZLIB #########################################################################
FIND_PACKAGE(ZLIB REQUIRED)
IF(ZLIB_FOUND)
	INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIRS})
	SET(requiredLibs ${requiredLibs} ${ZLIB_LIBRARIES})
ELSE(ZLIB_FOUND)
	MESSAGE("ZLIB not found.")
ENDIF(ZLIB_FOUND)


# GLEW ########################################################################
FIND_PACKAGE(GLEW REQUIRED)
IF(GLEW_FOUND)
    INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})
    SET(requiredLibs ${requiredLibs} ${GLEW_LIBRARY})
ELSE(GLUT_FOUND)
    MESSAGE(FATAL_ERROR "GLEW not found.")
ENDIF(GLEW_FOUND)


# GLFW ########################################################################
FIND_PACKAGE(glfw)
IF(glfw_FOUND)
    INCLUDE_DIRECTORIES(${GLFW_INCLUDE_DIR})
    LINK_DIRECTORIES(${GLFW_LIBRARY_DIR})
    SET(optionalLibs ${optionalLibs} ${GLFW_LIBRARY} ${GLFW_LIBRARIES})

    IF (APPLE)
        FIND_LIBRARY(COCOA_LIBRARY Cocoa)
        FIND_LIBRARY(IOKIT_LIBRARY IOKit)
        FIND_LIBRARY(QUARTZ_LIBRARY Quartz)
        FIND_LIBRARY(CORE_VIDEO_LIBRARY CoreVideo)

        SET(optionalLibs ${optionalLibs} ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${QUARTZ_LIBRARY} ${CORE_VIDEO_LIBRARY})
    ENDIF(APPLE)

    IF (UNIX)
        FIND_PACKAGE (X11 REQUIRED)
        iF (X11_FOUND)
            INCLUDE_DIRECTORIES(${X11_INCLUDE_DIRS})
            SET(optionalLibs ${optionalLibs} ${X11_X11_LIB} ${X11_Xrandr_LIB} ${X11_Xinput_LIB} ${X11_Xxf86vm_LIB})
        ELSE (X11_FOUND)
            MESSAGE(FATAL_ERROR "X11 not found.")
        ENDIF (X11_FOUND)
    ENDIF(UNIX)
ELSE(glfw_FOUND)
    MESSAGE(WARNING "GLFW not found.")
ENDIF(glfw_FOUND)


# GLUT ########################################################################
FIND_PACKAGE(GLUT)
IF(GLUT_FOUND)
    INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})
    SET(optionalLibs ${optionalLibs} ${GLUT_LIBRARIES})
ELSE(GLUT_FOUND)
    MESSAGE(WARNING "GLUT not found.")
ENDIF(GLUT_FOUND)


# FLTK ########################################################################
FIND_PACKAGE(FLTK)
if(FLTK_FOUND)
        ADD_DEFINITIONS(-DHAS_FLTK)
        INCLUDE_DIRECTORIES(${FLTK_INCLUDE_DIR})
        SET(optionalLibs ${optionalLibs} ${FLTK_LIBRARIES})
ELSE(FLTK_FOUND)
        MESSAGE(WARNING "FLTK not found, it will be disable.")
ENDIF(FLTK_FOUND)


# OpenGL ######################################################################
FIND_PACKAGE(OpenGL REQUIRED)
IF(OPENGL_FOUND)
    INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR} ${GLU_INCLUDE_PATH})
    SET(requiredLibs ${requiredLibs} ${OPENGL_LIBRARIES})
ELSE(OPENGL_FOUND)
    MESSAGE(FATAL_ERROR "OpenGL not found.")
ENDIF(OPENGL_FOUND)


# ITK ##########################################################################
FIND_PACKAGE(ITK)
IF(ITK_FOUND)
	ADD_DEFINITIONS(-DHAS_ITK)
	INCLUDE(${USE_ITK_FILE})
	SET(requiredLibs ${requiredLibs} ${ITK_LIBRARIES})

	IF(ITKVtkGlue_LOADED)
		FIND_PACKAGE(VTK REQUIRED)
		IF(VTK_FOUND)
			INCLUDE(${VTK_USE_FILE})
			LINK_DIRECTORIES(${VTK_LIBRARY_DIRS})
			SET(optionalLibs ${optionalLibs} ${VTK_LIBRARIES})
		ELSE(VTK_FOUND)
			MESSAGE(WARNING "VTK not found.")
		ENDIF(VTK_FOUND)
	ENDIF(ITKVtkGlue_LOADED)
ELSE(ITK_FOUND)
	MESSAGE(WARNING "ITK not found, tutorial 3 will be disable.")
ENDIF(ITK_FOUND)


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


# Check if GLUT is enable #####################################################
IF(NOT glfw_FOUND AND NOT GLUT_FOUND)
    MESSAGE(FATAL_ERROR "GLFW and GLUT were not found. Some examples cannot be built.")
ENDIF(NOT glfw_FOUND AND NOT GLUT_FOUND)


# Create the RESOURCES ###########################################################
SET (RESOURCES_SOURCES
    shaders/display_gl2.frag
    shaders/display_gl2.vert
    shaders/display_gl3.frag
    shaders/display_gl3.vert
    shaders/display_gui.frag
    shaders/display_gui.vert
)


FIND_PACKAGE(GZIP REQUIRED)
IF(NOT GZIP_FOUND)
    MESSAGE(FATAL_ERROR "GZIP not found.")
ENDIF(NOT GZIP_FOUND)


INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/CreateHeaderFiles.cmake)
CreateHeaderFiles("." "BinaryExampleFiles" "${RESOURCES_SOURCES}")


# Create the GUI files with Fluid ##############################################
IF (FLTK_FOUND)
    SET (FLUID_OUTPUT_FILES ${CMAKE_CURRENT_BINARY_DIR}/gui.h ${CMAKE_CURRENT_BINARY_DIR}/gui.cxx)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
    add_custom_command(OUTPUT ${FLUID_OUTPUT_FILES}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/gui.fl
        COMMAND ${FLTK_FLUID_EXECUTABLE}
        ARGS -c ${CMAKE_CURRENT_SOURCE_DIR}/src/gui.fl
        COMMENT "Build the GUI using Fluid"
    )

    ADD_CUSTOM_TARGET(FluidFiles ALL echo DEPENDS ${FLUID_OUTPUT_FILES})

    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_BINARY_DIR}/gui.h)
    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_BINARY_DIR}/gui.cxx)
ENDIF (FLTK_FOUND)


# Create the main programs #####################################################
ADD_EXECUTABLE (test_implicit_surface_density_function src/test_implicit_surface_density_function.cxx)
TARGET_LINK_LIBRARIES (test_implicit_surface_density_function gVirtualXRay ${optionalLibs} ${requiredLibs})


#IF(USE_BUNDLE)
#    REMOVE_DEFINITIONS(-DHAS_BUNDLE)
#ENDIF(USE_BUNDLE)


IF(glfw_FOUND)
    IF(ITK_FOUND)
        ADD_EXECUTABLE (implicit_surface_marching_cube src/implicit_surface_marching_cube.cxx)
        TARGET_LINK_LIBRARIES (implicit_surface_marching_cube gVirtualXRay ${optionalLibs} ${requiredLibs})
        ADD_DEPENDENCIES(implicit_surface_marching_cube BinaryExampleFiles)
        INSTALL(TARGETS implicit_surface_marching_cube RUNTIME DESTINATION examples/bin COMPONENT examples)

        ADD_EXECUTABLE (test_implicit_surface src/test_implicit_surface.cxx)
        TARGET_LINK_LIBRARIES (test_implicit_surface gVirtualXRay ${optionalLibs} ${requiredLibs})
        ADD_DEPENDENCIES(test_implicit_surface BinaryExampleFiles)
        INSTALL(TARGETS test_implicit_surface RUNTIME DESTINATION examples/bin COMPONENT examples)
    ENDIF(ITK_FOUND)

    ADD_EXECUTABLE (test_01 src/test_01.cxx)
    TARGET_LINK_LIBRARIES (test_01 gVirtualXRay ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(test_01 BinaryExampleFiles)
    INSTALL(TARGETS test_01 RUNTIME DESTINATION examples/bin COMPONENT examples)
    INSTALL(FILES data/cylinder_bin.stl DESTINATION examples/data COMPONENT examples)

    ADD_EXECUTABLE (test_mu_water src/test_mu_water.cxx)
    TARGET_LINK_LIBRARIES (test_mu_water gVirtualXRay ${optionalLibs} ${requiredLibs})

    ADD_EXECUTABLE (test_triangle_mesh src/test_triangle_mesh.cxx src/buildCube.cxx)
    TARGET_LINK_LIBRARIES (test_triangle_mesh gVirtualXRay ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(test_triangle_mesh BinaryExampleFiles)
    INSTALL(TARGETS test_triangle_mesh RUNTIME DESTINATION examples/bin COMPONENT examples)

    ADD_EXECUTABLE (welsh_dragon_glfw src/welsh_dragon_glfw.cxx)
    TARGET_LINK_LIBRARIES (welsh_dragon_glfw gVirtualXRay ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(welsh_dragon_glfw BinaryExampleFiles)
    INSTALL(TARGETS welsh_dragon_glfw RUNTIME DESTINATION examples/bin  COMPONENT examples)
    INSTALL(FILES data/welsh-dragon-small.stl DESTINATION examples/data COMPONENT examples)
ENDIF(glfw_FOUND)


IF(GLUT_FOUND)
    ADD_EXECUTABLE (welsh_dragon_glut src/welsh_dragon_glut.cxx)
    TARGET_LINK_LIBRARIES (welsh_dragon_glut gVirtualXRay ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(welsh_dragon_glut BinaryExampleFiles)
    INSTALL(TARGETS welsh_dragon_glut RUNTIME DESTINATION examples/bin  COMPONENT examples)
    INSTALL(FILES data/welsh-dragon-small.stl DESTINATION examples/data COMPONENT examples)
ENDIF(GLUT_FOUND)


IF (FLTK_FOUND)
    IF(WIN32)
        ADD_EXECUTABLE (gVirtualXRay-gui ${CMAKE_CURRENT_BINARY_DIR}/gui.cxx src/OpenGLWindow.cxx src/DragonDemo.res)
    ELSE(WIN32)
        ADD_EXECUTABLE (gVirtualXRay-gui ${CMAKE_CURRENT_BINARY_DIR}/gui.cxx src/OpenGLWindow.cxx)
    ENDIF(WIN32)
    TARGET_LINK_LIBRARIES (gVirtualXRay-gui gVirtualXRay ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(gVirtualXRay-gui BinaryExampleFiles)
    INSTALL(TARGETS gVirtualXRay-gui RUNTIME DESTINATION examples/bin COMPONENT examples)
    INSTALL(FILES ../doc/screenshots/welsh_dragon_demo/quick_reference_guide.pdf DESTINATION examples/doc  COMPONENT examples)
    INSTALL(FILES data/welsh-dragon-small.stl                                 DESTINATION examples/data COMPONENT examples)
ENDIF (FLTK_FOUND)


IF(USE_BUNDLE)
    # Set the package name
    SET (CPACK_PACKAGE_NAME "gVirtualXRay")

    # Handling of shared libraries
    #SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS library_name)
    #INCLUDE(InstallRequiredSystemLibraries)

    # Set the package version
    SET(CPACK_PACKAGE_VERSION_MAJOR ${examples_VERSION_MAJOR})
    SET(CPACK_PACKAGE_VERSION_MINOR ${examples_VERSION_MINOR})
    SET(CPACK_PACKAGE_VERSION_PATCH ${examples_VERSION_PATCH})
    set(CPACK_PACKAGE_VERSION "${examples_VERSION_MAJOR}.${examples_VERSION_MINOR}.${examples_VERSION_PATCH}")

    set(CPACK_PACKAGE_VENDOR "gVirtualXRay team")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "gVirtualXRay - Installation")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "gVirtualXRay")
    
    set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE.txt)

    SET (MACOSX_RPATH 1)
    ADD_DEFINITIONS(-DHAS_BUNDLE)

    IF (FLTK_FOUND)
        IF(WIN32)
            SET(DRAGON_DEMO_SRCS ${CMAKE_CURRENT_BINARY_DIR}/gui.cxx src/OpenGLWindow.cxx src/DragonDemo.res ../data/welsh-dragon-small.stl ../LICENSE.txt ../doc/screenshots/welsh_dragon_demo/quick_reference_guide.pdf)
        ELSE(WIN32)
            SET(DRAGON_DEMO_SRCS ${CMAKE_CURRENT_BINARY_DIR}/gui.cxx src/OpenGLWindow.cxx DragonDemo.icns ../data/welsh-dragon-small.stl ../LICENSE.txt ../doc/screenshots/welsh_dragon_demo/quick_reference_guide.pdf)
        ENDIF(WIN32)
    ELSE (FLTK_FOUND)
        SET(DRAGON_DEMO_SRCS src/welsh_dragon.cxx DragonDemo.icns ../data/welsh-dragon-small.stl ../LICENSE.txt ../doc/screenshots/welsh_dragon_demo/quick_reference_guide.pdf)
    ENDIF (FLTK_FOUND)

    ADD_EXECUTABLE (DragonDemo MACOSX_BUNDLE WIN32 ${DRAGON_DEMO_SRCS})
    #SET_PROPERTY(TARGET DragonDemo PROPERTY LINK_SEARCH_START_STATIC 1) 
    TARGET_LINK_LIBRARIES (DragonDemo gVirtualXRay ${optionalLibs} ${requiredLibs})

    # Define some settings for the Bundle
    IF(APPLE)
        set(MACOSX_BUNDLE_BUNDLE_NAME DragonDemo)
        set(MACOSX_BUNDLE_GUI_IDENTIFIER "DragonDemo")
        SET_SOURCE_FILES_PROPERTIES(../build/DragonDemo.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
        set(MACOSX_BUNDLE_ICON_FILE DragonDemo.icns)
        set(MACOSX_BUNDLE_INFO_STRING "${examples_VERSION_MAJOR}.${examples_VERSION_MINOR}.${examples_VERSION_PATCH}, Copyright 2014 gVirtualXRay team")
        set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${examples_VERSION_MAJOR}.${examples_VERSION_MINOR}")
        set(MACOSX_BUNDLE_LONG_VERSION_STRING "${examples_VERSION_MAJOR}.${examples_VERSION_MINOR}.${examples_VERSION_PATCH}")
        set(MACOSX_BUNDLE_BUNDLE_VERSION "${examples_VERSION_MAJOR}.${examples_VERSION_MINOR}.${examples_VERSION_PATCH}")
        set(MACOSX_BUNDLE_COPYRIGHT "(C) 2013-2014 Franck P. Vidal")
    ENDIF(APPLE)

    SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/DragonDemo")
    
    IF(APPLE)
        SET(APPS "\${CMAKE_INSTALL_PREFIX}/DragonDemo.app")
    ENDIF(APPLE)

    IF(WIN32)
        SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/DragonDemo.exe")
        #set(CPACK_NSIS_MENU_LINKS "${WEBSITE}" "Homepage for gVirtualXRay")
        #set(CPACK_NSIS_URL_INFO_ABOUT "${WEBSITE}")
        #set(CPACK_NSIS_HELP_LINK "${WEBSITE}")
        set(CPACK_NSIS_MENU_LINKS bin\\DragonDemo.exe "DragonDemo"
            doc\\quick_reference_guide.pdf "Quick reference guide"
        )
        set(CPACK_CREATE_DESKTOP_LINKS bin\\DragonDemo.exe "DragonDemo")
        #SET(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}\\build\\DragonDemo64x64.ico")
    ENDIF(WIN32)

    IF (APPLE)
        INSTALL(TARGETS DragonDemo BUNDLE DESTINATION examples COMPONENT applications RUNTIME DESTINATION examples/bin COMPONENT applications)
    ELSE (APPLE)
        install(TARGETS DragonDemo RUNTIME DESTINATION examples/bin COMPONENT applications)
    ENDIF (APPLE)

    if(APPLE)
        SET_SOURCE_FILES_PROPERTIES (
            ../data/welsh-dragon-small.stl
            PROPERTIES
            MACOSX_PACKAGE_LOCATION examples/data
        )

        SET_SOURCE_FILES_PROPERTIES (
            ../LICENSE.txt
            PROPERTIES
            MACOSX_PACKAGE_LOCATION .
        )

        SET_SOURCE_FILES_PROPERTIES (
            ../../doc/screenshots/welsh_dragon_demo/quick_reference_guide.pdf
            PROPERTIES
            MACOSX_PACKAGE_LOCATION help
        )
    else(APPLE)
        INSTALL(FILES ../../doc/screenshots/welsh_dragon_demo/quick_reference_guide.pdf DESTINATION examples/doc COMPONENT applications)
        INSTALL(FILES ../data/welsh-dragon-small.stl                                    DESTINATION examples/data COMPONENT applications)
    endif(APPLE)

    INSTALL(FILES ../LICENSE.txt DESTINATION examples)

    set(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "gVirtualXRay Application")
    set(CPACK_COMPONENT_APPLICATIONS_DESCRIPTION "A demo application that makes use of gVirtualXRay")
    set(CPACK_COMPONENT_APPLICATIONS_GROUP "Runtime")
    set(CPACK_COMPONENT_APPLICATIONS_INSTALL_TYPES Full)
    set(CPACK_COMPONENTS_ALL applications libraries headers)

    # directories to look for dependencies
    SET (DIRS /sw/lib /usr/local/lib ${Boost_LIBRARY_DIR})

    SET (LIBRARIES ${optionalLibs} ${requiredLibs})
    INSTALL(CODE "
        SET(BU_CHMOD_BUNDLE_ITEMS 1)
        include(BundleUtilities)
        fixup_bundle(\"${APPS}\" \"\${LIBRARIES}\" \"${DIRS}\")
        " COMPONENT applications)

    # To Create a package
    IF(APPLE)
        set(CPACK_BINARY_DRAGNDROP ON)
    ENDIF(APPLE)
    include(CPack)
ENDIF(USE_BUNDLE)



IF(USE_BUNDLE)
    ADD_DEPENDENCIES(DragonDemo BinaryExampleFiles)
ENDIF(USE_BUNDLE)


# Set the package name
SET (CPACK_PACKAGE_NAME "gVirtualXRay-examples")

# Handling of shared libraries
#SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS library_name)
#INCLUDE(InstallRequiredSystemLibraries)

# Set the package version
SET(CPACK_PACKAGE_VERSION_MAJOR ${examples_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${examples_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${examples_VERSION_PATCH})
SET(CPACK_PACKAGE_VERSION "${examples_VERSION_MAJOR}.${examples_VERSION_MINOR}.${examples_VERSION_PATCH}")

SET(CPACK_PACKAGE_VENDOR "gVirtualXRay team")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "gVirtualXRay - Installation")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "gVirtualXRay/examples")
    
SET(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE.txt)

INSTALL(FILES ../LICENSE.txt DESTINATION examples)

SET (CPACK_COMPONENT_EXAMPLES_DISPLAY_NAME "Demo programs")
SET (CPACK_COMPONENT_EXAMPLES_DESCRIPTION "All of the demos and example programs using gVirtualXRay")
#SET (CPACK_COMPONENT_EXAMPLES_INSTALL_TYPES Demos Full)

IF(APPLE)
    set(CPACK_BINARY_DRAGNDROP ON)
ENDIF(APPLE)

IF (NOT CPack_CMake_INCLUDED)
    INCLUDE(CPack)
ENDIF (NOT CPack_CMake_INCLUDED)

