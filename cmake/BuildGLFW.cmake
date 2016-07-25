# make a custom GLFW configuration file
macro(build_GLFW install_prefix staging_prefix)

if (NOT TARGET GLFW)
    SET (GLFW_VERSION_STRING 3.2)
    SET (GLFW_VERSION_MAJOR  3.2)
    SET (GLFW_VERSION_MINOR  0)

    set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
    if(APPLE)
        list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
            -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
            -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
            -DMACOSX_RPATH:BOOL=ON
            -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
        )
    endif()


    ExternalProject_Add(GLFW
        URL  "https://github.com/GLFW/GLFW/releases/download/3.2/GLFW-3.2.zip"
        URL_MD5 "d51fa09602abce26a65096ce97b4831c"
        UPDATE_COMMAND ""
        SOURCE_DIR  ${staging_prefix}/${install_prefix}-source
        BINARY_DIR  ${staging_prefix}/${install_prefix}-build
        INSTALL_DIR ${staging_prefix}/${install_prefix}-install
        LIST_SEPARATOR :::  
        CMAKE_GENERATOR ${CMAKE_GEN}
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
            -DBUILD_SHARED_LIBS:BOOL=OFF
            -DCMAKE_INSTALL_PREFIX:PATH=${staging_prefix}/${install_prefix}-install
            -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
            -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
            -DCMAKE_BUILD_TYPE=${configuration}
            -DCMAKE_CONFIGURATION_TYPES=${configuration}
    )

    SET(GLFW_INCLUDE_DIR ${staging_prefix}/${install_prefix}-install/include )
    SET(GLFW_DIR         ${staging_prefix}/${install_prefix}-install)
    SET(GLFW_FOUND ON)

    IF (UNIX)
        SET(GLFW_LIBRARY     ${staging_prefix}/${install_prefix}-install/lib/libglfw3.a )
    ELSE (UNIX)
        SET(GLFW_LIBRARY     ${staging_prefix}/${install_prefix}-install/lib/glfw3.lib )
    ENDIF (UNIX)

    install(DIRECTORY ${staging_prefix}/${install_prefix}-install/include/ DESTINATION ${INSTALL_DIR}/third_party/include COMPONENT Development FILES_MATCHING PATTERN "*.*")
    install(DIRECTORY ${staging_prefix}/${install_prefix}-install/lib/     DESTINATION ${INSTALL_DIR}/third_party/lib     COMPONENT Development FILES_MATCHING PATTERN "*.*")
    install(DIRECTORY ${staging_prefix}/${install_prefix}-install/lib64/   DESTINATION ${INSTALL_DIR}/third_party/lib64   COMPONENT Development FILES_MATCHING PATTERN "*.*")
    install(DIRECTORY ${staging_prefix}/${install_prefix}-install/bin/     DESTINATION ${INSTALL_DIR}/third_party/bin     COMPONENT Development FILES_MATCHING PATTERN "*.*")
    install(DIRECTORY ${staging_prefix}/${install_prefix}-install/share/   DESTINATION ${INSTALL_DIR}/third_party/share   COMPONENT Development FILES_MATCHING PATTERN "*.*")
    install(DIRECTORY ${staging_prefix}/${install_prefix}-install/man/     DESTINATION ${INSTALL_DIR}/third_party/man     COMPONENT Development FILES_MATCHING PATTERN "*.*")


endif(NOT TARGET GLFW)

endmacro(build_GLFW)

