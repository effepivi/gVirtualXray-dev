macro(build_libtiff install_prefix staging_prefix)


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


ExternalProject_Add(LibTIFF
  URL  "http://download.osgeo.org/libtiff/tiff-4.0.6.zip"
  URL_MD5 "f5b485d750b2001255ed64224b98b857"
  UPDATE_COMMAND ""
  SOURCE_DIR  ${staging_prefix}/${install_prefix}-source
  BINARY_DIR  ${staging_prefix}/${install_prefix}-build
  INSTALL_DIR ${staging_prefix}/${install_prefix}-install
  LIST_SEPARATOR :::  
  CMAKE_GENERATOR ${CMAKE_GEN}
  CMAKE_ARGS
      -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
      -DBUILD_SHARED_LIBS:BOOL=OFF
      -DCMAKE_INSTALL_PREFIX:PATH=${staging_prefix}/${install_prefix}-install
      -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
      -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
      -DCMAKE_BUILD_TYPE=${configuration}
      -DCMAKE_CONFIGURATION_TYPES=${configuration}
)

SET(TIFF_INCLUDE_DIR  ${staging_prefix}/${install_prefix}-install/include)
SET(TIFF_INCLUDE_DIRS ${staging_prefix}/${install_prefix}-install/include)
SET(TIFF_LIBRARY_DIR  ${staging_prefix}/${install_prefix}-install/lib)
SET(TIFF_LIBRARIES    tiff)
SET(TIFF_FOUND ON)

LINk_DIRECTORIES(${TIFF_LIBRARY_DIR})
INCLUDE_DIRECTORIES(${TIFF_INCLUDE_DIRS})

# copy libraries into install tree
install(DIRECTORY ${staging_prefix}/${install_prefix}-install/include/ DESTINATION ${INSTALL_DIR}/third_party/include COMPONENT Development)
install(DIRECTORY ${staging_prefix}/${install_prefix}-install/lib/     DESTINATION ${INSTALL_DIR}/third_party/lib     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${staging_prefix}/${install_prefix}-install/lib64/   DESTINATION ${INSTALL_DIR}/third_party/lib64   COMPONENT Development)
install(DIRECTORY ${staging_prefix}/${install_prefix}-install/bin/     DESTINATION ${INSTALL_DIR}/third_party/bin     COMPONENT Development)

endmacro(build_libtiff)
