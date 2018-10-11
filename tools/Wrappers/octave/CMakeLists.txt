find_package(Octave REQUIRED)

INCLUDE_DIRECTORIES(${OCTAVE_INCLUDE_DIRS})

SET (module gvxrOctave)


configure_file(../gvxrWrapper-doc.in ${CMAKE_CURRENT_BINARY_DIR}/gvxrOctave.i)
SET_PROPERTY(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/gvxrOctave.i PROPERTY CPLUSPLUS ON)

# Add swig module
SWIG_ADD_LIBRARY(gvxrOctave LANGUAGE octave SOURCES ${CMAKE_CURRENT_BINARY_DIR}/gvxrOctave.i)
swig_link_libraries(gvxrOctave SimpleGVXR ${OCTAVE_LIBRARIES})

IF (MSVC)
	ADD_DEPENDENCIES(_${module} SimpleGVXR)
ENDIF (MSVC)

configure_file(test.m.in ${CMAKE_CURRENT_BINARY_DIR}/test.m)
FILE(COPY ../../../examples/data/welsh-dragon-small.stl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
