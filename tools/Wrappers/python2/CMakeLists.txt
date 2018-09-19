# Include python
find_package(PythonLibs 2 REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})

SET (module gvxrPython2)
configure_file(../gvxrWrapper.in ${CMAKE_CURRENT_BINARY_DIR}/gvxrPython2.i)
SET_PROPERTY(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/gvxrPython2.i PROPERTY CPLUSPLUS ON)

# Add swig module
SWIG_ADD_LIBRARY(gvxrPython2 LANGUAGE python SOURCES ${CMAKE_CURRENT_BINARY_DIR}/gvxrPython2.i)
swig_link_libraries(gvxrPython2 gvxrWrappers ${PYTHON_LIBRARIES})

# Files to install with Python
set(PYTHON_INSTALL_FILES
        ${CMAKE_CURRENT_BINARY_DIR}/gvxrPython2.py
        ${CMAKE_CURRENT_BINARY_DIR}/_gvxrPython2.so)

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
#        DEPENDS _gvxrPython2
#        COMMAND python ${SETUP_PY_OUT} install)
