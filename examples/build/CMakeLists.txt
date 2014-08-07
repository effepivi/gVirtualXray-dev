################################################################################
#
#	@file		CMakeLists.txt
#
#	@brief		Configuration file to generate Makefiles, 
#			Project/Solution files for MS VC++, Xcode projects, etc.
#			 using the CMake tool. (see http://www.cmake.org/)
#
#	@version	1.0
#
#	@date		01/12/2013
#
#	@author		Dr Franck P. Vidal
#
#	@section	License
#				BSD 3-Clause License.
#
#				For details on use and redistribution please refer
#				to http://opensource.org/licenses/BSD-3-Clause
#
#	@section	Copyright
#				(c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
#				http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
#				BSD 3-Clause License
#
################################################################################


# Initialise some states by default ############################################
#set(BUILD_SHARED_LIBS OFF)
set(CMAKE_BUILD_TYPE Release CACHE STRING "Debug|Release|RelWithDebInfo|MinSizeRel")


# Minimum CMake version ########################################################
IF(WIN32)
	cmake_minimum_required(VERSION 2.6.0)
ENDIF(WIN32)


IF(COMMAND cmake_policy)
       cmake_policy(SET CMP0011 NEW)
ENDIF(COMMAND cmake_policy)


SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modules)
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../build/modules)


# Enable all compiler warnings #################################################
#IF(NOT WIN32)
#	add_definitions( -Wall )
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

SET(CPACK_PACKAGE_VERSION_MAJOR ${examples_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${examples_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${examples_VERSION_PATCH})


# Headers of the project #######################################################
INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}")
INCLUDE_DIRECTORIES("../include")


# Options #####################################################################
OPTION(USE_BUNDLE "Create bundles or packages using CPack." ON)
OPTION(USE_GLFW "Use GLFW." ON)
IF(APPLE)
    OPTION(USE_OPENMP "Use OpenMP if possible."  OFF)
ELSE(APPLE)
    OPTION(USE_OPENMP "Use OpenMP if possible."  ON)
ENDIF(APPLE)

# FLTK ########################################################################
FIND_PACKAGE(FLTK)
if(FLTK_FOUND)
        ADD_DEFINITIONS(-DHAS_FLTK)
        INCLUDE_DIRECTORIES(${FLTK_INCLUDE_DIR})
        SET(requiredLibs ${optionalLibs} ${FLTK_LIBRARIES})
ELSE(FLTK_FOUND)
        MESSAGE(WARNING "FLTK not found, it will be disable.")
ENDIF(FLTK_FOUND)


# Octave ######################################################################
FIND_PACKAGE(Octave)
if(OCTAVE_FOUND)
    ADD_DEFINITIONS(-DHAS_OCTAVE)
    INCLUDE_DIRECTORIES(${OCTAVE_INCLUDE_DIRS})
    LINK_DIRECTORIES(${OCTAVE_LINK_DIRS})
    SET(optionalLibs ${optionalLibs} ${OCTAVE_LIBRARY})
ELSE(OCTAVE_FOUND)
    MESSAGE(WARNING "GNU Octave not found, the use of GNU Octave will be disable.")
ENDIF(OCTAVE_FOUND)


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
FIND_PACKAGE(OpenGL REQUIRED)
IF(OPENGL_FOUND)
	INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR} ${GLU_INCLUDE_PATH})
	SET(requiredLibs ${requiredLibs} ${OPENGL_LIBRARIES} ${GLU_LIBRARY})
ELSE(OPENGL_FOUND)
	MESSAGE(FATAL_ERROR "OpenGL not found.")
ENDIF(OPENGL_FOUND)


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


# Check if GLFW is enable #####################################################
IF(NOT USE_GLFW)
	MESSAGE(FATAL_ERROR "GLFW is not enable, examples and tutorials cannot be built.")
ENDIF(NOT USE_GLFW)


# BOOST #######################################################################
IF(USE_BUNDLE)
	FIND_PACKAGE(Boost QUIET REQUIRED COMPONENTS system filesystem program_options)
	if(Boost_FOUND)
		INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
		LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
		SET(requiredLibs ${requiredLibs} ${Boost_LIBRARIES})
	ELSE(Boost_FOUND)
		MESSAGE(FATAL_ERROR "Boost not found.")
	ENDIF(Boost_FOUND)
ENDIF(USE_BUNDLE)


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
REMOVE_DEFINITIONS(-DHAS_BUNDLE)

ADD_EXECUTABLE (test_implicit_surface_density_function ../src/test_implicit_surface_density_function.cxx)
#SET_PROPERTY(TARGET test_implicit_surface_density_function PROPERTY LINK_SEARCH_START_STATIC 1) 
TARGET_LINK_LIBRARIES (test_implicit_surface_density_function gVirtualXRay ${requiredLibs} ${optionalLibs})

IF(USE_GLFW)
	ADD_EXECUTABLE (test_mu_water ../src/test_mu_water.cxx)
	#SET_PROPERTY(TARGET test_mu_water PROPERTY LINK_SEARCH_START_STATIC 1) 
	TARGET_LINK_LIBRARIES (test_mu_water gVirtualXRay ${requiredLibs} ${optionalLibs})

	ADD_EXECUTABLE (test_01 ../src/test_01.cxx)
	#SET_PROPERTY(TARGET test_01 PROPERTY LINK_SEARCH_START_STATIC 1) 
	TARGET_LINK_LIBRARIES (test_01 gVirtualXRay ${requiredLibs} ${optionalLibs})

	ADD_EXECUTABLE (welsh_dragon ../src/welsh_dragon.cxx)
	#SET_PROPERTY(TARGET welsh_dragon PROPERTY LINK_SEARCH_START_STATIC 1) 
	TARGET_LINK_LIBRARIES (welsh_dragon gVirtualXRay ${requiredLibs} ${optionalLibs})

	ADD_EXECUTABLE (test_triangle_mesh ../src/test_triangle_mesh.cxx ../src/buildCube.cxx)
	#SET_PROPERTY(TARGET test_triangle_mesh PROPERTY LINK_SEARCH_START_STATIC 1) 
	TARGET_LINK_LIBRARIES (test_triangle_mesh gVirtualXRay ${requiredLibs} ${optionalLibs})

	ADD_EXECUTABLE (implicit_surface_marching_cube ../src/implicit_surface_marching_cube.cxx)
	#SET_PROPERTY(TARGET implicit_surface_marching_cube PROPERTY LINK_SEARCH_START_STATIC 1) 
	TARGET_LINK_LIBRARIES (implicit_surface_marching_cube gVirtualXRay ${requiredLibs} ${optionalLibs})

	ADD_EXECUTABLE (test_implicit_surface ../src/test_implicit_surface.cxx)
	#SET_PROPERTY(TARGET test_implicit_surface PROPERTY LINK_SEARCH_START_STATIC 1) 
	TARGET_LINK_LIBRARIES (test_implicit_surface gVirtualXRay ${requiredLibs} ${optionalLibs})

	ADD_EXECUTABLE (tutorial_01 ../src/tutorial_01.cxx ../src/buildCube.cxx)
	#SET_PROPERTY(TARGET tutorial_01 PROPERTY LINK_SEARCH_START_STATIC 1) 
	TARGET_LINK_LIBRARIES (tutorial_01 gVirtualXRay ${requiredLibs} ${optionalLibs})

	ADD_EXECUTABLE (tutorial_02 ../src/tutorial_02.cxx ../src/buildCube.cxx)
	#SET_PROPERTY(TARGET tutorial_02 PROPERTY LINK_SEARCH_START_STATIC 1) 
	TARGET_LINK_LIBRARIES (tutorial_02 gVirtualXRay ${requiredLibs} ${optionalLibs})

	ADD_EXECUTABLE (tutorial_03 ../src/tutorial_03.cxx ../src/buildCube.cxx)
	#SET_PROPERTY(TARGET tutorial_01 PROPERTY LINK_SEARCH_START_STATIC 1) 
	TARGET_LINK_LIBRARIES (tutorial_03 gVirtualXRay ${requiredLibs} ${optionalLibs})

	ADD_EXECUTABLE (VoxelLabelisation ../src/VoxelLabelisation.cxx)
	#SET_PROPERTY(TARGET VoxelLabelisation PROPERTY LINK_SEARCH_START_STATIC 1) 
	TARGET_LINK_LIBRARIES (VoxelLabelisation gVirtualXRay ${requiredLibs} ${optionalLibs})
ENDIF(USE_GLFW)


IF (FLTK_FOUND)
    ADD_EXECUTABLE (gVirtualXRay-gui ${CMAKE_CURRENT_BINARY_DIR}/gui.cxx ../src/OpenGLWindow.cxx)
    TARGET_LINK_LIBRARIES (gVirtualXRay-gui gVirtualXRay ${requiredLibs} ${optionalLibs})
ENDIF (FLTK_FOUND)


IF(USE_BUNDLE)
	SET (MACOSX_RPATH 1)
        ADD_DEFINITIONS(-DHAS_BUNDLE)
	SET(DRAGON_DEMO_SRCS ../src/welsh_dragon.cxx DragonDemo.icns ../data/welsh-dragon-small.stl ../../LICENSE.txt ../../doc/screenshots/welsh_dragon_demo/quick_reference_guide.pdf)

	ADD_EXECUTABLE (DragonDemo MACOSX_BUNDLE WIN32 ${DRAGON_DEMO_SRCS})
	#SET_PROPERTY(TARGET DragonDemo PROPERTY LINK_SEARCH_START_STATIC 1) 
	TARGET_LINK_LIBRARIES (DragonDemo gVirtualXRay ${requiredLibs} ${optionalLibs})

	# Define some settings for the Bundle
	if(APPLE)
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
	ENDIF(WIN32)

	INSTALL(TARGETS DragonDemo 
		BUNDLE DESTINATION . COMPONENT Runtime
		RUNTIME DESTINATION bin COMPONENT Runtime
	)

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
		INSTALL(FILES ../../doc/screenshots/welsh_dragon_demo/quick_reference_guide.pdf DESTINATION doc)
		INSTALL(FILES ../data/welsh-dragon-small.stl                                    DESTINATION examples/data)
	endif(APPLE)

	INSTALL(FILES ../../LICENSE.txt DESTINATION .)

	# directories to look for dependencies
	SET (DIRS /sw/lib /usr/local/lib ${Boost_LIBRARY_DIR})

	SET (LIBRARIES ${requiredLibs} ${optionalLibs})
	INSTALL(CODE "
		include(BundleUtilities)
		fixup_bundle(\"${APPS}\" \"\${LIBRARIES}\" \"${DIRS}\")
		" COMPONENT Runtime)

	# To Create a package
	IF(APPLE)
		set(CPACK_BINARY_DRAGNDROP ON)
	ENDIF(APPLE)
	include(CPack)
ENDIF(USE_BUNDLE)


# Make sure the source is generated before the executable builds.
ADD_DEPENDENCIES(test_01 BinaryDemoFiles)
ADD_DEPENDENCIES(welsh_dragon BinaryDemoFiles)
ADD_DEPENDENCIES(test_triangle_mesh BinaryDemoFiles)
ADD_DEPENDENCIES(implicit_surface_marching_cube BinaryDemoFiles)
ADD_DEPENDENCIES(test_implicit_surface BinaryDemoFiles)
ADD_DEPENDENCIES(tutorial_01 BinaryDemoFiles)
ADD_DEPENDENCIES(tutorial_02 BinaryDemoFiles)
ADD_DEPENDENCIES(tutorial_03 BinaryDemoFiles)
ADD_DEPENDENCIES(VoxelLabelisation BinaryDemoFiles)
IF(USE_BUNDLE)
	ADD_DEPENDENCIES(DragonDemo BinaryDemoFiles)
ENDIF(USE_BUNDLE)
