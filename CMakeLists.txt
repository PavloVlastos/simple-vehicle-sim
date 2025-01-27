cmake_minimum_required(VERSION 3.13.4)
project(SVS VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/common)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/controller)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/interface_visualize)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/interface_stress)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/lin_alg)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/model)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/planner)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/map)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/parse_args)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/controller)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/interface_visualize)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/interface_stress)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/lin_alg)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/model)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/planner)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/map)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/parse_args)

set(sources
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/common/common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/controller/controller.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/interface_visualize/interface_visualize.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/interface_stress/interface_stress.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/lin_alg/lin_alg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/model/model.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/planner/planner.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/map/map.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/parse_args/parse_args.c
)

add_compile_options(-Wall -Wextra)

add_executable(svs main.c ${sources})

target_link_libraries(svs m)