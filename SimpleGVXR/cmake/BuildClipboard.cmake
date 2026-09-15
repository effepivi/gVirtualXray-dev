macro(build_clipboard install_prefix staging_prefix temp_path)


# make a custom Clipboard configuration file
SET (URL "https://github.com/dacap/clip/archive/refs/tags/v1.15.zip")
SET (MD5 "cd8e3886f16adc3985256ecfd7b98e1a")

SET (LIBCLIP_VERSION_STRING 1.15)
SET (LIBCLIP_VERSION_MAJOR  1.15)
SET (LIBCLIP_VERSION_MINOR  0)

set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
if(APPLE)
  list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
    -DMACOSX_RPATH:BOOL=ON
    -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
  )
endif()

SET (INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/third_party)
SET (INSTALL_LIBDIR lib)

SET(clip_INCLUDE_DIR  ${INSTALL_DIR}/include CACHE PATH "Where to find Clipboard header files" FORCE)
SET(clip_INCLUDE_DIRS ${INSTALL_DIR}/include CACHE PATH "Where to find Clipboard header files" FORCE)
SET(clip_LIBRARY_DIR  ${INSTALL_DIR}/${INSTALL_LIBDIR} CACHE PATH "Where to find Clipboard library files" FORCE)

SET(clip_LIBRARIES "${INSTALL_DIR}/${INSTALL_LIBDIR}/libclip.a")

if (WIN32 AND ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    string(FIND ${CMAKE_GENERATOR} "Visual Studio" GENERATED_WITH_MSVC)
    if (GENERATED_WITH_MSVC GREATER -1)
        SET(clip_LIBRARIES
                optimized ${INSTALL_DIR}/${INSTALL_LIBDIR}/clip.lib
                debug     ${INSTALL_DIR}/${INSTALL_LIBDIR}/clipd.lib
        )
    else (GENERATED_WITH_MSVC GREATER -1)
        SET(clip_LIBRARIES
                ${INSTALL_DIR}/${INSTALL_LIBDIR}/clip.lib
        )
    endif (GENERATED_WITH_MSVC GREATER -1)
else ()
    IF (UNIX AND NOT APPLE)
        SET(clip_LIBRARIES    ${INSTALL_DIR}/${INSTALL_LIBDIR}/libclip.a)
    ENDIF()
endif ()

SET (clip_LIBRARY ${clip_LIBRARIES})

SET(clip_LIBRARIES ${clip_LIBRARIES} CACHE FILEPATH "Clipboard library" FORCE)
SET(clip_LIBRARY ${clip_LIBRARY} CACHE FILEPATH "Clipboard library" FORCE)

SET(clip_FOUND ON)


IF (UNIX AND NOT APPLE)
    ExternalProject_Add(clipboard
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        #        URL  ${URL}
        #        URL_MD5 ${MD5}
        GIT_REPOSITORY git@github.com:effepivi/clip.git
        GIT_TAG main
        UPDATE_COMMAND ""
        SOURCE_DIR  ${temp_path}/${install_prefix}-source
        BINARY_DIR  ${temp_path}/${install_prefix}-build
        INSTALL_DIR ${INSTALL_DIR}
        LIST_SEPARATOR :::
        CMAKE_GENERATOR ${CMAKE_GEN}
        BUILD_BYPRODUCTS ${clip_LIBRARIES}
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE:STRING=Release
            -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
            -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
            -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
            -DCMAKE_EXE_LINKER_FLAGS:STRING=${LINKER_FLAGS}
            -DBUILD_SHARED_LIBS:BOOL=OFF
            -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}
            -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_DIR}
            -DCMAKE_INSTALL_LIBDIR=${INSTALL_LIBDIR}
            -DCLIP_ENABLE_IMAGE:BOOL=ON
            -DCLIP_X11_WITH_PNG:BOOL=ON
        #INSTALL_COMMAND ""
    )
ELSE ()
    ExternalProject_Add(clipboard
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
#        URL  ${URL}
#        URL_MD5 ${MD5}
        GIT_REPOSITORY git@github.com:effepivi/clip.git
        GIT_TAG main
        UPDATE_COMMAND ""
        SOURCE_DIR  ${temp_path}/${install_prefix}-source
        BINARY_DIR  ${temp_path}/${install_prefix}-build
        INSTALL_DIR ${INSTALL_DIR}
        LIST_SEPARATOR :::
        CMAKE_GENERATOR ${CMAKE_GEN}
        BUILD_BYPRODUCTS ${clip_LIBRARIES}
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
            -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_DIR}
            -DCMAKE_INSTALL_LIBDIR=${INSTALL_LIBDIR}
            -DCLIP_ENABLE_IMAGE:BOOL=ON
        #INSTALL_COMMAND ""
    )
ENDIF ()

# copy libraries into install tree
# install(DIRECTORY ${INSTALL_DIR}/lib/     DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/lib     COMPONENT Development FILES_MATCHING PATTERN "*.*")
# install(DIRECTORY ${INSTALL_DIR}/lib64/   DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/lib64   COMPONENT Development FILES_MATCHING PATTERN "*.*")
# install(DIRECTORY ${INSTALL_DIR}/bin/     DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/bin     COMPONENT Development FILES_MATCHING PATTERN "*.*")
# install(DIRECTORY ${INSTALL_DIR}/share/   DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/share   COMPONENT Development FILES_MATCHING PATTERN "*.*")
# install(DIRECTORY ${INSTALL_DIR}/man/     DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/man     COMPONENT Development FILES_MATCHING PATTERN "*.*")

endmacro(build_clipboard)
