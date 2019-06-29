cmake_minimum_required(VERSION 3.13)
project(delay_transport)

set(CMAKE_CXX_STANDARD 17)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    add_compile_options(-ggdb -O0)
endif()

add_subdirectory(src)

export(TARGETS delay_transport FILE delay-transport-exports.cmake)
