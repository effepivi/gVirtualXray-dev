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


if (USE_LIBTIFF AND USE_SYSTEM_LIBTIFF)
    find_package(TIFF REQUIRED)
    #find_package(BZip2 REQUIRED)
endif ()

if (USE_assimp AND USE_SYSTEM_ASSIMP)
    find_package(assimp REQUIRED)
endif ()

IF (USE_GLEW)
    IF (USE_SYSTEM_GLEW)
        find_package(GLEW REQUIRED)
        IF (GLEW_USE_STATIC_LIBS)
            SET (requiredLibs GLEW::glew_s ${requiredLibs})
        ELSE (GLEW_USE_STATIC_LIBS)
            SET (requiredLibs GLEW::GLEW ${requiredLibs})
        ENDIF (GLEW_USE_STATIC_LIBS)
    ELSE (USE_SYSTEM_GLEW)
        SET (requiredLibs ${GLEW_LIBRARY} ${requiredLibs})
    ENDIF (USE_SYSTEM_GLEW)
ENDIF (USE_GLEW)

IF (USE_GLAD AND USE_SYSTEM_GLAD)
        FIND_PACKAGE( glad REQUIRED)
ENDIF (USE_GLAD AND USE_SYSTEM_GLAD)

IF (USE_CLIPBOARD)
    IF (UNIX)
        FIND_PACKAGE( PNG )
        include_directories(${PNG_INCLUDE_DIRS})
    ENDIF (UNIX)
ENDIF (USE_CLIPBOARD)

file(GLOB APP_SOURCES ./*.cxx)
foreach (source_file ${APP_SOURCES})
    get_filename_component(prog_name ${source_file} NAME_WLE)

    add_executable(${prog_name} ${source_file})
    target_link_libraries(${prog_name} ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(${prog_name}   SimpleGVXR)
    install (TARGETS ${prog_name} DESTINATION ${INSTALL_DIR}/demos)

endforeach (source_file ${APP_SOURCES})

# Copy the DLLs
IF (WIN32 AND MSVC)
    FILE(COPY ${XRAYLIB_SHARED_LIB_PATH} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
ENDIF (WIN32 AND MSVC)

install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../Wrappers/welsh-dragon-small.stl
        DESTINATION ${INSTALL_DIR}/demos)

FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../../Wrappers/welsh-dragon-small.stl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../Wrappers/tensile.xtekct
        DESTINATION ${INSTALL_DIR}/demos)

FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../../Wrappers/tensile.xtekct DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
