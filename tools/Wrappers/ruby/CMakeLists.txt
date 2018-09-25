find_package(Ruby REQUIRED)

INCLUDE_DIRECTORIES(${RUBY_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${RUBY_INCLUDE_DIRS})

SET (module gvxrRuby)


configure_file(../gvxrWrapper.in ${CMAKE_CURRENT_BINARY_DIR}/gvxrRuby.i)
SET_PROPERTY(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/gvxrRuby.i PROPERTY CPLUSPLUS ON)

# Add swig module
SWIG_ADD_LIBRARY(gvxrRuby LANGUAGE ruby SOURCES ${CMAKE_CURRENT_BINARY_DIR}/gvxrRuby.i)
swig_link_libraries(gvxrRuby gvxrWrappers ${RUBY_LIBRARY})

configure_file(test.rb.in ${CMAKE_CURRENT_BINARY_DIR}/test.rb)
FILE(COPY ../../../examples/data/welsh-dragon-small.stl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
