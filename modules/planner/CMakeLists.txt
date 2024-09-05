cmake_minimum_required(VERSION 3.13.4)

add_library(planner planner.c)

target_link_libraries(planner PUBLIC common)
target_link_libraries(planner PUBLIC map)

target_include_directories(planner PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
