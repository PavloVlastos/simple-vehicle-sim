cmake_minimum_required(VERSION 3.22.1)

add_library(common common.c)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
