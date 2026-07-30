#
#
# Copyright (c) 2019, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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



if(${CMAKE_VERSION} VERSION_GREATER_EQUAL  "3.13.0")
    cmake_policy(SET CMP0075 NEW)
endif()

FIND_PACKAGE( Threads REQUIRED )


if (USE_SYSTEM_GTEST)
    find_package(GTest REQUIRED)
endif ()


if (USE_LIBTIFF AND USE_SYSTEM_LIBTIFF)
    find_package(TIFF REQUIRED)
    #find_package(BZip2 REQUIRED)
endif ()

if (USE_assimp AND USE_SYSTEM_ASSIMP)
    find_package(assimp REQUIRED)
endif ()

IF (USE_GLEW)
    SET (GLEW_USE_STATIC_LIBS ON)
    if (USE_SYSTEM_GLEW)
        find_package(GLEW REQUIRED)
        SET (requiredLibs GLEW::glew_s ${requiredLibs})
    ELSE ()
        SET (requiredLibs ${GLEW_LIBRARY} ${requiredLibs})
    endif ()
ENDIF (USE_GLEW)

IF (USE_GLFW)
    IF (USE_SYSTEM_GLFW)
        FIND_PACKAGE(glfw3 REQUIRED)
    ENDIF (USE_SYSTEM_GLFW)

    SET (requiredLibs ${glfw3_LIBRARIES} ${requiredLibs})
ENDIF (USE_GLFW)


# Enable unit testing
enable_testing()
include_directories(${GTest_INCLUDE_DIRS})
#link_directories(${GTest_LIBS_DIR})

# Functions needed
# does this system provide the log and exp functions?
SET(CMAKE_REQUIRED_LIBRARIES m)

include(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX("cmath" HAVE_CMATH_H)

include(CheckFunctionExists)
check_function_exists(cos HAVE_COS)
check_function_exists(sin HAVE_SIN)
check_function_exists(log HAVE_LOG)
check_function_exists(exp HAVE_EXP)
check_function_exists(pow HAVE_POW)
check_function_exists(sqrt HAVE_SQRT)
check_function_exists(abs HAVE_ABS)
check_function_exists(cbrt HAVE_CBRT)

list(APPEND GTest_LIBRARIES gtest gtest_main)
separate_arguments(GTest_LIBRARIES)

file(GLOB APP_SOURCES ./*.cxx)
FOREACH (source_file ${APP_SOURCES})
    get_filename_component(prog_name ${source_file} NAME_WLE)
    add_executable(SGVXR_${prog_name} ${source_file})

    IF (${prog_name} STREQUAL "TestMemoryLeak")
        FIND_FILE (VALGRIND_HEADER_FILE "valgrind/valgrind.h")
        IF (VALGRIND_HEADER_FILE)
            target_compile_definitions(SGVXR_${prog_name} PRIVATE HAS_VALGRIND)
        ENDIF (VALGRIND_HEADER_FILE)
    ENDIF()

    target_link_libraries(SGVXR_${prog_name} ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs}
            ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
    ADD_DEPENDENCIES(SGVXR_${prog_name} SimpleGVXR)

    IF (NOT USE_SYSTEM_GTEST)
        ADD_DEPENDENCIES(SGVXR_${prog_name} googletest)
    ENDIF (NOT USE_SYSTEM_GTEST)

    ADD_TEST(SGVXR_${prog_name} SGVXR_${prog_name})
ENDFOREACH (source_file ${APP_SOURCES})

FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/)
FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/point_source)
FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/SBP)
FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/FBP)
FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/ART)
FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/MART)
FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/SIRT)

CONFIGURE_FILE (data/hydrogen.tsv   data/hydrogen.tsv   COPYONLY)
CONFIGURE_FILE (data/carbon.tsv     data/carbon.tsv   COPYONLY)
CONFIGURE_FILE (data/nitrogen.tsv   data/nitrogen.tsv   COPYONLY)
CONFIGURE_FILE (data/oxygen.tsv     data/oxygen.tsv   COPYONLY)
CONFIGURE_FILE (data/sodium.tsv	data/sodium.tsv		COPYONLY)
CONFIGURE_FILE (data/magnesium.tsv	data/magnesium.tsv	COPYONLY)
CONFIGURE_FILE (data/phosphor.tsv	data/phosphor.tsv	COPYONLY)
CONFIGURE_FILE (data/sulfur.tsv	data/sulfur.tsv	COPYONLY)
CONFIGURE_FILE (data/clorine.tsv	data/clorine.tsv	COPYONLY)
CONFIGURE_FILE (data/argon.tsv	data/argon.tsv	COPYONLY)
CONFIGURE_FILE (data/potassium.tsv	data/potassium.tsv	COPYONLY)
CONFIGURE_FILE (data/calcium.tsv	data/calcium.tsv	COPYONLY)
CONFIGURE_FILE (data/titanium.tsv	data/titanium.tsv	COPYONLY)
CONFIGURE_FILE (data/copper.tsv	data/copper.tsv	COPYONLY)
CONFIGURE_FILE (data/zinc.tsv	data/zinc.tsv	COPYONLY)
CONFIGURE_FILE (data/silver.tsv	data/silver.tsv	COPYONLY)
CONFIGURE_FILE (data/tin.tsv	data/tin.tsv	COPYONLY)
CONFIGURE_FILE (data/iron.tsv	data/iron.tsv	COPYONLY)
CONFIGURE_FILE (data/iodine.tsv	data/iodine.tsv	COPYONLY)
CONFIGURE_FILE (data/Ti90Al6V4.tsv	data/Ti90Al6V4.tsv	COPYONLY)
CONFIGURE_FILE (data/H2O.tsv	data/H2O.tsv	COPYONLY)
CONFIGURE_FILE (data/SiC.tsv	data/SiC.tsv	COPYONLY)
CONFIGURE_FILE (runGnuplot.sh    runGnuplot.sh		COPYONLY)

CONFIGURE_FILE (data/bone.csv   data/bone.tsv   COPYONLY)
CONFIGURE_FILE (data/brain.csv  data/brain.tsv  COPYONLY)
CONFIGURE_FILE (data/gas.csv    data/gas.tsv    COPYONLY)
CONFIGURE_FILE (data/tissue.csv data/tissue.tsv COPYONLY)
CONFIGURE_FILE (data/water.csv  data/water.tsv  COPYONLY)
CONFIGURE_FILE (data/phantom_sinogram.mhd      data/phantom_sinogram.mhd  COPYONLY)
CONFIGURE_FILE (data/phantom_sinogram.raw      data/phantom_sinogram.raw  COPYONLY)
CONFIGURE_FILE (data/phantom_projections.mhd   data/phantom_projections.mhd  COPYONLY)
CONFIGURE_FILE (data/phantom_projections.raw   data/phantom_projections.raw  COPYONLY)
CONFIGURE_FILE (data/point_source/gate_simulation.mhd       data/point_source/gate_simulation.mhd  COPYONLY)
CONFIGURE_FILE (data/point_source/gate_simulation.raw       data/point_source/gate_simulation.raw  COPYONLY)
CONFIGURE_FILE (data/cube_source/gate_simulation.mhd       data/cube_source/gate_simulation.mhd  COPYONLY)
CONFIGURE_FILE (data/cube_source/gate_simulation.raw       data/cube_source/gate_simulation.raw  COPYONLY)
CONFIGURE_FILE (data/uncentered_source/gate_simulation.mhd       data/uncentered_source/gate_simulation.mhd  COPYONLY)
CONFIGURE_FILE (data/uncentered_source/gate_simulation.raw       data/uncentered_source/gate_simulation.raw  COPYONLY)

CONFIGURE_FILE (data/CT_reconstruction/ct_volume_in_mu_80keV.mhd   data/CT_reconstruction/ct_volume_in_mu_80keV.mhd  COPYONLY)
CONFIGURE_FILE (data/CT_reconstruction/ct_volume_in_mu_80keV.raw   data/CT_reconstruction/ct_volume_in_mu_80keV.raw  COPYONLY)
CONFIGURE_FILE (data/CT_reconstruction/projection_set.mhd          data/CT_reconstruction/projection_set.mhd         COPYONLY)
CONFIGURE_FILE (data/CT_reconstruction/projection_set.raw          data/CT_reconstruction/projection_set.raw         COPYONLY)
CONFIGURE_FILE (data/CT_reconstruction/sinogram.mhd                data/CT_reconstruction/sinogram.mhd               COPYONLY)
CONFIGURE_FILE (data/CT_reconstruction/sinogram.raw                data/CT_reconstruction/sinogram.raw               COPYONLY)

CONFIGURE_FILE (data/55kV.tsv                data/55kV.tsv               COPYONLY)

CONFIGURE_FILE (data/spekpy_filtered.dat      data/spekpy_filtered.dat      COPYONLY)
CONFIGURE_FILE (data/spekpy_unfiltered.dat    data/spekpy_unfiltered.dat    COPYONLY)
CONFIGURE_FILE (data/xpecgen_filtered.dat     data/xpecgen_filtered.dat     COPYONLY)
CONFIGURE_FILE (data/xpecgen_unfiltered.dat   data/xpecgen_unfiltered.dat   COPYONLY)
