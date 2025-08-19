cmake_minimum_required(VERSION 3.20)

add_executable(day17 day17.cpp)
if(OpenSSL_FOUND)
  target_compile_definitions(day17 PRIVATE HAVE_OPENSSL=1)
  target_link_libraries(day17 PRIVATE ctre OpenSSL::SSL OpenSSL::Crypto)
else()
  target_link_libraries(day17 PRIVATE ctre cryptopp::cryptopp)
endif()