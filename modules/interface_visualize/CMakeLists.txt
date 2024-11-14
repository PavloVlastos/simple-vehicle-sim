cmake_minimum_required(VERSION 3.13.4)

add_library(interface_visualize interface_visualize.c)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/common)

target_link_libraries(interface_visualize PUBLIC common)
target_link_libraries(interface_visualize PUBLIC map)

target_include_directories(interface_visualize PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
