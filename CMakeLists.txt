cmake_minimum_required(VERSION 3.10)
project(deepC)
include(BuildType.cmake)

add_library(deepC src/sdc_controller.c)
target_include_directories(deepC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


