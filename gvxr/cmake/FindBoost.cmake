# - Find Boost
# Find the native Boost includes and library
# This module defines
#  Boost_INCLUDE_DIR, where to find Boost.h, etc.
#  Boost_LIBRARIES, the libraries needed to use Boost.
#  Boost_FOUND, If false, do not try to use Boost.
# also defined, but not for general use are
#  Boost_LIBRARY, where to find the Boost library.

FIND_PATH(Boost_INCLUDE_DIR NAMES include/Boost PATHS $ENV{Boost_BASE_DIR}/include)

SET(Boost_NAMES ${Boost_NAMES} Boost)
FIND_LIBRARY(Boost_LIBRARY NAMES ${Boost_NAMES} PATHS $ENV{Boost_BASE_DIR}/lib)

# handle the QUIETLY and REQUIRED arguments and set Boost_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Boost DEFAULT_MSG Boost_LIBRARY Boost_INCLUDE_DIR)

IF(Boost_FOUND)
  SET(Boost_LIBRARIES ${Boost_LIBRARY})
ENDIF(Boost_FOUND)

# Deprecated declarations.
SET (NATIVE_Boost_INCLUDE_PATH ${Boost_INCLUDE_DIR} )
IF(Boost_LIBRARY)
  GET_FILENAME_COMPONENT (NATIVE_Boost_LIB_PATH ${Boost_LIBRARY} PATH)
ENDIF(Boost_LIBRARY)

MARK_AS_ADVANCED(Boost_LIBRARY Boost_INCLUDE_DIR)
