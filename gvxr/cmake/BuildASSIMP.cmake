macro(build_assimp install_prefix staging_prefix temp_path)


# make a custom ASSIMP configuration file
SET (URL "https://github.com/assimp/assimp/archive/refs/tags/v5.4.0.zip")
SET (MD5 "321a963e1405ea38d98fdc18a15874ac")

SET (ASSIMP_VERSION_STRING 5.4)
SET (ASSIMP_VERSION_MAJOR  4)
SET (ASSIMP_VERSION_MINOR  0)

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
SET (ASSIMP_INSTALL_LIBDIR lib)

IF (UNIX AND NOT APPLE)
    ExternalProject_Add(assimp
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        URL     ${URL}
        URL_MD5 ${MD5}
        UPDATE_COMMAND ""
        SOURCE_DIR  ${temp_path}/${install_prefix}-source
        BINARY_DIR  ${temp_path}/${install_prefix}-build
        INSTALL_DIR ${ASSIMP_INSTALL_DIR}
        LIST_SEPARATOR :::
        CMAKE_GENERATOR ${CMAKE_GEN}
        CMAKE_ARGS
            ${LOCAL_CMAKE_BUILD_OPTIONS}
            -DASSIMP_WARNINGS_AS_ERRORS:BOOL=OFF
            -DCMAKE_BUILD_TYPE:STRING=Release
            -DCMAKE_INSTALL_PREFIX:PATH=${ASSIMP_INSTALL_DIR}
            -DASSIMP_BUILD_TESTS:BOOL=OFF
            -DBUILD_SHARED_LIBS:BOOL=OFF
            -DBUILD_TESTING:BOOL=OFF
            -DASSIMP_BUILD_ASSIMP_TOOLS:BOOL=OFF
            -DASSIMP_BUILD_DOCBOOK_MANPAGES:BOOL=OFF
            -DCMAKE_INSTALL_LIBDIR=${ASSIMP_INSTALL_LIBDIR}
            -DZLIB_INCLUDE_DIR:STRING=${ZLIB_INCLUDE_DIR}
            -DZLIB_DIR:STRING=${ZLIB_DIR}
            -DZLIB_LIBRARY:STRING=${ZLIB_LIBRARY}
            -DZLIB_LIBRARY_DEBUG:STRING=${ZLIB_LIBRARY_DEBUG}
            -DZLIB_LIBRARY_RELEASE:STRING=${ZLIB_LIBRARY_RELEASE}
            -DZLIB_LIBRARY_DBG:STRING=${ZLIB_LIBRARY_DEBUG}
            -DZLIB_LIBRARY_REL:STRING=${ZLIB_LIBRARY_RELEASE}
        #INSTALL_COMMAND ""
    )
ELSE (UNIX AND NOT APPLE)
    ExternalProject_Add(assimp
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        URL     ${URL}
        URL_MD5 ${MD5}
        UPDATE_COMMAND ""
        SOURCE_DIR  ${temp_path}/${install_prefix}-source
        BINARY_DIR  ${temp_path}/${install_prefix}-build
        INSTALL_DIR ${ASSIMP_INSTALL_DIR}
        LIST_SEPARATOR :::
        CMAKE_GENERATOR ${CMAKE_GEN}
        CMAKE_ARGS
            ${LOCAL_CMAKE_BUILD_OPTIONS}
            -DCMAKE_BUILD_TYPE:STRING=Release
            -DCMAKE_INSTALL_PREFIX:PATH=${ASSIMP_INSTALL_DIR}
            -DASSIMP_BUILD_TESTS:BOOL=OFF
            -DBUILD_SHARED_LIBS:BOOL=OFF
            -DBUILD_TESTING:BOOL=OFF
            -DASSIMP_BUILD_ASSIMP_TOOLS:BOOL=OFF
            -DASSIMP_BUILD_DOCBOOK_MANPAGES:BOOL=OFF
            -DCMAKE_INSTALL_LIBDIR=${ASSIMP_INSTALL_LIBDIR}
            -DZLIB_INCLUDE_DIR:STRING=${ZLIB_INCLUDE_DIR}
            -DZLIB_DIR:STRING=${ZLIB_DIR}
            -DZLIB_LIBRARY:STRING=${ZLIB_LIBRARY}
            -DZLIB_LIBRARY_DEBUG:STRING=${ZLIB_LIBRARY_DEBUG}
            -DZLIB_LIBRARY_RELEASE:STRING=${ZLIB_LIBRARY_RELEASE}
            -DZLIB_LIBRARY_DBG:STRING=${ZLIB_LIBRARY_DEBUG}
            -DZLIB_LIBRARY_REL:STRING=${ZLIB_LIBRARY_RELEASE}
        #INSTALL_COMMAND ""
    )
ENDIF (UNIX AND NOT APPLE)

SET(ASSIMP_ROOT_DIR      ${ASSIMP_INSTALL_DIR}          CACHE PATH "Where ASSIMP is."   FORCE)
SET(ASSIMP_INCLUDE_DIRS  ${ASSIMP_INSTALL_DIR}/include/ CACHE PATH "Where ASSIMP's headers are."   FORCE)
SET(ASSIMP_LIBRARY_DIRS  ${ASSIMP_INSTALL_DIR}/${ASSIMP_INSTALL_LIBDIR}      CACHE PATH "Where ASSIMP's libraries are." FORCE)


IF (MSVC12)
    SET(ASSIMP_MSVC_VERSION "vc120")
ELSEIF (MSVC14)
    SET(ASSIMP_MSVC_VERSION "vc140")
ENDIF (MSVC12)

if (WIN32 AND ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	  FILE (WRITE ${staging_prefix}/${install_prefix}-build/code/Debug/assimp-${ASSIMP_MSVC_VERSION}-mt.pdb "Create a file to pass the install process")

    SET(ASSIMP_LIBRARIES

        optimized
            ${ASSIMP_INSTALL_DIR}/${ASSIMP_INSTALL_LIBDIR}/assimp-${ASSIMP_MSVC_VERSION}-mt.lib
        debug
            ${ASSIMP_INSTALL_DIR}/${ASSIMP_INSTALL_LIBDIR}/assimp-${ASSIMP_MSVC_VERSION}-mt.lib
            
        optimized
            ${ASSIMP_INSTALL_DIR}/${ASSIMP_INSTALL_LIBDIR}/IrrXML.lib
        debug
            ${ASSIMP_INSTALL_DIR}/${ASSIMP_INSTALL_LIBDIR}/IrrXML.lib

        #optimized
        #    ${ASSIMP_INSTALL_DIR}/${ASSIMP_INSTALL_LIBDIR}/zlibstatic.lib
        #debug
        #    ${ASSIMP_INSTALL_DIR}/${ASSIMP_INSTALL_LIBDIR}/zlibstaticd.lib

        CACHE FILEPATH "ASSIMP's libraries." FORCE
    )

    #install(FILES ${staging_prefix}/${install_prefix}-build/code/Debug/assimp-${ASSIMP_MSVC_VERSION}-mt.lib
    #        CONFIGURATIONS Debug
    #        RUNTIME DESTINATION ${INSTALL_DIR}/third_party/${ASSIMP_INSTALL_LIBDIR}/Debug)

    #install(FILES ${staging_prefix}/${install_prefix}-build/code/Release/assimp-${ASSIMP_MSVC_VERSION}-mt.lib
    #        CONFIGURATIONS Release
    #        RUNTIME DESTINATION ${INSTALL_DIR}/third_party/${ASSIMP_INSTALL_LIBDIR}/Release)
else ()
    SET(ASSIMP_LIBRARIES
        ${ASSIMP_INSTALL_DIR}/${ASSIMP_INSTALL_LIBDIR}/libassimp.a
        #${ASSIMP_INSTALL_DIR}/${ASSIMP_INSTALL_LIBDIR}/libIrrXML.a
		CACHE FILEPATH "ASSIMP's libraries." FORCE
    )

    IF (NOT WIN32)
        SET(ASSIMP_LIBRARIES
            ${ASSIMP_LIBRARIES}
            ${ASSIMP_INSTALL_DIR}/${ASSIMP_INSTALL_LIBDIR}/libassimp.a
            #${ASSIMP_INSTALL_DIR}/${ASSIMP_INSTALL_LIBDIR}/libIrrXML.a
			CACHE FILEPATH "ASSIMP's libraries." FORCE
        )
    ELSE (NOT WIN32)
        SET(ASSIMP_LIBRARIES
            ${ASSIMP_LIBRARIES}
            ${ASSIMP_INSTALL_DIR}/${ASSIMP_INSTALL_LIBDIR}/libassimp.a
            #${ASSIMP_INSTALL_DIR}/${ASSIMP_INSTALL_LIBDIR}/libIrrXML.a
			CACHE FILEPATH "ASSIMP's libraries." FORCE
        )
    ENDIF (NOT WIN32)

endif ()

SET (ASSIMP_LIBRARY ${ASSIMP_LIBRARIES} CACHE FILEPATH "ASSIMP's libraries." FORCE)

SET(assimp_FOUND ON)
SET(ASSIMP_FOUND ON)


# copy libraries into install tree
install(DIRECTORY ${ASSIMP_INSTALL_DIR}/include/ DESTINATION ${ASSIMP_INSTALL_DIR}/third_party/include COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${ASSIMP_INSTALL_DIR}/${ASSIMP_INSTALL_LIBDIR}     DESTINATION ${ASSIMP_INSTALL_DIR}/third_party/${ASSIMP_INSTALL_LIBDIR}     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${ASSIMP_INSTALL_DIR}/bin/     DESTINATION ${ASSIMP_INSTALL_DIR}/third_party/bin     COMPONENT Development FILES_MATCHING PATTERN "*.*")

install(DIRECTORY ${ASSIMP_INSTALL_DIR}/${ASSIMP_INSTALL_LIBDIR}     DESTINATION ${ASSIMP_INSTALL_DIR}/third_party/${ASSIMP_INSTALL_LIBDIR}     COMPONENT Runtime FILES_MATCHING PATTERN "*.so*")
install(DIRECTORY ${ASSIMP_INSTALL_DIR}/${ASSIMP_INSTALL_LIBDIR}/   DESTINATION ${ASSIMP_INSTALL_DIR}/third_party/${ASSIMP_INSTALL_LIBDIR}     COMPONENT Runtime FILES_MATCHING PATTERN "*.so*")

endmacro(build_assimp)
