cmake_minimum_required(VERSION 3.13.4)

add_library(map map.c)

target_link_libraries(map PUBLIC common)

target_include_directories(map PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
