macro(build_freetype install_prefix staging_prefix)

SET (FREETYPE_VERSION_STRING 2.6)
SET (FREETYPE_VERSION_MAJOR  2.6)
SET (FREETYPE_VERSION_MINOR  4)

set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
if(APPLE)
  list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
    -DMACOSX_RPATH:BOOL=ON
    -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
  )
endif()


ExternalProject_Add(FREETYPE
  URL  "http://download.savannah.gnu.org/releases/freetype/freetype-2.6.4.tar.gz"
  URL_MD5 "f2b84651cb70c0a7d64a7f427b6675cd"
  UPDATE_COMMAND ""
  SOURCE_DIR  ${staging_prefix}/${install_prefix}-source
  BINARY_DIR  ${staging_prefix}/${install_prefix}-build
  INSTALL_DIR ${staging_prefix}/${install_prefix}-install
  LIST_SEPARATOR :::  
  CMAKE_GENERATOR ${CMAKE_GEN}
  CMAKE_ARGS
#      -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
      -DCMAKE_CFG_INTDIR=Release
      -DCMAKE_BUILD_TYPE:STRING=Release
	  -DBUILD_SHARED_LIBS:BOOL=OFF
      -DCMAKE_INSTALL_PREFIX:PATH=${staging_prefix}/${install_prefix}-install
      -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
      -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
)

SET(FREETYPE_INCLUDE_DIRS ${staging_prefix}/${install_prefix}-install/include  ${staging_prefix}/${install_prefix}-install/include/freetype2)
SET(FREETYPE_DIR         ${staging_prefix}/${install_prefix}-install)
SET(FREETYPE_FOUND ON)

IF (UNIX)
	SET(FREETYPE_LIBRARIES    ${PROJECT_BINARY_DIR}/${staging_prefix}/${install_prefix}-install/lib/libfreetype.a )
ELSE (UNIX)
	if (${CMAKE_CFG_INTDIR} STREQUAL "$(Configuration)")
		SET(FREETYPE_LIBRARIES    ${staging_prefix}/${install_prefix}-install/lib/freetyped.lib)
	else ()
		SET(FREETYPE_LIBRARIES    ${staging_prefix}/${install_prefix}-install/lib/freetype.lib)
	endif()
	
ENDIF (UNIX)

endmacro(build_freetype)

