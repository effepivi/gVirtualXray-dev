# - Find gVirtualXRay
# Find the native gVirtualXRay includes and library
# This module defines
#  gVirtualXRay_INCLUDE_DIR, where to find GL/glew.h, etc.
#  gVirtualXRay_LIBRARIES, the libraries needed to use gVirtualXRay.
#  gVirtualXRay_FOUND, If false, do not try to use gVirtualXRay.
# also defined, but not for general use are
#  gVirtualXRay_LIBRARY, the gVirtualXRay library.
#  gVirtualXRay_LIBRARY_DIR, where to find the gVirtualXRay library.

FIND_PATH(gVirtualXRay_INCLUDE_DIR NAMES gVirtualXRay/XRayRenderer.h PATHS $ENV{gVirtualXRay_BASE_DIR}/include)

SET(gVirtualXRay_NAMES ${gVirtualXRay_NAMES} gVirtualXRay)
FIND_LIBRARY(gVirtualXRay_LIBRARY NAMES ${gVirtualXRay_NAMES} PATHS $ENV{gVirtualXRay_BASE_DIR}/lib)

# handle the QUIETLY and REQUIRED arguments and set gVirtualXRay_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(gVirtualXRay
        FOUND_VAR gVirtualXRay_FOUND
        REQUIRED_VARS gVirtualXRay_INCLUDE_DIR gVirtualXRay_LIBRARY)

IF(gVirtualXRay_FOUND)
  SET(gVirtualXRay_LIBRARIES ${gVirtualXRay_LIBRARY})
ENDIF(gVirtualXRay_FOUND)

# Deprecated declarations.
SET (NATIVE_gVirtualXRay_INCLUDE_PATH ${gVirtualXRay_INCLUDE_DIR} )
IF(gVirtualXRay_LIBRARY)
  GET_FILENAME_COMPONENT (NATIVE_gVirtualXRay_LIB_PATH ${gVirtualXRay_LIBRARY} PATH)
  SET(gVirtualXRay_LIBRARY_DIR ${NATIVE_gVirtualXRay_LIB_PATH})
ENDIF(gVirtualXRay_LIBRARY)

MARK_AS_ADVANCED(gVirtualXRay_LIBRARY gVirtualXRay_INCLUDE_DIR gVirtualXRay_LIBRARY_DIR)

