# - Find glfw3
# Find the native glfw3 includes and library
# This module defines
#  glfw3_INCLUDE_DIR, where to find glfw3.h, etc.
#  glfw3_LIBRARIES, the libraries needed to use glfw3.
#  glfw3_FOUND, If false, do not try to use glfw3.
# also defined, but not for general use are
#  glfw3_LIBRARY, where to find the glfw3 library.

FIND_PATH(glfw3_INCLUDE_DIR NAMES include/GLFW/ PATHS $ENV{glfw3_BASE_DIR}/include)

SET(glfw3_NAMES ${glfw3_NAMES} glfw)
FIND_LIBRARY(glfw3_LIBRARY NAMES ${glfw3_NAMES} PATHS $ENV{glfw3_BASE_DIR}/lib)

# handle the QUIETLY and REQUIRED arguments and set glfw3_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(glfw3 DEFAULT_MSG glfw3_LIBRARY glfw3_INCLUDE_DIR)

IF(glfw3_FOUND)
  SET(glfw3_LIBRARIES ${glfw3_LIBRARY})
ENDIF(glfw3_FOUND)

# Deprecated declarations.
SET (NATIVE_glfw3_INCLUDE_PATH ${glfw3_INCLUDE_DIR} )
IF(glfw3_LIBRARY)
  GET_FILENAME_COMPONENT (NATIVE_glfw3_LIB_PATH ${glfw3_LIBRARY} PATH)
ENDIF(glfw3_LIBRARY)

MARK_AS_ADVANCED(glfw3_LIBRARY glfw3_INCLUDE_DIR)
