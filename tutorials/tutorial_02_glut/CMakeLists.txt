project(gVirtualXRay-tutorial_02_glut)


# Create the RESOURCES ###########################################################
SET (RESOURCES_SOURCES
    tutorial_02_gl2.frag
    tutorial_02_gl2.vert
)

INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../CreateHeaderFiles.cmake)

CreateHeaderFiles("." "Binarytutorial_02_glutFiles" "${RESOURCES_SOURCES}")


# Headers of the project #######################################################
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})


# GLUT ########################################################################
FIND_PACKAGE(GLUT)
IF(GLUT_FOUND)
    INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})
    SET(optionalLibs ${optionalLibs} ${GLUT_LIBRARIES})
ELSE(GLUT_FOUND)
    MESSAGE(FATAL_ERROR "GLUT not found.")
ENDIF(GLUT_FOUND)


# Create the main program ######################################################
add_executable(tutorial_02_glut tutorial_02_glut.cxx buildCube.cxx)
target_link_libraries(tutorial_02_glut ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})

INSTALL(TARGETS tutorial_02_glut RUNTIME DESTINATION tutorials/tutorial_02_glut/bin  COMPONENT tutorials)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION tutorials/tutorial_02_glut COMPONENT tutorials)

