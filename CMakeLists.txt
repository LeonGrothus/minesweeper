cmake_minimum_required(VERSION 3.10.0)

project(Minesweeper VERSION 0.1.0 LANGUAGES CXX)

add_executable(minesweeper src/main.cpp)

set_property(TARGET minesweeper PROPERTY CXX_STANDARD 23)
