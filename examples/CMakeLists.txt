project(gVirtualXRay-demos)

INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../build/version.cmake)

find_package(gVirtualXRay REQUIRED)

add_subdirectory(STL-File)
add_subdirectory(GUI-FLTK)
add_subdirectory(Iso-Surfaces)

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE.txt DESTINATION demos COMPONENT demos)

