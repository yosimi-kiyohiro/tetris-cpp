cmake_minimum_required(VERSION 3.20)
project(tetris-cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib CONFIG REQUIRED)

add_executable(tetris src/main.cpp)
target_link_libraries(tetris PRIVATE raylib)
