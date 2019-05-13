MESSAGE (WARNING "When USE_SYSTEM_GLFW is ON, picking wrong PATH")

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
MESSAGE ("")


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
file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/SimpleGVXR-build)

IF (BUILD_WRAPPER)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Wrappers-build)
ENDIF()

MESSAGE ("Configure gvxr-core")
IF (WIN32)
    execute_process(
        COMMAND ${CMAKE_COMMAND}
            -DCMAKE_CXX_FLAGS=/DWIN32\ /D_WINDOWS\ /W3\ /GR\ /EHsc\ /MP
            -DCMAKE_INSTALL_PREFIX=${DESTINATION}
            -DBUILD_TESTING=${UNIT_TESTS}
            -DUSE_SYSTEM_XCOM:BOOL=${USE_SYSTEM_XCOM}
            -DUSE_SYSTEM_ASSIMP:BOOL=${USE_SYSTEM_ASSIMP}
            -DUSE_SYSTEM_GLEW:BOOL=${USE_SYSTEM_GLEW}
            -DUSE_SYSTEM_GLFW:BOOL=${USE_SYSTEM_GLFW}
            -DXCOM_PATH:STRING=/Users/eese10/Documents/gvirtualxray-code/XCOM

            ${CMAKE_CURRENT_LIST_DIR}/gvxr

            WORKING_DIRECTORY  ${CMAKE_CURRENT_LIST_DIR}/gvxr-build
            OUTPUT_FILE ../gvxr_cmake.out.log
            ERROR_FILE ../gvxr_cmake.err.log
            RESULT_VARIABLE gvxr_cmake_result
            OUTPUT_VARIABLE gvxr_cmake_output
            ERROR_VARIABLE  gvxr_cmake_error
      )
ELSE (WIN32)
    execute_process(
        COMMAND ${CMAKE_COMMAND}
            -DCMAKE_INSTALL_PREFIX=${DESTINATION}
            -DBUILD_TESTING=${UNIT_TESTS}
            -DUSE_SYSTEM_XCOM:BOOL=${USE_SYSTEM_XCOM}
            -DUSE_SYSTEM_ASSIMP:BOOL=${USE_SYSTEM_ASSIMP}
            -DUSE_SYSTEM_GLEW:BOOL=${USE_SYSTEM_GLEW}
            -DUSE_SYSTEM_GLFW:BOOL=${USE_SYSTEM_GLFW}
            -DXCOM_PATH:STRING=/Users/eese10/Documents/gvirtualxray-code/XCOM

            ${CMAKE_CURRENT_LIST_DIR}/gvxr

            WORKING_DIRECTORY  ${CMAKE_CURRENT_LIST_DIR}/gvxr-build
            OUTPUT_FILE ../gvxr_cmake.out.log
            ERROR_FILE ../gvxr_cmake.err.log
            RESULT_VARIABLE gvxr_cmake_result
            OUTPUT_VARIABLE gvxr_cmake_output
            ERROR_VARIABLE  gvxr_cmake_error
    )
ENDIF (WIN32)
MESSAGE ("Configure gvxr-core: DONE")
MESSAGE ("")

IF (NOT ${gvxr_cmake_result} EQUAL 0 OR
    NOT ${gvxr_cmake_output} STREQUAL  "" OR
    NOT ${gvxr_cmake_error} STREQUAL  "")

    MESSAGE (FATAL_ERROR "Error configuring the project, see the log files (gvxr_cmake.out.log and gvxr_cmake.err.log)")
ENDIF ()


MESSAGE ("Build gvxr-core")
IF (WIN32)
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/gvxr-build --config Debug
        OUTPUT_FILE gvxr_make.out.log
        ERROR_FILE gvxr_make.err.log
        RESULT_VARIABLE gvxr_make_result
        OUTPUT_VARIABLE gvxr_make_output
        ERROR_VARIABLE  gvxr_make_error
    )
ELSE (WIN32)
    MESSAGE ("Run parallel build, but not on MSVC")
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/gvxr-build --config Debug -- -j12
        OUTPUT_FILE gvxr_make.out.log
        ERROR_FILE gvxr_make.err.log
        RESULT_VARIABLE gvxr_make_result
        OUTPUT_VARIABLE gvxr_make_output
        ERROR_VARIABLE  gvxr_make_error
    )
ENDIF (WIN32)
MESSAGE ("Build gvxr-core: DONE")
MESSAGE ("")

IF (NOT ${gvxr_make_result} EQUAL 0 OR
    NOT ${gvxr_make_output} STREQUAL  "" OR
    NOT ${gvxr_make_error} STREQUAL  "")

    MESSAGE (FATAL_ERROR "Error building the project, see the log files (gvxr_make.out.log and gvxr_make.err.log)")
ENDIF ()

MESSAGE ("Install gvxr-core")
IF (WIN32)
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/gvxr-build  --config Debug --target INSTALL
        OUTPUT_FILE gvxr_install.out.log
        ERROR_FILE gvxr_install.err.log
        RESULT_VARIABLE gvxr_install_result
        OUTPUT_VARIABLE gvxr_install_output
        ERROR_VARIABLE  gvxr_install_error
    )
ELSE (WIN32)
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/gvxr-build  --config Debug --target install
        OUTPUT_FILE gvxr_install.out.log
        ERROR_FILE gvxr_install.err.log
        RESULT_VARIABLE gvxr_install_result
        OUTPUT_VARIABLE gvxr_install_output
        ERROR_VARIABLE  gvxr_install_error
    )
ENDIF (WIN32)
MESSAGE ("Install gvxr-core: DONE")
MESSAGE ("")

IF (NOT ${gvxr_install_result} EQUAL 0 OR
    NOT ${gvxr_install_output} STREQUAL  "" OR
    NOT ${gvxr_install_error} STREQUAL  "")

    MESSAGE (FATAL_ERROR "Error installing the project, see the log files (gvxr_install.out.log and gvxr_install.err.log)")
ENDIF ()

IF (UNIT_TESTS)
    MESSAGE ("Unit testing gvxr-core")
    IF (WIN32)
        execute_process(
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/gvxr-build  --config Debug --target RUN_TESTS
            OUTPUT_FILE gvxr_test.out.log
            ERROR_FILE gvxr_test.err.log
            RESULT_VARIABLE gvxr_test_result
            OUTPUT_VARIABLE gvxr_test_output
            ERROR_VARIABLE  gvxr_test_error
        )
    ELSE (WIN32)
        execute_process(
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/gvxr-build  --config Debug --target test
            OUTPUT_FILE gvxr_test.out.log
            ERROR_FILE gvxr_test.err.log
            RESULT_VARIABLE gvxr_test_result
            OUTPUT_VARIABLE gvxr_test_output
            ERROR_VARIABLE  gvxr_test_error
        )
    ENDIF (WIN32)
    MESSAGE ("Unit testing gvxr-core")
    MESSAGE ("")

    IF (NOT ${gvxr_test_result} EQUAL 0 OR
        NOT ${gvxr_test_output} STREQUAL  "" OR
        NOT ${gvxr_test_error} STREQUAL  "")

        MESSAGE (FATAL_ERROR "Error testing the project, see the log files (gvxr_test.out.log and gvxr_test.err.log)")
    ENDIF ()

ENDIF ()






MESSAGE ("Configure SimpleGVXR-core")
IF (WIN32)
    execute_process(
COMMAND ${CMAKE_COMMAND} -DCMAKE_CXX_FLAGS=/DWIN32\ /D_WINDOWS\ /W3\ /GR\ /EHsc\ /MP -DCMAKE_INSTALL_PREFIX=${DESTINATION} -DUSE_SYSTEM_gVirtualXRay:BOOL=ON
 -DBUILD_TESTING=${UNIT_TESTS} ${CMAKE_CURRENT_LIST_DIR}/SimpleGVXR
        WORKING_DIRECTORY  ${CMAKE_CURRENT_LIST_DIR}/SimpleGVXR-build
        OUTPUT_FILE ../SimpleGVXR_cmake.out.log
        ERROR_FILE ../SimpleGVXR_cmake.err.log
        RESULT_VARIABLE SimpleGVXR_cmake_result
        OUTPUT_VARIABLE SimpleGVXR_cmake_output
        ERROR_VARIABLE  SimpleGVXR_cmake_error
      )
ELSE (WIN32)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=${DESTINATION} -DUSE_SYSTEM_gVirtualXRay:BOOL=ON -DBUILD_TESTING=${UNIT_TESTS} --config Debug ${CMAKE_CURRENT_LIST_DIR}/SimpleGVXR
        WORKING_DIRECTORY  ${CMAKE_CURRENT_LIST_DIR}/SimpleGVXR-build
        OUTPUT_FILE ../SimpleGVXR_cmake.out.log
        ERROR_FILE ../SimpleGVXR_cmake.err.log
        RESULT_VARIABLE SimpleGVXR_cmake_result
        OUTPUT_VARIABLE SimpleGVXR_cmake_output
        ERROR_VARIABLE  SimpleGVXR_cmake_error
    )
ENDIF (WIN32)
MESSAGE ("Configure SimpleGVXR-core: DONE")
MESSAGE ("")

IF (NOT ${SimpleGVXR_cmake_result} EQUAL 0 OR
    NOT ${SimpleGVXR_cmake_output} STREQUAL  "" OR
    NOT ${SimpleGVXR_cmake_error} STREQUAL  "")

    MESSAGE (FATAL_ERROR "Error configuring the project, see the log files (SimpleGVXR_cmake.out.log and SimpleGVXR_cmake.err.log)")
ENDIF ()


MESSAGE ("Build SimpleGVXR-core")
IF (WIN32)
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/SimpleGVXR-build --config Debug
        OUTPUT_FILE SimpleGVXR_make.out.log
        ERROR_FILE SimpleGVXR_make.err.log
        RESULT_VARIABLE SimpleGVXR_make_result
        OUTPUT_VARIABLE SimpleGVXR_make_output
        ERROR_VARIABLE  SimpleGVXR_make_error
    )
ELSE (WIN32)
    MESSAGE ("Run parallel build, but not on MSVC")
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/SimpleGVXR-build --config Debug -- -j12
        OUTPUT_FILE SimpleGVXR_make.out.log
        ERROR_FILE SimpleGVXR_make.err.log
        RESULT_VARIABLE SimpleGVXR_make_result
        OUTPUT_VARIABLE SimpleGVXR_make_output
        ERROR_VARIABLE  SimpleGVXR_make_error
    )
ENDIF (WIN32)
MESSAGE ("Build SimpleGVXR-core: DONE")
MESSAGE ("")

IF (NOT ${SimpleGVXR_make_result} EQUAL 0 OR
    NOT ${SimpleGVXR_make_output} STREQUAL  "" OR
    NOT ${SimpleGVXR_make_error} STREQUAL  "")

    MESSAGE (FATAL_ERROR "Error building the project, see the log files (SimpleGVXR_make.out.log and SimpleGVXR_make.err.log)")
ENDIF ()

MESSAGE ("Install SimpleGVXR-core")
IF (WIN32)
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/SimpleGVXR-build  --config Debug --target INSTALL
        OUTPUT_FILE SimpleGVXR_install.out.log
        ERROR_FILE SimpleGVXR_install.err.log
        RESULT_VARIABLE SimpleGVXR_install_result
        OUTPUT_VARIABLE SimpleGVXR_install_output
        ERROR_VARIABLE  SimpleGVXR_install_error
    )
ELSE (WIN32)
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/SimpleGVXR-build  --config Debug --target install
        OUTPUT_FILE SimpleGVXR_install.out.log
        ERROR_FILE SimpleGVXR_install.err.log
        RESULT_VARIABLE SimpleGVXR_install_result
        OUTPUT_VARIABLE SimpleGVXR_install_output
        ERROR_VARIABLE  SimpleGVXR_install_error
    )
ENDIF (WIN32)
MESSAGE ("Install SimpleGVXR-core: DONE")
MESSAGE ("")

IF (NOT ${SimpleGVXR_install_result} EQUAL 0 OR
    NOT ${SimpleGVXR_install_output} STREQUAL  "" OR
    NOT ${SimpleGVXR_install_error} STREQUAL  "")

    MESSAGE (FATAL_ERROR "Error installing the project, see the log files (SimpleGVXR_install.out.log and SimpleGVXR_install.err.log)")
ENDIF ()

IF (UNIT_TESTS)
    MESSAGE ("Unit testing SimpleGVXR-core")
    IF (WIN32)
        execute_process(
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/SimpleGVXR-build  --config Debug --target RUN_TESTS
            OUTPUT_FILE SimpleGVXR_test.out.log
            ERROR_FILE SimpleGVXR_test.err.log
            RESULT_VARIABLE SimpleGVXR_test_result
            OUTPUT_VARIABLE SimpleGVXR_test_output
            ERROR_VARIABLE  SimpleGVXR_test_error
        )
    ELSE (WIN32)
        execute_process(
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/SimpleGVXR-build  --config Debug --target test
            OUTPUT_FILE SimpleGVXR_test.out.log
            ERROR_FILE SimpleGVXR_test.err.log
            RESULT_VARIABLE SimpleGVXR_test_result
            OUTPUT_VARIABLE SimpleGVXR_test_output
            ERROR_VARIABLE  SimpleGVXR_test_error
        )
    ENDIF (WIN32)
    MESSAGE ("Unit testing SimpleGVXR-core")
    MESSAGE ("")

    IF (NOT ${SimpleGVXR_test_result} EQUAL 0 OR
        NOT ${SimpleGVXR_test_output} STREQUAL  "" OR
        NOT ${SimpleGVXR_test_error} STREQUAL  "")

        MESSAGE (FATAL_ERROR "Error testing the project, see the log files (SimpleGVXR_test.out.log and SimpleGVXR_test.err.log)")
    ENDIF ()

ENDIF ()







MESSAGE ("Configure Wrappers-core")
IF (WIN32)
    execute_process(COMMAND ${CMAKE_COMMAND}
        -DCMAKE_CXX_FLAGS=/DWIN32\ /D_WINDOWS\ /W3\ /GR\ /EHsc\ /MP
        -DCMAKE_INSTALL_PREFIX=${DESTINATION}
        -DUSE_SYSTEM_SimpleGVXR:BOOL=ON
        -DBUILD_TESTING=${UNIT_TESTS}
        -DBUILD_CSHARP:BOOL=${BUILD_CSHARP}
        -DBUILD_JAVA:BOOL=${BUILD_JAVA}
        -DBUILD_OCTAVE:BOOL=${BUILD_OCTAVE}
        -DBUILD_PERL:BOOL=${BUILD_PERL}
        -DBUILD_PYTHON3:BOOL=${BUILD_PYTHON3}
        -DBUILD_PYTHON2:BOOL=${BUILD_PYTHON2}
        -DBUILD_R:BOOL=${BUILD_R}
        -DBUILD_RUBY:BOOL=${BUILD_RUBY}
        -DBUILD_TCL:BOOL=${BUILD_TCL}

        ${CMAKE_CURRENT_LIST_DIR}/Wrappers

        WORKING_DIRECTORY  ${CMAKE_CURRENT_LIST_DIR}/Wrappers-build
        OUTPUT_FILE ../Wrappers_cmake.out.log
        ERROR_FILE ../Wrappers_cmake.err.log
        RESULT_VARIABLE Wrappers_cmake_result
        OUTPUT_VARIABLE Wrappers_cmake_output
        ERROR_VARIABLE  Wrappers_cmake_error
      )
ELSE (WIN32)
    execute_process(COMMAND ${CMAKE_COMMAND}
        -DCMAKE_INSTALL_PREFIX=${DESTINATION}
        -DUSE_SYSTEM_SimpleGVXR:BOOL=ON
        -DBUILD_TESTING=${UNIT_TESTS}
        -DBUILD_CSHARP:BOOL=${BUILD_CSHARP}
        -DBUILD_JAVA:BOOL=${BUILD_JAVA}
        -DBUILD_OCTAVE:BOOL=${BUILD_OCTAVE}
        -DBUILD_PERL:BOOL=${BUILD_PERL}
        -DBUILD_PYTHON3:BOOL=${BUILD_PYTHON3}
        -DBUILD_PYTHON2:BOOL=${BUILD_PYTHON2}
        -DBUILD_R:BOOL=${BUILD_R}
        -DBUILD_RUBY:BOOL=${BUILD_RUBY}
        -DBUILD_TCL:BOOL=${BUILD_TCL}

        ${CMAKE_CURRENT_LIST_DIR}/Wrappers

        WORKING_DIRECTORY  ${CMAKE_CURRENT_LIST_DIR}/Wrappers-build
        OUTPUT_FILE ../Wrappers_cmake.out.log
        ERROR_FILE ../Wrappers_cmake.err.log
        RESULT_VARIABLE Wrappers_cmake_result
        OUTPUT_VARIABLE Wrappers_cmake_output
        ERROR_VARIABLE  Wrappers_cmake_error
    )
ENDIF (WIN32)
MESSAGE ("Configure Wrappers-core: DONE")
MESSAGE ("")

IF (NOT ${Wrappers_cmake_result} EQUAL 0 OR
    NOT ${Wrappers_cmake_output} STREQUAL  "" OR
    NOT ${Wrappers_cmake_error} STREQUAL  "")

    MESSAGE (FATAL_ERROR "Error configuring the project, see the log files (Wrappers_cmake.out.log and Wrappers_cmake.err.log)")
ENDIF ()


MESSAGE ("Build Wrappers-core")
IF (WIN32)
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/Wrappers-build --config Debug
        OUTPUT_FILE Wrappers_make.out.log
        ERROR_FILE Wrappers_make.err.log
        RESULT_VARIABLE Wrappers_make_result
        OUTPUT_VARIABLE Wrappers_make_output
        ERROR_VARIABLE  Wrappers_make_error
    )
ELSE (WIN32)
    MESSAGE ("Run parallel build, but not on MSVC")
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/Wrappers-build --config Debug -- -j12
        OUTPUT_FILE Wrappers_make.out.log
        ERROR_FILE Wrappers_make.err.log
        RESULT_VARIABLE Wrappers_make_result
        OUTPUT_VARIABLE Wrappers_make_output
        ERROR_VARIABLE  Wrappers_make_error
    )
ENDIF (WIN32)
MESSAGE ("Build Wrappers-core: DONE")
MESSAGE ("")

IF (NOT ${Wrappers_make_result} EQUAL 0 OR
    NOT ${Wrappers_make_output} STREQUAL  "" OR
    NOT ${Wrappers_make_error} STREQUAL  "")

    MESSAGE (FATAL_ERROR "Error building the project, see the log files (Wrappers_make.out.log and Wrappers_make.err.log)")
ENDIF ()

MESSAGE ("Install Wrappers-core")
IF (WIN32)
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/Wrappers-build  --config Debug --target INSTALL
        OUTPUT_FILE Wrappers_install.out.log
        ERROR_FILE Wrappers_install.err.log
        RESULT_VARIABLE Wrappers_install_result
        OUTPUT_VARIABLE Wrappers_install_output
        ERROR_VARIABLE  Wrappers_install_error
    )
ELSE (WIN32)
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/Wrappers-build  --config Debug --target install
        OUTPUT_FILE Wrappers_install.out.log
        ERROR_FILE Wrappers_install.err.log
        RESULT_VARIABLE Wrappers_install_result
        OUTPUT_VARIABLE Wrappers_install_output
        ERROR_VARIABLE  Wrappers_install_error
    )
ENDIF (WIN32)
MESSAGE ("Install Wrappers-core: DONE")
MESSAGE ("")

IF (NOT ${Wrappers_install_result} EQUAL 0 OR
    NOT ${Wrappers_install_output} STREQUAL  "" OR
    NOT ${Wrappers_install_error} STREQUAL  "")

    MESSAGE (FATAL_ERROR "Error installing the project, see the log files (Wrappers_install.out.log and Wrappers_install.err.log)")
ENDIF ()

IF (WRAPPER_TEST)
    MESSAGE (STATUS "Franck to do.")
ENDIF ()
