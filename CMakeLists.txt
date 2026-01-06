cmake_minimum_required(VERSION 3.14.0)

project(Minesweeper VERSION 0.1.0 LANGUAGES CXX)

# export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# find all cpp files in src directory AND subdirectories and store them in SOURCES
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(minesweeper ${SOURCES})

# add src/ as an include directory
target_include_directories(minesweeper PRIVATE ${CMAKE_SOURCE_DIR}/src)

# link curses library (pdcurses for Windows, ncurses for Linux)
if(WIN32)
    # try to find pdcurses first
    find_package(pdcurses QUIET)
    
    if(NOT pdcurses_FOUND)
        # download and build pdcurses if not found
        include(FetchContent)
        FetchContent_Declare(
            pdcurses
            URL https://github.com/wmcbrine/PDCurses/archive/refs/tags/3.10.tar.gz
            SOURCE_SUBDIR wincon
        )
        FetchContent_MakeAvailable(pdcurses)
        target_link_libraries(minesweeper pdcurses)
        target_include_directories(minesweeper PRIVATE ${pdcurses_SOURCE_DIR})
    else()
        target_link_libraries(minesweeper pdcurses::pdcurses_static)
    endif()
else()
    # ncurses for linux
    # libncurses-dev or ncurses-devel or is required to compile;
    find_package(Curses REQUIRED)
    target_link_libraries(minesweeper ${CURSES_LIBRARIES})
    target_include_directories(minesweeper PRIVATE ${CURSES_INCLUDE_DIR})
endif()

set_property(TARGET minesweeper PROPERTY CXX_STANDARD 23)


