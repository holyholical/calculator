cmake_minimum_required(VERSION 3.16)

project(calculator C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(calculator
    src/calculator.c
)

target_include_directories(calculator
    PUBLIC include
)

add_executable(calculator_app
    src/main.c
)

target_link_libraries(calculator_app
    calculator
)
