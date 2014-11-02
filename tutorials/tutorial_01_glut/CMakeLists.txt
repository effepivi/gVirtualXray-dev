project(gVirtualXRay-tutorial_01_glut)

# Initialise some states by default ############################################
set(CMAKE_BUILD_TYPE Release CACHE STRING "Debug|Release|RelWithDebInfo|MinSizeRel")
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../../build/version.cmake)


# GLUT ########################################################################
FIND_PACKAGE(GLUT)
IF(GLUT_FOUND)
    INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})
    SET(optionalLibs ${optionalLibs} ${GLUT_LIBRARIES})
ELSE(GLUT_FOUND)
    MESSAGE(FATAL_ERROR "GLUT not found.")
ENDIF(GLUT_FOUND)


# Create the main program ######################################################
add_executable(tutorial_01_glut tutorial_01_glut.cxx buildCube.cxx)
target_link_libraries(tutorial_01_glut ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})

INSTALL(TARGETS tutorial_01_glut RUNTIME DESTINATION tutorials/tutorial_01_glut/bin  COMPONENT tutorials)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION tutorials/tutorial_01_glut COMPONENT tutorials)

