cmake_minimum_required(VERSION 2.8.2)

if(POLICY CMP0048)
    cmake_policy (SET CMP0048 NEW)
    PROJECT(googletest-dow VERSION 1.0.0)
else ()
    PROJECT(googletest-dow)
endif()


find_package(Threads REQUIRED)

include(ExternalProject)

set(GTEST_DISABLE_PTHREADS ON)
set(GTEST_FORCE_SHARED_CRT ON)

FIND_PACKAGE(Git)
IF (GIT_FOUND)
    ExternalProject_Add(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        CMAKE_ARGS
            -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
            -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
            -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
            -DCMAKE_EXE_LINKER_FLAGS:STRING=${LINKER_FLAGS}
            -DBUILD_SHARED_LIBS:BOOL=OFF
            -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}
            -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
            -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
            -DMACOSX_RPATH:BOOL=ON
            -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
            -DBUILD_UTILS:BOOL=OFF
            -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
            -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
            -Dgtest_force_shared_crt=${GTEST_FORCE_SHARED_CRT}
            -Dgtest_disable_pthreads=${GTEST_DISABLE_PTHREADS}
            -DBUILD_GTEST=ON
            -DBUILD_GMOCK=OFF
            -DBUILD_UTILS=OFF
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
        # Disable install step
        INSTALL_COMMAND ""
    )
ELSE (GIT_FOUND)
    ExternalProject_Add(googletest
        URL https://github.com/google/googletest/archive/master.zip
        CMAKE_ARGS
            -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
            -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
            -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
            -DCMAKE_EXE_LINKER_FLAGS:STRING=${LINKER_FLAGS}
            -DBUILD_SHARED_LIBS:BOOL=OFF
            -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}
            -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
            -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
            -DMACOSX_RPATH:BOOL=ON
            -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
            -DBUILD_UTILS:BOOL=OFF
            -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
            -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
            -Dgtest_force_shared_crt=${GTEST_FORCE_SHARED_CRT}
            -Dgtest_disable_pthreads=${GTEST_DISABLE_PTHREADS}
            -DBUILD_GTEST=ON
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
        # Disable install step
        INSTALL_COMMAND ""
    )
ENDIF (GIT_FOUND)

# Specify include dir
ExternalProject_Get_Property(googletest source_dir)
set(GTEST_INCLUDE_DIRS ${source_dir}/googletest/include PARENT_SCOPE)

# Specify MainTest's link libraries
ExternalProject_Get_Property(googletest binary_dir)

IF (MSVC)
    SET(GTEST_LIBS_DIR ${binary_dir}/googletest/ReleaseLibs PARENT_SCOPE)
ELSE ()
    set(GTEST_LIBS_DIR ${binary_dir}/lib PARENT_SCOPE)
ENDIF ()
