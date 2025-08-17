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
        GIT_TAG        origin/main
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



# Try to find OpenSSL first
find_package(OpenSSL QUIET)

if(OpenSSL_FOUND)
    message(STATUS "Found OpenSSL: ${OPENSSL_VERSION}")
    target_compile_definitions(AOC2016 PRIVATE HAVE_OPENSSL=1)
else()
    message(WARNING "OpenSSL not found — falling back to Crypto++")

    FetchContent_Declare(
        cryptopp_cmake_modern
        GIT_REPOSITORY https://github.com/abdes/cryptopp-cmake.git
        GIT_TAG        master  # Or specific release tag
    )

    # Prevent building tests and extras
    set(CRYPTOPP_BUILD_TESTING OFF CACHE BOOL "Disable Crypto++ tests" FORCE)
    set(CRYPTOPP_BUILD_EXAMPLES OFF CACHE BOOL "Disable Crypto++ examples" FORCE)
    set(CRYPTOPP_BUILD_DOCUMENTATION OFF CACHE BOOL "Disable Crypto++ docs" FORCE)

    FetchContent_MakeAvailable(cryptopp_cmake_modern)
endif()

# Get Boost headers from GitHub
FetchContent_Declare(
  boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_TAG boost-1.86.0  # choose a specific version
)

# Only bring in the bimap and dependencies (header-only parts)
set(BOOST_INCLUDE_LIBRARIES bimap config core detail mpl preprocessor static_assert type_traits)
set(BOOST_ENABLE_CMAKE ON)

FetchContent_MakeAvailable(boost)

include_directories(${PROJECT_SOURCE_DIR}/common)

add_subdirectory(day1)
add_subdirectory(day2)
add_subdirectory(day3)
add_subdirectory(day4)
add_subdirectory(day5)
add_subdirectory(day6)
add_subdirectory(day7)
add_subdirectory(day8)
add_subdirectory(day9)
add_subdirectory(day10)
add_subdirectory(day11)
add_subdirectory(day12)
add_subdirectory(day13)
add_subdirectory(day14)