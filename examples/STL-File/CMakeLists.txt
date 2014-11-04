project(gVirtualXRay-STL-File)

# Initialise some states by default ############################################
set(CMAKE_BUILD_TYPE Release CACHE STRING "Debug|Release|RelWithDebInfo|MinSizeRel")
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../../build/version.cmake)


# Create the RESOURCES ###########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../resources.cmake)
CreateHeaderFiles("." "BinarySTLFiles" "${RESOURCES_SOURCES}")


# Headers of the project #######################################################
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})


# Configure the project ########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../config.cmake)


# Check if GLUT or GLFW are available #########################################
IF(NOT glfw_FOUND AND NOT GLUT_FOUND)
    MESSAGE(FATAL_ERROR "GLFW and GLUT were not found. The demo cannot be built.")
ENDIF(NOT glfw_FOUND AND NOT GLUT_FOUND)


# Create the main programs #####################################################
IF(glfw_FOUND)
    add_executable(welsh_dragon_glfw welsh_dragon_glfw.cxx )
    target_link_libraries(welsh_dragon_glfw ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(welsh_dragon_glfw BinarySTLFiles)

    INSTALL(TARGETS welsh_dragon_glfw RUNTIME DESTINATION demos/STL-File/bin  COMPONENT demos)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../data/welsh-dragon-small.stl DESTINATION demos/STL-File/data COMPONENT demos)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION demos/STL-File COMPONENT demos)
ENDIF(glfw_FOUND)


IF(GLUT_FOUND)
    add_executable(welsh_dragon_glut welsh_dragon_glut.cxx )
    target_link_libraries(welsh_dragon_glut ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(welsh_dragon_glut BinarySTLFiles)

    INSTALL(TARGETS welsh_dragon_glut RUNTIME DESTINATION demos/STL-File/bin  COMPONENT demos)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../data/welsh-dragon-small.stl DESTINATION demos/STL-File/data COMPONENT demos)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION demos/STL-File COMPONENT demos)
ENDIF(GLUT_FOUND)

