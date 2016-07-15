macro(build_fltk install_prefix staging_prefix)


# make a custom fltk configuration file

SET (fltk_VERSION_STRING 1.2)
SET (fltk_VERSION_MAJOR  1.2)
SET (fltk_VERSION_MINOR  8)

set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
if(APPLE)
  list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
    -DMACOSX_RPATH:BOOL=ON
    -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
  )
endif()


ExternalProject_Add(fltk
  URL  "http://fltk.org/pub/fltk/1.3.3/fltk-1.3.3-source.tar.gz"
  URL_MD5 "9ccdb0d19dc104b87179bd9fd10822e3"
  UPDATE_COMMAND ""
  SOURCE_DIR fltk
  BINARY_DIR fltk-build
  LIST_SEPARATOR :::  
  CMAKE_GENERATOR ${CMAKE_GEN}
  CMAKE_ARGS
      -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
      -DBUILD_SHARED_LIBS:BOOL=OFF
      -DCMAKE_SKIP_RPATH:BOOL=ON
      -DOPTION_BUILD_EXAMPLES:BOOL=OFF
      -DCMAKE_INSTALL_PREFIX:PATH=${install_prefix}
      -DCMAKE_C_FLAGS:STRING=-fPIC ${CMAKE_C_FLAGS}
      -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
      -DCMAKE_SKIP_RPATH:BOOL=OFF
      -DCMAKE_SKIP_INSTALL_RPATH:BOOL=OFF
      -DCMAKE_INSTALL_RPATH:PATH=${install_prefix}/lib${LIB_SUFFIX}
  INSTALL_COMMAND $(MAKE) install DESTDIR=${staging_prefix} 
  INSTALL_DIR ${staging_prefix}/${install_prefix}
)

#if(UNIX)
#  include(FindX11)
#  find_library(FLTK_MATH_LIBRARY m)
#  set( FLTK_PLATFORM_DEPENDENT_LIBS ${X11_LIBRARIES} ${FLTK_MATH_LIBRARY})
#endif()

#if(APPLE)
#  set( FLTK_PLATFORM_DEPENDENT_LIBS  "-framework Carbon -framework Cocoa -framework ApplicationServices -lz")
#endif()

SET (FLTK_FLUID_EXECUTABLE ${staging_prefix}/${install_prefix}-build/bin/fluid)
SET (FLTK_INCLUDE_DIR ${staging_prefix}/${install_prefix})
set(FLTK_INCLUDE_DIRS ${staging_prefix}/${install_prefix}-build ${FLTK_INCLUDE_DIR})

IF (UNIX)
    IF (NOT APPLE)
        FIND_PACKAGE (X11 REQUIRED)

        SET (FLTK_PLATFORM_DEPENDENT_LIBS 
            #pthread
            #dl 
            #SM
            #ICE
            #X11
            Xext
            #Xinerama
            Xfixes
            #Xcursor
            Xft
            fontconfig
            #m
        )

    ENDIF (NOT APPLE)
ENDIF (UNIX)

SET (FLTK_LIBRARIES 
    ${staging_prefix}/${install_prefix}-build/lib/libfltk.a
    ${staging_prefix}/${install_prefix}-build/lib/libfltk_forms.a
    ${staging_prefix}/${install_prefix}-build/lib/libfltk_gl.a
    ${staging_prefix}/${install_prefix}-build/lib/libfltk_images.a
    ${FLTK_PLATFORM_DEPENDENT_LIBS}
)

SET (FLTK_DIR ${staging_prefix}/${install_prefix})

endmacro(build_fltk)

