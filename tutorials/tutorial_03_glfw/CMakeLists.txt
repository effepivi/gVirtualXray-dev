project(gVirtualXRay-tutorial_03_glfw)


# Create the RESOURCES ###########################################################
SET (RESOURCES_SOURCES
    tutorial_03_gl3.frag
    tutorial_03_gl3.vert
)

INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../CreateHeaderFiles.cmake)

CreateHeaderFiles("." "Binarytutorial_03_glfwFiles" "${RESOURCES_SOURCES}")


# Headers of the project #######################################################
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})


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
    MESSAGE(FATAL_ERROR "GLFW not found.")
ENDIF(glfw_FOUND)


# Create the main program ######################################################
add_executable(tutorial_03_glfw tutorial_03_glfw.cxx buildCube.cxx)
target_link_libraries(tutorial_03_glfw ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})

INSTALL(TARGETS tutorial_03_glfw RUNTIME DESTINATION tutorials/tutorial_03_glfw/bin  COMPONENT tutorials)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION tutorials/tutorial_03_glfw COMPONENT tutorials)


