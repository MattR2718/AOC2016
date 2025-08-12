cmake_minimum_required(VERSION 3.20)

include(FetchContent)

# Try to find CUDA
find_package(CUDA)

if(CUDA_FOUND)
    project(day5CUDA LANGUAGES CXX CUDA)
    message(STATUS "CUDA found! Enabling CUDA support.")
    enable_language(CUDA)
    set(CMAKE_CUDA_STANDARD 20)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)
    
    add_executable(day5CUDA day5CUDA.cpp day5.cu)

else()
    message(STATUS "CUDA not found. Only building without CUDA.")
endif()


add_executable(day5 day5.cpp)
if(OpenSSL_FOUND)
  target_compile_definitions(day5 PRIVATE HAVE_OPENSSL=1)
  target_link_libraries(day5 PRIVATE ctre OpenSSL::SSL OpenSSL::Crypto)
else()
  target_link_libraries(day5 PRIVATE ctre cryptopp::cryptopp)
endif()