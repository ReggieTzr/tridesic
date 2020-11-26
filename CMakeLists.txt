cmake_minimum_required(VERSION 3.10)

set(CMAKE_VERBOSE_MAKEFILE ON)

# set the project name
project(tridesic)

# == Compile option to fix static/dynamic library linking
add_compile_options(-fPIC)

# == Deps
add_subdirectory(deps/geometry-central)
add_subdirectory(deps/pybind11)

# == Build our project
pybind11_add_module(tridesic src/main.cpp)
target_include_directories(tridesic PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_libraries(tridesic PRIVATE geometry-central)