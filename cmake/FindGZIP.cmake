# - Find GZIP
# Find the native GZIP command
# This module defines
#  GZIP_TOOL, where to find gizp
#  GZIP_FOUND, If false, do not try to use GLEW.

SET(GZIP_NAMES ${GZIP_NAMES} gzip gzip.exe)
FIND_PROGRAM(GZIP_TOOL NAMES ${GZIP_NAMES} PATHS /bin /usr/bin /usr/local/bin)

# handle the QUIETLY and REQUIRED arguments and set GLEW_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GZIP DEFAULT_MSG GZIP_TOOL)

