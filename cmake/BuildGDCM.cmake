macro(build_gdcm install_prefix staging_prefix)


    # make a custom GDCM configuration file

SET (GDCM_VERSION_STRING 2.6)
SET (GDCM_VERSION_MAJOR  2.6)
SET (GDCM_VERSION_MINOR  5)

set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
if(APPLE)
  list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
    -DMACOSX_RPATH:BOOL=ON
    -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
  )
endif()

SET (GDCM_INSTALL_DIR ${staging_prefix}/${install_prefix}-install)

ExternalProject_Add(gdcm
  URL  "http://downloads.sourceforge.net/project/gdcm/gdcm%202.x/GDCM%202.6.5/gdcm-2.6.5.tar.gz"
  URL_MD5 "594e0a23cb5dce554874b330b3584a03"
  UPDATE_COMMAND ""
  SOURCE_DIR  ${staging_prefix}/${install_prefix}-source
  BINARY_DIR  ${staging_prefix}/${install_prefix}-build
  INSTALL_DIR ${GDCM_INSTALL_DIR}
  LIST_SEPARATOR :::  
  CMAKE_GENERATOR ${CMAKE_GEN}
  CMAKE_ARGS
      -DBUILD_SHARED_LIBS:BOOL=OFF
      -DCMAKE_INSTALL_PREFIX:PATH=${GDCM_INSTALL_DIR}
      -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}
      #-DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
      #-DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
)

SET(GDCM_INCLUDE_DIR  ${GDCM_INSTALL_DIR}/include/gdcm-${GDCM_VERSION_MAJOR})
SET(GDCM_INCLUDE_DIRS ${GDCM_INCLUDE_DIR})
SET(GDCM_LIBRARY_DIR  ${GDCM_INSTALL_DIR}/lib)

if (WIN32 AND ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    SET(GDCM_LIBRARIES    
        optimized ${GDCM_INSTALL_DIR}/lib/gdcm.lib
        debug     ${GDCM_INSTALL_DIR}/lib/gdcm.lib
    )
else ()
    SET(GDCM_LIBRARIES 
        ${GDCM_INSTALL_DIR}/lib/libgdcmCommon.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmDSED.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmIOD.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmMEXD.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmopenjpeg.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmzlib.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmcharls.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmDICT.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmexpat.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmMSFF.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmuuid.a
        ${GDCM_INSTALL_DIR}/lib/libsocketxx.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmjpeg12.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmjpeg8.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmjpeg16.a
        dl
        )
endif ()

SET (GDCM_LIBRARY ${GDCM_LIBRARIES})

SET(GDCM_FOUND ON)


# copy libraries into install tree
install(DIRECTORY ${GDCM_INSTALL_DIR}/include/ DESTINATION ${INSTALL_DIR}/third_party/include COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GDCM_INSTALL_DIR}/lib/     DESTINATION ${INSTALL_DIR}/third_party/lib     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GDCM_INSTALL_DIR}/lib64/   DESTINATION ${INSTALL_DIR}/third_party/lib64   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GDCM_INSTALL_DIR}/bin/     DESTINATION ${INSTALL_DIR}/third_party/bin     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GDCM_INSTALL_DIR}/share/   DESTINATION ${INSTALL_DIR}/third_party/share   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GDCM_INSTALL_DIR}/man/     DESTINATION ${INSTALL_DIR}/third_party/man     COMPONENT Development FILES_MATCHING PATTERN "*.*")

endmacro(build_gdcm)
