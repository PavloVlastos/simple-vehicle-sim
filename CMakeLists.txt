cmake_minimum_required(VERSION 3.13.4)
project(svs_sim VERSION 0.1.0 LANGUAGES C)

set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/common)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/controller)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/interface)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/lin_alg)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/model)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/controller)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/interface)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/lin_alg)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/model)

set(sources
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/common/common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/controller/controller.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/interface/interface.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/lin_alg/lin_alg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/model/model.c
)

add_compile_options(-Wall -Wextra)

add_executable(svs_sim main.c ${sources})
add_executable(example_secondary secondary.c ${CMAKE_CURRENT_SOURCE_DIR}/modules/lin_alg/lin_alg.c)

target_link_libraries(svs_sim m)
target_link_libraries(example_secondary m)