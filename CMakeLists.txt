cmake_minimum_required(VERSION 3.25)
project(KeyLayout)

set(CMAKE_CXX_STANDARD 17)

add_link_options(--static)

add_executable(KeyLayout main.cpp)
