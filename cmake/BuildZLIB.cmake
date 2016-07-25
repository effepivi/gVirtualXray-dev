macro(build_zlib install_prefix staging_prefix)


# make a custom ZLIB configuration file

SET (ZLIB_VERSION_STRING 1.2)
SET (ZLIB_VERSION_MAJOR  1.2)
SET (ZLIB_VERSION_MINOR  8)

set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
if(APPLE)
  list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
    -DMACOSX_RPATH:BOOL=ON
    -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
  )
endif()


ExternalProject_Add(ZLIB
  URL  "http://zlib.net/zlib-1.2.8.tar.gz"
  URL_MD5 "44d667c142d7cda120332623eab69f40"
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

SET(ZLIB_INCLUDE_DIR ${staging_prefix}/${install_prefix}-install/include )
SET(ZLIB_DIR         ${staging_prefix}/${install_prefix}-install)
SET(ZLIB_FOUND ON)

IF (UNIX)
	SET(ZLIB_LIBRARY     ${staging_prefix}/${install_prefix}-install/lib/libz.a )
ELSE(UNIX)
	#if (${CMAKE_CFG_INTDIR} STREQUAL "$(Configuration)")
	#	SET(ZLIB_LIBRARY     ${staging_prefix}/${install_prefix}-install/lib/zlibstaticd.lib )
	#else ()
		SET(ZLIB_LIBRARY     ${staging_prefix}/${install_prefix}-install/lib/zlibstatic.lib )
	#endif ()
ENDIF(UNIX)

install(DIRECTORY ${staging_prefix}/${install_prefix}-install/include/ DESTINATION ${INSTALL_DIR}/third_party/include COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${staging_prefix}/${install_prefix}-install/lib/     DESTINATION ${INSTALL_DIR}/third_party/lib     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${staging_prefix}/${install_prefix}-install/lib64/   DESTINATION ${INSTALL_DIR}/third_party/lib64   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${staging_prefix}/${install_prefix}-install/bin/     DESTINATION ${INSTALL_DIR}/third_party/bin     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${staging_prefix}/${install_prefix}-install/share/   DESTINATION ${INSTALL_DIR}/third_party/share   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${staging_prefix}/${install_prefix}-install/man/     DESTINATION ${INSTALL_DIR}/third_party/man     COMPONENT Development FILES_MATCHING PATTERN "*.*")


endmacro(build_zlib)

