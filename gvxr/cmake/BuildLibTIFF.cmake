macro(build_libtiff install_prefix staging_prefix temp_path)


# make a custom LibTIFF configuration file

SET (LIBTIFF_VERSION_STRING 4.0)
SET (LIBTIFF_VERSION_MAJOR  4.0)
SET (LIBTIFF_VERSION_MINOR  6)

set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
if(APPLE)
  list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
    -DMACOSX_RPATH:BOOL=ON
    -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
  )
endif()

SET (TIFF_INSTALL_DIR ${staging_prefix}/${install_prefix}-install)

IF (UNIX AND NOT APPLE)
    ExternalProject_Add(libtiff
        URL  "http://download.osgeo.org/libtiff/tiff-4.0.6.zip"
        URL_MD5 "f5b485d750b2001255ed64224b98b857"
        UPDATE_COMMAND ""
        SOURCE_DIR  ${temp_path}/${install_prefix}-source
        BINARY_DIR  ${temp_path}/${install_prefix}-build
        INSTALL_DIR ${TIFF_INSTALL_DIR}
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
            -DCMAKE_INSTALL_PREFIX:PATH=${TIFF_INSTALL_DIR}
            -Djpeg:BOOL=OFF
            -Dlzma:BOOL=OFF
            -DCMAKE_INSTALL_LIBDIR=lib64
    )
ELSE ()
    ExternalProject_Add(libtiff
        URL  "http://download.osgeo.org/libtiff/tiff-4.0.6.zip"
        URL_MD5 "f5b485d750b2001255ed64224b98b857"
        UPDATE_COMMAND ""
        SOURCE_DIR  ${temp_path}/${install_prefix}-source
        BINARY_DIR  ${temp_path}/${install_prefix}-build
        INSTALL_DIR ${TIFF_INSTALL_DIR}
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
            -DCMAKE_INSTALL_PREFIX:PATH=${TIFF_INSTALL_DIR}
            -Djpeg:BOOL=OFF
            -Dlzma:BOOL=OFF
    )
ENDIF ()

SET(TIFF_INCLUDE_DIR  ${TIFF_INSTALL_DIR}/include CACHE PATH "Where to find LibTIFF header files" FORCE)
SET(TIFF_INCLUDE_DIRS ${TIFF_INSTALL_DIR}/include CACHE PATH "Where to find LibTIFF header files" FORCE)
SET(TIFF_LIBRARY_DIR  ${TIFF_INSTALL_DIR}/lib CACHE PATH "Where to find LibTIFF library files" FORCE)

SET(TIFF_LIBRARIES    ${TIFF_INSTALL_DIR}/lib/libtiff.a  CACHE FILEPATH "LibTIFF library" FORCE)

if (WIN32 AND ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    SET(TIFF_LIBRARIES
        optimized ${TIFF_INSTALL_DIR}/lib/tiff.lib
        debug     ${TIFF_INSTALL_DIR}/lib/tiffd.lib
        CACHE FILEPATH "LibTIFF library" FORCE
    )
else ()
    IF (UNIX AND NOT APPLE)
        SET(TIFF_LIBRARIES    ${TIFF_INSTALL_DIR}/lib64/libtiff.a   CACHE FILEPATH "LibTIFF library" FORCE)
    ENDIF()
endif ()

SET (TIFF_LIBRARY ${TIFF_LIBRARIES} CACHE FILEPATH "LibTIFF library" FORCE)

SET(TIFF_FOUND ON)


# copy libraries into install tree
install(DIRECTORY ${TIFF_INSTALL_DIR}/include/ DESTINATION ${INSTALL_DIR}/third_party/include COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${TIFF_INSTALL_DIR}/lib/     DESTINATION ${INSTALL_DIR}/third_party/lib     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${TIFF_INSTALL_DIR}/lib64/   DESTINATION ${INSTALL_DIR}/third_party/lib64   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${TIFF_INSTALL_DIR}/bin/     DESTINATION ${INSTALL_DIR}/third_party/bin     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${TIFF_INSTALL_DIR}/share/   DESTINATION ${INSTALL_DIR}/third_party/share   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${TIFF_INSTALL_DIR}/man/     DESTINATION ${INSTALL_DIR}/third_party/man     COMPONENT Development FILES_MATCHING PATTERN "*.*")

endmacro(build_libtiff)
