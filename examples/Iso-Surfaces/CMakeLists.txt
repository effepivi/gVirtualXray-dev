project(gVirtualXRay-Iso-Surfaces)

# Initialise some states by default ############################################
set(CMAKE_BUILD_TYPE Release CACHE STRING "Debug|Release|RelWithDebInfo|MinSizeRel")
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

