find_package(php7 REQUIRED)

INCLUDE_DIRECTORIES(${OCTAVE_INCLUDE_DIRS})

SET (module gvxrPhp)


configure_file(../gvxrWrapper-doc.in ${CMAKE_CURRENT_BINARY_DIR}/${module}.i)
SET_PROPERTY(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/${module}.i PROPERTY CPLUSPLUS ON)

# Add swig module
SWIG_ADD_LIBRARY(${module} LANGUAGE php7 SOURCES ${CMAKE_CURRENT_BINARY_DIR}/${module}.i)
swig_link_libraries(${module} SimpleGVXR ${OCTAVE_LIBRARIES})

IF (MSVC)
	ADD_DEPENDENCIES(_${module} SimpleGVXR)
ENDIF (MSVC)

configure_file(test.php.in ${CMAKE_CURRENT_BINARY_DIR}/test.php)
FILE(COPY ../../../examples/data/welsh-dragon-small.stl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
