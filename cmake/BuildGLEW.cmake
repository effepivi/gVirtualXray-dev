macro(build_glew install_prefix staging_prefix)

SET (GLEW_VERSION_STRING 1.13)
SET (GLEW_VERSION_MAJOR  1.13)
SET (GLEW_VERSION_MINOR  0)

set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
if(APPLE)
  list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
    -DMACOSX_RPATH:BOOL=ON
    -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
  )
endif()

SET (GLEW_INSTALL_DIR ${staging_prefix}/${install_prefix}-install)

ExternalProject_Add(getGLEW
  URL  "http://liquidtelecom.dl.sourceforge.net/project/glew/glew/1.13.0/glew-1.13.0.tgz"
  URL_MD5 "7cbada3166d2aadfc4169c4283701066"
  UPDATE_COMMAND ""
  SOURCE_DIR  ${staging_prefix}/${install_prefix}-source
  #BINARY_DIR  ${staging_prefix}/${install_prefix}-build
  #INSTALL_DIR ${GLEW_INSTALL_DIR}
  LIST_SEPARATOR :::  
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  CMAKE_GENERATOR ""
  INSTALL_COMMAND ""
)

ExternalProject_Add(glew
  DOWNLOAD_COMMAND ""
  UPDATE_COMMAND ""
  SOURCE_DIR  ${staging_prefix}/${install_prefix}-source/build/cmake
  #BINARY_DIR  ${staging_prefix}/${install_prefix}-build
  INSTALL_DIR ${GLEW_INSTALL_DIR}
  LIST_SEPARATOR :::  
  BUILD_IN_SOURCE 1
  CMAKE_GENERATOR ${CMAKE_GEN}
  CMAKE_ARGS
      -DBUILD_SHARED_LIBS:BOOL=OFF
      -DBUILD_UTILS:BOOL=OFF
      -DCMAKE_INSTALL_PREFIX:PATH=${GLEW_INSTALL_DIR}
      -DCMAKE_C_FLAGS:STRING=-fPIC ${CMAKE_C_FLAGS}
      -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
      -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}
)

ADD_DEPENDENCIES(glew getGLEW)

SET(GLEW_INCLUDE_DIR ${GLEW_INSTALL_DIR}/include  ${GLEW_INSTALL_DIR}/include)
SET(GLEW_DIR         ${GLEW_INSTALL_DIR})
SET(GLEW_FOUND ON)

IF (UNIX)
    SET(GLEW_LIBRARY     ${GLEW_INSTALL_DIR}/lib/libGLEW.a )
ENDIF (UNIX)

if (WIN32)
    if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	    SET(GLEW_LIBRARY
		optimized ${GLEW_INSTALL_DIR}/lib/libglew32.lib
		debug     ${GLEW_INSTALL_DIR}/lib/libglew32d.lib
        )
    ELSE ()
        SET(GLEW_LIBRARY
           ${GLEW_INSTALL_DIR}/lib/libglew32.a
        )
    ENDIF ()
endif (WIN32)


# copy libraries into install tree
install(DIRECTORY ${GLEW_INSTALL_DIR}/include/ DESTINATION ${INSTALL_DIR}/third_party/include COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GLEW_INSTALL_DIR}/lib/     DESTINATION ${INSTALL_DIR}/third_party/lib     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GLEW_INSTALL_DIR}/lib64/   DESTINATION ${INSTALL_DIR}/third_party/lib64   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GLEW_INSTALL_DIR}/bin/     DESTINATION ${INSTALL_DIR}/third_party/bin     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GLEW_INSTALL_DIR}/share/   DESTINATION ${INSTALL_DIR}/third_party/share   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GLEW_INSTALL_DIR}/man/     DESTINATION ${INSTALL_DIR}/third_party/man     COMPONENT Development FILES_MATCHING PATTERN "*.*")


endmacro(build_glew)

