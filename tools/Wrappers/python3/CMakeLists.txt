# Include python

find_package(PythonLibs 3 REQUIRED)

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})
	    

SET (module gvxrPython3)


configure_file(../gvxrWrapper.in ${CMAKE_CURRENT_BINARY_DIR}/gvxrPython3.i)
SET_PROPERTY(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/gvxrPython3.i PROPERTY CPLUSPLUS ON)

# Add swig module
SWIG_ADD_LIBRARY(gvxrPython3 LANGUAGE python SOURCES ${CMAKE_CURRENT_BINARY_DIR}/gvxrPython3.i)
swig_link_libraries(gvxrPython3 gvxrWrappers ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})

        configure_file(test.py.in ${CMAKE_CURRENT_BINARY_DIR}/test.py)

        FILE(COPY ../../../examples/data/welsh-dragon-small.stl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

        # Files to install with Python
        set(PYTHON_INSTALL_FILES
                ${CMAKE_CURRENT_BINARY_DIR}/gvxrPython3.py
                ${CMAKE_CURRENT_BINARY_DIR}/_gvxrPython3.so)

        # Configure setup.py and copy to output directory
        #set(SETUP_PY_IN ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in)
        #set(SETUP_PY_OUT ${CMAKE_CURRENT_BINARY_DIR}/setup.py)
        #configure_file(${SETUP_PY_IN} ${SETUP_PY_OUT})

        # Declare install target for python
        #install(TARGETS swig_example
        #        COMMAND "${PYTHON_EXECUTABLE} setup.py"
        #        COMPONENT swig-python)

        # Install target to call setup.py
        #add_custom_target(install-python
        #        DEPENDS _gvxrPython3
        #        COMMAND python ${SETUP_PY_OUT} install)
