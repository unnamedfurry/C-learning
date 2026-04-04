cmake_minimum_required(VERSION 4.1)
project(c-learning C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

find_package(Curses REQUIRED)
find_package(raylib 5.0 REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(main main.c)
add_executable(games games.c)

target_link_libraries(main ${CURSES_LIBRARIES})
target_include_directories(games PRIVATE ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(games PRIVATE raylib GL m pthread dl rt X11)
target_compile_options(games PRIVATE -Wall -Wextra -Wpedantic)
