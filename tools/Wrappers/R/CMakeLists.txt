find_package(R REQUIRED)

INCLUDE_DIRECTORIES(${R_INCLUDE_DIR})

SET (module gvxrR)


configure_file(../gvxrWrapper.in ${CMAKE_CURRENT_BINARY_DIR}/gvxrR.i)
SET_PROPERTY(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/gvxrR.i PROPERTY CPLUSPLUS ON)

# Add swig module
SWIG_ADD_LIBRARY(gvxrR LANGUAGE r SOURCES ${CMAKE_CURRENT_BINARY_DIR}/gvxrR.i)
swig_link_libraries(gvxrR gvxrWrappers ${R_LIBRARIES})

configure_file(test.m.in ${CMAKE_CURRENT_BINARY_DIR}/test.m)
FILE(COPY ../../../examples/data/welsh-dragon-small.stl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
