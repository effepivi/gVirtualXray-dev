project(gVirtualXRay-tutorials)

INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../build/version.cmake)

find_package(gVirtualXRay REQUIRED)

add_subdirectory(tutorial_00_glfw)
add_subdirectory(tutorial_00_glut)
add_subdirectory(tutorial_01_glfw)
add_subdirectory(tutorial_01_glut)
add_subdirectory(tutorial_02_glfw)
add_subdirectory(tutorial_02_glut)
add_subdirectory(tutorial_03_glfw)
add_subdirectory(tutorial_03_glut)

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE.txt DESTINATION tutorials COMPONENT tutorials)

