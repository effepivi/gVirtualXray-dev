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


# Configure the project ########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../cmake/configureGlfw.cmake)


# Check if GLFW is enable #####################################################
IF (USE_SYSTEM_GLFW)
    FIND_PACKAGE( glfw3   REQUIRED )
ENDIF (USE_SYSTEM_GLFW)

IF (glfw3_FOUND)
    INCLUDE_DIRECTORIES(${GLFW3_INCLUDE_DIR})
    SET(requiredLibs ${requiredLibs} ${GLFW3_LIBRARY})
ELSE (glfw3_FOUND)
    MESSAGE(WARNING "GLFW3 not found, (${PROJECT_NAME}) will be disable.")
ENDIF (glfw3_FOUND)


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

# FFTW3 #######################################################################
FIND_LIBRARY(FFTW3_LIBRARIES NAMES fftw3 libfftw3 libfftw3_omp)
IF (FFTW3_LIBRARIES)
    SET (requiredLibs ${requiredLibs} ${FFTW3_LIBRARIES})
    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_FFTW")
ELSE ()
    MESSAGE (WARNING "FFTW3 not found, the use of FFT will be disable.")
ENDIF ()

if(Boost_FOUND)
    # Valiation_02
    add_executable(TestDensity                     TestDensity.cxx)
    
    # Validation_01
    add_executable(TestMassAttenuationCoefficients TestMassAttenuationCoefficients.cxx)

	IF (glfw3_FOUND)
	    # Validation_03
	    add_executable(TestBeerLambertLawMonoChromatic TestBeerLambertLawMonoChromatic.cxx)
	
	    # Validation_05
	    add_executable(TestBeerLambertLawPolyChromatic TestBeerLambertLawPolyChromatic.cxx)
	
	    # Validation_08
	    add_executable(TestSinogramFromMesh            TestSinogramFromMesh.cxx)
	
	    # Validation_??
	    #add_executable(TestRadonTransformFromVoxels   TestRadonTransformFromVoxels.cxx)
	
	    # Validation_??
	    #add_executable(TestSinogramFromVoxels          TestSinogramFromVoxels.cxx)
	
	    # Validation_06
	    add_executable(TestPointSource                 TestPointSource.cxx)
	
	    # Validation_07
	    add_executable(TestCubeSource                  TestCubeSource.cxx)
	
	    # Validation_09
	    add_executable(TestUncenteredSource            TestUncenteredSource.cxx)
	
	    # Validation_??
	    #add_executable(TestPhantomMonoChromatic        TestPhantomMonoChromatic.cxx)
	
	    # Validation_04
	    add_executable(TestRadonTransformFromMesh       TestRadonTransformFromMesh.cxx)
	
	    IF (FFTW3_LIBRARIES)
		    # Validation_04
	        add_executable(TestFBP                     TestFBP.cxx)
	    ENDIF()
	ENDIF()
	
	    target_link_libraries(TestDensity                     ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})
	    target_link_libraries(TestMassAttenuationCoefficients ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})
	IF (glfw3_FOUND)
	    target_link_libraries(TestBeerLambertLawMonoChromatic ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})
	    target_link_libraries(TestBeerLambertLawPolyChromatic ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})
	    target_link_libraries(TestSinogramFromMesh            ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})
	    #target_link_libraries(TestSinogramFromVoxels          ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})
	    target_link_libraries(TestPointSource                 ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})
	    target_link_libraries(TestCubeSource                  ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})
	    target_link_libraries(TestUncenteredSource            ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})
	    #target_link_libraries(TestPhantomMonoChromatic        ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})
	    target_link_libraries(TestRadonTransformFromMesh      ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})
	
	    IF (FFTW3_LIBRARIES)
	        target_link_libraries(TestFBP                     ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})
	    ENDIF()
	ENDIF()
	
	    if (gVirtualXRay_BINARY_DIR)
	         ADD_DEPENDENCIES(TestDensity   gVirtualXRay)
	         ADD_DEPENDENCIES(TestMassAttenuationCoefficients   gVirtualXRay)
	IF (glfw3_FOUND)
	         ADD_DEPENDENCIES(TestBeerLambertLawMonoChromatic   gVirtualXRay)
	         ADD_DEPENDENCIES(TestBeerLambertLawPolyChromatic   gVirtualXRay)
	         ADD_DEPENDENCIES(TestSinogramFromMesh              gVirtualXRay)
	         #ADD_DEPENDENCIES(TestSinogramFromVoxels            gVirtualXRay)
	         ADD_DEPENDENCIES(TestPointSource                   gVirtualXRay)
	         ADD_DEPENDENCIES(TestCubeSource                    gVirtualXRay)
	         ADD_DEPENDENCIES(TestUncenteredSource              gVirtualXRay)
	         #ADD_DEPENDENCIES(TestPhantomMonoChromatic          gVirtualXRay)
	         ADD_DEPENDENCIES(TestRadonTransformFromMesh        gVirtualXRay)
	
	    IF (FFTW3_LIBRARIES)
	         ADD_DEPENDENCIES(TestFBP                           gVirtualXRay)
	    ENDIF()
	ENDIF()
	    endif (gVirtualXRay_BINARY_DIR)
	
	    add_test (TestDensity TestDensity)
	    add_test (TestMassAttenuationCoefficients TestMassAttenuationCoefficients)
	IF (glfw3_FOUND)
	    add_test (TestBeerLambertLawMonoChromatic TestBeerLambertLawMonoChromatic)
	    add_test (TestBeerLambertLawPolyChromatic TestBeerLambertLawPolyChromatic)
	    add_test (TestSinogramFromMesh            TestSinogramFromMesh)
	    #add_test (TestSinogramFromVoxels          TestSinogramFromVoxels)
	    add_test (TestPointSource                 TestPointSource)
	    add_test (TestCubeSource                  TestCubeSource)
	    add_test (TestUncenteredSource            TestUncenteredSource)
	    #add_test (TestPhantomMonoChromatic        TestPhantomMonoChromatic)
	    add_test (TestRadonTransformFromMesh      TestRadonTransformFromMesh)
	    
	    IF (FFTW3_LIBRARIES)
	        add_test (TestFBP                     TestFBP)
	    ENDIF()
	ENDIF()

    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/point_source)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/SBP)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/FBP)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/ART)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/MART)
    FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/SIRT)
    
    CONFIGURE_FILE (../validation/data/validation_01/bone.csv   data/bone.csv   COPYONLY)
    CONFIGURE_FILE (../validation/data/validation_01/brain.csv  data/brain.csv  COPYONLY)
    CONFIGURE_FILE (../validation/data/validation_01/gas.csv    data/gas.csv    COPYONLY)
    CONFIGURE_FILE (../validation/data/validation_01/tissue.csv data/tissue.csv COPYONLY)
    CONFIGURE_FILE (../validation/data/validation_01/water.csv  data/water.csv  COPYONLY)
    CONFIGURE_FILE (../validation/data/validation_08/phantom_sinogram.mhd      data/phantom_sinogram.mhd  COPYONLY)
    CONFIGURE_FILE (../validation/data/validation_08/phantom_sinogram.raw      data/phantom_sinogram.raw  COPYONLY)
    CONFIGURE_FILE (../validation/data/validation_08/phantom_projections.mhd   data/phantom_projections.mhd  COPYONLY)
    CONFIGURE_FILE (../validation/data/validation_08/phantom_projections.raw   data/phantom_projections.raw  COPYONLY)
    CONFIGURE_FILE (../validation/data/validation_06/gate_simulation.mhd       data/point_source/gate_simulation.mhd  COPYONLY)
    CONFIGURE_FILE (../validation/data/validation_06/gate_simulation.raw       data/point_source/gate_simulation.raw  COPYONLY)
    CONFIGURE_FILE (../validation/data/validation_07/gate_simulation.mhd       data/cube_source/gate_simulation.mhd  COPYONLY)
    CONFIGURE_FILE (../validation/data/validation_07/gate_simulation.raw       data/cube_source/gate_simulation.raw  COPYONLY)
    CONFIGURE_FILE (../validation/data/validation_09/gate_simulation.mhd       data/uncentered_source/gate_simulation.mhd  COPYONLY)
    CONFIGURE_FILE (../validation/data/validation_09/gate_simulation.raw       data/uncentered_source/gate_simulation.raw  COPYONLY)

    CONFIGURE_FILE (../validation/data/validation_04/ct_volume_in_mu_80keV.mhd   data/CT_reconstruction/ct_volume_in_mu_80keV.mhd  COPYONLY)
    CONFIGURE_FILE (../validation/data/validation_04/ct_volume_in_mu_80keV.raw   data/CT_reconstruction/ct_volume_in_mu_80keV.raw  COPYONLY)
    CONFIGURE_FILE (../validation/data/validation_04/projection_set.mhd          data/CT_reconstruction/projection_set.mhd         COPYONLY)
    CONFIGURE_FILE (../validation/data/validation_04/projection_set.raw          data/CT_reconstruction/projection_set.raw         COPYONLY)
    CONFIGURE_FILE (../validation/data/validation_04/sinogram.mhd                data/CT_reconstruction/sinogram.mhd               COPYONLY)
    CONFIGURE_FILE (../validation/data/validation_04/sinogram.raw                data/CT_reconstruction/sinogram.raw               COPYONLY)

ENDIF(Boost_FOUND)

