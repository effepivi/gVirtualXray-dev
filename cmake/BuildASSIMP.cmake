macro(build_assimp install_prefix staging_prefix)


# make a custom ASSIMP configuration file

SET (ASSIMP_VERSION_STRING 4.1)
SET (ASSIMP_VERSION_MAJOR  4)
SET (ASSIMP_VERSION_MINOR  1)

set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
if(APPLE)
  list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
    -DMACOSX_RPATH:BOOL=ON
    -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
  )
endif()

SET (ASSIMP_INSTALL_DIR ${staging_prefix}/${install_prefix}-install)

ExternalProject_Add(assimp
  URL  "https://github.com/assimp/assimp/archive/v4.1.0.zip"
  URL_MD5 "e98a03f9496ff761e67f29b645e04316"
  UPDATE_COMMAND ""
  SOURCE_DIR  ${staging_prefix}/${install_prefix}-source
  BINARY_DIR  ${staging_prefix}/${install_prefix}-build
  INSTALL_DIR ${ASSIMP_INSTALL_DIR}
  LIST_SEPARATOR :::  
  CMAKE_GENERATOR ${CMAKE_GEN}
  CMAKE_ARGS
      -DBUILD_SHARED_LIBS:BOOL=OFF
      -DCMAKE_INSTALL_PREFIX:PATH=${ASSIMP_INSTALL_DIR}
      -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}
      #-DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
      #-DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
      -DASSIMP_BUILD_DOCBOOK_MANPAGES:BOOL=OFF
)

SET(ASSIMP_ROOT_DIR      ${ASSIMP_INSTALL_DIR}          CACHE PATH "Where ASSIMP is."   FORCE)
SET(ASSIMP_INCLUDE_DIRS  ${ASSIMP_INSTALL_DIR}/include/ CACHE PATH "Where ASSIMP's headers are."   FORCE)
SET(ASSIMP_LIBRARY_DIRS  ${ASSIMP_INSTALL_DIR}/lib      CACHE PATH "Where ASSIMP's libraries are." FORCE)

if (WIN32 AND ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    SET(ASSIMP_LIBRARIES   
        ${ASSIMP_INSTALL_DIR}/lib/assimp-vc140-mt.lib
        ${ASSIMP_INSTALL_DIR}/lib/IrrXML.lib
        ${ASSIMP_INSTALL_DIR}/lib/zlibstatic.lib
		CACHE FILEPATH "ASSIMP's libraries." FORCE
    )
else ()
    SET(ASSIMP_LIBRARIES 
        ${ASSIMP_INSTALL_DIR}/lib/libassimp.a
        ${ASSIMP_INSTALL_DIR}/lib/libIrrXML.a
		CACHE FILEPATH "ASSIMP's libraries." FORCE
    )

    IF (NOT WIN32)
        SET(ASSIMP_LIBRARIES  
            ${ASSIMP_LIBRARIES} 
            ${ASSIMP_INSTALL_DIR}/lib/libassimp.a 
            ${ASSIMP_INSTALL_DIR}/lib/libIrrXML.a
			CACHE FILEPATH "ASSIMP's libraries." FORCE
        )
    ELSE (NOT WIN32)
        SET(ASSIMP_LIBRARIES  
            ${ASSIMP_LIBRARIES} 
            ${ASSIMP_INSTALL_DIR}/lib/libassimp.a
            ${ASSIMP_INSTALL_DIR}/lib/libIrrXML.a
			CACHE FILEPATH "ASSIMP's libraries." FORCE
        )
    ENDIF (NOT WIN32)

endif ()

SET (ASSIMP_LIBRARY ${ASSIMP_LIBRARIES} CACHE FILEPATH "ASSIMP's libraries." FORCE)

SET(assimp_FOUND ON)
SET(ASSIMP_FOUND ON)


# copy libraries into install tree
install(DIRECTORY ${ASSIMP_INSTALL_DIR}/include/ DESTINATION ${INSTALL_DIR}/third_party/include COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${ASSIMP_INSTALL_DIR}/lib/     DESTINATION ${INSTALL_DIR}/third_party/lib     COMPONENT Development FILES_MATCHING PATTERN "*.*")
#install(DIRECTORY ${ASSIMP_INSTALL_DIR}/lib64/   DESTINATION ${INSTALL_DIR}/third_party/lib64   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${ASSIMP_INSTALL_DIR}/bin/     DESTINATION ${INSTALL_DIR}/third_party/bin     COMPONENT Development FILES_MATCHING PATTERN "*.*")
#install(DIRECTORY ${ASSIMP_INSTALL_DIR}/share/   DESTINATION ${INSTALL_DIR}/third_party/share   COMPONENT Development FILES_MATCHING PATTERN "*.*")
#install(DIRECTORY ${ASSIMP_INSTALL_DIR}/man/     DESTINATION ${INSTALL_DIR}/third_party/man     COMPONENT Development FILES_MATCHING PATTERN "*.*")

endmacro(build_assimp)
