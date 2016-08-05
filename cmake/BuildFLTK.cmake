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
      -DOPTION_BUILD_EXAMPLES:BOOL=OFF
      -DCMAKE_INSTALL_PREFIX:PATH=${staging_prefix}/${install_prefix}-install
      -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
      -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
      -DCMAKE_BUILD_TYPE=${configuration}
      -DCMAKE_CONFIGURATION_TYPES=${configuration}
)

#if(UNIX)
#  include(FindX11)
#  find_library(FLTK_MATH_LIBRARY m)
#  set( FLTK_PLATFORM_DEPENDENT_LIBS ${X11_LIBRARIES} ${FLTK_MATH_LIBRARY})
#endif()

if(APPLE)
    FIND_LIBRARY(AGL_LIBRARY AGL)
    FIND_LIBRARY(CARBON_LIBRARY Carbon)
    FIND_LIBRARY(COCOA_LIBRARY Cocoa)
    SET (FLTK_PLATFORM_DEPENDENT_LIBS ${FLTK_PLATFORM_DEPENDENT_LIBS} ${AGL_LIBRARY} ${CARBON_LIBRARY} ${COCOA_LIBRARY})

#  set( FLTK_PLATFORM_DEPENDENT_LIBS  "-framework Carbon -framework Cocoa -framework ApplicationServices -lz")
endif()

SET (FLTK_FLUID_EXECUTABLE ${staging_prefix}/${install_prefix}-build/bin/fluid)
SET (FLTK_INCLUDE_DIR ${staging_prefix}/${install_prefix})
set (FLTK_INCLUDE_DIRS ${staging_prefix}/${install_prefix}-build ${FLTK_INCLUDE_DIR})
SET (FLTK_FOUND ON)

IF (UNIX)
    IF (NOT APPLE)
        FIND_PACKAGE (X11 REQUIRED)

        SET (FLTK_PLATFORM_DEPENDENT_LIBS 
            #pthread
            dl 
            #SM
            #ICE
            X11
            Xext
            Xinerama
            Xfixes
            Xcursor
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

# copy libraries into install tree
install(DIRECTORY ${staging_prefix}/${install_prefix}-install/include/ DESTINATION ${INSTALL_DIR}/third_party/include COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${staging_prefix}/${install_prefix}-install/lib/     DESTINATION ${INSTALL_DIR}/third_party/lib     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${staging_prefix}/${install_prefix}-install/lib64/   DESTINATION ${INSTALL_DIR}/third_party/lib64   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${staging_prefix}/${install_prefix}-install/bin/     DESTINATION ${INSTALL_DIR}/third_party/bin     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${staging_prefix}/${install_prefix}-install/share/   DESTINATION ${INSTALL_DIR}/third_party/share   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${staging_prefix}/${install_prefix}-install/man/     DESTINATION ${INSTALL_DIR}/third_party/man     COMPONENT Development FILES_MATCHING PATTERN "*.*")


endmacro(build_fltk)

