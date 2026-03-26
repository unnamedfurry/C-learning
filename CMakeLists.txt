cmake_minimum_required(VERSION 4.1)
project(c-learning C)

set(CMAKE_C_STANDARD 11)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(main main.c)

target_link_libraries(main ${CURSES_LIBRARIES})
