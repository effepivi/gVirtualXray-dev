#find_package(Perl REQUIRED)
find_package(PerlLibs REQUIRED)

INCLUDE_DIRECTORIES(${PERL_INCLUDE_PATH})

SET (module gvxrPerl)


configure_file(../gvxrWrapper.in ${CMAKE_CURRENT_BINARY_DIR}/gvxrPerl.i)
SET_PROPERTY(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/gvxrPerl.i PROPERTY CPLUSPLUS ON)

# Add swig module
SWIG_ADD_LIBRARY(gvxrPerl LANGUAGE perl SOURCES ${CMAKE_CURRENT_BINARY_DIR}/gvxrPerl.i)
swig_link_libraries(gvxrPerl gvxrWrappers ${PERL_LIBRARY})

configure_file(test.pl.in ${CMAKE_CURRENT_BINARY_DIR}/test.pl)
FILE(COPY ../../../examples/data/welsh-dragon-small.stl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
