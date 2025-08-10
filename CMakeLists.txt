cmake_minimum_required(VERSION 3.10.0)
project(AOC2016 VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

add_executable(AOC2016 main.cpp)


# Fetch ctre from github
include(FetchContent)
FetchContent_Declare(
        ctre
        GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions.git
        GIT_TAG        master
)
FetchContent_MakeAvailable(ctre)


# Copy inputs folder to ${CMAKE_BINARY_DIR}/inputs
# Copy every time project is built, even if there are no code changes
file(COPY inputs DESTINATION ${CMAKE_BINARY_DIR}/inputs)

add_subdirectory(day1)