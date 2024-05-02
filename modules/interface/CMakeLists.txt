cmake_minimum_required(VERSION 3.22.1)

add_library(interface interface.c)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/common)

target_link_libraries(interface PUBLIC common)

target_include_directories(interface PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
