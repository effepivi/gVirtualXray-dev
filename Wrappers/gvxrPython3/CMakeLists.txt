# Include python

find_package(Python3 COMPONENTS Interpreter Development.Module Development.Embed REQUIRED)




SET (module temp_gvxrPython3)

FIND_PACKAGE(Doxygen)


if(Doxygen_FOUND AND BUILD_SimpleGVXR_DOCUMENTATION)
    SET (SimpleGVXR_DOC_I_FILE 		${CMAKE_CURRENT_BINARY_DIR}/SimpleGVXR-doc.i)

    add_custom_command(
        COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/gvxrPython3/doxy2swig/doxy2swig.py ${SimpleGVXR_Doxy_XML}/index.xml ${SimpleGVXR_DOC_I_FILE}
        DEPENDS SGVXR_Documentation
        OUTPUT ${SimpleGVXR_DOC_I_FILE}
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        VERBATIM
    )
    add_custom_target(doxy2swig DEPENDS ${SimpleGVXR_DOC_I_FILE})

    configure_file(../gvxrWrapper-doc.in ${CMAKE_CURRENT_BINARY_DIR}/${module}.i)

    message(STATUS "The help about gVXR in Python will be built.")
ELSE ()
    SET (SimpleGVXR_DOC_I_FILE 		${CMAKE_CURRENT_BINARY_DIR}/SimpleGVXR-nodoc.i)
    configure_file(../gvxrWrapper-nodoc.in ${CMAKE_CURRENT_BINARY_DIR}/${module}.i)

    IF (NOT Doxygen_FOUND)
        message(WARNING "Doxygen was not found, the help about gVXR in Python will be disabled.")
    ELSE (NOT Doxygen_FOUND)
        IF (NOT BUILD_SimpleGVXR_DOCUMENTATION)
            message(WARNING "Doxygen was found but you disabled build the documentation, the help about gVXR in Python will be disabled.")
        ENDIF (NOT BUILD_SimpleGVXR_DOCUMENTATION)
    ENDIF (NOT Doxygen_FOUND)
 endif()


SET_PROPERTY(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/${module}.i PROPERTY CPLUSPLUS ON)

IF (WIN32)
  ADD_DEFINITIONS(-DSWIG_PYTHON_INTERPRETER_NO_DEBUG)
ENDIF()


# Add swig module
IF(${CMAKE_VERSION} VERSION_LESS "3.8.0")
    SWIG_ADD_MODULE(${module} python ${CMAKE_CURRENT_BINARY_DIR}/${module}.i)
ELSE (${CMAKE_VERSION} VERSION_LESS "3.8.0")
    SWIG_ADD_LIBRARY(${module} LANGUAGE python SOURCES ${CMAKE_CURRENT_BINARY_DIR}/${module}.i)
ENDIF (${CMAKE_VERSION} VERSION_LESS "3.8.0")


TARGET_INCLUDE_DIRECTORIES(${module} PRIVATE ${Python3_INCLUDE_DIRS})


set_property(
    SOURCE ${CMAKE_CURRENT_BINARY_DIR}/${module}.i
    PROPERTY SWIG_MODULE_NAME ${module})

#find_package(CUDAToolkit)
#IF (CUDAToolkit_FOUND)
#    SET (optionalLibs CUDA::cudart_static CUDA::cudart ${optionalLibs})
#ENDIF (CUDAToolkit_FOUND)



IF (APPLE)
    #swig_link_libraries(${module} Python3::Python ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs})
    swig_link_libraries(${module} ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs})
ELSE (APPLE)
    IF (WIN32)
        swig_link_libraries(${module} Python3::Python ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs})
    ELSE (WIN32)
        swig_link_libraries(${module} ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs})
    ENDIF (WIN32)
ENDIF (APPLE)

IF (USE_SUPERBUILD)

    ADD_DEPENDENCIES( ${module} SimpleGVXR )

    IF (Doxygen_FOUND AND BUILD_SimpleGVXR_DOCUMENTATION)
        MESSAGE (STATUS "Add dependencies to ${module}")
        ADD_DEPENDENCIES(${module} doxy2swig)
    ENDIF ()

ENDIF ()


configure_file(test.py.in ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/test.py)
FILE(COPY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/test.py #${CMAKE_CURRENT_SOURCE_DIR}/__init__.py
		DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
		FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
  			GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

FILE(COPY ../welsh-dragon-small.stl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

FILE(COPY computeLBuffer.py
		DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
		FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
  			GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# Files to install with Python
set(PYTHON_INSTALL_FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${module}.py
        #${CMAKE_CURRENT_SOURCE_DIR}/__init__.py
        ${CMAKE_CURRENT_SOURCE_DIR}/../welsh-dragon-small.stl
)

IF (WIN32 AND MSVC)

    FILE(COPY ${XRAYLIB_SHARED_LIB_PATH} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

	set(PYTHON_INSTALL_FILES
		${PYTHON_INSTALL_FILES}
		${CMAKE_CURRENT_BINARY_DIR}/Release/_${module}.pyd
        ${XRAYLIB_SHARED_LIB_PATH})

ELSE (WIN32 AND MSVC)
 	set(PYTHON_INSTALL_FILES
		${PYTHON_INSTALL_FILES}
		${CMAKE_CURRENT_BINARY_DIR}/_${module}.so)

ENDIF (WIN32 AND MSVC)

# Configure setup.py and copy to output directory
#set(SETUP_PY_IN ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in)
#set(SETUP_PY_OUT ${CMAKE_CURRENT_BINARY_DIR}/setup.py)
#configure_file(${SETUP_PY_IN} ${SETUP_PY_OUT})

# Install target to call setup.py
#add_custom_target(install-${module}
#        DEPENDS ${module}
#        COMMAND ${PYTHON_EXECUTABLE} ${SETUP_PY_OUT} install --skip-build --root ${CMAKE_CURRENT_BINARY_DIR})

#install(FILES ${gVirtualXRay_XRAYLIB_LIBS}
#    DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/_gvxrPython3/
#    COMPONENT Python)


#INSTALL (FILES ${gVirtualXRay_XRAYLIB_LIBS} ${XRAYLIB_SHARED_LIB_PATH}
#    DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/_gvxrPython3
#    COMPONENT Python
#)

install(FILES
    ${PYTHON_INSTALL_FILES}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/_gvxrPython3
    COMPONENT Python)

#install(FILES
#    ${CMAKE_CURRENT_SOURCE_DIR}/../welsh-dragon-small.stl
#    DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}
#    COMPONENT Python)

#install(FILES
#    ${CMAKE_CURRENT_SOURCE_DIR}/test.py ${CMAKE_CURRENT_SOURCE_DIR}/test-offscreen.py ${CMAKE_CURRENT_SOURCE_DIR}/job.sh
#    DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}
#    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
#    COMPONENT Python)

#install(FILES
#    DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/_gvxrPython3
#    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
#    COMPONENT Python)


add_custom_target(install-python
    ${CMAKE_COMMAND}
    -DCMAKE_INSTALL_COMPONENT=Python
    -DCMAKE_BUILD_TYPE=Release
    -P "${PROJECT_BINARY_DIR}/cmake_install.cmake"
    DEPENDS gVirtualXRay SimpleGVXR ${module}
    )

SET (CPACK_COMPONENTS_ALL ${CPACK_COMPONENTS_ALL}  Python PARENT_SCOPE)
