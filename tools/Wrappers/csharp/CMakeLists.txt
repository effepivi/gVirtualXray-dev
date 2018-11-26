SET (module gvxrCSharp)


configure_file(../gvxrWrapper-doc.in ${CMAKE_CURRENT_BINARY_DIR}/${module}.i)
SET_PROPERTY(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/${module}.i PROPERTY CPLUSPLUS ON)

# Add swig module
IF(${CMAKE_VERSION} VERSION_LESS "3.8.0") 
    SWIG_ADD_MODULE(${module} csharp ${CMAKE_CURRENT_BINARY_DIR}/${module}.i)
ELSE (${CMAKE_VERSION} VERSION_LESS "3.8.0") 
    SWIG_ADD_LIBRARY(${module} LANGUAGE csharp SOURCES ${CMAKE_CURRENT_BINARY_DIR}/${module}.i)
ENDIF (${CMAKE_VERSION} VERSION_LESS "3.8.0") 
swig_link_libraries(${module} SimpleGVXR ${CSHARP_LIBRARIES})

IF (MSVC)
    ADD_DEPENDENCIES(_${module} SimpleGVXR)
ENDIF (MSVC)

configure_file(test.cs.in ${CMAKE_CURRENT_BINARY_DIR}/test.cs)
FILE(COPY ../../../examples/data/welsh-dragon-small.stl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
