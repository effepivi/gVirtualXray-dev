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


# Project name #################################################################
PROJECT(gVirtualXRay)

INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.cmake)

# Initialise some states by default ############################################
SET (BUILD_SHARED_LIBS OFF)


# Minimum CMake version ########################################################
cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)


# Flag required for shared libraries
IF (UNIX)
    IF (NOT APPLE)
        IF ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
            SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
            SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
        ENDIF ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
    ENDIF (NOT APPLE)
ENDIF (UNIX)


#IF (COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
    cmake_policy(SET CMP0011 NEW)
    cmake_policy(SET CMP0013 NEW)
    cmake_policy(SET CMP0053 NEW)
#ENDIF (COMMAND cmake_policy)

IF (APPLE)
    cmake_policy(SET CMP0042 NEW)
ENDIF (APPLE)


# Support for the CTest dashboard testing system
INCLUDE(CTest)


# Set a default build type if none was specified
IF (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  SET (CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)

  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
ENDIF ()


SET (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


# Make include files in paths like /usr/local/include have preference before
# files in OSX frameworks. 
SET (CMAKE_FIND_FRAMEWORK "LAST")


# Enable all compiler warnings #################################################
#IF (NOT WIN32)
#	SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-Wall")
#ENDIF (NOT WIN32)


# for superbuild
SET (USE_SUPERBUILD ON)
INCLUDE(ExternalProject)
INCLUDE(ConfigureExternalProjects)


# Libraries ####################################################################
SET (optionalLibs)
SET (requiredLibs)

SET (gVirtualXRay_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET (gVirtualXRay_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
SET (gVirtualXRay_INSTALL_PREFIX ${PROJECT_BINARY_DIR})

SET (gVirtualXRay_LIBRARY ${PROJECT_BINARY_DIR}/libgVirtualXRay.a)

IF (APPLE)

    SET (requiredLibs ${requiredLibs} "-framework CoreFoundation")

    IF(${CMAKE_GENERATOR} STREQUAL "Xcode")
        SET (gVirtualXRay_LIBRARY 
            optimized ${PROJECT_BINARY_DIR}/Release/libgVirtualXRay.a
            debug     ${PROJECT_BINARY_DIR}/Debug/libgVirtualXRay.a
        )
    ENDIF ()
ENDIF ()

if (WIN32 AND ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    SET (gVirtualXRay_LIBRARY optimized ${PROJECT_BINARY_DIR}/Release/gVirtualXRay.lib
                              debug     ${PROJECT_BINARY_DIR}/Debug/gVirtualXRay.lib
    )
ENDIF ()


# Library source code ##########################################################
SET (CXX_SOURCES
	# gVirtualXRay
	${CMAKE_CURRENT_SOURCE_DIR}/src/gVirtualXRay/XRayDetector.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gVirtualXRay/XRayRenderer.cxx

	# Graphics
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/framebufferObject.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/Image.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/ImplicitSurface.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/OpenGL2VBO.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/OpenGL3VBO.cxx
    #${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/ParticleSystem.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/PolygonMesh.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/Shader.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/SphereMesh.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/StereoHelper.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/TextRenderer.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/VBO.cxx
	
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/AnatomicalMesh.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/InternalOrganMesh.cxx 
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/SoftTissueMesh.cxx 
	
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/BoneMesh.cxx 
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/DiaphragmMesh.cxx 
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/LiverMesh.cxx 
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/LungsMesh.cxx 
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/SkinMesh.cxx

	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/DepthMap.cxx

    # Utilities
	${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities/AtomicElement.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities/ElementSet.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities/FBOException.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities/MaterialSet.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities/OpenGLException.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities/OpenGLUtilities.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities/TissueMaterial.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities/Utilities.cxx
)


# Add header files to the project
SET (HXX_SOURCES
	# gVirtualXRay
	${CMAKE_CURRENT_BINARY_DIR}/gVirtualXRay/gVirtualXRayConfig.h

	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/AttenuationCoefficient.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/AttenuationCoefficient.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/SpectrumRecord.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/SpectrumRecord.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/XRayBeam.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/XRayBeam.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/XRayDetector.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/XRayDetector.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/XRayRenderer.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/XRayRenderer.inl

	# Graphics
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/CubeMesh.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/CubeMesh.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/framebufferObject.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/FFT.h
   	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/FFT.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Image.h
   	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Image.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/ImplicitSurface.h
   	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/ImplicitSurface.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Material.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Material.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/OpenGL2VBO.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/OpenGL2VBO.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/OpenGL3VBO.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/OpenGL3VBO.inl
	#${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/ParticleSystem.h
	#${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/ParticleSystem.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/PolygonMesh.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/PolygonMesh.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Shader.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Shader.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/StereoHelper.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/StereoHelper.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/SphereMesh.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/SphereMesh.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/VBO.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/VBO.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/TextRenderer.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/TextRenderer.inl

	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/AnatomicalMesh.h 
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/InternalOrganMesh.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/SoftTissueMesh.h
	
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/BoneMesh.h 
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/DiaphragmMesh.h 
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/LiverMesh.h 
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/LungsMesh.h 
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/SkinMesh.h

	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/DepthMap.h 
	
	# Math
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Matrix4x4.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Matrix4x4.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Vec2.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Vec2.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Vec3.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Vec3.inl

	# Utilities
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/AtomicElement.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/AtomicElement.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/ConstantValues.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/ElementSet.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/ElementSet.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Exception.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Exception.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/FBOException.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/FileDoesNotExistException.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/FileDoesNotExistException.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/MaterialSet.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/MaterialSet.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/OpenGLException.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/OpenGLUtilities.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/OpenGLUtilities.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/OutOfBoundsException.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/OutOfBoundsException.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/OutOfMemoryException.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/OutOfMemoryException.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/TissueMaterial.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/TissueMaterial.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Types.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Units.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Utilities.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Utilities.inl
)


# Add source to the project ####################################################
IF (WIN32)
	IF (NOT CYGWIN)
		SET (SOURCES ${CXX_SOURCES} ${HXX_SOURCES})
	ELSE(NOT CYGWIN)
		SET (SOURCES ${CXX_SOURCES})
	ENDIF (NOT CYGWIN)
ELSE(WIN32)
	IF (APPLE)
		SET (SOURCES ${CXX_SOURCES} ${HXX_SOURCES})
	ELSE(APPLE)
		SET (SOURCES ${CXX_SOURCES})
	ENDIF (APPLE)
ENDIF (WIN32)


# Headers of the project #######################################################
SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${PROJECT_BINARY_DIR}")
SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${CMAKE_CURRENT_SOURCE_DIR}/include")

IF (NOT APPLE)
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${CMAKE_CURRENT_SOURCE_DIR}/include/OpenGL4")
ENDIF (NOT APPLE)

#SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "/usr/local/include")


# OpenMP ######################################################################
FIND_PACKAGE(OpenMP)
IF (OPENMP_FOUND)

    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_OPENMP")
    SET (requiredLibs ${requiredLibs} ${OpenMP_EXE_LINKER_FLAGS})
ENDIF (OPENMP_FOUND)


# GZIP ########################################################################
FIND_PACKAGE(GZIP)
IF (NOT GZIP_FOUND)
    IF (WIN32)
        file(DOWNLOAD http://www.gzip.org/gzip124xN.exe ${PROJECT_BINARY_DIR}/gzip124xN.exe)
		
		# Execute gzip124xN.exe to extract gzip.exe
		execute_process(COMMAND ${PROJECT_BINARY_DIR}/gzip124xN.exe WORKING_DIRECTORY ${PROJECT_BINARY_DIR})


        FIND_PACKAGE(GZIP REQUIRED)
        IF (NOT GZIP_FOUND)
    	    MESSAGE(FATAL_ERROR "GZIP not found. You can find it on gzip's website at: http://www.gzip.org/")
        ENDIF (NOT GZIP_FOUND)

    ELSE (WIN32)
    	MESSAGE(FATAL_ERROR "GZIP not found.")
    ENDIF (WIN32)
ENDIF (NOT GZIP_FOUND)


# FFTW3 #######################################################################
FIND_LIBRARY(FFTW3_LIBRARIES NAMES fftw3 libfftw3 libfftw3_omp)
IF (FFTW3_LIBRARIES)
    SET (requiredLibs ${requiredLibs} ${FFTW3_LIBRARIES})
    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_FFTW")
ELSE ()
    MESSAGE (WARNING "FFTW3 not found, the use of FFT will be disable.")
ENDIF ()


# GLEW ########################################################################
# Deals with cross-compilation using Mac OS X (building GLEW does not work in this case)
OPTION(USE_SYSTEM_GLEW       "Use System GLEW"       OFF)

IF (USE_SYSTEM_GLEW)
  FIND_PACKAGE(GLEW REQUIRED)
ELSE(USE_SYSTEM_GLEW)
  INCLUDE(BuildGLEW)
  build_glew( glew ${PROJECT_BINARY_DIR} )
ENDIF (USE_SYSTEM_GLEW)


IF (GLEW_FOUND)
    SET (HAS_GLEW ON)
    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DGLEW_STATIC")
    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_GLEW")
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${GLEW_INCLUDE_DIR}")
    SET (gVirtualXRay_LIBRARY_DIRS ${gVirtualXRay_LIBRARY_DIRS} ${GLEW_LIBRARY_DIR})
    SET (requiredLibs "${requiredLibs}" "${GLEW_LIBRARY}")
ENDIF (GLEW_FOUND)


# OpenGL ######################################################################
FIND_PACKAGE(OpenGL REQUIRED)
IF (OPENGL_FOUND)
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${OPENGL_INCLUDE_DIR}")
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${GLU_INCLUDE_PATH}")
    SET (requiredLibs ${requiredLibs} ${OPENGL_LIBRARIES} ${GLU_LIBRARY})
ELSE(OPENGL_FOUND)
    MESSAGE(FATAL_ERROR "OpenGL not found.")
ENDIF (OPENGL_FOUND)


# ITK #########################################################################
FIND_PACKAGE(ITK)
IF (ITK_FOUND)
    include(${ITK_USE_FILE})

    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_ITK")

    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${ITK_INCLUDE_DIRS}")
    SET (requiredLibs ${requiredLibs} ${ITK_LIBRARIES})
ELSE(ITK_FOUND)
    MESSAGE(WARNING "ITK not found, it will be disable.")
ENDIF (ITK_FOUND)


# ZLIB #########################################################################
OPTION(USE_SYSTEM_ZLIB       "Use System ZLIB "       OFF)

IF (USE_SYSTEM_ZLIB)
    FIND_PACKAGE( ZLIB   REQUIRED )
ELSE(USE_SYSTEM_ZLIB)
    INCLUDE(BuildZLIB)
    build_zlib( zlib ${PROJECT_BINARY_DIR} )
ENDIF (USE_SYSTEM_ZLIB)

SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${ZLIB_INCLUDE_DIR}")
SET (gVirtualXRay_LIBRARY_DIRS ${gVirtualXRay_LIBRARY_DIRS} ${ZLIB_LIBRARY_DIR})
SET (requiredLibs "${requiredLibs}" "${ZLIB_LIBRARY}")


# FFTW3 ########################################################################
OPTION(USE_FFTW3       "Use FFTW3"       ON)

IF (USE_FFTW3)
    OPTION(USE_SYSTEM_FFTW3       "Use System FFTW3 "       OFF)

    IF (USE_SYSTEM_FFTW3)

        FIND_LIBRARY(FFTW3_LIBRARIES NAMES fftw3 libfftw3 libfftw3_omp)

    ELSE(USE_SYSTEM_FFTW3)

	IF (UNIX)
            INCLUDE(BuildFFTW3)
            build_fftw3( fftw3 ${PROJECT_BINARY_DIR} )
        ELSE (UNIX)
            MESSAGE (WARNING "Building FFTW3 on windows automatically is not supported. Either disable the Fast Fourier Transform, or build it by hand.")
        ENDIF (UNIX)

    ENDIF (USE_SYSTEM_FFTW3)

    IF (FFTW3_LIBRARIES)
        SET (requiredLibs ${requiredLibs} ${FFTW3_LIBRARIES})
        SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_FFTW")

        SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${FFTW3_INCLUDE_DIR}")
        SET (gVirtualXRay_LIBRARY_DIRS ${gVirtualXRay_LIBRARY_DIRS} ${FFTW3_LIBRARY_DIR})
        SET (requiredLibs "${requiredLibs}" "${FFTW3_LIBRARY}")

    ELSE ()
        MESSAGE (WARNING "FFTW3 not found, the use of FFT will be disable.")
    ENDIF ()

ENDIF (USE_FFTW3)


# FREETYPE ####################################################################
OPTION(USE_SYSTEM_FREETYPE       "Use System FreeType "       OFF)

IF (USE_SYSTEM_FREETYPE)
    FIND_PACKAGE(Freetype)

    IF (NOT FREETYPE_FOUND)
        MESSAGE(WARNING "FreeType not found. Its use will be disable. You can find it on FreeType's website at: https://www.freetype.org/")
    ENDIF (NOT FREETYPE_FOUND)

ELSE (USE_SYSTEM_FREETYPE)
    INCLUDE(BuildFreeType)
    build_freetype( freetype ${PROJECT_BINARY_DIR} )
ENDIF (USE_SYSTEM_FREETYPE)

IF (FREETYPE_FOUND)
    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_FREETYPE")
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${FREETYPE_INCLUDE_DIRS}")
    SET (optionalLibs "${optionalLibs}" "${FREETYPE_LIBRARIES}")

    FIND_PACKAGE(PNG)
    IF (PNG_FOUND)
        SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${PNG_INCLUDE_DIRS}")
        SET(optionalLibs ${optionalLibs} ${PNG_LIBRARIES})
    ENDIF (PNG_FOUND)

    FIND_PACKAGE(BZip2)
    IF (BZIP2_FOUND)
        SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${BZIP2_INCLUDE_DIR}")
        SET(optionalLibs ${optionalLibs} ${BZIP2_LIBRARIES})
    ENDIF (BZIP2_FOUND)

    #FIND_PACKAGE(HARFBUZZ)
    #IF (HARFBUZZ_FOUND)
    #    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${HARFBUZZ_INCLUDE_DIRS}")
    #    SET(optionalLibs ${optionalLibs} ${HARFBUZZ_LIBRARIES})
    #ENDIF (HARFBUZZ_FOUND)

ENDIF (FREETYPE_FOUND)


# GDCM #########################################################################

OPTION (USE_GDCM "Use GDCM (Grassroots DICOM library) to save DICOM files" ON)
IF (USE_GDCM)
    IF (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin" AND ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        OPTION(USE_SYSTEM_GDCM       "Use System GDCM"       ON)
    ELSE ()
        OPTION(USE_SYSTEM_GDCM       "Use System GDCM"       OFF)
   ENDIF ()
ENDIF (USE_GDCM)

IF (USE_GDCM)
    IF (USE_SYSTEM_GDCM)
        FIND_PACKAGE( GDCM   REQUIRED )
    ELSE(USE_SYSTEM_GDCM)
        INCLUDE(BuildGDCM)

        build_gdcm( gdcm ${PROJECT_BINARY_DIR} )
    ENDIF (USE_SYSTEM_GDCM)
ENDIF(USE_GDCM)

IF (GDCM_FOUND)
    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_GDCM")
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${GDCM_INCLUDE_DIRS}")
    SET (gVirtualXRay_LIBRARY_DIRS ${gVirtualXRay_LIBRARY_DIRS} ${GDCM_LIBRARY_DIR})
    SET (requiredLibs "${requiredLibs}" "${GDCM_LIBRARIES}")
ENDIF (GDCM_FOUND)


# TIFF #########################################################################

OPTION(USE_SYSTEM_LIBTIFF       "Use System LibTiff"       OFF)

IF (USE_SYSTEM_LIBTIFF)
    FIND_PACKAGE( TIFF   REQUIRED )
ELSE(USE_SYSTEM_LIBTIFF)
    INCLUDE(BuildLibTIFF)

    # Missing file on MV VC++ 2010
#    IF (NOT MSVC10)
        build_libtiff( LibTIFF ${PROJECT_BINARY_DIR} )
#    ENDIF (NOT MSVC10)

ENDIF (USE_SYSTEM_LIBTIFF)

IF (TIFF_FOUND)
    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_LIBTIFF" "-DHAS_TIFF")
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${TIFF_INCLUDE_DIRS}")
    SET (gVirtualXRay_LIBRARY_DIRS ${gVirtualXRay_LIBRARY_DIRS} ${TIFF_LIBRARY_DIR})
    SET (requiredLibs "${requiredLibs}" "${TIFF_LIBRARIES}")
ENDIF (TIFF_FOUND)


# XCOM Database ################################################################

# XCOM: Photon Cross Sections Database

# http://www.nist.gov/pml/data/xcom/

# http://physics.nist.gov/PhysRefData/Xcom/Text/download.html

OPTION(USE_SYSTEM_XCOM       "Use System XCOM database"       OFF)

IF (USE_SYSTEM_XCOM)
    find_path (XCOM_PATH MDATX3.001 MDATX3.100 NO_DEFAULT_PATH)
    IF (NOT XCOM_PATH)
        MESSAGE(WARNING "XCOM, Photon Cross Sections Database, not found. Its use will be disable. You can find it on the NIST's website at: http://physics.nist.gov/PhysRefData/Xcom/Text/download.html")
    ENDIF (NOT XCOM_PATH)

ELSE(USE_SYSTEM_XCOM)
    INCLUDE(BuildXCOM)
    build_xcom( ${PROJECT_BINARY_DIR} )
ENDIF (USE_SYSTEM_XCOM)


# configure a header file to pass some of the CMake settings
# to the source code
IF (XCOM_PATH)
    SET (HAS_XCOM ON)

    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_XCOM")

    configure_file (
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/gVirtualXRayConfigXCOM.h.in"
        "${PROJECT_BINARY_DIR}/gVirtualXRay/gVirtualXRayConfig.h"
    )
ELSE (XCOM_PATH)
    SET (HAS_XCOM OFF)

    configure_file (
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/gVirtualXRayConfigNoXCOM.h.in"
        "${PROJECT_BINARY_DIR}/gVirtualXRay/gVirtualXRayConfig.h"
    )
ENDIF (XCOM_PATH)


# Add preprocessor definitions
ADD_DEFINITIONS(${gVirtualXRay_ADD_DEFINITIONS})
INCLUDE_DIRECTORIES(${gVirtualXRay_INCLUDE_DIRS})


# Add library paths
LINK_DIRECTORIES(${gVirtualXRay_LIBRARY_DIRS})


# Create the library ###########################################################
ADD_LIBRARY (gVirtualXRay STATIC ${SOURCES})


# Linkage of the target ########################################################
set_target_properties(gVirtualXRay PROPERTIES LINK_SEARCH_START_STATIC ON)
SET (gVirtualXRay_LIBRARIES ${requiredLibs} ${optionalLibs} gVirtualXRay)
TARGET_LINK_LIBRARIES (gVirtualXRay ${requiredLibs} ${optionalLibs})



# Create the RESOURCES ###########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/cmake/CreateHeaderFiles.cmake)

IF (XCOM_PATH)
    #foreach(xcom_file RANGE 1 100)

    #    if (${xcom_file} LESS 10)
    #        SET (XCOM_SOURCES ${XCOM_SOURCES} ${XCOM_PATH}/MDATX3.00${xcom_file})
    #    else ()
    #        if (${xcom_file} LESS 100)
    #            SET (XCOM_SOURCES ${XCOM_SOURCES} ${XCOM_PATH}/MDATX3.0${xcom_file})
    #        else ()
    #            SET (XCOM_SOURCES ${XCOM_SOURCES} ${XCOM_PATH}/MDATX3.${xcom_file})
    #        endif ()
    #    endif ()

    #endforeach(xcom_file)

    SET (XCOM_SOURCES 
        ${XCOM_PATH}/MDATX3.001
        ${XCOM_PATH}/MDATX3.006
        ${XCOM_PATH}/MDATX3.007
        ${XCOM_PATH}/MDATX3.008

        ${XCOM_PATH}/MDATX3.011
        ${XCOM_PATH}/MDATX3.012
        ${XCOM_PATH}/MDATX3.015
        ${XCOM_PATH}/MDATX3.016
        ${XCOM_PATH}/MDATX3.017
        ${XCOM_PATH}/MDATX3.018
        ${XCOM_PATH}/MDATX3.019

        ${XCOM_PATH}/MDATX3.020
        ${XCOM_PATH}/MDATX3.026
        ${XCOM_PATH}/MDATX3.022
        ${XCOM_PATH}/MDATX3.029

        ${XCOM_PATH}/MDATX3.030

        ${XCOM_PATH}/MDATX3.047

        ${XCOM_PATH}/MDATX3.050
    )

    CreateHeaderFiles("cmake" "XCOMLibFiles" "${XCOM_SOURCES}")
    ADD_DEPENDENCIES(gVirtualXRay   XCOMLibFiles)
ENDIF (XCOM_PATH)

SET (RESOURCES_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/resources/clean_l_buffer_gl2.frag
	${CMAKE_CURRENT_SOURCE_DIR}/resources/clean_l_buffer_gl2.vert
	${CMAKE_CURRENT_SOURCE_DIR}/resources/clean_l_buffer_gl3.frag
	${CMAKE_CURRENT_SOURCE_DIR}/resources/clean_l_buffer_gl3.vert
	${CMAKE_CURRENT_SOURCE_DIR}/resources/cumulated_l_buffer_gl2.frag
	${CMAKE_CURRENT_SOURCE_DIR}/resources/cumulated_l_buffer_gl2.vert
	${CMAKE_CURRENT_SOURCE_DIR}/resources/cumulated_l_buffer_gl3.frag
	${CMAKE_CURRENT_SOURCE_DIR}/resources/cumulated_l_buffer_gl3.vert
	${CMAKE_CURRENT_SOURCE_DIR}/resources/l_buffer_gl2.frag
	${CMAKE_CURRENT_SOURCE_DIR}/resources/l_buffer_gl2.vert
	${CMAKE_CURRENT_SOURCE_DIR}/resources/l_buffer_gl3.frag
	${CMAKE_CURRENT_SOURCE_DIR}/resources/l_buffer_gl3.vert
	${CMAKE_CURRENT_SOURCE_DIR}/resources/MUx_Dx_gl2.frag
	${CMAKE_CURRENT_SOURCE_DIR}/resources/MUx_Dx_gl2.vert
	${CMAKE_CURRENT_SOURCE_DIR}/resources/MUx_Dx_gl3.frag
	${CMAKE_CURRENT_SOURCE_DIR}/resources/MUx_Dx_gl3.vert
	${CMAKE_CURRENT_SOURCE_DIR}/resources/outer_surface_l_buffer_gl2.frag
	${CMAKE_CURRENT_SOURCE_DIR}/resources/outer_surface_l_buffer_gl2.vert
	${CMAKE_CURRENT_SOURCE_DIR}/resources/outer_surface_l_buffer_gl3.frag
	${CMAKE_CURRENT_SOURCE_DIR}/resources/outer_surface_l_buffer_gl3.vert
	${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_attenuation_gl2.frag
	${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_attenuation_gl2.vert
	${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_attenuation_gl3.frag
	${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_attenuation_gl3.vert
	${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_display_gl2.frag
	${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_display_gl2.vert
	${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_display_gl3.frag
	${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_display_gl3.vert
	${CMAKE_CURRENT_SOURCE_DIR}/resources/correlation_CT_number_and_tissue_material.csv
	${CMAKE_CURRENT_SOURCE_DIR}/resources/mu_water.csv
	${CMAKE_CURRENT_SOURCE_DIR}/resources/element.csv
	${CMAKE_CURRENT_SOURCE_DIR}/resources/text_display_gl2.frag
	${CMAKE_CURRENT_SOURCE_DIR}/resources/text_display_gl2.vert
	${CMAKE_CURRENT_SOURCE_DIR}/resources/text_display_gl3.frag
	${CMAKE_CURRENT_SOURCE_DIR}/resources/text_display_gl3.vert
	${CMAKE_CURRENT_SOURCE_DIR}/resources/FreeSans.ttf
	
	${CMAKE_CURRENT_SOURCE_DIR}/resources/display_realistic_gl2.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/display_realistic_gl2.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/display_realistic_gl3.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/display_realistic_gl3.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/depth_map_gl2.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/depth_map_gl2.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/depth_map_gl3.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/depth_map_gl3.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/bump_map_generation_gl2.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/bump_map_generation_gl2.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/bump_map_generation_gl3.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/bump_map_generation_gl3.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/3d_bump_map_generation_gl2.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/3d_bump_map_generation_gl2.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/3d_bump_map_generation_gl3.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/3d_bump_map_generation_gl3.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/colour_texture_generation_gl2.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/colour_texture_generation_gl2.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/colour_texture_generation_gl3.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/colour_texture_generation_gl3.vert

)
CreateHeaderFiles("cmake" "BinaryLibFiles" "${RESOURCES_SOURCES}")


# Make sure the source is generated before the executable builds.
ADD_DEPENDENCIES(gVirtualXRay BinaryLibFiles)

IF (XCOM_PATH)
    ADD_DEPENDENCIES(gVirtualXRay XCOMLibFiles)
ENDIF (XCOM_PATH)

IF (NOT USE_SYSTEM_ZLIB)
    ADD_DEPENDENCIES(gVirtualXRay zlib)
ENDIF (NOT USE_SYSTEM_ZLIB)

IF (NOT USE_SYSTEM_XCOM)
    ADD_DEPENDENCIES(XCOMLibFiles xcom)
ENDIF (NOT USE_SYSTEM_XCOM)

IF (NOT USE_SYSTEM_GLEW)
    ADD_DEPENDENCIES(gVirtualXRay glew)
ENDIF (NOT USE_SYSTEM_GLEW)

IF (NOT USE_SYSTEM_FREETYPE)
    ADD_DEPENDENCIES(gVirtualXRay freetype)
ENDIF (NOT USE_SYSTEM_FREETYPE)

#IF (NOT USE_SYSTEM_LIBTIFF)
#    ADD_DEPENDENCIES(gVirtualXRay libtiff)
#ENDIF (NOT USE_SYSTEM_LIBTIFF)

IF (NOT USE_SYSTEM_GDCM)
    ADD_DEPENDENCIES(gVirtualXRay gdcm)
ENDIF (NOT USE_SYSTEM_GDCM)


# Installation variables #######################################################
if(NOT gVirtualXRay_INSTALL_RUNTIME_DIR)
  set(gVirtualXRay_INSTALL_RUNTIME_DIR bin)
endif()
if(NOT gVirtualXRay_INSTALL_LIBRARY_DIR)
    set(gVirtualXRay_INSTALL_LIBRARY_DIR lib)
endif()
if(NOT gVirtualXRay_INSTALL_ARCHIVE_DIR)
    set(gVirtualXRay_INSTALL_ARCHIVE_DIR lib)
endif()
if(NOT gVirtualXRay_INSTALL_INCLUDE_DIR)
    set(gVirtualXRay_INSTALL_INCLUDE_DIR include/gVirtualXRay-${gVirtualXRay_VERSION_MAJOR}.${gVirtualXRay_VERSION_MINOR})
endif()
if(NOT gVirtualXRay_INSTALL_DATA_DIR)
    set(gVirtualXRay_INSTALL_DATA_DIR share/gVirtualXRay-${gVirtualXRay_VERSION_MAJOR}.${gVirtualXRay_VERSION_MINOR})
endif()
if(NOT gVirtualXRay_INSTALL_DOC_DIR)
    set(gVirtualXRay_INSTALL_DOC_DIR share/doc/gVirtualXRay-${gVirtualXRay_VERSION_MAJOR}.${gVirtualXRay_VERSION_MINOR})
endif()
if(NOT gVirtualXRay_INSTALL_PACKAGE_DIR)
    set(gVirtualXRay_INSTALL_PACKAGE_DIR "lib/cmake/gVirtualXRay-${gVirtualXRay_VERSION_MAJOR}.${gVirtualXRay_VERSION_MINOR}")
endif()


# Installation #################################################################
INSTALL (TARGETS gVirtualXRay ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/lib                  COMPONENT Development)
INSTALL (FILES ${HXX_SOURCES}         DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/include/gVirtualXRay COMPONENT Development)
INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR})


# Generate gVirtualXRayConfig.cmake ############################################
set(gVirtualXRay_USE_FILE "${gVirtualXRay_SOURCE_DIR}/UsegVirtualXRay.cmake")
configure_file(cmake/gVirtualXRayConfig.cmake.in CMakeFiles/gVirtualXRayConfig.cmake @ONLY)

install(FILES ${gVirtualXRay_BINARY_DIR}/CMakeFiles/gVirtualXRayConfig.cmake
    UsegVirtualXRay.cmake
    DESTINATION ${gVirtualXRay_INSTALL_PACKAGE_DIR}
    COMPONENT Development)


# Options ######################################################################
option(BUILD_EXAMPLES   "Build examples." ON)
option(BUILD_TUTORIALS  "Build tutorials." ON)
option(BUILD_VALIDATION "Build validation tests." ON)


if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    #MESSAGE( "64 bits compiler detected" )
    SET( EX_PLATFORM 64 )
    SET( EX_PLATFORM_NAME "x64" )
else( CMAKE_SIZEOF_VOID_P EQUAL 8 ) 
    #MESSAGE( "32 bits compiler detected" )
    SET( EX_PLATFORM 32 )
    SET( EX_PLATFORM_NAME "x86" )
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )


IF (BUILD_EXAMPLES OR BUILD_TUTORIALS OR BUILD_VALIDATION)

    OPTION (USE_GLFW "Build the programs (demos, tutorials, and validation tests) that make use of GLFW" ON)
    IF (USE_GLFW)
        OPTION(USE_SYSTEM_GLFW       "Use System GLFW"       OFF)
    ENDIF (USE_GLFW)

ELSE ()
    SET (USE_GLFW OFF)
ENDIF ()

IF (BUILD_EXAMPLES OR BUILD_TUTORIALS)
	
    OPTION (USE_FLTK "Build the programs (demos, and tutorials) that make use of the Fast Light Toolkit (FLTK)" ON)
    IF (USE_FLTK)
        OPTION(USE_SYSTEM_FLTK   "Use System FLTK"       OFF)
    ENDIF (USE_FLTK)

    OPTION (USE_GLUT "Build the programs (demos, and tutorials) that make use of the OpenGL Utility Toolkit (GLUT)" ON)
    IF (USE_GLUT)
        IF (NOT APPLE)
            OPTION(USE_SYSTEM_GLUT   "Use System GLUT"       OFF)
        ELSE (NOT APPLE)
            OPTION(USE_SYSTEM_GLUT   "Use System GLUT"       ON)
        ENDIF (NOT APPLE)
    ENDIF (USE_GLUT)

    OPTION (USE_QT4  "Build the programs (demos, and tutorials) that make use of Qt4" ON)
    OPTION (USE_QT5  "Build the programs (demos, and tutorials) that make use of Qt5" ON)

    IF (WIN32)
	    IF (EX_PLATFORM EQUAL 32)
            OPTION (USE_QT4  "Build the programs (demos, and tutorials) that make use of Qt4" OFF)
	    ENDIF ()
    ENDIF (WIN32)
		    
ELSE ()
    SET (USE_FLTK OFF)
    SET (USE_GLUT OFF)
    SET (USE_QT4 OFF)
    SET (USE_QT5 OFF)
ENDIF ()


IF (USE_GLFW)
    IF (NOT USE_SYSTEM_GLFW)
        SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
        INCLUDE(BuildGlfw3)
        build_glfw3( glfw3 ${PROJECT_BINARY_DIR} )
        ADD_DEPENDENCIES(gVirtualXRay glfw3)
    ENDIF (NOT USE_SYSTEM_GLFW)
ENDIF (USE_GLFW)

IF (USE_GLUT)
    IF (NOT USE_SYSTEM_GLUT)
        SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
        INCLUDE(BuildGlut)
        build_glut( glut ${PROJECT_BINARY_DIR} )
	ADD_DEPENDENCIES(gVirtualXRay glut)
    ENDIF (NOT USE_SYSTEM_GLUT)
ENDIF (USE_GLUT)

IF (USE_FLTK)
    IF (NOT USE_SYSTEM_FLTK)
        SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
        INCLUDE(BuildFLTK)
        build_fltk( fltk ${PROJECT_BINARY_DIR} )
        ADD_DEPENDENCIES(gVirtualXRay fltk)
    ENDIF (NOT USE_SYSTEM_FLTK)
ENDIF (USE_FLTK)


# Doxygen documentation ########################################################
INCLUDE (${CMAKE_ROOT}/Modules/Documentation.cmake)
IF(BUILD_DOCUMENTATION)
    ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/doc/doxygen ${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen)
    MESSAGE(STATUS "Build documentation.")
ELSE (BUILD_DOCUMENTATION)
   	MESSAGE("Do not build documentation.")
ENDIF(BUILD_DOCUMENTATION)


# Tools ########################################################################
ADD_SUBDIRECTORY (${CMAKE_CURRENT_SOURCE_DIR}/tools ${CMAKE_CURRENT_BINARY_DIR}/tools_bin)


# Examples #####################################################################
IF (BUILD_EXAMPLES)
    MESSAGE(STATUS "Build examples.")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples ${CMAKE_CURRENT_BINARY_DIR}/examples_bin)
ELSE (BUILD_EXAMPLES)
   	MESSAGE("Do not build examples.")
ENDIF (BUILD_EXAMPLES)


# Tutorials ####################################################################
IF (BUILD_TUTORIALS)
   	MESSAGE(STATUS "Build tutorials.")
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tutorials ${CMAKE_CURRENT_BINARY_DIR}/tutorials_bin)
ELSE (BUILD_TUTORIALS)
   	MESSAGE("Do not build tutorials.")
ENDIF (BUILD_TUTORIALS)


# Unit tests ###################################################################
IF(BUILD_TESTING)
    ENABLE_TESTING()
	MESSAGE(STATUS "Build unit tests.")
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test ${CMAKE_CURRENT_BINARY_DIR}/test_bin)
ELSE (BUILD_TESTING)
   	MESSAGE("Do not build unit tests.")
ENDIF(BUILD_TESTING)


# Validation tests #############################################################
IF (BUILD_VALIDATION)
   	MESSAGE(STATUS "Build validation tests.")
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/validation ${CMAKE_CURRENT_BINARY_DIR}/validation_bin)
ELSE (BUILD_VALIDATION)
   	MESSAGE("Do not build validation tests.")
ENDIF (BUILD_VALIDATION)


# Installation packs ###########################################################
SET (CPACK_PACKAGE_FILE_NAME gVirtualXRay)
SET (CPACK_PACKAGE_ICON ${CMAKE_CURRENT_SOURCE_DIR}/doc/logo.ico)

IF (APPLE)
	SET (CPACK_BUNDLE_NAME gVirtualXRay)
	SET (CPACK_BUNDLE_ICON ${CMAKE_CURRENT_SOURCE_DIR}/examples/build/DragonDemo.icns)
	#SET (CPACK_BUNDLE_PLIST - path to a file that will become the bundle plist.
	#SET (CPACK_BUNDLE_STARTUP_COMMAND - path to a file that will be executed when the user opens the bundle. Could be a shell-script or a binary. 
ENDIF (APPLE)


SET (CPACK_SOURCE_IGNORE_FILES "~$")
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/cmake     cmake)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include   include)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/src       src)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/resources resources)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/examples  examples)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/test      test)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/tutorials tutorials)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/doc/dist_doc1    .)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/doc/dist_doc2   doc)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/doc/html        doc/html)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/doc/TUTORIALS   doc/tutorials)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/doc/screenshots doc/screenshots)
LIST( APPEND CPACK_SOURCE_INSTALLED_FILES       ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt      .)

SET (CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "Libraries")
SET (CPACK_COMPONENT_HEADERS_DISPLAY_NAME "C++ Headers")

SET (CPACK_COMPONENT_LIBRARIES_DESCRIPTION "Static libraries used to build programs with gVirtualXRay")
SET (CPACK_COMPONENT_HEADERS_DESCRIPTION "C/C++ header files for use with gVirtualXRay")

SET (CPACK_COMPONENT_LIBRARIES_GROUP "Development")
SET (CPACK_COMPONENT_HEADERS_GROUP "Development")

SET (CPACK_COMPONENT_GROUP_DEVELOPMENT_DESCRIPTION "All of the tools you'll ever need to develop software with gVirtualXRay")

SET (CPACK_COMPONENT_LIBRARIES_INSTALL_TYPES Developer Full)
SET (CPACK_COMPONENT_HEADERS_INSTALL_TYPES Developer Full)


IF (WIN32)
    SET (CPACK_GENERATOR NSIS)
    SET (CPACK_NSIS_PACKAGE_NAME ${MAIN_PACKAGE_NAME})
    SET (CPACK_NSIS_DISPLAY_NAME ${MAIN_PACKAGE_NAME})
    SET (CPACK_NSIS_CONTACT ${EMAIL})
    SET (CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${MAIN_PACKAGE_NAME}-${PROJECT_VERSION}")
    SET (CPACK_NSIS_MENU_LINKS "${MAIN_WEBSITE}" "Homepage for ${MAIN_PACKAGE_NAME}")
ENDIF (WIN32)


IF (NOT CPack_CMake_INCLUDED)
    INCLUDE(CPack)
ENDIF (NOT CPack_CMake_INCLUDED)

