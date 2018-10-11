find_package(R REQUIRED)

INCLUDE_DIRECTORIES(${R_INCLUDE_DIR})

SET (module gvxrR)


configure_file(../gvxrWrapper-doc.in ${CMAKE_CURRENT_BINARY_DIR}/${module}.i)
SET_PROPERTY(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/${module}.i PROPERTY CPLUSPLUS ON)

# Add swig module
SWIG_ADD_LIBRARY(${module} LANGUAGE r SOURCES ${CMAKE_CURRENT_BINARY_DIR}/${module}.i)
swig_link_libraries(${module} SimpleGVXR ${R_LIBRARIES})

IF (MSVC)
	ADD_DEPENDENCIES(_${module} SimpleGVXR)
ENDIF (MSVC)

configure_file(test.R.in ${CMAKE_CURRENT_BINARY_DIR}/test.R)
FILE(COPY ../../../examples/data/welsh-dragon-small.stl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
