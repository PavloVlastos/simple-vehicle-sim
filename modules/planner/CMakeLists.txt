cmake_minimum_required(VERSION 3.13.4)

add_library(plan plan.cpp)

target_link_libraries(plan PUBLIC common)

target_include_directories(plan PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
