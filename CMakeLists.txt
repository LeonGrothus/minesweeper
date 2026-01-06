cmake_minimum_required(VERSION 3.14.0)

# Added C to LANGUAGES so CMake can compile .c files
project(Minesweeper VERSION 0.1.0 LANGUAGES C CXX)

# export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# use c++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find all cpp files in src directory AND subdirectories and store them in SOURCES
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(minesweeper ${SOURCES})

# add src/ as an include directory
target_include_directories(minesweeper PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_custom_command(TARGET minesweeper POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:minesweeper>/assets"
        COMMENT "Copy Assets into build folder..."
)

# link curses library (pdcurses for Windows, ncurses for Linux)
if (WIN32)
    set_target_properties(minesweeper PROPERTIES
            VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )

    # prevent stupid win macros from braking everything (why do they exist, it makes no sense)
    target_compile_definitions(minesweeper PRIVATE NOMINMAX)

    # try to find pdcurses first
    find_package(pdcurses QUIET)

    if (NOT pdcurses_FOUND)
        # download pdcurses
        include(FetchContent)
        FetchContent_Declare(
                pdcurses
                URL https://github.com/wmcbrine/PDCurses/archive/refs/tags/3.9.zip
                DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        )

        # manually define target since pdcurses does not have cmakelists
        FetchContent_GetProperties(pdcurses)
        if (NOT pdcurses_POPULATED)
            FetchContent_Populate(pdcurses)

            # windows console stuff
            file(GLOB PDCURSES_SOURCES
                    "${pdcurses_SOURCE_DIR}/pdcurses/*.c"
                    "${pdcurses_SOURCE_DIR}/wincon/*.c"
            )

            add_library(pdcurses STATIC ${PDCURSES_SOURCES})

            # essential to handle uft16 and colors
            target_compile_definitions(pdcurses PRIVATE PDC_WIDE)

            target_include_directories(pdcurses PUBLIC "${pdcurses_SOURCE_DIR}")

            target_link_libraries(pdcurses PRIVATE user32 advapi32)
        endif ()

        target_link_libraries(minesweeper PRIVATE pdcurses)
    else ()
        target_link_libraries(minesweeper PRIVATE pdcurses::pdcurses_static)
    endif ()

    # sets minesweeper as default startup project in visual studio
    set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT minesweeper)
else ()
    # ncurses for linux
    # libncurses-dev or ncurses-devel or is required to compile;
    find_package(Curses REQUIRED)
    target_link_libraries(minesweeper ${CURSES_LIBRARIES})
    target_include_directories(minesweeper PRIVATE ${CURSES_INCLUDE_DIR})
endif ()

set_property(TARGET minesweeper PROPERTY CXX_STANDARD 23)