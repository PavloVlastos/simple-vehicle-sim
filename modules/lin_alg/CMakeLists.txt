cmake_minimum_required(VERSION 3.13.4)

add_library(lin_alg lin_alg.c)

target_link_libraries(lin_alg PUBLIC common)

target_include_directories(lin_alg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
