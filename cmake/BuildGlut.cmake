macro(build_glut install_prefix staging_prefix)


# make a custom FreeGlut configuration file

SET (FREEGLUT_VERSION_STRING 3.0)
SET (FREEGLUT_VERSION_MAJOR  3.0)
SET (FREEGLUT_VERSION_MINOR  0)

set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
if(APPLE)
  list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
    -DMACOSX_RPATH:BOOL=ON
    -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
  )
endif()


ExternalProject_Add(FreeGlut
  URL  "https://sourceforge.net/projects/freeglut/files/freeglut/3.0.0/freeglut-3.0.0.tar.gz"
  URL_MD5 "90c3ca4dd9d51cf32276bc5344ec9754"
  UPDATE_COMMAND ""
  SOURCE_DIR  ${staging_prefix}/${install_prefix}-source
  BINARY_DIR  ${staging_prefix}/${install_prefix}-build
  INSTALL_DIR ${staging_prefix}/${install_prefix}-install
  LIST_SEPARATOR :::  
  CMAKE_GENERATOR ${CMAKE_GEN}
  CMAKE_ARGS
      -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
      -DBUILD_SHARED_LIBS:BOOL=OFF
      -DFREEGLUT_BUILD_DEMOS:BOOL=OFF
      -DFREEGLUT_BUILD_SHARED_LIBS:BOOL=OFF
      -DCMAKE_INSTALL_PREFIX:PATH=${staging_prefix}/${install_prefix}-install
      -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
      -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
      -DCMAKE_BUILD_TYPE=${configuration}
      -DCMAKE_CONFIGURATION_TYPES=${configuration}
)

SET(GLUT_INCLUDE_DIR  ${staging_prefix}/${install_prefix}-install/include)


IF (UNIX)
    IF (NOT APPLE)
        FIND_PACKAGE (X11 REQUIRED)

        SET (GLUT_PLATFORM_DEPENDENT_LIBS 
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
            Xxf86vm
            Xi
            #Xrender
            Xrandr
        )

    ENDIF (NOT APPLE)
ENDIF (UNIX)

SET (GLUT_LIBRARIES 
    ${staging_prefix}/${install_prefix}-install/lib64/libglut.a
    ${GLUT_PLATFORM_DEPENDENT_LIBS}
)

set (GLUT_LIBRARY      ${GLUT_LIBRARIES})
set (GLUT_INCLUDE_PATH ${GLUT_INCLUDE_DIR})
SET(GLUT_FOUND ON)


# copy libraries into install tree
install(DIRECTORY ${staging_prefix}/${install_prefix}-install/include/ DESTINATION ${INSTALL_DIR}/third_party/include COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${staging_prefix}/${install_prefix}-install/lib/     DESTINATION ${INSTALL_DIR}/third_party/lib     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${staging_prefix}/${install_prefix}-install/lib64/   DESTINATION ${INSTALL_DIR}/third_party/lib64   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${staging_prefix}/${install_prefix}-install/bin/     DESTINATION ${INSTALL_DIR}/third_party/bin     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${staging_prefix}/${install_prefix}-install/share/   DESTINATION ${INSTALL_DIR}/third_party/share   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${staging_prefix}/${install_prefix}-install/man/     DESTINATION ${INSTALL_DIR}/third_party/man     COMPONENT Development FILES_MATCHING PATTERN "*.*")


endmacro(build_glut)
