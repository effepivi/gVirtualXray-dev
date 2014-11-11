project(gVirtualXRay-demos)

INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../build/version.cmake)

find_package(gVirtualXRay REQUIRED)

add_subdirectory(STL-File)
add_subdirectory(GUI-FLTK)
add_subdirectory(Iso-Surfaces)
add_subdirectory(Intersections)
add_subdirectory(SimpleBackProjection)
add_subdirectory(AnatomicalData)

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE.txt DESTINATION demos COMPONENT demos)

