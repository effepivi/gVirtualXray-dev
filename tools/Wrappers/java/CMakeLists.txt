find_package(Java COMPONENTS Development REQUIRED)
include(UseJava)

find_package(JNI REQUIRED)
INCLUDE_DIRECTORIES(${JNI_INCLUDE_DIRS})
	    

SET (module gvxrJava)


configure_file(../gvxrWrapper-doc.in ${CMAKE_CURRENT_BINARY_DIR}/gvxrJava.i)
SET_PROPERTY(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/gvxrJava.i PROPERTY CPLUSPLUS ON)

# Add swig module
SWIG_ADD_LIBRARY(gvxrJava LANGUAGE java SOURCES ${CMAKE_CURRENT_BINARY_DIR}/gvxrJava.i)
swig_link_libraries(gvxrJava SimpleGVXR ${JNI_LIBRARIES})

IF (MSVC)
	ADD_DEPENDENCIES(_${module} SimpleGVXR)
ENDIF (MSVC)

configure_file(test.java.in ${CMAKE_CURRENT_BINARY_DIR}/test.java)
FILE(COPY ../../../examples/data/welsh-dragon-small.stl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
FILE(COPY runJavaTest.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_jar(testJava ${CMAKE_CURRENT_BINARY_DIR}/test.java)


# Files to install with Python
#set(PYTHON_INSTALL_FILES
#                ${CMAKE_CURRENT_BINARY_DIR}/gvxrJava.java
#                ${CMAKE_CURRENT_BINARY_DIR}/_gvxrJava.jar)

