cmake_minimum_required(VERSION 3.13.4)

add_library(interface_stress interface_stress.c)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/common)

target_link_libraries(interface_stress PUBLIC common)
target_link_libraries(interface_stress PUBLIC map)

target_include_directories(interface_stress PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
