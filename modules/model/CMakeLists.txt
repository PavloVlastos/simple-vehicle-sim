cmake_minimum_required(VERSION 3.22.1)

add_library(model model.c)

target_link_libraries(model PUBLIC common)
target_link_libraries(model PUBLIC lin_alg)

target_include_directories(model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
