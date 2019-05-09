macro(build_glew install_prefix staging_prefix)

SET (GLEW_VERSION_STRING 2.1)
SET (GLEW_VERSION_MAJOR  2.1)
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
  URL  "https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0.zip/download"
  URL_MD5 "dff2939fd404d054c1036cc0409d19f1"
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
      -DBUILD_UTILS:BOOL=OFF
      -DCMAKE_INSTALL_PREFIX:PATH=${GLEW_INSTALL_DIR}
)

ADD_DEPENDENCIES(glew getGLEW)

SET(GLEW_INCLUDE_DIR ${GLEW_INSTALL_DIR}/include  ${GLEW_INSTALL_DIR}/include)
SET(GLEW_DIR         ${GLEW_INSTALL_DIR})
SET(GLEW_FOUND ON)

IF (UNIX)
    SET(GLEW_LIBRARY     ${GLEW_INSTALL_DIR}/lib64/libGLEW.a )
ENDIF (UNIX)

IF (APPLE)
    SET(GLEW_LIBRARY     ${GLEW_INSTALL_DIR}/lib/libGLEW.a )
ENDIF (APPLE)

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
