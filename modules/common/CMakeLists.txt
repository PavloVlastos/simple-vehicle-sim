cmake_minimum_required(VERSION 3.13.4)

add_library(common common.c)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
