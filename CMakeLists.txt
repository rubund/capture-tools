cmake_minimum_required (VERSION 2.6)

SET(CMAKE_BUILD_TYPE Release CACHE STRING "If build type is not specified on command line, build Release")

project (capture-tools)

add_subdirectory(src)
add_subdirectory(python)
