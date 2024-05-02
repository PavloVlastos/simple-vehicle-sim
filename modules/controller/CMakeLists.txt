cmake_minimum_required(VERSION 3.22.1)

add_library(controller controller.c)

target_link_libraries(controller PUBLIC lin_alg)

target_include_directories(controller PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
