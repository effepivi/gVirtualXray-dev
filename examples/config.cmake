# Fix linkage problem with windows and cross-compilation #######################
IF (WIN32)
    ADD_DEFINITIONS(-DGLEW_STATIC)
    ADD_DEFINITIONS(-DFREEGLUT_STATIC)
    SET (CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lwinmm")
ENDIF (WIN32)


# OpenGL ######################################################################
FIND_PACKAGE(OpenGL REQUIRED)
IF(OPENGL_FOUND)
    INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR} ${GLU_INCLUDE_PATH})
    SET(requiredLibs ${requiredLibs} ${OPENGL_LIBRARIES})
ELSE(OPENGL_FOUND)
    MESSAGE(FATAL_ERROR "OpenGL not found.")
ENDIF(OPENGL_FOUND)


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


# FREETYPE ####################################################################
FIND_PACKAGE(Freetype)
IF(FREETYPE_FOUND)
    ADD_DEFINITIONS(-DHAS_FREETYPE)
    INCLUDE_DIRECTORIES(${FREETYPE_INCLUDE_DIRS})
    SET(optionalLibs ${optionalLibs} ${FREETYPE_LIBRARIES})
ELSE(FREETYPE_FOUND)
    MESSAGE(WARNING "Freetype not found.")
ENDIF(FREETYPE_FOUND)
