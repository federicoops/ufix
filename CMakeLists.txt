cmake_minimum_required(VERSION 3.25)  # Required for C++26 support

project(MyProject
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "C++26 project using GCC 15"
)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Werror=return-type
    -Wdeprecated-variadic-comma-omission
    -Wheader-guard
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory("./src")