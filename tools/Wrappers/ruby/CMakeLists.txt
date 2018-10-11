find_package(Ruby REQUIRED)

INCLUDE_DIRECTORIES(${RUBY_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${RUBY_INCLUDE_DIRS})

SET (module gvxrRuby)


configure_file(../gvxrWrapper-nodoc.in ${CMAKE_CURRENT_BINARY_DIR}/${module}.i)
SET_PROPERTY(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/${module}.i PROPERTY CPLUSPLUS ON)

# Add swig module
SWIG_ADD_LIBRARY(${module} LANGUAGE ruby SOURCES ${CMAKE_CURRENT_BINARY_DIR}/${module}.i)
swig_link_libraries(${module} SimpleGVXR ${RUBY_LIBRARY})

IF (MSVC)
	ADD_DEPENDENCIES(_${module} SimpleGVXR)
ENDIF (MSVC)

configure_file(test.rb.in ${CMAKE_CURRENT_BINARY_DIR}/test.rb)
FILE(COPY ../../../examples/data/welsh-dragon-small.stl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
