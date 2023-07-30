cmake_minimum_required(VERSION 3.25)
project(KeyLayout)

set(CMAKE_CXX_STANDARD 17)
add_link_options(-static)

include_directories(include)
include_directories(vendor-libs/plog/include)
include_directories(vendor-libs/cxxopts)

add_executable(KeyLayout src/main.cpp src/interactive.cpp)
