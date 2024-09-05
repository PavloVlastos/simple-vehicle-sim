cmake_minimum_required(VERSION 3.13.4)

add_library(parse_args parse_args.c)

target_link_libraries(parse_args PUBLIC common)

target_include_directories(parse_args PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
