find_package(Octave REQUIRED)

INCLUDE_DIRECTORIES(${OCTAVE_INCLUDE_DIRS})

SET (module gvxrOctave)


configure_file(../gvxrWrapper-doc.in ${CMAKE_CURRENT_BINARY_DIR}/gvxrOctave.i)
SET_PROPERTY(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/gvxrOctave.i PROPERTY CPLUSPLUS ON)

# Add swig module
IF(${CMAKE_VERSION} VERSION_LESS "3.8.0") 
    SWIG_ADD_MODULE(${module} octave ${CMAKE_CURRENT_BINARY_DIR}/${module}.i)
ELSE (${CMAKE_VERSION} VERSION_LESS "3.8.0") 
    SWIG_ADD_LIBRARY(${module} LANGUAGE octave SOURCES ${CMAKE_CURRENT_BINARY_DIR}/${module}.i)
ENDIF (${CMAKE_VERSION} VERSION_LESS "3.8.0") 
swig_link_libraries(gvxrOctave SimpleGVXR ${OCTAVE_LIBRARIES})

IF (MSVC)
    ADD_DEPENDENCIES(_${module} SimpleGVXR)
ENDIF (MSVC)

configure_file(test.m.in ${CMAKE_CURRENT_BINARY_DIR}/test.m)
FILE(COPY ../../../examples/data/welsh-dragon-small.stl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
