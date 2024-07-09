cmake_minimum_required(VERSION 3.13.4)

add_library(path_planner path_planner.c)

target_link_libraries(path_planner PUBLIC common)
target_link_libraries(path_planner PUBLIC lin_alg)

target_include_directories(path_planner PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
