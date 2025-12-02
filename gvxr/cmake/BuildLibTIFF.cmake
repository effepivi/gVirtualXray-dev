macro(build_libtiff install_prefix staging_prefix temp_path)


# make a custom LibTIFF configuration file
SET (URL "http://download.osgeo.org/libtiff/tiff-4.4.0.zip")
SET (MD5 "81b88f590c77dbfb9eb8e3b2c1843e77")

SET (LIBTIFF_VERSION_STRING 4.4)
SET (LIBTIFF_VERSION_MAJOR  4.4)
SET (LIBTIFF_VERSION_MINOR  0)

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
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        URL  ${URL}
        URL_MD5 ${MD5}
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
            -Djbig:BOOL=OFF
            -Dzstd:BOOL=OFF
            -Dwebp:BOOL=OFF
            -DCMAKE_INSTALL_LIBDIR=lib64
        #INSTALL_COMMAND ""
    )
ELSE ()
    ExternalProject_Add(libtiff
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        URL  ${URL}
        URL_MD5 ${MD5}
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
            -Djbig:BOOL=OFF
            -Dzstd:BOOL=OFF
            -Dwebp:BOOL=OFF
        #INSTALL_COMMAND ""
    )
ENDIF ()

SET(TIFF_INCLUDE_DIR  ${TIFF_INSTALL_DIR}/include CACHE PATH "Where to find LibTIFF header files" FORCE)
SET(TIFF_INCLUDE_DIRS ${TIFF_INSTALL_DIR}/include CACHE PATH "Where to find LibTIFF header files" FORCE)
SET(TIFF_LIBRARY_DIR  ${TIFF_INSTALL_DIR}/lib CACHE PATH "Where to find LibTIFF library files" FORCE)

SET(TIFF_LIBRARIES    ${TIFF_INSTALL_DIR}/lib/libtiff.a  CACHE FILEPATH "LibTIFF library" FORCE)

if (WIN32 AND ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    string(FIND ${CMAKE_GENERATOR} "Visual Studio" GENERATED_WITH_MSVC)
    if (GENERATED_WITH_MSVC GREATER -1)
        SET(TIFF_LIBRARIES
                optimized ${TIFF_INSTALL_DIR}/lib/tiff.lib
                debug     ${TIFF_INSTALL_DIR}/lib/tiffd.lib
                CACHE FILEPATH "LibTIFF library" FORCE
        )
    else (GENERATED_WITH_MSVC GREATER -1)
        SET(TIFF_LIBRARIES
                ${TIFF_INSTALL_DIR}/lib/tiff.lib
                CACHE FILEPATH "LibTIFF library" FORCE
        )
    endif (GENERATED_WITH_MSVC GREATER -1)
else ()
    IF (UNIX AND NOT APPLE)
        SET(TIFF_LIBRARIES    ${TIFF_INSTALL_DIR}/lib64/libtiff.a   CACHE FILEPATH "LibTIFF library" FORCE)
    ENDIF()
endif ()

SET (TIFF_LIBRARY ${TIFF_LIBRARIES} CACHE FILEPATH "LibTIFF library" FORCE)

SET(TIFF_FOUND ON)


# copy libraries into install tree
install(DIRECTORY ${TIFF_INSTALL_DIR}/lib/     DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/lib     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${TIFF_INSTALL_DIR}/lib64/   DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/lib64   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${TIFF_INSTALL_DIR}/bin/     DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/bin     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${TIFF_INSTALL_DIR}/share/   DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/share   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${TIFF_INSTALL_DIR}/man/     DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/man     COMPONENT Development FILES_MATCHING PATTERN "*.*")

endmacro(build_libtiff)
