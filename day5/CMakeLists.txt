cmake_minimum_required(VERSION 3.20)


include(FetchContent)

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


add_executable(day5 day5.cpp)
target_link_libraries(day5 PRIVATE ctre mdspan cryptopp::cryptopp OpenSSL::SSL OpenSSL::Crypto)