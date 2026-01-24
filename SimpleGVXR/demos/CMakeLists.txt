#
#
# Copyright (c) 2022, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


if (USE_SYSTEM_LIBTIFF)
    find_package(TIFF REQUIRED)
    #find_package(BZip2 REQUIRED)
endif ()

if (USE_SYSTEM_ASSIMP)
    find_package(assimp REQUIRED)
endif ()

if (USE_SYSTEM_GLEW)
    find_package(GLEW REQUIRED)
    SET (requiredLibs GLEW::glew_s ${requiredLibs})
ELSE ()
    SET (requiredLibs ${GLEW_LIBRARY} ${requiredLibs})
endif ()

add_executable(DemoBeerLambertLawMonoChromatic DemoBeerLambertLawMonoChromatic.cxx)
target_link_libraries(DemoBeerLambertLawMonoChromatic ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs})

add_executable(Benchmark Benchmark.cxx)
target_link_libraries(Benchmark ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs})

# Add dependencies
ADD_DEPENDENCIES(DemoBeerLambertLawMonoChromatic   SimpleGVXR)
ADD_DEPENDENCIES(Benchmark   SimpleGVXR)

#FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/)
#FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/point_source)
#FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/SBP)
#FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/FBP)
#FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/ART)
#FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/MART)
#FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/SIRT)

# Copy the DLLs
IF (WIN32 AND MSVC)
    FILE(COPY ${XRAYLIB_SHARED_LIB_PATH} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
ENDIF (WIN32 AND MSVC)

# add the install targets
install (TARGETS DemoBeerLambertLawMonoChromatic
        DESTINATION ${INSTALL_DIR}/demos)

install (TARGETS Benchmark
        DESTINATION ${INSTALL_DIR}/demos)
