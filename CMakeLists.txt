cmake_minimum_required(VERSION 3.10.0)

project(Minesweeper VERSION 0.1.0 LANGUAGES CXX)

# export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# find ncurses library
find_package(Curses REQUIRED)

# find all .cpp files in src/ directory and subdirectories and store them in SOURCES
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(minesweeper ${SOURCES})

# add src/ as an include directory
target_include_directories(minesweeper PRIVATE ${CMAKE_SOURCE_DIR}/src)

# link ncurses library
target_link_libraries(minesweeper PRIVATE ${CURSES_LIBRARIES})

set_property(TARGET minesweeper PROPERTY CXX_STANDARD 23)
