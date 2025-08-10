cmake_minimum_required(VERSION 3.20.0)
project(AOC2016 VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(AOC2016 main.cpp)


# Fetch ctre from github
include(FetchContent)
FetchContent_Declare(
        ctre
        GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions.git
        GIT_TAG        master
)
FetchContent_MakeAvailable(ctre)


#
#
#
# Check for / fetch mdspan
include(CheckIncludeFileCXX)

check_include_file_cxx("mdspan" HAVE_STD_MDSPAN)
check_include_file_cxx("experimental/mdspan" HAVE_EXPERIMENTAL_MDSPAN)

if(HAVE_STD_MDSPAN)
    message(STATUS "Using system <mdspan>")
    target_compile_definitions(AOC2016 PRIVATE HAVE_STD_MDSPAN=1)
elseif(HAVE_EXPERIMENTAL_MDSPAN)
    message(STATUS "Using system <experimental/mdspan>")
    target_compile_definitions(AOC2016 PRIVATE HAVE_EXPERIMENTAL_MDSPAN=1)
else()
    message(STATUS "<mdspan> not found — fetching Kokkos mdspan")
    include(FetchContent)
    FetchContent_Declare(
        mdspan
        GIT_REPOSITORY https://github.com/kokkos/mdspan.git
        GIT_TAG stable
    )
    FetchContent_MakeAvailable(mdspan)
    target_include_directories(AOC2016 PRIVATE ${mdspan_SOURCE_DIR}/include)
    target_compile_definitions(AOC2016 PRIVATE HAVE_KOKKOS_MDSPAN=1)
endif()






# Copy inputs folder to ${CMAKE_BINARY_DIR}/inputs
# Copy every time project is built, even if there are no code changes
file(COPY inputs DESTINATION ${CMAKE_BINARY_DIR}/inputs)

add_subdirectory(day1)
add_subdirectory(day2)
add_subdirectory(day3)
add_subdirectory(day4)
add_subdirectory(day5)