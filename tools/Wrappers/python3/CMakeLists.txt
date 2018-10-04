# Include python

find_package(PythonLibs 3 REQUIRED)
find_package(PythonInterp REQUIRED)


INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})
	    

SET (module gvxrPython3)


configure_file(../gvxrWrapper-doc.in ${CMAKE_CURRENT_BINARY_DIR}/${module}.i)
SET_PROPERTY(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/${module}.i PROPERTY CPLUSPLUS ON)

# Add swig module
SWIG_ADD_LIBRARY(${module} LANGUAGE python SOURCES ${CMAKE_CURRENT_BINARY_DIR}/${module}.i)
swig_link_libraries(${module} SimpleGVXR ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})
#ADD_DEPENDENCIES(_${module} SimpleGVXR)

configure_file(test.py.in ${CMAKE_CURRENT_BINARY_DIR}/test.py)
FILE(COPY ../../../examples/data/welsh-dragon-small.stl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
FILE(COPY computeLBuffer.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Files to install with Python
set(PYTHON_INSTALL_FILES
		${CMAKE_CURRENT_BINARY_DIR}/${module}.py
		${CMAKE_CURRENT_BINARY_DIR}/_${module}.so)

# Configure setup.py and copy to output directory
set(SETUP_PY_IN ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in)
set(SETUP_PY_OUT ${CMAKE_CURRENT_BINARY_DIR}/setup.py)
configure_file(${SETUP_PY_IN} ${SETUP_PY_OUT})

# Install target to call setup.py
add_custom_target(install-${module}
        DEPENDS _${module}
        COMMAND ${PYTHON_EXECUTABLE} ${SETUP_PY_OUT} install --skip-build --root ${CMAKE_CURRENT_BINARY_DIR})

