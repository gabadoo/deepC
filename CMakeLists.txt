cmake_minimum_required(VERSION 3.10)
project(deepC)
include(BuildType.cmake)

add_subdirectory(src)
target_include_directories(deepC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


