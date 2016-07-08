#
#
# Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
# http://www.fpvidal.net/
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
#
# 3. Neither the name of the Bangor University nor the names of its contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


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

