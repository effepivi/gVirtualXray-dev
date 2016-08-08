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


project(gVirtualXRay-tests)

INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/version.cmake)


# gVirtualXRay #################################################################
if (NOT gVirtualXRay_BINARY_DIR)
    find_package(gVirtualXRay REQUIRED)
    include(${gVirtualXRay_USE_FILE})
else (NOT gVirtualXRay_BINARY_DIR)
    set(gVirtualXRay_DIR ${gVirtualXRay_BINARY_DIR}/CMakeFiles)
    find_package(gVirtualXRay REQUIRED)
endif (NOT gVirtualXRay_BINARY_DIR)


# Boost ########################################################################
find_package (Boost COMPONENTS unit_test_framework)
if(Boost_FOUND)
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
	add_definitions (-DBOOST_TEST_DYN_LINK)
	LINK_DIRECTORIES(${Boost_LIBRARY_DIR})
	SET(requiredLibs ${requiredLibs} ${Boost_LIBRARIES})
ELSE(Boost_FOUND)
    MESSAGE(WARNING "Boost not found, the unit test (${PROJECT_NAME}) will be disable.")
ENDIF(Boost_FOUND)


if(Boost_FOUND)
    add_executable(TestDensity TestDensity.cxx)
    target_link_libraries (TestDensity ${gVirtualXRay_LIBRARY} ${requiredLibs})

    add_test (test TestDensity)
ENDIF(Boost_FOUND)

