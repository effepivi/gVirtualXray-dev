# - Find GLEW
# Find the native GLEW includes and library
# This module defines
#  GLEW_INCLUDE_DIR, where to find GLEW.h, etc.
#  GLEW_LIBRARIES, the libraries needed to use GLEW.
#  GLEW_FOUND, If false, do not try to use GLEW.
# also defined, but not for general use are
#  GLEW_LIBRARY, where to find the GLEW library.

FIND_PATH(GLEW_INCLUDE_DIR NAMES include/GL/ PATHS $ENV{GLEW_BASE_DIR}/include)

SET(GLEW_NAMES ${GLEW_NAMES} GLEW)
FIND_LIBRARY(GLEW_LIBRARY NAMES ${GLEW_NAMES} PATHS $ENV{GLEW_BASE_DIR}/lib)

# handle the QUIETLY and REQUIRED arguments and set GLEW_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLEW DEFAULT_MSG GLEW_LIBRARY GLEW_INCLUDE_DIR)

IF(GLEW_FOUND)
  SET(GLEW_LIBRARIES ${GLEW_LIBRARY})
ENDIF(GLEW_FOUND)

# Deprecated declarations.
SET (NATIVE_GLEW_INCLUDE_PATH ${GLEW_INCLUDE_DIR} )
IF(GLEW_LIBRARY)
  GET_FILENAME_COMPONENT (NATIVE_GLEW_LIB_PATH ${GLEW_LIBRARY} PATH)
ENDIF(GLEW_LIBRARY)

MARK_AS_ADVANCED(GLEW_LIBRARY GLEW_INCLUDE_DIR)
