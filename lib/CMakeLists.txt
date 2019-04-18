cmake_minimum_required(VERSION 3.10)

add_library(Remote remoteApi/extApi.c remoteApi/extApiPlatform.c)

target_include_directories(Remote PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

