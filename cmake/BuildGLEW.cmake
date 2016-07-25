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


ExternalProject_Add(getGLEW
  URL  "http://liquidtelecom.dl.sourceforge.net/project/glew/glew/1.13.0/glew-1.13.0.tgz"
  URL_MD5 "7cbada3166d2aadfc4169c4283701066"
  UPDATE_COMMAND ""
  SOURCE_DIR  ${staging_prefix}/${install_prefix}-source
  BINARY_DIR  ${staging_prefix}/${install_prefix}-build
  INSTALL_DIR ${staging_prefix}/${install_prefix}-install
  LIST_SEPARATOR :::  
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  CMAKE_GENERATOR ""
  INSTALL_COMMAND ""
)

ExternalProject_Add(buildGLEW
  DOWNLOAD_COMMAND ""
  UPDATE_COMMAND ""
  SOURCE_DIR  ${staging_prefix}/${install_prefix}-source/build/cmake
  BINARY_DIR  ${staging_prefix}/${install_prefix}-build
  INSTALL_DIR ${staging_prefix}/${install_prefix}-install
  LIST_SEPARATOR :::  
  CMAKE_GENERATOR ${CMAKE_GEN}
  CMAKE_ARGS
#      -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DBUILD_SHARED_LIBS:BOOL=OFF
      -DBUILD_UTILS:BOOL=OFF
      -DCMAKE_INSTALL_PREFIX:PATH=${staging_prefix}/${install_prefix}-install
      -DCMAKE_C_FLAGS:STRING=-fPIC ${CMAKE_C_FLAGS}
      -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
)

ADD_DEPENDENCIES(buildGLEW getGLEW)

SET(GLEW_INCLUDE_DIR ${staging_prefix}/${install_prefix}-install/include  ${staging_prefix}/${install_prefix}-install/include)
SET(GLEW_DIR         ${staging_prefix}/${install_prefix}-install)
SET(GLEW_FOUND ON)

IF (UNIX)
	SET(GLEW_LIBRARY     ${staging_prefix}/${install_prefix}-install/lib/libGLEW.a )
ELSE(UNIX)
	#if (${CMAKE_CFG_INTDIR} STREQUAL "$(Configuration)")
	#	SET(GLEW_LIBRARY     ${staging_prefix}/${install_prefix}-install/lib/libglew32d.lib )
	#else()
		SET(GLEW_LIBRARY     ${staging_prefix}/${install_prefix}-install/lib/libglew32.lib )
	#endif()
ENDIF(UNIX)

# copy libraries into install tree
install(DIRECTORY ${staging_prefix}/${install_prefix}-install/include/ DESTINATION ${INSTALL_DIR}/third_party/include COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${staging_prefix}/${install_prefix}-install/lib/     DESTINATION ${INSTALL_DIR}/third_party/lib     COMPONENT Development FILES_MATCHING PATTERN "*.*")


endmacro(build_glew)

