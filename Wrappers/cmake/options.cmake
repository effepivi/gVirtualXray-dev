# The options ##################################################################
option(BUILD_WRAPPER_CSHARP  "Build C# SWIG module" OFF)
option(BUILD_WRAPPER_LUA     "Build Lua SWIG module" OFF)
option(BUILD_WRAPPER_JAVA    "Build Java SWIG module" OFF)
option(BUILD_WRAPPER_JAVASCRIPT "Build JavaScript SWIG module" OFF)
option(BUILD_WRAPPER_MATLAB  "Build Matlab SWIG module" OFF)
option(BUILD_WRAPPER_OCTAVE  "Build Octave SWIG module" OFF)
option(BUILD_WRAPPER_PERL    "Build Perl SWIG module" OFF)
option(BUILD_WRAPPER_PYTHON2 "Build Python2 SWIG module" OFF)
option(BUILD_WRAPPER_PYTHON3 "Build Python3 SWIG module" OFF)
option(BUILD_WRAPPER_R       "Build R SWIG module" OFF)
option(BUILD_WRAPPER_RUBY    "Build Ruby SWIG module" OFF)
option(BUILD_WRAPPER_TCL     "Build TCL SWIG module" OFF)

IF ( BUILD_WRAPPER_CSHARP )
    SET (BUILD_WRAPPERS ON)
ENDIF ()

IF ( BUILD_WRAPPER_LUA )
    #SET (BUILD_WRAPPERS ON)
    MESSAGE (WARNING "Lua wrapper is not available, contact Franck Vidal if it is needed")
ENDIF ()

IF ( BUILD_WRAPPER_JAVA )
    SET (BUILD_WRAPPERS ON)
ENDIF ()

IF ( BUILD_WRAPPER_JAVASCRIPT )
    #SET (BUILD_WRAPPERS ON)
    MESSAGE (WARNING "Javascript wrapper is not available, contact Franck Vidal if it is needed")
ENDIF ()

IF ( BUILD_WRAPPER_MATLAB )
    #SET (BUILD_WRAPPERS ON)
    MESSAGE (WARNING "Matlab wrapper is not available, contact Franck Vidal if it is needed")
ENDIF ()

IF ( BUILD_WRAPPER_OCTAVE )
    SET (BUILD_WRAPPERS ON)
ENDIF ()

IF ( BUILD_WRAPPER_PERL )
    SET (BUILD_WRAPPERS ON)
ENDIF ()

IF ( BUILD_WRAPPER_PYTHON2 )
    SET (BUILD_WRAPPERS ON)
ENDIF ()

IF ( BUILD_WRAPPER_PYTHON3 )
    SET (BUILD_WRAPPERS ON)
ENDIF ()

IF ( BUILD_WRAPPER_R )
    SET (BUILD_WRAPPERS ON)
ENDIF ()

IF ( BUILD_WRAPPER_RUBY )
    SET (BUILD_WRAPPERS ON)
ENDIF ()

IF ( BUILD_WRAPPER_TCL )
    SET (BUILD_WRAPPERS ON)
ENDIF ()
