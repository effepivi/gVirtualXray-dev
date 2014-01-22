# - Find GLUI
# Find the native GLUI includes and library
# This module defines
#  GLUI_INCLUDE_DIR, where to find GL/glui.h, etc.
#  GLUI_LIBRARIES, the libraries needed to use GLUI.
#  GLUI_FOUND, If false, do not try to use GLUI.
# also defined, but not for general use are
#  GLUI_LIBRARY, where to find the GLUI library.

FIND_PATH(GLUI_INCLUDE_DIR NAMES glui.h PATHS /usr/include /usr/include/GL /usr/local/include /usr/local/include/GL $ENV{GLUI_BASE_DIR}/include/GL)

SET(GLUI_NAMES ${GLUI_NAMES} glui GLUI glui32)
FIND_LIBRARY(GLUI_LIBRARY NAMES ${GLUI_NAMES} PATHS $ENV{GLUI_BASE_DIR}/lib)

# handle the QUIETLY and REQUIRED arguments and set GLUI_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLUI DEFAULT_MSG GLUI_LIBRARY GLUI_INCLUDE_DIR)

IF(GLUI_FOUND)
  SET(GLUI_LIBRARIES ${GLUI_LIBRARY})
ENDIF(GLUI_FOUND)

# Deprecated declarations.
SET (NATIVE_GLUI_INCLUDE_PATH ${GLUI_INCLUDE_DIR} )
IF(GLUI_LIBRARY)
  GET_FILENAME_COMPONENT (NATIVE_GLUI_LIB_PATH ${GLUI_LIBRARY} PATH)
ENDIF(GLUI_LIBRARY)

MARK_AS_ADVANCED(GLUI_LIBRARY GLUI_INCLUDE_DIR)
