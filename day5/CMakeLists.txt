cmake_minimum_required(VERSION 3.20)


include(FetchContent)




add_executable(day5 day5.cpp)
if(OpenSSL_FOUND)
  target_link_libraries(day5 PRIVATE ctre OpenSSL::SSL OpenSSL::Crypto)
else()
  target_link_libraries(day5 PRIVATE ctre cryptopp::cryptopp)
endif()