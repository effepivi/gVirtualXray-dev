project(gVirtualXRay-STL-File)

# Initialise some states by default ############################################
set(CMAKE_BUILD_TYPE Release CACHE STRING "Debug|Release|RelWithDebInfo|MinSizeRel")
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../../build/version.cmake)


# Create the RESOURCES ###########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../resources.cmake)
CreateHeaderFiles("." "BinarySTLFiles" "${RESOURCES_SOURCES}")


# Headers of the project #######################################################
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})


# Configure the project ########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/../config.cmake)


# Check if GLUT or GLFW are available #########################################
IF(NOT glfw_FOUND AND NOT GLUT_FOUND)
    MESSAGE(FATAL_ERROR "GLFW and GLUT were not found. The demo cannot be built.")
ENDIF(NOT glfw_FOUND AND NOT GLUT_FOUND)


# Create the main programs #####################################################
IF(glfw_FOUND)
    add_executable(welsh_dragon_glfw welsh_dragon_glfw.cxx )
    target_link_libraries(welsh_dragon_glfw ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(welsh_dragon_glfw BinarySTLFiles)

    INSTALL(TARGETS welsh_dragon_glfw RUNTIME DESTINATION demos/STL-File/bin  COMPONENT demos)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../data/welsh-dragon-small.stl DESTINATION demos/STL-File/data COMPONENT demos)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION demos/STL-File COMPONENT demos)
ENDIF(glfw_FOUND)


IF(GLUT_FOUND)
    add_executable(welsh_dragon_glut welsh_dragon_glut.cxx )
    target_link_libraries(welsh_dragon_glut ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})
    ADD_DEPENDENCIES(welsh_dragon_glut BinarySTLFiles)

    INSTALL(TARGETS welsh_dragon_glut RUNTIME DESTINATION demos/STL-File/bin  COMPONENT demos)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../data/welsh-dragon-small.stl DESTINATION demos/STL-File/data COMPONENT demos)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION demos/STL-File COMPONENT demos)
ENDIF(GLUT_FOUND)


IF (APPLE)
    IF(glfw_FOUND)

        SET (MACOSX_RPATH 1)
        SET(STL-File-GLFW_SRCS
            ${CMAKE_CURRENT_SOURCE_DIR}/welsh_dragon_glfw.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/../build/DragonDemo.icns
            ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt
            ${CMAKE_CURRENT_SOURCE_DIR}/../data/welsh-dragon-small.stl
        )

        ADD_EXECUTABLE (STL-File-GLFW MACOSX_BUNDLE ${STL-File-GLFW_SRCS})
        TARGET_LINK_LIBRARIES (STL-File-GLFW gVirtualXRay ${optionalLibs} ${requiredLibs})
        ADD_DEPENDENCIES(STL-File-GLFW BinaryGUIFLTKFiles)

        set(MACOSX_BUNDLE_BUNDLE_NAME STL-File)
        set(MACOSX_BUNDLE_GUI_IDENTIFIER "STL-File-GLFW")
        SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/../build/DragonDemo.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
        set(MACOSX_BUNDLE_ICON_FILE DragonDemo.icns)
        set(MACOSX_BUNDLE_INFO_STRING "${MAIN_PROJECT_VERSION}, ${COPYRIGHT_STRING}")
        set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${gVirtualXRay_VERSION_MAJOR}.${gVirtualXRay_VERSION_MINOR}")
        set(MACOSX_BUNDLE_LONG_VERSION_STRING "${MAIN_PROJECT_VERSION}")
        set(MACOSX_BUNDLE_BUNDLE_VERSION "${MAIN_PROJECT_VERSION}")
        set(MACOSX_BUNDLE_COPYRIGHT "${COPYRIGHT_STRING}")

        SET(APPS "\${CMAKE_INSTALL_PREFIX}/STL-File-GLFW.app")

        INSTALL(TARGETS STL-File-GLFW 
            BUNDLE DESTINATION . COMPONENT Runtime
            RUNTIME DESTINATION bin COMPONENT Runtime
        )

        SET_SOURCE_FILES_PROPERTIES (
            ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt
            ${CMAKE_CURRENT_SOURCE_DIR}/../data/welsh-dragon-small.stl
            PROPERTIES
            MACOSX_PACKAGE_LOCATION data
        )

        # directories to look for dependencies
        SET (DIRS /sw/lib /usr/local/lib ${Boost_LIBRARY_DIR})

        SET (LIBRARIES ${requiredLibs} ${optionalLibs})
        INSTALL(CODE "
            include(BundleUtilities)
            fixup_bundle(\"${APPS}\" \"\${LIBRARIES}\" \"${DIRS}\")
            " COMPONENT Runtime)

        set(CPACK_BINARY_DRAGNDROP ON)

    ENDIF(glfw_FOUND)

    IF(GLUT_FOUND)

        SET (MACOSX_RPATH 1)
        SET(STL-File-GLUT_SRCS
            ${CMAKE_CURRENT_SOURCE_DIR}/welsh_dragon_glut.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/../build/DragonDemo.icns
            ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt
            ${CMAKE_CURRENT_SOURCE_DIR}/../data/welsh-dragon-small.stl
        )

        ADD_EXECUTABLE (STL-File-GLUT MACOSX_BUNDLE ${STL-File-GLUT_SRCS})
        TARGET_LINK_LIBRARIES (STL-File-GLUT gVirtualXRay ${optionalLibs} ${requiredLibs})
        ADD_DEPENDENCIES(STL-File-GLUT BinaryGUIFLTKFiles)

        set(MACOSX_BUNDLE_BUNDLE_NAME STL-File)
        set(MACOSX_BUNDLE_GUI_IDENTIFIER "STL-File-GLUT")
        SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/../build/DragonDemo.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
        set(MACOSX_BUNDLE_ICON_FILE DragonDemo.icns)
        set(MACOSX_BUNDLE_INFO_STRING "${MAIN_PROJECT_VERSION}, ${COPYRIGHT_STRING}")
        set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${gVirtualXRay_VERSION_MAJOR}.${gVirtualXRay_VERSION_MINOR}")
        set(MACOSX_BUNDLE_LONG_VERSION_STRING "${MAIN_PROJECT_VERSION}")
        set(MACOSX_BUNDLE_BUNDLE_VERSION "${MAIN_PROJECT_VERSION}")
        set(MACOSX_BUNDLE_COPYRIGHT "${COPYRIGHT_STRING}")

        SET(APPS "\${CMAKE_INSTALL_PREFIX}/STL-File-GLUT.app")

        INSTALL(TARGETS STL-File-GLUT 
            BUNDLE DESTINATION . COMPONENT Runtime
            RUNTIME DESTINATION bin COMPONENT Runtime
        )

        SET_SOURCE_FILES_PROPERTIES (
            ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt
            ${CMAKE_CURRENT_SOURCE_DIR}/../data/welsh-dragon-small.stl
            PROPERTIES
            MACOSX_PACKAGE_LOCATION data
        )

        # directories to look for dependencies
        SET (DIRS /sw/lib /usr/local/lib ${Boost_LIBRARY_DIR})

        SET (LIBRARIES ${requiredLibs} ${optionalLibs})
        INSTALL(CODE "
            include(BundleUtilities)
            fixup_bundle(\"${APPS}\" \"\${LIBRARIES}\" \"${DIRS}\")
            " COMPONENT Runtime)

        set(CPACK_BINARY_DRAGNDROP ON)

    ENDIF(GLUT_FOUND)

ENDIF(APPLE)

SET (CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt)

include(CPack)
