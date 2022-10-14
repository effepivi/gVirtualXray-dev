macro(build_zlib install_prefix staging_prefix temp_path)


# make a custom ZLIB configuration file
SET (URL "http://zlib.net/zlib-1.2.13.tar.gz")
SET (HASH "b3a24de97a8fdbc835b9833169501030b8977031bcb54b3b3ac13740f846ab30")

SET (ZLIB_VERSION_STRING 1.2)
SET (ZLIB_VERSION_MAJOR  1.2)
SET (ZLIB_VERSION_MINOR  13)

set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
if(APPLE)
  list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
    -DMACOSX_RPATH:BOOL=ON
    -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
  )
endif()

SET (ZLIB_INSTALL_DIR ${staging_prefix}/${install_prefix}-install)

IF (UNIX AND NOT APPLE)
    ExternalProject_Add(zlib
        URL  ${URL}
        URL_HASH SHA256=${HASH}
        UPDATE_COMMAND ""
        SOURCE_DIR  ${temp_path}/${install_prefix}-source
        BINARY_DIR  ${temp_path}/${install_prefix}-build
        INSTALL_DIR ${ZLIB_INSTALL_DIR}
        LIST_SEPARATOR :::
        CMAKE_GENERATOR ${CMAKE_GEN}
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE:STRING=Release
            -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
            -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
            -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
            -DCMAKE_EXE_LINKER_FLAGS:STRING=${LINKER_FLAGS}
            -DBUILD_SHARED_LIBS:BOOL=OFF
            -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}
            -DCMAKE_INSTALL_PREFIX:PATH=${ZLIB_INSTALL_DIR}
            -DCMAKE_INSTALL_LIBDIR=lib64
    )
ELSE ()
    ExternalProject_Add(zlib
        URL  ${URL}
        URL_HASH SHA256=${HASH}
        UPDATE_COMMAND ""
        SOURCE_DIR  ${temp_path}/${install_prefix}-source
        BINARY_DIR  ${temp_path}/${install_prefix}-build
        INSTALL_DIR ${ZLIB_INSTALL_DIR}
        LIST_SEPARATOR :::
        CMAKE_GENERATOR ${CMAKE_GEN}
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE:STRING=Release
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
            -DCMAKE_INSTALL_PREFIX:PATH=${ZLIB_INSTALL_DIR}
    )
ENDIF ()

SET(ZLIB_INCLUDE_DIR ${ZLIB_INSTALL_DIR}/include CACHE PATH "Path of zlib's header files." FORCE)
SET(ZLIB_DIR         ${ZLIB_INSTALL_DIR} CACHE PATH "Path of zlib's directory." FORCE)
SET(ZLIB_FOUND ON FORCE)

SET (ZLIB_LIBRARY     ${ZLIB_INSTALL_DIR}/lib/libz.a CACHE FILEPATH "Path of zlib's library." FORCE )
SET (ZLIB_LIBRARY_DEBUG     ${ZLIB_INSTALL_DIR}/lib/libz.a CACHE FILEPATH "Path of zlib's debug library." FORCE )
SET (ZLIB_LIBRARY_RELEASE     ${ZLIB_INSTALL_DIR}/lib/libz.a CACHE FILEPATH "Path of zlib's release library." FORCE )

if (WIN32)
    IF (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
        SET (ZLIB_LIBRARY
            optimized ${ZLIB_INSTALL_DIR}/lib/zlibstatic.lib
            debug     ${ZLIB_INSTALL_DIR}/lib/zlibstaticd.lib
            CACHE FILEPATH "Path of zlib's library." FORCE
        )
    ELSE ()
        SET (ZLIB_LIBRARY     ${ZLIB_INSTALL_DIR}/lib/libzlibstatic.a CACHE FILEPATH "Path of zlib's library." FORCE)
        SET (ZLIB_LIBRARY_DEBUG     ${ZLIB_INSTALL_DIR}/lib/libzlibstatic.a CACHE FILEPATH "Path of zlib's debug library." FORCE)
        SET (ZLIB_LIBRARY_RELEASE     ${ZLIB_INSTALL_DIR}/lib/libzlibstatic.a CACHE FILEPATH "Path of zlib's release library." FORCE)
    ENDIF ()
endif (WIN32)

install(DIRECTORY ${ZLIB_INSTALL_DIR}/include/ DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/include COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${ZLIB_INSTALL_DIR}/lib/     DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/lib     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${ZLIB_INSTALL_DIR}/lib64/   DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/lib64   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${ZLIB_INSTALL_DIR}/bin/     DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/bin     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${ZLIB_INSTALL_DIR}/share/   DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/share   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${ZLIB_INSTALL_DIR}/man/     DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/man     COMPONENT Development FILES_MATCHING PATTERN "*.*")

install(DIRECTORY ${ZLIB_INSTALL_DIR}/lib/     DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/lib     COMPONENT Runtime FILES_MATCHING PATTERN "*.so*")
install(DIRECTORY ${ZLIB_INSTALL_DIR}/lib64/   DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/lib     COMPONENT Runtime FILES_MATCHING PATTERN "*.so*")

endmacro(build_zlib)
