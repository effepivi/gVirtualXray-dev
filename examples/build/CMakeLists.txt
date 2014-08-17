################################################################################
#
#   @file       CMakeLists.txt
#
#   @brief      Configuration file to generate Makefiles, 
#           Project/Solution files for MS VC++, Xcode projects, etc.
#            using the CMake tool. (see http://www.cmake.org/)
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

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/modules)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../build/modules)


# Enable all compiler warnings #################################################
#IF(NOT WIN32)
#   add_definitions( -Wall )
#ENDIF(NOT WIN32)


# Enable Stereo using OpenGL's quad buffering ##################################
IF(NOT APPLE)
    ADD_DEFINITIONS(-DUSE_STEREO)
ENDIF(NOT APPLE)


# Project name #################################################################
project (examples)
set (examples_VERSION_MAJOR 1)
set (examples_VERSION_MINOR 2)
set (examples_VERSION_PATCH 0)


# Headers of the project #######################################################
INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}")
INCLUDE_DIRECTORIES("../include")


# Options #####################################################################
OPTION(USE_BUNDLE "Create bundles or packages using CPack." ON)
OPTION(USE_GLFW "Use GLFW." ON)
OPTION(USE_GLUT "Use GLUT." ON)

IF(APPLE)
    OPTION(USE_OPENMP "Use OpenMP if possible."  OFF)
ELSE(APPLE)
    OPTION(USE_OPENMP "Use OpenMP if possible."  ON)
ENDIF(APPLE)

IF(WIN32)
    OPTION(USE_OCTAVE "Use GNU Octave if possible."  OFF)
ELSE(WIN32)
    SET(USE_OCTAVE ON)
ENDIF(WIN32)


# FLTK ########################################################################
FIND_PACKAGE(FLTK)
if(FLTK_FOUND)
        ADD_DEFINITIONS(-DHAS_FLTK)
        INCLUDE_DIRECTORIES(${FLTK_INCLUDE_DIR})
        SET(optionalLibs ${optionalLibs} ${FLTK_LIBRARIES})
ELSE(FLTK_FOUND)
        MESSAGE(WARNING "FLTK not found, it will be disable.")
ENDIF(FLTK_FOUND)


IF(FLTK_FOUND)
    FIND_PACKAGE(GLEW REQUIRED)
    IF(GLEW_FOUND)
        ADD_DEFINITIONS(-DGLEW_STATIC)
        ADD_DEFINITIONS(-DHAS_GLEW)
        INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})
        LINK_DIRECTORIES(${GLEW_LIBRARY_DIR})
        SET(optionalLibs ${optionalLibs} ${GLEW_LIBRARY})
    ELSE(GLEW_FOUND)
        MESSAGE(FATAL_ERROR "GLEW not found.")
    ENDIF(GLEW_FOUND)
ENDIF(FLTK_FOUND)


# OpenGL ######################################################################
FIND_PACKAGE(OpenGL REQUIRED)
IF(OPENGL_FOUND)
    INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR} ${GLU_INCLUDE_PATH})
    SET(requiredLibs ${requiredLibs} ${OPENGL_LIBRARIES} ${GLU_LIBRARY})
ELSE(OPENGL_FOUND)
    MESSAGE(FATAL_ERROR "OpenGL not found.")
ENDIF(OPENGL_FOUND)


# BOOST #######################################################################
FIND_PACKAGE(Boost QUIET REQUIRED COMPONENTS system filesystem program_options)
IF(Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
    SET(requiredLibs ${requiredLibs} ${Boost_LIBRARIES})
ELSE(Boost_FOUND)
    MESSAGE(FATAL_ERROR "Boost not found.")
ENDIF(Boost_FOUND)


# OpenMP #######################################################################
IF (USE_OPENMP)
    FIND_PACKAGE(OpenMP)
    IF (OPENMP_FOUND)
        ADD_DEFINITIONS(-DHAS_OPENMP)
        SET (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${OpenMP_C_FLAGS}")
        SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    ELSE(OPENMP_FOUND)
        MESSAGE(WARNING "OpenMP not found, it will be disable.")
        SET (USE_OPENMP OFF)
    ENDIF(OPENMP_FOUND)
ENDIF(USE_OPENMP)


# Octave ######################################################################
IF(USE_OCTAVE)
    FIND_PACKAGE(Octave)
    if(OCTAVE_FOUND)
        ADD_DEFINITIONS(-DHAS_OCTAVE)
        INCLUDE_DIRECTORIES(${OCTAVE_INCLUDE_DIRS})
        LINK_DIRECTORIES(${OCTAVE_LINK_DIRS})
        MESSAGE(WARNING ${OCTAVE_LINK_DIRS)
        SET(optionalLibs ${optionalLibs} ${OCTAVE_LIBRARY})
    ELSE(OCTAVE_FOUND)
        MESSAGE(WARNING "GNU Octave not found, the use of GNU Octave will be disable.")
    ENDIF(OCTAVE_FOUND)
ENDIF(USE_OCTAVE)


# GSL #########################################################################
FIND_PACKAGE(GSL)
if(GSL_FOUND)
    ADD_DEFINITIONS(-DHAS_GSL)
    INCLUDE_DIRECTORIES(${GSL_INCLUDE_DIR})
    LINK_DIRECTORIES(${GSL_LINK_DIRECTORIES})
    SET(optionalLibs ${optionalLibs} ${GSL_LIBRARIES})
ELSE(GSL_FOUND)
    MESSAGE(WARNING "GNU scientific library (GSL) (see http://www.gnu.org/software/gsl/) not found, the use of GSL will be disable.")
ENDIF(GSL_FOUND)


# OpenGL ######################################################################
IF(USE_GLFW)
    FIND_PACKAGE(glfw REQUIRED)
    IF(glfw_FOUND)
        ADD_DEFINITIONS(-DHAS_GLFW)
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
        MESSAGE(FATAL_ERROR "GLFW not found.")
    ENDIF(glfw_FOUND)
ENDIF(USE_GLFW)


IF(USE_GLUT)
    FIND_PACKAGE(GLUT REQUIRED)
    IF(GLUT_FOUND)
        ADD_DEFINITIONS(-DHAS_GLUT)
        INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})
        SET(optionalLibs ${optionalLibs} ${GLUT_LIBRARIES})
    ELSE(GLUT_FOUND)
        MESSAGE(FATAL_ERROR "GLUT not found.")
    ENDIF(GLUT_FOUND)
ENDIF(USE_GLUT)


# Check if GLFW is enable #####################################################
IF(NOT USE_GLFW)
    MESSAGE(WARNING "GLFW is not enable, examples and tutorials using GLFW cannot be built.")
ENDIF(NOT USE_GLFW)


# Check if GLUT is enable #####################################################
IF(NOT USE_GLUT)
    MESSAGE(WARNING "GLUT is not enable, examples and tutorials using GLUT cannot be built.")
ENDIF(NOT USE_GLUT)


# Create the RESOURCES ###########################################################
SET (RESOURCES_SOURCES
    ../../examples/shaders/display_gl2.frag
    ../../examples/shaders/display_gl2.vert
    ../../examples/shaders/display_gl3.frag
    ../../examples/shaders/display_gl3.vert
    ../../examples/shaders/display_gui.frag
    ../../examples/shaders/display_gui.vert
    ../src/tutorial_02.frag
    ../src/tutorial_02.vert
    ../src/tutorial_03.frag
    ../src/tutorial_03.vert
)


FIND_PACKAGE(GZIP REQUIRED)
IF(NOT GZIP_FOUND)
    MESSAGE(FATAL_ERROR "GZIP not found.")
ENDIF(NOT GZIP_FOUND)


INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../../build/CreateHeaderFiles.cmake)
CreateHeaderFiles("../.." "BinaryDemoFiles" "${RESOURCES_SOURCES}")


# Create the GUI files with Fluid ##############################################
IF (FLTK_FOUND)
    SET (FLUID_OUTPUT_FILES ${CMAKE_CURRENT_BINARY_DIR}/gui.h ${CMAKE_CURRENT_BINARY_DIR}/gui.cxx)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
    add_custom_command(OUTPUT ${FLUID_OUTPUT_FILES}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../src/gui.fl
        COMMAND ${FLTK_FLUID_EXECUTABLE}
        ARGS -c ${CMAKE_CURRENT_SOURCE_DIR}/../src/gui.fl
        COMMENT "Build the GUI using Fluid"
    )

    ADD_CUSTOM_TARGET(FluidFiles ALL echo DEPENDS ${FLUID_OUTPUT_FILES})

    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_BINARY_DIR}/gui.h)
    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_BINARY_DIR}/gui.cxx)
ENDIF (FLTK_FOUND)


# Create the main programs #####################################################
IF(USE_BUNDLE)
    REMOVE_DEFINITIONS(-DHAS_BUNDLE)
ENDIF(USE_BUNDLE)


ADD_EXECUTABLE (test_implicit_surface_density_function ../src/test_implicit_surface_density_function.cxx)
#SET_PROPERTY(TARGET test_implicit_surface_density_function PROPERTY LINK_SEARCH_START_STATIC 1) 
TARGET_LINK_LIBRARIES (test_implicit_surface_density_function gVirtualXRay ${requiredLibs} ${optionalLibs})


IF(USE_GLFW)
    IF(NOT WIN32)
        ADD_EXECUTABLE (implicit_surface_marching_cube ../src/implicit_surface_marching_cube.cxx)
        TARGET_LINK_LIBRARIES (implicit_surface_marching_cube gVirtualXRay ${requiredLibs} ${optionalLibs})

        ADD_EXECUTABLE (test_01 ../src/test_01.cxx)
        TARGET_LINK_LIBRARIES (test_01 gVirtualXRay ${requiredLibs} ${optionalLibs})

        ADD_EXECUTABLE (test_implicit_surface ../src/test_implicit_surface.cxx)
        TARGET_LINK_LIBRARIES (test_implicit_surface gVirtualXRay ${requiredLibs} ${optionalLibs})

        ADD_EXECUTABLE (test_mu_water ../src/test_mu_water.cxx)
        TARGET_LINK_LIBRARIES (test_mu_water gVirtualXRay ${requiredLibs} ${optionalLibs})

        ADD_EXECUTABLE (test_triangle_mesh ../src/test_triangle_mesh.cxx ../src/buildCube.cxx)
        TARGET_LINK_LIBRARIES (test_triangle_mesh gVirtualXRay ${requiredLibs} ${optionalLibs})

        ADD_EXECUTABLE (tutorial_01_glfw ../src/tutorial_01_glfw.cxx ../src/buildCube.cxx)
        TARGET_LINK_LIBRARIES (tutorial_01_glfw gVirtualXRay ${requiredLibs} ${optionalLibs})

        #ADD_EXECUTABLE (tutorial_02_glfw ../src/tutorial_02_glfw.cxx ../src/buildCube.cxx)
        #TARGET_LINK_LIBRARIES (tutorial_02_glfw gVirtualXRay ${requiredLibs} ${optionalLibs})

        #ADD_EXECUTABLE (tutorial_03_glfw ../src/tutorial_03_glfw.cxx ../src/buildCube.cxx)
        #TARGET_LINK_LIBRARIES (tutorial_03_glfw gVirtualXRay ${requiredLibs} ${optionalLibs})

        #ADD_EXECUTABLE (welsh_dragon_glfw ../src/welsh_dragon_glfw.cxx)
        #TARGET_LINK_LIBRARIES (welsh_dragon_glfw gVirtualXRay ${requiredLibs} ${optionalLibs})
    ENDIF(NOT WIN32)

    IF(USE_ITK AND ITK_FOUND)
        IF(USE_OCTAVE AND OCTAVE_FOUND)
            ADD_EXECUTABLE (VoxelLabelisation ../src/VoxelLabelisation.cxx)
            TARGET_LINK_LIBRARIES (VoxelLabelisation gVirtualXRay ${requiredLibs} ${optionalLibs})
        ENDIF(USE_OCTAVE AND OCTAVE_FOUND)
    ENDIF(USE_ITK AND ITK_FOUND)
ENDIF(USE_GLFW)


IF(USE_GLUT)
    IF(NOT WIN32)
        ADD_EXECUTABLE (tutorial_01_glut ../src/tutorial_01_glut.cxx ../src/buildCube.cxx)
        TARGET_LINK_LIBRARIES (tutorial_01_glut gVirtualXRay ${requiredLibs} ${optionalLibs})

        ADD_EXECUTABLE (tutorial_02_glut ../src/tutorial_02_glut.cxx ../src/buildCube.cxx)
        TARGET_LINK_LIBRARIES (tutorial_02_glut gVirtualXRay ${requiredLibs} ${optionalLibs})

        ADD_EXECUTABLE (tutorial_03_glut ../src/tutorial_03_glut.cxx ../src/buildCube.cxx)
        TARGET_LINK_LIBRARIES (tutorial_03_glut gVirtualXRay ${requiredLibs} ${optionalLibs})

        ADD_EXECUTABLE (welsh_dragon_glut ../src/welsh_dragon_glut.cxx)
        TARGET_LINK_LIBRARIES (welsh_dragon_glut gVirtualXRay ${requiredLibs} ${optionalLibs})
    ENDIF(NOT WIN32)
ENDIF(USE_GLUT)


IF (FLTK_FOUND)
    IF(WIN32)
        ADD_EXECUTABLE (gVirtualXRay-gui ${CMAKE_CURRENT_BINARY_DIR}/gui.cxx ../src/OpenGLWindow.cxx ../src/DragonDemo.res)
    ELSE(WIN32)
        ADD_EXECUTABLE (gVirtualXRay-gui ${CMAKE_CURRENT_BINARY_DIR}/gui.cxx ../src/OpenGLWindow.cxx)
    ENDIF(WIN32)
    TARGET_LINK_LIBRARIES (gVirtualXRay-gui gVirtualXRay ${requiredLibs} ${optionalLibs})
ENDIF (FLTK_FOUND)


IF(USE_BUNDLE)
    # Set the package name
    SET (CPACK_PACKAGE_NAME "gVirtualXRay")

    # Set the package version
    SET(CPACK_PACKAGE_VERSION_MAJOR ${examples_VERSION_MAJOR})
    SET(CPACK_PACKAGE_VERSION_MINOR ${examples_VERSION_MINOR})
    SET(CPACK_PACKAGE_VERSION_PATCH ${examples_VERSION_PATCH})
    set(CPACK_PACKAGE_VERSION "${examples_VERSION_MAJOR}.${examples_VERSION_MINOR}.${examples_VERSION_PATCH}")

    set(CPACK_PACKAGE_VENDOR "gVirtualXRay team")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "gVirtualXRay - Installation")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "gVirtualXRay")
    
    set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt)

    SET (MACOSX_RPATH 1)
    ADD_DEFINITIONS(-DHAS_BUNDLE)

    IF (FLTK_FOUND)
        IF(WIN32)
            SET(DRAGON_DEMO_SRCS ${CMAKE_CURRENT_BINARY_DIR}/gui.cxx ../src/OpenGLWindow.cxx ../src/DragonDemo.res ../data/welsh-dragon-small.stl ../../LICENSE.txt ../../doc/screenshots/welsh_dragon_demo/quick_reference_guide.pdf)
        ELSE(WIN32)
            SET(DRAGON_DEMO_SRCS ${CMAKE_CURRENT_BINARY_DIR}/gui.cxx ../src/OpenGLWindow.cxx DragonDemo.icns ../data/welsh-dragon-small.stl ../../LICENSE.txt ../../doc/screenshots/welsh_dragon_demo/quick_reference_guide.pdf)
        ENDIF(WIN32)
    ELSE (FLTK_FOUND)
        SET(DRAGON_DEMO_SRCS ../src/welsh_dragon.cxx DragonDemo.icns ../data/welsh-dragon-small.stl ../../LICENSE.txt ../../doc/screenshots/welsh_dragon_demo/quick_reference_guide.pdf)
    ENDIF (FLTK_FOUND)

    ADD_EXECUTABLE (DragonDemo MACOSX_BUNDLE WIN32 ${DRAGON_DEMO_SRCS})
    #SET_PROPERTY(TARGET DragonDemo PROPERTY LINK_SEARCH_START_STATIC 1) 
    TARGET_LINK_LIBRARIES (DragonDemo gVirtualXRay ${requiredLibs} ${optionalLibs})

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
        INSTALL(TARGETS DragonDemo BUNDLE DESTINATION . COMPONENT applications RUNTIME DESTINATION bin COMPONENT applications)
    ELSE (APPLE)
        install(TARGETS DragonDemo RUNTIME DESTINATION bin COMPONENT applications)
    ENDIF (APPLE)

    if(APPLE)
        SET_SOURCE_FILES_PROPERTIES (
            ../data/welsh-dragon-small.stl
            PROPERTIES
            MACOSX_PACKAGE_LOCATION examples/data
        )

        SET_SOURCE_FILES_PROPERTIES (
            ../../LICENSE.txt
            PROPERTIES
            MACOSX_PACKAGE_LOCATION .
        )

        SET_SOURCE_FILES_PROPERTIES (
            ../../doc/screenshots/welsh_dragon_demo/quick_reference_guide.pdf
            PROPERTIES
            MACOSX_PACKAGE_LOCATION help
        )
    else(APPLE)
        INSTALL(FILES ../../doc/screenshots/welsh_dragon_demo/quick_reference_guide.pdf DESTINATION doc COMPONENT applications)
        INSTALL(FILES ../data/welsh-dragon-small.stl                                    DESTINATION examples/data COMPONENT applications)
    endif(APPLE)

    INSTALL(FILES ../../LICENSE.txt DESTINATION .)

    set(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "gVirtualXRay Application")
    set(CPACK_COMPONENT_APPLICATIONS_DESCRIPTION "A demo application that makes use of gVirtualXRay")
    set(CPACK_COMPONENT_APPLICATIONS_GROUP "Runtime")
    set(CPACK_COMPONENT_APPLICATIONS_INSTALL_TYPES Full)

    set(CPACK_COMPONENTS_ALL applications libraries headers)

    # directories to look for dependencies
    SET (DIRS /sw/lib /usr/local/lib ${Boost_LIBRARY_DIR})

    SET (LIBRARIES ${requiredLibs} ${optionalLibs})
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


# Make sure the source is generated before the executable builds.
IF(USE_GLFW)
    IF(NOT WIN32)
        ADD_DEPENDENCIES(implicit_surface_marching_cube BinaryDemoFiles)
        ADD_DEPENDENCIES(test_01 BinaryDemoFiles)
        ADD_DEPENDENCIES(test_implicit_surface BinaryDemoFiles)
        ADD_DEPENDENCIES(test_triangle_mesh BinaryDemoFiles)
        ADD_DEPENDENCIES(tutorial_01_glfw BinaryDemoFiles)
        #ADD_DEPENDENCIES(tutorial_02_glfw BinaryDemoFiles)
        #ADD_DEPENDENCIES(tutorial_03_glfw BinaryDemoFiles)
        #ADD_DEPENDENCIES(welsh_dragon_glfw BinaryDemoFiles)
    ENDIF(NOT WIN32)

    IF(USE_ITK AND ITK_FOUND)
        IF(USE_OCTAVE AND OCTAVE_FOUND)
            ADD_DEPENDENCIES(VoxelLabelisation BinaryDemoFiles)
        ENDIF(USE_OCTAVE AND OCTAVE_FOUND)
    ENDIF(USE_ITK AND ITK_FO)
ENDIF(USE_GLFW)


IF(USE_GLUT)
    IF(NOT WIN32)
        ADD_DEPENDENCIES(tutorial_01_glut BinaryDemoFiles)
        #ADD_DEPENDENCIES(tutorial_02_glut BinaryDemoFiles)
        #ADD_DEPENDENCIES(tutorial_03_glut BinaryDemoFiles)
        #ADD_DEPENDENCIES(welsh_dragon_glut BinaryDemoFiles)
    ENDIF(NOT WIN32)
ENDIF(USE_GLUT)


IF(FLTK_FOUND)
    ADD_DEPENDENCIES(gVirtualXRay-gui BinaryDemoFiles)
ENDIF(FLTK_FOUND)


IF(USE_BUNDLE)
    ADD_DEPENDENCIES(DragonDemo BinaryDemoFiles)
ENDIF(USE_BUNDLE)

