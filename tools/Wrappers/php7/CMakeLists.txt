find_package(php7 REQUIRED)

INCLUDE_DIRECTORIES(${OCTAVE_INCLUDE_DIRS})

SET (module gvxrPhp)


configure_file(../gvxrWrapper-doc.in ${CMAKE_CURRENT_BINARY_DIR}/gvxrPhp.i)
SET_PROPERTY(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/gvxrPhp.i PROPERTY CPLUSPLUS ON)

# Add swig module
SWIG_ADD_LIBRARY(gvxrPhp LANGUAGE php7 SOURCES ${CMAKE_CURRENT_BINARY_DIR}/gvxrPhp.i)
swig_link_libraries(gvxrPhp SimpleGVXR ${OCTAVE_LIBRARIES})

configure_file(test.php.in ${CMAKE_CURRENT_BINARY_DIR}/test.php)
FILE(COPY ../../../examples/data/welsh-dragon-small.stl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
