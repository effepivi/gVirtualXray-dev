IF (WIN32)
    MESSAGE (STATUS "You're running CMake on Windows.")
ENDIF ()

MESSAGE (STATUS "You are running the install script from ${CMAKE_CURRENT_LIST_DIR}")

SET (DESTINATION ${CMAKE_CURRENT_LIST_DIR}/install)
MESSAGE (STATUS "The builds will be installed in ${DESTINATION}")

OPTION (UNIT_TESTS    "Build and run the unit tests"    ON)
OPTION (BUILD_CSHARP  "Build C# wrapper? (Y/N)"         OFF)
OPTION (BUILD_JAVA    "Build Java wrapper? (Y/N)"       OFF)
OPTION (BUILD_OCTAVE  "Build GNU Octave wrapper? (Y/N)" OFF)
OPTION (BUILD_PERL    "Build Perl wrapper? (Y/N)"       OFF)
OPTION (BUILD_PYTHON3 "Build Python 3 wrapper? (Y/N)"   OFF)
OPTION (BUILD_PYTHON2 "Build Python 2 wrapper? (Y/N)"   OFF)
OPTION (BUILD_R       "Build R wrapper? (Y/N)"          OFF)
OPTION (BUILD_RUBY    "Build Ruby wrapper? (Y/N)"       OFF)
OPTION (BUILD_TCL     "Build Tcl wrapper? (Y/N)"        OFF)

SET (BUILD_WRAPPER OFF)

IF (BUILD_CSHARP)
    SET (BUILD_WRAPPER ON)
ENDIF ()

IF (BUILD_JAVA)
    SET (BUILD_WRAPPER ON)
ENDIF ()

IF (BUILD_OCTAVE)
    SET (BUILD_WRAPPER ON)
ENDIF ()

IF (BUILD_PERL)
    SET (BUILD_WRAPPER ON)
ENDIF ()

IF (BUILD_PYTHON3)
    SET (BUILD_WRAPPER ON)
    SET (BUILD_PYTHON2 OFF)
ENDIF ()

IF (BUILD_PYTHON2)
    SET (BUILD_WRAPPER ON)
ENDIF ()

IF (BUILD_R)
    SET (BUILD_WRAPPER ON)
ENDIF ()

IF (BUILD_RUBY)
    SET (BUILD_WRAPPER ON)
ENDIF ()

IF (BUILD_TCL)
    SET (BUILD_WRAPPER ON)
ENDIF ()

IF (BUILD_WRAPPER)
    OPTION (WRAPPER_TEST    "Build and run the wrapper tests"    ON)
ENDIF ()

MESSAGE ("***************************")
MESSAGE ("* Build wrappers (summary):")
MESSAGE ("***************************")
MESSAGE ("Build C# wrapper: ${BUILD_CSHARP}")
MESSAGE ("Build Java wrapper: ${BUILD_JAVA}")
MESSAGE ("Build GNU Octave wrapper: ${BUILD_OCTAVE}")
MESSAGE ("Build Perl wrapper: ${BUILD_PERL}")
MESSAGE ("Build Python 2 wrapper: ${BUILD_PYTHON2}")
MESSAGE ("Build Python 3 wrapper: ${BUILD_PYTHON3}")
MESSAGE ("Build R wrapper: ${BUILD_R}")
MESSAGE ("Build Ruby wrapper: ${BUILD_RUBY}")
MESSAGE ("Build Tcl wrapper: ${BUILD_TCL}")

MESSAGE ("")

MESSAGE ("Build and run the unit tests: ${UNIT_TESTS}")

IF (BUILD_WRAPPER)
    MESSAGE ("Build and run the wrapper tests: ${WRAPPER_TEST}")
ENDIF ()


# Build and install the core library
#cd gvxr-build

MESSAGE ("*************************************")
MESSAGE ("* Configure, build and install gvxr *")
MESSAGE ("* (run the unit test, optional)     *")
MESSAGE ("*************************************")
MESSAGE ("")
MESSAGE ("Configure gvxr using CMake")
MESSAGE ("Please wait")
MESSAGE ("")

# The first external project will be built at *configure stage*
file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/gvxr-build)

execute_process(
    COMMAND ${CMAKE_COMMAND} ${CMAKE_CURRENT_LIST_DIR}/gvxr
    WORKING_DIRECTORY  ${CMAKE_CURRENT_LIST_DIR}/gvxr-build
    RESULT_VARIABLE gvxr_config_result
    )

message(STATUS "gVirtualXRay config result: ${gvxr_config_result}")


IF (MSVC)
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/gvxr --target gVirtualXRay
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/gvxr
        RESULT_VARIABLE gvxr_build_result
        )
ELSE (MSVC)
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/gvxr --target gVirtualXRay -- -j12
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/gvxr
        RESULT_VARIABLE gvxr_build_result
        )
ENDIF (MSVC)
