cmake_minimum_required(VERSION 3.30)

project(ufix
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "micro fix parser"
)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
enable_testing()
add_compile_options(
    -Wall
    -Wextra
    -Wextra
    -Wpedantic
    -Werror=return-type
    -Wdeprecated-variadic-comma-omission
    -Wheader-guard
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories("./third_party")
add_subdirectory("./src")